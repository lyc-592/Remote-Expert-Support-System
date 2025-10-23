#ifndef EXPERT_MAIN_WINDOW_H
#define EXPERT_MAIN_WINDOW_H


#include "../../client/core/client.h"
#include "expert_agenda_page.h"
#include "expert_device_page.h"
#include "../../ui/index/factory/user_page.h"
#include "expert_history_order.h"
#include "../../ui/index/factory/chat_dialog.h"
#include <QMainWindow>
#include <QStackedWidget>
#include <QToolButton>
#include <QPushButton>
#include <QListWidget>
#include <QVector>

class ExpertMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExpertMainWindow(Client* client, QWidget *parent = nullptr);
    ~ExpertMainWindow();
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
    void onGetAgendaResponse(bool success, const QVector<Agenda>& agendas, const QString& message);
    // void onAddDeviceReceived(bool success, const QString& message);
    // void onAddAgendaReceived(bool success, const QString& message);
    // void onRemoveDeviceReceived(bool success, const QString& message);
    void onAddContactButtonClicked();
    void onNonContactUsersReceived(const QList<User> &users);
    void onAddContactResponseReceived(bool success, const QString &message);
    void onNewChatMessage(int contactId, int senderId);
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

    ExpertAgendaPage *agendaPage;
    ExpertDevicePage *devicePage;

    // === 右侧固定联系人侧边栏 ===
    QVector<User> rawContacts;
    QWidget *contactsSidebar = nullptr;
    QListWidget *contactList = nullptr;

    // ExpertDevicePage *devicePage = nullptr;

    ExpertHistoryOrder *historyPage = nullptr;

    // === 聊天对话框管理 ===
    QMap<QString, ChatDialog*> chatDialogs;
    // 用户
    Client* client;

    // 前！后！端！应！该！分！离！
    DatabaseManager *dbManager;
};

#endif // EXPERT_MAIN_WINDOW_H
