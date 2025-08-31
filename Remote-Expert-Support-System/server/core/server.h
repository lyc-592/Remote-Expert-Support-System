#ifndef SERVER_H
#define SERVER_H

#include "../db/database_manager.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    // 运行前启动服务器
    bool startServer(quint16 port);
    void stopServer();
    bool isPortAvailable(quint16 port);
signals:
    // 服务器接收信号，并显示在公屏上
    void newMessage(const QByteArray &message);
    // 判断用户连接、断连
    void clientConnected();
    void clientDisconnected();

protected:
    // 允许多个用户接入同一个服务器
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    // 当接收到请求，首先在onReadyRead()中处理JSON文件，判断请求类型后调用这些方法
    void processLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void processRegisterRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void processSetUsernameRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void processSetUsernameWithPasswordRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void processGetUserRequest(QTcpSocket *clientSocket, const QJsonObject &request);

    void processGetAgendaRequest(QTcpSocket *clientSocket, const QJsonObject &request);

    void processGetContactRequest(QTcpSocket *clientSocket, const QJsonObject &request);

    void processGetDeviceRequest(QTcpSocket *clientSocket, const QJsonObject &request);

    void processAddAgenda(QTcpSocket *clientSocket, const QJsonObject &request);

    void processAddDevice(QTcpSocket *clientSocket, const QJsonObject &request);

    void sendResponse(QTcpSocket *clientSocket, const QJsonObject &response);


    QMap<QTcpSocket*, QString> clientSockets;
    // 在 Server 类中为每个 QTcpSocket 维护一个缓冲区
    QHash<QTcpSocket*, QByteArray> buffers;
    DatabaseManager *dbManager;
};

#endif // SERVER_H
