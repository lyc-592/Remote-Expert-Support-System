#ifndef CLIENT_SERVER_TEST_H
#define CLIENT_SERVER_TEST_H

#include <QApplication>
#include <QTimer>
#include <QDebug>
#include "../server/core/server.h"
#include "../client/core/client.h"

int clientServerTest(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // 创建服务器实例
    Server server;
    quint16 testPort = 1234;

    qInfo() << "=== Server Test with Multiple Clients ===";

    // 启动服务器
    if (!server.startServer(testPort)) {
        qCritical() << "Failed to start server!";
        return -1;
    }

    // 创建多个客户端实例
    QList<Client*> clients;
    clients.append(new Client("user1", "001", "password1"));
    clients.append(new Client("user2", "002", "password2"));
    clients.append(new Client("user3", "003", "password3"));

    // 连接计数器
    int connectedCount = 0;
    int totalClients = clients.size();

    // 连接所有客户端到服务器
    for (Client* client : clients) {
        // 连接客户端的信号到槽
        QObject::connect(client, &Client::connected, [&connectedCount, totalClients, client, &clients]() {
            connectedCount++;
            qInfo() << client->getUsername() << "connected successfully.";

            // 当所有客户端都连接成功后
            if (connectedCount == totalClients) {
                qInfo() << "All clients connected successfully!";

                // 等待一段时间后断开所有连接
                QTimer::singleShot(3000, [&clients]() {
                    qInfo() << "Disconnecting all clients...";
                    for (Client* client : clients) {
                        client->disconnectFromServer();
                    }

                    // 退出应用程序
                    QTimer::singleShot(2000, [&clients]() {
                        qInfo() << "=== Test completed ===";

                        // 清理客户端对象
                        for (Client* client : clients) {
                            delete client;
                        }
                        clients.clear();

                        QCoreApplication::quit();
                    });
                });
            }
        });

        QObject::connect(client, &Client::connectionFailed, [client]() {
            qWarning() << client->getUsername() << "failed to connect!";
        });

        // 连接到服务器
        client->connectToServer("127.0.0.1", testPort);
    }

    // 设置超时，防止某些客户端无法连接
    QTimer::singleShot(10000, [&]() {
        qWarning() << "Timeout reached. Not all clients connected.";
        qInfo() << "Connected:" << connectedCount << "/" << totalClients;

        // 清理客户端对象
        for (Client* client : clients) {
            delete client;
        }
        clients.clear();

        QCoreApplication::quit();
    });

    return a.exec();
}

#endif // CLIENT_SERVER_TEST_H
