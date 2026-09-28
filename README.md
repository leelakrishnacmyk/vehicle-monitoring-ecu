# Vehicle Monitoring ECU

A modular C-based embedded-systems simulation of a vehicle monitoring ECU. The project models sensor acquisition, ADC conversion, fault management, ECU state handling, UART diagnostics, watchdog supervision, periodic scheduling, and simulated safe-state outputs.

> **Scope:** This is a desktop firmware simulation, not production automotive firmware. The interfaces are intentionally structured so the desktop HAL/sensor simulator can be replaced by microcontroller peripherals later.

## What it demonstrates

- Modular C firmware architecture with `.h` interfaces and separate implementation files
- 12-bit ADC conversion with input clamping and rounded conversion
- 6:1 battery voltage divider model so a 16 V battery input remains below a 3.3 V ADC reference
- Temperature, battery-voltage, and engine-RPM monitoring
- Centralized configuration for thresholds and timing
- Fault bitmasks using `uint32_t`
- Fault debounce and latching
- Hysteresis-based fault clearing
- Sensor endpoint validity checks
- Normal / Warning / Fault state handling
- Simulated safe-state output disable/enable
- UART diagnostic abstraction
- Watchdog timeout detection with a reset-request flag
- Fixed-period scheduling based on a tick target rather than processing-time delay
- Separate sensor simulation from the sensor driver interface
- Automated boundary, state, ADC/sensor, and watchdog tests

## Architecture

```text
                    +----------------------+
                    |    Sensor Simulator  |
                    +----------+-----------+
                               |
                               v
                    +----------------------+
                    |   Sensor Driver      |
                    | validation + ADC     |
                    +----------+-----------+
                               |
                               v
                    +----------------------+
                    |   Fault Manager      |
                    | debounce + latch +   |
                    | hysteresis + state   |
                    +----------+-----------+
                               |
                +--------------+--------------+
                |                             |
                v                             v
       +----------------+           +----------------+
       |   ECU State    |           | UART Diagnostics|
       | Safe-state I/O |           +----------------+
       +----------------+
                |
                v
       +----------------+
       |    Watchdog    |
       +----------------+
                ^
                |
       +----------------+
       |      HAL       |
       | tick + delay   |
       +----------------+
```

## ECU states

```text
NORMAL
  |
  | warning condition
  v
WARNING
  |
  | debounced fault
  v
FAULT ---> SAFE STATE (outputs disabled)
  |
  | defined clear/hysteresis condition
  v
NORMAL
```

A fault does not disappear because of one immediately-good sample. Threshold hysteresis is used for physical faults, while sensor-validity faults clear when the sensor becomes valid again.

## Monitored conditions

| Signal | Warning | Fault | Clear / hysteresis |
|---|---:|---:|---:|
| Temperature | >= 110 C | >= 130 C | <= 125 C |
| Battery voltage | <= 12.0 V or >= 14.5 V | <= 11.0 V or >= 15.0 V | 11.5 V / 14.5 V |
| Engine RPM | >= 5000 | >= 6000 | <= 5500 |

Faults are debounced over two consecutive samples before being latched.

## ADC and battery-divider model

The simulated ADC is 12-bit with a 3.3 V reference. Battery voltage is scaled through a 6:1 divider:

```text
16.0 V battery
      |
      | / 6
      v
  2.67 V ADC input
      |
      v
  12-bit ADC
```

ADC conversion clamps values to the legal `0..4095` range and rounds instead of truncating. This avoids a simulated over-voltage input being represented by an impossible ADC code above the ADC maximum.

## Sensor validity

ADC endpoint values are treated as invalid sensor indications in the simulation:

- ADC `0` -> invalid sensor reading
- ADC `4095` -> invalid/saturated sensor reading

The sensor layer reports validity separately from the converted value so the fault manager can distinguish a real measured fault from a sensor fault.

## Watchdog

The watchdog is initialized with a 1000 ms timeout. The main loop checks the watchdog before starting a new cycle and kicks it only after the sensor read, fault processing, state update, diagnostics, and cycle work complete.

A separate test uses a simulated timestamp to force a timeout. On timeout the watchdog raises a reset-required flag and calls the system reset-request abstraction.

## Timing

The ECU uses a 100 ms target cycle. Instead of simply sleeping for 100 ms after each iteration, the scheduler advances a `next_wake` target, reducing drift caused by processing time.

Platform-specific timing is isolated in `hal.c` behind:

```c
uint32_t hal_tick_ms(void);
void hal_delay_ms(uint32_t milliseconds);
```

A real MCU port can replace the HAL implementation with a hardware timer/tick source without changing the ECU logic.

## Project structure

```text
vehicle-monitoring-ecu/
|
├── include/
│   ├── adc.h
│   ├── config.h
│   ├── ecu_state.h
│   ├── ecu_types.h
│   ├── fault_manager.h
│   ├── hal.h
│   ├── outputs.h
│   ├── sensor.h
│   ├── sensor_sim.h
│   ├── system.h
│   ├── timer.h
│   ├── uart.h
│   └── watchdog.h
|
├── src/
│   ├── adc.c
│   ├── ecu_state.c
│   ├── fault_manager.c
│   ├── hal.c
│   ├── main.c
│   ├── outputs.c
│   ├── sensor.c
│   ├── sensor_sim.c
│   ├── system.c
│   ├── timer.c
│   ├── uart.c
│   └── watchdog.c
|
├── tests/
│   ├── test_adc_sensor.c
│   ├── test_ecu_state.c
│   ├── test_fault_manager.c
│   └── test_watchdog.c
|
├── build.bat
├── test.bat
├── .gitattributes
├── .gitignore
└── README.md
```

## Build

Requirements:

- GCC / MinGW
- Windows for the included desktop batch scripts

Build and run the ECU simulation:

```powershell
.\build.bat
```

The build uses C99 plus `-Wall -Wextra -Wpedantic` so common mistakes are surfaced during compilation.

## Tests

Run the complete test suite:

```powershell
.\test.bat
```

The suite covers:

- Fault-threshold boundary conditions
- Warning-threshold boundary conditions
- Fault debounce and latching
- Hysteresis-based clearing
- Multiple simultaneous faults
- ECU state transitions
- Safe-state output control
- ADC conversion and 16 V battery round-trip
- Sensor validity detection
- Watchdog timeout simulation

## Example behavior

A normal cycle looks like:

```text
--- ECU CYCLE 1 ---
[UART] Temperature: 90.0
[UART] Battery Voltage: 13.8
[UART] Engine RPM: 2500
[UART] Sensor validity checked.
[UART] ECU State:
[UART] NORMAL
[UART] Faults: No Fault
[UART] System operating normally.
```

A debounced fault eventually produces:

```text
[UART] ECU State:
[UART] FAULT
[UART] Faults:
  - Engine Over Temperature
[UART] FAULT DETECTED!
[UART] SAFE STATE ACTIVE: outputs disabled.
```

## Technologies

- C99
- GCC / MinGW
- Embedded systems architecture
- ADC and sensor conversion
- UART abstraction
- Watchdog supervision
- State machines
- Fault bitmasks
- Debounce and hysteresis
- Hardware-abstraction layer concepts
- Modular testing
