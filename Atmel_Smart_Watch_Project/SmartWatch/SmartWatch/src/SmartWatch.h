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
    #include "smartwatch/display_manager.h"
    #define port_get_config_defaults(A)
    #define port_pin_set_config(A,B)
    #define port_pin_set_output_level(A,B)
    #define BOARD_DISP_BACKLIGHT_PIN 0
    #define PORT_PIN_DIR_OUTPUT 0
    #define BUTTON_L_VAL 1
    #define BUTTON_R_VAL 2
    #define true 1
    #define false 0
    static inline uint8_t is_new_measurement() {return 0;}
    static inline uint8_t get_measurement() {return 0;}
    static inline uint8_t bt_amt_notifications() {return 2;}
    static inline uint8_t bt_connection_state() {return 1;}
    static inline void bt_clear_amt_notifications() {}
    static inline char* bt_get_notification_1() {return "1";}
    static inline char* bt_get_notification_2() {return "2";}
    static inline void rtc_get_time(struct rtc_calendar_time *time) {
        time->second = 23;
        time->minute = 6;
        time->hour = 8;
        time->pm = 1;
        time->day = 7;
        time->month = 12;
        time->year = 2017;
    }
    static inline char* calendar_day_str(char* str, struct rtc_calendar_time *time) {
        str = "Thu";
        return str;
    }
#endif

#endif
