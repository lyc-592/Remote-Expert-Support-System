#include <QApplication>
#include <QTimer>
#include <QDebug>
#include "server.h"
#include "client.h"
#include "clientservertest.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // clientServerTest(argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
