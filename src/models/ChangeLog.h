#ifndef CHANGELOG_H
#define CHANGELOG_H

#include <QDateTime>
#include <QString>

class ChangeLog {
public:
    ChangeLog() : id(0), deviceId(0) {}

    int id;
    int deviceId;
    QString action;
    QString detail;
    QString operatorName;
    QDateTime createdAt;

    bool isValid() const {
        return deviceId > 0 && !action.isEmpty() && !operatorName.isEmpty();
    }
};

#endif // CHANGELOG_H