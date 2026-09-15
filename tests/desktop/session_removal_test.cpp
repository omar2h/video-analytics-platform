#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QEvent>
#include <QImage>
#include <QPointer>

#include <vap/streaming/manager/streaming_manager.hpp>
#include <vap/streaming/session/streaming_session.hpp>

#include "src/providers/video_frame_provider.hpp"
#include "src/viewmodels/camera_stream_view_model.hpp"
#include "src/viewmodels/live_monitoring_view_model.hpp"

TEST(SessionRemovalTest, RetiresViewModelAndClearsImage)
{
    int argc = 1;
    char applicationName[] = "session_removal_test";
    char* argv[] = {applicationName, nullptr};
    QCoreApplication app(argc, argv);

    vap::StreamingManager manager;
    vap::VideoFrameProvider provider;
    vap::LiveMonitoringViewModel liveViewModel(
        &manager, &provider);

    const QString cameraId = QStringLiteral("test-camera");
    manager.createSession(cameraId);

    QPointer<vap::CameraStreamViewModel> viewModel =
        liveViewModel.streamViewModel(cameraId);

    ASSERT_FALSE(viewModel.isNull());

    // Exercise the real view-model-to-provider connection.
    QImage frame(16, 16, QImage::Format_RGB32);
    frame.fill(Qt::red);
    viewModel->setCurrentFrame(frame);

    QSize imageSize;
    ASSERT_FALSE(
        provider.requestImage(cameraId, &imageSize, {}).isNull());

    manager.removeSession(cameraId);

    EXPECT_FALSE(manager.contains(cameraId));
    EXPECT_EQ(liveViewModel.streamViewModel(cameraId), nullptr);

    EXPECT_TRUE(
        provider.requestImage(cameraId, &imageSize, {}).isNull());

    // Deferred deletion has not run yet. The retired object
    // must already be safe for remaining UI accesses.
    ASSERT_FALSE(viewModel.isNull());

    EXPECT_EQ(
        viewModel->state(),
        static_cast<int>(vap::ConnectionState::Disconnected));

    EXPECT_TRUE(viewModel->currentFrame().isNull());
    EXPECT_FALSE(viewModel->hasVideo());
    EXPECT_FALSE(viewModel->recording());
    EXPECT_FALSE(viewModel->recordingActionEnabled());
    EXPECT_TRUE(viewModel->codec().isEmpty());
    EXPECT_TRUE(viewModel->resolution().isEmpty());
    EXPECT_DOUBLE_EQ(viewModel->fps(), 0.0);
    EXPECT_DOUBLE_EQ(viewModel->bitrateMbps(), 0.0);
    EXPECT_EQ(viewModel->framesDecoded(), 0u);
    EXPECT_EQ(viewModel->packetsReceived(), 0u);

    // These must safely do nothing without a session.
    viewModel->startRecording();
    viewModel->stopRecording();

    QCoreApplication::sendPostedEvents(
        nullptr, QEvent::DeferredDelete);

    EXPECT_TRUE(viewModel.isNull());
}
