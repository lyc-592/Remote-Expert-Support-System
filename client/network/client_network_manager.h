#ifndef CLIENT_NETWORK_MANAGER_H
#define CLIENT_NETWORK_MANAGER_H

#include "../../utils/constants.h"
#include "../../server/entities/user.h"
#include "../../server/entities/agenda.h"
#include "../../server/entities/device.h"
#include "../../server/entities/chat_message.h"
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
    void setUsernameRequest(const QString& username, int userId);
    void setUsernameRequest(const QString& username, const QString& password, int userId);
    void getUserRequest(const QString& username);
    void getAgendaRequest(const int userId, const UserType userType);
    void getContactRequest(const int userId);
    void getDeviceRequest(const int userId);

    void addAgendaRequest(int user1_id, const QString& expertname, const QString& device, const QString& ordername,
                          const QString &order_create_time, const QString &predict_time, const QString& status, const QString &fault);

    void addDeviceRequest(const QString& name, const QString& type, const QString& status, int userId);
    void removeDeviceRequest(const QString& deviceName, int userId);
    void sendMessage(const QByteArray &message);
    void sendChatMessageRequest(int senderId, int receiverId, const QString& message);
    void getChatHistoryRequest(int userId, int contactId);

    void addSimulateDataRequest(int deviceId, double temp, double pressure, const QString& otherData);
    void getNonContactUsersRequest(int userId);
    void addContactRequest(int user1Id, int user2Id);
    void getAllDevicesRequest();
    void updateDeviceRequest(const QString& deviceName, const QString& type,
                             const QString& status, int userId);


signals:
    // 信号：用户连接、断连、有误
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorMessage);
    // 用户接收到服务器返回的结果
    void loginResponseReceived(bool success, const QString &message);
    void registerResponseReceived(bool success, const QString& message);
    // 获取用户
    void setUserResponseReceived(bool success, const QString& message, User user);
    void getUserResponseReceived(bool success, const QString& message, User user);
    // 获取议程
    void getAgendaResponseReceived(bool success, const QVector<Agenda>& agendas, const QString& message);
    // 联系人
    void getContactResponseReceived(bool success, const QVector<User>& agendas, const QString& message);

    void getDeviceResponseReceived(bool successs, const QVector<Device>& devices, const QString& message);
    void addAgendaResponseReceived(bool success, const QString &message);
    void addDeviceResponseReceived(bool success, const QString& message);
    void removeDeviceResponseReceived(bool success, const QString& message);
    void sendChatMessageResponseReceived(bool success, const QString& message);
    void getChatHistoryResponseReceived(bool success, const QVector<ChatMessage>& history, const QString& message);

    void addSimulateDataResponseReceived(bool success, const QString& message);

    void nonContactUsersResponseReceived(const QList<User> &users);
    void addContactResponseReceived(bool success, const QString &message);
    void getAllDevicesResponseReceived(bool success, const QString& message, const QVector<Device>& devices);
    void updateDeviceResponseReceived(bool success, const QString& message);
    void newChatMessageReceived(int contactId, int senderId);
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
