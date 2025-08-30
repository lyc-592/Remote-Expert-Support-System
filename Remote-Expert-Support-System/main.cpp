#include <QApplication>
#include "server/core/server.h"
#include "ui/login/login_window.h"
#include "ui/login/register_dialog.h"
#include "utils/network_interface_constants.h"
#include "ui/index/factory/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Server server;
    server.startServer(hostPort);
    // 设置应用程序信息
    QApplication::setApplicationName("Qt QSS 示例");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("MyCompany");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return app.exec();
}
