/* UConn Senior Design Team 1814, November 2017
*/

#include <SmartWatch.h>

/* is_reading_timeout() triggered by:
        RTC Alarm (RTC interrupt)
    is_bt_active() triggered by:
        BT message send request
        BT interrupt
    is_screen_active() triggered by:
        user input (external interrupt)
*/

int main (void) {
    init_all();
    int i = 0;
    for(;;) {
        while (is_active()) {
            if (is_reading_timeout()) {
                take_measurement();
            }
            if (is_bt_active()) {
                bt_task();
            }
            if (is_screen_active()) {
                smartwatch_task();
            }
        }
        sleep();
		request_screen_on();
        wakeup();
    }
}
