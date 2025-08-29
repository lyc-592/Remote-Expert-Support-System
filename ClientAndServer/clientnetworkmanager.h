#ifndef CLIENTNETWORKMANAGER_H
#define CLIENTNETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ClientNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientNetworkManager(QObject *parent = nullptr);
    ~ClientNetworkManager();

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    bool isConnected() const;

    void loginRequest(const QString &username, const QString &password);
    void sendMessage(const QByteArray &message);

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorMessage);
    void loginResponseReceived(bool success, const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void onReadyRead();

private:
    QTcpSocket *tcpSocket;
    QMap<QString, std::function<void(const QJsonObject&)>> pendingRequests;
    quint32 requestIdCounter;
};

#endif // CLIENTNETWORKMANAGER_H
