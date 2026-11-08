# 第 4 步① 保存一条 TCP 温度

本次用 MySQL 保存设备编号、温度和入库时间。监控窗口继续实时显示，历史记录由 MySQL 保存在表里。

```mermaid
flowchart LR
    A["模拟器发 E,85"] --> B["监控端确认设备 E"]
    B --> C["实时表格更新"]
    B --> D["INSERT 写入一条历史记录"]
    D --> E["在 MySQL 用 SELECT 验证"]
```

## 今天只做一件事

你已经建好了 `device_monitor.temperature_readings` 表。现在让 Qt 自动执行一次 INSERT，再到 DataGrip 验证。先用单次发送，方便数清楚新增了几行。

本机的项目数据库地址是 `127.0.0.1:3307`。Qt 的账号、密码和端口，要与你在 DataGrip 成功连接这张表时使用的一致。窗口默认账号为 `monitor_app`，也可以改成你创建的其他账号。

电脑重启后，如果 DataGrip 提示 `Connection refused`，双击 `D:\codex\2026-11-02\q\industrial-device-monitor\tools\start-project-mysql.cmd` 启动现有项目数据库，再回到 DataGrip 重新执行查询。使用原来的账号和密码。

日常可以双击同目录的 `open-datagrip.cmd`，它会先启动项目数据库，再打开 DataGrip 项目。`open-monitor-and-simulator.cmd` 也会检查并启动已有的项目数据库。

## 在 Qt 发一条并验证

在文件资源管理器打开 `D:\codex\2026-11-02\q\industrial-device-monitor\tools`，双击 `open-monitor-and-simulator.cmd`。它会打开已编译好的监控窗口和模拟器，并设置当前电脑需要的运行库与项目数据库证书。

也可以从 PowerShell 启动：

```powershell
# 为当前启动的程序指定 Qt 运行库和项目 MySQL 的证书。
$env:PATH = 'C:\msys64\mingw64\bin;' + $env:PATH
$env:SSL_CERT_FILE = 'D:\codex\2026-11-02\q\industrial-device-monitor\.local-mysql\data\ca.pem'
& 'D:\codex\2026-11-02\q\industrial-device-monitor\build\IndustrialDeviceMonitor.exe'
```

1. 确认监控窗口数据库端口为 `3307`，填写 DataGrip 使用的账号和密码，点击「连接 MySQL」。密码只在你本机填写。
2. 看到「MySQL 已连接」后点击「开始 TCP 接收」。在模拟器文本框只留 `E,85`，点击一次「发送数据」。
3. 监控窗口应提示已保存设备 E 的温度。回到 DataGrip，在连接这张表的查询控制台中执行下面的 SQL。选中完整语句，按 `Ctrl+Enter`。

```sql
-- 按自增主键倒序，先看最新插入的行。
SELECT id, device_id, temperature, received_at
FROM device_monitor.temperature_readings
WHERE device_id = 'E'
ORDER BY id DESC
LIMIT 5;
```

看到最新记录中的 `E`、`85` 和时间，就完成了第一次自动入库。再发一次 `E,84`，重新执行查询，应看到新增的一行。两条记录的 `id` 不同；监控表格仍用设备 E 的同一行展示最新温度。

如果你之前手动插入过相同数据，通过最新的 `id` 和时间确认本次新增的记录。当前只保存连接数据库之后收到的有效 TCP 数据，本地随机模拟和之前收到的数据不会补写。

## 这次读哪三处

1. `sql/schema.sql` 定义一条记录有哪些列。`id` 是每条记录的编号，`device_id` 是哪台设备。
2. `src/mainwindow.cpp` 的 TCP 回调先调用 `applyReading()` 更新表格，再调用 `saveReading()` 保存。
3. `src/reading_store.cpp` 的 `insertReading()` 用 `prepare()` 准备 SQL，再用 `bindValue()` 把 `E` 和 `85` 放进各自的占位符，最后用 `exec()` 执行。

先能解释这句话就够了：收到设备 E 的 85 度，Qt 把设备编号和温度填进 INSERT 语句，MySQL 执行后新增一行，主键和时间自动生成。

## 如果连接失败

- 提示无法加载 QMYSQL 时，检查 Qt Kit 是否装了相匹配的驱动及其依赖。当前工作区使用 MSYS2 MINGW64，对应的客户端依赖包是 `mingw-w64-x86_64-libmariadbclient`。可在 MSYS2 MINGW64 终端运行 `pacman -S --needed mingw-w64-x86_64-libmariadbclient`，然后重启 Qt Creator。[MSYS2 官方包](https://packages.msys2.org/packages/mingw-w64-x86_64-libmariadbclient)
- 如果 Qt Creator 使用另一个构建目录，且遇到 TLS 错误，可以先用上面提供的 `build` 版本。这个目录中的客户端库使用 OpenSSL。
- 提示 `Access denied` 时，对照 DataGrip 检查账号、密码和端口是否一致；账号需要具有这张表的 INSERT 权限。
- 显示连接成功但第一条保存失败时，检查 `temperature_readings` 表是否已经创建。窗口会保留实时温度，并显示保存错误。
