#ifndef DATA_VIEWER_H
#define DATA_VIEWER_H

#include <QtCharts>
#include <QVector>
#include <QDateTime>

class DataViewer : public QWidget
{
    Q_OBJECT

public:
    DataViewer(QWidget *parent = nullptr);
    ~DataViewer();
    // void initDataViewer();
public slots:
    void setDeviceList(const QStringList& devices);
    void updateTemperatureChart(const QVector<QDateTime>& times, const QVector<double>& temps);
    void updatePressureChart(const QVector<QDateTime>& times, const QVector<double>& pressures);

private slots:
    void onDeviceSelected(int index);
    void onShowRecords();

signals:
    void requestDeviceList();
    void requestDeviceData(const QString& deviceId);
    void requestDeviceRecords(const QString& deviceId);

private:
    void setupUI();
    void updateChart(QChart* chart, const QVector<QDateTime>& times, const QVector<double>& values, const QString& yTitle);

    // UI组件
    QComboBox *deviceComboBox;
    QChartView *temperatureChartView;
    QChartView *pressureChartView;
    QChart *temperatureChart;
    QChart *pressureChart;
    // 数据
    QStringList deviceList;
    QString currentDeviceId;
};
#endif // DATA_VIEWER_H
