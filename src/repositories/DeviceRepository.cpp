#include "DeviceRepository.h"
#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>

DeviceRepository::DeviceRepository()
{}

DeviceRepository::~DeviceRepository()
{}

Device DeviceRepository::queryToDevice(QSqlQuery& query) {
    Device device;
    device.id = query.value("id").toInt();
    device.location = query.value("location").toString();
    device.category = query.value("category").toString();
    device.model = query.value("model").toString();
    device.quantity = query.value("quantity").toInt();
    device.holder = query.value("holder").toString();
    device.status = query.value("status").toString();
    device.note = query.value("note").toString();
    device.borrower = query.value("borrower").toString();
    device.borrowedAt = query.value("borrowed_at").toDateTime();
    device.dueAt = query.value("due_at").toDateTime();
    device.updatedAt = query.value("updated_at").toDateTime();
    device.updatedBy = query.value("updated_by").toString();
    return device;
}


Device DeviceRepository::findById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM devices WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        return queryToDevice(query);
    }
    qDebug() << "未找到 ID 为" << id << "的设备";
    return Device();
}

//QList<Device> findAll();
QList<Device> DeviceRepository::findAll() {
    QList<Device> devices;
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT * FROM device ORDER BY location , category , id")) {
        while (query.next()) {
            devices.append(queryToDevice(query));
        }
    }
    else {
        qDebug() << "查询失败:" << query.lastError().text();
    }

    return devices;
}

/*
QString location;
QString category;
QString status;
QString holder;
QString keyword;  // 模糊搜索（型号/备注/使用人）
*/

QList<Device> DeviceRepository::findByFilters(const DeviceFilters& filters)
{
    QList<Device> devices;
    QSqlQuery query(DatabaseManager::instance().database());

    QString sql = "SELECT * FROM device WHERE 1=1";
    
    if(!filters.location.isEmpty()){
        sql += "AND location = :location";
    }
    if (!filters.category.isEmpty()) {
        sql += "AND category = :category";
    }
    if (!filters.status.isEmpty()) {
        sql += "AND holder = :holder";
    }
    if (!filters.keyword.isEmpty()) {
        sql += "mode LINK :keyword OR note LINK :keyword OR holder LINK :keyword";
    }

    sql += " ORDER BY location, category, id";

    query.prepare(sql);

    // 绑定参数（命名参数）
    if (!filters.location.isEmpty()) {
        query.bindValue(":location", filters.location);
    }
    if (!filters.category.isEmpty()) {
        query.bindValue(":category", filters.category);
    }
    if (!filters.status.isEmpty()) {
        query.bindValue(":status", filters.status);
    }
    if (!filters.holder.isEmpty()) {
        query.bindValue(":holder", filters.holder);
    }
    if (!filters.keyword.isEmpty()) {
        query.bindValue(":keyword", "%" + filters.keyword + "%");
    }

    if (query.exec()) {
        while (query.next()) {
            devices.append(queryToDevice(query));
        }
    }
    else {
        qDebug() << "筛选查询失败:" << query.lastError().text();
    }

    return devices;
}

int DeviceRepository::count()
{
    QSqlQuery query(DatabaseManager::instance().database());
    if (query.exec("SELECT COUNT(*) FROM devices") && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

bool DeviceRepository::create(Device& device) {
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(R"(
          INSERT INTO devices (location, category, model, quantity, holder, status, note,
                             borrower, borrowed_at, due_at, updated_by)
          VALUES (:location, :category, :model, :quantity, :holder, :status, :note,
                  :borrower, :borrowed_at, :due_at, :updated_by)
      )");

    query.bindValue(":location", device.location);
    query.bindValue(":category", device.category);
    query.bindValue(":model", device.model);
    query.bindValue(":quantity", device.quantity);
    query.bindValue(":holder", device.holder);
    query.bindValue(":status", device.status);
    query.bindValue(":note", device.note);
    query.bindValue(":borrower", device.borrower);
    query.bindValue(":borrowed_at", device.borrowedAt);
    query.bindValue(":due_at", device.dueAt);
    query.bindValue(":updated_by", device.updatedBy);

    if (query.exec()) {
        device.id = query.lastInsertId().toInt();  // 设置自增ID
        qDebug() << "设备插入成功，ID:" << device.id;
        return true;
    }

    qDebug() << "设备插入失败:" << query.lastError().text();
    return false;
}

bool DeviceRepository::update(const Device& device) {
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(R"(
          UPDATE devices SET
              location = :location,
              category = :category,
              model = :model,
              quantity = :quantity,
              holder = :holder,
              status = :status,
              note = :note,
              borrower = :borrower,
              borrowed_at = :borrowed_at,
              due_at = :due_at,
              updated_by = :updated_by,
              updated_at = CURRENT_TIMESTAMP
          WHERE id = :id
      )");

    query.bindValue(":id", device.id);
    query.bindValue(":location", device.location);
    query.bindValue(":category", device.category);
    query.bindValue(":model", device.model);
    query.bindValue(":quantity", device.quantity);
    query.bindValue(":holder", device.holder);
    query.bindValue(":status", device.status);
    query.bindValue(":note", device.note);
    query.bindValue(":borrower", device.borrower);
    query.bindValue(":borrowed_at", device.borrowedAt);
    query.bindValue(":due_at", device.dueAt);
    query.bindValue(":updated_by", device.updatedBy);

    if (query.exec()) {
        qDebug() << "设备更新成功，ID:" << device.id;
        return true;
    }

    qDebug() << "设备更新失败:" << query.lastError().text();
    return false;
}

bool DeviceRepository::deleteById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM devices WHERE id = ?");
    query.addBindValue(id);

    if (query.exec()) {
        qDebug() << "设备删除成功，ID:" << id;
        return true;
    }

    qDebug() << "设备删除失败:" << query.lastError().text();
    return false;
}
