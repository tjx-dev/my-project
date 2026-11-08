$ErrorActionPreference = 'Stop'
$project = Split-Path -Parent $PSScriptRoot
$runtime = Join-Path $project '.local-mysql'
$data = Join-Path $runtime 'data'
$settingsFile = Join-Path $runtime 'instance.json'
$pidFile = Join-Path $runtime 'mysql.pid'
$logFile = Join-Path $runtime 'mysql.err'
$mysqlBase = 'C:\MySQL\mysql-8.0.45-winx64\mysql-8.0.45-winx64'
$mysqld = Join-Path $mysqlBase 'bin\mysqld.exe'

# 只启动已初始化的项目实例。
foreach ($file in @($settingsFile, (Join-Path $data 'auto.cnf'), (Join-Path $data 'mysql.ibd'), $mysqld)) {
    if (-not (Test-Path -LiteralPath $file -PathType Leaf)) {
        throw "Required file not found: $file"
    }
}
$settings = Get-Content -LiteralPath $settingsFile -Raw | ConvertFrom-Json
$port = [int]$settings.Port
if ($settings.Marker -ne 'IndustrialDeviceMonitor-project-mysql-v1' -or $port -lt 1024 -or $port -gt 65535 -or $port -eq 3306) {
    throw 'Invalid project MySQL instance settings.'
}

function Get-ListenerId {
    foreach ($line in (netstat.exe -ano -p TCP)) {
        if ($line -match "^\s*TCP\s+(?:127\.0\.0\.1|0\.0\.0\.0):$port\s+\S+\s+LISTENING\s+(\d+)\s*$") {
            return [int]$Matches[1]
        }
    }
    return 0
}

# 重复运行时，核对端口和项目 PID 文件。
$listenerId = Get-ListenerId
if ($listenerId) {
    $running = Get-Process -Id $listenerId -ErrorAction SilentlyContinue
    if ($running -and $running.ProcessName -eq 'mysqld' -and
        (Test-Path -LiteralPath $pidFile) -and
        ([IO.File]::ReadAllText($pidFile).Trim() -eq "$listenerId")) {
        Write-Host "Project MySQL already running on 127.0.0.1:$port (PID $listenerId)."
        return
    }
    throw "Port $port is already occupied by PID $listenerId."
}

# 使用现有数据目录启动，不需要数据库账号或密码。
Write-Host "Starting project MySQL on 127.0.0.1:$port..."
$server = Start-Process -FilePath $mysqld -WindowStyle Hidden -PassThru -ArgumentList @(
    '--no-defaults', '--no-monitor', "--basedir=`"$mysqlBase`"", "--datadir=`"$data`"",
    "--port=$port", '--bind-address=127.0.0.1', '--mysqlx=OFF', '--skip-log-bin',
    '--innodb-buffer-pool-size=64M', "--pid-file=`"$pidFile`"", "--log-error=`"$logFile`""
)
$deadline = (Get-Date).AddSeconds(20)
while (-not $server.HasExited -and (Get-Date) -lt $deadline) {
    if ((Get-ListenerId) -eq $server.Id) {
        Write-Host "Project MySQL ready on 127.0.0.1:$port (PID $($server.Id))."
        return
    }
    Start-Sleep -Milliseconds 300
}
throw "Project MySQL did not start listening. Check $logFile"
