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

class DevicePage : public QWidget
{
    Q_OBJECT
public:
    explicit DevicePage(QWidget *parent = nullptr);

private slots:
    void onAddDevice();
    void onRemoveDevice();
    void onEditDevice();
    void onViewDevice();

private:
    QListWidget *deviceList;
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    // 初始化数据库连接
    bool initDb();
    // 从数据库加载设备数据到 deviceList
    void loadDevicesFromDb();

    //Client *client;
};

#endif // DEVICE_PAGE_H
