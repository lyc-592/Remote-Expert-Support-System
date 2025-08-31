#include "../../ui/index/factory/device_page.h"
#include <QInputDialog>
#include <QMessageBox>
#include "../../server/entities/device.h"

DevicePage::DevicePage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左边布局
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // 顶部按钮
    QHBoxLayout *topLayout = new QHBoxLayout;
    btnAdd = new QPushButton("➕");
    btnRemove = new QPushButton("➖");
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnRemove);
    topLayout->addStretch();
    leftLayout->addLayout(topLayout);

    // 中部设备列表
    deviceList = new QListWidget;
    // 注释掉原来手动添加测试数据的代码，改为从数据库加载
    // deviceList->addItem("设备 A - 摄像头 - 正常");
    // deviceList->addItem("设备 B - 传感器 - 故障");
    // loadDevicesFromDb();  // 新增，从数据库加载设备数据
    leftLayout->addWidget(deviceList);

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

    emit addDeviceSignal(name, type);
}

/* 有误：没有直接删除 */
void DevicePage::onRemoveDevice()
{
    delete deviceList->takeItem(deviceList->currentRow());
}

void DevicePage::onEditDevice()
{
    QListWidgetItem *item = deviceList->currentItem();
    if (!item) return;

    // 获取当前项目文本作为默认值
    QString originalText = item->text();
    bool ok;

    // 显示输入对话框，提示用户按照指定格式输入
    QString newText = QInputDialog::getText(
        this,
        "编辑设备",
        "请按照格式修改设备信息：\n设备名称 - 设备类型 - 状态\n（状态可选：正常/故障/离线）",
        QLineEdit::Normal,
        originalText,
        &ok
        );

    if (ok && !newText.isEmpty()) {
        // 拆分新输入的文本为三部分：名称、类型、状态
        QStringList parts = newText.split(" - ");
        if (parts.size() >= 3) {
            // 更新界面显示
            item->setText(newText);

            // 解析各字段值
            QString newName = parts[0];
            QString newType = parts[1];
            QString newStatus = parts[2];

            // 解析原始文本获取旧名称（用于查询条件）
            QStringList originalParts = originalText.split(" - ");
            if (originalParts.isEmpty()) return;
            QString oldName = originalParts[0];

            // // 连接数据库并更新记录
            // if (initDb()) {
            //     QSqlQuery query;
            //     // 准备更新语句，包含status字段
            //     query.prepare("UPDATE device SET "
            //                   "device_name = :device_name, "
            //                   "device_type = :device_type, "
            //                   "status = :status "
            //                   "WHERE device_name = :old_name");

            //     // 绑定所有参数
            //     query.bindValue(":device_name", newName);
            //     query.bindValue(":device_type", newType);
            //     query.bindValue(":status", newStatus);
            //     query.bindValue(":old_name", oldName);

            //     if (!query.exec()) {
            //         QMessageBox::critical(this, "更新错误",
            //                               "设备数据更新失败：" + query.lastError().text());
            //     } else if (query.numRowsAffected() == 0) {
            //         QMessageBox::warning(this, "更新警告",
            //                              "未找到匹配的设备记录，可能已被修改");
            //     }

            //     QSqlDatabase::database().close();
            // }
        } else {
            QMessageBox::warning(this, "格式错误",
                                 "输入格式不正确，请使用：设备名称 - 设备类型 - 状态");
        }
    }
}


void DevicePage::onViewDevice()
{
    QListWidgetItem *item = deviceList->currentItem();
    if (!item) return;

    QMessageBox::information(this, "设备详情", "设备信息：\n" + item->text());

    // TODO: 打开一个设备详情界面（可扩展为设备实时数据）
}

void DevicePage::loadDevices(const QVector<Device>& devices)
{
    deviceList->clear();
    for (const Device& device : devices) {
        int deviceId = device.deviceId;
        QString deviceName = device.deviceName;
        QString deviceType = device.deviceType;
        QString itemText = QString("%1 - %2 - 正常").arg(deviceName).arg(deviceType);
        deviceList->addItem(itemText);
    }
}
