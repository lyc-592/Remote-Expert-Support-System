#include "server.h"
#include "databasemanager.h"
#include <QDebug>
#include <QTcpServer>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    dbManager = new DatabaseManager(this);
    if (!dbManager->connectToDatabase()) {
        qCritical() << "Failed to connect to database!";
    }
}

Server::~Server()
{
    stopServer();
    delete dbManager;
}

bool Server::isPortAvailable(quint16 port) {
    QTcpServer testServer;
    bool available = testServer.listen(QHostAddress::Any, port);
    if (available) {
        testServer.close(); // 关闭测试服务器，释放端口
    }
    return available;
}

bool Server::startServer(quint16 port) {
    if (!isPortAvailable(port)) {
        qCritical() << "Port" << port << "is already in use";
        return false;
    }

    // 现在才真正启动服务器监听
    if (!this->listen(QHostAddress::Any, port)) {
        qCritical() << "Failed to start server on port" << port;
        return false;
    }

    qInfo() << "Server started on port" << port;
    return true;
}

void Server::stopServer() {
    this->close();
    for (QTcpSocket *socket : clientSockets.keys()) {
        socket->close();
        socket->deleteLater();
    }
    clientSockets.clear();
    qInfo() << "Server stopped";
}
void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::clientReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

    clientSockets.insert(clientSocket, QString());
    qInfo() << "New client connected:" << clientSocket->peerAddress().toString();
}

void Server::onDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        qInfo() << "Client disconnected:" << clientSocket->peerAddress().toString();
        clientSockets.remove(clientSocket);
        clientSocket->deleteLater();
    }
}

void Server::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        return;
    }

    QByteArray data = clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        // 无效的JSON，发送错误响应
        QJsonObject response;
        response["success"] = false;
        response["message"] = "Invalid request format";
        sendResponse(clientSocket, response);
        return;
    }

    QJsonObject request = doc.object();
    QString type = request["type"].toString();

    if (type == "login") {
        processLoginRequest(clientSocket, request);
    } else {
        // 未知请求类型
        QJsonObject response;
        response["success"] = false;
        response["message"] = "Unknown request type";
        response["request_id"] = request["request_id"];
        sendResponse(clientSocket, response);
    }
}

void Server::processLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request)
{
    QString username = request["username"].toString();
    QString password = request["password"].toString();
    QString requestId = request["request_id"].toString();

    bool success = dbManager->validateUser(username, password);

    QJsonObject response;
    response["type"] = "login_response";
    response["request_id"] = requestId;
    response["success"] = success;
    response["message"] = success ? "Login successful" : "Invalid username or password";

    sendResponse(clientSocket, response);
}

void Server::sendResponse(QTcpSocket *clientSocket, const QJsonObject &response)
{
    QJsonDocument doc(response);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}
