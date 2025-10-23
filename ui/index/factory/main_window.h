#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../../client/core/client.h"
#include "agenda_page.h"
#include "device_page.h"
#include "user_page.h"
#include "history_order.h"
#include "chat_dialog.h"
#include "../../server/entities/chat_message.h"
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
    QVector<User> getCurrentContacts() {
        return rawContacts;
    }

private slots:
    void onMenuChanged(int index);
    void onIndexChanged(int index);
    void onAgendaReceived(bool success, const QVector<Agenda>& agendas, const QString& message);
    void onContactClicked(QListWidgetItem *item);
    void onContactReceived(bool success, const QVector<User>& contacts, const QString& message);
    void onDeviceReceived(bool success, const QVector<Device>& devices, const QString& message);
    void onAddDeviceReceived(bool success, const QString& message);
    void onAddAgendaReceived(bool success, const QString& message);
    void onRemoveDeviceReceived(bool success, const QString& message);
    void onAddSimulateDataReceived(bool success, const QString& message);
    void onAddContactButtonClicked();
    void onNonContactUsersReceived(const QList<User> &users);
    void onAddContactResponseReceived(bool success, const QString &message);
    void onUpdateDeviceReceived(bool success, const QString& message);
    void onNewChatMessage(int chatId, int senderId);
private:
    void setupContactsSidebar();
    void loadStyleSheet();
    void setupDatabase();
    void onSendChatMessage(int receiverId, const QString& message);

    QStackedWidget *stackedWidget;
    QVector<QToolButton*> buttons;

    UserPage *userPage;

    // === 议程页相关控件 ===
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnJoin;

    AgendaPage *agendaPage;

    // === 右侧固定联系人侧边栏 ===
    QVector<User> rawContacts;
    QWidget *contactsSidebar = nullptr;
    QListWidget *contactList = nullptr;

    DevicePage *devicePage = nullptr;

    HistoryOrder *historyPage = nullptr;

    // === 聊天对话框管理 ===
    QMap<QString, ChatDialog*> chatDialogs;
    // 用户
    Client* client;

    // 前！后！端！应！该！分！离！
    DatabaseManager *dbManager;
};

#endif // MAIN_WINDOW_H
