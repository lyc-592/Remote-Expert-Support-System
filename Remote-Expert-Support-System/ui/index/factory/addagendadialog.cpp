#include "ui/index/factory/addagendadialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QDateTime>
#include <QMessageBox>

AddAgendaDialog::AddAgendaDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("新增议程");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 横向布局：标题 | 设备 | 开始时间 | 截止时间 | 故障 | 状态 | 联系专家
    QHBoxLayout *rowLayout = new QHBoxLayout;

    titleEdit = new QLineEdit;
    titleEdit->setPlaceholderText("标题");

    deviceBox = new QComboBox;

    startDateEdit = new QDateEdit(QDate::currentDate());
    startDateEdit->setCalendarPopup(true);

    endDateEdit = new QDateEdit(QDate::currentDate());
    endDateEdit->setCalendarPopup(true);

    faultEdit = new QLineEdit;
    faultEdit->setPlaceholderText("故障描述");

    statusLabel = new QLabel("待处理"); // 默认值


    // 专家下拉多选
    expertBox = new QComboBox;
    expertModel = new QStandardItemModel(this);
    expertBox->setModel(expertModel);
    expertBox->setView(new QListView);  // ✅ 下拉时显示多选
    rowLayout->addWidget(expertBox);

    rowLayout->addWidget(titleEdit);
    rowLayout->addWidget(deviceBox);
    rowLayout->addWidget(startDateEdit);
    rowLayout->addWidget(endDateEdit);
    rowLayout->addWidget(faultEdit);
    rowLayout->addWidget(statusLabel);


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


void AddAgendaDialog::setExperts(const QList<QPair<int, QString>>& experts) {
    expertModel->clear();
    for (auto &pair : experts) {
        QStandardItem *item = new QStandardItem(pair.second); // 显示专家名字
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(pair.first, Qt::UserRole);              // 存专家id
        item->setCheckState(Qt::Unchecked);                   // 默认未勾选
        expertModel->appendRow(item);
    }
}


QStringList AddAgendaDialog::getExpertIds() const {
    QStringList ids;
    for (int i = 0; i < expertModel->rowCount(); ++i) {
        QStandardItem *item = expertModel->item(i);
        if (item->checkState() == Qt::Checked) {
            ids.append(QString::number(item->data(Qt::UserRole).toInt()));
        }
    }
    return ids;
}

void AddAgendaDialog::setDevices(const QList<QPair<int, QString>>& devs) {
    devices = devs;
    deviceBox->clear();
    for (auto &d : devs) {
        deviceBox->addItem(d.second, d.first);  // 显示名字，存 ID
    }
}


int AddAgendaDialog::getDeviceId() const {
    return deviceBox->currentData().toInt();
}


void AddAgendaDialog::onAccept()
{
    if (titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "议程标题不能为空！");
        return;
    }
    if (deviceBox->currentIndex() < 0) {  // ✅ 判断有没有选择设备
        QMessageBox::warning(this, "输入错误", "必须选择一个设备！");
        return;
    }
    if (faultEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "故障描述不能为空！");
        return;
    }

    updateStatus(); // 提交前更新状态
    accept();
}
