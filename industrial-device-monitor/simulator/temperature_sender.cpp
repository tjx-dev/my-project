// Windows 网络接口，提供 Socket 操作和 IP 地址转换。
#include <winsock2.h>
#include <ws2tcpip.h>

#include "temperature_sender.h"

#include <exception>
#include <iostream>
#include <stdexcept>

std::string sendReport(const std::string &message)
{
    if (message.empty() || message.size() > 4096) {
        return "Report must contain 1 to 4096 bytes.";
    }

    // 第 1 步：初始化 Windows 网络功能。
    WSADATA data{}; // 存放 Windows 返回的 Winsock 信息，{} 先清零。
    // 请求使用 Winsock 2.2；&data 让函数把结果写进 data。
    const int startupResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (startupResult != 0) {
        return "WSAStartup failed: " + std::to_string(startupResult);
    }

    // 第 2 步：创建 TCP Socket。
    // 三个参数：IPv4、字节流、TCP；返回系统分配的 Socket 句柄。
    SOCKET deviceSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (deviceSocket == INVALID_SOCKET) {
        const int error = WSAGetLastError();
        WSACleanup();
        return "Socket failed: " + std::to_string(error);
    }

    // 第 3 步：连接本机的监控窗口，端口要与接收端一致。
    sockaddr_in address{}; // IPv4 地址结构体，保存对方的 IP 和端口。
    address.sin_family = AF_INET; // 地址类型与创建 Socket 时一致。
    address.sin_port = htons(9000); // 把端口转成网络要求的字节顺序。
    // 把 IP 文本转换为二进制地址，结果写入 address.sin_addr。
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    // 这里的 connect 建立 TCP 连接，与 Qt 的信号槽 connect 不同。
    // 转成接口要求的通用地址指针；sizeof 告诉它地址结构占多少字节。
    if (connect(deviceSocket, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == SOCKET_ERROR) {
        const int error = WSAGetLastError();
        closesocket(deviceSocket);
        WSACleanup();
        return "Connect failed: " + std::to_string(error)
               + ". Start TCP receiving in the Qt window first.";
    }

    // 第 4 步：把整段文本发完，一条连接里可以有多行。
    int sent = 0; // 已成功交给系统发送的字节数。
    while (sent < static_cast<int>(message.size())) {
        // 从未发送的位置继续发送；第 3 个参数是剩余长度，0 表示常规发送。
        const int count = send(deviceSocket, message.data() + sent,
                               static_cast<int>(message.size()) - sent, 0);
        if (count <= 0) {
            const int error = WSAGetLastError();
            closesocket(deviceSocket);
            WSACleanup();
            return "Send failed: " + std::to_string(error);
        }
        sent += count; // 一次 send 不一定能把整条数据发完。
    }

    // 第 5 步：发送完毕，关闭连接并清理网络资源。
    shutdown(deviceSocket, SD_SEND); // 本次连接不再发送新数据。
    closesocket(deviceSocket); // 关闭这个 Socket，释放句柄。
    WSACleanup(); // 结束本程序这次 Winsock 使用，与 WSAStartup 配对。
    return {};
}

// 命令行模式从参数取值，最后调用与窗口相同的发送函数。
int sendFromCommandLine(int argc, char *argv[])
{
    int temperature;
    try {
        if (argc != 2) {
            throw std::invalid_argument("Expected one argument");
        }
        const std::string input = argv[1];
        std::size_t used = 0;
        // 文本转整数，used 记录读了几个字符，防止把 90abc 当作 90。
        temperature = std::stoi(input, &used);
        if (used != input.size()) {
            throw std::invalid_argument("Invalid temperature");
        }
        if (temperature < -50 || temperature > 150) {
            throw std::out_of_range("Temperature out of range");
        }
    } catch (const std::exception &) {
        // 参数错误就打印用法并退出，不再执行后面的网络代码。
        std::cerr << "Usage: DeviceSimulator.exe [temperature: -50..150]\n";
        return 1;
    }

    const std::string error = sendReport("A," + std::to_string(temperature) + "\n");
    if (!error.empty()) {
        std::cerr << error << '\n';
        return 1;
    }

    std::cout << "Sent temperature: " << temperature << " C. Check the Qt window.\n";
    return 0;
}
