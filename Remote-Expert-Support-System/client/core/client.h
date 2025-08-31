#ifndef CLIENT_H
#define CLIENT_H

#include "../network/client_network_manager.h"
#include "../../utils/constants.h"
#include "../../server/entities/user.h"
#include <QObject>

class Client : public QObject {
    Q_OBJECT
public:
    Client();
    Client(User *user); // 重载构造函数
    ~Client();
    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected();

    void getUser(const QString& username);
    // 添加获取用户信息的方法
    void setUsername(const QString& username) {
        this->clientNetworkManager->setUsernameRequest(username, this->user->userId);
    }
    void setUsername(const QString &username, const QString& password){
        this->clientNetworkManager->setUsernameRequest(username, password, this->user->userId);
    }
    User& getUser() {
        return *(this->user);
    }

    // 注册登录的入口
    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password,
                  const QString& contact, const UserType& usertype=FactoryUser, const QString& expertSkill="");
    // 获取设备信息
    void getDeviceData();
    // 获取议程信息
    void getAgenda();
    // 获取联系人信息
    void getContact();
    // 获取设备信息
    void getDevice();


    void addAgenda(const QString& ordername, const QString& device,
                   const QString &order_create_time, const QString &predict_time,
                   const QString &fault, const QString& status, const QString& expertname);

    void addDevice(QString name, QString type);


    // 发送信息
    void sendMessage(const QByteArray &message);
signals:
    void connected();           // 连接成功信号
    void disconnected();        // 断开连接信号
    void connectionFailed();    // 连接失败信号
    void messageReceived(const QByteArray &message);
    // 发送给登录注册页面的信息
    void loginResult(bool success, const QString& message);
    void registerResult(bool success, const QString& message);
    void setUserResult(bool success, const QString& message, User user);
    void agendaResult(bool success, const QVector<Agenda>& agendas, const QString& message);

    void contactResult(bool success, const QVector<User>& contacts, const QString& message);
    void deviceResult(bool success, const QVector<Device>& devices, const QString& message);
    void addDeviceResult(bool success, const QString& message);
    void addAgendaResult(bool success, const QString& message);

private slots:
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString &errorMsg);

    void onLoginResponseReceived(bool success, const QString& message);
    void onRegisterResponseReceived(bool success, const QString& message);
    void onSetUserResponseReceived(bool success, const QString& message, const User& user);
    void onGetUserResponseReceived(bool success, const QString& message, const User& userData);
    void onGetAgendaResponseReceived(bool success, const QVector<Agenda>& agendas, const QString& message);
    void onGetContactResponseReceived(bool success, const QVector<User>& contacts, const QString& message);
    void onGetDeviceResponseReceived(bool success, const QVector<Device>& devices, const QString& message);
    void onAddDeviceResponseReceived(bool success, const QString& messages);
    void onAddAgendaResponseReceived(bool success, const QString& messages);

private:
    ClientNetworkManager *clientNetworkManager;
    User *user;  // 对应的用户信息
};

#endif // CLIENT_H
