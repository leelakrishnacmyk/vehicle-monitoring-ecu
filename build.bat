@echo off

echo ====================================
echo   Building Vehicle Monitoring ECU
echo ====================================

gcc src/main.c ^
src/sensor.c ^
src/fault_manager.c ^
src/ecu_state.c ^
src/uart.c ^
src/adc.c ^
src/timer.c ^
src/watchdog.c ^
-Iinclude ^
-o vehicle_monitor.exe

if %errorlevel% neq 0 (
    echo.
    echo BUILD FAILED
    pause
    exit /b 1
)

echo.
echo BUILD SUCCESSFUL
echo.

vehicle_monitor.exe
