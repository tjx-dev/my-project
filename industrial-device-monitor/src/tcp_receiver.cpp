#include "tcp_receiver.h"

#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

TcpReceiver::TcpReceiver(QObject *parent)
    : QObject(parent), server_(new QTcpServer(this))
{
    // 这是 Qt 信号槽连接：有新客户端时调用 acceptClient()。
    connect(server_, &QTcpServer::newConnection, this, &TcpReceiver::acceptClient);
}

TcpReceiver::~TcpReceiver()
{
    stop();
}

bool TcpReceiver::start(quint16 port)
{
    if (server_->isListening()) {
        return true;
    }

    // 监听本机 127.0.0.1 的指定端口，等客户端发起连接。
    if (!server_->listen(QHostAddress::LocalHost, port)) {
        emit statusChanged("启动失败：" + server_->errorString());
        return false;
    }

    emit statusChanged(QString("正在监听 127.0.0.1:%1，等待设备发送温度").arg(server_->serverPort()));
    return true;
}

void TcpReceiver::stop()
{
    // 停止监听，关闭所有已接入的连接。
    server_->close();
    const auto sockets = buffers_.keys();
    buffers_.clear();
    for (QTcpSocket *socket : sockets) {
        disconnect(socket, nullptr, this, nullptr);
        socket->abort();
        socket->deleteLater();
    }
}

quint16 TcpReceiver::port() const
{
    return server_->serverPort();
}

void TcpReceiver::acceptClient()
{
    while (server_->hasPendingConnections()) {
        // 监听器继续等连接，这个 socket 专门和当前客户端通信。
        QTcpSocket *socket = server_->nextPendingConnection();

        // 新连接有自己的缓冲区，先从空开始。
        socket->setReadBufferSize(1024);
        buffers_.insert(socket, QByteArray());

        // 哪条连接发出 readyRead，就读取哪条连接。
        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
            readData(socket);
        });
        connect(socket, &QTcpSocket::disconnected, this, [this, socket]() {
            // 只清理断开的连接，其他连接仍能收数据。
            const bool incomplete = !buffers_.take(socket).isEmpty();
            socket->deleteLater();
            const QString message = incomplete
                ? QString("连接已断开，未结束的一行数据已丢弃")
                : QString("连接已断开，仍在监听");
            emit statusChanged(message + QString("，当前连接数：%1").arg(buffers_.size()));
        });

        emit statusChanged(QString("设备已连接，当前连接数：%1").arg(buffers_.size()));
        // 连接建立时，数据可能已经到达。
        readData(socket);
    }
}

void TcpReceiver::readData(QTcpSocket *socket)
{
    if (!buffers_.contains(socket)) {
        return;
    }

    // 只把收到的字节追加到这条连接的缓冲区。
    buffers_[socket].append(socket->readAll());

    while (buffers_.contains(socket)) {
        QByteArray &buffer = buffers_[socket];
        const int newline = buffer.indexOf('\n');
        if (newline < 0) {
            // 半行留到下次再拼，过长时只关闭这一条连接。
            if (buffer.size() > 64) {
                socket->abort();
                emit dataRejected("数据行过长，已关闭此连接；请缩短输入后重试。");
                emit statusChanged("数据行过长，已关闭此连接；仍可重新连接");
            }
            return;
        }

        // 每行是一条记录，例如 B,90。取出后移除这一行和换行。
        const QByteArray line = buffer.left(newline);
        buffer.remove(0, newline + 1);

        // 逗号左边是编号，右边是温度。旧的纯数字格式仍对应设备 A。
        const int comma = line.indexOf(',');
        const QByteArray deviceId = comma < 0 ? QByteArray("A") : line.left(comma).trimmed();
        const QByteArray temperatureText = (comma < 0 ? line : line.mid(comma + 1)).trimmed();

        bool ok = false;
        const int temperature = temperatureText.toInt(&ok);
        if (line.size() > 64 || deviceId.isEmpty() || !ok || temperature < -50 || temperature > 150) {
            emit dataRejected("忽略无效数据：每行应为设备编号,温度，温度范围 -50～150");
            emit statusChanged("忽略无效数据：每行应为设备编号,温度，温度范围 -50～150");
            continue;
        }

        // 接收器发出数据，窗口根据编号决定更新哪一行。
        const QString id = QString::fromLatin1(deviceId);
        emit temperatureReceived(id, temperature);
        emit statusChanged(QString("已接收设备 %1 的温度 %2 °C").arg(id).arg(temperature));
    }
}
