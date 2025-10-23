#include "ui/knowledge/data_viewer.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

QT_CHARTS_USE_NAMESPACE

// ==========================
// 工具函数: 创建卡片式图表容器
// ==========================
QFrame* createChartCard(QChartView *chartView, const QString &title) {
    QFrame *card = new QFrame();
    card->setStyleSheet(R"(
        QFrame {
            background-color: #faf7ff;
            border-radius: 15px;
            border: 1px solid #e0d6f5;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(card);

    QLabel *label = new QLabel(title);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 14px; font-weight: bold; margin: 5px;");

    layout->addWidget(label);
    layout->addWidget(chartView);

    return card;
}

DataViewer::DataViewer(QWidget *parent) : QWidget(parent)
{
    setupUI();
    loadDeviceListFromDb();   // ⭐ 初始化时加载设备列表
}

DataViewer::~DataViewer() {}

void DataViewer::onDeviceSelected(int index)
{
    if (index >= 0 && index < deviceComboBox->count()) {
        currentDeviceId = deviceComboBox->itemData(index).toInt();
        qDebug() << "选中设备ID:" << currentDeviceId;
        loadDeviceDataFromDb(currentDeviceId);  // ⭐ 选中设备时加载数据
    }
}

void DataViewer::onShowRecords()
{
    if (currentDeviceId <= 0) {
        QMessageBox::information(this, "提示", "请先选择一个设备");
        return;
    }
    // 刷新图表
    loadDeviceDataFromDb(currentDeviceId);
    // 显示设备信息弹窗
    showDeviceInfo(currentDeviceId);
}

void DataViewer::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 顶部选择栏
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QLabel *deviceLabel = new QLabel("选择设备:");
    deviceComboBox = new QComboBox();
    controlLayout->addWidget(deviceLabel);
    controlLayout->addWidget(deviceComboBox);

    QPushButton *refreshButton = new QPushButton("刷新数据");
    controlLayout->addWidget(refreshButton);
    connect(refreshButton, &QPushButton::clicked, [this]() {
        if (currentDeviceId > 0) {
            loadDeviceDataFromDb(currentDeviceId);
        }
    });

    mainLayout->addLayout(controlLayout);

    // 图表区
    QHBoxLayout *chartsLayout = new QHBoxLayout();

    temperatureChart = new QChart();
    temperatureChartView = new QChartView(temperatureChart);
    temperatureChartView->setRenderHint(QPainter::Antialiasing);

    pressureChart = new QChart();
    pressureChartView = new QChartView(pressureChart);
    pressureChartView->setRenderHint(QPainter::Antialiasing);

    chartsLayout->addWidget(createChartCard(temperatureChartView, "温度-时间变化"));
    chartsLayout->addWidget(createChartCard(pressureChartView, "压强-时间变化"));

    mainLayout->addLayout(chartsLayout);

    // 底部按钮
    QPushButton *recordsButton = new QPushButton("查看记录");
    recordsButton->setStyleSheet(R"(
        QPushButton {
            background-color: #8e6fdb;
            color: white;
            border-radius: 8px;
            font-size: 14px;
            padding: 6px 12px;
        }
        QPushButton:hover { background-color: #7a5fc4; }
        QPushButton:pressed { background-color: #5e46a1; }
    )");
    mainLayout->addWidget(recordsButton);
    connect(recordsButton, &QPushButton::clicked, this, &DataViewer::onShowRecords);

    // 设备下拉变化 → 查数据
    connect(deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DataViewer::onDeviceSelected);

    setLayout(mainLayout);
    resize(1200, 700);
}

void DataViewer::updateChart(QChart* chart, const QVector<QDateTime>& times,
                             const QVector<double>& values, const QString& yTitle)
{
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());

    if (times.isEmpty() || values.isEmpty()) {
        qDebug() << "updateChart: 没有数据，跳过绘图";
        return;
    }

    QLineSeries *series = new QLineSeries();
    series->setName(yTitle);

    for (int i = 0; i < times.size(); ++i) {
        series->append(times[i].toMSecsSinceEpoch(), values[i]);
    }

    chart->addSeries(series);

    // X轴：时间
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("MM-dd hh:mm");
    axisX->setTitleText("时间");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y轴：数值
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(yTitle);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    qDebug() << "updateChart: 绘制完成 -" << yTitle << "数据点数:" << values.size();
}

void DataViewer::updateTemperatureChart(const QVector<QDateTime>& times,
                                        const QVector<double>& temps)
{
    updateChart(temperatureChart, times, temps, "温度 (°C)");
}

void DataViewer::updatePressureChart(const QVector<QDateTime>& times,
                                     const QVector<double>& pressures)
{
    updateChart(pressureChart, times, pressures, "压强 (kPa)");
}

void DataViewer::loadDeviceListFromDb()
{
    deviceComboBox->clear();

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "DeviceListConn");
        db.setDatabaseName("default.db");

        if (!db.open()) {
            QMessageBox::critical(this, "数据库错误", "无法打开数据库: " + db.lastError().text());
            qDebug() << "数据库打开失败:" << db.lastError().text();
            return;
        }

        QSqlQuery query(db);
        // ⭐ 使用 device_id 作为主键
        if (query.exec("SELECT device_id, device_name FROM device")) {
            while (query.next()) {
                int id = query.value(0).toInt();       // device_id
                QString name = query.value(1).toString(); // device_name
                deviceComboBox->addItem(name, id);
                qDebug() << "加载设备:" << id << name;
            }
        } else {
            QMessageBox::warning(this, "查询失败", "查询设备失败: " + query.lastError().text());
            qDebug() << "查询设备失败:" << query.lastError().text();
        }

        db.close();
    }

    QSqlDatabase::removeDatabase("DeviceListConn"); // ⭐ 必须放在 db 析构之后

    qDebug() << "设备数量:" << deviceComboBox->count();

    if (deviceComboBox->count() > 0) {
        currentDeviceId = deviceComboBox->itemData(0).toInt();
        deviceComboBox->setCurrentIndex(0);
        qDebug() << "默认选中设备 ID:" << currentDeviceId;
    } else {
        QMessageBox::information(this, "提示", "没有查询到任何设备，请检查数据库数据。");
    }
}

void DataViewer::loadDeviceDataFromDb(int deviceId)
{
    struct Record {
        QDateTime time;
        double temp;
        double pressure;
    };

    QVector<Record> records;

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "DeviceDataConn");
        db.setDatabaseName("default.db");

        if (!db.open()) {
            QMessageBox::critical(this, "数据库错误", "无法打开数据库: " + db.lastError().text());
            qDebug() << "数据库打开失败:" << db.lastError().text();
            return;
        }

        QSqlQuery query(db);
        query.prepare("SELECT data_time, temp, pressure FROM device_data "
                      "WHERE device_id = ?");
        query.addBindValue(deviceId);

        if (query.exec()) {
            while (query.next()) {
                QString rawTime = query.value(0).toString();
                QDateTime t;

                // ⭐ 尝试多种格式解析
                QStringList formats = {
                    "yyyy-MM-dd hh:mm:ss",
                    "yyyy-MM-ddThh:mm:ss",
                    "yyyy/M/d",
                    "yyyy/M/d hh:mm",
                    "yyyy/MM/dd",
                    "yyyy/MM/dd hh:mm"
                };
                for (const QString &fmt : formats) {
                    t = QDateTime::fromString(rawTime, fmt);
                    if (t.isValid()) break;
                }

                if (!t.isValid()) {
                    qDebug() << "⚠️ 时间解析失败:" << rawTime;
                } else {
                    Record r;
                    r.time = t;
                    r.temp = query.value(1).toDouble();
                    r.pressure = query.value(2).toDouble();
                    records.append(r);

                    qDebug() << "记录:" << rawTime
                             << "解析后:" << t.toString("yyyy-MM-dd hh:mm")
                             << "温度:" << r.temp << "压强:" << r.pressure;
                }
            }
        } else {
            QMessageBox::warning(this, "查询失败", "查询设备数据失败: " + query.lastError().text());
            qDebug() << "查询设备数据失败:" << query.lastError().text();
        }

        db.close();
    }
    QSqlDatabase::removeDatabase("DeviceDataConn");

    // ⭐ 关键：在 C++ 层排序，保证时间顺序正确
    std::sort(records.begin(), records.end(), [](const Record &a, const Record &b) {
        return a.time < b.time;
    });

    QVector<QDateTime> times;
    QVector<double> temps, pressures;
    for (const Record &r : records) {
        times.append(r.time);
        temps.append(r.temp);
        pressures.append(r.pressure);
    }

    qDebug() << "查询到数据条数(排序后):" << times.size();

    updateTemperatureChart(times, temps);
    updatePressureChart(times, pressures);
}


// ==========================
// 新增: 显示设备信息弹窗
// ==========================
void DataViewer::showDeviceInfo(int deviceId)
{
    if (deviceId <= 0) return;

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "DeviceInfoConn");
        db.setDatabaseName("default.db");

        if (!db.open()) {
            QMessageBox::critical(this, "数据库错误", db.lastError().text());
            return;
        }

        QSqlQuery query(db);

        // 先查设备表
        query.prepare("SELECT device_id, user_id, device_name, device_type, status "
                      "FROM device WHERE device_id = ?");
        query.addBindValue(deviceId);

        QString info;
        if (query.exec() && query.next()) {
            int devId = query.value(0).toInt();
            int userId = query.value(1).toInt();
            QString name = query.value(2).toString();
            QString type = query.value(3).toString();
            QString status = query.value(4).toString();

            info = QString("设备 ID: %1\n用户 ID: %2\n设备名称: %3\n设备类型: %4\n状态: %5\n\n历史数据：\n")
                       .arg(devId)
                       .arg(userId)
                       .arg(name)
                       .arg(type)
                       .arg(status);
        } else {
            QMessageBox::warning(this, "提示", "未找到该设备信息。");
            db.close();
            QSqlDatabase::removeDatabase("DeviceInfoConn");
            return;
        }

        // 再查 device_data 表，按时间顺序排序
        query.prepare("SELECT data_time, temp, pressure, other_data "
                      "FROM device_data WHERE device_id = ? ORDER BY data_time ASC");
        query.addBindValue(deviceId);

        if (query.exec()) {
            while (query.next()) {
                QString time = query.value(0).toString();
                double temp = query.value(1).toDouble();
                double pressure = query.value(2).toDouble();
                QString other = query.value(3).toString();

                info += QString("时间: %1, 温度: %2, 压力: %3, 状态: %4\n")
                            .arg(time)
                            .arg(temp)
                            .arg(pressure)
                            .arg(other);
            }
        }

        QMessageBox::information(this, "设备信息", info);

        db.close();
    }

    QSqlDatabase::removeDatabase("DeviceInfoConn");
}
