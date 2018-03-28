/* UConn Senior Design Team 1814, January 2018
*/

#include <SmartWatch.h>

#ifndef CONSOLE_VERSION

    static uint8_t screen_request, screen_sleep;

    void init_all(void) {
        system_init();

        // Configure debugging LED if in use
        #if DEBUG_MODE!=DEBUG_NONE
        struct port_config pin;
        port_get_config_defaults(&pin);
        pin.direction = PORT_PIN_DIR_OUTPUT;

        port_pin_set_config(BOARD_DEBUG_LED, &pin);
        #if DEBUG_MODE==DEBUG_DISPLAY
        port_pin_set_output_level(BOARD_DEBUG_LED, true);
        #endif
        #endif

        clock_driver_init();
        // battery_reader_init();
        // bluetooth_driver_init();
        button_listener_init();
        // measurement_controller_init();
        display_manager_init();

        sleepmgr_init();

        // Enable interrupts
        system_interrupt_enable_global();

        screen_request = 1;
        screen_sleep = 0;
    }

    void smartwatch_task(void) {
        // Perform smartwatch subtasks
        // bt_task();
        measurement_task();
        if (is_new_measurement()) {
            updateGraph(get_measurement());
        }
        // battery_task();

        #if DEBUG_MODE==DEBUG_BUTTON_L
            port_pin_set_output_level(BOARD_DEBUG_LED, port_pin_get_input_level(BOARD_BUTTON_L_PIN));
        #endif
        #if DEBUG_MODE==DEBUG_BUTTON_R
            port_pin_set_output_level(BOARD_DEBUG_LED, port_pin_get_input_level(BOARD_BUTTON_R_PIN));
        #endif

        if (is_screen_active()) {

            // No need to deal with new battery reads if screen is not active
            if (!is_battery_active() && is_battery_timeout()) {
                start_battery_read();
                set_battery_timeout(BATTERY_TIMEOUT);
            }

            uint8_t buttons = get_buttons(); // Returns identifier to determine which buttons were pressed
            display_ui_task(buttons);

            // We need to set a timeout for screen if it was freshly activated
            if (screen_request || buttons) { // i.e. by explicit request or button press
                screen_request = 0;

                // Wakeup screen if needed
                if (screen_sleep) {
                    screen_sleep = 0;
                    disp_sleep_disable();
                }

                set_screen_timeout(SCREEN_TIMEOUT);
            }
        } else {
            // Trigger battery timer for next screen activation
            set_battery_timeout(0);
        }
    }

    // If this is true, system will stay awake
    uint8_t is_active(void) {
        // We stay awake when bluetooth, screen, or measurement is active
        return is_screen_active_soft();
    }

    // If this is true, screen will be rendered
    uint8_t is_screen_active_soft(void) {
        // Render screen on button press, explicit screen request, or if screen timeout has not run out
        return is_button_interrupt_soft() || !is_screen_timeout_soft() || screen_request;
    }

    // Similar to is_screen_active_soft except clears any handling flags
    //  e.g. clear flag to indicate button interrupt is handled (or else will keep returning true)
    uint8_t is_screen_active(void) {
        return is_button_interrupt() || !is_screen_timeout() || screen_request;
    }

    void request_screen_on(void) {
        screen_request = 1;
    }

    void sleep(void) {
        // First put display to sleep
        disp_sleep_enable();
        screen_sleep = 1;

        #if DEBUG_MODE==DEBUG_DISPLAY
            port_pin_set_output_level(BOARD_DEBUG_LED, false);
        #endif

        sleepmgr_enter_sleep();
    }

    void wakeup(void) {
        #if DEBUG_MODE==DEBUG_DISPLAY
            port_pin_set_output_level(BOARD_DEBUG_LED, true);
        #endif

        // Don't want to read wakeup button as input
        if (is_button_interrupt()) {
            get_buttons();
            request_screen_on();
        }

        // Wakeup display if needed
        if (is_screen_active_soft()) {
            // Trigger battery timer for next wakeup
            // set_battery_timeout(0);
            screen_sleep = 0;
            disp_sleep_disable();
        }
    }

// The following is specifically for console testing code,
// e.g. compiled with flag: -D CONSOLE_VERSION=<anything>
#else

    struct rtc_calendar_time rtc_instance = {
        58,
        59,
        11,
        1,
        31,
        12,
        2017
    };

    void rtc_get_time(struct rtc_calendar_time *time) {
        time->second = rtc_instance.second;
        time->minute = rtc_instance.minute;
        time->hour = rtc_instance.hour;
        time->pm = rtc_instance.pm;
        time->day = rtc_instance.day;
        time->month = rtc_instance.month;
        time->year = rtc_instance.year;
    }

    void rtc_update_time(struct rtc_calendar_time *time) {
        rtc_instance.second = time->second;
        rtc_instance.minute = time->minute;
        rtc_instance.hour = time->hour;
        rtc_instance.pm = time->pm;
        rtc_instance.day = time->day;
        rtc_instance.month = time->month;
        rtc_instance.year = time->year;
    }

    void rtc_next_time() {
        rtc_instance.second++;
        if (rtc_instance.second>=60) {
            rtc_instance.second = 0;
            rtc_instance.minute++;
            if (rtc_instance.minute>=60) {
                rtc_instance.minute = 0;
                rtc_instance.hour++;
                if (rtc_instance.hour > 11) {
                    if (rtc_instance.hour == 12) {
                        if (rtc_instance.pm) {
                            add_to_date_uchar(1, &(rtc_instance.year), &(rtc_instance.month), &(rtc_instance.day));
                            rtc_instance.pm = !rtc_instance.pm;
                        }
                    } else
                        rtc_instance.hour = 1;
                }
            }
        }
    }

#endif
