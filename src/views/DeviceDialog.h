#ifndef DEVICEDIALOG_H
#define DEVICEDIALOG_H

#include <QDialog>
#include "../models/Device.h"

class QLineEdit;
class QSpinBox;
class QComboBox;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;

class DeviceDialog : public QDialog
{
    Q_OBJECT

public:
    // device 为 nullptr 时是新增，否则是编辑
    explicit DeviceDialog(Device* device, QWidget* parent = nullptr);
    ~DeviceDialog();

    // 获取填写的数据
    Device getDevice() const;

private slots:
    void onStatusChanged(int index);  // 状态改变时，控制外借字段的显示
    void onAccept();                  // 点击确定时验证数据

private:
    void setupUi();
    bool validate();  // 验证输入

    Device* m_device;  // 编辑时传入的设备（nullptr 表示新增）
    bool m_isEditMode;

    // 表单控件
    QComboBox* m_cmbLocation;
    QComboBox* m_cmbCategory;
    QLineEdit* m_txtModel;
    QSpinBox* m_spnQuantity;
    QLineEdit* m_txtHolder;
    QComboBox* m_cmbStatus;
    QTextEdit* m_txtNote;

    // 外借相关（状态为"外借"时才显示）
    QLineEdit* m_txtBorrower;
    QDateTimeEdit* m_dtBorrowedAt;
    QDateTimeEdit* m_dtDueAt;

    QPushButton* m_btnOk;
    QPushButton* m_btnCancel;
};

#endif // DEVICEDIALOG_H
