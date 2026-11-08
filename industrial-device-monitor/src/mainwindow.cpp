#include "mainwindow.h"
#include "tcp_receiver.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QColor>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QFileDialog>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSaveFile>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

namespace {
// 温度达到这个值时，界面显示告警。
const int kAlarmTemperature = 85;
const QString kHistoryTimeFormat = "yyyy-MM-dd HH:mm:ss.zzz";
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::MainWindow),
      timer_(new QTimer(this)),
      receiver_(new TcpReceiver(this))
{
    // 把 mainwindow.ui 里的布局和控件装到这个窗口上。
    ui_->setupUi(this);

    // 每台设备占一行；你增加的设备 D 也保留在这里。
    devices_.push_back({"A", "设备 A", 0, 0});
    devices_.push_back({"B", "设备 B", 0, 0});
    devices_.push_back({"C", "设备 C", 0, 0});
    devices_.push_back({"D", "设备 D", 0, 0});

    // 设备数量决定表格行数，列名由 Designer 设置。
    ui_->deviceTable->setRowCount(static_cast<int>(devices_.size()));


    // 先填好设备名字和等待状态，开始后再填入温度。
    for (int row = 0; row < static_cast<int>(devices_.size()); ++row) {
        ui_->deviceTable->setItem(row, 0, new QTableWidgetItem(devices_[row].name));
        ui_->deviceTable->setItem(row, 1, new QTableWidgetItem("-- °C"));
        ui_->deviceTable->setItem(row, 2, new QTableWidgetItem("等待开始"));
        ui_->deviceTable->setItem(row, 3, new QTableWidgetItem("--"));
        ui_->deviceTable->setItem(row, 4, new QTableWidgetItem("0 次"));
        // 下拉框显示设备名，查询时用它保存的设备编号。
        ui_->historyDevice->addItem(devices_[row].name, devices_[row].id);
    }

    ui_->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui_->historyTable->horizontalHeader()->setStretchLastSection(true);
    // 默认时间范围是本地的今天零点到明天零点。
    const QDateTime today(QDate::currentDate(), QTime(0, 0));
    ui_->historyStartTime->setDateTime(today);
    ui_->historyEndTime->setDateTime(today.addDays(1));

    // connect 把按钮点击、定时器到点与对应函数连接起来。
    connect(ui_->startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(ui_->tcpButton, &QPushButton::clicked, this, &MainWindow::startTcpReceiving);
    connect(ui_->stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);
    connect(timer_, &QTimer::timeout, this, &MainWindow::updateReading);
    connect(ui_->connectDatabaseButton, &QPushButton::clicked, this, &MainWindow::connectDatabase);
    connect(ui_->queryHistoryButton, &QPushButton::clicked, this, &MainWindow::queryHistory);
    connect(ui_->exportHistoryButton, &QPushButton::clicked, this, &MainWindow::exportHistory);
    // 修改筛选条件后清掉旧结果，等用户再次点击查询。
    const auto resetHistoryForFilter = [this] {
        clearHistoryResults();
        ui_->historyStatus->setText("点击「查询最近 10 条」应用当前筛选条件。");
    };
    connect(ui_->historyDevice, &QComboBox::currentTextChanged, this, resetHistoryForFilter);
    connect(ui_->historyTimeFilter, &QGroupBox::toggled, this, resetHistoryForFilter);
    connect(ui_->historyStartTime, &QDateTimeEdit::dateTimeChanged, this, resetHistoryForFilter);
    connect(ui_->historyEndTime, &QDateTimeEdit::dateTimeChanged, this, resetHistoryForFilter);

    // 根据收到的编号找到设备，只更新匹配的行。
    connect(receiver_, &TcpReceiver::temperatureReceived, this, [this](const QString &deviceId, int temperature) {
        for (int row = 0; row < static_cast<int>(devices_.size()); ++row) {
            if (devices_[row].id == deviceId) {
                applyReading(row, temperature);
                ui_->dataStatus->setText(QString("已接收设备 %1 的温度 %2 °C").arg(deviceId).arg(temperature));
                // 只保存已登记设备通过 TCP 发来的温度。
                saveReading(deviceId, temperature);
                return;
            }
        }
        ui_->dataStatus->setText(QString("忽略未登记设备 %1，请使用表格中已有的设备编号。").arg(deviceId));
    });
    connect(receiver_, &TcpReceiver::statusChanged, ui_->connectionStatus, &QLabel::setText);
    // 数据错误单独显示，连接断开时仍能看到原因。
    connect(receiver_, &TcpReceiver::dataRejected, ui_->dataStatus, &QLabel::setText);
}

MainWindow::~MainWindow()
{
    delete ui_;
}


//读取数据库界面里的账号、密码和端口
void MainWindow::connectDatabase()
{
    // 使用窗口里填写的账号、端口和密码连接。
    const QString userName = ui_->databaseUser->text().trimmed();
    const QString password = ui_->databasePassword->text();
    if (userName.isEmpty() || password.isEmpty()) {
        ui_->databaseStatus->setText("请填写 MySQL 账号和密码。");
        return;
    }

    QString error;
    if (!readingStore_.open(ui_->databasePort->value(), userName, password, error)) {
        ui_->databaseStatus->setText("MySQL 连接失败。 " + error);
        return;
    }

    // 连接成功后清空输入框，后续记录复用这条数据库连接。
    ui_->databasePassword->clear();
    ui_->databaseUser->setEnabled(false);
    ui_->databasePassword->setEnabled(false);
    ui_->databasePort->setEnabled(false);
    ui_->connectDatabaseButton->setEnabled(false);
    ui_->databaseStatus->setText("MySQL 已连接，等待 TCP 温度入库。");
    ui_->historyDevice->setEnabled(true);
    ui_->historyTimeFilter->setEnabled(true);
    ui_->queryHistoryButton->setEnabled(true);
    ui_->historyStatus->setText("点击「查询最近 10 条」查看所选设备的记录。");
}

//查询数据的历史信息
void MainWindow::queryHistory()
{
    clearHistoryResults();
    // 取出设备编号，使用已有的 MySQL 连接查询。
    const QString deviceId = ui_->historyDevice->currentData().toString();
    const bool filterByTime = ui_->historyTimeFilter->isChecked();
    const QDateTime startTime = filterByTime ? ui_->historyStartTime->dateTime() : QDateTime();
    const QDateTime endTime = filterByTime ? ui_->historyEndTime->dateTime() : QDateTime();
    if (filterByTime && (!startTime.isValid() || !endTime.isValid() || startTime >= endTime)) {
        ui_->historyStatus->setText("开始时间必须早于结束时间。");
        return;
    }
    ReadingStore::HistoryResult result;
    QString error;
    if (!readingStore_.selectRecentReadings(deviceId, startTime, endTime, result, error)) {
        handleDatabaseFailure("历史查询失败。 " + error);
        return;
    }
    if (result.readings.isEmpty()) {
        ui_->historyStatus->setText(QString("设备 %1 没有符合条件的历史记录。").arg(deviceId));
        return;
    }

    // 查询结果有几条，历史表格就填几行。
    const int rowCount = static_cast<int>(result.readings.size());
    ui_->historyTable->setRowCount(rowCount);
    for (int row = 0; row < rowCount; ++row) {
        const ReadingStore::Reading &reading = result.readings[row];
        ui_->historyTable->setItem(row, 0, new QTableWidgetItem(QString::number(reading.id)));
        ui_->historyTable->setItem(row, 1, new QTableWidgetItem(reading.deviceId));
        ui_->historyTable->setItem(row, 2, new QTableWidgetItem(QString::number(reading.temperature) + " °C"));
        const QString time = reading.receivedAt.toLocalTime().toString(kHistoryTimeFormat);
        ui_->historyTable->setItem(row, 3, new QTableWidgetItem(time));
    }
    // 分开显示匹配总数和明细条数，平均温度保留一位小数。
    ui_->historyStatus->setText(QString("设备 %1 共 %2 条符合条件，显示最近 %3 条。\n"
                                       "最低 %4 °C　最高 %5 °C　平均 %6 °C")
                                   .arg(deviceId).arg(result.totalCount).arg(rowCount)
                                   .arg(result.minimumTemperature).arg(result.maximumTemperature)
                                   .arg(result.averageTemperature, 0, 'f', 1));
    historyReadings_ = result.readings;
    ui_->exportHistoryButton->setEnabled(true);
}

void MainWindow::clearHistoryResults()
{
    historyReadings_.clear();
    ui_->historyTable->setRowCount(0);
    ui_->exportHistoryButton->setEnabled(false);
}

void MainWindow::handleDatabaseFailure(const QString &message)
{
    // 保存和查询出错都恢复输入框，允许用户重新连接。
    readingStore_.close();
    ui_->databaseStatus->setText(message);
    ui_->databaseUser->setEnabled(true);
    ui_->databasePassword->setEnabled(true);
    ui_->databasePort->setEnabled(true);
    ui_->connectDatabaseButton->setEnabled(true);
    ui_->historyDevice->setEnabled(false);
    ui_->historyTimeFilter->setEnabled(false);
    ui_->queryHistoryButton->setEnabled(false);
    clearHistoryResults();
    ui_->historyStatus->setText("请重新连接 MySQL 后查询。");
}

//把数据记录列表安全地保存成 CSV
void MainWindow::exportHistory()
{
    if (historyReadings_.isEmpty()) {
        return;
    }

    // 先把当前明细转成 CSV。温度列保留数字，单位写在列名中。
    QString csv = "记录 ID,设备,温度 (°C),入库时间\r\n";
    for (const ReadingStore::Reading &reading : historyReadings_) {
        QString device = reading.deviceId;
        device.replace('"', "\"\""); // CSV 中的引号写成两个引号。
        const QString time = reading.receivedAt.toLocalTime().toString(kHistoryTimeFormat);
        csv += QString("%1,\"%2\",%3,%4,\r\n")
                   .arg(QString::number(reading.id), device, QString::number(reading.temperature), time);
    }
    // 添加 UTF-8 标记，方便表格软件识别中文列名。
    const QByteArray bytes = "\xEF\xBB\xBF" + csv.toUtf8();

    QFileDialog dialog(this, "导出温度明细");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("CSV 文件 (*.csv)");
    dialog.setDefaultSuffix("csv");
    dialog.selectFile("temperature_readings.csv");
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // 写完再提交文件，避免写入失败时留下不完整的 CSV。
    QSaveFile file(dialog.selectedFiles().constFirst());
    if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size() || !file.commit()) {
        QMessageBox::warning(this, "导出失败", file.errorString());
        return;
    }
    QMessageBox::information(this, "导出成功", "温度明细已保存。\n" + file.fileName());
}


//它负责检查数据库是否连接，再调用保存函数
void MainWindow::saveReading(const QString &deviceId, int temperature)
{
    if (!readingStore_.isOpen()) {
        return;
    }

    QString error;
    if (!readingStore_.insertReading(deviceId, temperature, error)) {
        // 入库失败时保留实时显示，并允许用户重新连接数据库。
        handleDatabaseFailure("本条温度未保存。 " + error);
        return;
    }
    ui_->databaseStatus->setText(QString("MySQL 已保存设备 %1 的温度 %2 °C").arg(deviceId).arg(temperature));
}

void MainWindow::startSimulation()
{
    // 防止重复点击“开始”，并允许点击“停止”。
    ui_->startButton->setEnabled(false);
    ui_->tcpButton->setEnabled(false);
    ui_->stopButton->setEnabled(true);
    ui_->connectionStatus->setText("本地模拟：所有设备每 2 秒更新一次");
    ui_->dataStatus->setText("正在本地生成温度数据。");
    // 先立即刷新一次，再启动定时器。
    updateReading();
    timer_->start(2000); // 保留你设置的 2 秒更新间隔。
}

void MainWindow::startTcpReceiving()
{
    // 启动失败时，状态栏会显示原因，还可以重新尝试。
    if (!receiver_->start()) {
        return;
    }
    ui_->startButton->setEnabled(false);
    ui_->tcpButton->setEnabled(false);
    ui_->stopButton->setEnabled(true);

    // 每台设备都等待带有自己编号的数据。
    ui_->dataStatus->setText("等待有效温度记录。");
    for (int row = 0; row < static_cast<int>(devices_.size()); ++row) {
        ui_->deviceTable->item(row, 2)->setText("等待 TCP 数据");
        ui_->deviceTable->item(row, 2)->setForeground(QBrush(Qt::black));
    }
}

void MainWindow::stopSimulation()
{
    // 关闭定时器和网络接收，结束当前模式。
    timer_->stop();
    receiver_->stop();
    ui_->startButton->setEnabled(true);
    ui_->tcpButton->setEnabled(true);
    ui_->stopButton->setEnabled(false);
    ui_->connectionStatus->setText("已停止，可以重新选择数据来源");

    // 给每台设备都显示“已停止”。
    for (int row = 0; row < static_cast<int>(devices_.size()); ++row) {
        ui_->deviceTable->item(row, 2)->setText("已停止");
        ui_->deviceTable->item(row, 2)->setForeground(QBrush(Qt::red));
    }
}

void MainWindow::updateReading()
{
    // 本地模式用随机数生成温度，再交给统一的显示函数。
    for (int row = 0; row < static_cast<int>(devices_.size()); ++row) {
        applyReading(row, QRandomGenerator::global()->bounded(50, 96));
    }
}

void MainWindow::applyReading(int row, int temperature)
{
    // 无论数据来自随机数还是 TCP，都用相同的方法计数和显示。
    DeviceState &device = devices_[row];
    device.temperature = temperature;
    ++device.readingCount;
    ++readingCount_;

    ui_->deviceTable->item(row, 1)->setText(QString::number(device.temperature) + " °C");
    ui_->deviceTable->item(row, 3)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    ui_->deviceTable->item(row, 4)->setText(QString::number(device.readingCount) + " 次");

    // 两种数据来源都使用你设置的 85°C 告警阈值。
    QTableWidgetItem *status = ui_->deviceTable->item(row, 2);
    if (device.temperature >= kAlarmTemperature) {
        status->setText(QString("高温告警（≥ %1 °C）").arg(kAlarmTemperature));
        status->setForeground(QBrush(Qt::red));
    } else {
        status->setText("正常运行");
        status->setForeground(QBrush(Qt::green));
    }
    ui_->totalCount->setText(QString::number(readingCount_) + " 次");
}
