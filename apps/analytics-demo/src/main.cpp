#include <vap/analytics/detection/onnx/onnx_yolox_detector.hpp>

#include <opencv2/imgcodecs.hpp>

#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <model.onnx> <image-path>\n";
        return 1;
    }

    try
    {
        const cv::Mat image =
            cv::imread(argv[2], cv::IMREAD_COLOR);

        if (image.empty())
        {
            throw std::runtime_error(
                "Failed to load the input image.");
        }

        vap::OnnxYoloXConfiguration configuration;
        configuration.modelPath = argv[1];

        std::unique_ptr<vap::IObjectDetector> detector =
            std::make_unique<vap::OnnxYoloXDetector>(
                configuration);

        const auto detections = detector->detect(image);

        std::cout << "Detections after NMS: "
                  << detections.size() << '\n';

        for (const auto& detection : detections)
        {
            std::cout
                << detector->className(detection.classId)
                << "  score="
                << std::fixed << std::setprecision(3)
                << detection.score
                << "  box=("
                << detection.box.x << ", "
                << detection.box.y << ", "
                << detection.box.width << ", "
                << detection.box.height << ")\n";
        }

        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Detection failed: "
                  << error.what() << '\n';
        return 1;
    }
}
