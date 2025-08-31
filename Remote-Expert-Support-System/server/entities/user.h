#ifndef USER_H
#define USER_H
#include <QMetaType>

#include "utils/constants.h"
#include <QJsonObject>

struct User {
    int userId;
    UserType userType;
    QString account;
    QString password;
    QString contact;
    QString expertSkill;

    // 默认构造函数
    User() : userId(0), userType(FactoryUser), account("default") {}

    // 带参数的构造函数
    User(UserType type, const QString& acc, const QString& pwd, const QString& cont, const QString& skill)
        : userType(type), account(acc), password(pwd), contact(cont), expertSkill(skill) {}

    // 转换为JSON格式，便于网络传输
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["userId"] = userId;
        obj["userType"] = static_cast<int>(userType);
        obj["account"] = account;
        obj["password"] = password;
        obj["contact"] = contact;
        obj["expertSkill"] = expertSkill;
        return obj;
    }

    // 从JSON解析
    static User fromJson(const QJsonObject& json) {
        User data;
        data.userId = json["userId"].toInt();
        data.userType = static_cast<UserType>(json["userType"].toInt());
        data.account = json["account"].toString();
        data.password = json["password"].toString();
        data.contact = json["contact"].toString();
        data.expertSkill = json["expertSkill"].toString();
        return data;
    }
};

// 注册元类型，便于信号槽传递
Q_DECLARE_METATYPE(User)
#endif // USER_H
