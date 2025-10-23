#include "../../ui/index/factory/device_page.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QWidget>
#include "../../server/entities/device.h"

DevicePage::DevicePage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左边布局
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // 顶部按钮
    QHBoxLayout *topLayout = new QHBoxLayout;
    btnAdd = new QPushButton("添加设备");
    btnRemove = new QPushButton("删除设备");
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnRemove);
    topLayout->addStretch();
    leftLayout->addLayout(topLayout);

    // 新-中间表格
    deviceTable = new QTableWidget(this);
    deviceTable->setColumnCount(3);
    deviceTable->setHorizontalHeaderLabels({"设备名称", "设备类型", "状态"});
    deviceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自适应列宽
    deviceTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    deviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可直接编辑
    leftLayout->addWidget(deviceTable);

    // 右边按钮区
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QPushButton *btnEdit = new QPushButton("编辑");
    QPushButton *btnView = new QPushButton("查看");
    rightLayout->addStretch();
    rightLayout->addWidget(btnEdit);
    rightLayout->addWidget(btnView);
    rightLayout->addStretch();

    // 拼接主布局
    mainLayout->addLayout(leftLayout, 4);
    mainLayout->addLayout(rightLayout, 1);

    // 信号槽
    connect(btnAdd, &QPushButton::clicked, this, &DevicePage::onAddDevice);
    connect(btnRemove, &QPushButton::clicked, this, &DevicePage::onRemoveDevice);
    connect(btnEdit, &QPushButton::clicked, this, &DevicePage::onEditDevice);
    connect(btnView, &QPushButton::clicked, this, &DevicePage::onViewDevice);
}

void DevicePage::onAddDevice()
{
    bool ok;

    // 获取设备名称
    QString name = QInputDialog::getText(this, "新增设备", "请输入设备名称：", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    // 获取设备类型
    QString type = QInputDialog::getText(this, "新增设备", "请输入设备类型：", QLineEdit::Normal, "", &ok);
    if (!ok || type.isEmpty()) return;

    // 选择设备状态（使用下拉列表方式更规范）
    QStringList statusOptions = {"正常", "故障", "离线"};
    QString status = QInputDialog::getItem(
        this,
        "选择状态",
        "请选择设备状态：",
        statusOptions,
        0,  // 默认选中第一个选项（正常）
        false,  // 不允许编辑
        &ok
        );
    if (!ok) return;

    // 在表格中添加新行
    int row = deviceTable->rowCount();
    deviceTable->insertRow(row);

    deviceTable->setItem(row, 0, new QTableWidgetItem(name));
    deviceTable->setItem(row, 1, new QTableWidgetItem(type));
    deviceTable->setItem(row, 2, new QTableWidgetItem(status));

    emit addDeviceSignal(name, type, status);
}

void DevicePage::onRemoveDevice() {
    int row = deviceTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要删除的设备！");
        return;
    }
    // 获取设备名称
    QString deviceName = deviceTable->item(row, 0)->text();
    qDebug() << deviceName;
    int hyphenPos = deviceName.indexOf(' ');
    if (hyphenPos != -1) { // QString::indexOf() 在未找到时返回 -1
        deviceName = deviceName.left(hyphenPos);
    }
    int ret = QMessageBox::question(this, "确认删除",
                                    "确定要删除设备 " + deviceName + " 吗？");
    if (ret == QMessageBox::Yes) {
        emit removeDeviceSignal(deviceName); // 连接到MainWindow，再由MainWindow调用client->removeDevice()
    }
}

void DevicePage::onEditDevice()
{
    int row = deviceTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要编辑的设备！");
        return;
    }

    // 获取当前设备信息
    QString name = deviceTable->item(row, 0)->text();
    QString type = deviceTable->item(row, 1)->text();
    QString status = deviceTable->item(row, 2)->text();

    // 获取新设备类型
    QString newType = QInputDialog::getText(this, "编辑设备", "请输入设备类型：",
                                            QLineEdit::Normal, type, &ok);
    if (!ok || newType.isEmpty()) return;

    // 选择新设备状态
    QStringList statusOptions = {"正常", "故障", "离线"};
    int currentIndex = statusOptions.indexOf(status);
    if (currentIndex < 0) currentIndex = 0;

    QString newStatus = QInputDialog::getItem(
        this,
        "选择状态",
        "请选择设备状态：",
        statusOptions,
        currentIndex,
        false,
        &ok
        );
    if (!ok) return;

    // 更新表格
    deviceTable->setItem(row, 0, new QTableWidgetItem(name));
    deviceTable->setItem(row, 1, new QTableWidgetItem(newType));
    deviceTable->setItem(row, 2, new QTableWidgetItem(newStatus));

    // 编辑设备通过先删除再建立的方式实现
    emit removeDeviceSignal(name);
    emit addDeviceSignal(name, newType, newStatus);
}


void DevicePage::onViewDevice()
{
    // 新方案
    int row = deviceTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要查看的设备！");
        return;
    }

    // 获取设备信息
    QString name = deviceTable->item(row, 0)->text();
    QString type = deviceTable->item(row, 1)->text();
    QString status = deviceTable->item(row, 2)->text();

    // 显示设备详情
    QString details = QString("设备名称: %1\n设备类型: %2\n状态: %3")
                          .arg(name)
                          .arg(type)
                          .arg(status);

    QMessageBox::information(this, "设备详情", details);

    // TODO: 打开一个设备详情界面（可扩展为设备实时数据）
}

void DevicePage::loadDevices(const QVector<Device>& devices)
{
    // 新-清空表格
    deviceTable->setRowCount(0);

    // 添加设备数据到表格
    for (const Device& device : devices) {
        int row = deviceTable->rowCount();
        deviceTable->insertRow(row);

        deviceTable->setItem(row, 0, new QTableWidgetItem(device.deviceName));
        deviceTable->setItem(row, 1, new QTableWidgetItem(device.deviceType));
        deviceTable->setItem(row, 2, new QTableWidgetItem(device.status));
    }
}
