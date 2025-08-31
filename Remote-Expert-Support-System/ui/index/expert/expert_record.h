#ifndef EXPERT_RECORD_H
#define EXPERT_RECORD_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include "../../server/db/database_manager.h"

class ExpertRecord : public QDialog
{
    Q_OBJECT

public:
    explicit ExpertRecord(int orderId, DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~ExpertRecord();

private:
    void setupUI();
    void loadRecordDetails();

    // 文件查看函数
    void viewVideo();
    void viewFile();
    void viewData();

    int orderId;
    DatabaseManager* dbManager;

    // UI组件
    QLabel *orderIdLabel;
    QLabel *faultTypeLabel;
    QLabel *solutionLabel;
    QLabel *uploadTimeLabel;
    QPushButton *viewVideoButton;
    QPushButton *viewFileButton;
    QPushButton *viewDataButton;

    // 文件路径
    QString videoPath;
    QString filePath;
    QString dataPath;
};


#endif // EXPERT_RECORD_H
