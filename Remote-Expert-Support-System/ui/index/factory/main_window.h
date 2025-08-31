#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../../client/core/client.h"
#include "agenda_page.h"
#include "chat_dialog.h"
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
    explicit MainWindow(Client* client, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMenuChanged(int index);
    void onIndexChanged(int index);
    void onAgendaReceived(bool success, const QVector<Agenda>& agendas, const QString& message);
    void onContactClicked(QListWidgetItem *item);
    void onContactReceived(bool success, const QVector<User>& contacts, const QString& message);
private:
    void setupContactsSidebar();
    void loadStyleSheet();

    QStackedWidget *stackedWidget;
    QVector<QToolButton*> buttons;

    // === 议程页相关控件 ===
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;

    AgendaPage *agendaPage;

    // === 右侧固定联系人侧边栏 ===
    QVector<User> rawContacts;
    QWidget *contactsSidebar = nullptr;
    QListWidget *contactList = nullptr;

    // === 聊天对话框管理 ===
    QMap<QString, ChatDialog*> chatDialogs;
    // 用户
    Client* client;
};

#endif // MAIN_WINDOW_H
