#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define ECU_CYCLE_MS 100U
#define WATCHDOG_TIMEOUT_MS 1000U

/* Temperature thresholds (Celsius). */
#define TEMP_WARN_C 110.0f
#define TEMP_WARN_CLEAR_C 105.0f
#define TEMP_FAULT_C 130.0f
#define TEMP_CLEAR_C 125.0f

/* Engine speed thresholds (RPM). */
#define RPM_WARN 5000U
#define RPM_WARN_CLEAR 4800U
#define RPM_FAULT 6000U
#define RPM_CLEAR 5500U

/* Battery voltage thresholds (Volts). */
#define VOLTAGE_WARN_LOW 12.0f
#define VOLTAGE_WARN_LOW_CLEAR 12.3f
#define VOLTAGE_WARN_HIGH 14.5f
#define VOLTAGE_WARN_HIGH_CLEAR 14.2f
#define VOLTAGE_UNDER_FAULT 11.0f
#define VOLTAGE_UNDER_CLEAR 11.5f
#define VOLTAGE_OVER_FAULT 15.0f
#define VOLTAGE_OVER_CLEAR 14.5f

/* Debounce filters. */
#define FAULT_SET_DEBOUNCE_SAMPLES 2U
#define FAULT_CLEAR_DEBOUNCE_SAMPLES 2U

/* Hardware scaling and sensor calibration. */
#define BATTERY_DIVIDER_RATIO 6.0f
#define TEMP_SENSOR_V_OFFSET 0.5f
#define TEMP_SENSOR_V_SPAN 2.0f
#define TEMP_SENSOR_MAX_TEMP 150.0f

#endif
