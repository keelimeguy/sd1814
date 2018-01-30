/* UConn Senior Design Team 1814, January 2018
*/

#include <SmartWatch.h>

/*  is_active() triggered by:
        is_measure_busy() triggered by:
            RTC Alarm (RTC interrupt)
        is_bt_active() triggered by:
            BT message send request
            BT received (aci_loop must always run)
        is_screen_active() triggered by:
            user input (external interrupt)
*/

int main(void) {
    init_all();

    // TODO: future improvement may be running components as separate processes in an RTOS
    for(;;) {
        while (is_active()) {
            //aci_loop();
            //bt_task();
            smartwatch_task();
        }
        sleep();
        request_screen_on(); // TODO: should be removed later
        wakeup();
    }
    return 0;
}
