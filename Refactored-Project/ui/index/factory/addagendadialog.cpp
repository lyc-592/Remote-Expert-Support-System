#include "addagendadialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QDateTime>

AddAgendaDialog::AddAgendaDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("新增议程");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 横向布局：标题 | 设备 | 开始时间 | 截止时间 | 故障 | 状态 | 联系专家
    QHBoxLayout *rowLayout = new QHBoxLayout;

    titleEdit = new QLineEdit;
    titleEdit->setPlaceholderText("标题");

    deviceEdit = new QLineEdit;
    deviceEdit->setPlaceholderText("设备信息");

    startDateEdit = new QDateEdit(QDate::currentDate());
    startDateEdit->setCalendarPopup(true);

    endDateEdit = new QDateEdit(QDate::currentDate());
    endDateEdit->setCalendarPopup(true);

    faultEdit = new QLineEdit;
    faultEdit->setPlaceholderText("故障描述");

    statusLabel = new QLabel("待处理"); // 默认值

    expertBox = new QComboBox;
    expertBox->addItems({"专家A", "专家B", "专家C"}); // TODO: 联系人模块加载

    rowLayout->addWidget(titleEdit);
    rowLayout->addWidget(deviceEdit);
    rowLayout->addWidget(startDateEdit);
    rowLayout->addWidget(endDateEdit);
    rowLayout->addWidget(faultEdit);
    rowLayout->addWidget(statusLabel);
    rowLayout->addWidget(expertBox);

    mainLayout->addLayout(rowLayout);

    // 底部按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnOk = new QPushButton("确定");
    btnCancel = new QPushButton("取消");
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    mainLayout->addLayout(btnLayout);

    // 信号槽
    connect(btnOk, &QPushButton::clicked, this, &AddAgendaDialog::onAccept);
    connect(btnCancel, &QPushButton::clicked, this, &AddAgendaDialog::reject);

    connect(startDateEdit, &QDateEdit::dateChanged, this, &AddAgendaDialog::updateStatus);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &AddAgendaDialog::updateStatus);
}

void AddAgendaDialog::updateStatus()
{
    QDate today = QDate::currentDate();
    if (today > endDateEdit->date()) {
        statusLabel->setText("已过期");
    } else {
        statusLabel->setText("待处理");
    }
}

void AddAgendaDialog::onAccept()
{
    if (titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "议程标题不能为空！");
        return;
    }
    if (deviceEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "设备信息不能为空！");
        return;
    }
    if (faultEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "故障描述不能为空！");
        return;
    }

    updateStatus(); // 提交前更新状态
    accept();
}
