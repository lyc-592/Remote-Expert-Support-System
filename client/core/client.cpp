#include "client.h"
#include "../../utils/network_interface_constants.h"
#include "../../server/entities/agenda.h"
#include <QString>
#include <QVector>
#include <QRandomGenerator>

Client::Client()
    : QObject(nullptr) {
    clientNetworkManager = new ClientNetworkManager(this);
    user = new User();
    qDebug() << QString("Client: %1, %2")
                    .arg(user->userId)
                    .arg(user->account);

    simulateTimer = new QTimer(this);
    connect(simulateTimer, &QTimer::timeout, this, [this]() {
        if (deviceList.isEmpty()) {
            qWarning() << "⚠️ 没有设备可用于生成模拟数据";
            return;
        }

        int index = QRandomGenerator::global()->bounded(deviceList.size());
        int deviceId = deviceList[index].deviceId;

        double temp = 20.0 + (qrand() % 8000) / 100.0;
        double pressure = 10.0 + (qrand() % 29000) / 100.0;
        QStringList statusOptions = {"正常", "故障", "离线"};
        QString otherData = statusOptions[qrand() % statusOptions.size()];

        this->addSimulateData(deviceId, temp, pressure, otherData);
        qDebug() << "🌀 自动模拟数据请求 → deviceId:" << deviceId;
    });
    simulateTimer->start(5000);

    // 连接信号和槽
    connect(clientNetworkManager, &ClientNetworkManager::connected, this, &Client::onConnected);
    connect(clientNetworkManager, &ClientNetworkManager::disconnected, this, &Client::onDisconnected);
    connect(clientNetworkManager, &ClientNetworkManager::errorOccurred, this, &Client::onConnectionError);
    connect(clientNetworkManager, &ClientNetworkManager::loginResponseReceived,
            this, &Client::onLoginResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::registerResponseReceived,
            this, &Client::onRegisterResponseReceived);
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
    connect(clientNetworkManager, &ClientNetworkManager::removeDeviceResponseReceived,
            this, &Client::onRemoveDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addAgendaResponseReceived,
            this, &Client::onAddAgendaResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::setUserResponseReceived,
            this, &Client::onSetUserResponseReceived);

    connect(clientNetworkManager, &ClientNetworkManager::sendChatMessageResponseReceived,
            this, &Client::onSendChatMessageResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getChatHistoryResponseReceived,
            this, &Client::onGetChatHistoryResponseReceived);

    connect(clientNetworkManager, &ClientNetworkManager::addSimulateDataResponseReceived,
            this, &Client::onAddSimulateDataResponseReceived);

    connect(clientNetworkManager, &ClientNetworkManager::nonContactUsersResponseReceived,
            this, &Client::onNonContactUsersResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addContactResponseReceived,
            this, &Client::onAddContactResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getAllDevicesResponseReceived,
            this, &Client::onGetAllDevicesResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::newChatMessageReceived,
            this, &Client::onNewChatMessageReceived);
    clientNetworkManager->connectToServer(host, hostPort);
}

Client::Client(User *user)
    : QObject(nullptr) {
    clientNetworkManager = new ClientNetworkManager(this);
    this->user = user;
    clientNetworkManager->connectToServer(host, hostPort);
    qDebug() << QString("Client: %1, %2")
                    .arg(user->userId)
                    .arg(user->account);

    simulateTimer = new QTimer(this);
    connect(simulateTimer, &QTimer::timeout, this, [this]() {
        if (deviceList.isEmpty()) {
            qWarning() << "⚠️ 没有设备可用于生成模拟数据";
            return;
        }

        int index = QRandomGenerator::global()->bounded(deviceList.size());
        int deviceId = deviceList[index].deviceId;

        double temp = 20.0 + (qrand() % 8000) / 100.0;
        double pressure = 10.0 + (qrand() % 29000) / 100.0;
        QStringList statusOptions = {"正常", "故障", "离线"};
        QString otherData = statusOptions[qrand() % statusOptions.size()];

        this->addSimulateData(deviceId, temp, pressure, otherData);
        qDebug() << "🌀 自动模拟数据请求 → deviceId:" << deviceId;
    });
    simulateTimer->start(5000);
    // 连接信号和槽

    connect(clientNetworkManager, &ClientNetworkManager::connected, this, &Client::onConnected);
    connect(clientNetworkManager, &ClientNetworkManager::disconnected, this, &Client::onDisconnected);
    connect(clientNetworkManager, &ClientNetworkManager::errorOccurred, this, &Client::onConnectionError);
    connect(clientNetworkManager, &ClientNetworkManager::loginResponseReceived,
            this, &Client::onLoginResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::registerResponseReceived,
            this, &Client::onRegisterResponseReceived);
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
    connect(clientNetworkManager, &ClientNetworkManager::removeDeviceResponseReceived,
            this, &Client::onRemoveDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addAgendaResponseReceived,
            this, &Client::onAddAgendaResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::setUserResponseReceived,
            this, &Client::onSetUserResponseReceived);

    connect(clientNetworkManager, &ClientNetworkManager::sendChatMessageResponseReceived,
            this, &Client::onSendChatMessageResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getChatHistoryResponseReceived,
            this, &Client::onGetChatHistoryResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addSimulateDataResponseReceived,
            this, &Client::onAddSimulateDataResponseReceived);

    connect(clientNetworkManager, &ClientNetworkManager::nonContactUsersResponseReceived,
            this, &Client::onNonContactUsersResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::addContactResponseReceived,
            this, &Client::onAddContactResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::getAllDevicesResponseReceived,
            this, &Client::onGetAllDevicesResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::updateDeviceResponseReceived,
            this, &Client::onUpdateDeviceResponseReceived);
    connect(clientNetworkManager, &ClientNetworkManager::newChatMessageReceived,
            this, &Client::onNewChatMessageReceived);

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

void Client::addDevice(QString name, QString type, QString status) {
    clientNetworkManager->addDeviceRequest(name, type, status, this->user->userId);
}

void Client::sendChatMessage(int receiverId, const QString& message) {
    clientNetworkManager->sendChatMessageRequest(this->user->userId, receiverId, message);
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
        emit setUserResult(success, message, userData);
        qDebug() << "User data updated for account:" << user->userId;
    } else {
        qWarning() << "Failed to get user data:" << message;
        // 清空用户数据或保持原状
    }
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
        getDevice();
    } else {
        qWarning() << "Failed to get user data:" << message;
        // 清空用户数据或保持原状
    }
    emit getUserResult(success, message, userData);
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
    if (success) {
        deviceList = devices; // ⭐ 保存下来，给定时器用
        qDebug() << "设备列表已更新，共" << devices.size() << "个设备";
    } else {
        qWarning() << "获取设备失败:" << message;
    }
    emit deviceResult(success, devices, message);
}

void Client::onAddDeviceResponseReceived(bool success, const QString& message) {
    emit addDeviceResult(success, message);
}

void Client::removeDevice(const QString& deviceName) {
    clientNetworkManager->removeDeviceRequest(deviceName, this->user->userId);
}

void Client::onRemoveDeviceResponseReceived(bool success, const QString& message) {
    emit removeDeviceResult(success, message);
}

void Client::onAddAgendaResponseReceived(bool success, const QString &message) {
    emit addAgendaResult(success, message);
}

void Client::onSendChatMessageResponseReceived(bool success, const QString& message) {
    emit sendChatMessageResult(success, message);
}

void Client::getChatHistory(int contactId) {
    clientNetworkManager->getChatHistoryRequest(this->user->userId, contactId);
}

void Client::onGetChatHistoryResponseReceived(bool success, const QVector<ChatMessage>& history, const QString& message) {
    qDebug() << "Client: Get Chat History";
    emit getChatHistoryResult(success, history, message);
}

void Client::addSimulateData(int deviceId, double temp, double pressure, const QString& otherData) {
    clientNetworkManager->addSimulateDataRequest(deviceId, temp, pressure, otherData);
}

void Client::onAddSimulateDataResponseReceived(bool success, const QString& message) {
    emit addSimulateDataResult(success, message);
}

void Client::getNonContactUsers()
{
    clientNetworkManager->getNonContactUsersRequest(this->user->userId);
}

void Client::addContact(int contactUserId)
{
    clientNetworkManager->addContactRequest(this->user->userId, contactUserId);
}

void Client::onNonContactUsersResponseReceived(const QList<User> &users)
{
    emit nonContactUsersReceived(users);
}

void Client::onAddContactResponseReceived(bool success, const QString &message)
{
    emit addContactResult(success, message);
}

void Client::getAllDevices() {
    clientNetworkManager->getAllDevicesRequest();
}

void Client::onGetAllDevicesResponseReceived(bool success, const QString& message, const QVector<Device>& devices) {
    emit getAllDevicesResult(success, message, devices);
}

void Client::updateDevice(const QString& deviceName, const QString& type, const QString& status) {
    clientNetworkManager->updateDeviceRequest(deviceName, type, status, this->user->userId);
}

void Client::onUpdateDeviceResponseReceived(bool success, const QString& message) {
    emit updateDeviceResult(success, message);
}


void Client::onNewChatMessageReceived(int contactId, int senderId)
{
    emit newChatMessage(contactId, senderId);
}
