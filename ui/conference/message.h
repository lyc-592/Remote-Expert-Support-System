#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <QImage>

enum MessageType {
    TextMessage = 0,
    ImageMessage = 1,
    FileMessage = 2
};

class Message
{
public:
    Message();
    Message(MessageType type, const QByteArray &data, const QString &info = "");

    static Message createTextMessage(const QString &text);
    static Message createImageMessage(const QImage &image, const QString &format = "PNG");
    static Message createFileMessage(const QByteArray &fileData, const QString &fileName);

    QByteArray toByteArray() const;
    static Message fromByteArray(const QByteArray &data);

    MessageType type() const;
    QByteArray data() const;
    QString info() const;

    QString text() const;
    QImage image() const;
    QString fileName() const;
    qint64 fileSize() const;

    bool saveToFile(const QString &filePath) const;

private:
    MessageType m_type;
    QByteArray m_data;
    QString m_info;
};

#endif // MESSAGE_H
