  #include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "repositories/DatabaseManager.h"
#include "repositories/DeviceRepository.h"
#include "models/Device.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

void testInsertDevice() {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO devices (location, category, model) VALUES (?, ?, ?)");
    query.addBindValue("天津");
    query.addBindValue("mac");
    query.addBindValue("MacBook Pro 2023");

    if (query.exec()) {
        qDebug() << "插入成功！ID:" << query.lastInsertId();
    }
    else {
        qDebug() << "插入失败:" << query.lastError().text();
    }
}

void testInsertUsers() {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO users (username, password_hash, display_name) VALUES (?, ?, ?)");
    query.addBindValue("嗡嗡嗡");
    query.addBindValue("123456");
    query.addBindValue("呜呜呜");

    if (query.exec()) {
        qDebug() << "插入成功！ID:" << query.lastInsertId();
    }
    else {
        qDebug() << "插入失败:" << query.lastError().text();
    }
}

void testRepository() {
    DeviceRepository repo;

    // 测试1：查询所有
    LOG_INFO << "========== 查询所有设备 ==========";
    QList<Device> all = repo.findAll();
    LOG_INFO << "总数: " << all.size();
    for (const Device& d : all) {
        LOG_INFO << d.id << " " << d.location.toStdString()
            << " " << d.category.toStdString()
            << " " << d.model.toStdString();
    }

    // 测试2：新增设备
    LOG_INFO << "\n========== 新增设备 ==========";
    Device newDevice;
    newDevice.location = "北京";
    newDevice.category = "手机";
    newDevice.model = "iPhone 15 Pro";
    newDevice.quantity = 2;
    newDevice.status = "在用";
    repo.create(newDevice);
    LOG_INFO << "新增设备ID: " << newDevice.id;

    // 测试3：根据ID查询
    LOG_INFO << "\n========== 根据ID查询 ==========";
    Device found = repo.findById(newDevice.id);
    LOG_INFO << "找到设备: " << found.model.toStdString()
        << " 数量: " << found.quantity;

    // 测试4：更新
    LOG_INFO << "\n========== 更新设备 ==========";
    found.holder = "张三";
    found.updatedBy = "admin";
    repo.update(found);
    LOG_INFO << "更新完成，持有人改为: " << found.holder.toStdString();

    // 测试5：筛选查询
    LOG_INFO << "\n========== 筛选查询 ==========";
    DeviceFilters filters;
    filters.location = "天津";
    QList<Device> filtered = repo.findByFilters(filters);
    LOG_INFO << "天津的设备数量: " << filtered.size();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    plog::init(plog::debug, "device-manager.log");

    DatabaseManager &dbManager = DatabaseManager::instance();
    if (!dbManager.initialize()) {
        qDebug() << "数据库初始化失败！";
        return -1;
    }
    qDebug() << "数据库初始化成功！";

    PLOGD << "Database connected successfully";

    testInsertDevice();
    testInsertUsers();

    testRepository();

    QWidget window;
    window.setWindowTitle("设备管理系统 - 数据库测试");
    window.resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *label = new QLabel("数据库已初始化！\n检查 data/devices.db 文件");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *btn = new QPushButton("关闭");
    layout->addWidget(btn);
    QObject::connect(btn, &QPushButton::clicked, &window, &QWidget::close);

    window.show();

    return app.exec();
}
