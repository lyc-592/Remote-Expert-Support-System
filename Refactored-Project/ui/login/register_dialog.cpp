#include "register_dialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("用户注册");
    setFixedSize(380, 300);
    setWindowModality(Qt::ApplicationModal);
    setModal(true);
}

void RegisterDialog::setupUI()
{
    mailEdit    = new QLineEdit;
    userEdit    = new QLineEdit;
    passEdit    = new QLineEdit;
    confirmEdit = new QLineEdit;
    passEdit->setEchoMode(QLineEdit::Password);
    confirmEdit->setEchoMode(QLineEdit::Password);
    // 用户类型单选按钮
    factoryRadio = new QRadioButton("工厂", this);
    expertRadio = new QRadioButton("专家", this);
    factoryRadio->setObjectName("rbFactory");
    expertRadio->setObjectName("rbExpert");
    //userTypeWidget->setObjectName("userTypeWidget");

    // 创建按钮组并添加单选按钮
    userTypeGroup = new QButtonGroup(this);
    userTypeGroup->addButton(factoryRadio, 0); // 0 表示普通用户
    userTypeGroup->addButton(expertRadio, 1); // 1 表示专家用户

    // 默认选择工厂
    factoryRadio->setChecked(true);

    expertSkillEdit = new QLineEdit(this);

    regBtn    = new QPushButton("注册");
    cancelBtn = new QPushButton("取消");

    form = new QFormLayout(this);
    form->addRow("邮  箱:", mailEdit);
    form->addRow("用户名:", userEdit);
    form->addRow("密  码:", passEdit);
    form->addRow("确认密码:", confirmEdit);
    // 用户类型单选按钮布局
    QHBoxLayout *userTypeLayout = new QHBoxLayout;
    userTypeLayout->addWidget(factoryRadio);
    userTypeLayout->addWidget(expertRadio);
    QWidget *userTypeWidget = new QWidget(this);
    userTypeWidget->setLayout(userTypeLayout);
    form->addRow("用户类型:", userTypeWidget);
    form->addRow("擅长技能:", expertSkillEdit);

    QHBoxLayout *btnLay = new QHBoxLayout;
    btnLay->addStretch();
    btnLay->addWidget(regBtn);
    btnLay->addWidget(cancelBtn);
    form->addRow(btnLay);

    connect(regBtn,    &QPushButton::clicked, this, &RegisterDialog::onRegisterClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &RegisterDialog::onCancelClicked);
    connect(factoryRadio, &QRadioButton::toggled, this, &RegisterDialog::onUserTypeChanged);
    connect(expertRadio, &QRadioButton::toggled, this, &RegisterDialog::onUserTypeChanged);
    expertSkillEdit->setVisible(false);
    form->labelForField(expertSkillEdit)->setVisible(false);
}

void RegisterDialog::onUserTypeChanged(bool checked)
{
    if (!checked) return; // 只处理被选中的按钮

    // 当用户类型改变时，显示或隐藏专家技能输入框
    bool isExpert = expertRadio->isChecked();
    expertSkillEdit->setVisible(isExpert);
    form->labelForField(expertSkillEdit)->setVisible(isExpert);
}

void RegisterDialog::onRegisterClicked()
{
    QString user = userEdit->text().trimmed();
    QString pwd = passEdit->text();
    QString confirmPwd = confirmEdit->text();
    QString contact = mailEdit->text().trimmed();
    // 获取选中的用户类型
    UserType userType = FactoryUser; // 默认为工厂
    if (expertRadio->isChecked()) {
        userType = ExpertUser;
    }
    QString expertSkill = expertSkillEdit->text().trimmed();

    if (user.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名不能为空！");
        userEdit->setFocus();
        return;
    }

    if (pwd.isEmpty()) {
        QMessageBox::warning(this, "错误", "密码不能为空！");
        passEdit->setFocus();
        return;
    }
    if (passEdit->text() != confirmEdit->text()) {
        QMessageBox::warning(this, "错误", "两次密码输入不一致！");
        confirmEdit->setFocus();
        return;
    }
    if (contact.isEmpty()) {
        QMessageBox::warning(this, "错误", "联系方式不能为空！");
        mailEdit->setFocus();
        return;
    }

    emit registered(user, pwd, userType, contact, expertSkill);   // 发信号
    accept();                     // 关闭对话框
}

void RegisterDialog::onCancelClicked()
{
    reject();
}

void RegisterDialog::setUiEnabled(bool enabled)
{
    userEdit->setEnabled(enabled);
    passEdit->setEnabled(enabled);
    confirmEdit->setEnabled(enabled);
    mailEdit->setEnabled(enabled);
    expertSkillEdit->setEnabled(enabled);
    factoryRadio->setEnabled(enabled);
    expertRadio->setEnabled(enabled);
    regBtn->setEnabled(enabled);
    cancelBtn->setEnabled(enabled);
}

void RegisterDialog::onRegisterResult(bool success, const QString &message)
{
    // 启用UI
    setUiEnabled(true);

    if (success) {
        // 注册成功，关闭对话框
        QMessageBox::information(this, "成功", message);
        accept();
    } else {
        // 注册失败，显示错误信息
        QMessageBox::warning(this, "错误", message);
    }
}
