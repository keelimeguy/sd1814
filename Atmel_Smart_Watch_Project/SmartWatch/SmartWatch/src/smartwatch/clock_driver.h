/* UConn Senior Design Team 1814, January 2018
*/

#ifndef CLOCK_DRIVER_H
#define CLOCK_DRIVER_H

#include "../SmartWatch.h"

// If this changes, also change next_alarm() in clock_driver.c for appropriate unit
#define RTC_ALARM_UNIT_MASK RTC_CALENDAR_ALARM_MASK_YEAR

#define BATTERY_TIMEOUT 20 // s
#define SCREEN_TIMEOUT  120 // s
#define READING_TIMEOUT measure_get_reading_timeout()
#define BUTTON_TIMEOUT 500 // ms

void clock_driver_init(void);
uint8_t is_reading_timeout(void);
uint8_t is_reading_timeout_soft(void);
void rtc_get_time(struct rtc_calendar_time *time);
void rtc_update_time(struct rtc_calendar_time *time);
char* calendar_day_str(char* str, struct rtc_calendar_time *time);
uint8_t is_screen_timeout(void);
uint8_t is_screen_timeout_soft(void);
void set_screen_timeout(uint16_t val);
uint8_t is_pulse_timeout(void);
uint8_t is_pulse_timeout_soft(void);
void set_pulse_timeout(uint16_t val);
uint8_t is_battery_timeout(void);
uint8_t is_battery_timeout_soft(void);
void set_battery_timeout(uint16_t val);
uint8_t is_button_timeout(void);
uint8_t is_button_timeout_soft(void);
void set_button_timeout(uint16_t val);

#endif
