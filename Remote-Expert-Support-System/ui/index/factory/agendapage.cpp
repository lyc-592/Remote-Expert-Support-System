#include "ui/index/factory/agendapage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "ui/index/factory/addagendadialog.h"
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AgendaPage::AgendaPage(Client *c, QWidget *parent)
    : QWidget(parent), client(c)
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

    // ===== 信号槽 =====
    connect(btnAdd, &QPushButton::clicked, this, &AgendaPage::onAddAgenda);
    connect(btnRemove, &QPushButton::clicked, this, &AgendaPage::onRemoveAgenda);

    // 缓存专家列表
    connect(client, &Client::expertsLoaded, this,
        [this](const QList<QPair<int, QString>> &list){
            experts = list;
        }
    );

    // 缓存设备列表
    connect(client, &Client::devicesLoaded, this,
        [this](const QList<QPair<int, QString>> &list){
            devices = list;
        }
    );

    // 监听新增议程结果
    connect(client, &Client::addAgendaResult, this,
        [this](bool success, const QString &msg, const QJsonObject &obj){
            if (success) {
                int row = agendaTable->rowCount();
                agendaTable->insertRow(row);
                agendaTable->setItem(row, 0, new QTableWidgetItem(obj["title"].toString()));
                agendaTable->setItem(row, 1, new QTableWidgetItem(obj["device_name"].toString())); // ✅ 展示设备名
                agendaTable->setItem(row, 2, new QTableWidgetItem(obj["startDate"].toString()));
                agendaTable->setItem(row, 3, new QTableWidgetItem(obj["endDate"].toString()));
                agendaTable->setItem(row, 4, new QTableWidgetItem(obj["fault"].toString()));
                agendaTable->setItem(row, 5, new QTableWidgetItem(obj["status"].toString()));

                QStringList expertNames;
                for (auto e : obj["experts"].toArray())
                    expertNames << e.toString();  // ✅ 展示专家名字
                agendaTable->setItem(row, 6, new QTableWidgetItem(expertNames.join(",")));
            } else {
                QMessageBox::warning(this, "失败", msg);
            }
        }
    );
}

void AgendaPage::onAddAgenda()
{
    // 请求服务端刷新专家和设备列表
    QJsonObject req1; req1["type"] = "getExperts"; client->sendMessage(QJsonDocument(req1).toJson());
    QJsonObject req2; req2["type"] = "getDevices"; client->sendMessage(QJsonDocument(req2).toJson());

    // 🔑 检查是否已经加载到数据（避免空弹窗）
    if (experts.isEmpty() || devices.isEmpty()) {
        QMessageBox::information(this, "提示", "正在加载专家和设备列表，请稍后再试");
        return;
    }

    // 打开新增议程对话框
    AddAgendaDialog dlg(this);
    dlg.setExperts(experts);   // 专家：显示名字，存 ID
    dlg.setDevices(devices);   // 设备：显示名字，存 ID

    if (dlg.exec() == QDialog::Accepted) {
        // 构造 JSON 请求
        QJsonObject request;
        request["type"] = "addAgenda";
        request["title"] = dlg.getTitle();
        request["device_id"] = dlg.getDeviceId();   // 传设备 ID
        request["startDate"] = dlg.getStartDate().toString("yyyy-MM-dd");
        request["endDate"] = dlg.getEndDate().toString("yyyy-MM-dd");
        request["fault"] = dlg.getFaultDesc();
        request["status"] = dlg.getStatus();
        request["user1_id"] = client->getUserId().toInt();   // 工厂当前用户 ID
        request["experts"] = QJsonArray::fromStringList(dlg.getExpertIds()); // 专家 ID 列表

        // 发送到服务端
        client->sendMessage(QJsonDocument(request).toJson());
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

    // TODO: 发 deleteAgenda 请求到服务端删除数据库记录
}
