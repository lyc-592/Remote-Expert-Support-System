#include "homepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include "cardwidget.h"



HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ===== 上部统计信息 =====
    statsLabel = new QLabel(this);
    statsLabel->setStyleSheet("font-size: 14px; font-weight: bold; margin: 5px;");
    mainLayout->addWidget(statsLabel);

    // ===== 中部图表卡片 =====
    cardFrame = new QFrame(this);
    cardFrame->setObjectName("Card");
    cardFrame->setStyleSheet(R"(
        QFrame#Card {
            background-color: #faf7ff;
            border-radius: 20px;
            border: 1px solid #e0d6f5;
        }
    )");
    QVBoxLayout *cardLayout = new QVBoxLayout(cardFrame);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
    cardLayout->addWidget(chartView);

    mainLayout->addWidget(cardFrame);

    // 下部三个跳转卡片
    QHBoxLayout *bottomLayout = new QHBoxLayout;

    // 三个卡片
    CardWidget *agendaCard = new CardWidget("议程", "查看最新议程");
    CardWidget *deviceCard = new CardWidget("设备", "管理与监控设备");
    CardWidget *historyCard = new CardWidget("历史", "查看历史记录");

    // 点击信号
    connect(agendaCard, &CardWidget::clicked, this, [this]() { emit gotoAgenda(); });
    connect(deviceCard, &CardWidget::clicked, this, [this]() { emit gotoDevice(); });
    connect(historyCard, &CardWidget::clicked, this, [this]() { emit gotoHistory(); });

    // 添加到底部布局
    bottomLayout->addWidget(agendaCard);
    bottomLayout->addWidget(deviceCard);
    bottomLayout->addWidget(historyCard);

    mainLayout->addLayout(bottomLayout);


    connect(agendaCard, &CardWidget::clicked, this, [this]() { emit gotoAgenda(); });
    connect(deviceCard, &CardWidget::clicked, this, [this]() { emit gotoDevice(); });
    connect(historyCard, &CardWidget::clicked, this, [this]() { emit gotoHistory(); });

    // ===== 加载数据库数据 =====
    loadStatsFromDb();
    loadDeviceDataFromDb();
}

void HomePage::loadStatsFromDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "StatsConn");
    db.setDatabaseName("default.db");

    if (!db.open()) {
        qDebug() << "数据库打开失败:" << db.lastError().text();
        statsLabel->setText("⚠️ 无法连接数据库");
        return;
    }

    int userCount = 0, deviceCount = 0;
    QSqlQuery query(db);

    if (query.exec("SELECT COUNT(*) FROM user") && query.next())
        userCount = query.value(0).toInt();

    if (query.exec("SELECT COUNT(*) FROM device") && query.next())
        deviceCount = query.value(0).toInt();

    db.close();

    statsLabel->setText(QString("👤 用户总数: %1   🖥️ 设备总数: %2")
                        .arg(userCount)
                        .arg(deviceCount));
}

void HomePage::loadDeviceDataFromDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "DeviceConn");
    db.setDatabaseName("default.db");

    if (!db.open()) {
        qDebug() << "数据库打开失败:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT device_id, temp, pressure FROM device_data")) {
        qDebug() << "查询失败:" << query.lastError().text();
        db.close();
        return;
    }

    QBarSet *setTemp = new QBarSet("温度 temp");
    QBarSet *setPressure = new QBarSet("压力 pressure");
    QStringList categories;

    while (query.next()) {
        int id = query.value(0).toInt();
        double temp = query.value(1).toDouble();
        double pressure = query.value(2).toDouble();

        categories << QString("设备 %1").arg(id);
        *setTemp << temp;
        *setPressure << pressure;
    }
    db.close();

    setTemp->setColor(QColor("#655B8D"));
    setPressure->setColor(QColor("#C8BFE7"));
    setTemp->setBorderColor(Qt::transparent);
    setPressure->setBorderColor(Qt::transparent);

    QBarSeries *series = new QBarSeries();
    series->append(setTemp);
    series->append(setPressure);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("设备温度与压力统计");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    //axisY->setTitleText("数值");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setAlignment(Qt::AlignTop);
    chart->legend()->setFont(QFont("Microsoft YaHei", 10));

    chartView->setChart(chart);
}

QWidget* HomePage::createCard(const QString &title, const QString &iconPath, const QString &style)
{
    QPushButton *card = new QPushButton;
    card->setStyleSheet(QString(
        "QPushButton { "
        "background-color: #EDE7F6;"   // 浅紫背景
        "border-radius: 20px;"
        "border: 2px solid #D1C4E9;"
        "padding: 0px;"               // 去掉内部留白
        "}"
        "QPushButton:hover { background-color: #D1C4E9; }"
        "QPushButton:pressed { background-color: #B39DDB; }"
    ));
    card->setFixedSize(300, 100);
    card->setFlat(true);              // 去掉按钮阴影
    card->setIcon(QIcon());           // 关键：清空按钮默认 icon


    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(10, 5, 10, 5);

    QLabel *iconLabel = new QLabel;
    QPixmap pix(iconPath);
    QPixmap scaled = pix.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap colored(scaled.size());
    colored.fill(Qt::transparent);

    // 用深紫色填充图标
    QPainter p(&colored);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap(0, 0, scaled);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(colored.rect(), QColor("#5E35B1")); // 深紫
    p.end();

    iconLabel->setPixmap(colored);
    iconLabel->setAlignment(Qt::AlignCenter);

    QLabel *textLabel = new QLabel(title);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #5E35B1;");

    layout->addStretch();
    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->addStretch();

    return card;
}

