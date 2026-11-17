@echo off
rem Start the existing project database before opening its DataGrip project.
if not exist "D:\datagrip\DataGrip 2025.3.5\bin\datagrip64.exe" (
    echo DataGrip was not found at its configured installation path.
    pause
    exit /b 1
)
call "%~dp0start-project-mysql.cmd"
if errorlevel 1 exit /b 1
start "" "D:\datagrip\DataGrip 2025.3.5\bin\datagrip64.exe" "%~dp0..\.local-mysql\DataGripProject"
