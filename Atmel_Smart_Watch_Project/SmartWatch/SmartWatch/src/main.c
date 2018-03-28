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
        is_battery_active() triggered by:
            active ADC conversion
*/
#include "smartwatch/display_driver/GFXfont/Fonts/FreeMono9pt7b.h"
#define FONT_9PT                 &FreeMono9pt7b
int main(void) {
    system_init();
    disp_init();

    disp_set_font(FONT_9PT);
    disp_draw_line(0,0,DISP_WIDTH-1,DISP_HEIGHT-1,DISP_PIXEL_YELLOW);
    disp_fill_rect(25,25,49,21,DISP_PIXEL_GREY);
    disp_set_pos(0,50);
    disp_write_str(" Hello, world!");
    disp_commit();
    for(;;) {}
    // init_all();

    // // TODO: future improvement may be running components as separate processes in an RTOS
    // for(;;) {
    //     while (is_active()) {
    //         smartwatch_task();
    //     }
    //     sleep();
    //     wakeup();
    // }
    return 0;
}
