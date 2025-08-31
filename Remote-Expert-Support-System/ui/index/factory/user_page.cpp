#include "user_page.h"
#include <QWidget>
#include <QMessageBox>
#include <QJsonDocument>
#include <QFormLayout>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

UserPage::UserPage(User &user, QWidget *parent) : user(user), QWidget(parent)
{
    qDebug() << user.toJson();
    setupUI();
}

void UserPage::saveUserData()
{
    // 获取输入的新数据
    QString newUsername = usernameEdit->text();
    QString newPassword = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();

    // 验证输入
    if (newUsername.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名不能为空");
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "警告", "两次输入的密码不一致");
        return;
    }

    // 更新用户数据
    emit updateUser(newUsername, newPassword);
}

void UserPage::setupUI()
{
    // 检查是否已经有布局，如果有则清除
    if (this->layout()) {
        QLayout* oldLayout = this->layout();
        delete oldLayout;
    }

    // 创建表单布局
    QFormLayout *formLayout = new QFormLayout();

    // 用户ID显示（不可编辑）
    userIdLabel = new QLabel(QString::number(this->user.userId));
    formLayout->addRow("用户ID:", userIdLabel);

    // 用户名编辑
    usernameEdit = new QLineEdit(this->user.account);
    if (this->user.account.isEmpty()) {
        usernameEdit->setPlaceholderText("请输入用户名");
    }
    formLayout->addRow("用户名:", usernameEdit);

    // 密码编辑
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("请输入新密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow("新密码:", passwordEdit);

    // 确认密码
    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setPlaceholderText("请再次输入密码");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow("确认密码:", confirmPasswordEdit);

    // 保存按钮
    QPushButton *saveButton = new QPushButton("保存更改", this);
    connect(saveButton, &QPushButton::clicked, this, &UserPage::saveUserData);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveButton);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setWindowTitle("个人用户管理");
    resize(400, 300);
}

void UserPage::refresh(User& user) {
    this->user = user;

    // 更新UI而不重新创建整个布局
    userIdLabel->setText(QString::number(this->user.userId));

    if (!this->user.account.isEmpty()) {
        usernameEdit->setText(this->user.account);
        usernameEdit->setPlaceholderText("");
    } else {
        usernameEdit->setText("");
        usernameEdit->setPlaceholderText("请输入用户名");
    }

    passwordEdit->clear();
    confirmPasswordEdit->clear();
}
