#include "audiothread.h"
#include <QDebug>
#include <QThread>

AudioThread::AudioThread(ThreadType type,
                         QUdpSocket *socket,
                         QIODevice *device,
                         const QHostAddress &remoteAddress,
                         quint16 remotePort,
                         QObject *parent)
    : QObject(parent), QRunnable(),
    udpSocket(socket), audioDevice(device),
    targetAddress(remoteAddress), targetPort(remotePort),
    type(type), running(true)
{
    setAutoDelete(false);
}

AudioThread::~AudioThread()
{
    stop();
}

void AudioThread::run()
{
    if (type == Receiver) {
        while (running) {
            if (udpSocket->hasPendingDatagrams()) {
                QByteArray buffer;
                buffer.resize(udpSocket->pendingDatagramSize());
                QHostAddress sender;
                quint16 senderPort;

                qint64 bytesRead = udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
                if (bytesRead > 0 && audioDevice && audioDevice->isWritable()) {
                    qint64 bytesWritten = audioDevice->write(buffer);
                    if (bytesWritten != bytesRead) {
                        qDebug() << "Audio write error:" << bytesWritten << "of" << bytesRead;
                    }
                }
            }
            QThread::msleep(10);
        }
    }
}

void AudioThread::handleData(QByteArray data)
{
    if (type == Sender && running) {
        qint64 sent = udpSocket->writeDatagram(data, targetAddress, targetPort);
        if (sent != data.size()) {
            qDebug() << "Audio send error:" << sent << "of" << data.size();
        }
    }
}

void AudioThread::stop()
{
    running = false;
}
