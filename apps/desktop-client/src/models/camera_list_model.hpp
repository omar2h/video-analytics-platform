#pragma once

#include <QAbstractListModel>

#include <vap/camera/camera.hpp>

namespace vap
{

class CameraListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        NameRole,
        UrlRole
    };

    explicit CameraListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setCameras(const QList<Camera>& cameras);

private:
    QList<Camera> m_cameras;
};

}
