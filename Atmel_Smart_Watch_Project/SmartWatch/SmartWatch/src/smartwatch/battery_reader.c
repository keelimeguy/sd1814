/* UConn Senior Design Team 1814, January 2018
*/

#include "battery_reader.h"

static struct adc_module adc_instance;
static int last_max;
static uint16_t adc_result, battery_level;
static uint8_t adc_active;

void battery_reader_init(void) {
    struct adc_config config_adc;
    adc_get_config_defaults(&config_adc);

    // If resolution changes, be sure to change MAX_ADC in battery_reader.h
    config_adc.resolution = ADC_RESOLUTION_12BIT;
    config_adc.positive_input = ADC_POSITIVE_INPUT_BANDGAP;

    adc_init(&adc_instance, ADC, &config_adc);
    adc_enable(&adc_instance);
    adc_start_conversion(&adc_instance);

    last_max = 0;
    adc_result = 0;
    adc_active = 1;
    battery_level = 0;

    #if DEBUG_MODE==DEBUG_BATTERY
        port_pin_set_output_level(BOARD_DEBUG_LED, false);
    #endif
}

void battery_task(void) {
    uint16_t result;
    if (adc_active && adc_read(&adc_instance, &result) != STATUS_OK) {

        #if DEBUG_MODE==DEBUG_BATTERY
            port_pin_set_output_level(BOARD_DEBUG_LED, false);
        #endif
        adc_result = result;
        battery_level = adc_result*last_max/MAX_ADC;
        // battery_level = (((1100L * 1024L) / result) + 5L) / 10L;
        adc_active = 0;
    }
}

void start_battery_read(void) {
    if (!adc_active) {
        adc_active = 1;
        adc_start_conversion(&adc_instance);
        #if DEBUG_MODE==DEBUG_BATTERY
            port_pin_set_output_level(BOARD_DEBUG_LED, true);
        #endif
    }
}

uint8_t is_battery_active(void) {
    return adc_active;
}

int get_battery_level(int max) {
    if (max == last_max) {
        return battery_level;
    }
    last_max = max;
    battery_level = adc_result*max/MAX_ADC;
    // battery_level = (((1100L * 1024L) / adc_result) + 5L) / 10L;
    return battery_level;
}
