#include "record_dialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

// Record_Dialog::Record_Dialog() {}

RecordDialog::RecordDialog(int orderId, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), orderId(orderId), dbManager(dbManager)
{
    setWindowTitle("现场记录");
    resize(600, 400);

    // 设置最小大小避免布局问题
    setMinimumSize(400, 300);

    // 先设置一个简单布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *loadingLabel = new QLabel("加载中...");
    loadingLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(loadingLabel);

    // setupUI();
    loadRecordDetails();
}

RecordDialog::~RecordDialog()
{
    // 清理资源
}

// void RecordDialog::setupUI()
// {
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setSpacing(10);
//     mainLayout->setContentsMargins(15, 15, 15, 15);

//     // 创建表单显示现场记录
//     QFormLayout *formLayout = new QFormLayout;
//     formLayout->setSpacing(10);
//     formLayout->setLabelAlignment(Qt::AlignRight);

//     // 初始化标签
//     faultTypeLabel = new QLabel;
//     solutionLabel = new QLabel;
//     uploadTimeLabel = new QLabel;

//     // 设置标签可以换行
//     solutionLabel->setWordWrap(true);
//     solutionLabel->setMinimumWidth(400);

//     // 添加到表单
//     formLayout->addRow("故障类型:", faultTypeLabel);
//     formLayout->addRow("解决方案:", solutionLabel);
//     formLayout->addRow("上传时间:", uploadTimeLabel);

//     mainLayout->addLayout(formLayout);

//     // 文件操作按钮
//     QHBoxLayout *videoLayout = new QHBoxLayout;
//     QLabel *videoLabel = new QLabel("录制视频:");
//     videoButton = new QPushButton("查看视频");
//     videoButton->setEnabled(false);
//     connect(videoButton, &QPushButton::clicked, this, &RecordDialog::viewVideo);
//     videoLayout->addWidget(videoLabel);
//     videoLayout->addWidget(videoButton);
//     videoLayout->addStretch();

//     QHBoxLayout *fileLayout = new QHBoxLayout;
//     QLabel *fileLabel = new QLabel("相关文件:");
//     fileButton = new QPushButton("查看文件");
//     fileButton->setEnabled(false);
//     connect(fileButton, &QPushButton::clicked, this, &RecordDialog::viewFile);
//     fileLayout->addWidget(fileLabel);
//     fileLayout->addWidget(fileButton);
//     fileLayout->addStretch();

//     QHBoxLayout *dataLayout = new QHBoxLayout;
//     QLabel *dataLabel = new QLabel("设备数据:");
//     dataButton = new QPushButton("查看数据");
//     dataButton->setEnabled(false);
//     connect(dataButton, &QPushButton::clicked, this, &RecordDialog::viewData);
//     dataLayout->addWidget(dataLabel);
//     dataLayout->addWidget(dataButton);
//     dataLayout->addStretch();

//     mainLayout->addLayout(videoLayout);
//     mainLayout->addLayout(fileLayout);
//     mainLayout->addLayout(dataLayout);

//     // 添加关闭按钮
//     QHBoxLayout *buttonLayout = new QHBoxLayout;
//     buttonLayout->addStretch();

//     QPushButton *closeButton = new QPushButton("关闭");
//     connect(closeButton, &QPushButton::clicked, this, &RecordDialog::accept);
//     closeButton->setFixedSize(20, 8);
//     buttonLayout->addWidget(closeButton, 1);

//     mainLayout->addLayout(buttonLayout);
// }

// void RecordDialog::loadRecordDetails()
// {
//     if (!dbManager->isConnected()) {
//         QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
//         return;
//     }

//     // 获取数据库连接
//     QSqlDatabase db = dbManager->getDatabase();

//     // 从数据库获取现场记录
//     QSqlQuery query(db);
//     query.prepare("SELECT * FROM record WHERE order_id = :order_id");
//     query.bindValue(":order_id", orderId);

//     if (!query.exec()) {
//         QMessageBox::critical(this, "错误", "无法获取现场记录: " + query.lastError().text());
//         return;
//     }

//     if (query.next()) {
//         // 设置标签文本
//         faultTypeLabel->setText(query.value("fault_type").toString());
//         solutionLabel->setText(query.value("solution").toString());
//         uploadTimeLabel->setText(query.value("upload_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

//         // 保存文件路径（根据实际存储调整）
//         videoPath = query.value("video_path").toString();
//         filePath = query.value("file_path").toString();
//         dataPath = query.value("device_status_data_path").toString();

//         // 启用按钮（如果文件存在）
//         videoButton->setEnabled(!videoPath.isEmpty() && QFileInfo::exists(videoPath));
//         fileButton->setEnabled(!filePath.isEmpty() && QFileInfo::exists(filePath));
//         dataButton->setEnabled(!dataPath.isEmpty() && QFileInfo::exists(dataPath));
//     } else {
//         QLabel *noRecordLabel = new QLabel("该工单没有现场记录");
//         noRecordLabel->setAlignment(Qt::AlignCenter);
//         QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
//         if (layout) {
//             layout->insertWidget(0, noRecordLabel);
//         }
//     }
// }

// void RecordDialog::loadRecordDetails()
// {
//     if (!dbManager->isConnected()) {
//         QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
//         return;
//     }

//     // 清空现有内容
//     QLayoutItem* child;
//     while ((child = this->layout()->takeAt(0)) != nullptr) {
//         delete child->widget();
//         delete child;
//     }

//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setSpacing(15);
//     mainLayout->setContentsMargins(20, 20, 20, 20);

//     // 查询SQL保持不变
//     QString queryStr = R"(
//         SELECT
//             ho.history_order_id,
//             ho.order_create_time,
//             ho.order_end_time,
//             ho.order_status,
//             d.device_name,
//             d.device_type,
//             u_factory.user_account as factory_account,
//             u_factory.user_contact as factory_contact,
//             u_expert.user_account as expert_account,
//             u_expert.user_contact as expert_contact,
//             r.fault_type,
//             r.solution,
//             r.upload_time,
//             r.video_path,
//             r.file_path,
//             r.device_status_data_path,
//             wo.fault as original_fault
//         FROM history_order ho
//         LEFT JOIN device d ON ho.device_id = d.device_id
//         LEFT JOIN user u_factory ON ho.user1_id = u_factory.user_id
//         LEFT JOIN user u_expert ON ho.user2_id = u_expert.user_id
//         LEFT JOIN record r ON ho.history_order_id = r.order_id
//         LEFT JOIN workorder wo ON ho.device_id = wo.device_id
//         WHERE ho.history_order_id = :order_id
//     )";

//     QSqlQuery query = dbManager->createQuery();
//     query.prepare(queryStr);
//     query.bindValue(":order_id", orderId);

//     if (!query.exec() || !query.next()) {
//         QLabel *noRecordLabel = new QLabel(query.exec() ? "该工单没有找到相关记录" : "查询失败: " + query.lastError().text());
//         noRecordLabel->setAlignment(Qt::AlignCenter);
//         noRecordLabel->setStyleSheet("font-size: 16px; color: #666; padding: 50px;");
//         mainLayout->addWidget(noRecordLabel);
//         return;
//     }

//     // ==================== 工单基本信息区域 ====================
//     QFrame *basicInfoFrame = new QFrame;
//     basicInfoFrame->setObjectName("BasicInfoFrame");
//     basicInfoFrame->setStyleSheet("QFrame#BasicInfoFrame { background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; padding: 15px; }");

//     QGridLayout *basicInfoLayout = new QGridLayout(basicInfoFrame);
//     basicInfoLayout->setSpacing(10);
//     basicInfoLayout->setColumnStretch(1, 1);

//     // 添加基本信息行
//     addInfoRow(basicInfoLayout, 0, "工单号:", query.value("history_order_id").toString(), true);
//     addInfoRow(basicInfoLayout, 1, "设备名称:", query.value("device_name").toString());
//     addInfoRow(basicInfoLayout, 2, "设备类型:", query.value("device_type").toString());
//     addInfoRow(basicInfoLayout, 3, "创建时间:", query.value("order_create_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
//     addInfoRow(basicInfoLayout, 4, "完成时间:", query.value("order_end_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

//     QString statusText = query.value("order_status").toInt() == 0 ? "已完成" : "已拒绝";
//     addInfoRow(basicInfoLayout, 5, "工单状态:", statusText, true);

//     addInfoRow(basicInfoLayout, 6, "工厂用户:", query.value("factory_account").toString() + " (" + query.value("factory_contact").toString() + ")");
//     addInfoRow(basicInfoLayout, 7, "处理专家:", query.value("expert_account").toString() + " (" + query.value("expert_contact").toString() + ")");
//     addInfoRow(basicInfoLayout, 8, "原始故障:", query.value("original_fault").toString());

//     mainLayout->addWidget(basicInfoFrame);

//     // ==================== 现场记录信息区域 ====================
//     if (!query.value("fault_type").isNull()) {
//         QFrame *recordInfoFrame = new QFrame;
//         recordInfoFrame->setObjectName("RecordInfoFrame");
//         recordInfoFrame->setStyleSheet("QFrame#RecordInfoFrame { background-color: #e8f4fd; border: 1px solid #b8d6f3; border-radius: 8px; padding: 15px; margin-top: 20px; }");

//         QVBoxLayout *recordLayout = new QVBoxLayout(recordInfoFrame);

//         QLabel *recordTitle = new QLabel("📝 现场处理记录");
//         recordTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #0d6efd; margin-bottom: 15px;");
//         recordLayout->addWidget(recordTitle);

//         QGridLayout *recordInfoLayout = new QGridLayout;
//         recordInfoLayout->setSpacing(10);
//         recordInfoLayout->setColumnStretch(1, 1);

//         // 故障类型和解决方案
//         addInfoRow(recordInfoLayout, 0, "故障类型:", query.value("fault_type").toString());

//         QLabel *solutionLabel = new QLabel(query.value("solution").toString());
//         solutionLabel->setWordWrap(true);
//         solutionLabel->setStyleSheet("background-color: white; padding: 8px; border: 1px solid #dee2e6; border-radius: 4px;");
//         recordInfoLayout->addWidget(new QLabel("解决方案:"), 1, 0);
//         recordInfoLayout->addWidget(solutionLabel, 1, 1);

//         addInfoRow(recordInfoLayout, 2, "上传时间:", query.value("upload_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

//         recordLayout->addLayout(recordInfoLayout);
//         mainLayout->addWidget(recordInfoFrame);

//         // ==================== 文件操作按钮区域 ====================
//         // 保存文件路径到成员变量（这样您的辅助函数就能使用了）
//         videoPath = query.value("video_path").toString();
//         filePath = query.value("file_path").toString();
//         dataPath = query.value("device_status_data_path").toString();

//         QFrame *buttonFrame = new QFrame;
//         buttonFrame->setObjectName("ButtonFrame");
//         buttonFrame->setStyleSheet("QFrame#ButtonFrame { background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; padding: 15px; margin-top: 20px; }");

//         QVBoxLayout *buttonLayout = new QVBoxLayout(buttonFrame);

//         QLabel *fileTitle = new QLabel("📁 相关文件");
//         fileTitle->setStyleSheet("font-size: 14px; font-weight: bold; margin-bottom: 15px;");
//         buttonLayout->addWidget(fileTitle);

//         // 使用统一的按钮创建方法
//         if (!videoPath.isEmpty() && QFileInfo::exists(videoPath)) {
//             QPushButton *videoBtn = createActionButton("🎥 查看录制视频", [this]() { viewVideo(); });
//             buttonLayout->addWidget(videoBtn);
//         }

//         if (!filePath.isEmpty() && QFileInfo::exists(filePath)) {
//             QPushButton *fileBtn = createActionButton("📄 查看相关文件", [this]() { viewFile(); });
//             buttonLayout->addWidget(fileBtn);
//         }

//         if (!dataPath.isEmpty() && QFileInfo::exists(dataPath)) {
//             QPushButton *dataBtn = createActionButton("📊 查看设备数据", [this]() { viewData(); });
//             buttonLayout->addWidget(dataBtn);
//         }

//         // 如果没有可用文件
//         if (buttonLayout->count() == 1) { // 只有标题
//             QLabel *noFileLabel = new QLabel("暂无相关文件");
//             noFileLabel->setAlignment(Qt::AlignCenter);
//             noFileLabel->setStyleSheet("color: #6c757d; font-style: italic;");
//             buttonLayout->addWidget(noFileLabel);
//         }

//         mainLayout->addWidget(buttonFrame);
//     } else {
//         QLabel *noRecordLabel = new QLabel("该工单暂无现场处理记录");
//         noRecordLabel->setAlignment(Qt::AlignCenter);
//         noRecordLabel->setStyleSheet("font-size: 14px; color: #6c757d; padding: 20px;");
//         mainLayout->addWidget(noRecordLabel);
//     }

//     mainLayout->addStretch();
// }

void RecordDialog::loadRecordDetails()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    // 清空现有内容
    QLayoutItem* child;
    while ((child = this->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 查询现场记录和工单的完整信息
    QString queryStr = R"(
        SELECT
            ho.history_order_id,
            ho.order_create_time,
            ho.order_end_time,
            ho.order_status,
            d.device_name,
            d.device_type,
            d.device_id,
            u_factory.user_account as factory_account,
            u_factory.user_contact as factory_contact,
            u_expert.user_account as expert_account,
            u_expert.user_contact as expert_contact,
            u_expert.expert_skill as expert_skill,
            r.fault_type,
            r.solution,
            r.upload_time,
            r.video_path,
            r.file_path,
            r.device_status_data_path,
            wo.fault as original_fault,
            wo.order_create_time as wo_create_time,
            wo.predict_time as wo_predict_time
        FROM history_order ho
        LEFT JOIN device d ON ho.device_id = d.device_id
        LEFT JOIN user u_factory ON ho.user1_id = u_factory.user_id
        LEFT JOIN user u_expert ON ho.user2_id = u_expert.user_id
        LEFT JOIN record r ON ho.history_order_id = r.order_id
        LEFT JOIN workorder wo ON ho.device_id = wo.device_id
        WHERE ho.history_order_id = :order_id
    )";

    QSqlQuery query = dbManager->createQuery();
    query.prepare(queryStr);
    query.bindValue(":order_id", orderId);

    if (!query.exec() || !query.next()) {
        QLabel *noRecordLabel = new QLabel(query.exec() ? "该工单没有找到相关记录" : "查询失败: " + query.lastError().text());
        noRecordLabel->setAlignment(Qt::AlignCenter);
        noRecordLabel->setStyleSheet("font-size: 16px; color: #666; padding: 50px;");
        mainLayout->addWidget(noRecordLabel);

        // 添加关闭按钮
        QPushButton *closeButton = new QPushButton("关闭");
        connect(closeButton, &QPushButton::clicked, this, &RecordDialog::accept);
        mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

        return;
    }

    // ==================== 工单基本信息区域 ====================
    QFrame *basicInfoFrame = new QFrame;
    basicInfoFrame->setObjectName("BasicInfoFrame");
    basicInfoFrame->setStyleSheet("QFrame#BasicInfoFrame { "
                                  "background-color: #f8f9fa; "
                                  "border: 2px solid #dee2e6; "
                                  "border-radius: 10px; "
                                  "padding: 20px; "
                                  "margin-bottom: 15px;"
                                  "}");

    QVBoxLayout *basicFrameLayout = new QVBoxLayout(basicInfoFrame);

    // 标题
    QLabel *basicTitle = new QLabel("📋 工单基本信息");
    basicTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin-bottom: 15px;");
    basicFrameLayout->addWidget(basicTitle);

    QGridLayout *basicInfoLayout = new QGridLayout;
    basicInfoLayout->setSpacing(12);
    basicInfoLayout->setColumnStretch(1, 1);

    // 添加工单基本信息
    addInfoRow(basicInfoLayout, 0, "工单编号:", query.value("history_order_id").toString(), true);
    addInfoRow(basicInfoLayout, 1, "设备名称:", query.value("device_name").toString());
    addInfoRow(basicInfoLayout, 2, "设备类型:", query.value("device_type").toString());
    addInfoRow(basicInfoLayout, 3, "设备ID:", query.value("device_id").toString());
    addInfoRow(basicInfoLayout, 4, "创建时间:", query.value("order_create_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    addInfoRow(basicInfoLayout, 5, "完成时间:", query.value("order_end_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    QString statusText = query.value("order_status").toInt() == 0 ? "✅ 已完成" : "❌ 已拒绝";
    QString statusColor = query.value("order_status").toInt() == 0 ? "#28a745" : "#dc3545";
    addInfoRow(basicInfoLayout, 6, "工单状态:", statusText, true, statusColor);

    addInfoRow(basicInfoLayout, 7, "工厂用户:", query.value("factory_account").toString() + "\n(" + query.value("factory_contact").toString() + ")");
    addInfoRow(basicInfoLayout, 8, "处理专家:", query.value("expert_account").toString() + "\n(" + query.value("expert_contact").toString() + ")");
    addInfoRow(basicInfoLayout, 9, "专家技能:", query.value("expert_skill").toString());

    // 原始故障信息
    QLabel *faultLabel = new QLabel(query.value("original_fault").toString());
    faultLabel->setWordWrap(true);
    faultLabel->setStyleSheet("background-color: white; padding: 10px; border: 1px solid #dee2e6; border-radius: 6px;");
    basicInfoLayout->addWidget(new QLabel("原始故障描述:"), 10, 0, Qt::AlignRight | Qt::AlignTop);
    basicInfoLayout->addWidget(faultLabel, 10, 1);

    basicFrameLayout->addLayout(basicInfoLayout);
    mainLayout->addWidget(basicInfoFrame);

    // ==================== 现场处理记录区域 ====================
    if (!query.value("fault_type").isNull()) {
        QFrame *recordInfoFrame = new QFrame;
        recordInfoFrame->setObjectName("RecordInfoFrame");
        recordInfoFrame->setStyleSheet("QFrame#RecordInfoFrame { "
                                       "background-color: #e8f5e8; "
                                       "border: 2px solid #28a745; "
                                       "border-radius: 10px; "
                                       "padding: 20px; "
                                       "margin: 15px 0;"
                                       "}");

        QVBoxLayout *recordFrameLayout = new QVBoxLayout(recordInfoFrame);

        // 标题
        QLabel *recordTitle = new QLabel("🔧 现场处理记录");
        recordTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #28a745; margin-bottom: 15px;");
        recordFrameLayout->addWidget(recordTitle);

        QGridLayout *recordInfoLayout = new QGridLayout;
        recordInfoLayout->setSpacing(12);
        recordInfoLayout->setColumnStretch(1, 1);

        // 故障类型
        addInfoRow(recordInfoLayout, 0, "诊断故障类型:", query.value("fault_type").toString(), true, "#dc3545");

        // 解决方案
        QLabel *solutionLabel = new QLabel(query.value("solution").toString());
        solutionLabel->setWordWrap(true);
        solutionLabel->setStyleSheet(R"(
            background-color: white;
            padding: 12px;
            border: 1px solid #dee2e6;
            border-radius: 6px;
            border-left: 4px solid #28a745;
        )");
        recordInfoLayout->addWidget(new QLabel("处理解决方案:"), 1, 0, Qt::AlignRight | Qt::AlignTop);
        recordInfoLayout->addWidget(solutionLabel, 1, 1);

        // 上传时间
        addInfoRow(recordInfoLayout, 2, "记录上传时间:", query.value("upload_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        recordFrameLayout->addLayout(recordInfoLayout);
        mainLayout->addWidget(recordInfoFrame);

        // ==================== 文件操作区域 ====================
        QFrame *fileFrame = new QFrame;
        fileFrame->setObjectName("FileFrame");
        fileFrame->setStyleSheet("QFrame#FileFrame { "
                                 "background-color: #e8f4fd; "
                                 "border: 2px solid #0d6efd; "
                                 "border-radius: 10px; "
                                 "padding: 20px; "
                                 "margin: 15px 0;"
                                 "}");

        QVBoxLayout *fileFrameLayout = new QVBoxLayout(fileFrame);

        // 标题
        QLabel *fileTitle = new QLabel("📁 相关文件资料");
        fileTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #0d6efd; margin-bottom: 15px;");
        fileFrameLayout->addWidget(fileTitle);

        QVBoxLayout *fileButtonLayout = new QVBoxLayout;
        fileButtonLayout->setSpacing(8);

        // 保存文件路径
        videoPath = query.value("video_path").toString();
        filePath = query.value("file_path").toString();
        dataPath = query.value("device_status_data_path").toString();

        // 创建文件按钮
        bool hasFiles = false;
        if (!videoPath.isEmpty() && QFileInfo::exists(videoPath)) {
            QPushButton *videoBtn = createActionButton("🎥 查看现场录制视频", [this]() { viewVideo(); });
            videoBtn->setToolTip("视频路径: " + videoPath);
            fileButtonLayout->addWidget(videoBtn);
            hasFiles = true;
        }

        if (!filePath.isEmpty() && QFileInfo::exists(filePath)) {
            QPushButton *fileBtn = createActionButton("📄 查看相关文档文件", [this]() { viewFile(); });
            fileBtn->setToolTip("文件路径: " + filePath);
            fileButtonLayout->addWidget(fileBtn);
            hasFiles = true;
        }

        if (!dataPath.isEmpty() && QFileInfo::exists(dataPath)) {
            QPushButton *dataBtn = createActionButton("📊 查看设备状态数据", [this]() { viewData(); });
            dataBtn->setToolTip("数据路径: " + dataPath);
            fileButtonLayout->addWidget(dataBtn);
            hasFiles = true;
        }

        if (!hasFiles) {
            QLabel *noFileLabel = new QLabel("暂无相关文件资料");
            noFileLabel->setAlignment(Qt::AlignCenter);
            noFileLabel->setStyleSheet("color: #6c757d; font-style: italic; padding: 20px;");
            fileButtonLayout->addWidget(noFileLabel);
        }

        fileFrameLayout->addLayout(fileButtonLayout);
        mainLayout->addWidget(fileFrame);
    } else {
        // 没有现场记录的情况
        QFrame *noRecordFrame = new QFrame;
        noRecordFrame->setObjectName("NoRecordFrame");
        noRecordFrame->setStyleSheet("QFrame#NoRecordFrame { "
                                     "background-color: #fff3cd; "
                                     "border: 2px solid #ffc107; "
                                     "border-radius: 10px; "
                                     "padding: 30px; "
                                     "margin: 15px 0;"
                                     "}");

        QVBoxLayout *noRecordLayout = new QVBoxLayout(noRecordFrame);

        QLabel *noRecordIcon = new QLabel("📋");
        noRecordIcon->setAlignment(Qt::AlignCenter);
        noRecordIcon->setStyleSheet("font-size: 32px; margin-bottom: 10px;");

        QLabel *noRecordText = new QLabel("该工单暂无现场处理记录");
        noRecordText->setAlignment(Qt::AlignCenter);
        noRecordText->setStyleSheet("font-size: 16px; color: #856404; font-weight: bold;");

        QLabel *noRecordHint = new QLabel("专家尚未提交现场处理记录");
        noRecordHint->setAlignment(Qt::AlignCenter);
        noRecordHint->setStyleSheet("font-size: 14px; color: #6c757d; margin-top: 5px;");

        noRecordLayout->addWidget(noRecordIcon);
        noRecordLayout->addWidget(noRecordText);
        noRecordLayout->addWidget(noRecordHint);

        mainLayout->addWidget(noRecordFrame);
    }

    // ==================== 关闭按钮区域 ====================
    QFrame *closeFrame = new QFrame;
    closeFrame->setStyleSheet("background: transparent; margin-top: 20px;");

    QHBoxLayout *closeLayout = new QHBoxLayout(closeFrame);
    closeLayout->addStretch();

    QPushButton *closeButton = new QPushButton("关闭");
    connect(closeButton, &QPushButton::clicked, this, &RecordDialog::accept);

    closeLayout->addWidget(closeButton);
    closeLayout->addStretch();

    mainLayout->addWidget(closeFrame);
    mainLayout->addStretch();
}

// 辅助函数：添加信息行
// void RecordDialog::addInfoRow(QGridLayout *layout, int row, const QString &labelText,
//                               const QString &valueText, bool bold)
// {
//     QLabel *label = new QLabel(labelText);
//     label->setStyleSheet("font-weight: bold; color: #495057;");

//     QLabel *value = new QLabel(valueText);
//     value->setStyleSheet(bold ? "font-weight: bold; color: #0d6efd;" : "color: #6c757d;");
//     value->setWordWrap(true);

//     layout->addWidget(label, row, 0, Qt::AlignRight | Qt::AlignTop);
//     layout->addWidget(value, row, 1);
// }

// 增强的addInfoRow函数，支持颜色设置
void RecordDialog::addInfoRow(QGridLayout *layout, int row, const QString &labelText,
                              const QString &valueText, bool bold, const QString &color)
{
    QLabel *label = new QLabel(labelText);
    label->setStyleSheet("font-weight: bold; color: #495057; font-size: 14px;");

    QLabel *value = new QLabel(valueText);
    QString style = QString("color: %1; font-size: 14px;").arg(color.isEmpty() ? "#6c757d" : color);
    if (bold) {
        style += "font-weight: bold;";
    }
    value->setStyleSheet(style);
    value->setWordWrap(true);

    layout->addWidget(label, row, 0, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(value, row, 1);
}

// 辅助函数：创建动作按钮（使用lambda调用您的现有函数）
QPushButton* RecordDialog::createActionButton(const QString &text, const std::function<void()> &action)
{
    QPushButton *button = new QPushButton(text);
    connect(button, &QPushButton::clicked, action);
    return button;
}

void RecordDialog::viewVideo()
{
    if (!videoPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));
    }
}

void RecordDialog::viewFile()
{
    if (!filePath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}

void RecordDialog::viewData()
{
    if (!dataPath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(dataPath));
    }
}
