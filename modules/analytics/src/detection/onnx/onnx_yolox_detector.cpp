#include <vap/analytics/detection/onnx/onnx_yolox_detector.hpp>
#include "yolox_postprocessing.hpp"
#include "yolox_model_spec.hpp"

#include <onnxruntime_cxx_api.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <string>
#include <span>
#include <utility>
#include <array>
#include <vector>
#include <string_view>
#include <algorithm>

namespace vap
{

namespace
{

constexpr auto CocoClassNames = std::to_array<std::string_view>({
    "person",
    "bicycle",
    "car",
    "motorcycle",
    "airplane",
    "bus",
    "train",
    "truck",
    "boat",
    "traffic light",
    "fire hydrant",
    "stop sign",
    "parking meter",
    "bench",
    "bird",
    "cat",
    "dog",
    "horse",
    "sheep",
    "cow",
    "elephant",
    "bear",
    "zebra",
    "giraffe",
    "backpack",
    "umbrella",
    "handbag",
    "tie",
    "suitcase",
    "frisbee",
    "skis",
    "snowboard",
    "sports ball",
    "kite",
    "baseball bat",
    "baseball glove",
    "skateboard",
    "surfboard",
    "tennis racket",
    "bottle",
    "wine glass",
    "cup",
    "fork",
    "knife",
    "spoon",
    "bowl",
    "banana",
    "apple",
    "sandwich",
    "orange",
    "broccoli",
    "carrot",
    "hot dog",
    "pizza",
    "donut",
    "cake",
    "chair",
    "couch",
    "potted plant",
    "bed",
    "dining table",
    "toilet",
    "tv",
    "laptop",
    "mouse",
    "remote",
    "keyboard",
    "cell phone",
    "microwave",
    "oven",
    "toaster",
    "sink",
    "refrigerator",
    "book",
    "clock",
    "vase",
    "scissors",
    "teddy bear",
    "hair drier",
    "toothbrush"
});

static_assert(
    CocoClassNames.size() == detail::ClassCount,
    "The class list must contain exactly 80 labels.");

} // namespace

class OnnxYoloXDetector::Impl
{

private:
    struct PreparedImage
    {
        std::vector<float> pixels;
        double scale{};
    };

public:
    explicit Impl(const OnnxYoloXConfiguration& configuration)
        : m_configuration(configuration),
          m_environment(
              ORT_LOGGING_LEVEL_WARNING,
              "vap.analytics")
    {
        validateConfiguration();

        Ort::SessionOptions options;
        options.SetIntraOpNumThreads(
            m_configuration.intraOpThreads);

        m_session = Ort::Session{
            m_environment,
            m_configuration.modelPath.c_str(),
            options
        };
	validateModel();

        // Copy the names into strings owned by this implementation.
        Ort::AllocatorWithDefaultOptions allocator;

        const auto inputName =
            m_session.GetInputNameAllocated(0, allocator);

        const auto outputName =
            m_session.GetOutputNameAllocated(0, allocator);

        m_inputName = inputName.get();
        m_outputName = outputName.get();
    }

[[nodiscard]]
std::vector<Detection> detect(const cv::Mat& image)
{
    auto prepared = prepareImage(image);

    auto outputs = runInference(prepared);

    if (outputs.size() != 1 || !outputs[0].IsTensor())
    {
        throw std::runtime_error(
            "Expected exactly one output tensor.");
    }

    const auto& output = outputs[0];
    const auto outputInfo = output.GetTensorTypeAndShapeInfo();

    const std::vector<std::int64_t> expectedShape{
        1, 3549, 85
    };

    if (outputInfo.GetElementType() !=
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT ||
        outputInfo.GetShape() != expectedShape)
    {
        throw std::runtime_error(
            "Unexpected YOLOX inference output type or shape.");
    }

    const std::span<const float> predictions{
        output.GetTensorData<float>(),
        outputInfo.GetElementCount()
    };

    auto candidates = detail::decodeDetections(
        predictions,
        prepared.scale,
        m_configuration.scoreThreshold
    );

    return detail::applyNms(
        std::move(candidates),
        m_configuration.nmsThreshold
    );
}

private:
    void validateConfiguration() const
    {
        if (m_configuration.modelPath.empty() ||
            !std::filesystem::is_regular_file(
                m_configuration.modelPath))
        {
            throw std::invalid_argument(
                "Model path must point to an existing ONNX file.");
        }

        if (!std::isfinite(m_configuration.scoreThreshold) ||
            m_configuration.scoreThreshold < 0.0F ||
            m_configuration.scoreThreshold > 1.0F)
        {
            throw std::invalid_argument(
                "Score threshold must be between 0 and 1.");
        }

        if (!std::isfinite(m_configuration.nmsThreshold) ||
            m_configuration.nmsThreshold < 0.0 ||
            m_configuration.nmsThreshold > 1.0)
        {
            throw std::invalid_argument(
                "NMS threshold must be between 0 and 1.");
        }

        if (m_configuration.intraOpThreads < 1)
        {
            throw std::invalid_argument(
                "Intra-op thread count must be positive.");
        }
    }

	void validateModel() const
{
    if (m_session.GetInputCount() != 1 ||
        m_session.GetOutputCount() != 1)
    {
        throw std::runtime_error(
            "YOLOX detector expects exactly one input and one output.");
    }

    const auto inputType = m_session.GetInputTypeInfo(0);
    const auto outputType = m_session.GetOutputTypeInfo(0);

    if (inputType.GetONNXType() != ONNX_TYPE_TENSOR ||
        outputType.GetONNXType() != ONNX_TYPE_TENSOR)
    {
        throw std::runtime_error(
            "YOLOX detector expects tensor input and output.");
    }

    const auto inputInfo =
        inputType.GetTensorTypeAndShapeInfo();

    const auto outputInfo =
        outputType.GetTensorTypeAndShapeInfo();

    if (inputInfo.GetElementType() !=
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT ||
        outputInfo.GetElementType() !=
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT)
    {
        throw std::runtime_error(
            "YOLOX detector expects float32 input and output.");
    }

    const std::vector<std::int64_t> expectedInputShape{
        1, 3, detail::InputHeight, detail::InputWidth
    };

    const std::vector<std::int64_t> expectedOutputShape{
        1, 3549, 85
    };

    if (inputInfo.GetShape() != expectedInputShape)
    {
        throw std::runtime_error(
            "YOLOX detector expects input shape [1, 3, 416, 416].");
    }

    if (outputInfo.GetShape() != expectedOutputShape)
    {
        throw std::runtime_error(
            "YOLOX detector expects output shape [1, 3549, 85].");
    }
}

[[nodiscard]]
PreparedImage prepareImage(const cv::Mat& image) const
{
    if (image.empty() || image.type() != CV_8UC3)
    {
        throw std::invalid_argument(
            "Detector input must be a nonempty CV_8UC3 BGR image.");
    }

    PreparedImage prepared;

    prepared.scale = std::min(
        static_cast<double>(detail::InputWidth) / image.cols,
        static_cast<double>(detail::InputHeight) / image.rows
    );

    const int resizedWidth =
        static_cast<int>(image.cols * prepared.scale);

    const int resizedHeight =
        static_cast<int>(image.rows * prepared.scale);

    if (resizedWidth < 1 || resizedHeight < 1)
    {
        throw std::invalid_argument(
            "Image aspect ratio is too extreme for this model.");
    }

    cv::Mat resizedImage;

    cv::resize(
        image,
        resizedImage,
        cv::Size{resizedWidth, resizedHeight},
        0.0,
        0.0,
        cv::INTER_LINEAR
    );

    cv::Mat paddedImage(
        detail::InputHeight,
        detail::InputWidth,
        CV_8UC3,
        cv::Scalar{114, 114, 114}
    );

    const cv::Rect imageRegion{
        0, 0, resizedWidth, resizedHeight
    };

    resizedImage.copyTo(paddedImage(imageRegion));

    constexpr std::size_t pixelsPerChannel =
        static_cast<std::size_t>(detail::InputWidth) * detail::InputHeight;

    prepared.pixels.resize(3 * pixelsPerChannel);

    for (int y = 0; y < detail::InputHeight; ++y)
    {
        const auto* row = paddedImage.ptr<cv::Vec3b>(y);

        for (int x = 0; x < detail::InputWidth; ++x)
        {
            const std::size_t index =
                static_cast<std::size_t>(y) * detail::InputWidth + x;

            prepared.pixels[index] =
                static_cast<float>(row[x][0]);

            prepared.pixels[pixelsPerChannel + index] =
                static_cast<float>(row[x][1]);

            prepared.pixels[2 * pixelsPerChannel + index] =
                static_cast<float>(row[x][2]);
        }
    }

    return prepared;
}

[[nodiscard]]
std::vector<Ort::Value> runInference(PreparedImage& prepared)
{
    constexpr std::size_t expectedPixelCount =
        3 * static_cast<std::size_t>(detail::InputWidth) * detail::InputHeight;

    if (prepared.pixels.size() != expectedPixelCount)
    {
        throw std::invalid_argument(
            "Prepared image has an unexpected number of values.");
    }

    const std::array<std::int64_t, 4> inputShape{
        1, 3, detail::InputHeight, detail::InputWidth
    };

    const auto memoryInfo = Ort::MemoryInfo::CreateCpu(
        OrtArenaAllocator,
        OrtMemTypeDefault
    );

    // Borrows prepared.pixels; no additional pixel copy here.
    auto inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo,
        prepared.pixels.data(),
        prepared.pixels.size(),
        inputShape.data(),
        inputShape.size()
    );

    const std::array<const char*, 1> inputNames{
        m_inputName.c_str()
    };

    const std::array<const char*, 1> outputNames{
        m_outputName.c_str()
    };

    return m_session.Run(
        Ort::RunOptions{nullptr},
        inputNames.data(),
        &inputTensor,
        inputNames.size(),
        outputNames.data(),
        outputNames.size()
    );
}

private:
    OnnxYoloXConfiguration m_configuration;

    // Member order matters: session must be destroyed first.
    Ort::Env m_environment;
    Ort::Session m_session{nullptr};

    std::string m_inputName;
    std::string m_outputName;
};

OnnxYoloXDetector::OnnxYoloXDetector(
    const OnnxYoloXConfiguration& configuration)
    : m_impl(std::make_unique<Impl>(configuration))
{
}

OnnxYoloXDetector::~OnnxYoloXDetector() = default;

std::vector<Detection> OnnxYoloXDetector::detect(
    const cv::Mat& image)
{
    return m_impl->detect(image);
}

std::string_view OnnxYoloXDetector::className(
    int classId) const noexcept
{
    if (classId < 0 ||
        static_cast<std::size_t>(classId) >= CocoClassNames.size())
    {
        return "unknown";
    }

    return CocoClassNames[static_cast<std::size_t>(classId)];
}

} // namespace vap
