#include "order_detail_dialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFont>

OrderDetailDialog::OrderDetailDialog(int orderId, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), orderId(orderId), dbManager(dbManager)
{
    setWindowTitle("工单详情");
    resize(500, 400);

    setupUI();
    loadOrderDetails();
}

OrderDetailDialog::~OrderDetailDialog()
{
    // 清理资源
}

void OrderDetailDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // 创建表单显示详细信息
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(10);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // 初始化标签
    orderIdLabel = new QLabel;
    deviceNameLabel = new QLabel;
    deviceTypeLabel = new QLabel;
    createTimeLabel = new QLabel;
    endTimeLabel = new QLabel;
    statusLabel = new QLabel;
    expertLabel = new QLabel;
    faultLabel = new QLabel;

    // 设置标签可以换行
    faultLabel->setWordWrap(true);
    faultLabel->setMinimumWidth(300);

    // 添加到表单
    formLayout->addRow("工单号:", orderIdLabel);
    formLayout->addRow("设备名称:", deviceNameLabel);
    formLayout->addRow("设备类型:", deviceTypeLabel);
    formLayout->addRow("创建时间:", createTimeLabel);
    formLayout->addRow("完成时间:", endTimeLabel);
    formLayout->addRow("故障描述:", faultLabel);
    formLayout->addRow("状态:", statusLabel);
    // formLayout->addRow("联系人:", contactLabel);
    formLayout->addRow("处理专家:", expertLabel);

    mainLayout->addLayout(formLayout);

    // 添加关闭按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    QPushButton *closeButton = new QPushButton("关闭");
    connect(closeButton, &QPushButton::clicked, this, &OrderDetailDialog::accept);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
}

void OrderDetailDialog::loadOrderDetails()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    // 使用 DatabaseManager 提供的方法
    QSqlQuery query = dbManager->getHistoryOrderDetails(orderId);

    if (!query.isActive()) {
        QMessageBox::critical(this, "错误", "无法获取工单详情: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::information(this, "提示", "未找到对应的工单详情");
        return;
    }

    // 设置标签文本
    orderIdLabel->setText(query.value("history_order_id").toString());
    deviceNameLabel->setText(query.value("device_name").toString());
    deviceTypeLabel->setText(query.value("device_type").toString());
    createTimeLabel->setText(query.value("order_create_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    endTimeLabel->setText(query.value("order_end_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    faultLabel->setText(query.value("fault").toString());

    QString statusText = query.value("order_status").toInt() == 0 ? "已完成" : "已拒绝";
    statusLabel->setText(statusText);

    // contactLabel->setText(query.value("contact_info").toString());
    expertLabel->setText(query.value("expert_info").toString());

    // 设置重要信息的字体样式
    QFont boldFont;
    boldFont.setBold(true);
    deviceNameLabel->setFont(boldFont);
    statusLabel->setFont(boldFont);
}
