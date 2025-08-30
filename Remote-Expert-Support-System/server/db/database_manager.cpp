#include "database_manager.h"
#include "../../utils/funcs.h"
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

/* 数据库连接、断连、检查 */
bool DatabaseManager::connectToDatabase(const QString& databaseName)
{
    if (QSqlDatabase::contains("factory_connection")) {
        db = QSqlDatabase::database("factory_connection");
    } else {
        // 加载SQLite驱动
        db = QSqlDatabase::addDatabase("QSQLITE", "factory_connection");
        db.setDatabaseName(databaseName);
    }
    // 打开数据库
    if (!db.open()) {
        qCritical() << "[数据库错误] 打开失败：" << db.lastError().text();
        return false;
    }
    // 创建所有表结构
    if (!createAllTables()) {
        qCritical() << "[数据库错误] 表创建失败";
        disconnectFromDatabase();
        return false;
    }
    qInfo() << "[数据库成功] 初始化完成，连接已建立";
    return true;
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
    // 移除连接
    QSqlDatabase::removeDatabase("factory_connection");
}

bool DatabaseManager::isConnected() const {
    return db.isOpen();
}

// 创建所有表结构
bool DatabaseManager::createAllTables() {
    QSqlQuery query(db);
    QString sql;

    // 1. 创建用户表（user）
    sql = R"(
        CREATE TABLE IF NOT EXISTS user (
            user_id INTEGER PRIMARY KEY,
            user_type INTEGER NOT NULL CHECK (user_type IN (0, 1)),
            user_account TEXT NOT NULL UNIQUE,
            user_password TEXT NOT NULL,
            user_contact TEXT NOT NULL,
            expert_skill TEXT,
            -- 约束：专家用户必须填写擅长技能
            CHECK (
                (user_type = 1 AND expert_skill IS NOT NULL AND expert_skill != '') OR
                user_type = 0
            )
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] user表：" << query.lastError().text();
        return false;
    }

    // 2. 创建设备表（device）
    sql = R"(
        CREATE TABLE IF NOT EXISTS device (
            device_id INTEGER PRIMARY KEY,
            user_id INTEGER NOT NULL,
            device_name TEXT NOT NULL,
            device_type TEXT NOT NULL,
            -- 外键：关联用户表
            FOREIGN KEY (user_id) REFERENCES user(user_id) ON DELETE CASCADE,
            -- 约束：设备名称+类型唯一
            UNIQUE (device_name, device_type)
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] device表：" << query.lastError().text();
        return false;
    }

    // 3. 创建设备信息表（device_data）- 复合主键
    sql = R"(
        CREATE TABLE IF NOT EXISTS device_data (
            info_id INTEGER NOT NULL,
            data_time DATETIME NOT NULL,
            device_id INTEGER NOT NULL,
            temp REAL,
            pressure REAL,
            other_data TEXT,
            -- 复合主键
            PRIMARY KEY (info_id, data_time),
            -- 外键关联设备表
            FOREIGN KEY (device_id) REFERENCES device(device_id) ON DELETE CASCADE
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] device_data表：" << query.lastError().text();
        return false;
    }

    // 4. 创建联系人表（contact）- 复合主键
    sql = R"(
        CREATE TABLE IF NOT EXISTS contact (
            user1_id INTEGER NOT NULL,
            user2_id INTEGER NOT NULL,
            contact_status INTEGER NOT NULL CHECK (contact_status IN (0, 1, 2)),
            create_time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
            -- 复合主键
            PRIMARY KEY (user1_id, user2_id),
            -- 外键关联用户表
            FOREIGN KEY (user1_id) REFERENCES user(user_id) ON DELETE CASCADE,
            FOREIGN KEY (user2_id) REFERENCES user(user_id) ON DELETE CASCADE,
            -- 约束：不能自己加自己为联系人
            CHECK (user1_id != user2_id)
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] contact表：" << query.lastError().text();
        return false;
    }

    // 5. 创建工单记录表（workorder）
    sql = R"(
        CREATE TABLE IF NOT EXISTS workorder (
            order_id INTEGER PRIMARY KEY,
            user1_id INTEGER NOT NULL,
            user2_id INTEGER NOT NULL,
            device_id INTEGER NOT NULL,
            order_status INTEGER NOT NULL CHECK (order_status IN (0, 1)),
            order_create_time DATETIME NOT NULL,
            predict_time DATETIME NOT NULL,
            fault TEXT NOT NULL,
            -- 外键关联联系人表（复合外键）
            FOREIGN KEY (user1_id, user2_id) REFERENCES contact(user1_id, user2_id) ON DELETE CASCADE,
            -- 外键关联设备表
            FOREIGN KEY (device_id) REFERENCES device(device_id) ON DELETE CASCADE,
            -- 约束：预计时间不能早于创建时间
            CHECK (predict_time >= order_create_time)
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] workorder表：" << query.lastError().text();
        return false;
    }

    // 6. 创建历史工单表（history_order）
    sql = R"(
        CREATE TABLE IF NOT EXISTS history_order (
            history_order_id INTEGER PRIMARY KEY,
            user1_id INTEGER NOT NULL,
            user2_id INTEGER NOT NULL,
            device_id INTEGER NOT NULL,
            order_status INTEGER NOT NULL CHECK (order_status IN (0, 1)),
            order_create_time DATETIME NOT NULL,
            order_end_time DATETIME NOT NULL,
            -- 外键关联联系人表（复合外键）
            FOREIGN KEY (user1_id, user2_id) REFERENCES contact(user1_id, user2_id) ON DELETE CASCADE,
            -- 外键关联设备表
            FOREIGN KEY (device_id) REFERENCES device(device_id) ON DELETE CASCADE,
            -- 约束：结束时间不能早于创建时间
            CHECK (order_end_time >= order_create_time)
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] history_order表：" << query.lastError().text();
        return false;
    }

    // 7. 创建现场记录表（record）
    sql = R"(
        CREATE TABLE IF NOT EXISTS record (
            record_id INTEGER PRIMARY KEY,
            order_id INTEGER NOT NULL,
            video_path TEXT,
            file_path TEXT,
            device_status_data_path TEXT,
            fault_type TEXT NOT NULL,
            solution TEXT NOT NULL,
            upload_time DATETIME NOT NULL,
            -- 外键关联工单表
            FOREIGN KEY (order_id) REFERENCES workorder(order_id) ON DELETE CASCADE,
            -- 约束：故障类型和解决方案不能为空
            CHECK (fault_type != '' AND solution != '')
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] record表：" << query.lastError().text();
        return false;
    }

    qInfo() << "[表创建成功] 所有表结构已生成";
    return true;
}

bool DatabaseManager::validateUser(const QString& account, const QString &password) {
    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return false;
    }
    QString encryptedPassword = encryptPassword(password);
    // 查询用户表，寻找是否有用户
    QSqlQuery query(db);
    query.prepare("SELECT user_id FROM user WHERE user_account = :account AND user_password = :password");
    query.bindValue(":account", account);
    query.bindValue(":password", encryptedPassword);
    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return false;
    }
    return query.next();
}

bool DatabaseManager::addUser(UserType userType, const QString& account,
                              const QString& password, const QString& contact,
                              const QString& expertSkill) {
    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO user (user_type, user_account, user_password, user_contact, expert_skill)
        VALUES (:userType, :account, :password, :contact, :expertSkill)
    )");
    query.bindValue(":userType", userType);
    query.bindValue(":account", account);
    query.bindValue(":password", password);
    query.bindValue(":contact", contact);
    query.bindValue(":expertSkill", expertSkill);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 添加失败：" << query.lastError().text();
        return false;
    } else {
        QVariant id = query.lastInsertId();
        if (id.isValid()) {
            int newUserId = id.toInt();
            qDebug() << "New user created with ID:" << newUserId;
            qInfo() << "[用户操作成功] 添加用户：ID=" << newUserId << "，账号=" << account;
            return true;
        } else {
            qDebug() << "Failed to get last insert ID";
            return false;
        }
    }
}

bool DatabaseManager::getUser(int userId, UserType& userType, QString& account,
                              QString& password, QString& contact, QString& expertSkill) {
    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM user WHERE user_id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec() || !query.next()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return false;
    }

    userType = static_cast<UserType>(query.value("user_type").toInt());
    account = query.value("user_account").toString();
    password = query.value("user_password").toString();
    contact = query.value("user_contact").toString();
    expertSkill = query.value("expert_skill").toString();

    return true;
}

// ==================== 议程 ====================
QList<int> DatabaseManager::getDeviceIds() {
    QList<int> list;
    if (!isConnected()) return list;

    QSqlQuery query(db);
    if (!query.exec("SELECT device_id FROM device")) {
        qCritical() << "[数据库错误] getDeviceIds:" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        list.append(query.value(0).toInt());
    }
    return list;
}

QList<QPair<int, QString>> DatabaseManager::getExperts() {
    QList<QPair<int, QString>> list;
    if (!isConnected()) return list;

    QSqlQuery query(db);
    if (!query.exec("SELECT user_id, user_account FROM user WHERE user_type=1")) {
        qCritical() << "[数据库错误] getExperts:" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        list.append({query.value(0).toInt(), query.value(1).toString()});
    }
    return list;
}

bool DatabaseManager::addWorkOrder(int user1_id, int user2_id, int device_id,
                                   int order_status, const QString &order_create_time,
                                   const QString &predict_time, const QString &fault) {
    if (!isConnected()) return false;

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO workorder (user1_id, user2_id, device_id, order_status,
                               order_create_time, predict_time, fault)
        VALUES (:u1, :u2, :d, :st, :ct, :pt, :fault)
    )");
    query.bindValue(":u1", user1_id);
    query.bindValue(":u2", user2_id);
    query.bindValue(":d", device_id);
    query.bindValue(":st", order_status);
    query.bindValue(":ct", order_create_time);
    query.bindValue(":pt", predict_time);
    query.bindValue(":fault", fault);

    if (!query.exec()) {
        qCritical() << "[数据库错误] addWorkOrder:" << query.lastError().text();
        return false;
    }
    return true;
}

