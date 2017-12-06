/* UConn Senior Design Team 1814, November 2017
*/

#include <SmartWatch.h>

static uint8_t screen_request;

void init_all() {
    system_init();
    clock_driver_init();
    bluetooth_driver_init();
    button_listener_init();
    measurement_controller_init();
    display_manager_init();

    sleepmgr_init();

    system_interrupt_enable_global();

    screen_request = 1;
}

void smartwatch_task(void) {
    uint8_t buttons = get_buttons();
    display_ui_task(buttons);
    if (screen_request || buttons) {
        screen_request = 0;
        set_screen_timeout(READING_TIMEOUT);
    }
}

uint8_t is_active(void) {
    return is_bt_active_soft() || is_screen_active_soft() || is_reading_timeout_soft();
}

uint8_t is_screen_active_soft(void) {
    return is_button_interrupt_soft() || !is_screen_timeout_soft() || screen_request;
}

uint8_t is_screen_active(void) {
    return is_button_interrupt() || !is_screen_timeout() || screen_request;
}

void request_screen_on(void) {
    screen_request = 1;
}

void sleep(void) {
    disp_sleep_enable();
    sleepmgr_enter_sleep();
}

void wakeup(void) {
    if (is_screen_active_soft()) {
        disp_sleep_disable();
    }
}
