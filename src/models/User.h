#ifndef USER_H
#define USER_H

#include <QDateTime>
#include <QString>

class User {
public:
    User() : id(0), role("viewer") {}

    int id;
    QString username;
    QString passwordHash;
    QString displayName;
    QString role;
    QDateTime createdAt;

    bool isValid() const {
        return !username.isEmpty() && !passwordHash.isEmpty();
    }
};

#endif // USER_H