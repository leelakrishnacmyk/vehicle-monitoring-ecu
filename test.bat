@echo off
setlocal

if not exist build mkdir build

echo ====================================
echo   Running ECU Test Suite
echo ====================================

gcc -std=c99 -Wall -Wextra -Wpedantic -Iinclude ^
tests/test_fault_manager.c src/fault_manager.c ^
-o build\test_fault_manager.exe
if errorlevel 1 exit /b 1

build\test_fault_manager.exe
if errorlevel 1 exit /b 1

gcc -std=c99 -Wall -Wextra -Wpedantic -Iinclude ^
tests/test_ecu_state.c src/fault_manager.c src/ecu_state.c src/outputs.c src/uart.c ^
-o build\test_ecu_state.exe
if errorlevel 1 exit /b 1

build\test_ecu_state.exe
if errorlevel 1 exit /b 1

gcc -std=c99 -Wall -Wextra -Wpedantic -Iinclude ^
tests/test_adc_sensor.c src/adc.c src/sensor.c src/sensor_sim.c ^
-o build\test_adc_sensor.exe
if errorlevel 1 exit /b 1

build\test_adc_sensor.exe
if errorlevel 1 exit /b 1

gcc -std=c99 -Wall -Wextra -Wpedantic -Iinclude ^
tests/test_watchdog.c src/hal.c src/watchdog.c src/uart.c ^
-o build\test_watchdog.exe
if errorlevel 1 exit /b 1

build\test_watchdog.exe
if errorlevel 1 exit /b 1

echo.
echo ALL TEST SUITES PASSED
