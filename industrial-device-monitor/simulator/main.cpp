#include "temperature_sender.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // 带温度参数时，保留原来的命令行发送方式。
    if (argc > 1) {
        return sendFromCommandLine(argc, argv);
    }

    // 从 Qt Creator 运行时，显示用 Designer 设计的窗口。
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}
