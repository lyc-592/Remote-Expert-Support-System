#include "agenda_page.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "add_agenda_dialog.h"
#include "../../server/entities/agenda.h"
#include <QHeaderView>


AgendaPage::AgendaPage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *agendaLayout = new QHBoxLayout(this);

    // 左边内容
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // 顶部按钮区
    QHBoxLayout *topBtnLayout = new QHBoxLayout;
    btnAdd = new QPushButton("增加议程");
    btnRemove = new QPushButton("删除议程");
    topBtnLayout->addWidget(btnAdd);
    topBtnLayout->addWidget(btnRemove);
    topBtnLayout->addStretch();
    leftLayout->addLayout(topBtnLayout);

    // 中间表格
    agendaTable = new QTableWidget(this);
    agendaTable->setColumnCount(7);  // 7列：标题、设备、开始、截止、故障、状态、专家
    agendaTable->setHorizontalHeaderLabels({"标题", "设备", "开始时间", "截止时间", "故障描述", "状态", "专家"});
    agendaTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自适应列宽
    agendaTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    agendaTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可直接编辑
    leftLayout->addWidget(agendaTable);

    // 右边加入按钮
    QVBoxLayout *rightLayout = new QVBoxLayout;
    btnJoin = new QPushButton("加入");
    rightLayout->addStretch();
    rightLayout->addWidget(btnJoin);
    rightLayout->addStretch();

    // 拼接左右布局
    agendaLayout->addLayout(leftLayout, 4);
    agendaLayout->addLayout(rightLayout, 1);

    // 信号槽
    connect(btnAdd, &QPushButton::clicked, this, &AgendaPage::onAddAgenda);
    connect(btnRemove, &QPushButton::clicked, this, &AgendaPage::onRemoveAgenda);
    // 议程信息界面

}

void AgendaPage::onAddAgenda()
{
    AddAgendaDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        int row = agendaTable->rowCount();
        agendaTable->insertRow(row);

        agendaTable->setItem(row, 0, new QTableWidgetItem(dlg.getTitle()));
        agendaTable->setItem(row, 1, new QTableWidgetItem(dlg.getDevice()));
        agendaTable->setItem(row, 2, new QTableWidgetItem(dlg.getStartDate().toString("yyyy-MM-dd")));
        agendaTable->setItem(row, 3, new QTableWidgetItem(dlg.getEndDate().toString("yyyy-MM-dd")));
        agendaTable->setItem(row, 4, new QTableWidgetItem(dlg.getFaultDesc()));
        agendaTable->setItem(row, 5, new QTableWidgetItem(dlg.getStatus()));
        agendaTable->setItem(row, 6, new QTableWidgetItem(dlg.getExpert()));

        // TODO: 插入数据库
    }
}

void AgendaPage::onRemoveAgenda()
{
    int row = agendaTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "提示", "请先选择要删除的议程！");
        return;
    }
    agendaTable->removeRow(row);

    // TODO: 从数据库删除对应记录
}

void AgendaPage::updateAgendas(const QVector<Agenda>& agendas)
{

    qInfo() << "update Agenda";

    for (const Agenda &agenda : agendas) {
        qInfo() << agenda.orderId;

        bool alreadyExists = false;
        for (int i = 0; i < agendaTable->rowCount(); ++i) {
            if (agendaTable->item(i, 0) && agendaTable->item(i, 0)->data(Qt::UserRole).toInt() == agenda.orderId) {
                alreadyExists = true;
                break;
            }
        }

        if (alreadyExists) {
            qInfo() << "Agenda with orderId " << agenda.orderId << " already exists, skipping.";
            continue; // 如果已存在，则跳过当前议程
        }

        int row = agendaTable->rowCount();
        agendaTable->insertRow(row);

        // 设置各列数据
        agendaTable->setItem(row, 0, new QTableWidgetItem(QString::number(agenda.orderId)));
        agendaTable->setItem(row, 1, new QTableWidgetItem(QString::number(agenda.deviceId)));
        agendaTable->setItem(row, 2, new QTableWidgetItem(agenda.createTime.toString("yyyy-MM-dd hh:mm")));
        agendaTable->setItem(row, 3, new QTableWidgetItem(agenda.predictTime.toString("yyyy-MM-dd hh:mm")));
        agendaTable->setItem(row, 4, new QTableWidgetItem(agenda.fault));
        agendaTable->setItem(row, 5, new QTableWidgetItem(agenda.statusText()));
        agendaTable->setItem(row, 6, new QTableWidgetItem(QString::number(agenda.user2Id)));

        // 存储议程ID作为用户数据
        agendaTable->item(row, 0)->setData(Qt::UserRole, agenda.orderId);

        // 根据状态设置不同的颜色
        for (int col = 0; col < agendaTable->columnCount(); ++col) {
            QTableWidgetItem *item = agendaTable->item(row, col);
            if (agenda.status == 0) {
                item->setBackground(QColor(255, 255, 200)); // 浅黄色背景
            } else {
                item->setBackground(QColor(200, 255, 200)); // 浅绿色背景
            }
        }
    }
}
