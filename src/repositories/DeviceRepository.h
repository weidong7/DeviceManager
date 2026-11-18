#ifndef DEVICEREPOSITORY_H
#define DEVICEREPOSITORY_H

#include "../models/Device.h"
#include <QString>
#include <QList>

struct DeviceFilters{
      QString location;
      QString category;
      QString status;
      QString holder;
      QString keyword;  // 模糊搜索（型号/备注/使用人）
};

class DeviceRepository 
{
public:

    Device findById(int id);
    QList<Device> findAll();
    QList<Device> findByFilters(const DeviceFilters& filters);
    int count();

    bool create(Device &device);
    bool update(const Device& device);
    bool deleteById(int id);

	DeviceRepository();


	~DeviceRepository();

private:
    // 辅助方法：把QSqlQuery的一行转换成Device对象
    Device queryToDevice(class QSqlQuery& query);
};

#endif // DEVICEREPOSITORY_H