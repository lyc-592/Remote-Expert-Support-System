#include "expert_agenda_page.h"
#include "../../client/core/client.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "../../server/entities/agenda.h"
#include "../../ui/conference/conference_ui.h"
#include "../../ui/conference/conferencewindow.h"
#include "../../ui/conference/mainconferencewindow.h"
#include <QHeaderView>
#include <QException>

ExpertAgendaPage::ExpertAgendaPage(QWidget *parent)
    : QWidget(parent)//, m_client(client)
{
    setupUI();
}

void ExpertAgendaPage::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左边内容 - 工单表格
    QVBoxLayout *leftLayout = new QVBoxLayout;

    agendaTable = new QTableWidget(this);
    agendaTable->setColumnCount(7);
    agendaTable->setHorizontalHeaderLabels({"标题", "设备", "开始时间", "截止时间", "故障描述", "状态", "工厂"});
    agendaTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    agendaTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    agendaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    agendaTable->setSelectionMode(QAbstractItemView::SingleSelection);
    leftLayout->addWidget(agendaTable);

    // 右边加入按钮
    QVBoxLayout *rightLayout = new QVBoxLayout;
    btnJoin = new QPushButton("加入");
    rightLayout->addStretch();
    rightLayout->addWidget(btnJoin);
    rightLayout->addStretch();

    // 拼接左右布局
    mainLayout->addLayout(leftLayout, 4);
    mainLayout->addLayout(rightLayout, 1);

    // 议程信息界面
    connect(btnJoin, &QPushButton::clicked, this, &ExpertAgendaPage::attendOrder);
}

void ExpertAgendaPage::updateAgendas(const QVector<Agenda>& agendas)
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
        agendaTable->setItem(row, 6, new QTableWidgetItem(QString::number(agenda.user1Id)));

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


void ExpertAgendaPage::attendOrder() {
    // 选择议程
    QList<QTableWidgetItem*> selectedItems = agendaTable->selectedItems();

    // 检查是否只选中了一条议程
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择要加入的议程！");
        return;
    }

    // 获取选中行的开始时间（开始时间在表格中索引为2）
    int row = agendaTable->row(selectedItems.first());
    QTableWidgetItem* startTimeItem = agendaTable->item(row, 2);

    // 解析开始时间
    QDateTime startTime = QDateTime::fromString(startTimeItem->text(), "yyyy-MM-dd hh:mm");
    QDateTime currentTime = QDateTime::currentDateTime();

    // 检查当前时间是否晚于开始时间
    if (currentTime < startTime) {
        QMessageBox::warning(this, "提示", "当前时间早于议程开始时间，无法加入！");
        return;
    }

    // 满足所有条件后进入聊天
    qRegisterMetaType<QCameraInfo>();
    MainConferenceWindow *w = new MainConferenceWindow();
    try {
        w->setWindowTitle("视频聊天系统 (Qt5版本)");
        w->setAttribute(Qt::WA_DeleteOnClose); // 设置窗口关闭时自动删除
        w->show();
    } catch(QException) {
        w->close();
        delete w;
    }
}
