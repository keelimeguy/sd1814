/* UConn Senior Design Team 1814, November 2017
*/

#include "display_driver.h"
#include "GFXfont/Fonts/FreeMono9pt7b.h"

int main() {
    disp_init();
    disp_set_font(&FreeMono9pt7b);

    disp_start_write();
    disp_set_pos(0,0);
    disp_write_str("\\Hello, world!");
    disp_end_write();
    uint8_t x1;
    uint8_t y1;
    uint8_t w;
    uint8_t h;
    disp_get_text_bounds("\\Hello, world!", 0, 0, &x1, &y1, &w, &h);
    printf("%d, %d, %d, %d\n", x1, y1, w, h);

    return 0;
}
