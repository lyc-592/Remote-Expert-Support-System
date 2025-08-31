#include "find_password_dialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

FindPasswordDialog::FindPasswordDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("找回密码");
    setFixedSize(380, 200);
}

void FindPasswordDialog::setupUI()
{
    inputEdit = new QLineEdit;
    verifyEdit = new QLineEdit;
    inputEdit->setPlaceholderText("请输入注册时填写的邮箱");
    verifyEdit->setPlaceholderText("请输入收到的验证码");

    findBtn    = new QPushButton("发送验证码");
    cancelBtn  = new QPushButton("取消");

    QFormLayout *form = new QFormLayout(this);
    form->addRow("邮箱:", inputEdit);
    form->addRow("验证码:", verifyEdit);

    QHBoxLayout *btnLay = new QHBoxLayout;
    btnLay->addStretch();
    btnLay->addWidget(findBtn);
    btnLay->addWidget(cancelBtn);
    form->addRow(btnLay);

    connect(findBtn,   &QPushButton::clicked, this, &FindPasswordDialog::onFindClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &FindPasswordDialog::onCancelClicked);
}

void FindPasswordDialog::onFindClicked()
{
    const QString text = inputEdit->text().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入邮箱！");
        return;
    }
    emit findRequested(text);   // 发出找回请求
    accept();
}

void FindPasswordDialog::onCancelClicked()
{
    reject();
}
