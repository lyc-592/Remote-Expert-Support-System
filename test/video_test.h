#ifndef VIDEO_TEST_H
#define VIDEO_TEST_H

#include "../ui/conference/mainwindow.h"
#include <QCameraInfo>

int videoTest(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 注册元类型
    qRegisterMetaType<QCameraInfo>();

    ConferenceWindow w;
    w.setWindowTitle("视频聊天系统 (Qt5版本)");
    w.resize(800, 600);
    w.show();
    return a.exec();
}

#endif // VIDEO_TEST_H
