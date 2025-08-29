#ifndef CLIENT_H
#define CLIENT_H

#include "clientnetworkmanager.h"
#include <QObject>

class Client : public QObject {
    Q_OBJECT
public:
    Client(const QString& username, const QString& userId, const QString& password);
    ~Client();
    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected();

    // 添加获取用户信息的方法
    QString getUsername() const { return username; }
    QString getUserId() const { return userId; }

    void sendMessage(const QByteArray &message);
signals:
    void connected();           // 连接成功信号
    void disconnected();        // 断开连接信号
    void connectionFailed();    // 连接失败信号
    void messageReceived(const QByteArray &message);
private slots:
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString &errorMsg);

private:
    ClientNetworkManager *clientNetworkManager;
    QString username;
    QString userId;
    QString password; // 可能不应该写在这里
};

#endif // CLIENT_H
