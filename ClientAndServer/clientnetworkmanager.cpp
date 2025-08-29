#include "clientnetworkmanager.h"
#include <QUuid>
#include <QDebug>

ClientNetworkManager::ClientNetworkManager(QObject *parent) : QObject(parent), requestIdCounter(0)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, this, &ClientNetworkManager::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ClientNetworkManager::onDisconnected);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &ClientNetworkManager::onErrorOccurred);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientNetworkManager::onReadyRead);
}

ClientNetworkManager::~ClientNetworkManager()
{
    disconnectFromServer();
    delete tcpSocket;
}

void ClientNetworkManager::connectToServer(const QString &host, quint16 port)
{
    tcpSocket->connectToHost(host, port);
}

void ClientNetworkManager::disconnectFromServer()
{
    if (tcpSocket->state() != QAbstractSocket::UnconnectedState) {
        tcpSocket->disconnectFromHost();
    }
}

bool ClientNetworkManager::isConnected() const
{
    return tcpSocket->state() == QAbstractSocket::ConnectedState;
}

void ClientNetworkManager::onConnected()
{
    qInfo() << "Connected to server";
    emit connected();
}

void ClientNetworkManager::onDisconnected()
{
    qInfo() << "Disconnected from server";
    emit disconnected();
}

void ClientNetworkManager::onErrorOccurred(QAbstractSocket::SocketError error)
{
    QString errorMsg = tcpSocket->errorString();
    qWarning() << "Socket error:" << errorMsg;
    emit errorMsg;
}

/* 下面存放放出请求的功能代码 */

/* 请求登陆 */
void ClientNetworkManager::loginRequest(const QString &username, const QString &password)
{
    if (!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }

    // 创建登录请求JSON
    QJsonObject request;
    request["type"] = "login";
    request["request_id"] = QString::number(++requestIdCounter);
    request["username"] = username;
    request["password"] = password;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit loginResponseReceived(success, message);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
}


void ClientNetworkManager::onReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        emit errorOccurred("Invalid JSON response from server");
        return;
    }

    QJsonObject response = doc.object();
    QString requestId = response["request_id"].toString();

    if (pendingRequests.contains(requestId)) {
        pendingRequests[requestId](response);
        pendingRequests.remove(requestId);
    }
}

void ClientNetworkManager::sendMessage(const QByteArray &message)
{
    if (this->tcpSocket->state() == QAbstractSocket::ConnectedState) {
        this->tcpSocket->write(message);
    }
}
