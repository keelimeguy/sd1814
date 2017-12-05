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
#include "smartwatch/display_driver/GFXfont/gfxfont.h"
#include "smartwatch/display_driver/GFXfont/Fonts/FreeMono9pt7b.h"

#include "smartwatch/display_driver/grapher/grapher.h"

#define FONT_9PT                 &FreeMono9pt7b

int main (void) {
	system_init();
	disp_init();
	disp_set_font(FONT_9PT);
	disp_draw_line(0,0,DISP_WIDTH-1,DISP_HEIGHT-1,DISP_PIXEL_YELLOW);
	disp_fill_rect(25,25,49,21,DISP_PIXEL_GREY);
	disp_set_pos(0,0);
	disp_write_str(" Hello, world!");
	
//	struct port_config pin;
	//port_get_config_defaults(&PIN_PA);
	//pin.direction = PORT_PIN_DIR_OUTPUT;

//	port_pin_set_config(ST7735S_DC_PIN, &pin);
	
//  init_all();
    for(;;) {
    /*    while (is_active()) {
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
        wakeup();
    */}
}
