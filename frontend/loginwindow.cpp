#include "loginwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    // 标签和输入框
    usernameLabel = new QLabel("用户名:");
    passwordLabel = new QLabel("密码:");
    usernameEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);  // 隐藏密码

    // 按钮
    loginButton = new QPushButton("登录");
    changePwdButton = new QPushButton("修改密码");

    // 表单布局
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(usernameLabel, usernameEdit);
    formLayout->addRow(passwordLabel, passwordEdit);

    // 垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(changePwdButton);

    setLayout(mainLayout);
    setWindowTitle("登录窗口");

    // 连接信号与槽
    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
    connect(changePwdButton, &QPushButton::clicked,
            this, &LoginWindow::onChangePasswordClicked);
}

LoginWindow::~LoginWindow()
{
    // Qt 会自动删除子控件，所以这里不用手动 delete
}

void LoginWindow::onLoginClicked()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username == "admin" && password == "123456") {
        QMessageBox::information(this, "登录成功", "欢迎回来: " + username);
        emit loginSuccess(username);
        close(); // 登录成功后关闭窗口
    } else {
        QMessageBox::warning(this, "登录失败", "账号或密码错误！");
    }
}

void LoginWindow::onChangePasswordClicked()
{
    QMessageBox::information(this, "修改密码", "这里可以跳转到修改密码界面");
}
