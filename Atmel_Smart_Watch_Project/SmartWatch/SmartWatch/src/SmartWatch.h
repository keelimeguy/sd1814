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

#else
    #include <stdint.h>
    typedef uint8_t bool;
    struct rtc_calendar_time {
        /** Second value */
        uint8_t  second;
        /** Minute value */
        uint8_t  minute;
        /** Hour value */
        uint8_t  hour;
        /** PM/AM value, \c true for PM, or \c false for AM */
        bool     pm;
        /** Day value, where day 1 is the first day of the month */
        uint8_t  day;
        /** Month value, where month 1 is January */
        uint8_t  month;
        /** Year value */
        uint16_t year;
    };
    enum port_pin_dir {
        /** The pin's input buffer should be enabled, so that the pin state can
         *  be read */
        PORT_PIN_DIR_INPUT               = 0,
        /** The pin's output buffer should be enabled, so that the pin state can
         *  be set */
        PORT_PIN_DIR_OUTPUT              = 1,
        /** The pin's output and input buffers should be enabled, so that the pin
         *  state can be set and read back */
        PORT_PIN_DIR_OUTPUT_WTH_READBACK = 2,
    };
    enum port_pin_pull {
        /** No logical pull should be applied to the pin */
        PORT_PIN_PULL_NONE = 0,
        /** Pin should be pulled up when idle */
        PORT_PIN_PULL_UP   = 1,
        /** Pin should be pulled down when idle */
        PORT_PIN_PULL_DOWN = 2,
    };
    struct port_config {
        /** Port buffer input/output direction */
        enum port_pin_dir  direction;

        /** Port pull-up/pull-down for input pins */
        enum port_pin_pull input_pull;

        /** Enable lowest possible powerstate on the pin
         *
         *  \note All other configurations will be ignored, the pin will be disabled.
         */
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
