#ifndef CLIENT_H
#define CLIENT_H

#include "../network/client_network_manager.h"
#include "../../utils/constants.h"
#include <QObject>

class Client : public QObject {
    Q_OBJECT
public:
    Client(const QString& username="", const QString& userId="", const QString& password="");
    ~Client();
    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected();

    // 添加获取用户信息的方法
    QString getUsername() const { return username; }
    QString getUserId() const { return userId; }
    // 注册登录的入口
    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password,
                  const QString& contact, const UserType& usertype=FactoryUser, const QString& expertSkill="");
    // 发送信息
    void sendMessage(const QByteArray &message);
signals:
    void connected();           // 连接成功信号
    void disconnected();        // 断开连接信号
    void connectionFailed();    // 连接失败信号
    void messageReceived(const QByteArray &message);

    void loginResult(bool success, const QString& message);
    void registerResult(bool success, const QString& message);

    void expertsLoaded(const QList<QPair<int, QString>> &experts);
    void addAgendaResult(bool success, const QString &message, const QJsonObject &obj);


    void devicesLoaded(const QList<QPair<int, QString>> &list);




private slots:
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString &errorMsg);
    void onLoginResponseReceived(bool success, const QString& message);
    void onRegisterResponseReceived(bool success, const QString& message);

    void onMessageReceived(const QByteArray &message);

private:
    ClientNetworkManager *clientNetworkManager;
    QString username;
    QString userId;
    QString password; // 可能不应该写在这里
};

#endif // CLIENT_H
