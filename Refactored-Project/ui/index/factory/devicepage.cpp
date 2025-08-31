#include "devicepage.h"
#include <QInputDialog>
#include <QMessageBox>

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
    deviceList->addItem("设备 A - 摄像头 - 正常");
    deviceList->addItem("设备 B - 传感器 - 故障");
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
    QString name = QInputDialog::getText(this, "新增设备", "请输入设备名称：", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString type = QInputDialog::getText(this, "新增设备", "请输入设备类型：", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    deviceList->addItem(name + " - " + type + " - 正常");

    // TODO: 存数据库 + 发送给专家端
}

void DevicePage::onRemoveDevice()
{
    delete deviceList->takeItem(deviceList->currentRow());
}

void DevicePage::onEditDevice()
{
    QListWidgetItem *item = deviceList->currentItem();
    if (!item) return;

    bool ok;
    QString newText = QInputDialog::getText(this, "编辑设备", "修改设备信息：", QLineEdit::Normal, item->text(), &ok);
    if (ok && !newText.isEmpty()) {
        item->setText(newText);
    }

    // TODO: 更新数据库
}

void DevicePage::onViewDevice()
{
    QListWidgetItem *item = deviceList->currentItem();
    if (!item) return;

    QMessageBox::information(this, "设备详情", "设备信息：\n" + item->text());

    // TODO: 打开一个设备详情界面（可扩展为设备实时数据）
}
