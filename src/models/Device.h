#ifndef DEVICE_H
#define DEVICE_H

#include <QDateTime>
#include <QString>

class Device {
public:
    //构造函数
    Device() : id(0), quantity(1) {}

      // 成员变量（对应数据库字段）
      int id;                      // 主键
      QString location;            // 位置（天津/北京）
      QString category;            // 类型（pad/mac/手机...）
      QString model;               // 型号
      int quantity;                // 数量
      QString holder;              // 使用人
      QString status;              // 状态（在用/闲置/外借...）
      QString note;                // 备注

      // 外借相关
      QString borrower;            // 借用人
      QDateTime borrowedAt;        // 借出时间
      QDateTime dueAt;             // 应还时间

      // 审计字段
      QDateTime updatedAt;         // 最后更新时间
      QString updatedBy;           // 最后更新人

      //
      bool isValid() const {
        return !location.isEmpty() && !category.isEmpty() && !model.isEmpty();
      }

};

#endif // DEVICE_H
