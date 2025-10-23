#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <QDateTime>
#include <QJsonObject>

struct ChatMessage {
    int senderId;
    int receiverId;
    QString message;
    QString timestamp;
    bool isOwnMessage; // 用于前端判断是否是自己发送的消息

    ChatMessage() : senderId(0), receiverId(0) {}

    // 带参数的构造函数
    ChatMessage(int sId, int rId, const QString& msg, bool isOwnMessage, const QString& time = "")
        : senderId(sId), receiverId(rId), message(msg), isOwnMessage(isOwnMessage)
    {
        if (time.isEmpty()) {
            timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        } else {
            timestamp = time;
        }
    }

    // 转换为JSON格式，便于网络传输
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["sender_id"] = senderId;
        obj["receiver_id"] = receiverId;
        obj["message"] = message;
        obj["is_own"] = isOwnMessage;
        obj["timestamp"] = timestamp;
        return obj;
    }

    // 从JSON解析
    static ChatMessage fromJson(const QJsonObject& json) {
        ChatMessage msg;
        msg.senderId = json["sender_id"].toInt();
        msg.receiverId = json["receiver_id"].toInt();
        msg.message = json["message"].toString();
        msg.timestamp = json["timestamp"].toString();
        msg.isOwnMessage = json["is_own"].toBool();
        return msg;
    }

    // 判断是否是自己发送的消息
    bool isFromUser(int userId) const {
        return senderId == userId;
    }

    // 获取格式化时间（可选）
    QString getFormattedTime(const QString& format = "HH:mm") const {
        QDateTime dt = QDateTime::fromString(timestamp, Qt::ISODate);
        if (dt.isValid()) {
            return dt.toString(format);
        }
        return timestamp; // 如果解析失败，返回原始时间戳
    }
};

#endif // CHAT_MESSAGE_H
