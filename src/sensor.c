#include "sensor.h"
#include "adc.h"
#include "config.h"
#include "sensor_sim.h"

void sensor_set_scenario(uint32_t scenario)
{
    sensor_sim_set_scenario(scenario);
}

void sensor_read_all(SensorData *data)
{
    int temperature_adc;
    int voltage_adc;
    float sensor_voltage;

    if (data == 0)
    {
        return;
    }

    data->temperature_c = sensor_sim_temperature_c();
    data->battery_voltage_v = sensor_sim_battery_voltage_v();
    data->engine_rpm = sensor_sim_engine_rpm();

    temperature_adc = temperature_to_adc(data->temperature_c);
    voltage_adc = battery_voltage_to_adc(data->battery_voltage_v);

    data->temperature_adc = (uint16_t)temperature_adc;
    data->battery_voltage_adc = (uint16_t)voltage_adc;

    /* Endpoint ADC readings are treated as open/short sensor indications. */
    data->temperature_valid =
        (temperature_adc > 0 && temperature_adc < (int)ADC_MAX_VALUE);
    data->battery_voltage_valid =
        (voltage_adc > 0 && voltage_adc < (int)ADC_MAX_VALUE);
    data->rpm_valid = 1U;

    /* Firmware-facing values are reconstructed from raw ADC readings. */
    sensor_voltage = adc_to_voltage(temperature_adc);
    if (data->temperature_valid != 0U)
    {
        data->temperature_c = ((sensor_voltage - 0.5f) / 2.0f) * 150.0f;
    }

    sensor_voltage = adc_to_voltage(voltage_adc);
    if (data->battery_voltage_valid != 0U)
    {
        data->battery_voltage_v = sensor_voltage * BATTERY_DIVIDER_RATIO;
    }
}
