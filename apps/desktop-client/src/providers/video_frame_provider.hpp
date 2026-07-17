#pragma once

#include <map>
#include <QMutex>
#include <QQuickImageProvider>

namespace vap
{

class VideoFrameProvider : public QQuickImageProvider
{
public:
    VideoFrameProvider();

    void setImage(const QString& cameraId, const QImage& image);

    QImage requestImage(const QString& id,
                        QSize* size,
                        const QSize& requestedSize) override;

private:
    mutable QMutex m_mutex;
    std::map<QString, QImage> m_images;
};

}
