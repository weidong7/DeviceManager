#include "SessionManager.h"

SessionManager& SessionManager::getInstance() {
    static SessionManager instance;
    return instance;
}

void SessionManager::setCurrentUser(const QString& username, bool admin) {
    currentUsername_ = username;
    isAdmin_ = admin;
}

QString SessionManager::getCurrentUser() const
{
    return currentUsername_;
}

bool SessionManager::isAdmin() const
{
    return isAdmin_;
}

void SessionManager::clearSession()
{
    currentUsername_.clear();
    isAdmin_ = false;
}
