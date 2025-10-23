#include "client_network_manager.h"
#include "../../utils/funcs.h"
#include "../../utils/constants.h"
#include "../../server/entities/user.h"
#include "../../server/entities/device.h"
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
            } else if(requestId == "newChatMessage") {
                int contactId = response["contact_id"].toInt();
                int senderId = response["sender_id"].toInt();
                emit newChatMessageReceived(contactId, senderId);
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

void ClientNetworkManager::setUsernameRequest(const QString& username, int userId) {
    QJsonObject request;
    request["type"] = "setUsername";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;
    request["username"] = username;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();

        if (success) {
            User userData = User::fromJson(response["user_data"].toObject());
            // 发出获取用户信息成功的信号
            emit setUserResponseReceived(true, "User data retrieved successfully", userData);
        } else {
            QString message = response["message"].toString();
            // 发出获取用户信息失败的信号
            emit setUserResponseReceived(false, message, User());
        }
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Set Username Request Sending...";
}

void ClientNetworkManager::setUsernameRequest(const QString& username, const QString& password, int userId) {
    QJsonObject request;
    request["type"] = "setUsernameWithPassword";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;
    request["username"] = username;
    request["password"] = encryptPassword(password);

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();

        if (success) {
            User userData = User::fromJson(response["user_data"].toObject());
            // 发出获取用户信息成功的信号
            emit setUserResponseReceived(true, "User data retrieved successfully", userData);
        } else {
            QString message = response["message"].toString();
            // 发出获取用户信息失败的信号
            emit setUserResponseReceived(false, message, User());
        }
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Set Username With Password Request Sending...";
}

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
        emit registerResponseReceived(success, message);
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
            qDebug() << "Receive Get User" << userData.toJson();
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

void ClientNetworkManager::getDeviceRequest(const int userId) {
    QJsonObject request;
    request["type"] = "getDevice";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QVector<Device> devices;
        QString message;
        if (success) {
            // 解析议程列表
            QJsonArray deviceArray = response["device"].toArray();
            for (const QJsonValue &value : deviceArray) {
                Device device = Device::fromJson(value.toObject());
                devices.append(device);
            }
            message = "Device retrieved successfully";
        } else {
            message = response["message"].toString();
        }
        emit getDeviceResponseReceived(success, devices ,message);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    // qDebug() << "Is socket connected:" << (tcpSocket->state() == QAbstractSocket::ConnectedState);
    qInfo() << "Contact Request Sending...";
}

void ClientNetworkManager::addAgendaRequest(int user1_id, const QString& expertname, const QString& device, const QString& ordername,
                                            const QString &order_create_time, const QString &predict_time, const QString& status, const QString &fault) {
    QJsonObject request;
    request["type"] = "addAgenda";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user1_id"] = user1_id;
    request["expertname"] = expertname;
    request["device"] = device;
    request["ordername"] = ordername;
    request["order_create_time"] = order_create_time;
    request["predict_time"] = predict_time;
    request["status"] = status;
    request["fault"] = fault;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message;

        if (success) {
            message = "Agenda added successfully";
        } else {
            message = response["message"].toString();
        }
        emit addAgendaResponseReceived(success, message);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Add Agenda Request Sending...";
}

void ClientNetworkManager::addDeviceRequest(const QString& name, const QString& type, const QString& status, int userId) {
    QJsonObject request;
    request["type"] = "addDevice";
    request["request_id"] = QString::number(++requestIdCounter);
    request["device_name"] = name;
    request["device_type"] = type;
    request["status"] = status;
    request["user_id"] = userId;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit addDeviceResponseReceived(success, message);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    // qDebug() << "Is socket connected:" << (tcpSocket->state() == QAbstractSocket::ConnectedState);
    qInfo() << "Contact Request Sending...";
}

void ClientNetworkManager::removeDeviceRequest(const QString& deviceName, int userId) {
    QJsonObject request;
    request["type"] = "removeDevice";
    request["request_id"] = QString::number(++requestIdCounter);
    request["device_name"] = deviceName;
    request["user_id"] = userId;

    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit removeDeviceResponseReceived(success, message);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Remove Device Request Sending...";
}

void ClientNetworkManager::sendMessage(const QByteArray &message)
{
    if (this->tcpSocket->state() == QAbstractSocket::ConnectedState) {
        this->tcpSocket->write(message);
    }
}

void ClientNetworkManager::sendChatMessageRequest(int senderId, int receiverId, const QString& message) {
    QJsonObject request;
    request["type"] = "sendChatMessage";
    request["request_id"] = QString::number(++requestIdCounter);
    request["sender_id"] = senderId;
    request["receiver_id"] = receiverId;
    request["message"] = message;

    // 注册回调函数
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        qDebug() << "Send Chat Received";
        emit sendChatMessageResponseReceived(success, message);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Chat Message Request Sending...";
}

void ClientNetworkManager::getChatHistoryRequest(int userId, int contactId) {
    QJsonObject request;
    request["type"] = "getChatHistory";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;
    request["contact_id"] = contactId;

    // 注册回调函数
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QVector<ChatMessage> history;

        if (success) {
            QJsonArray messagesArray = response["history"].toArray();
            for (const QJsonValue& value : messagesArray) {
                QJsonObject msgObj = value.toObject();
                ChatMessage msg(
                    msgObj["sender_id"].toInt(),
                    msgObj["receiver_id"].toInt(),
                    msgObj["message"].toString(),
                    msgObj["is_own"].toBool(),
                    msgObj["timestamp"].toString()
                    );
                history.append(msg);
            }
        }

        QString message = response["message"].toString();
        emit getChatHistoryResponseReceived(success, history, message);
        qDebug() << "CNM: get history response";
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Get Chat History Request Sending...";
}

void ClientNetworkManager::addSimulateDataRequest(int deviceId, double temp, double pressure, const QString& otherData) {
    // 封装请求
    QJsonObject request;
    request["type"] = "addSimulateData";
    request["request_id"] = QString::number(++requestIdCounter);
    request["device_id"] = deviceId;
    request["temp"] = temp;
    request["pressure"] = pressure;
    request["other_data"] = otherData;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit addSimulateDataResponseReceived(success, message);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();

    qInfo() << "AddSimulateData Request Sending...";
}

// client_network_manager.cpp
void ClientNetworkManager::getNonContactUsersRequest(int userId) {
    QJsonObject request;
    request["type"] = "getNonContactUsers";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user_id"] = userId;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        QList<User> users;
        QJsonArray usersArray = response["users"].toArray();
        for (const QJsonValue &value : usersArray) {
            users.append(User::fromJson(value.toObject()));
            // qDebug() << "CNM:" << User::fromJson(value.toObject()).toJson();
        }
        emit nonContactUsersResponseReceived(users);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "GetNonContactUsers Request Sending...";
}

void ClientNetworkManager::addContactRequest(int user1Id, int user2Id) {
    QJsonObject request;
    request["type"] = "addContact";
    request["request_id"] = QString::number(++requestIdCounter);
    request["user1_id"] = user1Id;
    request["user2_id"] = user2Id;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit addContactResponseReceived(success, message);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "AddContact Request Sending...";
}

void ClientNetworkManager::getAllDevicesRequest() {
    if (!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }

    QJsonObject request;
    request["type"] = "getAllDevices"; // 传输给服务器
    request["request_id"] = QString::number(++requestIdCounter);

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {

        bool success = response["success"].toBool();
        qDebug() << success;
        QVector<Device> devices;
        QString message;

        if (success) {
            QJsonArray devicesArray = response["devices"].toArray();
            qDebug() << "Devices array size:" << devicesArray.size();

            for (const QJsonValue &value : devicesArray) {
                Device device = Device::fromJson(value.toObject());
                devices.append(device);
            }
            message = "Devices retrieved successfully";
            qDebug() << "Devices retrieved successfully";
        } else {
            message = response["message"].toString();
        }

        emit getAllDevicesResponseReceived(success, message, devices);
    };

    // 发送请求
    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "GetAllDevices Request Sending...";
}

void ClientNetworkManager::updateDeviceRequest(const QString& deviceName, const QString& type,
                                               const QString& status, int userId) {
    // 封装请求
    QJsonObject request;
    request["type"] = "updateDevice";
    request["request_id"] = QString::number(++requestIdCounter);
    request["device_name"] = deviceName;
    request["device_type"] = type;
    request["status"] = status;
    request["user_id"] = userId;

    // 注册回调函数处理响应
    pendingRequests[request["request_id"].toString()] = [this](const QJsonObject &response) {
        bool success = response["success"].toBool();
        QString message = response["message"].toString();
        emit updateDeviceResponseReceived(success, message);
    };

    QJsonDocument doc(request);
    tcpSocket->write(doc.toJson());
    tcpSocket->flush();
    qInfo() << "Update Device Request Sending...";
}
