#include "camera_list_model.hpp"

CameraListModel::CameraListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    m_cameras =
        {
            {
                "lobby",
                "Lobby Camera",
                {"rtsp://lobby"}
            },

            {
                "parking",
                "Parking Camera",
                {"rtsp://parking"}
            },

            {
                "warehouse",
                "Warehouse Camera",
                {"rtsp://warehouse"}
            }
        };
}

int CameraListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_cameras.size();
}

QHash<int, QByteArray> CameraListModel::roleNames() const
{
    return
        {
            {IdRole, "id"},
            {NameRole, "name"},
            {UrlRole, "url"}
        };
}

QVariant CameraListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    const auto& camera = m_cameras.at(index.row());

    switch(role)
    {
    case IdRole:
        return camera.id;

    case NameRole:
        return camera.name;

    case UrlRole:
        return camera.config.url;

    default:
        return {};
    }
}
