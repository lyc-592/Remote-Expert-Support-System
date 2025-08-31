#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "../../utils/constants.h"
#include "../entities/user.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonArray>
#include <QJsonObject>
#include "../entities/agenda.h"
#include "../entities/device.h"

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
    User setUsername(const QString &username, int userId);
    User setUsername(const QString &username, const QString &password, int userId);
    User getUser(const QString& account);
    QVector<Agenda> getAgenda(const int userId, const UserType userType);
    QVector<User> getContact(const int userId);

    QVector<Device> getDevice(const int userId);

    // QList<int> getDeviceIds();
    // QList<QPair<int, QString>> getExperts();  // 查询专家列表
    bool addWorkOrder(int user1_id, const QString& expertname, const QString& device, const QString& ordername,
                      const QString &order_create_time, const QString &predict_time, const QString& status, const QString &fault);

    bool addDevice(const QString& name, const QString& type, int userId);

    // 添加数据库访问方法
    QSqlDatabase getDatabase() const;
    QSqlQuery createQuery() const;
    bool executeQuery(const QString &queryStr, QSqlQuery &query);

    // 添加特定的业务方法
    QSqlQuery getHistoryOrderDetails(int orderId);
private:
    QSqlDatabase db;

    bool createAllTables();
};

#endif // DATABASE_MANAGER_H
