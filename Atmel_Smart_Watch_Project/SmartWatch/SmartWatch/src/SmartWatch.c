/* UConn Senior Design Team 1814, November 2017
*/

#include <SmartWatch.h>

void init_all() {
    system_init();
    clock_driver_init();
    bluetooth_driver_init();
    button_listener_init();
    measurement_controller_init();
    display_manager_init();

    sleepmgr_init();

    system_interrupt_enable_global();
}

void smartwatch_task(void) {
    uint8_t buttons = get_buttons();
    display_ui_task(buttons);
    if (buttons) {
        set_screen_timeout(READING_TIMEOUT);
    }
}

uint8_t is_active(void) {
    return is_bt_active_soft() || is_screen_active_soft() || is_reading_timeout_soft();
}

uint8_t is_screen_active_soft(void) {
    return is_button_interrupt_soft() || !is_screen_timeout_soft();
}

uint8_t is_screen_active(void) {
    return is_button_interrupt() || !is_screen_timeout();
}

void sleep(void) {
    sleepmgr_enter_sleep();
}

void wakeup(void) {
}

