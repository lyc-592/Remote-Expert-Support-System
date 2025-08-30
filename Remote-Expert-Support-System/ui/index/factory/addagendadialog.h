#ifndef ADDAGENDADIALOG_H
#define ADDAGENDADIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QListWidget>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>

class AddAgendaDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddAgendaDialog(QWidget *parent = nullptr);

    void setExperts(const QList<QPair<int, QString>>& experts);
    void setDevices(const QList<QPair<int, QString>>& devs); // ✅ 新增：设置设备列表

    QStringList getExpertIds() const;             // 返回专家 id 列表
    int getDeviceId() const;                      // 返回选中的设备 id

    QString getTitle() const { return titleEdit->text().trimmed(); }
    QDate getStartDate() const { return startDateEdit->date(); }
    QDate getEndDate() const { return endDateEdit->date(); }
    QString getFaultDesc() const { return faultEdit->text().trimmed(); }
    QString getStatus() const { return statusLabel->text(); }

private slots:
    void onAccept();
    void updateStatus();

private:
    QLineEdit *titleEdit;
    QLineEdit *deviceEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QLineEdit *faultEdit;
    QLabel *statusLabel;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    QComboBox *expertBox;                // 下拉框（外观）
    QStandardItemModel *expertModel;     // 模型（存储勾选状态 + 数据）



    QComboBox *deviceBox;   // ✅ 新增：设备下拉框
    QList<QPair<int, QString>> devices;  // 保存 device_id 和名字对应关系

};

#endif // ADDAGENDADIALOG_H
