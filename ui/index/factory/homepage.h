#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

class HomePage : public QWidget {
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

signals:
    void gotoAgenda();  // 跳转议程
    void gotoDevice();  // 跳转设备
    void gotoHistory(); // 跳转历史

private:
    void loadStatsFromDb();        // 显示总数信息
    void loadDeviceDataFromDb();   // 显示柱状图
    QWidget* createCard(const QString &title, const QString &iconPath, const QString &style);
    QTimer *timer;

    QLabel *statsLabel;
    QChartView *chartView;
    QFrame *cardFrame;
};

#endif // HOMEPAGE_H
