@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%.") do set "ROOT_DIR=%%~fI"
set "BUILD_DIR=%ROOT_DIR%\build"
set "QT_ROOT=I:\workspace\mylibs\Qt6.6.3\6.6.3\msvc2019_64"
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

if not exist "%VCVARS%" (
  echo Visual Studio environment script not found: %VCVARS%
  exit /b 1
)

if not exist "%QT_ROOT%\bin\windeployqt.exe" (
  echo Qt deployment tool not found: %QT_ROOT%\bin\windeployqt.exe
  exit /b 1
)

if exist "%BUILD_DIR%\CMakeCache.txt" (
  findstr /C:"CMAKE_GENERATOR:INTERNAL=Visual Studio 17 2022" "%BUILD_DIR%\CMakeCache.txt" >nul
  if errorlevel 1 (
    echo Clearing incompatible CMake cache in %BUILD_DIR%
    if exist "%BUILD_DIR%\CMakeCache.txt" del /f /q "%BUILD_DIR%\CMakeCache.txt"
    if exist "%BUILD_DIR%\CMakeFiles" rmdir /s /q "%BUILD_DIR%\CMakeFiles"
  )
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

call "%VCVARS%" >nul
if errorlevel 1 exit /b 1

cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%" -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=%QT_ROOT%
exit /b %errorlevel%
