/* UConn Senior Design Team 1814, November 2017
*/

#ifndef CLOCK_DRIVER_H
#define CLOCK_DRIVER_H

#include <SmartWatch.h>

// If this changes, also change next_alarm() in clock_driver.c for appropriate unit
#define RTC_ALARM_UNIT_MASK RTC_CALENDAR_ALARM_MASK_SEC
#define READING_TIMEOUT 7 // s
#define SCREEN_TIMEOUT 6000 // ms

#define RTC_YEAR_INIT  2017
#define RTC_MONTH_INIT 1
#define RTC_DAY_INIT   1
#define RTC_PM_INIT    0
#define RTC_HOUR_INIT  0
#define RTC_MIN_INIT   0
#define RTC_SEC_INIT   4

void clock_driver_init(void);
uint8_t is_reading_timeout(void);
uint8_t is_reading_timeout_soft(void);
void rtc_get_time(struct rtc_calendar_time *time);
void rtc_update_time(struct rtc_calendar_time *time);
char* calendar_day_str(char* str, struct rtc_calendar_time *time);
uint8_t is_screen_timeout(void);
uint8_t is_screen_timeout_soft(void);
void set_screen_timeout(uint32_t val);
uint8_t is_pulse_timeout(void);
uint8_t is_pulse_timeout_soft(void);
void set_pulse_timeout(uint32_t val);

#endif
