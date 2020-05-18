@ECHO OFF
SET parent=%~dp0
cd %parent%

REM ---- 32 bits paths
SET  QT=C:\Qt\5.12.8\mingw73_32
SET GCC=C:\Qt\Tools\mingw730_32
SET DST_LIB=packages\lib\data\bin
SET DST_BIN=packages\bin\data\bin
SET DST_DOC=packages\doc\data\documentation

echo.
echo === Creating data directories ===
FOR %%S IN (%DST_BIN% %DST_LIB% %DST_LIB%\platforms %DST_DOC%) DO (
    echo %%S
    IF not exist "%%S" ( mkdir "%%S" ) ELSE ( del /Q "%%S\*" )
)

echo.
echo === Preparing Qt* libs ===
FOR %%S IN (Qt5Core Qt5Gui Qt5SerialPort Qt5Widgets)    DO copy "%QT%\bin\%%~S.dll*"                "%DST_LIB%"

echo.
REM Not required for Qt12, but required with Qt15
echo === Preparing Platforms libs ===
FOR %%S IN (qwindows)                                   DO copy "%QT%\plugins\platforms\%%~S.dll*"  "%DST_LIB%\platforms"

echo.
REM Qt15 requires libgcc_s_seh-1 in place of libgcc_s_dw2-1
echo === Preparing Gcc libs ===
FOR %%S IN (libgcc_s_dw2-1 libstdc++-6 libwinpthread-1) DO copy "%GCC%\bin\%%~S.dll*"               "%DST_LIB%"

echo.
echo === Preparing documentation ===
copy ..\doc\*.fodt "%DST_DOC%

echo.
echo === Preparing program ===
copy "..\build\release\ET232_extended.exe" "%DST_BIN%

echo.
echo === Creating the installer... ===
del ET232_extended-installer.exe
C:\Qt\Tools\QtInstallerFramework\3.2\bin\binarycreator.exe --offline-only --ignore-translations --config config\config.xml --packages packages ET232_extended-installer.exe

echo.
echo === Done ===
dir /B ET232_extended-installer.exe
pause
