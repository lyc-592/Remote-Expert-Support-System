#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "../../utils/constants.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonArray>
#include <QJsonObject>

/* 服务器内嵌DatabaseManager，实现与SQLite数据库的关联 */
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // 数据库连接与初始化等
    bool connectToDatabase(const QString& databaseName="default.db");
    void disconnectFromDatabase();
    bool isConnected() const;

    bool validateUser(const QString &account, const QString &password);
    bool addUser(UserType userType, const QString& account,
                            const QString& password, const QString& contact,
                            const QString& expertSkill="");
    bool getUser(int userId, UserType& userType, QString& account,
                 QString& password, QString& contact, QString& expertSkill);

    // 专家和议程相关接口
    QList<int> getDeviceIds();
    QList<QPair<int, QString>> getExperts();  // 查询专家列表
    bool addWorkOrder(int user1_id, int user2_id, int device_id,
                      int order_status, const QString& order_create_time,
                      const QString& predict_time, const QString& fault);
private:
    QSqlDatabase db;

    bool createAllTables();
};

#endif // DATABASE_MANAGER_H
