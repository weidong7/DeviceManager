#include "LogRepository.h"
#include "databasemanager.h"
#include <plog/Log.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>

LogRepository::LogRepository()
{}

LogRepository::~LogRepository()
{}

ChangeLog LogRepository::queryToLog(QSqlQuery & query)
{
    ChangeLog log;
    log.id = query.value("id").toInt();
    log.deviceId = query.value("device_id").toInt();
    log.action = query.value("action").toString();
    log.detail = query.value("detail").toString();
    log.operatorName = query.value("operator").toString();
    log.createdAt = query.value("created_at").toDateTime();
    return log;
}

bool LogRepository::create(ChangeLog& log)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(R"(
          INSERT INTO change_logs (device_id, action, detail, operator)
          VALUES (:device_id, :action, :detail, :operator))");

    query.bindValue(":device_id", log.deviceId);
    query.bindValue(":action", log.action);
    query.bindValue(":detail", log.detail);
    query.bindValue(":operator", log.operatorName);

    if (query.exec()) {
        log.id = query.lastInsertId().toInt();  // 设置自增ID
        return true;
    }

    PLOG_ERROR << "日志写入失败: " << query.lastError().text().toStdString();

    return false;
}


QList<ChangeLog> LogRepository::findByDeviceId(int deviceId, int limit)
{
    QList<ChangeLog> logs;
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(R"(
          SELECT * FROM change_logs
          WHERE device_id = :device_id
          ORDER BY id DESC
          LIMIT :limit
      )");

    query.bindValue(":device_id", deviceId);
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        PLOG_ERROR << "查询日志失败: " << query.lastError().text().toStdString();
        return logs;
    }

    while (query.next()) {      // 多行，必须循环
        logs.append(queryToLog(query));
    }


    return logs;
}
