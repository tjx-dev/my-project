#include "mainwindow.h"
#include "tcp_receiver.h"

#include <QApplication>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSignalSpy>
#include <QSpinBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <QTimer>
#include <QtTest>

#ifdef Q_OS_WIN
#include "widget.h"
#include <QPlainTextEdit>
#endif

// 检查真正的 TCP 收发和窗口更新，防止半条数据被当成完整温度。
class TcpReceiverTest : public QObject
{
    Q_OBJECT

private slots:
    void fragmentedAndCombinedMessages();
    void simultaneousClientsKeepSeparateBuffers();
    void reconnectClearsIncompleteData();
    void oversizedClientDoesNotBlockOtherClients();
    void unregisteredDeviceHasClearStatus();
    void periodicConnectionFailureCanRetry();
    void windowReceivesFromWinsockSimulator();
    void periodicSimulatorReusesConnection();
    void savesAndQueriesTcpReadingsInMySql();
};

void TcpReceiverTest::fragmentedAndCombinedMessages()
{
    TcpReceiver receiver;
    QSignalSpy readings(&receiver, &TcpReceiver::temperatureReceived);
    QSignalSpy statuses(&receiver, &TcpReceiver::statusChanged);
    QVERIFY(receiver.start(0)); // 让系统选择空闲端口，避免测试互相占用。

    QTcpSocket device;
    device.connectToHost(QHostAddress::LocalHost, receiver.port());
    QVERIFY(device.waitForConnected(1000));
    QTRY_VERIFY(statuses.count() >= 2);

    // 先只送半条数据；没有换行就不应产生温度。
    device.write("B,9");
    device.flush();
    QTest::qWait(50);
    QCOMPARE(readings.count(), 0);

    // 补完 B 的记录，再送 A 的记录和无效数据。
    device.write("0\nA,60\nB,invalid\nA,151\n,84\n");
    device.flush();
    QTRY_COMPARE(readings.count(), 2);
    QCOMPARE(readings.at(0).at(0).toString(), QString("B"));
    QCOMPARE(readings.at(0).at(1).toInt(), 90);
    QCOMPARE(readings.at(1).at(0).toString(), QString("A"));
    QCOMPARE(readings.at(1).at(1).toInt(), 60);
}

void TcpReceiverTest::simultaneousClientsKeepSeparateBuffers()
{
    TcpReceiver receiver;
    QSignalSpy readings(&receiver, &TcpReceiver::temperatureReceived);
    QSignalSpy statuses(&receiver, &TcpReceiver::statusChanged);
    QVERIFY(receiver.start(0));

    QTcpSocket clientA;
    clientA.connectToHost(QHostAddress::LocalHost, receiver.port());
    QVERIFY(clientA.waitForConnected(1000));
    QTRY_VERIFY(statuses.count() >= 2);
    auto *acceptedA = receiver.findChild<QTcpSocket *>();
    QVERIFY(acceptedA);
    QSignalSpy arrival(acceptedA, &QTcpSocket::readyRead);

    // 等 A 的半行到达后，再接入 B。
    clientA.write("A,8");
    clientA.flush();
    QTRY_VERIFY(!arrival.isEmpty());
    QCOMPARE(readings.count(), 0);

    QTcpSocket clientB;
    clientB.connectToHost(QHostAddress::LocalHost, receiver.port());
    QVERIFY(clientB.waitForConnected(1000));
    clientB.write("B,90\n");
    clientB.flush();
    QTRY_COMPARE(readings.count(), 1);
    QCOMPARE(readings.at(0).at(0).toString(), QString("B"));
    QCOMPARE(readings.at(0).at(1).toInt(), 90);

    // B 已完成一条记录，A 还保有之前的半行。
    clientA.write("4\n");
    clientA.flush();
    QTRY_COMPARE(readings.count(), 2);
    QCOMPARE(readings.at(1).at(0).toString(), QString("A"));
    QCOMPARE(readings.at(1).at(1).toInt(), 84);

    receiver.stop();
    QTRY_COMPARE(clientA.state(), QAbstractSocket::UnconnectedState);
    QTRY_COMPARE(clientB.state(), QAbstractSocket::UnconnectedState);
}

void TcpReceiverTest::reconnectClearsIncompleteData()
{
    TcpReceiver receiver;
    QSignalSpy readings(&receiver, &TcpReceiver::temperatureReceived);
    QSignalSpy statuses(&receiver, &TcpReceiver::statusChanged);
    QVERIFY(receiver.start(0));

    QTcpSocket first;
    first.connectToHost(QHostAddress::LocalHost, receiver.port());
    QVERIFY(first.waitForConnected(1000));
    QTRY_VERIFY(statuses.count() >= 2);
    first.write("9");
    first.flush();
    QTest::qWait(50);
    first.disconnectFromHost();
    QTRY_VERIFY(statuses.last().at(0).toString().contains("连接已断开"));

    // 新连接的 0 不应和旧连接的 9 拼成 90。
    QTcpSocket second;
    second.connectToHost(QHostAddress::LocalHost, receiver.port());
    QVERIFY(second.waitForConnected(1000));
    second.write("0\n");
    second.flush();
    QTRY_COMPARE(readings.count(), 1);
    QCOMPARE(readings.first().at(0).toString(), QString("A"));
    QCOMPARE(readings.first().at(1).toInt(), 0);

    receiver.stop();
    QTRY_COMPARE(second.state(), QAbstractSocket::UnconnectedState);
    QVERIFY(receiver.start(0)); // 停止后仍然可以重新启动。
}

void TcpReceiverTest::oversizedClientDoesNotBlockOtherClients()
{
    TcpReceiver receiver;
    QSignalSpy readings(&receiver, &TcpReceiver::temperatureReceived);
    QVERIFY(receiver.start(0));
    QTcpSocket oversized;
    QTcpSocket healthy;
    oversized.connectToHost(QHostAddress::LocalHost, receiver.port());
    healthy.connectToHost(QHostAddress::LocalHost, receiver.port());
    QVERIFY(oversized.waitForConnected(1000));
    QVERIFY(healthy.waitForConnected(1000));
    oversized.write(QByteArray(65, 'X'));
    oversized.flush();
    QTRY_COMPARE(oversized.state(), QAbstractSocket::UnconnectedState);
    healthy.write("B,90\n");
    healthy.flush();
    QTRY_COMPARE(readings.count(), 1);
    QCOMPARE(readings.first().at(0).toString(), QString("B"));
}

void TcpReceiverTest::unregisteredDeviceHasClearStatus()
{
    MainWindow window;
    auto *start = window.findChild<QPushButton *>("tcpButton");
    auto *status = window.findChild<QLabel *>("dataStatus");
    auto *total = window.findChild<QLabel *>("totalCount");
    auto *table = window.findChild<QTableWidget *>("deviceTable");
    QVERIFY(start && status && total && table);
    start->click();
    QTcpSocket device;
    device.connectToHost(QHostAddress::LocalHost, 9000);
    QVERIFY(device.waitForConnected(1000));
    device.write("Z,90\n");
    device.flush();
    QTRY_VERIFY(status->text().contains("未登记"));
    QCOMPARE(total->text(), QString("0"));

    // 错误输入不增加计数，合法的两个边界温度仍可接收。
    device.write("A,abc\nB,151\nC,-51\n,80\nA,1,2\n");
    device.flush();
    QTRY_VERIFY(status->text().contains("忽略无效数据"));
    QCOMPARE(total->text(), QString("0"));
    device.disconnectFromHost();
    QTRY_COMPARE(device.state(), QAbstractSocket::UnconnectedState);
    QVERIFY(status->text().contains("忽略无效数据"));
    device.connectToHost(QHostAddress::LocalHost, 9000);
    QVERIFY(device.waitForConnected(1000));
    device.write("A,-50\nB,150\n");
    device.flush();
    QTRY_COMPARE(total->text(), QString("2 次"));
    QCOMPARE(table->item(0, 1)->text(), QString("-50 °C"));
    QCOMPARE(table->item(1, 1)->text(), QString("150 °C"));
}

void TcpReceiverTest::periodicConnectionFailureCanRetry()
{
#ifdef Q_OS_WIN
    Widget simulator;
    auto *input = simulator.findChild<QPlainTextEdit *>("reportInput");
    auto *start = simulator.findChild<QPushButton *>("startPeriodicButton");
    auto *stop = simulator.findChild<QPushButton *>("stopPeriodicButton");
    auto *status = simulator.findChild<QLabel *>("statusLabel");
    QVERIFY(input && start && stop && status);
    input->setPlainText("A,84");
    start->click();
    QTRY_VERIFY(start->isEnabled());
    QVERIFY2(status->text().contains("失败"), qPrintable(status->text()));
    QVERIFY(!stop->isEnabled());

    // 监控端恢复后可重试，监控端停止时模拟器也恢复按钮。
    TcpReceiver receiver;
    QSignalSpy readings(&receiver, &TcpReceiver::temperatureReceived);
    QVERIFY(receiver.start());
    start->click();
    QTRY_COMPARE(readings.count(), 1);
    receiver.stop();
    QTRY_VERIFY(start->isEnabled());
    QVERIFY(!stop->isEnabled());
    QVERIFY(!input->isReadOnly());
#else
    QSKIP("Device simulator is built on Windows");
#endif
}

void TcpReceiverTest::windowReceivesFromWinsockSimulator()
{
#ifdef Q_OS_WIN
    MainWindow window;
    auto *table = window.findChild<QTableWidget *>();
    auto *status = window.findChild<QLabel *>("connectionStatus");
    auto *total = window.findChild<QLabel *>("totalCount");
    QPushButton *tcpButton = nullptr;
    QPushButton *stopButton = nullptr;
    QPushButton *localButton = nullptr;
    for (auto *button : window.findChildren<QPushButton *>()) {
        if (button->text() == "开始 TCP 接收") tcpButton = button;
        if (button->text() == "停止") stopButton = button;
        if (button->text() == "开始本地模拟") localButton = button;
    }
    QVERIFY(table && status && total && tcpButton && stopButton && localButton);
    tcpButton->click();
    QVERIFY2(stopButton->isEnabled(), qPrintable(status->text()));
    QVERIFY(!localButton->isEnabled());

    // 用实际的 Winsock 程序发送 90，检查窗口告警和计数。
    QProcess sender;
    const QString executable = QCoreApplication::applicationDirPath() + "/DeviceSimulator.exe";
    sender.start(executable, {"90"});
    QVERIFY(sender.waitForFinished(3000));
    QCOMPARE(sender.exitCode(), 0);
    QTRY_COMPARE(table->item(0, 1)->text(), QString("90 °C"));
    QVERIFY(table->item(0, 2)->text().contains("高温告警"));
    QCOMPARE(table->item(0, 4)->text(), QString("1 次"));
    QCOMPARE(total->text(), QString("1 次"));
    QTRY_VERIFY(status->text().contains("连接已断开"));

    // 一次点击发送多行，两个已登记的编号应分别更新对应行。
    Widget simulator;
    auto *reportInput = simulator.findChild<QPlainTextEdit *>("reportInput");
    auto *sendButton = simulator.findChild<QPushButton *>("sendButton");
    auto *sendStatus = simulator.findChild<QLabel *>("statusLabel");
    QVERIFY(reportInput && sendButton && sendStatus);
    reportInput->setPlainText("A,84\nZ,100\nB,90");
    sendButton->click();
    QTRY_VERIFY(sendButton->isEnabled());
    QVERIFY2(sendStatus->text().startsWith("已发送"), qPrintable(sendStatus->text()));
    QTRY_COMPARE(table->item(0, 1)->text(), QString("84 °C"));
    QTRY_COMPARE(table->item(1, 1)->text(), QString("90 °C"));
    QCOMPARE(table->item(0, 2)->text(), QString("正常运行"));
    QVERIFY(table->item(1, 2)->text().contains("高温告警"));
    QCOMPARE(table->item(0, 4)->text(), QString("2 次"));
    QCOMPARE(table->item(1, 4)->text(), QString("1 次"));
    QCOMPARE(total->text(), QString("3 次"));
    for (int row = 2; row < table->rowCount(); ++row) {
        QCOMPARE(table->item(row, 4)->text(), QString("0 次"));
    }

    // 停止 TCP 后，仍可切回本地模拟；每台设备增加一条记录。
    stopButton->click();
    QVERIFY(localButton->isEnabled());
    localButton->click();
    QCOMPARE(total->text(), QString::number(3 + table->rowCount()) + " 次");
    stopButton->click();
#else
    QSKIP("Winsock simulator runs on Windows");
#endif
}

void TcpReceiverTest::periodicSimulatorReusesConnection()
{
#ifdef Q_OS_WIN
    TcpReceiver receiver;
    QSignalSpy readings(&receiver, &TcpReceiver::temperatureReceived);
    QSignalSpy statuses(&receiver, &TcpReceiver::statusChanged);
    QVERIFY(receiver.start());

    Widget simulator;
    auto *input = simulator.findChild<QPlainTextEdit *>("reportInput");
    auto *start = simulator.findChild<QPushButton *>("startPeriodicButton");
    auto *stop = simulator.findChild<QPushButton *>("stopPeriodicButton");
    auto *socket = simulator.findChild<QTcpSocket *>();
    QVERIFY(input && start && stop && socket);
    QSignalSpy connections(socket, &QTcpSocket::connected);

    // 自动发送至少两次，整个过程只建立一条连接。
    input->setPlainText("B,90");
    start->click();
    QTRY_VERIFY(readings.count() >= 2);
    QCOMPARE(connections.count(), 1);
    QCOMPARE(readings.at(0).at(0).toString(), QString("B"));
    QCOMPARE(readings.at(1).at(1).toInt(), 90);

    // 点击停止后，接收端看到连接关闭，发送端可以重新开始。
    stop->click();
    QTRY_VERIFY(statuses.last().at(0).toString().contains("当前连接数：0"));
    QVERIFY(start->isEnabled());
    QVERIFY(!stop->isEnabled());
#else
    QSKIP("Device simulator target is built on Windows");
#endif
}

void TcpReceiverTest::savesAndQueriesTcpReadingsInMySql()
{
    // 只在明确提供临时 MySQL 实例时，才运行会写库的测试。
    const QString password = qEnvironmentVariable("MONITOR_TEST_MYSQL_PASSWORD");
    const QString userName = qEnvironmentVariable("MONITOR_TEST_MYSQL_USER", "monitor_app");
    if (password.isEmpty()) {
        QSKIP("No dedicated MySQL test instance configured");
    }
    bool ok = false;
    const int port = qEnvironmentVariableIntValue("MONITOR_TEST_MYSQL_PORT", &ok);
    QVERIFY(ok && port > 0 && port <= 65535 && port != 3306 && port != 3307);

    MainWindow window;
    auto *portInput = window.findChild<QSpinBox *>("databasePort");
    auto *passwordInput = window.findChild<QLineEdit *>("databasePassword");
    auto *userInput = window.findChild<QLineEdit *>("databaseUser");
    auto *connectButton = window.findChild<QPushButton *>("connectDatabaseButton");
    auto *status = window.findChild<QLabel *>("databaseStatus");
    auto *tcpButton = window.findChild<QPushButton *>("tcpButton");
    auto *deviceFilter = window.findChild<QComboBox *>("historyDevice");
    auto *queryButton = window.findChild<QPushButton *>("queryHistoryButton");
    auto *exportButton = window.findChild<QPushButton *>("exportHistoryButton");
    auto *history = window.findChild<QTableWidget *>("historyTable");
    auto *timeFilter = window.findChild<QGroupBox *>("historyTimeFilter");
    auto *startTimeEdit = window.findChild<QDateTimeEdit *>("historyStartTime");
    auto *endTimeEdit = window.findChild<QDateTimeEdit *>("historyEndTime");
    auto *historyStatus = window.findChild<QLabel *>("historyStatus");
    QVERIFY(portInput && userInput && passwordInput && connectButton && status && tcpButton);
    QVERIFY(deviceFilter && queryButton && exportButton && history);
    QVERIFY(timeFilter && startTimeEdit && endTimeEdit && historyStatus);
    QVERIFY(!queryButton->isEnabled());
    QVERIFY(!exportButton->isEnabled());
    QVERIFY(!timeFilter->isChecked());
    portInput->setValue(port);
    userInput->setText(userName);
    passwordInput->setText(password);
    connectButton->click();
    QVERIFY2(status->text().startsWith("MySQL 已连接"), qPrintable(status->text()));
    QVERIFY(queryButton->isEnabled());
    tcpButton->click();

    // 发送超过十条 E 记录，夹一条 B 记录来检查设备筛选。
    QTcpSocket simulator;
    simulator.connectToHost(QHostAddress::LocalHost, 9000);
    QVERIFY(simulator.waitForConnected(1000));
    QByteArray payload;
    for (int temperature = 60; temperature <= 70; ++temperature) {
        payload += "E," + QByteArray::number(temperature) + "\n";
    }
    payload += "B,82\nE,85\n";
    simulator.write(payload);
    simulator.flush();
    QTRY_COMPARE(status->text(), QString("MySQL 已保存设备 E 的温度 85 °C"));

    QString latestId;
    QString latestTime;
    qulonglong expectedDeviceCount = 0;
    const QDateTime rangeStart = QDateTime::fromSecsSinceEpoch(QDateTime::currentSecsSinceEpoch() - 86400).toUTC();
    const QDateTime rangeEnd = rangeStart.addSecs(1);
    const QString connectionName = "mysql-test-verification";
    {
        QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        database.setHostName("127.0.0.1");
        database.setPort(port);
        database.setDatabaseName("device_monitor");
        QVERIFY(database.open(userName, password));
        QSqlQuery query(database);
        QVERIFY(query.exec("SELECT device_id, temperature, received_at, id "
                           "FROM temperature_readings ORDER BY id DESC LIMIT 1"));
        QVERIFY(query.next());
        QCOMPARE(query.value(0).toString(), QString("E"));
        QCOMPARE(query.value(1).toInt(), 85);
        QVERIFY(!query.value(2).isNull());
        latestTime = query.value(2).toDateTime().toLocalTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
        latestId = query.value(3).toString();

        // 直接数出设备 E 的全部记录，与界面统计对照。
        QVERIFY(query.exec("SELECT COUNT(*) FROM temperature_readings WHERE device_id = 'E'"));
        QVERIFY(query.next());
        expectedDeviceCount = query.value(0).toULongLong();
        QVERIFY(expectedDeviceCount > 10);

        // 按 UTC 写入边界样本，稍后用窗口的本地时间查询。
        QVERIFY(query.exec("SET time_zone = '+00:00'"));
        QVERIFY(query.prepare("INSERT INTO temperature_readings (device_id, temperature, received_at) VALUES "
                              "('C', 61, :before_start), ('C', 62, :at_start), ('C', 63, :before_end), "
                              "('C', 64, :at_end), ('A', 65, :other_device)"));
        const QString format = "yyyy-MM-dd HH:mm:ss.zzz";
        query.bindValue(":before_start", rangeStart.addMSecs(-1).toString(format));
        query.bindValue(":at_start", rangeStart.toString(format));
        query.bindValue(":before_end", rangeEnd.addMSecs(-1).toString(format));
        query.bindValue(":at_end", rangeEnd.toString(format));
        query.bindValue(":other_device", rangeStart.toString(format));
        QVERIFY2(query.exec(), qPrintable(query.lastError().text()));
    }
    QSqlDatabase::removeDatabase(connectionName);

    // 从窗口发起查询，检查表格确实显示最新十条 E 记录。
    deviceFilter->setCurrentIndex(deviceFilter->findData(QString("E")));
    queryButton->click();
    QCOMPARE(history->rowCount(), 10);
    QVERIFY2(historyStatus->text().contains(QString("共 %1 条符合条件").arg(expectedDeviceCount)),
             qPrintable(historyStatus->text()));
    QCOMPARE(history->item(0, 0)->text(), latestId);
    QCOMPARE(history->item(0, 2)->text(), QString("85 °C"));
    QVERIFY(!latestTime.isEmpty());
    QCOMPARE(history->item(0, 3)->text(), latestTime);
    QCOMPARE(history->item(9, 2)->text(), QString("62 °C"));
    for (int row = 0; row < history->rowCount(); ++row) {
        QCOMPARE(history->item(row, 1)->text(), QString("E"));
    }

    // 走导出按钮和保存对话框，确认 CSV 只包含表格中的十条明细。
    QVERIFY(exportButton->isEnabled());
    QTemporaryDir exportDirectory;
    QVERIFY(exportDirectory.isValid());
    const QString csvPath = exportDirectory.filePath("temperature_readings.csv");
    const QString transientDirectory = exportDirectory.filePath("removed-before-write");
    QVERIFY(QDir().mkdir(transientDirectory));
    QString selectedCsvPath = csvPath;
    bool cancelSave = true;
    bool removeParentBeforeWrite = false;
    bool parentRemoved = false;
    QString exportMessageTitle;
    QTimer handleDialog;
    int saveAttempts = 0;
    connect(&handleDialog, &QTimer::timeout, &window, [&] {
        QWidget *modal = QApplication::activeModalWidget();
        if (auto *dialog = qobject_cast<QFileDialog *>(modal)) {
            if (cancelSave) {
                dialog->reject();
                return;
            }
            if (++saveAttempts > 10) {
                dialog->reject();
                return;
            }
            dialog->selectFile(selectedCsvPath);
            if (removeParentBeforeWrite) {
                // 模拟选完文件后目标目录不可用，触发真正的写入失败。
                connect(dialog, &QDialog::accepted, &window, [&] {
                    parentRemoved = QDir().rmdir(transientDirectory);
                });
            }
            static_cast<QDialog *>(dialog)->accept();
        } else if (auto *message = qobject_cast<QMessageBox *>(modal)) {
            exportMessageTitle = message->windowTitle();
            message->accept();
        }
    });
    handleDialog.start(100);
    exportButton->click(); // 取消保存后，仍然能再次导出。
    QVERIFY(!QFile::exists(csvPath));
    QVERIFY(exportButton->isEnabled());
    cancelSave = false;
    removeParentBeforeWrite = true;
    selectedCsvPath = transientDirectory + "/readings.csv";
    exportButton->click();
    QVERIFY(parentRemoved);
    QCOMPARE(exportMessageTitle, QString("导出失败"));
    QVERIFY(!QFile::exists(selectedCsvPath));
    QVERIFY(exportButton->isEnabled());
    removeParentBeforeWrite = false;
    saveAttempts = 0;
    selectedCsvPath = csvPath;
    exportButton->click();
    handleDialog.stop();
    QCOMPARE(exportMessageTitle, QString("导出成功"));
    QFile csvFile(csvPath);
    QVERIFY2(csvFile.open(QIODevice::ReadOnly), qPrintable(csvFile.errorString()));
    const QList<QByteArray> csvLines = csvFile.readAll().split('\n');
    QCOMPARE(static_cast<int>(csvLines.size()), 12); // 表头、十条数据和结尾换行。
    QCOMPARE(csvLines[0], QByteArray("\xEF\xBB\xBF" "记录 ID,设备,温度 (°C),入库时间\r"));
    QCOMPARE(csvLines[1], QString("%1,\"E\",85,%2\r").arg(latestId, latestTime).toUtf8());
    csvFile.close();

    deviceFilter->setCurrentIndex(deviceFilter->findData(QString("B")));
    QCOMPARE(history->rowCount(), 0);
    QVERIFY(!exportButton->isEnabled());
    queryButton->click();
    QVERIFY(history->rowCount() > 0);
    QCOMPARE(history->item(0, 1)->text(), QString("B"));
    QCOMPARE(history->item(0, 2)->text(), QString("82 °C"));

    // 开始时刻算在范围内，结束时刻和其他设备的记录都排除。
    deviceFilter->setCurrentIndex(deviceFilter->findData(QString("C")));
    timeFilter->setChecked(true);
    startTimeEdit->setDateTime(rangeStart.toLocalTime());
    endTimeEdit->setDateTime(rangeEnd.toLocalTime());
    QCOMPARE(startTimeEdit->dateTime().toSecsSinceEpoch(), rangeStart.toSecsSinceEpoch());
    QCOMPARE(endTimeEdit->dateTime().toSecsSinceEpoch(), rangeEnd.toSecsSinceEpoch());
    queryButton->click();
    QVERIFY2(history->rowCount() == 2, qPrintable(historyStatus->text()));
    QCOMPARE(history->item(0, 2)->text(), QString("63 °C"));
    QCOMPARE(history->item(1, 2)->text(), QString("62 °C"));
    // 两条符合条件的温度是 62 和 63，其他记录不参与统计。
    QVERIFY(historyStatus->text().contains("共 2 条符合条件"));
    QVERIFY(historyStatus->text().contains("最低 62 °C"));
    QVERIFY(historyStatus->text().contains("最高 63 °C"));
    QVERIFY(historyStatus->text().contains("平均 62.5 °C"));

    // 结束时间与开始时间相同时，窗口提示修改条件。
    endTimeEdit->setDateTime(rangeStart.toLocalTime());
    queryButton->click();
    QCOMPARE(history->rowCount(), 0);
    QCOMPARE(historyStatus->text(), QString("开始时间必须早于结束时间。"));

    // 仅在专用测试库中断开应用连接，检查查询失败后是否可以重连。
    {
        QSqlDatabase verifier = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        verifier.setHostName("127.0.0.1");
        verifier.setPort(port);
        verifier.setDatabaseName("device_monitor");
        QVERIFY(verifier.open(userName, password));
        QSqlQuery query(verifier);
        const auto killMonitorConnection = [&] {
            if (!query.exec("SELECT ID FROM information_schema.PROCESSLIST "
                            "WHERE USER = SUBSTRING_INDEX(CURRENT_USER(), '@', 1) "
                            "AND ID <> CONNECTION_ID()") || !query.next()) {
                return false;
            }
            const qulonglong connectionId = query.value(0).toULongLong();
            query.finish();
            return query.exec(QString("KILL CONNECTION %1").arg(connectionId));
        };
        QVERIFY2(killMonitorConnection(), qPrintable(query.lastError().text()));
        endTimeEdit->setDateTime(rangeEnd.toLocalTime());
        queryButton->click();
        QVERIFY(connectButton->isEnabled());
        QVERIFY(!queryButton->isEnabled());
        QVERIFY(!exportButton->isEnabled());
        QCOMPARE(history->rowCount(), 0);

        passwordInput->setText(password + "wrong");
        connectButton->click();
        QVERIFY(status->text().startsWith("MySQL 连接失败"));
        QVERIFY(connectButton->isEnabled());
        passwordInput->setText(password);
        connectButton->click();
        QVERIFY2(status->text().startsWith("MySQL 已连接"), qPrintable(status->text()));
        queryButton->click();
        QCOMPARE(history->rowCount(), 2);

        // 保存失败时，实时温度照常更新，用户仍能重新连接数据库。
        QVERIFY2(killMonitorConnection(), qPrintable(query.lastError().text()));
        simulator.write("E,86\n");
        simulator.flush();
        QTRY_VERIFY(status->text().contains("未保存"));
        auto *live = window.findChild<QTableWidget *>("deviceTable");
        QVERIFY(live);
        QCOMPARE(live->item(4, 1)->text(), QString("86 °C"));
        QVERIFY(connectButton->isEnabled());
        passwordInput->setText(password);
        connectButton->click();
        QVERIFY(status->text().startsWith("MySQL 已连接"));
    }
    QSqlDatabase::removeDatabase(connectionName);
}

QTEST_MAIN(TcpReceiverTest)
#include "tcp_receiver_test.moc"
