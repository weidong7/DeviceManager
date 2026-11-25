#ifndef DEVICETABLEMODEL_H
#define DEVICETABLEMODEL_H

#include "../models/Device.h"
#include <QAbstractTableModel>


class DeviceTableModel  : public QAbstractTableModel
{
	Q_OBJECT

public:

	enum Column {
		ColId = 0,
		ColLocation,
		ColCategory,
		ColModel,
		ColQuantity,
		ColHolder,
		ColStatus,
		ColBorrower,
		ColNote,
		ColumnCount        // 放最后，自动等于列数
	};

	explicit DeviceTableModel(QObject *parent = nullptr);
	~DeviceTableModel() override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void setDevices(const QList<Device>& devices);
	Device deviceAt(int row) const;                  // 取某行对应的 Device

private:
	QList<Device> m_devices; //设备数据容器
};

#endif //DEVICETABLEMODEL_H