/* UConn Senior Design Team 1814, January 2018
*/

#include "clock_driver.h"
#include "date_calc.h"

static struct rtc_module rtc_instance;
static struct rtc_calendar_alarm_time alarm;
static volatile uint8_t rtc_alarm_flag;

static struct tcc_module screen_timer, pulse_timer, battery_timer;
static struct tc_module button_timer;
static volatile int16_t screen_timeout, pulse_timeout, battery_timeout, button_timeout;
static const char* day_str[7] = {"Wed", "Thu", "Fri", "Sat", "Sun", "Mon", "Tue"};

static void rtc_alarm_callback(void);
static void screen_timer_callback(struct tcc_module *const module);
static void pulse_timer_callback(struct tcc_module *const module);
static void battery_timer_callback(struct tcc_module *const module);
static void button_timer_callback(struct tc_module *const module);

static inline void next_alarm(void) {
    if (measure_get_reading_timeout() > 0) {
        alarm.time.second += measure_get_reading_timeout();
        while (alarm.time.second >= 60) {
            alarm.time.second -= 60;
            alarm.time.minute++;
            if (alarm.time.minute>=60) {
                alarm.time.minute = 0;
                alarm.time.hour++;
                if (alarm.time.hour > 11) {
                    if (alarm.time.hour == 12) {
                        if (alarm.time.pm) {
                            add_to_date_uchar(1, &(alarm.time.year), &(alarm.time.month), &(alarm.time.day));
                        }
                        alarm.time.pm = !alarm.time.pm;
                    } else
                    alarm.time.hour = 1;
                }
            }
        }
    }
}

void clock_driver_init(void) {
    /* Initialize RTC in calendar mode. */

    struct rtc_calendar_time time;
    rtc_calendar_get_time_defaults(&time);

    struct rtc_calendar_config config_rtc_calendar;
    rtc_calendar_get_config_defaults(&config_rtc_calendar);

    alarm.time.year      = time.year;
    alarm.time.month     = time.month;
    alarm.time.day       = time.day;
    alarm.time.hour      = time.hour;
    alarm.time.pm        = time.pm;
    alarm.time.minute    = time.minute;
    alarm.time.second    = time.second;

    alarm.mask = RTC_ALARM_UNIT_MASK;
    next_alarm();

    config_rtc_calendar.clock_24h = false;
    config_rtc_calendar.alarm[0].time = alarm.time;

    rtc_calendar_init(&rtc_instance, RTC, &config_rtc_calendar);

    /* Set current time. */
    rtc_calendar_set_time(&rtc_instance, &time);

    rtc_calendar_register_callback(&rtc_instance, rtc_alarm_callback, RTC_CALENDAR_CALLBACK_ALARM_0);

    rtc_calendar_enable(&rtc_instance);
    rtc_calendar_enable_callback(&rtc_instance, RTC_CALENDAR_CALLBACK_ALARM_0);

    rtc_alarm_flag = 0;


    /* Initialize TC. */

    struct tcc_config config_tcc;
    tcc_get_config_defaults(&config_tcc, TCC0);

    // Assuming GCLK generator 0 source is 8MHz:
    //  256 prescaler at 31250 compare match => 8e6/31250*256 = 1Hz
    config_tcc.counter.clock_source = GCLK_GENERATOR_0;
    config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV256;
    config_tcc.counter.period = 0x7a11; // 31250-1

    tcc_init(&screen_timer, TCC0, &config_tcc);

    tcc_register_callback(&screen_timer, screen_timer_callback, TCC_CALLBACK_OVERFLOW);
    tcc_enable_callback(&screen_timer, TCC_CALLBACK_OVERFLOW);

    tcc_enable(&screen_timer);

    // Set to -1 to disable trigger
    screen_timeout = -1;

    tcc_get_config_defaults(&config_tcc, TCC1);

    // Assuming GCLK generator 0 source is 8MHz:
    //  256 prescaler at 31250 compare match => 8e6/125*64 = 1kHz
    config_tcc.counter.clock_source = GCLK_GENERATOR_0;
    config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV64;
    config_tcc.counter.period = 0x7c; // 125-1

    tcc_init(&pulse_timer, TCC1, &config_tcc);

    tcc_register_callback(&pulse_timer, pulse_timer_callback, TCC_CALLBACK_OVERFLOW);
    tcc_enable_callback(&pulse_timer, TCC_CALLBACK_OVERFLOW);

    tcc_enable(&pulse_timer);

    // Set to -1 to disable trigger
    pulse_timeout = -1;

    tcc_get_config_defaults(&config_tcc, TCC2);

    // Assuming GCLK generator 0 source is 8MHz:
    //  256 prescaler at 31250 compare match => 8e6/31250*256 = 1Hz
    config_tcc.counter.clock_source = GCLK_GENERATOR_0;
    config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV256;
    config_tcc.counter.period = 0x7a11; // 31250-1

    tcc_init(&battery_timer, TCC2, &config_tcc);

    tcc_register_callback(&battery_timer, battery_timer_callback, TCC_CALLBACK_OVERFLOW);
    tcc_enable_callback(&battery_timer, TCC_CALLBACK_OVERFLOW);

    tcc_enable(&battery_timer);

    // Start battery timeout at 0 to trigger next battery read
    battery_timeout = 0;

    struct tc_config config_tc;
    tc_get_config_defaults(&config_tc);

    // Assuming GCLK generator 0 source is 8MHz:
    //  64 prescaler at 125 compare match => 8e6/125*64 = 1kHz
    config_tc.counter_size =   TC_COUNTER_SIZE_16BIT;
    config_tc.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
    config_tc.clock_source = GCLK_GENERATOR_0;
    config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV64;
    config_tc.counter_16_bit.compare_capture_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = 0x7c; // 125-1

    tc_init(&button_timer, TC3, &config_tc);

    tc_register_callback(&button_timer, button_timer_callback, TC_CALLBACK_CC_CHANNEL0);
    tc_enable_callback(&button_timer, TC_CALLBACK_CC_CHANNEL0);

    tc_enable(&button_timer);

    // Start button timeout at 0 to enable next button press
    button_timeout = 0;
}

void rtc_get_time(struct rtc_calendar_time *time) {
    rtc_calendar_get_time(&rtc_instance, time);
}

void rtc_update_time(struct rtc_calendar_time *time) {
    rtc_calendar_set_time(&rtc_instance, time);
}

char* calendar_day_str(char* str, struct rtc_calendar_time *time) {
    str = day_str[(long)date_to_day_number(time->year, time->month, time->day)%7];
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

void set_screen_timeout(uint16_t val) {
    tcc_stop_counter(&screen_timer);
    screen_timeout = val;
    if (val != 0) {
        tcc_set_count_value(&screen_timer, 0);
        tcc_restart_counter(&screen_timer);
    }
}

static void screen_timer_callback(struct tcc_module *const module) {
    if (screen_timeout>0) {
        screen_timeout--;
    } else {
        tcc_stop_counter(&screen_timer);
    }
}

uint8_t is_pulse_timeout_soft(void) {
    return pulse_timeout==0;
}

uint8_t is_pulse_timeout(void) {
    return pulse_timeout==0;
}

void set_pulse_timeout(uint16_t val) {
    tcc_stop_counter(&pulse_timer);
    pulse_timeout = val;
    if (val != 0) {
        tcc_set_count_value(&pulse_timer, 0);
        tcc_restart_counter(&pulse_timer);
    }
}

static void pulse_timer_callback(struct tcc_module *const module) {
    if (pulse_timeout>0) {
        pulse_timeout--;
    } else {
        tcc_stop_counter(&pulse_timer);
    }
}

uint8_t is_battery_timeout_soft(void) {
    return battery_timeout==0;
}

uint8_t is_battery_timeout(void) {
    return battery_timeout==0;
}

void set_battery_timeout(uint16_t val) {
    tcc_stop_counter(&battery_timer);
    battery_timeout = val;
    if (val != 0) {
        tcc_set_count_value(&battery_timer, 0);
        tcc_restart_counter(&battery_timer);
    }
}

static void battery_timer_callback(struct tcc_module *const module) {
    if (battery_timeout>0) {
        battery_timeout--;
    } else {
        tcc_stop_counter(&battery_timer);
    }
}

uint8_t is_button_timeout_soft(void) {
    return button_timeout==0;
}

uint8_t is_button_timeout(void) {
    return button_timeout==0;
}

void set_button_timeout(uint16_t val) {
    tc_stop_counter(&button_timer);
    button_timeout = val;
    if (val != 0) {
        tc_set_count_value(&button_timer, 0);
        tc_start_counter(&button_timer);
    }
}

static void button_timer_callback(struct tc_module *const module) {
    if (button_timeout>0) {
        button_timeout--;
    } else {
        tc_stop_counter(&button_timer);
    }
}

static void rtc_alarm_callback(void) {
    rtc_alarm_flag = 1;

    /* Trigger measurement */
    if (measure_get_reading_timeout() > 0) {
        take_measurement();
    }

    /* Set new alarm */
    next_alarm();

    rtc_calendar_set_alarm(&rtc_instance, &alarm, RTC_CALENDAR_ALARM_0);
}
