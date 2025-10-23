#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QRunnable>
#include <QObject>
#include <QAudioFormat>
#include <QIODevice>
#include <QUdpSocket>

class AudioThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum ThreadType { Sender, Receiver };

    explicit AudioThread(ThreadType type,
                         QUdpSocket *socket,
                         QIODevice *device,
                         const QHostAddress &remoteAddress,
                         quint16 remotePort,
                         QObject *parent = nullptr);
    ~AudioThread();

    void run() override;
    void stop();

public slots:
    void handleData(QByteArray data);

private:
    QUdpSocket *udpSocket;
    QIODevice *audioDevice;
    QHostAddress targetAddress;
    quint16 targetPort;
    ThreadType type;
    bool running;
};

#endif // AUDIOTHREAD_H
