#ifndef PASSWORDHASHER_H
#define PASSWORDHASHER_H

#include <QByteArray>
#include <QString>

class PasswordHasher {
public:
    // 生成随机盐并计算哈希
    // 返回格式建议：迭代次数$盐(Base64)$哈希(Base64)
    static QString hashPassword(const QString& password);

    // 校验密码
    static bool verifyPassword(const QString& password, const QString& storedHash);

private:
    static QByteArray generateSalt(int length = 16);
};

#endif