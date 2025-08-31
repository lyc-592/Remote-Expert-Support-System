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
    void addDeviceSignal(const QString& name, const QString &type);

private:
    QListWidget *deviceList;
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    // 初始化数据库连接
    bool initDb();
    // 从数据库加载设备数据到 deviceList


    //Client *client;
};

#endif // DEVICE_PAGE_H
