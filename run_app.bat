@echo off
setlocal

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Debug"

if /I not "%CONFIG%"=="Debug" if /I not "%CONFIG%"=="Release" (
  echo Usage: %~n0 [Debug^|Release]
  exit /b 1
)

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%.") do set "ROOT_DIR=%%~fI"
set "TARGET_EXE=%ROOT_DIR%\bin\%CONFIG%\QFileMatrixApp.exe"

call "%ROOT_DIR%\deploy_app.bat" %CONFIG%
if errorlevel 1 exit /b 1

if not exist "%TARGET_EXE%" (
  echo Runnable executable not found: %TARGET_EXE%
  exit /b 1
)

start "QFileMatrixApp" "%TARGET_EXE%"
exit /b 0
