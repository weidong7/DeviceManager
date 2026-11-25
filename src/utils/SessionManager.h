#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager
{
public:

    static SessionManager& getInstance();

    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    void setCurrentUser(const QString& username, bool admin);
    QString getCurrentUser() const;
    bool isAdmin() const;
    void clearSession();

private:

    SessionManager() = default;

    QString currentUsername_;
    bool isAdmin_ = false;
};

#endif //SESSIONMANAGER_H
