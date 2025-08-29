#include "databasemanager.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QCryptographicHash>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    disconnectFromDatabase();
}

bool DatabaseManager::connectToDatabase(const QString& databaseName)
{
    db.setDatabaseName(databaseName);

    if (!db.open()) {
        qCritical() << "Database connection error:" << db.lastError().text();
        return false;
    }

    qInfo() << "Connected to database successfully";
    return true;
}

void DatabaseManager::disconnectFromDatabase()
{
    if (db.isOpen()) {
        db.close();
        qInfo() << "Disconnected from database";
    }
}

QJsonArray DatabaseManager::getUserInfo(int userId)
{
    QJsonArray result;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return result;
    }

    QSqlQuery query;
    query.prepare("SELECT id, name, department, role, email FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (query.exec() && query.next()) {
        QJsonObject user;
        user["id"] = query.value("id").toInt();
        user["name"] = query.value("name").toString();
        user["department"] = query.value("department").toString();
        user["role"] = query.value("role").toString();
        user["email"] = query.value("email").toString();

        result.append(user);
    } else {
        qWarning() << "Failed to get user info:" << query.lastError().text();
    }

    return result;
}

QJsonArray DatabaseManager::getAllUsers()
{
    QJsonArray result;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return result;
    }

    QSqlQuery query("SELECT id, name, department, role, email FROM users");

    while (query.next()) {
        QJsonObject user;
        user["id"] = query.value("id").toInt();
        user["name"] = query.value("name").toString();
        user["department"] = query.value("department").toString();
        user["role"] = query.value("role").toString();
        user["email"] = query.value("email").toString();

        result.append(user);
    }

    return result;
}

bool DatabaseManager::isConnected() const {
    return db.isOpen();
}

bool DatabaseManager::validateUser(const QString &username, const QString &password) {
    if (!isConnected()) {
        return false;
    }
    QString encryptedPassword = encryptPassword(password);
    // 查询用户表，寻找是否有用户
    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", encryptedPassword);
    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return false;
    }
    return query.next();
}

QString DatabaseManager::encryptPassword(const QString &password)
{
    // 使用SHA-256加密密码
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}
