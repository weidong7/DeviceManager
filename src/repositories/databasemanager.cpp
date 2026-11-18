#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

DatabaseManager::DatabaseManager():m_initialized(false) {}
DatabaseManager::~DatabaseManager() {}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize(const QString &dbPath)
{
    if(m_initialized) return true;

    QDir dir;
    dir.mkdir("data");

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("data/" + dbPath);

    if(!m_db.open()) {
        qDebug() << "数据库打开失败:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "数据库连接成功:" << dbPath;

    // 创建表
    if (!createTables()) {
        return false;
    }

    m_initialized = true;
    return true;
}

bool DatabaseManager::createTables() {
    QSqlQuery query(m_db);

    // 创建设备表
    QString createDevices = R"(
          CREATE TABLE IF NOT EXISTS devices (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              location TEXT NOT NULL,
              category TEXT NOT NULL,
              model TEXT NOT NULL,
              quantity INTEGER DEFAULT 1,
              holder TEXT DEFAULT '',
              status TEXT DEFAULT '在用',
              note TEXT DEFAULT '',
              borrower TEXT DEFAULT '',
              borrowed_at TIMESTAMP,
              due_at TIMESTAMP,
              updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
              updated_by TEXT DEFAULT ''
          )
      )";

    if (!query.exec(createDevices)) {
        qDebug() << "创建devices表失败:" << query.lastError().text();
        return false;
    }

    // 创建用户表
    QString createUsers = R"(
          CREATE TABLE IF NOT EXISTS users (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              username TEXT UNIQUE NOT NULL,
              password_hash TEXT NOT NULL,
              display_name TEXT DEFAULT '',
              role TEXT DEFAULT 'viewer',
              created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
          )
      )";

    if (!query.exec(createUsers)) {
        qDebug() << "创建users表失败:" << query.lastError().text();
        return false;
    }

    // 创建日志表
    QString createLogs = R"(
          CREATE TABLE IF NOT EXISTS change_logs (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              device_id INTEGER NOT NULL,
              action TEXT NOT NULL,
              detail TEXT DEFAULT '',
              operator TEXT NOT NULL,
              created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
          )
      )";

    if (!query.exec(createLogs)) {
        qDebug() << "创建change_logs表失败:" << query.lastError().text();
        return false;
    }

    qDebug() << "数据表创建成功";
    return true;
}

QSqlDatabase& DatabaseManager::database() {
    return m_db;
}

void DatabaseManager::close() {
    if (m_db.isOpen()) {
        m_db.close();
    }

    m_initialized = false;   // 允许重新初始化
}