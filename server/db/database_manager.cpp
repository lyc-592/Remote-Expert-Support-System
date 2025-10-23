#include "database_manager.h"
#include "../../utils/funcs.h"
#include "../entities/user.h"
#include "../entities/agenda.h"
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
            status TEXT NOT NULL,
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

    sql = R"(
    CREATE TRIGGER IF NOT EXISTS trigger_workorder_to_history
    AFTER DELETE ON workorder
    FOR EACH ROW
    BEGIN
        -- 插入历史表：复制原工单所有字段，order_end_time设为当前时间
        INSERT INTO history_order (
            user1_id, user2_id, device_id,
            order_status, order_create_time, order_end_time, fault
        ) VALUES (
            OLD.user1_id,          -- 原工单的发起用户ID
            OLD.user2_id,          -- 原工单的接收用户ID
            OLD.device_id,         -- 原工单关联的设备ID
            OLD.order_status,      -- 原工单状态
            OLD.order_create_time, -- 原工单创建时间
            datetime('now', 'localtime'), -- 当前本地时间作为工单结束时间
            OLD.fault              -- 原工单的故障描述
        );
    END;
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] 触发器表：" << query.lastError().text();
        return false;
    }

    sql = R"(
        CREATE TABLE IF NOT EXISTS message (
            user1_id INTEGER NOT NULL,
            user2_id INTEGER NOT NULL,
            time TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            message TEXT NOT NULL,
            PRIMARY KEY(user1_id, user2_id, time),
            FOREIGN KEY(user1_id, user2_id) REFERENCES contact(user1_id, user2_id)
        )
    )";
    if (!query.exec(sql)) {
        qCritical() << "[表创建错误] message表：" << query.lastError().text();
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
    QString encryptedPassword = password;
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
    // 首先检查账号是否已存在
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM user WHERE user_account = :account");
    checkQuery.bindValue(":account", account);

    if (!checkQuery.exec()) {
        qWarning() << "[用户操作错误] 检查用户存在性失败：" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qWarning() << "[用户操作错误] 账号已存在：" << account;
        return false;
    }

    int type = userType == FactoryUser ? 0 : 1;
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO user (user_type, user_account, user_password, user_contact, expert_skill)
        VALUES (:userType, :account, :password, :contact, :expertSkill)
    )");
    query.bindValue(":userType", type);
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

User DatabaseManager::setUsername(const QString &username, int userId) {
    User userData;

    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return userData;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE user SET user_account = :username WHERE user_id = :userId");
    query.bindValue(":username", username);
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return userData;
    }

    if (query.numRowsAffected() <= 0) {
        qWarning() << "[用户操作错误] 未找到用户ID：" << userId;
        return userData;
    }

    QSqlQuery returnQuery(db);
    returnQuery.prepare("SELECT user_id, user_type, user_account, user_password, user_contact, expert_skill FROM user WHERE user_id = :userId");
    returnQuery.bindValue(":userId", userId);
    if (!returnQuery.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << returnQuery.lastError().text();
        return userData;
    }

    if (!returnQuery.next()) {
        qWarning() << "[用户操作错误] 未找到用户账号：" << username;
        return userData;
    }

    userData.userId = returnQuery.value("user_id").toInt();
    userData.userType = static_cast<UserType>(returnQuery.value("user_type").toInt());
    userData.account = returnQuery.value("user_account").toString();
    userData.password = returnQuery.value("user_password").toString();
    userData.contact = returnQuery.value("user_contact").toString();
    userData.expertSkill = returnQuery.value("expert_skill").toString();
    return userData;
}

User DatabaseManager::setUsername(const QString &username, const QString &password, int userId) {
    User userData;

    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return userData;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE user SET user_account = :username AND user_password = :password WHERE user_id = :userId");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return userData;
    }

    if (query.numRowsAffected() <= 0) {
        qWarning() << "[用户操作错误] 未找到用户ID：" << userId;
        return userData;
    }

    QSqlQuery returnQuery(db);
    returnQuery.prepare("SELECT user_id, user_type, user_account, user_password, user_contact, expert_skill FROM user WHERE user_id = :userId");
    returnQuery.bindValue(":userId", userId);
    if (!returnQuery.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << returnQuery.lastError().text();
        return userData;
    }

    if (!returnQuery.next()) {
        qWarning() << "[用户操作错误] 未找到用户账号：" << username;
        return userData;
    }

    userData.userId = returnQuery.value("user_id").toInt();
    userData.userType = static_cast<UserType>(returnQuery.value("user_type").toInt());
    userData.account = returnQuery.value("user_account").toString();
    userData.password = returnQuery.value("user_password").toString();
    userData.contact = returnQuery.value("user_contact").toString();
    userData.expertSkill = returnQuery.value("expert_skill").toString();

    return userData;
}

// 返回整个条目
User DatabaseManager::getUser(const QString& account) {
    User userData;

    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return userData;
    }

    QSqlQuery query(db);
    query.prepare("SELECT user_id, user_type, user_account, user_password, user_contact, expert_skill FROM user WHERE user_account = :userAccount");
    query.bindValue(":userAccount", account); // 注意占位符名称要与SQL中一致

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return userData;
    }

    if (!query.next()) {
        qWarning() << "[用户操作错误] 未找到用户账号：" << account;
        return userData;
    }

    userData.userId = query.value("user_id").toInt();
    userData.userType = static_cast<UserType>(query.value("user_type").toInt());
    userData.account = query.value("user_account").toString();
    userData.password = query.value("user_password").toString();
    userData.contact = query.value("user_contact").toString();
    userData.expertSkill = query.value("expert_skill").toString();

    qDebug() << "DB found account" << userData.toJson();
    return userData;
}

QVector<Agenda> DatabaseManager::getAgenda(const int userId, const UserType userType) {
    QVector<Agenda> agendas;

    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return agendas;
    }

    QSqlQuery query(db);
    if(userType == FactoryUser) {
        query.prepare("SELECT order_id, user1_id, user2_id, device_id,"
                      "order_status, order_create_time, predict_time, fault FROM workorder WHERE user1_id = :userId");
    } else {
        query.prepare("SELECT order_id, user1_id, user2_id, device_id,"
                      "order_status, order_create_time, predict_time, fault FROM workorder WHERE user2_id = :userId");
    }
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return agendas;
    }
    while (query.next()) {
        Agenda agenda;
        agenda.orderId = query.value("order_id").toInt();
        agenda.user1Id = query.value("user1_id").toInt();
        agenda.user2Id = query.value("user2_id").toInt();
        agenda.deviceId = query.value("device_id").toInt();
        agenda.status = query.value("order_status").toInt();
        agenda.createTime = query.value("order_create_time").toDateTime();
        agenda.predictTime = query.value("predict_time").toDateTime();
        agenda.fault = query.value("fault").toString();
        agendas.append(agenda);
        qInfo() << "GetAgenda: " << agenda.orderId;
    }

    return agendas;
}

QVector<User> DatabaseManager::getContact(const int userId) {
    QVector<User> contacts;

    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return contacts;
    }

    QSqlQuery query(db);
    query.prepare("SELECT u.user_id, u.user_type, u.user_account, u.user_password, u.user_contact, u.expert_skill"
                    " FROM user AS u"
                    " WHERE u.user_id IN ("
                        "SELECT c.user2_id FROM contact AS c WHERE c.user1_id = :userId);");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return contacts;
    }
    while (query.next()) {
        User contact;
        contact.userId = query.value("user_id").toInt();
        contact.userType = static_cast<UserType>(query.value("user_type").toInt());
        contact.account = query.value("user_account").toString();
        contact.password = query.value("user_password").toString();
        contact.contact = query.value("user_contact").toString();
        contact.expertSkill = query.value("expert_skill").toString();
        contacts.append(contact);
        qInfo() << "GetContact: " << contact.userId;
    }

    return contacts;
}

QVector<Device> DatabaseManager::getDevice(const int userId) {
    QVector<Device> devices;

    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return devices;
    }

    QSqlQuery query(db);
    query.prepare("SELECT device_id, device_name, device_type, status FROM device"
                  " WHERE user_id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 获取失败：" << query.lastError().text();
        return devices;
    }
    while (query.next()) {
        Device device;
        device.deviceId = query.value("device_id").toInt();
        device.deviceName = query.value("device_name").toString();
        device.deviceType = query.value("device_type").toString();
        device.status = query.value("status").toString();
        devices.append(device);
        qInfo() << "GetDevice: " << device.deviceId;
    }

    return devices;
}

bool DatabaseManager::addWorkOrder(int user1_id, const QString& expertname, const QString& device, const QString& ordername,
                                   const QString &order_create_time, const QString &predict_time, const QString& status, const QString &fault) {
    if (!isConnected()) return false;

    // 首先根据expertname查询user2_id
    QSqlQuery userQuery(db);
    userQuery.prepare("SELECT user_id FROM user WHERE user_account = :expertname");
    userQuery.bindValue(":expertname", expertname);

    if (!userQuery.exec() || !userQuery.next()) {
        qCritical() << "[数据库错误] 查询专家用户ID失败:" << userQuery.lastError().text();
        return false;
    }

    int user2_id = userQuery.value("user_id").toInt();

    // 然后根据device查询device_id
    QSqlQuery deviceQuery(db);
    deviceQuery.prepare("SELECT device_id FROM device WHERE device_name = :device");
    deviceQuery.bindValue(":device", device);

    if (!deviceQuery.exec() || !deviceQuery.next()) {
        qCritical() << "[数据库错误] 查询设备ID失败:" << deviceQuery.lastError().text();
        return false;
    }

    int device_id = deviceQuery.value("device_id").toInt();

    int statusNum;
    if(status == "待处理") {
        statusNum = 0;
    } else if(status == "处理中") {
        statusNum = 1;
    } else {
        statusNum = -1;
    }
    qDebug() << statusNum;
    // 最后插入工单记录
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO workorder (user1_id, user2_id, device_id, order_status,
                               order_create_time, predict_time, fault)
        VALUES (:u1, :u2, :d, :st, :ct, :pt, :fault)
    )");
    query.bindValue(":u1", user1_id);
    query.bindValue(":u2", user2_id);
    query.bindValue(":d", device_id);
    // query.bindValue(":oname", ordername);  // 添加了ordername参数
    query.bindValue(":st", statusNum);
    query.bindValue(":ct", QDateTime::fromString(order_create_time + " 12:00:00", "yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":pt", QDateTime::fromString(predict_time + " 12:00:00", "yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":fault", fault);

    if (!query.exec()) {
        qCritical() << "[数据库错误] 插入工单失败:" << query.lastError().text();
        return false;
    }

    qInfo() << "工单添加成功: 用户1 ID=" << user1_id << ", 用户2 ID=" << user2_id
            << ", 设备 ID=" << device_id << ", 订单名称=" << ordername;
    return true;
}

bool DatabaseManager::addDevice(const QString &name, const QString &type, const QString& status, int userId) {
    if (!isConnected()) return false;

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO device (user_id, device_name, device_type, status)
        VALUES (:id, :name, :type, :status)
    )");
    query.bindValue(":id", userId);
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":status", status);

    if (!query.exec()) {
        qCritical() << "[数据库错误] addDevice:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::removeDevice(const QString &deviceName, int userId) {
    if (!isConnected()) return false;

    QSqlQuery query(db);
    query.prepare(R"(
        DELETE FROM device
        WHERE device_name = :name AND user_id = :userId
    )");
    query.bindValue(":name", deviceName);
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qCritical() << "[数据库错误] removeDevice:" << query.lastError().text();
        return false;
    }

    // 检查是否实际删除了记录
    return query.numRowsAffected() > 0;
}

QSqlDatabase DatabaseManager::getDatabase() const
{
    return db;
}

QSqlQuery DatabaseManager::createQuery() const
{
    return QSqlQuery(db);
}

bool DatabaseManager::executeQuery(const QString &queryStr, QSqlQuery &query)
{
    if (!isConnected()) {
        return false;
    }
    return query.exec(queryStr);
}

QSqlQuery DatabaseManager::getHistoryOrderDetails(int orderId)
{
    QSqlQuery query(db);
    query.prepare("SELECT ho.*, d.device_name, d.device_type, wo.fault, "
                  "u1.user_contact as contact_info, u2.user_contact as expert_info "
                  "FROM history_order ho "
                  "LEFT JOIN device d ON ho.device_id = d.device_id "
                  "LEFT JOIN workorder wo ON ho.device_id = wo.device_id "
                  "LEFT JOIN user u1 ON ho.user1_id = u1.user_id "
                  "LEFT JOIN user u2 ON ho.user2_id = u2.user_id "
                  "WHERE ho.history_order_id = :id");
    query.bindValue(":id", orderId);
    query.exec();
    return query;
}

bool DatabaseManager::addChatMessage(int user1Id, int user2Id, const QString& message) {
    if (!isConnected()) return false;

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO message (user1_id, user2_id, message)
        VALUES (:user1_id, :user2_id, :message)
    )");
    query.bindValue(":user1_id", user1Id);
    query.bindValue(":user2_id", user2Id);
    query.bindValue(":message", message);

    if (!query.exec()) {
        qCritical() << "[数据库错误] addChatMessage:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<ChatMessage> DatabaseManager::getChatHistory(int user1Id, int user2Id, int limit) {
    QVector<ChatMessage> history;
    if (!isConnected()) return history;

    QSqlQuery query(db);
    query.prepare("SELECT user1_id, user2_id, message, time"
        " FROM message"
        " WHERE (user1_id = :user1_id AND user2_id = :user2_id) "
           "OR (user1_id = :user2_id AND user2_id = :user1_id)"
        " ORDER BY time DESC"
        " LIMIT :limit"
    );

    query.bindValue(":user1_id", user1Id);
    query.bindValue(":user2_id", user2Id);
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        qCritical() << "[数据库错误] getChatHistory:" << query.lastError().text();
        return history;
    }

    while (query.next()) {
        int user1IdFromDb = query.value("user1_id").toInt();
        int user2IdFromDb = query.value("user2_id").toInt();
        QString message = query.value("message").toString();
        QString time = query.value("time").toString();
        bool isOwn = (user1IdFromDb == user1Id); // 正确比较

        ChatMessage msg(
            user1IdFromDb,
            user2IdFromDb,
            message,
            isOwn,
            time
        );
        history.append(msg);
        qDebug() << "Db get history" << msg.toJson();
    }

    // 按时间正序排列（最早的在前）
    std::reverse(history.begin(), history.end());
    return history;
}

bool DatabaseManager::addSimulateData(int deviceId, double temp, double pressure, const QString& otherData)
{
    if (!isConnected()) {
        qCritical() << "[数据库错误] addSimulateData: 数据库未连接";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO device_data (info_id, data_time, device_id, temp, pressure, other_data)
        VALUES (:info_id, :time, :device_id, :temp, :pressure, :other_data)
    )");
    query.bindValue(":info_id", QDateTime::currentMSecsSinceEpoch()); // 用时间戳当唯一ID
    query.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":device_id", deviceId);
    query.bindValue(":temp", temp);
    query.bindValue(":pressure", pressure);
    query.bindValue(":other_data", otherData);

    if (!query.exec()) {
        qCritical() << "[数据库错误] addSimulateData:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ 模拟数据写入成功 → 设备ID:" << deviceId
             << "温度:" << temp
             << "压力:" << pressure
             << "状态:" << otherData;

    return true;
}

QList<User> DatabaseManager::getNonContactUsers(int userId) {
    QList<User> users;
    if (!isConnected()) return users;

    // 查询不是当前用户联系人且不是自己的用户
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT user_id, user_account, user_type, user_contact, expert_skill
        FROM user
        WHERE user_id != :userId AND user_id != 0
        AND user_id NOT IN (
            SELECT user2_id FROM contact WHERE user1_id = :userId
        )
    )");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qCritical() << "[数据库错误] getNonContactUsers:" << query.lastError().text();
        return users;
    }

    while (query.next()) {
        User user;
        user.userId = query.value("user_id").toInt();
        user.account = query.value("user_account").toString();
        user.contact = query.value("user_contact").toString();
        user.userType = static_cast<UserType>(query.value("user_type").toInt());
        user.expertSkill = query.value("expert_skill").toString();
        qDebug() << "Get Non Contact" << user.toJson();
        users.append(user);
    }

    return users;
}

bool DatabaseManager::addContact(int user1Id, int user2Id) {
    if (!isConnected()) return false;

    // 检查是否已经是联系人
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT * FROM contact WHERE user1_id = :user1Id AND user2_id = :user2Id");
    checkQuery.bindValue(":user1Id", user1Id);
    checkQuery.bindValue(":user2Id", user2Id);

    if (checkQuery.exec() && checkQuery.next()) {
        qWarning() << "Contact already exists between" << user1Id << "and" << user2Id;
        return false;
    }

    // 插入新的联系人关系
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO contact (user1_id, user2_id, contact_status, create_time)
        VALUES (:user1Id, :user2Id, 0, CURRENT_TIMESTAMP)
    )");
    query.bindValue(":user1Id", user1Id);
    query.bindValue(":user2Id", user2Id);

    if (!query.exec()) {
        qCritical() << "[数据库错误] addContact:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<Device> DatabaseManager::getAllDevices() {
    QVector<Device> devices;
    if (!isConnected()) {
        qCritical() << "Database not connected";
        return devices;
    }

    QSqlQuery query(db);
    query.prepare("SELECT device_name, device_type, status FROM device");

    if (!query.exec()) {
        qCritical() << "[数据库错误] getAllDevices:" << query.lastError().text();
        return devices;
    }

    while (query.next()) {
        Device device;
        device.deviceId = query.value("id").toInt();
        device.deviceName = query.value("device_name").toString();
        device.deviceName = query.value("device_name").toString();
        device.deviceType = query.value("device_type").toString();
        device.status = query.value("status").toString();
        devices.append(device);
    }

    qDebug() << "Found" << devices.size() << "devices in database";
    return devices;
}

bool DatabaseManager::updateDevice(const QString& deviceName, const QString& type,
                                   const QString& status, int userId) {
    if (!isConnected()) return false;

    QSqlQuery query(db);
    query.prepare(R"(
        UPDATE device
        SET device_type = :type, status = :status
        WHERE device_name = :device_name AND user_id = :user_id
    )");
    query.bindValue(":type", type);
    query.bindValue(":status", status);
    query.bindValue(":device_name", deviceName);
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qCritical() << "[数据库错误] updateDevice:" << query.lastError().text();
        return false;
    }

    // 检查是否确实更新了记录
    if (query.numRowsAffected() == 0) {
        qWarning() << "[数据库警告] updateDevice: 没有找到匹配的设备记录";
        return false;
    }

    return true;
}
