#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QToolButton>
#include <QPushButton>
#include <QListWidget>
#include <QVector>
#include "server/db/database_manager.h"

// 前置声明 ChatDialog
class ChatDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUsername(const QString &username);

private slots:
    void onMenuChanged(int index);
    void onContactClicked(QListWidgetItem *item);

private:
    void loadStyleSheet();
    void setupContactsSidebar();
    void setupDatabase();
    QStackedWidget *stackedWidget;
    QVector<QToolButton*> buttons;
    DatabaseManager *dbManager;

    // === 议程页相关控件 ===
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;
    QListWidget *agendaList;

    // === 右侧固定联系人侧边栏 ===
    QWidget *contactsSidebar = nullptr;
    QListWidget *contactList = nullptr;

    // === 聊天对话框管理 ===
    QMap<QString, ChatDialog*> chatDialogs;
};

#endif // MAINWINDOW_H
