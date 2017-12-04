/* UConn Senior Design Team 1814, November 2017
*/

#include "clock_driver.h"
#include "date_calc.h"

static struct rtc_module rtc_instance;
static struct rtc_calendar_alarm_time alarm;
static volatile uint8_t rtc_alarm_flag;

static struct tc_module screen_timer;
static volatile uint32_t screen_timeout;

static void rtc_alarm_callback(void);
static void screen_timer_callback(void);

void clock_driver_init(void) {
    /* Initialize RTC in calendar mode. */

    struct rtc_calendar_time time;
    rtc_calendar_get_time_defaults(&time);
    time.year   = RTC_YEAR_INIT;
    time.month  = RTC_MONTH_INIT;
    time.day    = RTC_DAY_INIT;
    time.hour   = RTC_HOUR_INIT;
    time.minute = RTC_MIN_INIT;
    time.second = RTC_SEC_INIT;

    struct rtc_calendar_config config_rtc_calendar;
    rtc_calendar_get_config_defaults(&config_rtc_calendar);

    alarm.time.year      = time.year;
    alarm.time.month     = time.month;
    alarm.time.day       = time.day;
    alarm.time.hour      = time.hour;
    alarm.time.minute    = time.minute;
    alarm.time.second    = time.second;

    alarm.mask = RTC_CALENDAR_ALARM_MASK_MIN;
    alarm.time.minute += READING_TIMEOUT;
    alarm.time.minute = alarm.time.minute % 60;

    config_rtc_calendar.clock_24h = true;
    config_rtc_calendar.alarm[0].time = alarm.time;

    rtc_calendar_init(&rtc_instance, RTC, &config_rtc_calendar);

    /* Set current time. */
    rtc_calendar_set_time(&rtc_instance, &time);

    rtc_calendar_enable(&rtc_instance);

    rtc_calendar_register_callback(&rtc_instance, rtc_alarm_callback, RTC_CALENDAR_CALLBACK_ALARM_0);
    rtc_calendar_enable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_ALARM_0);

    rtc_alarm_flag = 0;


    /* Initialize TC. */

    struct tc_config config_tc;
    tc_get_config_defaults(&config_tc);

    // Assuming GCLK generator 0 source is 8MHz:
    //  512 prescaler at 15625 compare match => 8e6/15625*512 = 1MHz
    config->clock_prescaler = TC_CLOCK_PRESCALER_DIV512;
    config_tc->counter_16_bit.compare_capture_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = 0x3D08; // 15625-1
    config_tc->wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
    config_tc->enable_capture_on_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = true;

    tc_init(&screen_timer, TC0, &config_tc);
    tc_init_enable(&screen_timer);

    rtc_calendar_register_callback(&screen_timer, screen_timer_callback, TC_CALLBACK_OVERFLOW);
    rtc_calendar_enable_callback(&screen_timer, TC_CALLBACK_OVERFLOW);

    rtc_calendar_register_callback(&screen_timer, screen_timer_callback, TC_CALLBACK_CC_CHANNEL0);
    rtc_calendar_enable_callback(&screen_timer, TC_CALLBACK_CC_CHANNEL0);

    screen_timeout = 1;
}

void rtc_get_time (struct rtc_calendar_time *const time) {
    rtc_calendar_get_time(&rtc_instance, time);
}

static const char** day_str= {"Wed\0", "Thu\0", "Fri\0", "Sat\0", "Sun\0", "Mon\0", "Tue\0"};

char* calendar_day_str(char* str, struct rtc_calendar_time *const time) {
    str = day_str[(int)date_to_day_number(time.year, time.month, time.day)%7];
    return str;
}

uint8_t is_reading_timeout_soft(void) {
        return rtc_alarm_flag;
}

uint8_t is_reading_timeout(void) {
    if (rtc_alarm_flag) {
        rtc_alarm_flag = 0;
        return 1;
    }
    return 0;
}

uint8_t is_screen_timeout_soft(void) {
    return screen_timeout==0;
}

uint8_t is_screen_timeout(void) {
    return screen_timeout==0;
}

void set_screen_timeout(uint32_t val) {
    screen_timeout = val;
    tc_set_count_value(&screen_timer, 0);
    tc_start_counter(&screen_timer);
}

static void screen_timer_callback(void) {
    if (screen_timeout>0) {
        screen_timeout--;
    } else {
        tc_stop_counter(&screen_timer);
    }
}

static void rtc_alarm_callback(void) {
    rtc_alarm_flag = 1;

    /* Set new alarm */
    alarm.time.minute += READING_TIMEOUT;
    alarm.time.minute = alarm.time.minute % 60;

    rtc_calendar_set_alarm(&rtc_instance, &alarm, RTC_CALENDAR_ALARM_0);
}
