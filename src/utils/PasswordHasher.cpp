#include "PasswordHasher.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QStringList>
#include <QPasswordDigestor>

QByteArray PasswordHasher::generateSalt(int length) {
    QByteArray salt(length, 0);
    // 使用系统级随机数生成器填充盐值
    QRandomGenerator::system()->generate(salt.begin(), salt.end());
    return salt;
}

QString PasswordHasher::hashPassword(const QString& password) {
    // 1. 生成随机盐 (16字节)
    QByteArray salt = generateSalt(16);
    int iterations = 100000; // 迭代次数，可根据硬件性能调整（通常 10w+）

    // 2. 使用 PBKDF2-SHA256 派生密钥
    QByteArray hash = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        password.toUtf8(),
        salt,
        iterations,
        32 // 输出 32 字节哈希
    );

    // 3. 将 迭代次数、盐、哈希 组合成一个字符串存储
    // 格式：iterations$saltBase64$hashBase64
    return QString("%1$%2$%3")
        .arg(iterations)
        .arg(QString(salt.toBase64()))
        .arg(QString(hash.toBase64()));
}

bool PasswordHasher::verifyPassword(const QString& password, const QString& storedHash) {
    // 1. 解析存储的哈希字符串
    QStringList parts = storedHash.split('$');
    if (parts.size() != 3) return false;

    int iterations = parts[0].toInt();
    QByteArray salt = QByteArray::fromBase64(parts[1].toUtf8());
    QByteArray expectedHash = QByteArray::fromBase64(parts[2].toUtf8());

    // 2. 用相同的盐和迭代次数重新计算
    QByteArray actualHash = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        password.toUtf8(),
        salt,
        iterations,
        expectedHash.size() // 保持长度一致
    );

    // 3. 恒定时间比较，防止时序攻击
    // 这里简单比较，实际生产环境可自行实现恒定时间比较逻辑
    return (actualHash == expectedHash);
}