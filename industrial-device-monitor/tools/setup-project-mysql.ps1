param(
    [ValidateRange(1024, 65535)][int]$Port = 3307,
    [string]$UserName,
    [Security.SecureString]$Password,
    [string]$RuntimeDirectory,
    [switch]$SkipDataGrip,
    [switch]$OpenConsole
)

# 创建项目专用的 MySQL 实例，不需要管理员权限。
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$projectDirectory = Split-Path -Parent $PSScriptRoot
$mysqlBase = 'C:\MySQL\mysql-8.0.45-winx64\mysql-8.0.45-winx64'
$mysqld = Join-Path $mysqlBase 'bin\mysqld.exe'
$mysql = Join-Path $mysqlBase 'bin\mysql.exe'
$dataGrip = 'D:\datagrip\DataGrip 2025.3.5\bin\datagrip64.exe'

if (-not $RuntimeDirectory) {
    $RuntimeDirectory = Join-Path $projectDirectory '.local-mysql'
}
$RuntimeDirectory = [IO.Path]::GetFullPath($RuntimeDirectory)
if ($RuntimeDirectory -match '[^\x20-\x7e]' -or $RuntimeDirectory -match '\s') {
    throw 'Use a project runtime path containing ASCII characters and no spaces.'
}
foreach ($required in @($mysqld, $mysql, (Join-Path $projectDirectory 'sql\schema.sql'))) {
    if (-not (Test-Path -LiteralPath $required -PathType Leaf)) {
        throw "Required file not found: $required"
    }
}

function Get-SecretText([Security.SecureString]$Secret) {
    $pointer = [Runtime.InteropServices.Marshal]::SecureStringToBSTR($Secret)
    try { return [Runtime.InteropServices.Marshal]::PtrToStringBSTR($pointer) }
    finally { [Runtime.InteropServices.Marshal]::ZeroFreeBSTR($pointer) }
}

# 只用于正常关闭项目实例，不操作 MySQL80 服务。
if (-not ('ProjectMySqlStopSignal' -as [type])) {
    Add-Type @'
using System;
using System.Runtime.InteropServices;
public static class ProjectMySqlStopSignal {
    [DllImport("kernel32.dll", CharSet=CharSet.Unicode, SetLastError=true)]
    private static extern IntPtr OpenEvent(uint access, bool inherit, string name);
    [DllImport("kernel32.dll")] private static extern bool SetEvent(IntPtr handle);
    [DllImport("kernel32.dll")] private static extern bool CloseHandle(IntPtr handle);
    public static bool Request(int processId) {
        IntPtr handle = OpenEvent(2, false, "MYSQLShutdown" + processId);
        if (handle == IntPtr.Zero) return false;
        try { return SetEvent(handle); }
        finally { CloseHandle(handle); }
    }
}
'@
}

function Stop-ProjectServer([Diagnostics.Process]$Server) {
    if ($Server.HasExited) { return }
    if (-not [ProjectMySqlStopSignal]::Request($Server.Id)) {
        throw 'Cannot request shutdown of the project MySQL. Run this launcher using the Windows account that started it.'
    }
    if (-not $Server.WaitForExit(20000)) {
        throw 'The project MySQL is still shutting down. Keep this window open and retry after it exits.'
    }
}

if (-not $UserName) {
    $UserName = Read-Host 'Choose your SQL username (Enter = monitor_app)'
    if (-not $UserName) { $UserName = 'monitor_app' }
}
if ($UserName -notmatch '^[a-zA-Z][a-zA-Z0-9_]{0,31}$' -or $UserName -ieq 'root') {
    throw 'Choose a username of 1-32 letters/digits/underscores, starting with a letter. Use an application account.'
}
if (-not $Password) {
    $Password = Read-Host 'Choose your SQL password' -AsSecureString
    $confirmation = Read-Host 'Repeat your SQL password' -AsSecureString
    try {
        if ((Get-SecretText $Password) -cne (Get-SecretText $confirmation)) {
            throw 'The passwords do not match.'
        }
    } finally { $confirmation.Dispose() }
}
$passwordText = Get-SecretText $Password
if ([string]::IsNullOrWhiteSpace($passwordText) -or $passwordText -match '[\x00-\x1f]') {
    throw 'Enter a nonempty password without control characters.'
}

# SQL 从标准输入传入，密码不放到命令参数中。
function Invoke-ProjectSql([string]$Account, [string]$Secret, [string]$Sql) {
    $info = [Diagnostics.ProcessStartInfo]::new($mysql)
    $info.UseShellExecute = $false
    $info.CreateNoWindow = $true
    $info.RedirectStandardInput = $true
    $info.RedirectStandardOutput = $true
    $info.RedirectStandardError = $true
    $info.Arguments = "--no-defaults --protocol=TCP --host=127.0.0.1 --port=$Port --user=$Account --connect-timeout=2 --batch --skip-column-names --default-character-set=utf8mb4"
    $info.EnvironmentVariables['MYSQL_PWD'] = $Secret
    $process = [Diagnostics.Process]::Start($info)
    $outputTask = $process.StandardOutput.ReadToEndAsync()
    $errorTask = $process.StandardError.ReadToEndAsync()
    $sqlBytes = [Text.Encoding]::UTF8.GetBytes($Sql + [Environment]::NewLine)
    $process.StandardInput.BaseStream.Write($sqlBytes, 0, $sqlBytes.Length)
    $process.StandardInput.Close()
    if (-not $process.WaitForExit(10000)) {
        $process.Kill()
        throw 'The MySQL client did not finish in time.'
    }
    return [pscustomobject]@{
        ExitCode = $process.ExitCode
        Output = $outputTask.Result.Trim()
        Error = $errorTask.Result.Replace($passwordText, '[hidden]').Replace($passwordText.Replace("'", "''"), '[hidden]').Replace($adminPassword, '[hidden]')
    }
}

$dataDirectory = Join-Path $RuntimeDirectory 'data'
$settingsFile = Join-Path $RuntimeDirectory 'instance.json'
$privateDirectory = Join-Path $RuntimeDirectory 'private'
$adminFile = Join-Path $privateDirectory 'admin-password.txt'
$bootstrapFile = Join-Path $privateDirectory 'bootstrap.sql'
$serverLog = Join-Path $RuntimeDirectory 'mysql.err'
$pidFile = Join-Path $RuntimeDirectory 'mysql.pid'
$marker = 'IndustrialDeviceMonitor-project-mysql-v1'
$sourceId = [Guid]::NewGuid().ToString()
if (Test-Path -LiteralPath $RuntimeDirectory) {
    if (-not (Test-Path -LiteralPath $settingsFile)) {
        throw 'The runtime directory already exists without a project marker. Choose a new empty directory.'
    }
    $settings = Get-Content -LiteralPath $settingsFile -Raw | ConvertFrom-Json
    if ($settings.Marker -ne $marker -or $settings.Port -ne $Port) {
        throw 'This runtime directory belongs to another instance or port.'
    }
    $sourceId = $settings.SourceId
} else {
    New-Item -ItemType Directory -Path $RuntimeDirectory | Out-Null
    [pscustomobject]@{Marker=$marker;Port=$Port;SourceId=$sourceId} |
        ConvertTo-Json | Set-Content -LiteralPath $settingsFile -Encoding Ascii
}

# 管理密码单独随机生成，只允许当前 Windows 用户和 SYSTEM 访问。
# 使用文件权限保护，不依赖出错的 DPAPI；用户输入的 SQL 密码不保存。
[IO.Directory]::CreateDirectory($privateDirectory) | Out-Null
$privateAcl = [Security.AccessControl.DirectorySecurity]::new()
$privateAcl.SetAccessRuleProtection($true, $false)
$currentIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
foreach ($allowedSid in @($currentIdentity.User, [Security.Principal.SecurityIdentifier]::new('S-1-5-18'))) {
    $rule = [Security.AccessControl.FileSystemAccessRule]::new(
        $allowedSid, 'FullControl', 'ContainerInherit, ObjectInherit', 'None', 'Allow')
    [void]$privateAcl.AddAccessRule($rule)
}
# 只设置访问权限，避免 Set-Acl 重复运行时要求额外的审计权限。
if ($PSVersionTable.PSEdition -eq 'Desktop') {
    [IO.Directory]::SetAccessControl($privateDirectory, $privateAcl)
} else {
    [IO.FileSystemAclExtensions]::SetAccessControl([IO.DirectoryInfo]::new($privateDirectory), $privateAcl)
}
if (Test-Path -LiteralPath $adminFile) {
    $adminPassword = [IO.File]::ReadAllText($adminFile).Trim()
    if ($adminPassword -notmatch '^Admin-[a-f0-9]{32}!8a$') { throw 'The local administrator credential file is invalid.' }
} else {
    $adminPassword = 'Admin-' + [Guid]::NewGuid().ToString('N') + '!8a'
    [IO.File]::WriteAllText($adminFile, $adminPassword, [Text.Encoding]::ASCII)
}

function Start-ProjectServer {
    # 启动时恢复项目管理账号，兼容旧的加密文件已无法读取的情况。
    [IO.File]::WriteAllLines($bootstrapFile, [string[]]@(
        "SET SESSION sql_mode = 'NO_BACKSLASH_ESCAPES';",
        "ALTER USER 'root'@'localhost' IDENTIFIED BY '$adminPassword';"
    ), [Text.UTF8Encoding]::new($false))
    Write-Host "Starting the project MySQL on 127.0.0.1:$Port..."
    return Start-Process -FilePath $mysqld -ArgumentList @(
        '--no-defaults', '--no-monitor', "--basedir=$mysqlBase", "--datadir=$dataDirectory",
        "--port=$Port", '--bind-address=127.0.0.1', '--mysqlx=OFF', '--skip-log-bin',
        '--innodb-buffer-pool-size=64M', "--pid-file=$pidFile", "--log-error=$serverLog",
        "--init-file=$bootstrapFile"
    ) -WindowStyle Hidden -PassThru
}

function Get-VerifiedProjectProcess {
    # 恢复旧实例前核对 PID、程序路径和启动时间，避免关闭其他 MySQL。
    if (-not (Test-Path -LiteralPath $pidFile)) { throw 'The project PID file was not found; no running server was changed.' }
    $serverId = [int]([IO.File]::ReadAllText($pidFile).Trim())
    $server = Get-Process -Id $serverId -ErrorAction Stop
    $pidWritten = (Get-Item -LiteralPath $pidFile).LastWriteTimeUtc
    $startedAt = $server.StartTime.ToUniversalTime()
    if ($server.ProcessName -ne 'mysqld' -or $server.Path -ine $mysqld -or
        $pidWritten -lt $startedAt -or $pidWritten -gt $startedAt.AddMinutes(1)) {
        throw 'The running process could not be verified as this project instance; no server was changed.'
    }
    return $server
}

if (-not (Test-Path -LiteralPath (Join-Path $dataDirectory 'auto.cnf'))) {
    if ((Test-Path -LiteralPath $dataDirectory) -and @(Get-ChildItem -LiteralPath $dataDirectory -Force).Count) {
        throw 'The data directory contains an incomplete initialization. Keep it for diagnosis and choose a new directory.'
    }
    Write-Host 'Preparing the project database. This normally takes several seconds...'
    $initializeLog = Join-Path $RuntimeDirectory 'initialize.err'
    & $mysqld --no-defaults --initialize-insecure "--basedir=$mysqlBase" "--datadir=$dataDirectory" "--log-error=$initializeLog"
    if ($LASTEXITCODE -ne 0) { throw "Initialization failed. Check $initializeLog" }
}

# Verify the server UUID before making account changes.
$uuidLine = Get-Content -LiteralPath (Join-Path $dataDirectory 'auto.cnf') | Where-Object { $_ -match '^server-uuid=' }
$expectedUuid = ($uuidLine -split '=', 2)[1].Trim()
$probe = [Net.Sockets.TcpClient]::new()
try {
    $connecting = $probe.ConnectAsync('127.0.0.1', $Port)
    try { $listening = $connecting.Wait(500) -and $probe.Connected } catch { $listening = $false }
} finally { $probe.Dispose() }

$startedProcess = $null
$setupSucceeded = $false
try {
if (-not $listening) {
    $startedProcess = Start-ProjectServer
}

$ready = $false
$loginSecret = $adminPassword
for ($attempt = 0; $attempt -lt 20; $attempt++) {
    $loginSecret = $adminPassword
    $result = Invoke-ProjectSql 'root' $adminPassword 'SELECT @@server_uuid;'
    if ($result.ExitCode -ne 0 -and $result.Error -match 'ERROR 1045') {
        # 兼容旧脚本只初始化、尚未设置管理密码的实例。
        $result = Invoke-ProjectSql 'root' '' 'SELECT @@server_uuid;'
        $loginSecret = ''
        if ($result.ExitCode -ne 0 -and $result.Error -match 'ERROR 1045' -and -not $startedProcess) {
            Write-Host 'Recovering the project administrator account. Existing tables will be kept.'
            $oldServer = Get-VerifiedProjectProcess
            Stop-ProjectServer $oldServer
            $startedProcess = Start-ProjectServer
            continue
        }
    }
    if ($result.ExitCode -eq 0) {
        if ($result.Output -ne $expectedUuid) { throw 'The port belongs to a different MySQL instance. Account setup was stopped.' }
        $ready = $true
        break
    }
    if ($startedProcess -and $startedProcess.HasExited) { break }
    Start-Sleep -Milliseconds 500
}
if (-not $ready) { throw "The project MySQL did not become ready. Check $serverLog" }
if (Test-Path -LiteralPath $bootstrapFile) { Remove-Item -LiteralPath $bootstrapFile -Force }

$rootLiteral = $adminPassword.Replace("'", "''")
$userLiteral = $passwordText.Replace("'", "''")
# 明确按 UTF-8 读取，避免旧 PowerShell 把中文注释和下一行混在一起。
$schema = [IO.File]::ReadAllText((Join-Path $projectDirectory 'sql\schema.sql'), [Text.Encoding]::UTF8)
$sql = @"
SET SESSION sql_mode = 'NO_BACKSLASH_ESCAPES';
ALTER USER 'root'@'localhost' IDENTIFIED BY '$rootLiteral';
$schema
CREATE USER IF NOT EXISTS '$UserName'@'127.0.0.1' IDENTIFIED BY '$userLiteral';
ALTER USER '$UserName'@'127.0.0.1' IDENTIFIED BY '$userLiteral';
GRANT ALL PRIVILEGES ON device_monitor.* TO '$UserName'@'127.0.0.1';
"@
$result = Invoke-ProjectSql 'root' $loginSecret $sql
if ($result.ExitCode -ne 0) { throw ('Account setup failed: ' + $result.Error) }
$result = Invoke-ProjectSql $UserName $passwordText 'SELECT CURRENT_USER(), @@port; SELECT COUNT(*) FROM device_monitor.temperature_readings;'
if ($result.ExitCode -ne 0) { throw ('New account verification failed: ' + $result.Error) }
Write-Host 'New SQL account created. Login and project-table access verified.'
$setupSucceeded = $true

# Generate a DataGrip project. Only the username is saved here.
$gripProject = Join-Path $RuntimeDirectory 'DataGripProject'
$idea = Join-Path $gripProject '.idea'
New-Item -ItemType Directory -Force -Path $idea | Out-Null
$sharedXml = @"
<project version="4">
  <component name="DataSourceManagerImpl" format="xml" multifile-model="true">
    <data-source source="LOCAL" name="Device Monitor MySQL ($Port)" uuid="$sourceId">
      <driver-ref>mysql.8</driver-ref>
      <synchronize>true</synchronize>
      <jdbc-driver>com.mysql.cj.jdbc.Driver</jdbc-driver>
      <jdbc-url>jdbc:mysql://127.0.0.1:$Port/device_monitor</jdbc-url>
      <working-dir>$gripProject</working-dir>
    </data-source>
  </component>
</project>
"@
$localXml = @"
<project version="4">
  <component name="dataSourceStorageLocal">
    <data-source name="Device Monitor MySQL ($Port)" uuid="$sourceId">
      <user-name>$UserName</user-name>
    </data-source>
  </component>
</project>
"@
[IO.File]::WriteAllText((Join-Path $idea 'dataSources.xml'), $sharedXml, [Text.UTF8Encoding]::new($false))
[IO.File]::WriteAllText((Join-Path $idea 'dataSources.local.xml'), $localXml, [Text.UTF8Encoding]::new($false))
$learningSql = @(
    '-- Run in the Device Monitor MySQL connection.'
    'SELECT CURRENT_USER(), DATABASE();'
    'SELECT id, device_id, temperature, received_at FROM device_monitor.temperature_readings ORDER BY id DESC LIMIT 10;'
) -join [Environment]::NewLine
[IO.File]::WriteAllText((Join-Path $gripProject 'learning.sql'), $learningSql, [Text.UTF8Encoding]::new($false))
Write-Host "MySQL ready: 127.0.0.1:$Port / device_monitor / $UserName"
if (-not $SkipDataGrip -and -not $OpenConsole) {
    Write-Host 'In DataGrip, enter the password you just chose and click Test Connection.'
    Write-Host "Project folder: $gripProject"
    if (-not (Test-Path -LiteralPath $dataGrip)) { throw 'DataGrip was not found at its known installation path. Open the project folder manually.' }
    Start-Process -FilePath $dataGrip -ArgumentList @($gripProject) | Out-Null
}
$passwordText = $null
$adminPassword = $null
$sql = $null

if ($OpenConsole) {
    # 进入 SQL 控制台；密码由 MySQL 自己询问，不写入命令参数。
    Write-Host 'Enter the same SQL password once more. Then type SQL after mysql>.'
    & $mysql --no-defaults --protocol=TCP --host=127.0.0.1 "--port=$Port" "--user=$UserName" --password --database=device_monitor --default-character-set=utf8mb4
    if ($LASTEXITCODE -ne 0) { throw 'The SQL console could not log in. Run the launcher again and check your input.' }
}
} finally {
    if (-not $setupSucceeded -and $startedProcess -and -not $startedProcess.HasExited) {
        try { Stop-ProjectServer $startedProcess }
        catch { Write-Warning 'The project instance is still running. No process was forcibly terminated.' }
    }
    if (Test-Path -LiteralPath $bootstrapFile) { Remove-Item -LiteralPath $bootstrapFile -Force }
    $passwordText = $null
    $adminPassword = $null
    $sql = $null
}
