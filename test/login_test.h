#ifndef LOGIN_TEST_H
#define LOGIN_TEST_H
#include <QApplication>
#include "../server/core/server.h"
#include "../ui/login/login_window.h"
int loginTest(int argc, char *argv[]) {

    QApplication app(argc, argv);

    Server server;
    server.startServer(hostPort);
    // 设置应用程序信息
    QApplication::setApplicationName("Qt QSS 示例");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("MyCompany");

    LoginWindow w;
    w.show();

    return app.exec();
}

#endif // LOGIN_TEST_H
