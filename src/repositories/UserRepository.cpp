#include "UserRepository.h"
#include "databasemanager.h"
#include <plog/Log.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>

UserRepository::UserRepository()
{}

UserRepository::~UserRepository()
{}

User UserRepository::queryToUser(QSqlQuery & query)
{
    User user;
    user.id = query.value("id").toInt();
    user.username = query.value("username").toString();
    user.passwordHash = query.value("password_hash").toString();
    user.displayName = query.value("display_name").toString();
    user.role = query.value("role").toString();
    user.createdAt = query.value("created_at").toDateTime();
    return user;
}

User UserRepository::findByUsername(const QString& name)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM users WHERE username = :username");
    query.bindValue(":username", name);

    if (query.exec() && query.next()) {
        return queryToUser(query);
    }
    PLOG_WARNING << "未找到用户: " << name.toStdString();

    return User();
}


bool UserRepository::create(User& user)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(R"(
      INSERT INTO users (username, password_hash, display_name, role)
      VALUES (:username, :password_hash, :display_name, :role))");

    query.bindValue(":username", user.username);
    query.bindValue(":password_hash", user.passwordHash);
    query.bindValue(":display_name", user.displayName);
    query.bindValue(":role", user.role);

    if (query.exec()) {
        user.id = query.lastInsertId().toInt();  // 设置自增ID
        return true;
    }

    PLOG_ERROR << "用户写入失败: " << query.lastError().text().toStdString();

    return false;
}

bool UserRepository::update(const User& user)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(R"(
          UPDATE users SET
              username = :username,
              password_hash = :password_hash,
              display_name = :display_name,
              role = :role
          WHERE id = :id
      )");

    query.bindValue(":id", user.id);
    query.bindValue(":username", user.username);
    query.bindValue(":password_hash", user.passwordHash);
    query.bindValue(":display_name", user.displayName);
    query.bindValue(":role", user.role);

    if (query.exec()) {
        PLOG_INFO << "用户更新成功，ID: " << user.id;
        return true;
    }

    PLOG_ERROR << "用户更新失败: " << query.lastError().text().toStdString();
    return false;
}

bool UserRepository::deleteById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM users WHERE id = ?");
    query.addBindValue(id);

    if (query.exec()) {
        PLOG_INFO << "用户删除成功，ID: " << id;
        return true;
    }

    PLOG_ERROR << "用户删除失败: " << query.lastError().text().toStdString();
    return false;
}