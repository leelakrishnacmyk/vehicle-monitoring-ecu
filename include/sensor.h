#ifndef SENSOR_H
#define SENSOR_H

void set_scenario(int scenario);

float read_temperature(void);
float read_battery_voltage(void);
int read_rpm(void);

#endif
