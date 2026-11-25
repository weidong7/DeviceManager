#include "./RepositoryTests.h"
#include "../repositories/DeviceRepository.h"
#include "../repositories/UserRepository.h"
#include "../repositories/LogRepository.h"
#include "../models/ChangeLog.h"
#include "../models/User.h"
#include <plog/Log.h>
#include <QSqlQuery>
#include <QSqlError>

void RepositoryTests::testDeviceRepository() {
    DeviceRepository repo;

    PLOG_INFO << "========== 测试设备查询 ==========";
    QList<Device> all = repo.findAll();
    PLOG_INFO << "设备总数: " << all.size();

    for (const Device& d : all) {
        PLOG_INFO << d.id << " " << d.location.toStdString()
                  << " " << d.category.toStdString()
                  << " " << d.model.toStdString();
    }

    PLOG_INFO << "========== 测试新增设备 ==========";
    Device newDevice;
    newDevice.location = "北京";
    newDevice.category = "手机";
    newDevice.model = "iPhone 15 Pro";
    newDevice.quantity = 2;
    newDevice.status = "在用";

    if (repo.create(newDevice)) {
        PLOG_INFO << "新增设备ID: " << newDevice.id;

        // 测试查询
        Device found = repo.findById(newDevice.id);
        PLOG_INFO << "查询验证: " << found.model.toStdString()
                  << " 数量: " << found.quantity;

        // 测试更新
        found.holder = "张三";
        found.updatedBy = "admin";
        repo.update(found);

        // 测试筛选
        DeviceFilters filters;
        filters.location = "北京";
        QList<Device> filtered = repo.findByFilters(filters);
        PLOG_INFO << "北京的设备数量: " << filtered.size();
    }
}

void RepositoryTests::testUserRepository() {
    PLOG_INFO << "========== 测试用户管理 ==========";
    UserRepository userRepo;

    User admin;
    admin.username = "test_admin";
    admin.passwordHash = "临时密码哈希";
    admin.displayName = "测试管理员";
    admin.role = "admin";

    if (userRepo.create(admin)) {
        PLOG_INFO << "创建用户成功，ID: " << admin.id;

        User found = userRepo.findByUsername("test_admin");
        PLOG_INFO << "查到用户: " << found.username.toStdString()
                  << " " << found.displayName.toStdString();
    }
}

void RepositoryTests::testLogRepository() {
    PLOG_INFO << "========== 测试日志记录 ==========";
    LogRepository logRepo;

    ChangeLog log;
    log.deviceId = 1;
    log.action = "test";
    log.detail = "单元测试创建的日志";
    log.operatorName = "test";

    if (logRepo.create(log)) {
        PLOG_INFO << "日志记录成功，ID: " << log.id;

        QList<ChangeLog> logs = logRepo.findByDeviceId(1);
        PLOG_INFO << "设备 1 的日志数量: " << logs.size();
    }
}

void RepositoryTests::runAll() {
    PLOG_INFO << "========================================";
    PLOG_INFO << "开始 Repository 层单元测试";
    PLOG_INFO << "========================================";

    testDeviceRepository();
    testUserRepository();
    testLogRepository();

    PLOG_INFO << "========================================";
    PLOG_INFO << "所有测试完成";
    PLOG_INFO << "========================================";
}
