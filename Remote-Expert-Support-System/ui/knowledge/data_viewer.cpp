#include "../../ui/knowledge/data_viewer.h"
#include <QtCharts>

DataViewer::DataViewer(QWidget *parent) : QWidget(parent)
{
    // 初始化UI和数据库
    setupUI();
}

DataViewer::~DataViewer()
{

}

void DataViewer::onDeviceSelected(int index)
{
    if (index >= 0 && index < deviceList.size()) {
        currentDeviceId = deviceList[index];
        emit requestDeviceData(currentDeviceId);
    }
}

void DataViewer::onShowRecords()
{
    if (currentDeviceId.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择一个设备");
        return;
    }

    emit requestDeviceRecords(currentDeviceId);
}

void DataViewer::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 顶部控制区域
    QHBoxLayout *controlLayout = new QHBoxLayout();

    // 设备选择
    QLabel *deviceLabel = new QLabel("选择设备:");
    deviceComboBox = new QComboBox();
    controlLayout->addWidget(deviceLabel);
    controlLayout->addWidget(deviceComboBox);

    // 刷新按钮
    QPushButton *refreshButton = new QPushButton("刷新数据");
    controlLayout->addWidget(refreshButton);
    connect(refreshButton, &QPushButton::clicked, [this]() {
        if (!currentDeviceId.isEmpty()) {
            emit requestDeviceData(currentDeviceId);
        }
    });

    controlLayout->addStretch();
    mainLayout->addLayout(controlLayout);

    // 图表区域
    QHBoxLayout *chartsLayout = new QHBoxLayout();

    // 温度图表
    temperatureChart = new QChart();
    temperatureChartView = new QChartView(temperatureChart);
    temperatureChartView->setRenderHint(QPainter::Antialiasing);
    temperatureChart->setTitle("温度-时间变化");
    chartsLayout->addWidget(temperatureChartView);

    // 压强图表
    pressureChart = new QChart();
    pressureChartView = new QChartView(pressureChart);
    pressureChartView->setRenderHint(QPainter::Antialiasing);
    pressureChart->setTitle("压强-时间变化");
    chartsLayout->addWidget(pressureChartView);

    mainLayout->addLayout(chartsLayout);

    // 底部按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    // 记录查看按钮
    QPushButton *recordsButton = new QPushButton("查看记录");
    buttonLayout->addWidget(recordsButton);
    connect(recordsButton, &QPushButton::clicked, this, &DataViewer::onShowRecords);

    mainLayout->addLayout(buttonLayout);

    // 连接信号
    connect(deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DataViewer::onDeviceSelected);

    // 设置窗口属性
    setWindowTitle("工厂设备数据可视化系统");
    resize(1200, 700);
}

void DataViewer::updateChart(QChart* chart, const QVector<QDateTime>& times, const QVector<double>& values, const QString& yTitle)
{
    chart->removeAllSeries();

    if (times.isEmpty() || values.isEmpty() || times.size() != values.size()) {
        return;
    }

    QLineSeries *series = new QLineSeries();
    series->setName(yTitle);

    for (int i = 0; i < times.size(); ++i) {
        series->append(times[i].toMSecsSinceEpoch(), values[i]);
    }

    chart->addSeries(series);

    // 设置X轴为时间轴
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("MM-dd hh:mm");
    axisX->setTitleText("时间");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 设置Y轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(yTitle);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
}

void DataViewer::setDeviceList(const QStringList& devices)
{
    deviceList = devices;
    deviceComboBox->clear();
    deviceComboBox->addItems(deviceList);

    if (!deviceList.isEmpty()) {
        currentDeviceId = deviceList.first();
    }
}

void DataViewer::updateTemperatureChart(const QVector<QDateTime>& times, const QVector<double>& temps)
{
    updateChart(temperatureChart, times, temps, "温度 (°C)");
}

void DataViewer::updatePressureChart(const QVector<QDateTime>& times, const QVector<double>& pressures)
{
    updateChart(pressureChart, times, pressures, "压强 (Pa)");
}

void updateChart(QChart* chart, const QVector<QDateTime>& times, const QVector<double>& values, const QString& yTitle)
{
    chart->removeAllSeries();

    if (times.isEmpty() || values.isEmpty() || times.size() != values.size()) {
        return;
    }

    QLineSeries *series = new QLineSeries();
    series->setName(yTitle);

    for (int i = 0; i < times.size(); ++i) {
        series->append(times[i].toMSecsSinceEpoch(), values[i]);
    }

    chart->addSeries(series);

    // 设置X轴为时间轴
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("MM-dd hh:mm");
    axisX->setTitleText("时间");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 设置Y轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(yTitle);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
}
