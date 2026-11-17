#include "reading_store.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>
#include <QVariant>

ReadingStore::ReadingStore()
    : connectionName_(QUuid::createUuid().toString())
{
}

ReadingStore::~ReadingStore()
{
    // 先释放连接对象，再从 Qt 的连接列表中移除它。
    close();
    database_ = QSqlDatabase();
    if (QSqlDatabase::contains(connectionName_)) {
        QSqlDatabase::removeDatabase(connectionName_);
    }
}


//数据库与外界连接；以下为示例
bool ReadingStore::open(int port, const QString &userName, const QString &password, QString &error)
{
    if (!database_.isValid()) {//isValid检查的是数据库对象是否有有效驱动
        database_ = QSqlDatabase::addDatabase("QMYSQL", connectionName_);
        if (!database_.isValid()) {
            error = "Qt 无法加载 QMYSQL 驱动，请检查当前 Qt Kit 的 MySQL 驱动及其依赖。";
            return false;
        }
    }

    // 连接本机的项目数据库。密码只由用户在窗口输入。
    database_.setHostName("127.0.0.1");
    database_.setPort(port);
    database_.setDatabaseName("device_monitor");
    database_.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3;MYSQL_OPT_READ_TIMEOUT=3;MYSQL_OPT_WRITE_TIMEOUT=3");
    if (!database_.open(userName, password)) {
        error = database_.lastError().text();
        return false;
    }
    return true;
}

bool ReadingStore::isOpen() const
{
    return database_.isOpen();
}

//数据库里数据插入
bool ReadingStore::insertReading(const QString &deviceId, int temperature, QString &error)
{
    QSqlQuery query(database_);
    // 先写 SQL 模板，再把编号和温度分别填入占位符。
    if (!query.prepare("INSERT INTO temperature_readings (device_id, temperature) "
                       "VALUES (:device_id, :temperature)")) {
        error = query.lastError().text();
        return false;
    }
    query.bindValue(":device_id", deviceId);
    query.bindValue(":temperature", temperature);

    // MySQL 会为新记录自动填入接收时间。
    if (!query.exec()) {
        error = query.lastError().text();
        return false;
    }
    return true;
}


//数据库里面寻找开示时间和结束时间
bool ReadingStore::selectRecentReadings(const QString &deviceId, const QDateTime &startTime, const QDateTime &endTime,
                                       HistoryResult &result, QString &error)
{
    result = HistoryResult();
    QSqlQuery query(database_);
    // OVER () 统计全部符合 WHERE 的记录，LIMIT 只限制返回的明细。
    QString sql = "SELECT id, device_id, temperature, received_at, "
                  "COUNT(*) OVER (), MIN(temperature) OVER (), "
                  "MAX(temperature) OVER (), AVG(temperature) OVER () "
                  "FROM temperature_readings WHERE device_id = :device_id";
    const bool filterByTime = startTime.isValid() && endTime.isValid();
    if (filterByTime) {
        // 包含开始时刻，结束时刻留给下一个时间段。
        sql += " AND UNIX_TIMESTAMP(received_at) >= :start_time"
               " AND UNIX_TIMESTAMP(received_at) < :end_time";
    }
    sql += " ORDER BY id DESC LIMIT 10";
    if (!query.prepare(sql)) {
        error = query.lastError().text();
        return false;
    }
    query.bindValue(":device_id", deviceId);
    if (filterByTime) {
        // 界面时间和入库时间都换成时间戳，再比较范围。
        query.bindValue(":start_time", startTime.toSecsSinceEpoch());
        query.bindValue(":end_time", endTime.toSecsSinceEpoch());
    }
    if (!query.exec()) {
        error = query.lastError().text();
        return false;
    }

    // next() 每次走到一条结果，value() 按 SELECT 的列顺序取值。
    while (query.next()) {
        // 每行附带同一组统计值，从第一行取一次即可。
        if (result.readings.isEmpty()) {
            result.totalCount = query.value(4).toULongLong();
            result.minimumTemperature = query.value(5).toInt();
            result.maximumTemperature = query.value(6).toInt();
            result.averageTemperature = query.value(7).toDouble();
        }
        result.readings.push_back({query.value(0).toULongLong(), query.value(1).toString(),
                                   query.value(2).toInt(), query.value(3).toDateTime()});
    }
    if (query.lastError().isValid()) {
        error = query.lastError().text();
        result = HistoryResult();
        return false;
    }
    return true;
}

void ReadingStore::close()
{
    if (database_.isOpen()) {
        database_.close();
    }
}
