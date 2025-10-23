#include "expert_device_page.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QWidget>
#include <QInputDialog>

ExpertDevicePage::ExpertDevicePage(Client* client, QWidget *parent)
    : QWidget(parent), client(client)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 添加刷新按钮
    refreshButton = new QPushButton("刷新设备列表");
    connect(refreshButton, &QPushButton::clicked, this, &ExpertDevicePage::onRefreshButtonClicked);

    // 创建设备表格
    devicesTable = new QTableWidget;
    devicesTable->setColumnCount(3);
    devicesTable->setHorizontalHeaderLabels(QStringList() << "设备名称" << "设备类型" << "状态");
    devicesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自适应列宽
    devicesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    devicesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(refreshButton);
    layout->addWidget(devicesTable);

    // 连接Client的信号
    connect(this->client, &Client::getAllDevicesResult, this, &ExpertDevicePage::onGetAllDevicesReceived);
}

void ExpertDevicePage::onRefreshButtonClicked()
{
    emit refreshDevicesSignal();
}

void ExpertDevicePage::onGetAllDevicesReceived(bool success, const QString& message, const QVector<Device>& devices)
{
    if (success) {
        loadDevices(devices);
    } else {
        QMessageBox::warning(this, "错误", "获取设备列表失败: " + message);
    }
}

void ExpertDevicePage::loadDevices(const QVector<Device>& devices)
{
    devicesTable->setRowCount(0);
    for (const Device& device : devices) {
        int row = devicesTable->rowCount();
        devicesTable->insertRow(row);

        devicesTable->setItem(row, 0, new QTableWidgetItem(device.deviceName));
        devicesTable->setItem(row, 1, new QTableWidgetItem(device.deviceType));
        devicesTable->setItem(row, 2, new QTableWidgetItem(device.status));
    }
    devicesTable->resizeColumnsToContents();
    devicesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自适应列宽
}

void ExpertDevicePage::onControlDevice()
{
    int row = devicesTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要编辑的设备！");
        return;
    }

    // 获取当前设备信息
    QString name = devicesTable->item(row, 0)->text();
    QString type = devicesTable->item(row, 1)->text();
    QString status = devicesTable->item(row, 2)->text();

    // 控制设备类型
    QString control = QInputDialog::getText(this, "控制指令", "请输入对该设备的控制指令：",
                                            QLineEdit::Normal, status, &ok);
    if (!ok || control.isEmpty()) return;

    QString newStatus = status + ": " + control;

    // 更新表格
    devicesTable->setItem(row, 0, new QTableWidgetItem(name));
    devicesTable->setItem(row, 1, new QTableWidgetItem(type));
    devicesTable->setItem(row, 2, new QTableWidgetItem(newStatus));

    // 编辑设备通过先删除再建立的方式实现
    // emit removeDeviceSignal(name);
    // emit addDeviceSignal(name, type, newStatus);
    emit updateDeviceSignal(name, type, newStatus);
}
