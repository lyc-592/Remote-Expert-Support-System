#include "client.h"
#include "../../utils/network_interface_constants.h"
#include "../../server/entities/agenda.h"
#include <QString>
#include <QVector>

Client::Client()
    : QObject(nullptr) {
    clientNetworkManager = new ClientNetworkManager(this);
    user = new User();
    clientNetworkManager->connectToServer(host, hostPort);
    qDebug() << QString("Client: %1, %2")
                    .arg(user->userId)
                    .arg(user->account);
    // 连接信号和槽
    connect(clientNetworkManager, &ClientNetworkManager::connected, this, &Client::onConnected);
    connect(clientNetworkManager, &ClientNetworkManager::disconnected, this, &Client::onDisconnected);
    connect(clientNetworkManager, &ClientNetworkManager::errorOccurred, this, &Client::onConnectionError);
    connect(clientNetworkManager, &ClientNetworkManager::loginResponseReceived,
            this, &Client::onLoginResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getUserResponseReceived,
            this, &Client::onGetUserResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getAgendaResponseReceived,
            this, &Client::onGetAgendaResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getContactResponseReceived,
            this, &Client::onGetContactResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getDeviceResponseReceived,
            this, &Client::onGetDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addDeviceResponseReceived,
            this, &Client::onAddDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addAgendaResponseReceived,
            this, &Client::onAddAgendaResponseReceived);
}

Client::Client(User *user)
    : QObject(nullptr) {
    clientNetworkManager = new ClientNetworkManager(this);
    this->user = user;
    clientNetworkManager->connectToServer(host, hostPort);
    qDebug() << QString("Client: %1, %2")
                    .arg(user->userId)
                    .arg(user->account);
    // 连接信号和槽
    connect(clientNetworkManager, &ClientNetworkManager::connected, this, &Client::onConnected);
    connect(clientNetworkManager, &ClientNetworkManager::disconnected, this, &Client::onDisconnected);
    connect(clientNetworkManager, &ClientNetworkManager::errorOccurred, this, &Client::onConnectionError);
    connect(clientNetworkManager, &ClientNetworkManager::loginResponseReceived,
            this, &Client::onLoginResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getUserResponseReceived,
            this, &Client::onGetUserResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getAgendaResponseReceived,
            this, &Client::onGetAgendaResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getContactResponseReceived,
            this, &Client::onGetContactResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getDeviceResponseReceived,
            this, &Client::onGetDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addDeviceResponseReceived,
            this, &Client::onAddDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addAgendaResponseReceived,
            this, &Client::onAddAgendaResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::setUserResponseReceived,
            this, &Client::onSetUserResponseReceived);
}

Client::~Client() {
    // 清理资源
    disconnectFromServer();
    delete user;
    user = nullptr;
}

void Client::connectToServer(const QString &host, quint16 port) {
    this->clientNetworkManager->connectToServer(host, port);
}

void Client::onConnected() {
    qDebug() << "Client" << user->account << "connected to server";
    emit connected(); // 发出连接成功信号
}

void Client::sendMessage(const QByteArray &message) {
    this->clientNetworkManager->sendMessage(message);
}

void Client::disconnectFromServer() {
    this->clientNetworkManager->disconnectFromServer();
}

void Client::onDisconnected() {
    qDebug() << "Client" << user->account << "disconnected from server";
    emit disconnected(); // 发出断开连接信号
}

bool Client::isConnected() {
    return clientNetworkManager->isConnected();
}

void Client::onConnectionError(const QString& errorMsg) {
    qDebug() << "Connection error for client" << user->account << ":" << errorMsg;
    emit connectionFailed(); // 发出连接失败信号
}

/* 下面是功能的封装函数 */
void Client::getUser(const QString &username) {
    clientNetworkManager->getUserRequest(username);
}

void Client::login(const QString& username, const QString& password) {
    clientNetworkManager->loginRequest(username, password);
}

void Client::registerUser(const QString &username, const QString &password,
                          const QString &contact, const UserType &usertype, const QString &expertSkill) {
    clientNetworkManager->registerRequest(username, password, contact, usertype, expertSkill);
}

void Client::getAgenda() {
    clientNetworkManager->getAgendaRequest(this->user->userId, this->user->userType);
}

void Client::getContact() {
    clientNetworkManager->getContactRequest(this->user->userId);
}

void Client::getDevice() {
    clientNetworkManager->getDeviceRequest(this->user->userId);
}

void Client::addAgenda(const QString& ordername, const QString& device,
                       const QString &order_create_time, const QString &predict_time,
                       const QString &fault, const QString& status, const QString& expertname) {
    clientNetworkManager->addAgendaRequest(this->user->userId, expertname, device, ordername, order_create_time, predict_time, status, fault);
}

void Client::addDevice(QString name, QString type) {
    clientNetworkManager->addDeviceRequest(name, type, this->user->userId);
}


/* 槽函数，通常负责向UI界面发送信号 */
void Client::onLoginResponseReceived(bool success, const QString& message)
{
    qDebug() << "Client on Login Response Received!";
    emit loginResult(success, message);
    qDebug() << "Success:" << success << "Message:" << message;
}

void Client::onRegisterResponseReceived(bool success, const QString& message)
{
    qDebug() << "Client on Register Response Received!";
    emit registerResult(success, message);
    qDebug() << "Success:" << success << "Message:" << message;
}

void Client::onSetUserResponseReceived(bool success, const QString& message, const User& userData) {
    if (success) {
        if (user) {
            delete user;
            user = nullptr;
        }
        // 创建新的用户数据对象
        user = new User(userData);
        qDebug() << "User data updated for account:" << user->userId;
    } else {
        qWarning() << "Failed to get user data:" << message;
        // 清空用户数据或保持原状
    }
    emit setUserResult(success, message, userData);
}

void Client::onGetUserResponseReceived(bool success, const QString& message, const User& userData)
{
    if (success) {
        if (user) {
            delete user;
            user = nullptr;
        }

        // 创建新的用户数据对象
        user = new User(userData);
        qDebug() << "User data updated for account:" << user->userId;
    } else {
        qWarning() << "Failed to get user data:" << message;
        // 清空用户数据或保持原状
    }
}

void Client::onGetAgendaResponseReceived(bool success, const QVector<Agenda>& agendas, const QString& message)
{
    emit agendaResult(success, agendas, message);
}

void Client::onGetContactResponseReceived(bool success, const QVector<User>& contacts, const QString& message) {
    qDebug() << "Client: getContactReceived";
    emit contactResult(success, contacts, message);
}

void Client::onGetDeviceResponseReceived(bool success, const QVector<Device>& devices, const QString& message) {
    emit deviceResult(success, devices, message);
}

void Client::onAddDeviceResponseReceived(bool success, const QString& message) {
    emit addDeviceResult(success, message);
}

void Client::onAddAgendaResponseReceived(bool success, const QString &message) {
    emit addAgendaResult(success, message);
}
