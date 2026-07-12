#include <vap/camera/repositories/sqlite_camera_repository.hpp>

#include <QSqlError>

#include <vap/database/database.hpp>

namespace vap
{

SQLiteCameraRepository::SQLiteCameraRepository(Database& database)
    : m_database(database)
{}

QList<Camera> SQLiteCameraRepository::cameras() const
{
    QList<Camera> cameras;

    QSqlQuery query(m_database.connection());

    query.prepare(R"(
        SELECT
            id,
            name,
            url
        FROM cameras
    )");

    if(!query.exec())
    {
        qWarning() << query.lastError();
        return cameras;
    }

    while(query.next())
    {
        cameras.append(mapCamera(query));
    }
    return cameras;
}

void SQLiteCameraRepository::addCamera(const Camera &camera)
{
    QSqlQuery query(m_database.connection());

    query.prepare(R"(
        INSERT INTO cameras
        (
            id,
            name,
            url
        )
        VALUES
        (
            :id,
            :name,
            :url
        )
    )");

    query.bindValue(":id", camera.id);
    query.bindValue(":name", camera.name);
    query.bindValue(":url", camera.config.url);

    if(!query.exec())
    {
        qWarning() << query.lastError();
    }
}

bool SQLiteCameraRepository::updateCamera(const Camera &camera)
{
    QSqlQuery query(m_database.connection());

    query.prepare(R"(
        UPDATE cameras
        SET
            name = :name,
            url = :url
        WHERE id = :id
    )");

    query.bindValue(":id", camera.id);
    query.bindValue(":name", camera.name);
    query.bindValue(":url", camera.config.url);

    if (!query.exec())
    {
        qWarning() << query.lastError();
        return false;
    }
    return true;
}

void SQLiteCameraRepository::removeCamera(const CameraId &id)
{
    QSqlQuery query(m_database.connection());

    query.prepare(R"(
        DELETE FROM cameras
        WHERE id = :id
    )");

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qWarning() << query.lastError();
    }
}

Camera SQLiteCameraRepository::mapCamera(const QSqlQuery &query) const
{
    return Camera{
        query.value("id").toString(),
        query.value("name").toString(),
        CameraConfig{
            query.value("url").toString()
        }
    };
}

}
