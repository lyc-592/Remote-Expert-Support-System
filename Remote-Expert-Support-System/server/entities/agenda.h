#ifndef AGENDA_H
#define AGENDA_H

#include <QDateTime>
#include <QString>
#include <QMetaType>
#include <QJsonObject>

struct Agenda {
    int orderId;
    int user1Id;
    int user2Id;
    int deviceId;
    int status;
    QDateTime createTime;
    QDateTime predictTime;
    QString fault;

    // 默认构造函数
    Agenda()
        : orderId(0), user1Id(0), user2Id(0), deviceId(0), status(0)
    {}

    // 带参数的构造函数
    Agenda(int orderId, int user1Id, int user2Id, int deviceId,
           int status, const QDateTime &createTime,
           const QDateTime &predictTime, const QString &fault)
        : orderId(orderId), user1Id(user1Id), user2Id(user2Id), deviceId(deviceId),
        status(status), createTime(createTime), predictTime(predictTime), fault(fault)
    {}

    // 状态文本描述
    QString statusText() const {
        switch(status) {
        case 0: return "待处理";
        case 1: return "已完成";
        default: return "未知状态";
        }
    }

    // 转换为JSON格式，便于网络传输
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["orderId"] = orderId;
        obj["user1Id"] = user1Id;
        obj["user2Id"] = user2Id;
        obj["deviceId"] = deviceId;
        obj["status"] = status;
        obj["createTime"] = createTime.toString(Qt::ISODate);
        obj["predictTime"] = predictTime.toString(Qt::ISODate);
        obj["fault"] = fault;
        return obj;
    }

    // 从JSON解析
    static Agenda fromJson(const QJsonObject& json) {
        Agenda agenda;
        agenda.orderId = json["orderId"].toInt();
        agenda.user1Id = json["user1Id"].toInt();
        agenda.user2Id = json["user2Id"].toInt();
        agenda.deviceId = json["deviceId"].toInt();
        agenda.status = json["status"].toInt();
        agenda.createTime = QDateTime::fromString(json["createTime"].toString(), Qt::ISODate);
        agenda.predictTime = QDateTime::fromString(json["predictTime"].toString(), Qt::ISODate);
        agenda.fault = json["fault"].toString();
        return agenda;
    }
};

// 注册元类型，便于信号槽传递
Q_DECLARE_METATYPE(Agenda)

#endif // AGENDA_H
