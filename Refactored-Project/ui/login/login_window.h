#ifndef MAINWINDOW_H
#define MAINWindow_H

#include "../../client/core/client.h"
#include "../../ui/index/factory/mainwindow.h"
#include "../../ui/index/expert/expertmainwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QScrollArea>  // 添加滚动区域头文件

/* 登录窗口，能够跳转注册窗口 */
/* 一个窗口包含一个Client，UI交互中Client发出请求，并由Server作出回应，根据信号来作出UI变化 */
class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
signals:
    void registerResult(bool succes, const QString& message);

private slots:
    void onLoginClicked();
    void onCancelClicked();
    void openRegisterDialog();

    // 处理注册结果，从RegisterDialog接收信号，之后调用client->registerUser()
    void onUserRegistered(const QString &user, const QString &pwd,
                          UserType userType, const QString &contact,
                          const QString &expertSkill);
    // 根据登录结果，确定登录成功或失败
    void onLoginResult(bool success, const QString& message);

    void openFindPasswordDialog();
    void onFindRequested(const QString &emailOrPhone);

private:
    void setupUI();
    void loadStyleSheet();
    // 客户
    Client *client;

    // UI 组件
    QScrollArea *scrollArea;  // 滚动区域
    QWidget *scrollContent;   // 滚动内容容器
    QLabel *titleLabel;
    QLabel *userLabel;
    QLineEdit *userEdit;
    QLabel *passLabel;
    QLineEdit *passEdit;
    QCheckBox *rememberCheck;
    QCheckBox *autoLoginCheck;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QGroupBox *settingsGroup;
    QLabel      *registerLabel;
    QPushButton *registerButton;
    QPushButton *forgetButton;
    MainWindow *mainWindow = nullptr;
    ExpertMainWindow *expertmainWindow = nullptr;
};

#endif // MAINWINDOW_H
