/* UConn Senior Design Team 1814, November 2017
*/

#include "display_driver.h"

int main() {
    disp_init();

    disp_start_write();
    disp_draw_line(0,0,63,63,1);
    disp_end_write();

    disp_setPos(0,20);

    disp_start_write();
    disp_write_str("A");
    disp_end_write();

    return 0;
}