#ifndef SERVER_H
#define SERVER_H

#include "databasemanager.h"
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

    bool startServer(quint16 port);
    void stopServer();
    bool isPortAvailable(quint16 port);
signals:
    void newMessage(const QByteArray &message);
    void clientConnected();
    void clientDisconnected();
    void clientReadyRead();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    void processLoginRequest(QTcpSocket *clientSocket, const QJsonObject &request);
    void sendResponse(QTcpSocket *clientSocket, const QJsonObject &response);

    QMap<QTcpSocket*, QString> clientSockets;
    DatabaseManager *dbManager;
};

#endif // SERVER_H
