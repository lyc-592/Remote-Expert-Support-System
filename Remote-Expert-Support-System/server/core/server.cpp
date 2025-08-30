#include "server.h"
#include "../../utils/funcs.h"
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
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);

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

    // 读取数据并添加到缓冲区
    QByteArray data = clientSocket->readAll();
    QByteArray &buffer = buffers[clientSocket];
    buffer.append(data);

    qInfo() << "Received data from client:" << data;

    // 尝试处理缓冲区中的所有完整JSON消息
    while (!buffer.isEmpty()) {
        // 查找JSON对象的开始和结束
        int jsonStart = buffer.indexOf('{');
        if (jsonStart == -1) {
            // 没有找到JSON起始标记，清空缓冲区
            buffer.clear();
            break;
        }

        // 移除起始标记之前的所有数据
        if (jsonStart > 0) {
            buffer = buffer.mid(jsonStart);
        }

        // 尝试解析JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(buffer, &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            // 成功解析JSON对象
            QJsonObject request = doc.object();
            QString type = request["type"].toString();

            if (type == "login") {
                processLoginRequest(clientSocket, request);
                qInfo() << "Login Request Processing...";
            } else if (type == "register") {
                processRegisterRequest(clientSocket, request);
                qInfo() << "Register Request Processing...";
            } else if (type == "getExperts") {
                processGetExperts(clientSocket);
                qInfo() << "GetExperts Request Processing...";
            } else if (type == "addAgenda") {
                processAddAgenda(clientSocket, request);
                qInfo() << "AddAgenda Request Processing...";
            } else {
                // 未知请求类型
                QJsonObject response;
                response["success"] = false;
                response["message"] = "Unknown request type";
                response["request_id"] = request["request_id"];
                sendResponse(clientSocket, response);
            }

            // 从缓冲区中移除已处理的数据
            int jsonLength = doc.toJson().length();
            buffer = buffer.mid(jsonLength);
        } else if (error.error != QJsonParseError::NoError &&
                   error.error != QJsonParseError::UnterminatedObject) {
            // JSON解析错误（不是未完成的对象错误）
            qWarning() << "JSON parse error:" << error.errorString();

            QJsonObject response;
            response["success"] = false;
            response["message"] = "Invalid JSON format";
            sendResponse(clientSocket, response);

            // 清空缓冲区
            buffer.clear();
            break;
        } else {
            // 数据不完整，等待更多数据
            break;
        }
    }
}

// int Server::findJsonEnd(const QByteArray &data)
// {
//     int braceCount = 0;
//     bool inString = false;
//     bool escapeNext = false;

//     for (int i = 0; i < data.length(); i++) {
//         char c = data[i];

//         if (escapeNext) {
//             escapeNext = false;
//             continue;
//         }

//         if (c == '\\') {
//             escapeNext = true;
//             continue;
//         }

//         if (c == '"') {
//             inString = !inString;
//             continue;
//         }

//         if (!inString) {
//             if (c == '{') braceCount++;
//             else if (c == '}') braceCount--;

//             if (braceCount == 0 && i > 0) {
//                 return i;
//             }
//         }
//     }

//     return -1;
// }

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

    qDebug() << response;

    sendResponse(clientSocket, response);
}

void Server::processRegisterRequest(QTcpSocket *clientSocket, const QJsonObject &request)
{
    UserType usertype = static_cast<UserType>(request["usertype"].toInt());
    QString username = request["username"].toString();
    QString password = request["password"].toString();
    QString contact = request["contact"].toString();
    QString requestId = request["request_id"].toString();
    QString expertSkill = request["skill"].toString();

    bool success = dbManager->addUser(usertype, username, encryptPassword(password), contact, expertSkill);

    QJsonObject response;
    response["type"] = "register_response";
    response["request_id"] = requestId;
    response["success"] = success;
    response["message"] = success ? "Register successful" : "Register Fail";

    qDebug() << response;

    sendResponse(clientSocket, response);
}

void Server::sendResponse(QTcpSocket *clientSocket, const QJsonObject &response)
{
    QJsonDocument doc(response);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}

void Server::processGetExperts(QTcpSocket *clientSocket)
{
    QJsonArray arr;
    auto experts = dbManager->getExperts();
    for (auto &e : experts) {
        QJsonObject obj;
        obj["id"] = e.first;
        obj["name"] = e.second;
        arr.append(obj);
    }

    QJsonObject response;
    response["type"] = "getExpertsResponse";
    response["success"] = true;
    response["experts"] = arr;

    sendResponse(clientSocket, response);
}

void Server::processAddAgenda(QTcpSocket *clientSocket, const QJsonObject &request)
{
    int user1_id = request["user1_id"].toInt();   // 发起人（工厂用户）
    int device_id = request["device_id"].toInt();
    QString fault = request["fault"].toString();
    QString start = request["startDate"].toString();
    QString end   = request["endDate"].toString();
    int order_status = 0; // 默认：待处理

    bool allOk = true;
    QJsonArray expertArray = request["experts"].toArray();
    for (auto e : expertArray) {
        int expertId = e.toString().toInt();
        bool ok = dbManager->addWorkOrder(user1_id, expertId, device_id,
                                          order_status, start, end, fault);
        if (!ok) allOk = false;
    }

    QJsonObject response;
    response["type"] = "addAgendaResponse";
    response["success"] = allOk;
    response["title"] = request["title"].toString();
    response["device"] = request["device"].toString();
    response["startDate"] = start;
    response["endDate"] = end;
    response["fault"] = fault;
    response["status"] = QString("待处理");
    response["experts"] = expertArray;
    response["message"] = allOk ? "议程创建成功" : "议程创建失败";

    sendResponse(clientSocket, response);
}

