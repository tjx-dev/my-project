#ifndef READING_STORE_H
#define READING_STORE_H

#include <QDateTime>
#include <QSqlDatabase>
#include <QString>
#include <QVector>

class ReadingStore
{
public:
    // 对应查询结果中的一行温度记录。
    struct Reading {
        qulonglong id;
        QString deviceId;
        int temperature;
        QDateTime receivedAt;
    };

    // 最近十条明细和整个筛选范围的统计一起返回。
    struct HistoryResult {
        QVector<Reading> readings;
        qulonglong totalCount = 0;
        int minimumTemperature = 0;
        int maximumTemperature = 0;
        double averageTemperature = 0;
    };

    ReadingStore();
    ~ReadingStore();
    ReadingStore(const ReadingStore &) = delete;
    ReadingStore &operator=(const ReadingStore &) = delete;

    bool open(int port, const QString &userName, const QString &password, QString &error);
    bool isOpen() const;
    bool insertReading(const QString &deviceId, int temperature, QString &error);
    bool selectRecentReadings(const QString &deviceId, const QDateTime &startTime, const QDateTime &endTime,
                              HistoryResult &result, QString &error);
    void close();

private:
    QString connectionName_;
    QSqlDatabase database_;
};

#endif
