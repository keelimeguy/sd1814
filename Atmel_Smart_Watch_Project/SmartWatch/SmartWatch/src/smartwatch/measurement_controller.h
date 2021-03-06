/* UConn Senior Design Team 1814, January 2018
*/

#ifndef _MEASUREMENT_CONTROLLER_H
#define _MEASUREMENT_CONTROLLER_H

#include "../SmartWatch.h"

#define PHOTODIODE_EIC       BOARD_EIC_PHOTODIODE

#define LED_PIN              BOARD_LED_PIN

#define LONGEST_FREQ_PERIOD_us      125 // us

// glucose = M_STEP_CONVERSION(freq-BASE_FREQ)+BASE_GLUCOSE
#define BASE_FREQ         1000.0f
#define BASE_GLUCOSE      80
#define M_STEP_CONVERSION 0.005f // glucose will change by M_STEP_CONVERSION per Hz

// Assuming 8MHz clock frequency => .125us
#define microsecondsToClockCycles(usecs)    (  usecs / 0.125 )
#define clockCyclesToMicroseconds(cycles)   ( cycles * 0.125 )

void measurement_controller_init(void);
void take_measurement(void);
uint8_t is_measure_busy(void);
uint16_t get_measurement(void);
uint32_t get_measurement_continuous(void);
float get_freq(void);
uint8_t is_new_measurement_soft(void);
uint8_t is_new_measurement(void);
void measure_set_reading_timeout(uint16_t timeout);
uint16_t measure_get_reading_timeout(void);
void measure_set_pulse_one(uint16_t pulse);
void measure_set_pulse_two(uint16_t pulse);
void measurement_task(void);

#endif
