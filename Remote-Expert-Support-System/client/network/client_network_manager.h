#ifndef CLIENT_NETWORK_MANAGER_H
#define CLIENT_NETWORK_MANAGER_H

#include "../../utils/constants.h"
#include "../../server/entities/user.h"
#include "../../server/entities/agenda.h"
#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/* 负责用户请求管理的具体实现类 */
class ClientNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientNetworkManager(QObject *parent = nullptr);
    ~ClientNetworkManager();

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected() const;

    // 发送请求
    void loginRequest(const QString &username, const QString &password);
    void registerRequest(const QString& username, const QString& password, const QString& contact,
                         const UserType& usertype=FactoryUser, const QString &expertSkill="");
    void getUserRequest(const QString& username);
    void getAgendaRequest(const int userId, const UserType userType);
    void getContactRequest(const int userId);

    void sendMessage(const QByteArray &message);

signals:
    // 信号：用户连接、断连、有误
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorMessage);
    // 用户接收到服务器返回的结果
    void loginResponseReceived(bool success, const QString &message);
    void registerResonpseReceived(bool success, const QString& message);
    // 获取用户
    void getUserResponseReceived(bool success, const QString& message, User user);
    // 获取议程
    void getAgendaResponseReceived(bool success, const QVector<Agenda>& agendas, const QString& message);
    // 联系人
    void getContactResponseReceived(bool success, const QVector<User>& agendas, const QString& message);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void onReadyRead();

private:
    QTcpSocket *tcpSocket;
    QMap<QString, std::function<void(const QJsonObject&)>> pendingRequests;
    quint32 requestIdCounter;
    QByteArray responseBuffer;

    void sendJsonRequest(const QJsonObject &request);
};

#endif // CLIENT_NETWORK_MANAGER_H
