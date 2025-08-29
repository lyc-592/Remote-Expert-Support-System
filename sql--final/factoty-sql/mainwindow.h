#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QVariantMap>
#include <QList>

// 枚举定义
enum UserType {
    FactoryUser = 0,  // 工厂用户
    ExpertUser = 1    // 专家用户
};

enum OrderStatus {
    // 工单表状态
    PendingAccept = 0,  // 待接受
    Processing = 1,      // 处理中
    // 历史工单表状态
    Completed = 0,       // 已完成
    Rejected = 1         // 已拒绝
};

enum ContactStatus {
    PendingConfirmation = 0,  // 待确认
    Confirmed = 1,            // 已生效
    Terminated = 2            // 已解除
};

// 数据库管理单例类
class DatabaseManager {
public:
    // 单例模式：获取唯一实例
    static DatabaseManager& getInstance() {
        static DatabaseManager instance;
        return instance;
    }

    // 禁止拷贝构造和赋值
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // 数据库基础操作
    bool initDatabase(const QString& dbName = "factory_db.sqlite");  // 初始化数据库
    void closeDatabase();                                           // 关闭数据库
    bool isConnected() const;                                       // 检查连接状态

    // 1. 用户表操作
    bool addUser(int userId, UserType userType, const QString& account,
                 const QString& password, const QString& contact,
                 const QString& expertSkill = "");                  // 添加用户
    bool getUser(int userId, UserType& userType, QString& account,
                 QString& password, QString& contact, QString& expertSkill);  // 获取用户

    // 2. 设备表操作
    bool addDevice(int deviceId, int userId, const QString& deviceName,
                   const QString& deviceType);                      // 添加设备
    bool getDevice(int deviceId, int& userId, QString& deviceName,
                   QString& deviceType);                            // 获取设备

    // 3. 设备信息表操作（复合主键：info_id + data_time）
    bool addDeviceData(int infoId, const QDateTime& dataTime, int deviceId,
                       double temp, double pressure, const QVariantMap& otherData = QVariantMap());  // 添加设备数据
    bool getDeviceData(int infoId, const QDateTime& dataTime, int& deviceId,
                       double& temp, double& pressure, QVariantMap& otherData);  // 获取设备数据

    // 4. 联系人表操作（复合主键：user1_id + user2_id）
    bool addContact(int user1Id, int user2Id, ContactStatus status,
                    const QDateTime& createTime = QDateTime::currentDateTime());  // 添加联系人
    bool getContact(int user1Id, int user2Id, ContactStatus& status,
                    QDateTime& createTime);                          // 获取联系人

    // 5. 工单记录表操作
    bool addWorkOrder(int orderId, int user1Id, int user2Id, int deviceId,
                      OrderStatus status, const QDateTime& createTime,
                      const QDateTime& predictTime, const QString& fault);       // 添加工单
    bool getWorkOrder(int orderId, int& user1Id, int& user2Id, int& deviceId,
                      OrderStatus& status, QDateTime& createTime,
                      QDateTime& predictTime, QString& fault);                   // 获取工单

    // 6. 历史工单表操作
    bool addHistoryOrder(int historyOrderId, int user1Id, int user2Id, int deviceId,
                         OrderStatus status, const QDateTime& createTime,
                         const QDateTime& endTime);                  // 添加历史工单
    bool getHistoryOrder(int historyOrderId, int& user1Id, int& user2Id, int& deviceId,
                         OrderStatus& status, QDateTime& createTime,
                         QDateTime& endTime);                        // 获取历史工单

    // 7. 现场记录表操作
    bool addRecord(int recordId, int orderId, const QString& videoPath,
                   const QString& filePath, const QString& deviceDataPath,
                   const QString& faultType, const QString& solution,
                   const QDateTime& uploadTime = QDateTime::currentDateTime());  // 添加现场记录
    bool getRecord(int recordId, int& orderId, QString& videoPath,
                   QString& filePath, QString& deviceDataPath,
                   QString& faultType, QString& solution, QDateTime& uploadTime);  // 获取现场记录

private:
    // 私有构造/析构
    DatabaseManager() = default;
    ~DatabaseManager() { closeDatabase(); }

    // 私有方法：创建所有表结构
    bool createAllTables();

    // 成员变量：数据库连接对象
    QSqlDatabase m_db;
};

#endif // MAINWINDOW_H
