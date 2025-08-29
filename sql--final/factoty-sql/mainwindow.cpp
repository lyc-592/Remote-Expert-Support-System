#include "mainwindow.h"

// 初始化数据库（创建连接+建表）
bool DatabaseManager::initDatabase(const QString& dbName) {
    // 移除已存在的连接
    if (QSqlDatabase::contains("factory_connection")) {
        m_db = QSqlDatabase::database("factory_connection");
    } else {
        // 加载SQLite驱动
        m_db = QSqlDatabase::addDatabase("QSQLITE", "factory_connection");
        m_db.setDatabaseName(dbName);
    }

    // 打开数据库
    if (!m_db.open()) {
        qCritical() << "[数据库错误] 打开失败：" << m_db.lastError().text();
        return false;
    }

    // 创建所有表结构
    if (!createAllTables()) {
        qCritical() << "[数据库错误] 表创建失败";
        closeDatabase();
        return false;
    }

    qInfo() << "[数据库成功] 初始化完成，连接已建立";
    return true;
}

// 关闭数据库
void DatabaseManager::closeDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
        qInfo() << "[数据库成功] 连接已关闭";
    }
    // 移除连接
    QSqlDatabase::removeDatabase("factory_connection");
}

// 检查数据库连接状态
bool DatabaseManager::isConnected() const {
    return m_db.isOpen();
}

// 创建所有表结构（按要求设计）
bool DatabaseManager::createAllTables() {
    QSqlQuery query(m_db);
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

// ------------------------------ 用户表操作 ------------------------------
// 添加用户
bool DatabaseManager::addUser(int userId, UserType userType, const QString& account,
                              const QString& password, const QString& contact,
                              const QString& expertSkill) {
    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO user (user_id, user_type, user_account, user_password, user_contact, expert_skill)
        VALUES (:userId, :userType, :account, :password, :contact, :expertSkill)
    )");
    query.bindValue(":userId", userId);
    query.bindValue(":userType", userType);
    query.bindValue(":account", account);
    query.bindValue(":password", password);
    query.bindValue(":contact", contact);
    query.bindValue(":expertSkill", expertSkill);

    if (!query.exec()) {
        qWarning() << "[用户操作错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[用户操作成功] 添加用户：ID=" << userId << "，账号=" << account;
    return true;
}

// 获取用户
bool DatabaseManager::getUser(int userId, UserType& userType, QString& account,
                              QString& password, QString& contact, QString& expertSkill) {
    if (!isConnected()) {
        qWarning() << "[用户操作错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
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

// ------------------------------ 设备表操作 ------------------------------
// 添加设备
bool DatabaseManager::addDevice(int deviceId, int userId, const QString& deviceName,
                                const QString& deviceType) {
    if (!isConnected()) {
        qWarning() << "[设备操作错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO device (device_id, user_id, device_name, device_type)
        VALUES (:deviceId, :userId, :deviceName, :deviceType)
    )");
    query.bindValue(":deviceId", deviceId);
    query.bindValue(":userId", userId);
    query.bindValue(":deviceName", deviceName);
    query.bindValue(":deviceType", deviceType);

    if (!query.exec()) {
        qWarning() << "[设备操作错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[设备操作成功] 添加设备：ID=" << deviceId;
    return true;
}

// 获取设备
bool DatabaseManager::getDevice(int deviceId, int& userId, QString& deviceName,
                                QString& deviceType) {
    if (!isConnected()) {
        qWarning() << "[设备操作错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM device WHERE device_id = :deviceId");
    query.bindValue(":deviceId", deviceId);

    if (!query.exec() || !query.next()) {
        qWarning() << "[设备操作错误] 获取失败：" << query.lastError().text();
        return false;
    }

    userId = query.value("user_id").toInt();
    deviceName = query.value("device_name").toString();
    deviceType = query.value("device_type").toString();

    return true;
}

// ------------------------------ 设备信息表操作 ------------------------------
// 添加设备数据
bool DatabaseManager::addDeviceData(int infoId, const QDateTime& dataTime, int deviceId,
                                    double temp, double pressure, const QVariantMap& otherData) {
    if (!isConnected()) {
        qWarning() << "[设备数据错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO device_data (info_id, data_time, device_id, temp, pressure, other_data)
        VALUES (:infoId, :dataTime, :deviceId, :temp, :pressure, :otherData)
    )");
    query.bindValue(":infoId", infoId);
    query.bindValue(":dataTime", dataTime);
    query.bindValue(":deviceId", deviceId);
    query.bindValue(":temp", temp);
    query.bindValue(":pressure", pressure);

    // 处理其他数据
    QString otherDataStr;
    for (auto it = otherData.begin(); it != otherData.end(); ++it) {
        if (!otherDataStr.isEmpty()) otherDataStr += ";";
        otherDataStr += it.key() + "=" + it.value().toString();
    }
    query.bindValue(":otherData", otherDataStr);

    if (!query.exec()) {
        qWarning() << "[设备数据错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[设备数据成功] 添加数据：info_id=" << infoId;
    return true;
}

// 获取设备数据
bool DatabaseManager::getDeviceData(int infoId, const QDateTime& dataTime, int& deviceId,
                                    double& temp, double& pressure, QVariantMap& otherData) {
    if (!isConnected()) {
        qWarning() << "[设备数据错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM device_data WHERE info_id = :infoId AND data_time = :dataTime");
    query.bindValue(":infoId", infoId);
    query.bindValue(":dataTime", dataTime);

    if (!query.exec() || !query.next()) {
        qWarning() << "[设备数据错误] 获取失败：" << query.lastError().text();
        return false;
    }

    deviceId = query.value("device_id").toInt();
    temp = query.value("temp").toDouble();
    pressure = query.value("pressure").toDouble();

    // 解析其他数据
    QString otherDataStr = query.value("other_data").toString();
    QStringList pairs = otherDataStr.split(";");
    for (const QString& pair : pairs) {
        if (pair.isEmpty()) continue;
        QStringList keyValue = pair.split("=");
        if (keyValue.size() == 2) {
            otherData[keyValue[0]] = keyValue[1];
        }
    }

    return true;
}

// ------------------------------ 联系人表操作 ------------------------------
// 添加联系人
bool DatabaseManager::addContact(int user1Id, int user2Id, ContactStatus status,
                                 const QDateTime& createTime) {
    if (!isConnected()) {
        qWarning() << "[联系人错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO contact (user1_id, user2_id, contact_status, create_time)
        VALUES (:user1Id, :user2Id, :status, :createTime)
    )");
    query.bindValue(":user1Id", user1Id);
    query.bindValue(":user2Id", user2Id);
    query.bindValue(":status", status);
    query.bindValue(":createTime", createTime);

    if (!query.exec()) {
        qWarning() << "[联系人错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[联系人成功] 添加联系：" << user1Id << "->" << user2Id;
    return true;
}

// 获取联系人
bool DatabaseManager::getContact(int user1Id, int user2Id, ContactStatus& status,
                                 QDateTime& createTime) {
    if (!isConnected()) {
        qWarning() << "[联系人错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM contact WHERE user1_id = :user1Id AND user2_id = :user2Id");
    query.bindValue(":user1Id", user1Id);
    query.bindValue(":user2Id", user2Id);

    if (!query.exec() || !query.next()) {
        qWarning() << "[联系人错误] 获取失败：" << query.lastError().text();
        return false;
    }

    status = static_cast<ContactStatus>(query.value("contact_status").toInt());
    createTime = query.value("create_time").toDateTime();

    return true;
}

// ------------------------------ 工单记录表操作 ------------------------------
// 添加工单
bool DatabaseManager::addWorkOrder(int orderId, int user1Id, int user2Id, int deviceId,
                                   OrderStatus status, const QDateTime& createTime,
                                   const QDateTime& predictTime, const QString& fault) {
    if (!isConnected()) {
        qWarning() << "[工单错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO workorder (order_id, user1_id, user2_id, device_id, order_status,
        order_create_time, predict_time, fault)
        VALUES (:orderId, :user1Id, :user2Id, :deviceId, :status, :createTime, :predictTime, :fault)
    )");
    query.bindValue(":orderId", orderId);
    query.bindValue(":user1Id", user1Id);
    query.bindValue(":user2Id", user2Id);
    query.bindValue(":deviceId", deviceId);
    query.bindValue(":status", status);
    query.bindValue(":createTime", createTime);
    query.bindValue(":predictTime", predictTime);
    query.bindValue(":fault", fault);

    if (!query.exec()) {
        qWarning() << "[工单错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[工单成功] 添加工单：ID=" << orderId;
    return true;
}

// 获取工单
bool DatabaseManager::getWorkOrder(int orderId, int& user1Id, int& user2Id, int& deviceId,
                                   OrderStatus& status, QDateTime& createTime,
                                   QDateTime& predictTime, QString& fault) {
    if (!isConnected()) {
        qWarning() << "[工单错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM workorder WHERE order_id = :orderId");
    query.bindValue(":orderId", orderId);

    if (!query.exec() || !query.next()) {
        qWarning() << "[工单错误] 获取失败：" << query.lastError().text();
        return false;
    }

    user1Id = query.value("user1_id").toInt();
    user2Id = query.value("user2_id").toInt();
    deviceId = query.value("device_id").toInt();
    status = static_cast<OrderStatus>(query.value("order_status").toInt());
    createTime = query.value("order_create_time").toDateTime();
    predictTime = query.value("predict_time").toDateTime();
    fault = query.value("fault").toString();

    return true;
}

// ------------------------------ 历史工单表操作 ------------------------------
// 添加历史工单
bool DatabaseManager::addHistoryOrder(int historyOrderId, int user1Id, int user2Id, int deviceId,
                                      OrderStatus status, const QDateTime& createTime,
                                      const QDateTime& endTime) {
    if (!isConnected()) {
        qWarning() << "[历史工单错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO history_order (history_order_id, user1_id, user2_id, device_id, order_status,
        order_create_time, order_end_time)
        VALUES (:historyOrderId, :user1Id, :user2Id, :deviceId, :status, :createTime, :endTime)
    )");
    query.bindValue(":historyOrderId", historyOrderId);
    query.bindValue(":user1Id", user1Id);
    query.bindValue(":user2Id", user2Id);
    query.bindValue(":deviceId", deviceId);
    query.bindValue(":status", status);
    query.bindValue(":createTime", createTime);
    query.bindValue(":endTime", endTime);

    if (!query.exec()) {
        qWarning() << "[历史工单错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[历史工单成功] 添加历史工单：ID=" << historyOrderId;
    return true;
}

// 获取历史工单
bool DatabaseManager::getHistoryOrder(int historyOrderId, int& user1Id, int& user2Id, int& deviceId,
                                      OrderStatus& status, QDateTime& createTime,
                                      QDateTime& endTime) {
    if (!isConnected()) {
        qWarning() << "[历史工单错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM history_order WHERE history_order_id = :historyOrderId");
    query.bindValue(":historyOrderId", historyOrderId);

    if (!query.exec() || !query.next()) {
        qWarning() << "[历史工单错误] 获取失败：" << query.lastError().text();
        return false;
    }

    user1Id = query.value("user1_id").toInt();
    user2Id = query.value("user2_id").toInt();
    deviceId = query.value("device_id").toInt();
    status = static_cast<OrderStatus>(query.value("order_status").toInt());
    createTime = query.value("order_create_time").toDateTime();
    endTime = query.value("order_end_time").toDateTime();

    return true;
}

// ------------------------------ 现场记录表操作 ------------------------------
// 添加现场记录
bool DatabaseManager::addRecord(int recordId, int orderId, const QString& videoPath,
                                const QString& filePath, const QString& deviceDataPath,
                                const QString& faultType, const QString& solution,
                                const QDateTime& uploadTime) {
    if (!isConnected()) {
        qWarning() << "[记录错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO record (record_id, order_id, video_path, file_path, device_status_data_path,
        fault_type, solution, upload_time)
        VALUES (:recordId, :orderId, :videoPath, :filePath, :deviceDataPath,
        :faultType, :solution, :uploadTime)
    )");
    query.bindValue(":recordId", recordId);
    query.bindValue(":orderId", orderId);
    query.bindValue(":videoPath", videoPath);
    query.bindValue(":filePath", filePath);
    query.bindValue(":deviceDataPath", deviceDataPath);
    query.bindValue(":faultType", faultType);
    query.bindValue(":solution", solution);
    query.bindValue(":uploadTime", uploadTime);

    if (!query.exec()) {
        qWarning() << "[记录错误] 添加失败：" << query.lastError().text();
        return false;
    }
    qInfo() << "[记录成功] 添加记录：ID=" << recordId;
    return true;
}

// 获取现场记录
bool DatabaseManager::getRecord(int recordId, int& orderId, QString& videoPath,
                                QString& filePath, QString& deviceDataPath,
                                QString& faultType, QString& solution, QDateTime& uploadTime) {
    if (!isConnected()) {
        qWarning() << "[记录错误] 数据库未连接";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM record WHERE record_id = :recordId");
    query.bindValue(":recordId", recordId);

    if (!query.exec() || !query.next()) {
        qWarning() << "[记录错误] 获取失败：" << query.lastError().text();
        return false;
    }

    orderId = query.value("order_id").toInt();
    videoPath = query.value("video_path").toString();
    filePath = query.value("file_path").toString();
    deviceDataPath = query.value("device_status_data_path").toString();
    faultType = query.value("fault_type").toString();
    solution = query.value("solution").toString();
    uploadTime = query.value("upload_time").toDateTime();

    return true;
}
