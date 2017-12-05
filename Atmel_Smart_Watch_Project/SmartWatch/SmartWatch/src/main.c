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
	int i;
    for(;;) {
        while (is_active()||i<8) {
            if (is_reading_timeout()) {
				if (++i > 10) {
					i =0;
				}
                take_measurement();
            }
            if (is_bt_active()) {
                bt_task();
            }
            if (is_screen_active()||i<8) {
                smartwatch_task();
            }
        }
        sleep();
        wakeup();
		if (i<8)
		disp_sleep_disable();
    }
}
