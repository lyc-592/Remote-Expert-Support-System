#include "expert_history_order.h"
#include "expert_order_detail.h"
#include "expert_record.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

ExpertHistoryOrder::ExpertHistoryOrder(DatabaseManager* dbManager, QWidget *parent)
    : QWidget(parent), dbManager(dbManager)
{
    // 设置窗口属性
    setWindowTitle("历史工单");

    // 检查数据库连接
    if (!dbManager || !dbManager->isConnected()) {
        QLabel *errorLabel = new QLabel("数据库未连接，无法显示历史工单");
        errorLabel->setAlignment(Qt::AlignCenter);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(errorLabel);
        return;
    }

    setupUI();
    refreshTable();
}

ExpertHistoryOrder::~ExpertHistoryOrder()
{
    // 不需要手动关闭数据库，由DatabaseManager管理
}

void ExpertHistoryOrder::setupUI()
{
    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // 左边内容
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setSpacing(10);

    // 搜索区域
    QHBoxLayout *searchLayout = new QHBoxLayout;
    QLabel *searchLabel = new QLabel("搜索:");
    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("输入设备名称、故障描述或联系人关键字");
    btnSearch = new QPushButton("搜索");

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(btnSearch);

    leftLayout->addLayout(searchLayout);

    // 创建表格视图
    tableView = new QTableView(this);
    model = new QSqlQueryModel(this);

    // 设置表格属性
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自适应列宽
    tableView->verticalHeader()->setDefaultSectionSize(40); // 设置行高

    // 添加表格到布局
    leftLayout->addWidget(tableView);

    // 底部按钮区域
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnView = new QPushButton("查看详情");
    btnViewRecord = new QPushButton("查看现场记录");
    btnRefresh = new QPushButton("刷新");

    btnLayout->addWidget(btnView);
    btnLayout->addWidget(btnViewRecord);
    btnLayout->addStretch(); // 添加弹性空间
    btnLayout->addWidget(btnRefresh);

    leftLayout->addLayout(btnLayout);

    // 将左侧布局添加到主布局，占据全部空间（没有右侧按钮）
    mainLayout->addLayout(leftLayout, 1);

    // 信号槽连接
    connect(btnView, &QPushButton::clicked, this, &ExpertHistoryOrder::viewDetails);
    connect(btnViewRecord, &QPushButton::clicked, this, &ExpertHistoryOrder::viewRecord);
    connect(btnRefresh, &QPushButton::clicked, this, &ExpertHistoryOrder::refreshTable);
    connect(tableView, &QTableView::doubleClicked, this, &ExpertHistoryOrder::viewDetails);
    connect(btnSearch, &QPushButton::clicked, this, &ExpertHistoryOrder::searchOrders);
    connect(searchEdit, &QLineEdit::returnPressed, this, &ExpertHistoryOrder::searchOrders);
}

void ExpertHistoryOrder::refreshData()
{
    refreshTable();
}

void ExpertHistoryOrder::refreshTable()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    // 获取数据库连接
    QSqlDatabase db = dbManager->getDatabase();

    // 查询历史工单数据，关联联系人表和用户表获取详细信息
    QString queryStr = "SELECT "
                       "ho.history_order_id, "
                       "d.device_name, "
                       "ho.order_create_time, "
                       "ho.order_end_time, "
                       "ho.fault, "
                       "CASE WHEN ho.order_status = 0 THEN '已完成' ELSE '已拒绝' END as status, "
                       "user.user_account as factory_info "    // 用户的联系信息（工厂）
                       "FROM history_order ho "
                       "LEFT JOIN device d ON ho.device_id = d.device_id "       // 关联设备表获取设备名称
                       "LEFT JOIN user ON ho.user1_id = user.user_id "          // 关联用户表获取专家信息
                       "ORDER BY ho.order_end_time DESC";

    QSqlQuery query(db);
    if (!query.exec(queryStr)) {
        QMessageBox::critical(this, "查询错误", "无法获取历史工单数据: " + query.lastError().text());
        return;
    }

    // 更新模型数据
    model->setQuery(query);

    // 设置表头
    model->setHeaderData(0, Qt::Horizontal, tr("工单号"));
    model->setHeaderData(1, Qt::Horizontal, tr("设备"));
    model->setHeaderData(2, Qt::Horizontal, tr("开始时间"));
    model->setHeaderData(3, Qt::Horizontal, tr("截止时间"));
    model->setHeaderData(4, Qt::Horizontal, tr("故障描述"));
    model->setHeaderData(5, Qt::Horizontal, tr("状态"));
    model->setHeaderData(6, Qt::Horizontal, tr("工厂"));

    // 隐藏ID列
    // tableView->hideColumn(0);
}

void ExpertHistoryOrder::searchOrders()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    QString keyword = searchEdit->text().trimmed();
    if (keyword.isEmpty()) {
        refreshTable();
        return;
    }

    // 获取数据库连接
    QSqlDatabase db = dbManager->getDatabase();

    // 构建搜索查询
    QString queryStr = "SELECT "
                       "ho.history_order_id, "
                       "d.device_name, "
                       "ho.order_create_time, "
                       "ho.order_end_time, "
                       "ho.fault, "
                       "CASE WHEN ho.order_status = 0 THEN '已完成' ELSE '已拒绝' END as status, "
                       "user.user_account as factory_info "
                       "FROM history_order ho "
                       "LEFT JOIN device d ON ho.device_id = d.device_id "
                       "LEFT JOIN user ON ho.user1_id = user.user_id "
                       "WHERE d.device_name LIKE :keyword OR "
                       "ho.fault LIKE :keyword OR "
                       "user.user_account LIKE :keyword OR "
                       "ho.history_order_id LIKE :keyword "
                       "ORDER BY ho.order_end_time DESC";

    QSqlQuery query(db);
    query.prepare(queryStr);
    query.bindValue(":keyword", "%" + keyword + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "搜索错误", "搜索失败: " + query.lastError().text());
        return;
    }

    // 更新模型数据
    model->setQuery(query);

    // 设置表头
    model->setHeaderData(0, Qt::Horizontal, tr("工单号"));
    model->setHeaderData(1, Qt::Horizontal, tr("设备"));
    model->setHeaderData(2, Qt::Horizontal, tr("开始时间"));
    model->setHeaderData(3, Qt::Horizontal, tr("截止时间"));
    model->setHeaderData(4, Qt::Horizontal, tr("故障描述"));
    model->setHeaderData(5, Qt::Horizontal, tr("状态"));
    model->setHeaderData(6, Qt::Horizontal, tr("工厂"));


    // 隐藏ID列
    tableView->hideColumn(0);
}

void ExpertHistoryOrder::viewDetails()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一个工单");
        return;
    }

    int row = selectedIndexes.first().row();
    int id = model->data(model->index(row, 0)).toInt(); // 获取ID

    // 创建详情对话框
    ExpertOrderDetail *detailDialog = new ExpertOrderDetail(id, dbManager, this);
    detailDialog->exec();
    delete detailDialog;
}

void ExpertHistoryOrder::viewRecord()
{
    if (!dbManager->isConnected()) {
        QMessageBox::warning(this, "数据库错误", "数据库连接已断开");
        return;
    }

    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一个工单");
        return;
    }

    int row = selectedIndexes.first().row();
    int id = model->data(model->index(row, 0)).toInt(); // 获取历史工单ID

    // 创建现场记录对话框
    ExpertRecord *recordDialog = new ExpertRecord(id, dbManager, this);
    recordDialog->exec();
    delete recordDialog;
}
