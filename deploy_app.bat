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
set "BIN_DIR=%ROOT_DIR%\bin\%CONFIG%"
set "SOURCE_EXE=%BUILD_DIR%\%CONFIG%\QFileMatrixApp.exe"
set "TARGET_EXE=%BIN_DIR%\QFileMatrixApp.exe"
set "SOURCE_PDB=%BUILD_DIR%\%CONFIG%\QFileMatrixApp.pdb"
set "QT_ROOT=I:\workspace\mylibs\Qt6.6.3\6.6.3\msvc2019_64"
set "WINDEPLOYQT=%QT_ROOT%\bin\windeployqt.exe"
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

call "%ROOT_DIR%\build_app.bat" %CONFIG%
if errorlevel 1 exit /b 1

call "%VCVARS%" >nul
if errorlevel 1 exit /b 1

if not exist "%SOURCE_EXE%" (
  echo Built executable not found: %SOURCE_EXE%
  exit /b 1
)

if exist "%BIN_DIR%" rmdir /s /q "%BIN_DIR%"
mkdir "%BIN_DIR%"

copy /y "%SOURCE_EXE%" "%TARGET_EXE%" >nul
if errorlevel 1 exit /b 1

if exist "%SOURCE_PDB%" copy /y "%SOURCE_PDB%" "%BIN_DIR%\QFileMatrixApp.pdb" >nul

set "DEPLOY_FLAG=--debug"
if /I "%CONFIG%"=="Release" set "DEPLOY_FLAG=--release"

"%WINDEPLOYQT%" %DEPLOY_FLAG% --dir "%BIN_DIR%" "%TARGET_EXE%"
exit /b %errorlevel%
