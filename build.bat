@echo off
setlocal

if not exist build mkdir build

echo ====================================
echo   Building Vehicle Monitoring ECU
echo ====================================

gcc -std=c99 -Wall -Wextra -Wpedantic -Iinclude ^
src/main.c ^
src/sensor.c ^
src/sensor_sim.c ^
src/fault_manager.c ^
src/ecu_state.c ^
src/outputs.c ^
src/uart.c ^
src/adc.c ^
src/timer.c ^
src/hal.c ^
src/system.c ^
src/watchdog.c ^
-o build\vehicle_monitor.exe

if errorlevel 1 (
    echo.
    echo BUILD FAILED
    exit /b 1
)

echo.
echo BUILD SUCCESSFUL
echo.
build\vehicle_monitor.exe
