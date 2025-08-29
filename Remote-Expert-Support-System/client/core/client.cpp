#include "client.h"
#include "../../utils/network_interface_constants.h"
#include <QString>

Client::Client(const QString& username, const QString& userId, const QString& password)
    : QObject(nullptr), username(username), userId(userId), password(password) {
    clientNetworkManager = new ClientNetworkManager(this);
    clientNetworkManager->connectToServer(host, hostPort);
    // 连接信号和槽
    connect(clientNetworkManager, &ClientNetworkManager::connected, this, &Client::onConnected);
    connect(clientNetworkManager, &ClientNetworkManager::disconnected, this, &Client::onDisconnected);
    connect(clientNetworkManager, &ClientNetworkManager::errorOccurred, this, &Client::onConnectionError);
    connect(clientNetworkManager, &ClientNetworkManager::loginResponseReceived,
            this, &Client::onLoginResponseReceived);
}

Client::~Client() {
    // 清理资源
    disconnectFromServer();
}

void Client::connectToServer(const QString &host, quint16 port) {
    this->clientNetworkManager->connectToServer(host, port);
}

void Client::onConnected() {
    qDebug() << "Client" << username << "connected to server";
    emit connected(); // 发出连接成功信号
}

void Client::sendMessage(const QByteArray &message) {
    this->clientNetworkManager->sendMessage(message);
}

void Client::disconnectFromServer() {
    this->clientNetworkManager->disconnectFromServer();
}

void Client::onDisconnected() {
    qDebug() << "Client" << username << "disconnected from server";
    emit disconnected(); // 发出断开连接信号
}

bool Client::isConnected() {
    return clientNetworkManager->isConnected();
}

void Client::onConnectionError(const QString& errorMsg) {
    qDebug() << "Connection error for client" << username << ":" << errorMsg;
    emit connectionFailed(); // 发出连接失败信号
}

void Client::login(const QString& username, const QString& password) {
    clientNetworkManager->loginRequest(username, password);
}

void Client::registerUser(const QString &username, const QString &password,
                          const QString &contact, const UserType &usertype, const QString &expertSkill) {
    clientNetworkManager->registerRequest(username, password, contact, usertype, expertSkill);
}

void Client::onLoginResponseReceived(bool success, const QString& message)
{
    qDebug() << "Client on Login Response Received!";
    emit loginResult(success, message);
    qDebug() << "Success:" << success << "Message:" << message;
}

void Client::onRegisterResponseReceived(bool success, const QString& message)
{
    qDebug() << "Client on Register Response Received!";
    emit loginResult(success, message);
    qDebug() << "Success:" << success << "Message:" << message;
}
