#ifndef ORDER_DETAIL_DIALOG_H
#define ORDER_DETAIL_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "server/db/database_manager.h"

class OrderDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDetailDialog(int orderId, DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~OrderDetailDialog();

private:
    int orderId;                 // 工单ID
    DatabaseManager* dbManager;  // 数据库管理器

    // UI组件
    QLabel *orderIdLabel;
    QLabel *deviceNameLabel;
    QLabel *deviceTypeLabel;
    QLabel *createTimeLabel;
    QLabel *endTimeLabel;
    QLabel *statusLabel;
    QLabel *expertLabel;
    QLabel *faultLabel;

    void setupUI();
    void loadOrderDetails();
};
#endif // ORDER_DETAIL_DIALOG_H
