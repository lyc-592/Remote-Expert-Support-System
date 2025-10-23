#ifndef VIDEO_TEST_H
#define VIDEO_TEST_H

#include "../../server/core/server.h"
#include "../ui/conference/conferencewindow.h"
#include "../../utils/network_interface_constants.h"
#include <QCameraInfo>

int videoTest(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 注册元类型
    qRegisterMetaType<QCameraInfo>();
    Server server;
    server.startServer(videoPort);

    // ConferenceWindow *w = new ConferenceWindow("192.168.149.129", videoPort);
    ConferenceWindow *w = new ConferenceWindow("", 0);
    w->setWindowTitle("视频聊天系统 (Qt5版本)");
    w->resize(800, 600);
    w->show();
    return a.exec();
}

#endif // VIDEO_TEST_H
