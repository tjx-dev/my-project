#include "widget.h"
#include "temperature_sender.h"
#include "ui_widget.h"

#include <QHostAddress>
#include <QTcpSocket>
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::Widget),
      periodicSocket_(new QTcpSocket(this)),
      periodicTimer_(new QTimer(this))
{
    // 把 widget.ui 里的控件和布局装到这个窗口上。
    ui_->setupUi(this);
    connect(ui_->sendButton, &QPushButton::clicked, this, &Widget::startSending);
    connect(&sendWatcher_, &QFutureWatcher<std::string>::finished, this, &Widget::showSendResult);
    connect(ui_->startPeriodicButton, &QPushButton::clicked, this, &Widget::startPeriodicSending);
    connect(ui_->stopPeriodicButton, &QPushButton::clicked, this, &Widget::stopPeriodicSending);
    periodicTimer_->setInterval(3000); // 保留你设置的每 3 秒发送一次。
    connect(periodicTimer_, &QTimer::timeout, this, &Widget::sendNextReport);

    // 连上监控端后，先发一次，再按定时器间隔重复发送。
    connect(periodicSocket_, &QTcpSocket::connected, this, [this]() {
        ui_->statusLabel->setText(QString("已连接，每 %1 秒发送一次。").arg(periodicTimer_->interval() / 1000.0));
        periodicTimer_->start();
        sendNextReport();
    });

    // 连接失败或断开后，停止定时器，并恢复窗口按钮。
    connect(periodicSocket_, &QTcpSocket::stateChanged, this,
            [this](QAbstractSocket::SocketState state) {
        if (state == QAbstractSocket::UnconnectedState) {
            periodicTimer_->stop();
            ui_->reportInput->setReadOnly(false);
            ui_->sendButton->setEnabled(true);
            ui_->startPeriodicButton->setEnabled(true);
            ui_->stopPeriodicButton->setEnabled(false);
            ui_->statusLabel->setText("定时发送已停止。重试前请确认监控窗口正在接收。");
        }
    });
    connect(periodicSocket_, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        // 先恢复按钮，再显示连接失败或中断的具体原因。
        const QString error = periodicSocket_->errorString();
        stopPeriodicSending();
        ui_->statusLabel->setText("连接失败或中断。 " + error);
    });
}

Widget::~Widget()
{
    disconnect(periodicSocket_, nullptr, this, nullptr);
    delete ui_;
}

QByteArray Widget::reportBytes()
{
    // 取出文本框里的所有行，并为最后一条补上换行。
    QByteArray report = ui_->reportInput->toPlainText().trimmed().toUtf8();
    if (report.isEmpty()) {
        ui_->statusLabel->setText("请输入设备编号和温度，例如 A,84。");
        return {};
    }
    report += '\n';
    if (report.size() > 4096) {
        ui_->statusLabel->setText("每次发送的文本最多 4096 字节。");
        return {};
    }
    return report;
}

void Widget::startSending()
{
    const std::string message = reportBytes().toStdString();
    if (message.empty()) {
        return;
    }

    // 单次发送还没结束时，先禁用两种发送方式的开始按钮。
    ui_->sendButton->setEnabled(false);
    ui_->startPeriodicButton->setEnabled(false);
    ui_->statusLabel->setText("正在发送…");

    // Winsock 连接可能要等待，把发送放到后台，窗口仍可操作。
    sendWatcher_.setFuture(QtConcurrent::run([message]() {
        return sendReport(message);
    }));
}

void Widget::showSendResult()
{
    // 发送结束后回到窗口线程，显示结果并恢复按钮。
    const std::string error = sendWatcher_.result();
    if (error.empty()) {
        ui_->statusLabel->setText("已发送，请在监控窗口确认。");
    } else {
        ui_->statusLabel->setText("发送失败。\n" + QString::fromStdString(error));
    }
    ui_->sendButton->setEnabled(true);
    ui_->startPeriodicButton->setEnabled(true);
}

void Widget::startPeriodicSending()
{
    periodicReport_ = reportBytes();
    if (periodicReport_.isEmpty()) {
        return;
    }

    ui_->reportInput->setReadOnly(true);
    ui_->sendButton->setEnabled(false);
    ui_->startPeriodicButton->setEnabled(false);
    ui_->stopPeriodicButton->setEnabled(true);
    ui_->statusLabel->setText("正在连接监控窗口…");

    // 点击开始时只连接一次，定时器每次触发都使用这个 Socket。
    periodicSocket_->connectToHost(QHostAddress::LocalHost, 9000);
}

void Widget::sendNextReport()
{
    // 上一批字节还在等待发送时，先等到下一次定时。
    if (periodicSocket_->state() != QAbstractSocket::ConnectedState
        || periodicSocket_->bytesToWrite() > 0) {
        return;
    }
    if (periodicSocket_->write(periodicReport_) != periodicReport_.size()) {
        stopPeriodicSending();
        ui_->statusLabel->setText("发送失败，定时发送已停止。");
    }
}

void Widget::stopPeriodicSending()
{
    // 停止产生新数据，并关闭这条连接。
    periodicTimer_->stop();
    periodicSocket_->abort();
}
