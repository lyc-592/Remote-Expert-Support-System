#include "expert_record.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QFont>

ExpertRecord::ExpertRecord(int orderId, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), orderId(orderId), dbManager(dbManager)
{
    setWindowTitle("现场信息详情");
    resize(600, 500);

    setupUI();
    loadRecordDetails();
}

ExpertRecord::~ExpertRecord()
{
    // 清理资源
}

void ExpertRecord::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 创建表单显示详细信息
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // 初始化标签
    orderIdLabel = new QLabel;
    faultTypeLabel = new QLabel;
    solutionLabel = new QLabel;
    uploadTimeLabel = new QLabel;

    // 设置标签可以换行
    solutionLabel->setWordWrap(true);
    solutionLabel->setMinimumWidth(400);

    // 添加到表单
    formLayout->addRow("工单号:", orderIdLabel);
    formLayout->addRow("故障类型:", faultTypeLabel);
    formLayout->addRow("解决方案:", solutionLabel);
    formLayout->addRow("上传时间:", uploadTimeLabel);

    mainLayout->addLayout(formLayout);

    // 文件查看按钮区域
    QFrame *buttonFrame = new QFrame;
    buttonFrame->setFrameStyle(QFrame::Box);
    buttonFrame->setLineWidth(1);

    QVBoxLayout *buttonFrameLayout = new QVBoxLayout(buttonFrame);
    buttonFrameLayout->setSpacing(10);

    QLabel *fileSectionLabel = new QLabel("相关文件");
    QFont sectionFont;
    sectionFont.setBold(true);
    sectionFont.setPointSize(12);
    fileSectionLabel->setFont(sectionFont);
    buttonFrameLayout->addWidget(fileSectionLabel);

    // 视频查看按钮
    viewVideoButton = new QPushButton("查看现场视频");
    viewVideoButton->setEnabled(false);
    connect(viewVideoButton, &QPushButton::clicked, this, &ExpertRecord::viewVideo);
    buttonFrameLayout->addWidget(viewVideoButton);

    // 文件查看按钮
    viewFileButton = new QPushButton("查看相关文档");
    viewFileButton->setEnabled(false);
    connect(viewFileButton, &QPushButton::clicked, this, &ExpertRecord::viewFile);
    buttonFrameLayout->addWidget(viewFileButton);

    // 数据查看按钮
    viewDataButton = new QPushButton("查看设备数据");
    viewDataButton->setEnabled(false);
    connect(viewDataButton, &QPushButton::clicked, this, &ExpertRecord::viewData);
    buttonFrameLayout->addWidget(viewDataButton);

    mainLayout->addWidget(buttonFrame);

    // 添加关闭按钮
    QHBoxLayout *closeButtonLayout = new QHBoxLayout;
    closeButtonLayout->addStretch();

    QPushButton *closeButton = new QPushButton("关闭");
    closeButton->setMinimumSize(100, 35);
    connect(closeButton, &QPushButton::clicked, this, &ExpertRecord::accept);
    closeButtonLayout->addWidget(closeButton);
    closeButtonLayout->addStretch();

    mainLayout->addLayout(closeButtonLayout);
}

void ExpertRecord::loadRecordDetails()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    // 查询现场记录信息
    QString queryStr = R"(
        SELECT
            ho.history_order_id,
            r.fault_type,
            r.solution,
            r.upload_time,
            r.video_path,
            r.file_path,
            r.device_status_data_path,
            d.device_name
        FROM record r
        LEFT JOIN history_order ho ON r.order_id = ho.history_order_id
        LEFT JOIN device d ON ho.device_id = d.device_id
        WHERE r.order_id = :order_id
    )";

    QSqlQuery query = dbManager->createQuery();
    query.prepare(queryStr);
    query.bindValue(":order_id", orderId);

    if (!query.exec()) {
        QMessageBox::critical(this, "错误", "无法获取现场记录: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::information(this, "提示", "该工单没有现场记录");
        return;
    }

    // 设置标签文本
    orderIdLabel->setText(query.value("history_order_id").toString());
    faultTypeLabel->setText(query.value("fault_type").toString());
    solutionLabel->setText(query.value("solution").toString());
    uploadTimeLabel->setText(query.value("upload_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    // 保存文件路径
    videoPath = query.value("video_path").toString();
    filePath = query.value("file_path").toString();
    dataPath = query.value("device_status_data_path").toString();

    // 启用按钮（如果文件存在）
    viewVideoButton->setEnabled(!videoPath.isEmpty() && QFileInfo::exists(videoPath));
    viewFileButton->setEnabled(!filePath.isEmpty() && QFileInfo::exists(filePath));
    viewDataButton->setEnabled(!dataPath.isEmpty() && QFileInfo::exists(dataPath));

    // 设置重要信息的字体样式
    QFont boldFont;
    boldFont.setBold(true);
    orderIdLabel->setFont(boldFont);
    faultTypeLabel->setFont(boldFont);
}

// 辅助函数
void ExpertRecord::viewVideo()
{
    if (!videoPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));
    } else {
        QMessageBox::warning(this, "错误", "视频文件路径为空");
    }
}

void ExpertRecord::viewFile()
{
    if (!filePath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    } else {
        QMessageBox::warning(this, "错误", "文件路径为空");
    }
}

void ExpertRecord::viewData()
{
    if (!dataPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dataPath));
    } else {
        QMessageBox::warning(this, "错误", "数据文件路径为空");
    }
}
