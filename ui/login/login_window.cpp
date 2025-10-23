#include "login_window.h"
#include "register_dialog.h"
#include "../index/factory/main_window.h"
#include "../index/expert/expert_main_window.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    loadStyleSheet();
    client = new Client();
    /* 客户端操作！ */
    connect(client, &Client::loginResult, this, &LoginWindow::onLoginResult);
    connect(client, &Client::getUserResult, this, &LoginWindow::openMainWindow);
    connect(client, &Client::registerResult, this, &LoginWindow::onRegisterResult);
    setWindowTitle("Qt QSS 示例 - 用户登录");
    setMinimumSize(500, 400);
    resize(550, 650);
}

LoginWindow::~LoginWindow() {
    if (client) delete client;
}

void LoginWindow::setupUI()
{
    /* ---------- 滚动区域 ---------- */
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    /* ---------- 居中容器 ---------- */
    QWidget *centerContainer = new QWidget;
    scrollArea->setWidget(centerContainer);

    QHBoxLayout *hLay = new QHBoxLayout(centerContainer);
    hLay->setContentsMargins(0, 0, 0, 0);

    QWidget *formContainer = new QWidget;
    formContainer->setMaximumWidth(600);
    formContainer->setMaximumHeight(900);
    hLay->addStretch();
    hLay->addWidget(formContainer);
    hLay->addStretch();

    /* ---------- 主垂直布局 ---------- */
    QVBoxLayout *mainLayout = new QVBoxLayout(formContainer);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    /* ---------- 标题 ---------- */
    titleLabel = new QLabel("欢迎进入工业现场远程专家支持系统！");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setMinimumHeight(80);
    titleLabel->setWordWrap(true);

    /* ---------- 表单控件 ---------- */
    userLabel = new QLabel("用户名:");
    userEdit  = new QLineEdit;
    userEdit->setPlaceholderText("请输入您的用户名");
    userEdit->setMinimumHeight(40);

    passLabel = new QLabel("密码:");
    passEdit = new QLineEdit;
    passEdit->setPlaceholderText("请输入您的密码");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setMinimumHeight(40);

    /* ---------- 按钮 ---------- */
    loginButton  = new QPushButton("登录");
    loginButton->setObjectName("loginButton");
    loginButton->setMinimumHeight(40);

    cancelButton = new QPushButton("取消");
    cancelButton->setObjectName("cancelButton");
    cancelButton->setMinimumHeight(40);

    QHBoxLayout *btnLay = new QHBoxLayout;
    btnLay->addWidget(loginButton);
    btnLay->addWidget(cancelButton);

    /* ---------- "还没有账户？立即注册" ---------- */
    QWidget *regWidget = new QWidget;
    QHBoxLayout *regLay = new QHBoxLayout(regWidget);
    regLay->setContentsMargins(0, 0, 0, 0);

    registerLabel  = new QLabel("还没有账户？");
    registerLabel->setObjectName("registerLabel");

    registerButton = new QPushButton("立即注册");
    registerButton->setObjectName("registerButton");
    registerButton->setFlat(true);
    registerButton->setCursor(Qt::PointingHandCursor);

    regLay->addStretch();
    regLay->addWidget(registerLabel);
    regLay->addWidget(registerButton);
    regLay->addStretch();

    /* ---------- 组装 ---------- */
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(userEdit);
    mainLayout->addWidget(passLabel);
    mainLayout->addWidget(passEdit);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLay);
    mainLayout->addWidget(regWidget);

    setCentralWidget(scrollArea);

    /* ---------- 信号槽 ---------- */
    connect(loginButton,  &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(cancelButton, &QPushButton::clicked, this, &LoginWindow::onCancelClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegisterDialog);
}

void LoginWindow::loadStyleSheet()
{
    QFile styleFile(":/qss/style1.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString style = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(style);
        styleFile.close();
        qDebug() << "QSS 文件加载成功！";
    }
    else
    {
        QMessageBox::warning(this, "警告", "无法加载样式表文件！");
    }
}

void LoginWindow::onLoginClicked()
{
    const QString user = userEdit->text().trimmed();
    const QString pass = passEdit->text().trimmed();

    if (user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "输入错误", "用户名和密码不能为空！");
        return;
    }

    client->login(user, pass);
}

void LoginWindow::onLoginResult(bool success, const QString& message)
{
    qDebug() << "LoginWindow on Login Result";
    if (success) {
        QMessageBox::information(this, "登录成功",
                                 QString("欢迎 %1！").arg(userEdit->text()));
        client->getUser(userEdit->text());
    } else {
        QMessageBox::warning(this, "登录失败", message);
    }
}

void LoginWindow::onCancelClicked()
{
    if (QMessageBox::question(this, "确认", "确定要退出吗？",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes)
    {
        qApp->quit();
    }
}

void LoginWindow::openRegisterDialog()
{
    RegisterDialog dlg(this);

    // 直接连接 Client 的 registerResult 信号到 RegisterDialog 的 onRegisterResult 槽
    connect(this->client, &Client::registerResult,
            &dlg, &RegisterDialog::onRegisterResult);

    connect(&dlg, &RegisterDialog::registered,
            this, &LoginWindow::onUserRegistered);

    dlg.exec();
    // 断开连接
    disconnect(this->client, &Client::registerResult,
               &dlg, &RegisterDialog::onRegisterResult);
    disconnect(&dlg, &RegisterDialog::registered,
               this, &LoginWindow::onUserRegistered);
}

void LoginWindow::onUserRegistered(const QString &user, const QString &pwd,
                                   UserType userType, const QString &contact,
                                   const QString &expertSkill)
{
    // 处理注册请求
    qDebug() << "Registering user:" << user << "type:" << userType;
    // 调用Client发送注册请求
    if (client) {
        client->registerUser(user, pwd, contact, userType, expertSkill);
    }
}

void LoginWindow::onRegisterResult(bool success, const QString &message)
{
    emit registerSignal(success, message);
}

void LoginWindow::openMainWindow(bool success, const QString& message, const User& userData) {
    if (userData.userType == FactoryUser) {
        MainWindow *mainWindow = new MainWindow(this->client);
        mainWindow->show();
        this->close();
    } else {
        ExpertMainWindow *expMainWindow = new ExpertMainWindow(this->client);
        expMainWindow->show();
        this->close();
    }
}
