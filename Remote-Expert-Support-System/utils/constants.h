#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <map>

// 用户类型枚举
enum UserType {
    FactoryUser = 0,  // 工厂用户
    ExpertUser = 1    // 专家用户
};

enum class RequestType {
    Login,
    Register,
    GetUser,
    GetAgenda,
    GetContact,
    Unknown // 用于处理未知的请求类型
};

// 辅助函数：将字符串映射到 RequestType
const static RequestType requestTypeFromString(const QString& typeString) {
    // 使用 static 局部变量来确保 map 只被初始化一次
    static const std::map<QString, RequestType> typeMap = {
        {"login", RequestType::Login},
        {"register", RequestType::Register},
        {"getUser", RequestType::GetUser},
        {"getAgenda", RequestType::GetAgenda},
        {"getContact", RequestType::GetContact}
    };

    // 查找映射
    auto it = typeMap.find(typeString);
    if (it != typeMap.end()) {
        return it->second;
    }
    return RequestType::Unknown; // 如果找不到对应的字符串，返回 Unknown
}
#endif // CONSTANTS_H
