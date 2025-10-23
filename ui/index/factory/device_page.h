#ifndef DEVICE_PAGE_H
#define DEVICE_PAGE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include "../../server/entities/device.h"

class DevicePage : public QWidget
{
    Q_OBJECT
public:
    explicit DevicePage(QWidget *parent = nullptr);
    void loadDevices(const QVector<Device>& devices);

private slots:
    void onAddDevice();
    void onRemoveDevice();
    void onEditDevice();
    void onViewDevice();
signals:
    void addDeviceSignal(const QString& name, const QString &type, const QString& status);
    void removeDeviceSignal(const QString& deviceName);
    void updateDeviceSignal(const QString& deviceName, const QString& type, const QString& status);

private:
    QListWidget *deviceList;
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QTableWidget *deviceTable;

    bool ok;

};

#endif // DEVICE_PAGE_H

