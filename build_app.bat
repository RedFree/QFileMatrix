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
set "BUILD_DIR=%ROOT_DIR%\build"
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

call "%ROOT_DIR%\configure_app.bat"
if errorlevel 1 exit /b 1

call "%VCVARS%" >nul
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --config %CONFIG% --target QFileMatrixApp
exit /b %errorlevel%
