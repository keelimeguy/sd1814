/* UConn Senior Design Team 1814, November 2017
*/

#include <SmartWatch.h>

/* is_reading_timeout() triggered by:
        RTC Alarm (RTC interrupt)
    is_bt_active() triggered by:
        BT message send request
        BT received (aci_loop must always run) // TODO: implement interrupt-driven bluetooth
    is_screen_active() triggered by:
        user input (external interrupt)
*/

int main(void) {

/*START BT_TESTING*/
    init_all();
    lib_aci_echo_msg(2, "hi");
    while(1) {
        aci_loop();
    }
    return 0;
/*END BT_TESTING*/


    // // TODO: future improvement may be running components as separate processes in an RTOS
    // for(;;) {
    //     while (is_active()) {
    //         aci_loop(); // TODO: implement interrupt-driven bluetooth
    //         if (is_reading_timeout()) {
    //             take_measurement();
    //         }
    //         if (is_bt_active()) {
    //             bt_task();
    //         }
    //         if (is_screen_active()) {
    //             smartwatch_task();
    //         }
    //     }
    //     sleep();
    //     request_screen_on(); // TODO: should be removed later
    //     wakeup();
    // }
    // return 0;
}
