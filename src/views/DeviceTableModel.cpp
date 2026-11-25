#include "DeviceTableModel.h"
#include <QDebug>

DeviceTableModel::DeviceTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{}

DeviceTableModel::~DeviceTableModel()
{}

int DeviceTableModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid()) return 0;
	return m_devices.size();
}

int DeviceTableModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid()) return 0;
	return ColumnCount;
}

QVariant DeviceTableModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() < 0 || index.row() >= m_devices.size()) return {};

	const Device& d = m_devices.at(index.row());

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case ColId:        return d.id;
		case ColLocation:  return d.location;
		case ColCategory:  return d.category;
		case ColModel:     return d.model;
		case ColQuantity:  return d.quantity;
		case ColHolder:    return d.holder;
		case ColStatus:    return d.status;
		case ColBorrower:  return d.borrower;
		case ColNote:      return d.note;
		default:           return QVariant();
		}
	}

	// TextAlignmentRole = "这格文字怎么对齐"
	if (role == Qt::TextAlignmentRole) {
		if (index.column() == ColId || index.column() == ColQuantity) {
			return int(Qt::AlignRight | Qt::AlignVCenter);
		}
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	}

	return QVariant();
}

QVariant DeviceTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case ColId:        return "ID";
		case ColLocation:  return "位置";
		case ColCategory:  return "类型";
		case ColModel:     return "型号";
		case ColQuantity:  return "数量";
		case ColHolder:    return "使用人";
		case ColStatus:    return "状态";
		case ColBorrower:  return "借用人";
		case ColNote:      return "备注";
		default:           return QVariant();
		}
	}

	return section + 1;  // 垂直表头显示行号
}

void DeviceTableModel::setDevices(const QList<Device>& devices)
{
	beginResetModel();
	m_devices = devices;
	endResetModel();
}

Device DeviceTableModel::deviceAt(int row) const
{
	if (row < 0 || row >= m_devices.size()) return Device();
	return m_devices.at(row);
}