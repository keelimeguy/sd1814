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
    int button = 0;

    add_to_graph(repeat);
    // Negative repeat will loop forever
    while (repeat > 0) {
        display_ui_task(button);
        disp_end_write();
        button = GRAPH_BUTTON;
        if (repeat > 0) repeat--;
        add_to_graph(repeat);
    }
    return 0;
}
