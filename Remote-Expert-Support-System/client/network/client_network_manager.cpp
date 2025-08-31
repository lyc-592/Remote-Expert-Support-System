#include "client_network_manager.h"
#include "../../utils/funcs.h"
#include "../../utils/constants.h"
#include "../../server/entities/user.h"
#include <QUuid>
#include <QDebug>
#include <QDataStream>

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

// ***** MODIFIED onReadyRead() for Message Framing *****
void ClientNetworkManager::onReadyRead() {
    // 读取数据并添加到缓冲区
    QByteArray data = tcpSocket->readAll();
    responseBuffer.append(data);

    // 尝试处理缓冲区中的所有完整JSON消息
    while (!responseBuffer.isEmpty()) {
        // 查找第一个JSON对象的起始位置
        int jsonStart = responseBuffer.indexOf('{');
        if (jsonStart == -1) {
            // 没有找到JSON起始标记，清空缓冲区
            responseBuffer.clear();
            break;
        }

        // 移除起始标记之前的所有非JSON数据
        if (jsonStart > 0) {
            responseBuffer = responseBuffer.mid(jsonStart);
        }

        // 尝试找到匹配的结束大括号
        int braceCount = 0;
        int jsonEnd = -1;

        for (int i = 0; i < responseBuffer.length(); i++) {
            if (responseBuffer[i] == '{') {
                braceCount++;
            } else if (responseBuffer[i] == '}') {
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
        QByteArray jsonData = responseBuffer.left(jsonEnd + 1);
        responseBuffer = responseBuffer.mid(jsonEnd + 1).trimmed(); // 移除已处理的数据并去除空白

        // 解析JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject response = doc.object();

            // 获取请求ID并查找对应的回调函数
            QString requestId = response["request_id"].toString();
            if (pendingRequests.contains(requestId)) {
                // 调用注册的回调函数
                auto callback = pendingRequests[requestId];
                callback(response);

                // 移除已处理的请求
                pendingRequests.remove(requestId);
            } else {
                qWarning() << "Received response for unknown request ID:" << requestId;

                // 如果没有找到对应的回调，尝试根据类型处理通用响应
                QString type = response["type"].toString();
                if (type == "error") {
                    QString errorMsg = response["message"].toString();
                    emit errorOccurred(errorMsg);
                }
            }
        } else {
            qWarning() << "Invalid JSON response from server:" << error.errorString();
            emit errorOccurred("Invalid JSON response from server");
        }
    }
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
    request["password"] = encryptPassword(password);

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

    qInfo() << "Login Request Sending...";
}

void ClientNetworkManager::registerRequest(const QString& username, const QString& password,
                                           const QString& contact, const UserType& usertype,
                                            const QString &expertSkill)
{
    if (!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }

    // 创建注册请求JSON
    QJsonObject request;
    request["type"] = "register";
    request["request_id"] = QString::number(++requestIdCounter);
    request["usertype"] = usertype;
    request["username"] = username;
    request["password"] = encryptPassword(password);
    request["contact"] = contact;
    request["skill"] = expertSkill;

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

    qInfo() << "Register Request Sending...";
}


void ClientNetworkManager::getUserRequest(const QString& username)
{
    if (!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }

    // 创建获取用户信息请求JSON
    QJsonObject request;
    request["type"] = "getUser";
    request["request_id"] = QString::number(++requestIdCounter);
    request["username"] = username;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();

        if (success) {
            // 解析用户数据
            // 创建UserData对象并填充数据
            User userData = User::fromJson(response["user_data"].toObject());

            // 发出获取用户信息成功的信号
            emit getUserResponseReceived(true, "User data retrieved successfully", userData);
        } else {
            QString message = response["message"].toString();
            // 发出获取用户信息失败的信号
            emit getUserResponseReceived(false, message, User());
        }
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();

    qInfo() << "Get User Request Sending for username:" << username;
}

void ClientNetworkManager::getAgendaRequest(const int userId, const UserType userType) {
    QJsonObject request;
    request["type"] = "getAgenda";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;
    request["user_type"] = userType;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QVector<Agenda> agendas;
        QString message;

        if (success) {
            // 解析议程列表
            QJsonArray agendaArray = response["agenda"].toArray();
            for (const QJsonValue &value : agendaArray) {
                Agenda agenda = Agenda::fromJson(value.toObject());
                agendas.append(agenda);
            }
            message = "Agenda retrieved successfully";
        } else {
            message = response["message"].toString();
        }
        emit getAgendaResponseReceived(success, agendas, message);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    // qDebug() << "Is socket connected:" << (tcpSocket->state() == QAbstractSocket::ConnectedState);
    qInfo() << "Agenda Request Sending...";
}

void ClientNetworkManager::getContactRequest(const int userId) {
    QJsonObject request;
    request["type"] = "getContact";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QVector<User> contacts;
        QString message;

        if (success) {
            // 解析议程列表
            QJsonArray contactArray = response["contact"].toArray();
            for (const QJsonValue &value : contactArray) {
                User contact = User::fromJson(value.toObject());
                contacts.append(contact);
            }
            message = "Contact retrieved successfully";
        } else {
            message = response["message"].toString();
        }
        emit getContactResponseReceived(success, contacts, message);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    // qDebug() << "Is socket connected:" << (tcpSocket->state() == QAbstractSocket::ConnectedState);
    qInfo() << "Contact Request Sending...";
}

void ClientNetworkManager::addAgendaRequest(int user1_id, QVector<int> user2_ids, int device_id,
                                            const QString &order_create_time,
                                            const QString &predict_time, const QString &fault) {
    QJsonObject request;
    request["type"] = "addAgenda";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user1_id"] = user1_id;
    QJsonArray expertsArray;
    for (int id : user2_ids) {
        expertsArray.append(id);
    }
    request["experts"] = expertsArray;
    request["device_id"] = device_id;
    request["startDate"] = order_create_time;
    request["endDate"] = predict_time;
    request["fault"] = fault;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit addAgendaResponseReceived(success, message);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    // qDebug() << "Is socket connected:" << (tcpSocket->state() == QAbstractSocket::ConnectedState);
    qInfo() << "Contact Request Sending...";
}

void ClientNetworkManager::sendMessage(const QByteArray &message)
{
    if (this->tcpSocket->state() == QAbstractSocket::ConnectedState) {
        this->tcpSocket->write(message);
    }
}
