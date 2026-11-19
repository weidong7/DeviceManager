#ifndef LOGREPOSITORY_H
#define LOGREPOSITORY_H

#include "../models/ChangeLog.h"

class LogRepository 
{
public:

	bool create(ChangeLog &log);
	QList<ChangeLog> findByDeviceId(int deviceId, int limit = 50);  // 一个设备有多条日志

	LogRepository();
	~LogRepository();

private:
	ChangeLog queryToLog(class QSqlQuery& query);

};

#endif //LOGREPOSITORY_H