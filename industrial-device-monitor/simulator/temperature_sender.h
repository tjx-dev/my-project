#ifndef TEMPERATURE_SENDER_H
#define TEMPERATURE_SENDER_H

#include <string>

// 在一条 TCP 连接中发送整段文本，成功返回空字符串。
std::string sendReport(const std::string &message);

// 读取命令行参数并发送，返回程序退出码。
int sendFromCommandLine(int argc, char *argv[]);

#endif
