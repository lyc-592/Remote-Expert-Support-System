#ifndef EXPERT_ORDER_DETAIL_H
#define EXPERT_ORDER_DETAIL_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include "../../server/db/database_manager.h"

class ExpertOrderDetail : public QDialog
{
    Q_OBJECT

public:
    explicit ExpertOrderDetail(int orderId, DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~ExpertOrderDetail();

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
    QLabel *factoryLabel;
    QLabel *faultLabel;

    void setupUI();
    void loadOrderDetails();
};

#endif // EXPERT_ORDER_DETAIL_H
