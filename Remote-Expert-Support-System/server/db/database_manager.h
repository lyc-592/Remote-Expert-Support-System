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

private:
    QSqlDatabase db;

    bool createAllTables();
};

#endif // DATABASE_MANAGER_H
