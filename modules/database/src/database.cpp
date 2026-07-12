#include <vap/database/database.hpp>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace vap
{
Database::Database(QString databasePath)
    : m_databasePath(std::move(databasePath))
{}

bool Database::open()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");

    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open())
    {
        qWarning() << "";
        return false;
    }

    return initializeSchema();
}

void Database::close()
{
    if (m_database.isOpen())
    {
        m_database.close();
    }
}

Database::~Database()
{
    close();
}

QSqlDatabase Database::connection() const
{
    return m_database;
}

bool Database::initializeSchema()
{
    return createCameraTable();
}

bool Database::createCameraTable()
{
    QSqlQuery query(m_database);

    const bool success = query.exec(R"(
        CREATE TABLE IF NOT EXISTS cameras
        (
            id   TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            url  TEXT NOT NULL
        )
    )");

    if (!success)
    {
        qWarning() << "Failed to create cameras table:"
                   << query.lastError().text();
        return false;
    }

    return true;
}

}
