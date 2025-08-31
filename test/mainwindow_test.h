#ifndef MAINWINDOW_TEST_H
#define MAINWINDOW_TEST_H
#include <QApplication>
#include "../client/core/client.h"
#include "../server/core/server.h"
#include "ui/index/factory/main_window.h"
#include "utils/constants.h"

// int mainwindowTest(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     Server server;
//     server.startServer(hostPort);
//     QThread::msleep(1000);

//     User *user = new User(0, FactoryUser, "default", "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3", "2642569199@qq.com", nullptr);
//     Client *client = new Client(user);

//     MainWindow *mainWindow = new MainWindow(client);
//     mainWindow->show();

//     return app.exec();
// }

#endif // MAINWINDOW_TEST_H
