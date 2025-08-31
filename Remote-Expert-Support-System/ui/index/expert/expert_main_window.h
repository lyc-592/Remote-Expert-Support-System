#ifndef EXPERT_MAIN_WINDOW_H
#define EXPERT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolButton>
#include <QVector>

class ExpertMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExpertMainWindow(QWidget *parent = nullptr);
    ~ExpertMainWindow();

private slots:
    void onMenuChanged(int index);

private:
    QStackedWidget *stackedWidget;
    QVector<QToolButton*> buttons;
};

#endif // EXPERT_MAIN_WINDOW_H
