#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:

    //单例模式：全局只有一个数据库连接
    static DatabaseManager& instance();
    
    //初始化数据库（创建表）
    bool initialize(const QString &dbPath = "device.db");
    
    //获取数据库连接
    QSqlDatabase& database();

    void close();

private:

    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    bool m_initialized;

    // 创建表的SQL
    bool createTables();
};

#endif // DATABASEMANAGER_H
