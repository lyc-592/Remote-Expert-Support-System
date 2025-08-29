#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonArray>
#include <QJsonObject>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connectToDatabase(const QString& databaseName="default.db");
    void disconnectFromDatabase();
    bool isConnected() const;

    QJsonArray getUserInfo(int userId);
    QJsonArray getAllUsers();

    bool validateUser(const QString &username, const QString &password);

private:
    QSqlDatabase db;

    QString encryptPassword(const QString &password);
};

#endif // DATABASEMANAGER_H
