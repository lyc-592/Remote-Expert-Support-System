#include "server.h"
#include "../../utils/constants.h"
#include "../../utils/funcs.h"
#include "../entities/agenda.h"
#include "../entities/device.h"
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
    qDebug() << "Server onReadyRead";
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) {
        return;
    }

    // 读取数据并添加到缓冲区
    QByteArray data = clientSocket->readAll();
    QByteArray &buffer = buffers[clientSocket];
    buffer.append(data);

    qInfo() << "Received data from client:" << buffer;

    // 处理缓冲区中的所有完整JSON消息
    while (!buffer.isEmpty()) {
        // 查找第一个JSON对象的起始位置
        int jsonStart = buffer.indexOf('{');
        if (jsonStart == -1) {
            // 没有找到JSON起始标记，清空缓冲区
            buffer.clear();
            break;
        }

        // 移除起始标记之前的所有非JSON数据
        if (jsonStart > 0) {
            buffer = buffer.mid(jsonStart);
        }

        // 尝试找到匹配的结束大括号
        int braceCount = 0;
        int jsonEnd = -1;

        for (int i = 0; i < buffer.length(); i++) {
            if (buffer[i] == '{') {
                braceCount++;
            } else if (buffer[i] == '}') {
                braceCount--;
                if (braceCount == 0) {
                    jsonEnd = i;
                    break;
                }
            }
        }

        // 如果没有找到完整的JSON对象，等待更多数据
        if (jsonEnd == -1) {
            break;
        }

        // 提取完整的JSON对象
        QByteArray jsonData = buffer.left(jsonEnd + 1);
        buffer = buffer.mid(jsonEnd + 1).trimmed(); // 移除已处理的数据并去除空白

        // 解析JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            // 成功解析JSON对象
            QJsonObject request = doc.object();
            RequestType type = requestTypeFromString(request["type"].toString());
            qDebug() << "Request Type: " << request["type"].toString();

            switch (type) {
            case RequestType::Login:
                processLoginRequest(clientSocket, request);
                qInfo() << "Login Request Processing...";
                break;
            case RequestType::Register:
                processRegisterRequest(clientSocket, request);
                qInfo() << "Register Request Processing...";
                break;
            case RequestType::GetUser:
                processGetUserRequest(clientSocket, request);
                qInfo() << "GetUser Request Processing...";
                break;
            case RequestType::GetAgenda:
                processGetAgendaRequest(clientSocket, request);
                qInfo() << "GetAgenda Request Processing...";
                break;
            case RequestType::GetContact:
                processGetContactRequest(clientSocket, request);
                qInfo() << "GetContact Request Processing...";
                break;
            case RequestType::AddAgenda:
                processAddAgenda(clientSocket, request);
                qInfo() << "AddAgenda Request Processing...";
                break;
            case RequestType::GetDevice:
                processGetDeviceRequest(clientSocket, request);
                qInfo() << "GetDevice Request Processing...";
                break;
            case RequestType::AddDevice:
                processAddDevice(clientSocket, request);
                qInfo() << "AddDevice Request Processing...";
                break;
            case RequestType::SetUserName:
                processSetUsernameRequest(clientSocket, request);
                qInfo() << "set user Request Processing...";
                break;
            case RequestType::SetUserNameWithPassword:
                processSetUsernameWithPasswordRequest(clientSocket, request);
                qInfo() << "set user Request Processing...";
                break;
            default:
                // 未知请求类型
                QJsonObject response;
                response["success"] = false;
                response["message"] = "Unknown request type";
                response["request_id"] = request["request_id"];
                sendResponse(clientSocket, response);
                break;
            }
        } else {
            // JSON解析错误
            qWarning() << "JSON parse error:" << error.errorString();

            QJsonObject response;
            response["success"] = false;
            response["message"] = "Invalid JSON format";
            sendResponse(clientSocket, response);
        }
    }
}

void Server::processSetUsernameRequest(QTcpSocket *clientSocket, const QJsonObject &request) {
    QString username = request["username"].toString();
    int userId = request["user_id"].toInt();
    QString requestId = request["request_id"].toString();

    // 调用数据库管理器的方法
    User userData = dbManager->setUsername(username, userId);

    // 准备响应
    QJsonObject response;
    response["type"] = "setUsernameWithPasswordResponse";
    response["success"] = !userData.account.isEmpty();
    response["request_id"] = requestId;

    if (response["success"].toBool()) {
        response["userData"] = userData.toJson();
        response["message"] = "Username updated successfully with password verification";
    } else {
        response["message"] = "Failed to update username - password verification failed";
    }
    qDebug() << response;


    // 发送响应
    sendResponse(clientSocket, response);
}

void Server::processSetUsernameWithPasswordRequest(QTcpSocket *clientSocket, const QJsonObject &request) {
    QString username = request["username"].toString();
    QString password = request["password"].toString();
    int userId = request["user_id"].toInt();
    QString requestId = request["request_id"].toString();

    // 调用数据库管理器的方法
    User userData = dbManager->setUsername(username, password, userId);

    // 准备响应
    QJsonObject response;
    response["type"] = "setUsernameWithPasswordResponse";
    response["success"] = !userData.account.isEmpty();
    response["request_id"] = requestId;

    if (response["success"].toBool()) {
        response["userData"] = userData.toJson();
        response["message"] = "Username updated successfully with password verification";
    } else {
        response["message"] = "Failed to update username - password verification failed";
    }
    qDebug() << response;

    // 发送响应
    sendResponse(clientSocket, response);
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

void Server::processGetUserRequest(QTcpSocket *clientSocket, const QJsonObject &request) {
    QString account = request["username"].toString();
    QString requestId = request["request_id"].toString();
    // 调用DatabaseManager获取用户数据
    User userData = dbManager->getUser(account);
    // 准备响应
    QJsonObject response;
    response["type"] = "getUserResponse";
    response["success"] = !userData.account.isEmpty(); // 检查是否找到用户
    response["request_id"] = requestId;

    if (response["success"].toBool()) {
        response["userData"] = userData.toJson();
    } else {
        response["error"] = "User not found";
    }
    qDebug() << response;

    sendResponse(clientSocket, response);
}

void Server::processGetAgendaRequest(QTcpSocket *clientSocket, const QJsonObject &request) {
    int userId = request["user_id"].toInt();
    QString requestId = request["request_id"].toString();
    UserType userType =  static_cast<UserType>(request["user_type"].toInt());
    QVector<Agenda> agendas = dbManager->getAgenda(userId, userType);

    // 准备响应
    QJsonObject response;
    response["type"] = "getAgendaResponse";
    response["success"] = true;
    response["request_id"] = requestId;

    // 将议程列表转换为JSON数组
    QJsonArray agendaArray;
    for (const Agenda &agenda : agendas) {
        agendaArray.append(agenda.toJson());
    }
    response["agenda"] = agendaArray;

    // 发送响应
    sendResponse(clientSocket, response);
}

void Server::processGetContactRequest(QTcpSocket *clientSocket, const QJsonObject &request) {
    QString requestId = request["request_id"].toString();
    int userId = request["user_id"].toInt();
    QVector<User> contacts = dbManager->getContact(userId);

    // 准备响应
    QJsonObject response;
    response["type"] = "getContactResponse";
    response["success"] = true;
    response["request_id"] = requestId;

    // 将议程列表转换为JSON数组
    QJsonArray contactArray;
    for (const User &contact : contacts) {
        contactArray.append(contact.toJson());
    }
    response["contact"] = contactArray;

    // 发送响应
    sendResponse(clientSocket, response);
}

void Server::processGetDeviceRequest(QTcpSocket *clientSocket, const QJsonObject &request) {
    QString requestId = request["request_id"].toString();
    int userId = request["user_id"].toInt();
    QVector<Device> devices = dbManager->getDevice(userId);

    QJsonObject response;
    response["type"] = "getDeviceResponse";
    response["success"] = true;
    response["request_id"] = requestId;

    // 将议程列表转换为JSON数组
    QJsonArray deviceArray;
    for (const Device& device : devices) {
        deviceArray.append(device.toJson());
    }
    response["device"] = deviceArray;

    // 发送响应
    sendResponse(clientSocket, response);
}

void Server::processAddAgenda(QTcpSocket *clientSocket, const QJsonObject &request) {
    // 从请求中提取参数
    int user1_id = request["user1_id"].toInt();
    QString expertname = request["expertname"].toString();
    QString device = request["device"].toString();
    QString ordername = request["ordername"].toString();
    QString order_create_time = request["order_create_time"].toString();
    QString predict_time = request["predict_time"].toString();
    QString status = request["status"].toString();
    QString fault = request["fault"].toString();
    QString requestId = request["request_id"].toString();
    // 调用数据库管理器的方法
    bool success = dbManager->addWorkOrder(user1_id, expertname, device, ordername,
                                           order_create_time, predict_time, status, fault);
    // 准备响应
    QJsonObject response;
    response["type"] = "addAgendaResponse";
    response["success"] = success;
    response["request_id"] = requestId;

    if (success) {
        response["message"] = "Agenda added successfully";
    } else {
        response["message"] = "Failed to add agenda";
        // 如果需要，可以从dbManager获取更详细的错误信息
        // response["message"] = dbManager->getLastError();
    }
    // 发送响应
    sendResponse(clientSocket, response);
}

void Server::processAddDevice(QTcpSocket *clientSocket, const QJsonObject &request) {
    QString requestId = request["request_id"].toString();
    QString deviceName = request["device_name"].toString();
    QString deviceType = request["device_type"].toString();
    int userId = request["user_id"].toInt();

    bool success = dbManager->addDevice(deviceName, deviceType, userId);

    QJsonObject response;
    response["type"] = "addAgendaResponse";
    response["success"] = success;
    response["message"] = success ? "设备创建成功" : "设备创建失败";
    response["request_id"] = requestId;

    sendResponse(clientSocket, response);
}

void Server::sendResponse(QTcpSocket *clientSocket, const QJsonObject &response) {
    qDebug() << "Send Response :" << response["type"].toString();
    QJsonDocument doc(response);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    jsonData.append("\nEND_MSG\n"); // 定义一个明确的结束符

    clientSocket->write(jsonData);
    clientSocket->flush();
}


