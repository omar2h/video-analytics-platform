#include "video_frame_provider.hpp"

namespace vap
{

VideoFrameProvider::VideoFrameProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

void VideoFrameProvider::setImage(const QImage& image)
{
    QMutexLocker locker(&m_mutex);
    m_image = image;
}

QImage VideoFrameProvider::requestImage(
    const QString& id,
    QSize* size,
    const QSize& requestedSize)
{
    Q_UNUSED(id);
    QMutexLocker locker(&m_mutex);

    if (m_image.isNull())
        return {};

    if (size)
        *size = m_image.size();

    if (requestedSize.isValid())
    {
        return m_image.scaled(
            requestedSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
    }

    return m_image;
}

}
