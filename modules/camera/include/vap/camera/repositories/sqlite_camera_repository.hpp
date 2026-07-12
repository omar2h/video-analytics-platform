#pragma once

#include <QSqlQuery>

#include <vap/camera/repositories/i_camera_repository.hpp>

namespace vap
{

class Database;
class SQLiteCameraRepository final : public ICameraRepository
{
public:
    explicit SQLiteCameraRepository(Database& database);

    QList<Camera> cameras() const override;

    void addCamera(const Camera& camera) override;

    bool updateCamera(const Camera& camera) override;

    void removeCamera(const CameraId& id) override;

private:
    Camera mapCamera(const QSqlQuery& query) const;

    Database& m_database;
};

}
