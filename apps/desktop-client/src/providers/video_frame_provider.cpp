#include "video_frame_provider.hpp"

namespace vap
{

VideoFrameProvider::VideoFrameProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{

}

void VideoFrameProvider::setImage(const QString& cameraId, const QImage& image)
{
    QMutexLocker locker(&m_mutex);
    m_images[cameraId] = image;
}

QImage VideoFrameProvider::requestImage(
    const QString& id,
    QSize* size,
    const QSize& requestedSize)
{
    QMutexLocker locker(&m_mutex);
    QString cameraId = id.section('?', 0, 0);

    auto it = m_images.find(cameraId);
    if(it == m_images.end())
        return {};

    const QImage& image = it->second;

    if (image.isNull())
        return {};

    if (size)
        *size = image.size();

    if (requestedSize.isValid())
    {
        return image.scaled(
            requestedSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
    }
    return image;
}

}
