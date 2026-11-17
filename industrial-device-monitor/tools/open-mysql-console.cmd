@echo off
rem Create the project account, then open the MySQL command-line client.
powershell.exe -NoProfile -ExecutionPolicy RemoteSigned -File "%~dp0setup-project-mysql.ps1" -SkipDataGrip -OpenConsole
if errorlevel 1 (
    echo Setup failed. Keep this window open to read the error above.
    pause
    exit /b 1
)
