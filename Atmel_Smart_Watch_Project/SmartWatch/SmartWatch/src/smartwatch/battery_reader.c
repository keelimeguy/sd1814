/* UConn Senior Design Team 1814, January 2018
*/

#include "battery_reader.h"

static struct adc_module adc_instance;
static int last_max;
static uint16_t adc_result, battery_level;

void battery_reader_init(void) {
    struct adc_config config_adc;
    adc_get_config_defaults(&config_adc);

    // If resolution changes, be sure to change MAX_ADC in battery_reader.h
    config_adc->resolution = ADC_RESOLUTION_8BIT;
    config_adc->positive_input = ADC_POSITIVE_INPUT_BANDGAP;

    adc_init(&adc_instance, ADC, &config_adc);
    adc_enable(&adc_instance);
    adc_start_conversion(&adc_instance);

    last_max = 0;
    adc_result = 0;
    battery_level = 0;
}

void battery_task(void) {
    uint16_t result;
    if (adc_read(&adc_instance, &result) != STATUS_OK) {
        adc_result = result;
        battery_level = adc_result*last_max/MAX_ADC;
        // battery_level = (((1100L * 1024L) / valueRead) + 5L) / 10L;

        // adc_start_conversion(&adc_instance);
    }

}

int get_battery_level(int max) {
    if (max == last_max) {
        return battery_level;
    }
    last_max = max;
    battery_level = adc_result*max/MAX_ADC;
    // battery_level = (((1100L * 1024L) / valueRead) + 5L) / 10L;
    return battery_level;
}
