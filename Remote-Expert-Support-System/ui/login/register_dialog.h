#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QMainWindow>
#include <QDialog>
#include <QComboBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFormLayout>
#include "../../utils/constants.h"

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class RegisterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    // 由LoginWindow返回的结果来判断注册成功与否；目前的问题是无法正确关闭窗口
    void onRegisterResult(bool success, const QString &message);

signals:
    // 给LoginWindow的信号
    void registered(const QString &user, const QString &pwd, UserType userType,
                    const QString &contact, const QString &expertSkill);   // 新增信号

private slots:
    void onRegisterClicked();
    void onCancelClicked();
    // RadioButton状态切换
    void onUserTypeChanged(bool checked);

private:
    void setupUI();
    void setUiEnabled(bool enabled);

    QFormLayout *form;

    QLineEdit *mailEdit;
    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QLineEdit *confirmEdit;
    QLineEdit *expertSkillEdit;

    QRadioButton *factoryRadio;
    QRadioButton *expertRadio;
    QButtonGroup *userTypeGroup;

    QPushButton *regBtn;
    QPushButton *cancelBtn;
};

#endif // REGISTER_DIALOG_H
