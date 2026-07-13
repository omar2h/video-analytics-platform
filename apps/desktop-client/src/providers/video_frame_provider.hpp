#pragma once

#include <QMutex>
#include <QQuickImageProvider>

namespace vap
{

class VideoFrameProvider : public QQuickImageProvider
{
public:
    VideoFrameProvider();

    void setImage(const QImage& image);

    QImage requestImage(const QString& id,
                        QSize* size,
                        const QSize& requestedSize) override;

private:
    mutable QMutex m_mutex;
    QImage m_image;
};

}
