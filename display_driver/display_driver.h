/* UConn Senior Design Team 1814, November 2017
*/

#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define NONE            0

#define DISP_CONSOLE    1 // 1.44" TFT LCD
#define DISP_ST7735S    2 // 1.44" TFT LCD
#define DISP_ILI9163    3 // 1.8" TFT LCD

#ifndef DISP_SCREEN
    #define DISP_SCREEN NONE
#endif

#if DISP_SCREEN == DISP_CONSOLE
    #include "displays/default/console_display.h"
    #define DISP_WIDTH               CONSOLE_DISPLAY_WIDTH
    #define DISP_HEIGHT              CONSOLE_DISPLAY_HEIGHT
    #define disp_write_command       console_display_write_command
    #define disp_write_data          console_display_write_data
    #define disp_hard_reset          console_display_hard_reset
    #define disp_sleep_enable        console_display_sleep_enable
    #define disp_sleep_disable       console_display_sleep_disable
    #define disp_set_row_address     console_display_set_row_address
    #define disp_set_column_address  console_display_set_column_address
    #define disp_sub_init            console_display_init
    #define disp_start_write         console_display_start_write
    #define disp_end_write           console_display_end_write
#elif DISP_SCREEN == DISP_ST7735S
    #include "displays/st7735s/st7735s.h"
    #define DISP_WIDTH               128
    #define DISP_HEIGHT              128
    #define disp_write_command       st7735s_write_command
    #define disp_write_data          st7735s_write_data
    #define disp_hard_reset          st7735s_hard_reset
    #define disp_sleep_enable        st7735s_sleep_enable
    #define disp_sleep_disable       st7735s_sleep_disable
    #define disp_set_row_address     st7735s_set_row_address
    #define disp_set_column_address  st7735s_set_column_address
    #define disp_sub_init            st7735s_init
    #define disp_start_write()       0
    #define disp_end_write()         0
#elif DISP_SCREEN == DISP_ILI9163
    #include "displays/ili9163/ili9163.h"
    #define DISP_WIDTH               128
    #define DISP_HEIGHT              160
    #define disp_write_command       ili9163_write_command
    #define disp_write_data          ili9163_write_data
    #define disp_hard_reset          ili9163_hard_reset
    #define disp_sleep_enable        ili9163_sleep_enable
    #define disp_sleep_disable       ili9163_sleep_disable
    #define disp_set_row_address     ili9163_set_row_address
    #define disp_set_column_address  ili9163_set_column_address
    #define disp_sub_init            ili9163_init
    #define disp_start_write()       0
    #define disp_end_write()         0
#endif

/* All configurations:
     Color format is 16bit (RRRRRGGGGGGBBBBB)
     Interface mode is 4-wire serial
*/
#define DISP_FORMAT_COLOR(r, g, b) (((r&0x1f)<<11) | ((g&0x3f)<<5) | (b&0x1f))

void disp_init();
void disp_setPos(uint8_t x, uint8_t y);
void disp_write_pixel(uint16_t color);
void disp_write_pixel_at(uint8_t x, uint8_t y, uint16_t color);
void disp_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
void disp_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void disp_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void disp_clear_screen(uint16_t color);
void disp_write(uint8_t c);
void disp_write_str(char *str);
void disp_get_text_bounds(char *str, uint8_t x, uint8_t y, uint8_t *x1, uint8_t *y1, uint8_t *w, uint8_t *h);

#endif
