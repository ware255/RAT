@echo off
echo コンパイル中

mkdir bin\bot > nul 2>&1
mkdir bin\cnc > nul 2>&1
g++ bot/main.cpp -L. -static -lwsock32 -lws2_32 -o bin/bot/game.exe
g++ cnc/main.cpp -L. -static -lwsock32 -lws2_32 -o bin/cnc/cnc.exe

echo.
echo コンパイル終了
pause