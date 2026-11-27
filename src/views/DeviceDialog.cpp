#include "DeviceDialog.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>

DeviceDialog::DeviceDialog(Device* device, QWidget* parent)
    : QDialog(parent)
    , m_device(device)
    , m_isEditMode(device != nullptr)
{
    setupUi();

    // 编辑模式：填充现有数据
    if (m_isEditMode) {
        setWindowTitle("编辑设备");

        // 设置位置（查找匹配的索引）
        int locIndex = m_cmbLocation->findText(m_device->location);
        if (locIndex >= 0) m_cmbLocation->setCurrentIndex(locIndex);

        // 设置类型
        int catIndex = m_cmbCategory->findText(m_device->category);
        if (catIndex >= 0) m_cmbCategory->setCurrentIndex(catIndex);

        m_txtModel->setText(m_device->model);
        m_spnQuantity->setValue(m_device->quantity);
        m_txtHolder->setText(m_device->holder);

        // 设置状态
        int statusIndex = m_cmbStatus->findText(m_device->status);
        if (statusIndex >= 0) m_cmbStatus->setCurrentIndex(statusIndex);

        m_txtNote->setPlainText(m_device->note);

        // 外借信息
        m_txtBorrower->setText(m_device->borrower);
        if (m_device->borrowedAt.isValid()) {
            m_dtBorrowedAt->setDateTime(m_device->borrowedAt);
        }
        if (m_device->dueAt.isValid()) {
            m_dtDueAt->setDateTime(m_device->dueAt);
        }
    }
    else {
        setWindowTitle("新增设备");
    }
}

DeviceDialog::~DeviceDialog()
{
}

void DeviceDialog::setupUi()
{
    resize(500, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // ========== 基本信息 ==========
    QGroupBox* basicGroup = new QGroupBox("基本信息", this);
    QFormLayout* basicForm = new QFormLayout(basicGroup);

    // 位置
    m_cmbLocation = new QComboBox(this);
    m_cmbLocation->addItem("天津");
    m_cmbLocation->addItem("北京");
    m_cmbLocation->setEditable(true);  // 允许输入自定义位置
    basicForm->addRow("位置*:", m_cmbLocation);

    // 类型
    m_cmbCategory = new QComboBox(this);
    m_cmbCategory->addItem("mac");
    m_cmbCategory->addItem("手机");
    m_cmbCategory->addItem("pad");
    m_cmbCategory->addItem("盒子");
    m_cmbCategory->addItem("眼镜");
    m_cmbCategory->addItem("cix");
    m_cmbCategory->addItem("pgx");
    m_cmbCategory->addItem("笔记本");
    m_cmbCategory->addItem("无影一体机");
    m_cmbCategory->addItem("样机");
    m_cmbCategory->setEditable(true);  // 允许输入自定义类型
    basicForm->addRow("类型*:", m_cmbCategory);

    // 型号
    m_txtModel = new QLineEdit(this);
    m_txtModel->setPlaceholderText("例如：MacBook Pro 2023");
    basicForm->addRow("型号*:", m_txtModel);

    // 数量
    m_spnQuantity = new QSpinBox(this);
    m_spnQuantity->setRange(1, 9999);
    m_spnQuantity->setValue(1);
    basicForm->addRow("数量*:", m_spnQuantity);

    // 使用人
    m_txtHolder = new QLineEdit(this);
    m_txtHolder->setPlaceholderText("例如：张三");
    basicForm->addRow("使用人:", m_txtHolder);

    // 状态
    m_cmbStatus = new QComboBox(this);
    m_cmbStatus->addItem("在用");
    m_cmbStatus->addItem("闲置");
    m_cmbStatus->addItem("外借");
    m_cmbStatus->addItem("未拆封");
    m_cmbStatus->addItem("报废");
    basicForm->addRow("状态*:", m_cmbStatus);

    // 备注
    m_txtNote = new QTextEdit(this);
    m_txtNote->setPlaceholderText("备注信息");
    m_txtNote->setMaximumHeight(80);
    basicForm->addRow("备注:", m_txtNote);

    mainLayout->addWidget(basicGroup);

    // ========== 外借信息 ==========
    QGroupBox* borrowGroup = new QGroupBox("外借信息（状态为\"外借\"时填写）", this);
    QFormLayout* borrowForm = new QFormLayout(borrowGroup);

    // 借用人
    m_txtBorrower = new QLineEdit(this);
    m_txtBorrower->setPlaceholderText("外借时必填");
    borrowForm->addRow("借用人:", m_txtBorrower);

    // 借出时间
    m_dtBorrowedAt = new QDateTimeEdit(this);
    m_dtBorrowedAt->setCalendarPopup(true);
    m_dtBorrowedAt->setDateTime(QDateTime::currentDateTime());
    borrowForm->addRow("借出时间:", m_dtBorrowedAt);

    // 应还时间
    m_dtDueAt = new QDateTimeEdit(this);
    m_dtDueAt->setCalendarPopup(true);
    m_dtDueAt->setDateTime(QDateTime::currentDateTime().addDays(7));  // 默认7天后
    borrowForm->addRow("应还时间:", m_dtDueAt);

    mainLayout->addWidget(borrowGroup);

    // 状态改变时控制外借字段可用性
    connect(m_cmbStatus, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &DeviceDialog::onStatusChanged);
    onStatusChanged(m_cmbStatus->currentIndex());  // 初始化

    // ========== 按钮 ==========
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_btnOk = new QPushButton("确定", this);
    m_btnCancel = new QPushButton("取消", this);

    buttonLayout->addWidget(m_btnOk);
    buttonLayout->addWidget(m_btnCancel);

    mainLayout->addLayout(buttonLayout);

    // 连接信号
    connect(m_btnOk, &QPushButton::clicked, this, &DeviceDialog::onAccept);
    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void DeviceDialog::onStatusChanged(int index)
{
    QString status = m_cmbStatus->itemText(index);
    bool isBorrowing = (status == "外借");

    // 外借状态时启用外借字段，否则禁用
    m_txtBorrower->setEnabled(isBorrowing);
    m_dtBorrowedAt->setEnabled(isBorrowing);
    m_dtDueAt->setEnabled(isBorrowing);

    // 非外借状态时清空外借字段
    if (!isBorrowing) {
        m_txtBorrower->clear();
    }
}

void DeviceDialog::onAccept()
{
    if (validate()) {
        accept();  // 关闭对话框，返回 QDialog::Accepted
    }
}

bool DeviceDialog::validate()
{
    // 验证位置
    QString location = m_cmbLocation->currentText().trimmed();
    if (location.isEmpty()) {
        QMessageBox::warning(this, "验证失败", "位置不能为空");
        m_cmbLocation->setFocus();
        return false;
    }

    // 验证类型
    QString category = m_cmbCategory->currentText().trimmed();
    if (category.isEmpty()) {
        QMessageBox::warning(this, "验证失败", "类型不能为空");
        m_cmbCategory->setFocus();
        return false;
    }

    // 验证型号
    QString model = m_txtModel->text().trimmed();
    if (model.isEmpty()) {
        QMessageBox::warning(this, "验证失败", "型号不能为空");
        m_txtModel->setFocus();
        return false;
    }

    // 验证外借状态
    if (m_cmbStatus->currentText() == "外借") {
        QString borrower = m_txtBorrower->text().trimmed();
        if (borrower.isEmpty()) {
            QMessageBox::warning(this, "验证失败", "状态为\"外借\"时，借用人不能为空");
            m_txtBorrower->setFocus();
            return false;
        }
    }

    return true;
}

Device DeviceDialog::getDevice() const
{
    Device device;

    // 编辑模式保留原 ID
    if (m_isEditMode) {
        device.id = m_device->id;
        device.updatedAt = m_device->updatedAt;
    }

    device.location = m_cmbLocation->currentText().trimmed();
    device.category = m_cmbCategory->currentText().trimmed();
    device.model = m_txtModel->text().trimmed();
    device.quantity = m_spnQuantity->value();
    device.holder = m_txtHolder->text().trimmed();
    device.status = m_cmbStatus->currentText();
    device.note = m_txtNote->toPlainText().trimmed();

    // 外借信息（只有状态为"外借"时才保存）
    if (device.status == "外借") {
        device.borrower = m_txtBorrower->text().trimmed();
        device.borrowedAt = m_dtBorrowedAt->dateTime();
        device.dueAt = m_dtDueAt->dateTime();
    }
    else {
        device.borrower = "";
        device.borrowedAt = QDateTime();
        device.dueAt = QDateTime();
    }

    // updatedBy 由调用方设置（MainWindow 知道当前用户是谁）

    return device;
}
