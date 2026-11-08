@echo off
rem Start the project's existing database so DataGrip can connect.
powershell.exe -NoProfile -ExecutionPolicy RemoteSigned -File "%~dp0start-project-mysql.ps1"
if errorlevel 1 (
    pause
    exit /b 1
)
