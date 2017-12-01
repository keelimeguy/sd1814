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
}

void smartwatch_service(void) {
    if (is_button_interrupt()) {
        set_screen_timeout(READING_TIMEOUT);
    }
}

uint8_t is_active(void) {
}

uint8_t is_screen_active(void) {
    return is_button_interrupt() || !is_screen_timeout();
}

void sleep(void) {
}

void wakeup(void) {
}

