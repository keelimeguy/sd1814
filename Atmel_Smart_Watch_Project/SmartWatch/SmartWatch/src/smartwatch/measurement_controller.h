/* UConn Senior Design Team 1814, January 2018
*/

#ifndef _MEASUREMENT_CONTROLLER_H
#define _MEASUREMENT_CONTROLLER_H

#include <SmartWatch.h>

void measurement_controller_init(void);
void take_measurement(uint8_t button);
uint8_t is_measure_busy(void);
float get_measurement(void);
uint8_t is_new_measurement_soft(void);
uint8_t is_new_measurement(void);
void measure_set_reading_timeout(uint32_t timeout);
uint32_t measure_get_reading_timeout(void);
void measure_set_pulse_one(uint32_t pulse);
void measure_set_pulse_two(uint32_t pulse);
void measure_set_pulse_three(uint32_t pulse);
void measurement_task(void);
void do_kalman_bt_cmd(long calibration);
void do_kalman(unsigned long freq, uint8_t sensorNum);
void cal_kalman(void);

#endif
