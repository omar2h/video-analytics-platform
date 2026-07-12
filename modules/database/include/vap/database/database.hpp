#pragma once

#include <QObject>
#include <QSqlDatabase>
namespace vap
{
class Database
{
public:
    explicit Database(QString databasePath);
    ~Database();

    bool open();
    void close();

    QSqlDatabase connection() const;

private:
    bool initializeSchema();
    bool createCameraTable();

private:
    QString m_databasePath;
    QSqlDatabase m_database;
};

}

