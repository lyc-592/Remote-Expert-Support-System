#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QMetaType>
#include <QJsonObject>

struct Device {
    int deviceId;
    QString deviceName;
    QString deviceType;

    // 默认构造函数
    Device() {}

    // 带参数的构造函数
    Device(int deviceId, const QString& deviceName, const QString& deviceType)
        : deviceId(deviceId), deviceName(deviceName), deviceType(deviceType)
    {}

    // 转换为JSON格式，便于网络传输
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["deviceId"] = deviceId;
        obj["deviceName"] = deviceName;
        obj["deviceType"] = deviceType;
        return obj;
    }

    // 从JSON解析
    static Device fromJson(const QJsonObject& json) {
        Device device;
        device.deviceId = json["deviceId"].toInt();
        device.deviceName = json["deviceName"].toString();
        device.deviceType = json["deviceType"].toString();
        return device;
    }
};

// 注册元类型，便于信号槽传递
Q_DECLARE_METATYPE(Device)
#endif // DEVICE_H
