@echo off

REM Setup variables
set WIN_MINGW_32_PATH=C:\Program Files (x86)\mingw-w64\i686-7.3.0-posix-dwarf-rt_v5-rev0\mingw32\bin\
set PATH=C:\Windows\System32;C:\Windows;%WIN_MINGW_32_PATH%;%WIN_MINGW_32_PATH%\..\opt\bin;
path

set SCRIPT_PATH=%~dp0
set RELEASE_PATH=%SCRIPT_PATH%..\release
set BUILD_PATH=%SCRIPT_PATH%..\_Build

REM Remove binaries if any
call :__CLEAN

@REM mingw32-make clean SHELL=cmd

mingw32-make all SHELL=cmd


IF %ERRORLEVEL% EQU 0 (

    mkdir %RELEASE_PATH%\msvc
    copy %SCRIPT_PATH%..\imports\msvc\msvcp100.dll %RELEASE_PATH%\msvc\msvcp100.dll /y
    copy %SCRIPT_PATH%..\imports\msvc\msvcr100.dll %RELEASE_PATH%\msvc\msvcr100.dll /y

    echo "Copy msvcp100.dll and msvcr100.dll in msvc folder into C:\Windows\System32" >> %RELEASE_PATH%\readme.txt

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

endlocal
goto :EOF