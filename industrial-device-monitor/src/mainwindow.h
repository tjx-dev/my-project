#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "reading_store.h"

#include <QString>
#include <QWidget>
#include <vector>

class QTimer;
class TcpReceiver;

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void connectDatabase();
    void queryHistory();
    void exportHistory();
    void startSimulation();
    void startTcpReceiving();
    void stopSimulation();
    void updateReading();

private:
    // 编号用来匹配 TCP 数据，名字用来显示。
    struct DeviceState {
        QString id;
        QString name;
        int temperature;
        int readingCount;
    };

    void applyReading(int row, int temperature); // 两种数据来源共用显示和告警逻辑。
    void saveReading(const QString &deviceId, int temperature);
    void clearHistoryResults();
    void handleDatabaseFailure(const QString &message);

    std::vector<DeviceState> devices_;
    Ui::MainWindow *ui_; // 通过它访问 Designer 中的控件。
    QTimer *timer_;
    TcpReceiver *receiver_;
    ReadingStore readingStore_;
    QVector<ReadingStore::Reading> historyReadings_; // 保存当前历史表格对应的记录。
    int readingCount_ = 0;
};

#endif // MAINWINDOW_H
