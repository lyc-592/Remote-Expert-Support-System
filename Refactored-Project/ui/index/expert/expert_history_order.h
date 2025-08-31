#ifndef EXPERT_HISTORY_ORDER_H
#define EXPERT_HISTORY_ORDER_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include "server/db/database_manager.h"

class ExpertHistoryOrder : public QWidget
{
    Q_OBJECT

public:
    explicit ExpertHistoryOrder(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~ExpertHistoryOrder();

    void refreshData();  // 公开的刷新方法，可供外部调用

private slots:
    void refreshTable();          // 刷新表格数据
    void searchOrders();          // 搜索工单
    void viewDetails();           // 查看工单详情
    void viewRecord();            // 查看现场记录

private:
    DatabaseManager* dbManager;   // 使用您的数据库管理类
    QSqlQueryModel *model;        // 数据模型
    QTableView *tableView;        // 表格视图

    QLineEdit *searchEdit;        // 搜索框
    QPushButton *btnSearch;       // 搜索按钮
    QPushButton *btnView;         // 查看详情按钮
    QPushButton *btnViewRecord;   // 查看现场记录按钮
    QPushButton *btnRefresh;      // 刷新按钮

    void setupUI();               // 初始化UI
};

#endif // EXPERT_HISTORY_ORDER_H
