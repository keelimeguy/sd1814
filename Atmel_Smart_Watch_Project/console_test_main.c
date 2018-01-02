/* UConn Senior Design Team 1814, November 2017
*/

#include <display_manager.h>

// For testing GUI, compile using: -D DISP_SCREEN=DISP_CONSOLE

int main (void) {
    display_manager_init();
    for (int i = 0; i < 2; i ++) {
        display_ui_task(0);
        disp_end_write();
    }
    add_to_graph(50);
    add_to_graph(200);
    add_to_graph(100);
    for (;;) {
        display_ui_task(0);
        disp_end_write();
    }
    return 0;
}
