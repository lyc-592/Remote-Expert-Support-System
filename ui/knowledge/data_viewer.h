#ifndef DATA_VIEWER_H
#define DATA_VIEWER_H

#include <QWidget>
#include <QComboBox>
#include <QDateTime>
#include <QVector>

// ⭐ 正确引入 QtCharts 头文件
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE   // 使用 QtCharts 命名空间

    class DataViewer : public QWidget
{
    Q_OBJECT
public:
    explicit DataViewer(QWidget *parent = nullptr);
    ~DataViewer();

private slots:
    void onDeviceSelected(int index);
    void onShowRecords();

private:
    void setupUI();

    // 更新图表的通用函数
    void updateChart(QChart* chart, const QVector<QDateTime>& times,
                     const QVector<double>& values, const QString& yTitle);

    // 分别更新温度和压强折线图
    void updateTemperatureChart(const QVector<QDateTime>& times,
                                const QVector<double>& temps);
    void updatePressureChart(const QVector<QDateTime>& times,
                             const QVector<double>& pressures);

    // 从数据库加载设备列表 & 设备数据
    void loadDeviceListFromDb();
    void loadDeviceDataFromDb(int deviceId);

    // ⭐ 新增：显示设备信息弹窗
    void showDeviceInfo(int deviceId);

private:
    QComboBox *deviceComboBox;
    QChart *temperatureChart;
    QChartView *temperatureChartView;
    QChart *pressureChart;
    QChartView *pressureChartView;

    int currentDeviceId = -1;   // 当前选中的设备ID
};

#endif // DATA_VIEWER_H
