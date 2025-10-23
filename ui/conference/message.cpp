#include "message.h"
#include <QFile>
#include <QDir>
#include <QIODevice>

Message::Message() : m_type(TextMessage)
{
}

Message::Message(MessageType type, const QByteArray &data, const QString &info)
    : m_type(type), m_data(data), m_info(info)
{
}

Message Message::createTextMessage(const QString &text)
{
    return Message(TextMessage, text.toUtf8());
}

Message Message::createImageMessage(const QImage &image, const QString &format)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, format.toStdString().c_str());
    return Message(ImageMessage, imageData, format);
}

Message Message::createFileMessage(const QByteArray &fileData, const QString &fileName)
{
    return Message(FileMessage, fileData, fileName);
}

QByteArray Message::toByteArray() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << static_cast<qint32>(m_type);
    stream << m_info;
    stream << m_data;
    return data;
}

Message Message::fromByteArray(const QByteArray &data)
{
    QDataStream stream(data);
    qint32 type;
    QString info;
    QByteArray messageData;

    stream >> type;
    stream >> info;
    stream >> messageData;

    return Message(static_cast<MessageType>(type), messageData, info);
}

MessageType Message::type() const { return m_type; }
QByteArray Message::data() const { return m_data; }
QString Message::info() const { return m_info; }

QString Message::text() const
{
    if (m_type == TextMessage)
        return QString::fromUtf8(m_data);
    return QString();
}

QImage Message::image() const
{
    if (m_type == ImageMessage)
        return QImage::fromData(m_data);
    return QImage();
}

QString Message::fileName() const
{
    if (m_type == FileMessage)
        return m_info;
    return QString();
}

qint64 Message::fileSize() const
{
    if (m_type == FileMessage)
        return m_data.size();
    return 0;
}

bool Message::saveToFile(const QString &filePath) const
{
    if (m_data.isEmpty()) {
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    qint64 bytesWritten = file.write(m_data);
    file.close();

    return bytesWritten == m_data.size();
}
