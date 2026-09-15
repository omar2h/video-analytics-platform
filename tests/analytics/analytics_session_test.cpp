#include <gtest/gtest.h>

#include <vap/analytics/analytics_session.hpp>

#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>

#include <chrono>
#include <memory>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

namespace
{

class FakeDetector final : public vap::IObjectDetector
{
public:
    std::vector<vap::Detection> detect(
        const cv::Mat&) override
    {
        return {
            {{1.0, 2.0, 3.0, 4.0}, 2, 0.90f}
        };
    }

    std::string_view className(int) const noexcept override
    {
        return "car";
    }
};

} // namespace

TEST(AnalyticsSessionTest, ReturnsResultForSubmittedFrame)
{
    int argc = 1;
    char applicationName[] = "analytics_session_test";
    char* argv[] = {applicationName, nullptr};
    QCoreApplication app(argc, argv);

    vap::AnalyticsSession session(
        []
        {
            return std::make_unique<FakeDetector>();
        });

    QEventLoop loop;
    QTimer timeout;
    timeout.setSingleShot(true);

    std::optional<vap::AnalyticsResult> received;
    QString failure;
    bool timedOut = false;

    QObject::connect(
        &session,
        &vap::AnalyticsSession::resultAvailable,
        &loop,
        [&]
        {
            received = session.takeLatestResult();

            if (received)
                loop.quit();
        });

    QObject::connect(
        &session,
        &vap::AnalyticsSession::errorOccurred,
        &loop,
        [&](const QString& message)
        {
            failure = message;
            loop.quit();
        });

    QObject::connect(
        &timeout,
        &QTimer::timeout,
        &loop,
        [&]
        {
            timedOut = true;
            loop.quit();
        });

    const auto generation = session.enable(7);

    vap::AnalyticsFrame frame;
    frame.identity = {7, generation, 42};
    frame.decodeTime = std::chrono::steady_clock::now();
    frame.image = QImage(16, 16, QImage::Format_RGB888);
    frame.image.fill(Qt::red);

    const auto expectedIdentity = frame.identity;
    const auto expectedTime = frame.decodeTime;

    ASSERT_TRUE(session.submitFrame(std::move(frame)));

    timeout.start(5000);
    loop.exec();
    timeout.stop();

    ASSERT_FALSE(timedOut);
    ASSERT_TRUE(failure.isEmpty()) << failure.toStdString();
    ASSERT_TRUE(received.has_value());

    EXPECT_EQ(received->identity, expectedIdentity);
    EXPECT_EQ(received->decodeTime, expectedTime);
    EXPECT_EQ(received->imageSize, QSize(16, 16));

    ASSERT_EQ(received->detections.size(), 1u);

    const auto& detection = received->detections.front();

    EXPECT_EQ(detection.classId, 2);
    EXPECT_FLOAT_EQ(detection.score, 0.90f);
    EXPECT_DOUBLE_EQ(detection.box.x, 1.0);
    EXPECT_DOUBLE_EQ(detection.box.y, 2.0);
    EXPECT_DOUBLE_EQ(detection.box.width, 3.0);
    EXPECT_DOUBLE_EQ(detection.box.height, 4.0);

    EXPECT_FALSE(session.takeLatestResult().has_value());

    session.disable();
    EXPECT_FALSE(session.isEnabled());
}
