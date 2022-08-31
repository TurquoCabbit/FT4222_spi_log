@echo off

REM Setup variables
set WIN_MINGW_32_PATH=C:\Program Files (x86)\mingw-w64\i686-7.3.0-posix-dwarf-rt_v5-rev0\mingw32\bin\
set PATH=C:\Windows\System32;C:\Windows;%WIN_MINGW_32_PATH%;%WIN_MINGW_32_PATH%\..\opt\bin;
path

set SCRIPT_PATH=%~dp0
set RELEASE_PATH=%SCRIPT_PATH%..\release
set BUILD_PATH=%SCRIPT_PATH%..\_Build
set OUT_PATH=%SCRIPT_PATH%..\_Out

REM Remove binaries if any
call :__CLEAN

@REM mingw32-make clean SHELL=cmd

mingw32-make -j8 all SHELL=cmd


IF %ERRORLEVEL% EQU 0 (
    if not exist %RELEASE_PATH% (
        mkdir %RELEASE_PATH%
    )

    copy %OUT_PATH%\FT4222.exe %RELEASE_PATH%\ft4222_spiLog.exe
    copy %OUT_PATH%\LibFT4222.dll %RELEASE_PATH%\LibFT4222.dll
    
    goto __EXIT_OK
)

goto __EXIT_FAIL



REM ==============================================
:__EXIT_OK
echo ok
pause
exit 0
goto :EOF

REM ==============================================
:__EXIT_FAIL
echo fail
pause
exit 1
goto :EOF

REM ==============================================
:__CLEAN
setlocal

echo.
echo %DATE% %TIME% ======= remove output file
echo.

if exist %RELEASE_PATH% (
    rd /s/q %RELEASE_PATH%
)

if exist %BUILD_PATH% (
    rd /s/q %BUILD_PATH%
)

if exist %OUT_PATH% (
    rd /s/q %OUT_PATH%
)

endlocal
goto :EOF