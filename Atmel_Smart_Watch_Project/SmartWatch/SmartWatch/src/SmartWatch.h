/* UConn Senior Design Team 1814, November 2017
*/

#ifndef SMARTWATCH_H
#define SMARTWATCH_H

#ifndef CONSOLE_VERSION

    #include <ASF.h>

    #ifndef PROGMEM
    #define PROGMEM
    #endif

	#define LED_PIN  BOARD_DEBUG_LED

    #include "smartwatch/clock_driver.h"
    #include "smartwatch/display_manager.h"
    #include "smartwatch/button_listener.h"
    #include "smartwatch/bluetooth_driver.h"
    #include "smartwatch/measurement_controller.h"
    #include "smartwatch/kalman.h"

    void init_all(void);
    void smartwatch_task(void);
    uint8_t is_active(void);
    uint8_t is_screen_active(void);
    uint8_t is_screen_active_soft(void);
    void request_screen_on(void);
    void sleep(void);
    void wakeup(void);


// The following is specifically for console testing code,
// e.g. compiled with flag: -D CONSOLE_VERSION=<anything>
#else
    #include <stdint.h>
    #include "smartwatch/bluetooth_driver.h"
    typedef uint8_t bool;
    struct rtc_calendar_time {
        uint8_t  second;
        uint8_t  minute;
        uint8_t  hour;
        bool     pm;
        uint8_t  day;
        uint8_t  month;
        uint16_t year;
    };
    enum port_pin_dir {
        PORT_PIN_DIR_INPUT               = 0,
        PORT_PIN_DIR_OUTPUT              = 1,
        PORT_PIN_DIR_OUTPUT_WTH_READBACK = 2,
    };
    enum port_pin_pull {
        PORT_PIN_PULL_NONE = 0,
        PORT_PIN_PULL_UP   = 1,
        PORT_PIN_PULL_DOWN = 2,
    };
    struct port_config {
        enum port_pin_dir  direction;
        enum port_pin_pull input_pull;
        bool powersave;
    };
    #include "smartwatch/measurement_controller.h"
    #include "smartwatch/display_manager.h"
    #include "smartwatch/date_calc.h"
    #define port_get_config_defaults(A)
    #define port_pin_set_config(A,B)
    #define port_pin_set_output_level(A,B)
    #define BOARD_DISP_BACKLIGHT_PIN 0
    #define PORT_PIN_DIR_OUTPUT 0
    #define BUTTON_L_VAL 1
    #define BUTTON_R_VAL 2
    #define true 1
    #define false 0
    static struct rtc_calendar_time rtc_instance = {
        22,
        22,
        12,
        1,
        27,
        1,
        2018
    };
    static inline void rtc_get_time(struct rtc_calendar_time *time) {
        time->second = rtc_instance.second;
        time->minute = rtc_instance.minute;
        time->hour = rtc_instance.hour;
        time->pm = rtc_instance.pm;
        time->day = rtc_instance.day;
        time->month = rtc_instance.month;
        time->year = rtc_instance.year;
    }
    static inline void rtc_update_time(struct rtc_calendar_time *time) {
        rtc_instance.second = time->second;
        rtc_instance.minute = time->minute;
        rtc_instance.hour = time->hour;
        rtc_instance.pm = time->pm;
        rtc_instance.day = time->day;
        rtc_instance.month = time->month;
        rtc_instance.year = time->year;
    }
    static inline void request_screen_on(void) {}
    static inline void kalman_setT(float val) {}
    static inline void kalman_CGM(float z, float R, unsigned char sensorNum, float x_result[6]) {}
    static inline void set_pulse_timeout(uint32_t val) {}
    static inline bool uart_tx(uint8_t *buffer, uint8_t buffer_len) {}
    static inline void BLEsetup(void) {}
    static const char* day_str[7] = {"Wed", "Thu", "Fri", "Sat", "Sun", "Mon", "Tue"};
    static inline char* calendar_day_str(char* str, struct rtc_calendar_time *time) {
        str = day_str[(int)date_to_day_number(time->year, time->month, time->day)%7];
        return str;
    }
#endif

#endif
