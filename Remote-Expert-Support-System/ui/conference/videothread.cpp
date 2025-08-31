#include "videothread.h"
#include <QThread>

VideoThread::VideoThread(QObject *parent)
    : QObject(parent), QRunnable(), video_Label(nullptr)
{
    setAutoDelete(true);
}

VideoThread::VideoThread(QLabel *widget, QObject *parent)
    : QObject(parent), QRunnable(), video_Label(widget)
{
    ok = true;
    setAutoDelete(true);
}

VideoThread::~VideoThread()
{
    handle_CloseThread();
}

void VideoThread::run()
{
    while(ok) {
        QThread::msleep(100);
    }
}

void VideoThread::handle_ShowReceiveImg(QByteArray data)
{
    if(!video_Label || data.isEmpty()) return;

    QPixmap pixmap;
    pixmap.loadFromData(data);

    if(!pixmap.isNull()){
        video_Label->setPixmap(pixmap.scaled(video_Label->size(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    }
}

void VideoThread::handle_CloseThread()
{
    ok = false;
}
