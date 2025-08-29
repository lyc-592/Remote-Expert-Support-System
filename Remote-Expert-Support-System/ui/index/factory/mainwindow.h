#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolButton>
#include <QPushButton>
#include <QListWidget>
#include <QVector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMenuChanged(int index);

private:
    QStackedWidget *stackedWidget;
    QVector<QToolButton*> buttons;

    // === 议程页相关控件 ===
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;
    QListWidget *agendaList;
};

#endif // MAINWINDOW_H
