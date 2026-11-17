#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QString>

class QTcpServer;
class QTcpSocket;

// 只负责 TCP 接收和解析；窗口负责显示数据。
class TcpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit TcpReceiver(QObject *parent = nullptr);
    ~TcpReceiver() override;

    bool start(quint16 port = 9000);
    void stop();
    quint16 port() const;

signals:
    // 一条完整记录包含设备编号和温度。
    void temperatureReceived(const QString &deviceId, int temperature);
    void statusChanged(const QString &message);
    void dataRejected(const QString &message); // 通知窗口显示数据被拒绝的原因。

private:
    void acceptClient();
    void readData(QTcpSocket *socket);

    QTcpServer *server_;
    // 键是连接的 Socket，值是这条连接还没处理完的字节。
    QHash<QTcpSocket *, QByteArray> buffers_;
};

#endif // TCP_RECEIVER_H
