/* UConn Senior Design Team 1814, November 2017
*/

#include <display_manager.h>

// For testing GUI, compile using: -D DISP_SCREEN=DISP_CONSOLE
#include <math.h>
// Usage: ./main.exe [repeat]
int main (int argc, char *argv[]) {
    measurement_controller_init();

    int repeat = 1;
    if (argc > 1) {
        repeat = atoi(argv[1]);
    }

    display_manager_init();
    int button = GRAPH_BUTTON;
    struct rtc_calendar_time *time;
    bt_set_notification_1("test");

    display_ui_task(0);
    rtc_next_time();

    display_ui_task(0);
    rtc_next_time();

    display_ui_task(VIEW_BUTTON);
    rtc_next_time();

    display_ui_task(VIEW_BUTTON);
    rtc_next_time();

    display_ui_task(VIEW_BUTTON);
    rtc_next_time();

    display_ui_task(CLR_BUTTON);
    rtc_next_time();

    display_ui_task(GRAPH_BUTTON);
    rtc_next_time();

    // Negative repeat will loop forever
    while (repeat != 0) {
        measurement_task();
        display_ui_task(GRAPH_BUTTON);
        if (repeat > 0) repeat--;
        take_measurement(0);
        measurement_task();
        if (is_new_measurement()) {
            updateGraph(get_measurement());
        }
        rtc_next_time();
    }
    return 0;
}
