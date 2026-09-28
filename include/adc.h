#ifndef ADC_H
#define ADC_H

#define ADC_MAX_VALUE 4095.0f
#define ADC_REFERENCE_VOLTAGE 3.3f

void adc_init(void);

float adc_to_voltage(int adc_value);

int temperature_to_adc(float temperature);

int battery_voltage_to_adc(float voltage);

#endif
