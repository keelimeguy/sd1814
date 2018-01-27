/* UConn Senior Design Team 1814, November 2017
*/

#include <display_manager.h>

// For testing GUI, compile using: -D DISP_SCREEN=DISP_CONSOLE

// Usage: ./main.exe [repeat]
int main (int argc, char *argv[]) {
    int repeat = 1;
    if (argc > 1) {
        repeat = atoi(argv[1]);
    }

    display_manager_init();
    int button = GRAPH_BUTTON;

    bt_set_notification_1("test");
    display_ui_task(0);
    disp_end_write();
    display_ui_task(VIEW_BUTTON);
    disp_end_write();
    display_ui_task(CLR_BUTTON);
    disp_end_write();
    // Negative repeat will loop forever
    while (repeat > 0) {
        display_ui_task(button);
        disp_end_write();
        button = GRAPH_BUTTON;
        if (repeat > 0) repeat--;
        take_measurement();
    }
    return 0;
}
