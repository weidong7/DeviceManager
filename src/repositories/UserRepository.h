#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "../models/User.h"
#include <QDateTime>
#include <QString>

class UserRepository 
{
public:

    User findByUsername(const QString &name);

    bool create(User &user);
    bool update(const User &user);
    bool deleteById(int id);

	UserRepository();
	~UserRepository();

private:
    User queryToUser(class QSqlQuery& query);  // 辅助方法
};

#endif //USERREPOSITORY_H