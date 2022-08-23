@echo off

make clean SHELL=cmd

make -j8 all SHELL=cmd


IF %ERRORLEVEL% EQU 0 (
    cd _Out
    FT4222.exe
    cd ..
)