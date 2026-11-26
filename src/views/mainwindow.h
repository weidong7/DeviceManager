#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtableview.h>
#include "DeviceTableModel.h"
#include <qcombobox.h>
#include <src/repositories/DeviceRepository.h>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class Mainwindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit Mainwindow(QWidget* parent = nullptr);
	~Mainwindow();

private slots:
	// 工具栏操作
	void onAdd();
	void onEdit();
	void onDelete();
	void onRefresh();
	// 筛选
	void onFilterChanged();
	//// 双击查看日志
	void onRowDoubleClicked(const QModelIndex& index);
	//// 菜单
	void onShowSummary();
	void onExportCsv();
	void onManageUsers();
	void onChangePassword();
	void onLogout();
	void onAbout();
private:
	void setupUi();
	void setupToolBar();
	void setupFilterBar();
	void setupMenuBar();
	void loadData();
	void applyPermissions();   // 根据角色控制按钮可用性

	// UI 组件
	QTableView* m_tableView;
	DeviceTableModel* m_model;

	// 筛选器
	QComboBox* m_cmbLocation;
	QComboBox* m_cmbCategory;
	QComboBox* m_cmbStatus;
	QLineEdit* m_txtKeyword;

	// 按钮
	QPushButton* m_btnAdd;
	QPushButton* m_btnEdit;
	QPushButton* m_btnDelete;
	QPushButton* m_btnRefresh;

	// 状态栏标签
	QLabel* m_lblStatus;

	// 数据访问
	DeviceRepository* m_repository;
};

#endif // MAINWINDOW_H
