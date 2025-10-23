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
#include <QGroupBox>
#include <QSpacerItem>
#include <QFrame>

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

    // 保存后自动退出编辑模式
    toggleEditMode(false);
    editToggleButton->setText("编辑信息");
}

void UserPage::toggleEditMode(bool enabled)
{
    // 切换编辑模式
    isEditMode = enabled;

    usernameEdit->setReadOnly(!enabled);
    passwordEdit->setReadOnly(!enabled);
    confirmPasswordEdit->setReadOnly(!enabled);

    // 根据编辑模式更新按钮文本
    if (enabled) {
        editToggleButton->setText("取消编辑");
        passwordEdit->setFocus();
    } else {
        editToggleButton->setText("编辑信息");
        // 退出编辑模式时清空密码字段
        passwordEdit->clear();
        confirmPasswordEdit->clear();
    }
}

void UserPage::setupUI()
{
    // 初始化编辑模式状态
    isEditMode = false;

    // 检查是否已经有布局，如果有则清除
    if (this->layout()) {
        QLayout* oldLayout = this->layout();
        delete oldLayout;
    }

    // 创建主垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20); // 减小边距使界面更紧凑

    // 创建用户信息分组框
    QGroupBox *userGroup = new QGroupBox("用户信息");
    userGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    // 创建表单布局
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(15, 20, 15, 15); // 分组框内边距

    // 用户ID显示（不可编辑）
    QLabel *userIdTitle = new QLabel("用户ID:");
    userIdTitle->setStyleSheet("font-weight: bold;");
    userIdLabel = new QLabel(QString::number(this->user.userId));
    userIdLabel->setStyleSheet("background-color: #f0f0f0; padding: 5px; border-radius: 3px;");
    formLayout->addRow(userIdTitle, userIdLabel);

    // 用户名编辑
    QLabel *usernameTitle = new QLabel("用户名:");
    usernameTitle->setStyleSheet("font-weight: bold;");
    usernameEdit = new QLineEdit(this->user.account);
    usernameEdit->setReadOnly(true); // 初始状态为只读
    usernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 3px; }"
                                "QLineEdit:read-only { background-color: #f0f0f0; }");
    if (this->user.account.isEmpty()) {
        usernameEdit->setPlaceholderText("请输入用户名");
    }
    formLayout->addRow(usernameTitle, usernameEdit);

    // 密码编辑
    QLabel *passwordTitle = new QLabel("新密码:");
    passwordTitle->setStyleSheet("font-weight: bold;");
    passwordEdit = new QLineEdit();
    passwordEdit->setReadOnly(true); // 初始状态为只读
    passwordEdit->setPlaceholderText("请输入新密码");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 3px; }"
                                "QLineEdit:read-only { background-color: #f0f0f0; }");
    formLayout->addRow(passwordTitle, passwordEdit);

    // 确认密码
    QLabel *confirmPasswordTitle = new QLabel("确认密码:");
    confirmPasswordTitle->setStyleSheet("font-weight: bold;");
    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setReadOnly(true); // 初始状态为只读
    confirmPasswordEdit->setPlaceholderText("请再次输入密码");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 3px; }"
                                       "QLineEdit:read-only { background-color: #f0f0f0; }");
    formLayout->addRow(confirmPasswordTitle, confirmPasswordEdit);

    userGroup->setLayout(formLayout);
    mainLayout->addWidget(userGroup);

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    // 编辑/取消编辑切换按钮
    editToggleButton = new QPushButton("编辑信息", this);
    editToggleButton->setStyleSheet("QPushButton { background-color: #5cb85c; color: white; padding: 8px; border-radius: 4px; }"
                                    "QPushButton:hover { background-color: #4cae4c; }");
    connect(editToggleButton, &QPushButton::clicked, this, [this]() {
        toggleEditMode(!isEditMode);
    });
    buttonLayout->addWidget(editToggleButton);

    // 保存按钮
    QPushButton *saveButton = new QPushButton("保存更改", this);
    saveButton->setStyleSheet("QPushButton { background-color: #337ab7; color: white; padding: 8px; border-radius: 4px; }"
                              "QPushButton:hover { background-color: #286090; }"
                              "QPushButton:disabled { background-color: #cccccc; }");
    saveButton->setEnabled(false); // 初始状态禁用
    connect(saveButton, &QPushButton::clicked, this, &UserPage::saveUserData);
    buttonLayout->addWidget(saveButton);

    // 连接编辑模式变化信号，启用/禁用保存按钮
    connect(this, &UserPage::editModeChanged, [saveButton](bool enabled) {
        saveButton->setEnabled(enabled);
    });

    // 退出登录按钮
    QPushButton *logoutBtn = new QPushButton("退出登录", this);
    logoutBtn->setStyleSheet("QPushButton { background-color: #d9534f; color: white; padding: 8px; border-radius: 4px; }"
                             "QPushButton:hover { background-color: #c9302c; }");
    connect(logoutBtn, &QPushButton::clicked, this, [this]{
        emit logoutRequested();
    });
    buttonLayout->addWidget(logoutBtn);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("个人用户管理");
    resize(350, 350); // 缩小窗口尺寸
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

    // 刷新后退出编辑模式
    toggleEditMode(false);
    editToggleButton->setText("编辑信息");
}
