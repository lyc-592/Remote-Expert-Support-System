#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QRunnable>
#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QThread>
#include <QFile> // 添加 QFile 头文件

class VideoThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit VideoThread(QObject *parent = nullptr);
    VideoThread(QLabel *widget, QObject *parent = nullptr);
    ~VideoThread();

    void run() override;

public slots:
    void handle_ShowReceiveImg(QByteArray data);
    void handle_CloseThread();

private:
    QLabel *video_Label;
    bool ok;
};

#endif // VIDEOTHREAD_H
