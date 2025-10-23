#ifndef EXPERT_DEVICE_PAGE_H
#define EXPERT_DEVICE_PAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "../../client/core/client.h"

class ExpertDevicePage : public QWidget
{
    Q_OBJECT

public:
    explicit ExpertDevicePage(Client* client, QWidget *parent = nullptr);
    void loadDevices(const QVector<Device>& devices);

signals:
    void refreshDevicesSignal();
    void updateDeviceSignal(const QString& deviceName, const QString& type, const QString& status);

private slots:
    void onRefreshButtonClicked();
    void onGetAllDevicesReceived(bool success, const QString& message, const QVector<Device>& devices);
    void onControlDevice();

private:
    Client* client;
    QTableWidget* devicesTable;
    QPushButton* refreshButton;
    bool ok;
};

#endif // EXPERT_DEVICE_PAGE_H
