#include "login_window.h"
#include "register_dialog.h"
#include "find_password_dialog.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QToolButton>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    loadStyleSheet();
    client = new Client();
    /* 客户端操作 */
    connect(client, &Client::loginResult, this, &LoginWindow::onLoginResult, Qt::QueuedConnection);
    setWindowTitle("Qt QSS 示例 - 用户登录");
    setMinimumSize(500, 400);
    resize(550, 650);
}

LoginWindow::~LoginWindow() = default;

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

    // 创建密码输入框
    passEdit = new QLineEdit;
    passEdit->setPlaceholderText("请输入您的密码");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setMinimumHeight(40);

    // 创建忘记密码按钮并内嵌到密码框中
    QToolButton *forgetBtn = new QToolButton(passEdit);
    forgetBtn->setText("忘记密码?");
    forgetBtn->setObjectName("forgetInlineBtn");
    forgetBtn->setCursor(Qt::PointingHandCursor);
    forgetBtn->setStyleSheet("QToolButton { border: none; padding: 0 8px; color: #7B6DC2; background: transparent; }"
                             "QToolButton:hover { color: #5C4F9E; text-decoration: underline; }");

    // 使用布局将按钮放置在密码框右侧
    QHBoxLayout *passEditLayout = new QHBoxLayout(passEdit);
    passEditLayout->setContentsMargins(0, 0, 0, 0);
    passEditLayout->setSpacing(0);
    passEditLayout->addStretch(); // 将按钮推到最右边
    passEditLayout->addWidget(forgetBtn);

    /* ---------- 登录设置组 ---------- */
    settingsGroup = new QGroupBox("登录设置");
    settingsGroup->setObjectName("settingsGroup");
    settingsGroup->setMinimumHeight(100);

    rememberCheck = new QCheckBox("记住密码");
    autoLoginCheck = new QCheckBox("自动登录");

    QHBoxLayout *groupLay = new QHBoxLayout(settingsGroup);
    groupLay->addWidget(rememberCheck);
    groupLay->addWidget(autoLoginCheck);

    /* ---------- 按钮 ---------- */
    loginButton  = new QPushButton("登录");
    loginButton->setObjectName("loginButton");
    loginButton->setMinimumHeight(40);

    cancelButton = new QPushButton("取消");
    cancelButton->setObjectName("cancelButton");
    cancelButton->setMinimumHeight(40);

    QHBoxLayout *btnLay = new QHBoxLayout;
    //btnLay->addStretch();
    btnLay->addWidget(loginButton);
    btnLay->addWidget(cancelButton);

    /* ---------- “还没有账户？立即注册” ---------- */
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

    /* ---------- “忘记密码” ---------- */
    forgetButton = new QPushButton("忘记密码");
    forgetButton->setObjectName("forgetButton");
    forgetButton->setFlat(true);
    forgetButton->setCursor(Qt::PointingHandCursor);

    /* ---------- 组装 ---------- */
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(userEdit);
    mainLayout->addWidget(passLabel);
    mainLayout->addWidget(passEdit);
    mainLayout->addWidget(settingsGroup);
    mainLayout->addStretch();
    mainLayout->addLayout(btnLay);
    mainLayout->addWidget(regWidget);

    setCentralWidget(scrollArea);

    /* ---------- 信号槽 ---------- */
    connect(loginButton,  &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(cancelButton, &QPushButton::clicked, this, &LoginWindow::onCancelClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegisterDialog);
    connect(forgetBtn, &QToolButton::clicked, this, &LoginWindow::openFindPasswordDialog);

}

void LoginWindow::loadStyleSheet()
{
    QFile styleFile(":/qss/style.qss");
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
                                 QString("欢迎 %1！\n登录设置：\n记住密码：%2\n自动登录：%3")
                                     .arg(userEdit->text())
                                     .arg(rememberCheck->isChecked() ? "是" : "否")
                                     .arg(autoLoginCheck->isChecked() ? "是" : "否"));
        // 登录成功后的其他操作，如跳转到主界面
        mainWindow = new MainWindow();
        // 如果需要在主页显示相关信息选择实现该函数
        //mainWindow->setUsername(userEdit->text());
        mainWindow->show();

        // ===== 可选操作 =====
        // 隐藏登录窗口
        this->hide();

        // 当主窗口关闭时，也关闭登录窗口（清理资源）
        connect(mainWindow, &MainWindow::destroyed, this, [this]() {
            this->close();
        });
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
    connect(&dlg, &RegisterDialog::registered,
            this, &LoginWindow::onUserRegistered);   // 接收注册信号
    connect(this, &LoginWindow::registerResult,
            &dlg, &RegisterDialog::onRegisterResult);
    dlg.exec();
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

void LoginWindow::openFindPasswordDialog()
{
    FindPasswordDialog dlg(this);
    connect(&dlg, &FindPasswordDialog::findRequested,
            this, &LoginWindow::onFindRequested);
    dlg.exec();
}

void LoginWindow::onFindRequested(const QString &emailOrPhone)
{
    QMessageBox::information(this, "找回提示",
                             QString("已向 %1 发送找回邮件，请查收！").arg(emailOrPhone));
    /* 此处可扩展：调用后台接口等 */
}
