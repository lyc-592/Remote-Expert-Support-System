#include "agendapage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "addagendadialog.h"
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
