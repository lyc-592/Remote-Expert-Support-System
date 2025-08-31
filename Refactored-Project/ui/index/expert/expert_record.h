#ifndef EXPERT_RECORD_H
#define EXPERT_RECORD_H

#include <QDialog>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "server/db/database_manager.h"

class ExpertRecord : public QDialog
{
    Q_OBJECT

public:
    explicit ExpertRecord(int orderId, DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~ExpertRecord();

private slots:
    void viewVideo();
    void viewFile();
    void viewData();

private:
    int orderId;                 // 工单ID
    DatabaseManager* dbManager;  // 数据库管理器

    // 文件路径
    QString videoPath;
    QString filePath;
    QString dataPath;

    // UI组件
    QLabel *faultTypeLabel;
    QLabel *solutionLabel;
    QLabel *uploadTimeLabel;
    QPushButton *videoButton;
    QPushButton *fileButton;
    QPushButton *dataButton;

    void setupUI();
    void loadRecordDetails();
    void addInfoRow(QGridLayout *layout, int row, const QString &labelText,
                    const QString &valueText, bool bold = false);
    QPushButton* createActionButton(const QString &text, const std::function<void()> &action);
    void setupFileButtons(QVBoxLayout *layout);
};

#endif // EXPERT_RECORD_H
