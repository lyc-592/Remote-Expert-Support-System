#include "mainwindow.h"
#include <QCoreApplication>
#include <QDateTime>

// 演示数据库操作
void demoDatabaseOperations() {
    // 获取数据库实例
    DatabaseManager& dbMgr = DatabaseManager::getInstance();

    // 初始化数据库
    if (!dbMgr.initDatabase()) {
        qCritical() << "数据库初始化失败，无法进行操作";
        return;
    }

    // 添加用户
    dbMgr.addUser(1, FactoryUser, "factory_001", "password123", "13800138000");
    dbMgr.addUser(2, ExpertUser, "expert_001", "expert123", "13900139000",
                  "PLC编程,工业机器人故障处理");

    // 添加设备（属于工厂用户1）
    dbMgr.addDevice(101, 1, "注塑机A", "注塑设备");
    dbMgr.addDevice(102, 1, "冲压机B", "冲压设备");

    // 添加设备数据
    QVariantMap otherData;
    otherData["vibration"] = 0.05;
    otherData["current"] = 12.5;
    dbMgr.addDeviceData(1, QDateTime::currentDateTime(), 101, 35.2, 0.65, otherData);

    // 添加联系人关系（工厂用户1联系专家用户2）
    dbMgr.addContact(1, 2, PendingConfirmation);

    // 添加工单（关联联系人1-2和设备101）
    dbMgr.addWorkOrder(1001, 1, 2, 101, PendingAccept,
                       QDateTime::currentDateTime(),
                       QDateTime::currentDateTime().addDays(1),
                       "设备运行时有异常噪音，温度偏高");

    // 添加历史工单
    dbMgr.addHistoryOrder(2001, 1, 2, 102, Completed,
                          QDateTime::currentDateTime().addDays(-2),
                          QDateTime::currentDateTime().addDays(-1));

    // 添加现场记录
    dbMgr.addRecord(3001, 1001, "/videos/record1.mp4", "/files/report.pdf",
                    "/data/status1.json", "机械磨损", "更换轴承后恢复正常");

    // 验证数据
    UserType userType;
    QString account, password, contact, skill;
    if (dbMgr.getUser(1, userType, account, password, contact, skill)) {
        qInfo() << "\n验证用户数据：";
        qInfo() << "用户ID: 1, 账号:" << account
                << ", 类型:" << (userType == FactoryUser ? "工厂用户" : "专家用户");
    }

    // 关闭数据库
    dbMgr.closeDatabase();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);  // 无UI界面，使用QCoreApplication

    // 执行数据库操作演示
    demoDatabaseOperations();

    return a.exec();
}
