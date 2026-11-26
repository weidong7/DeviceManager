#include "./mainwindow.h"
#include "DeviceTableModel.h"
//#include "DeviceDialog.h"
//#include "LogDialog.h"
//#include "SummaryDialog.h"
//#include "UserManagementDialog.h"
#include "../repositories/DeviceRepository.h"
#include "../repositories/LogRepository.h"
#include "../repositories/UserRepository.h"
#include "../utils/SessionManager.h"
#include "../utils/PasswordHasher.h"
//#include "../utils/CsvExporter.h"
#include <QTableView>
#include <QToolBar>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QStatusBar>
#include <plog/Log.h>

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow{parent}
    , m_repository(new DeviceRepository)
{
    setWindowTitle("设备管理系统");
    resize(1200, 700);
    setupUi();
    setupMenuBar();
    loadData();
    applyPermissions();
}

Mainwindow::~Mainwindow()
{
    delete m_repository;
}

void Mainwindow::setupUi() {

    m_model = new DeviceTableModel(this);
    m_lblStatus = new QLabel("就绪", this);
    statusBar()->addWidget(m_lblStatus);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupToolBar();
    setupFilterBar();

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_tableView);
}

void Mainwindow::setupToolBar()
{
    QToolBar* toolBar = addToolBar("工具栏");
    toolBar->setMovable(false);
    toolBar->setIconSize(QSize(16, 16));

    m_btnAdd = new QPushButton("新增", this);
    m_btnEdit = new QPushButton("编辑", this);
    m_btnDelete = new QPushButton("删除", this);
    m_btnRefresh = new QPushButton("刷新", this);

    toolBar->addWidget(m_btnAdd);
    toolBar->addWidget(m_btnEdit);
    toolBar->addWidget(m_btnDelete);
    toolBar->addSeparator();
    toolBar->addWidget(m_btnRefresh);

    connect(m_btnAdd, &QPushButton::clicked, this, &Mainwindow::onAdd);
    connect(m_btnEdit, &QPushButton::clicked, this, &Mainwindow::onEdit);
    connect(m_btnDelete, &QPushButton::clicked, this, &Mainwindow::onDelete);
    connect(m_btnRefresh, &QPushButton::clicked, this, &Mainwindow::onRefresh);
}

void Mainwindow::setupFilterBar() {
    QToolBar * filterBar = addToolBar("筛选栏");
    filterBar->setMovable(false);

    filterBar->addWidget(new QLabel("位置:"));
    m_cmbLocation = new QComboBox(this);
    m_cmbLocation->addItem("全部", "");
    m_cmbLocation->addItem("天津", "天津");
    m_cmbLocation->addItem("北京", "北京");
    filterBar->addWidget(m_cmbLocation);

    filterBar->addWidget(new QLabel("类型:"));
    m_cmbCategory = new QComboBox(this);
    m_cmbCategory->addItem("全部", "");
    m_cmbCategory->addItem("mac", "mac");
    m_cmbCategory->addItem("手机", "手机");
    m_cmbCategory->addItem("pad", "pad");
    m_cmbCategory->addItem("盒子", "盒子");
    m_cmbCategory->addItem("眼镜", "眼镜");
    m_cmbCategory->addItem("cix", "cix");
    m_cmbCategory->addItem("pgx", "pgx");
    m_cmbCategory->addItem("笔记本", "笔记本");
    m_cmbCategory->addItem("无影一体机", "无影一体机");
    m_cmbCategory->addItem("样机", "样机");
    filterBar->addWidget(m_cmbCategory);

    filterBar->addWidget(new QLabel("状态:"));
    m_cmbStatus = new QComboBox(this);
    m_cmbStatus->addItem("全部", "");
    m_cmbStatus->addItem("在用", "在用");
    m_cmbStatus->addItem("闲置", "闲置");
    m_cmbStatus->addItem("外借", "外借");
    m_cmbStatus->addItem("未拆封", "未拆封");
    m_cmbStatus->addItem("报废", "报废");
    filterBar->addWidget(m_cmbStatus);

    filterBar->addWidget(new QLabel("关键字:"));
    m_txtKeyword = new QLineEdit(this);
    m_txtKeyword->setPlaceholderText("型号/使用人/备注");
    m_txtKeyword->setMaximumWidth(200);
    filterBar->addWidget(m_txtKeyword);

    connect(m_cmbLocation, &QComboBox::currentIndexChanged, this, &Mainwindow::onFilterChanged);
    connect(m_cmbCategory, &QComboBox::currentIndexChanged, this, &Mainwindow::onFilterChanged);
    connect(m_cmbStatus, &QComboBox::currentIndexChanged, this, &Mainwindow::onFilterChanged);
    connect(m_txtKeyword, &QLineEdit::textChanged, this, &Mainwindow::onFilterChanged);
}
void Mainwindow::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();

    // 设备菜单
    QMenu* deviceMenu = menuBar->addMenu("设备(&D)");
    deviceMenu->addAction("新增", this, &Mainwindow::onAdd);
    deviceMenu->addAction("编辑", this, &Mainwindow::onEdit);
    deviceMenu->addAction("删除", this, &Mainwindow::onDelete);
    deviceMenu->addSeparator();
    deviceMenu->addAction("刷新", this, &Mainwindow::onRefresh);
    deviceMenu->addSeparator();
    deviceMenu->addAction("导出 CSV...", this, &Mainwindow::onExportCsv);
    deviceMenu->addAction("统计汇总...", this, &Mainwindow::onShowSummary);

    // 用户菜单
    QMenu* userMenu = menuBar->addMenu("用户(&U)");
    userMenu->addAction("用户管理...", this, &Mainwindow::onManageUsers);
    userMenu->addAction("修改密码...", this, &Mainwindow::onChangePassword);
    userMenu->addSeparator();
    userMenu->addAction("注销", this, &Mainwindow::onLogout);

    // 帮助菜单
    QMenu* helpMenu = menuBar->addMenu("帮助(&H)");
    helpMenu->addAction("关于", this, &Mainwindow::onAbout);
}


void Mainwindow::loadData()
{
    DeviceFilters filters;
    filters.location = m_cmbLocation->currentData().toString();
    filters.category = m_cmbCategory->currentData().toString();
    filters.status = m_cmbStatus->currentData().toString();
    filters.keyword = m_txtKeyword->text();

    QList<Device> devices = m_repository->findByFilters(filters);
    m_model->setDevices(devices);

    m_lblStatus->setText(QString("共 %1 条记录").arg(devices.size()));
    PLOG_INFO << "加载设备数据: " << devices.size() << " 条";

}
void Mainwindow::applyPermissions()
{
    // 只读用户禁用增删改按钮
    bool canEdit = SessionManager::getInstance().isAdmin();

    m_btnAdd->setEnabled(canEdit);
    m_btnEdit->setEnabled(canEdit);
    m_btnDelete->setEnabled(canEdit);
}

void Mainwindow::onAdd() {
}
void Mainwindow::onEdit() {
}
void Mainwindow::onDelete() {
}
void Mainwindow::onRefresh() {
    loadData();
}
void Mainwindow::onFilterChanged()
{
    loadData();
}

void Mainwindow::onRowDoubleClicked(const QModelIndex& index)
{
}

void Mainwindow::onShowSummary()
{
}

void Mainwindow::onExportCsv()
{
}

void Mainwindow::onManageUsers()
{
}

void Mainwindow::onChangePassword()
{
}

void Mainwindow::onLogout()
{
}

void Mainwindow::onAbout()
{

}
