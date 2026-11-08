#ifndef SIMULATOR_WIDGET_H
#define SIMULATOR_WIDGET_H

#include <QByteArray>
#include <QFutureWatcher>
#include <QWidget>
#include <string>

class QTcpSocket;
class QTimer;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
private:
    QByteArray reportBytes();
    void startSending();
    void showSendResult();
    void startPeriodicSending();
    void sendNextReport();
    void stopPeriodicSending();

    Ui::Widget *ui_;
    QFutureWatcher<std::string> sendWatcher_; // 后台发送完成时通知窗口。
    QTcpSocket *periodicSocket_;
    QTimer *periodicTimer_;
    QByteArray periodicReport_; // 保存本次定时发送使用的文本。
};

#endif
