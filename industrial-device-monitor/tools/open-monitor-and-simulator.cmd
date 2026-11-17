@echo off
setlocal
rem Use this computer's Qt libraries and the project MySQL certificate.
set "PATH=C:\msys64\mingw64\bin;%PATH%"
set "SSL_CERT_FILE=%~dp0..\.local-mysql\data\ca.pem"

rem Open the already-built monitor and simulator.
if not exist "%~dp0..\build\IndustrialDeviceMonitor.exe" (
    echo Please build IndustrialDeviceMonitor before opening it.
    pause
    exit /b 1
)
if not exist "%~dp0..\build\DeviceSimulator.exe" (
    echo Please build DeviceSimulator before opening it.
    pause
    exit /b 1
)
rem Start an existing project MySQL instance before opening the monitor.
if exist "%~dp0..\.local-mysql\instance.json" (
    powershell.exe -NoProfile -ExecutionPolicy RemoteSigned -File "%~dp0start-project-mysql.ps1"
    if errorlevel 1 echo Project MySQL startup failed. See the error above.
)
start "" "%~dp0..\build\IndustrialDeviceMonitor.exe"
start "" "%~dp0..\build\DeviceSimulator.exe"
