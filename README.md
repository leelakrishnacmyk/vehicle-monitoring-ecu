# Vehicle Monitoring ECU

A C-based embedded systems simulation of a Vehicle Monitoring Electronic Control Unit (ECU).

The project simulates sensor acquisition, ADC processing, fault detection, ECU state management, UART diagnostics, periodic monitoring, and watchdog functionality.

## Features

- Simulated vehicle sensors
- ADC-based sensor conversion
- Engine temperature monitoring
- Battery voltage monitoring
- Engine RPM monitoring
- Fault detection using bitmasks
- ECU state machine
- Safe-state handling
- UART diagnostic output
- Periodic 100 ms monitoring cycle
- Watchdog timer simulation
- Automated fault detection tests
- ECU state transition tests

## System Architecture

```text
             Vehicle Sensors
                   |
                   v
              ADC / Sensors
                   |
                   v
             Fault Manager
                   |
          +--------+--------+
          |                 |
          v                 v
     ECU State          Fault Status
      Machine                |
          |                  |
          +--------+---------+
                   |
                   v
            UART Diagnostics
                   |
                   v
             Watchdog Timer
```

## Fault Conditions

The ECU detects:

- Engine over-temperature
- Battery under-voltage
- Battery over-voltage
- Engine over-speed
- Multiple simultaneous faults

## ECU States

```text
NORMAL
   |
   v
WARNING
   |
   v
FAULT
   |
   v
SAFE STATE
```

The simulation returns to `NORMAL` when normal operating conditions are detected.

## Project Structure

```text
vehicle-monitoring-ecu/
|
├── include/
│   ├── adc.h
│   ├── ecu_state.h
│   ├── fault_manager.h
│   ├── sensor.h
│   ├── timer.h
│   ├── uart.h
│   └── watchdog.h
|
├── src/
│   ├── adc.c
│   ├── ecu_state.c
│   ├── fault_manager.c
│   ├── main.c
│   ├── sensor.c
│   ├── timer.c
│   ├── uart.c
│   └── watchdog.c
|
├── tests/
│   ├── test_ecu_state.c
│   └── test_fault_manager.c
|
├── build.bat
├── .gitignore
└── README.md
```

## Build and Run

Requirements:

- GCC / MinGW
- Windows

From the project directory, run:

```powershell
.\build.bat
```

The script compiles the ECU simulation and runs it.

## Run Tests

Fault detection tests:

```powershell
gcc tests/test_fault_manager.c src/fault_manager.c -Iinclude -o test_faults
.\test_faults.exe
```

Expected result: **5/5 tests passed**.

ECU state tests:

```powershell
gcc tests/test_ecu_state.c src/ecu_state.c -Iinclude -o test_ecu_state
.\test_ecu_state.exe
```

Expected result: **4/4 tests passed**.

The tests cover normal operation, warning/fault states, individual faults, multiple simultaneous faults, and state recovery.

## Technologies

- C
- GCC / MinGW
- Embedded Systems Concepts
- ADC
- UART
- Watchdog Timer
- State Machines
- Fault Management
- Modular C Architecture

## Note

This is a desktop simulation of an ECU firmware architecture. The sensor, ADC, UART, timer, and watchdog interfaces model concepts that would normally be connected to microcontroller peripherals on a real embedded target.
