#include "videothread.h"
#include <QThread>
#include <QDebug>
#include <QFile> // 确保包含 QFile

// 避免重复定义构造函数，保留一个实现

VideoThread::VideoThread(QObject *parent)
    : QObject(parent), QRunnable(), video_Label(nullptr), ok(false) // 初始化 ok
{
    setAutoDelete(true);
}

VideoThread::VideoThread(QLabel *widget, QObject *parent)
    : QObject(parent), QRunnable(), video_Label(widget), ok(true) // 初始化 ok
{
    setAutoDelete(true);
}

VideoThread::~VideoThread()
{
    handle_CloseThread();
}

void VideoThread::run()
{
    qDebug() << "视频接收线程启动";
    while(ok) {
        QThread::msleep(100);
    }
    qDebug() << "视频接收线程结束";
}

void VideoThread::handle_ShowReceiveImg(QByteArray data)
{
    if(!video_Label || data.isEmpty()) return;

    // 添加调试输出
    qDebug() << "接收到图像数据大小:" << data.size() << "字节";

    // 尝试加载图像数据
    QPixmap pixmap;
    if(pixmap.loadFromData(data, "JPEG")) {
        // 成功加载图像
        qDebug() << "成功加载图像";
        video_Label->setPixmap(pixmap.scaled(video_Label->size(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    } else {
        qDebug() << "无法加载图像数据";

        // 保存接收到的数据到文件以便调试
        QFile file("received_data.bin");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
            qDebug() << "已将接收到的数据保存到 received_data.bin";
        }
    }
}

void VideoThread::handle_CloseThread()
{
    ok = false;
}
