/* UConn Senior Design Team 1814, November 2017
*/

#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define NONE            0

#define DISP_CONSOLE    1 // printf
#define DISP_ST7735S    2 // 1.44" TFT LCD
#define DISP_ILI9163    3 // 1.8" TFT LCD

#ifdef CONSOLE_VERSION
    #include <Smartwatch.h>
#endif

#ifndef DISP_SCREEN
    #define DISP_SCREEN NONE
#endif

#if DISP_SCREEN == DISP_CONSOLE
    #include "displays/default/console_display.h"
    #define disp_write_command       console_display_write_command
    #define disp_write_data          console_display_write_data
    #define disp_hard_reset          console_display_hard_reset
    #define disp_sub_sleep_enable    console_display_sleep_enable
    #define disp_sub_sleep_disable   console_display_sleep_disable
    #define disp_set_row_address     console_display_set_row_address
    #define disp_set_column_address  console_display_set_column_address
    #define disp_sub_init            console_display_init
    #define disp_sub_display_on      console_display_on
    #define disp_start_write         console_display_start_write
    #define disp_end_write           console_display_end_write

    #define DISP_WIDTH               CONSOLE_DISPLAY_WIDTH
    #define DISP_HEIGHT              CONSOLE_DISPLAY_HEIGHT

    #define DISP_GRAPH_WIDTH             (DISP_WIDTH-8) // pixels
    #define DISP_GRAPH_HEIGHT            54 // pixels
    #define DISP_GRAPH_BAR_WIDTH         2  // pixels
    #define DISP_GRAPH_MIN_BAR_HEIGHT    1  // pixels
    #define DISP_GRAPH_MAX_BAR_HEIGHT    DISP_GRAPH_HEIGHT
    #define DISP_GRAPH_BAR_PADDING_LOW   1
    #define DISP_GRAPH_BAR_PADDING_HIGH  1

    #define DISP_GRAPH_X 4
    #define DISP_GRAPH_Y 22

#elif DISP_SCREEN == DISP_ST7735S
    #include "displays/st7735s/st7735s.h"
    #define disp_write_command       st7735s_write_command
    #define disp_write_data          st7735s_write_data
    #define disp_hard_reset          st7735s_hard_reset
    #define disp_sub_sleep_enable    st7735s_sleep_enable
    #define disp_sub_sleep_disable   st7735s_sleep_disable
    #define disp_set_row_address     st7735s_set_row_address
    #define disp_set_column_address  st7735s_set_column_address
    #define disp_sub_init            st7735s_init
    #define disp_sub_display_on      st7735s_display_on
    #define disp_start_write()       0
    #define disp_end_write()         0

    #define DISP_WIDTH               128
    #define DISP_HEIGHT              128

    #define DISP_GRAPH_WIDTH             (DISP_WIDTH-8) // pixels
    #define DISP_GRAPH_HEIGHT            54 // pixels
    #define DISP_GRAPH_BAR_WIDTH         2  // pixels
    #define DISP_GRAPH_MIN_BAR_HEIGHT    1  // pixels
    #define DISP_GRAPH_MAX_BAR_HEIGHT    DISP_GRAPH_HEIGHT
    #define DISP_GRAPH_BAR_PADDING_LOW   1
    #define DISP_GRAPH_BAR_PADDING_HIGH  1

    #define DISP_GRAPH_X 4
    #define DISP_GRAPH_Y 22

#elif DISP_SCREEN == DISP_ILI9163
    #include "displays/ili9163/ili9163.h"
    #define disp_write_command       ili9163_write_command
    #define disp_write_data          ili9163_write_data
    #define disp_hard_reset          ili9163_hard_reset
    #define disp_sub_sleep_enable    ili9163_sleep_enable
    #define disp_sub_sleep_disable   ili9163_sleep_disable
    #define disp_set_row_address     ili9163_set_row_address
    #define disp_set_column_address  ili9163_set_column_address
    #define disp_sub_init            ili9163_init
    #define disp_sub_display_on      ili9163_display_on
    #define disp_start_write()       0
    #define disp_end_write()         0

    #define DISP_WIDTH               128
    #define DISP_HEIGHT              160

    #define DISP_GRAPH_WIDTH             (DISP_WIDTH-8) // pixels
    #define DISP_GRAPH_HEIGHT            54 // pixels
    #define DISP_GRAPH_BAR_WIDTH         2  // pixels
    #define DISP_GRAPH_MIN_BAR_HEIGHT    1  // pixels
    #define DISP_GRAPH_MAX_BAR_HEIGHT    DISP_GRAPH_HEIGHT
    #define DISP_GRAPH_BAR_PADDING_LOW   1
    #define DISP_GRAPH_BAR_PADDING_HIGH  1

    #define DISP_GRAPH_X 4
    #define DISP_GRAPH_Y 22

#endif

// Font library
#include "GFXfont/gfxfont.h"

/* All configurations:
     Color format is 16bit (RRRR_RGGG_GGGB_BBBB)
     Interface mode is 4-wire serial
*/
#define DISP_FORMAT_COLOR(r, g, b) (((r&0x1f)<<11) | ((g&0x3f)<<5) | (b&0x1f))

// Warning levels for glucose
#define DISP_DANGER_HIGH  300.0
#define DISP_WARNING_HIGH 180.0
#define DISP_WARNING_LOW  70.0
#define DISP_DANGER_LOW   50.0

static const uint16_t COLOR_ARRAY[] = {0x0000, 0x8410, 0xffff, 0xf800, 0xffe0, 0x07e0, 0x07ff, 0x001f, 0xf81f};
#define DISP_PIXEL_BLACK          0
#define DISP_PIXEL_GREY           1
#define DISP_PIXEL_WHITE          2
#define DISP_PIXEL_RED            3
#define DISP_PIXEL_YELLOW         4
#define DISP_PIXEL_GREEN          5
#define DISP_PIXEL_CYAN           6
#define DISP_PIXEL_BLUE           7
#define DISP_PIXEL_MAGENTA        8

#define DISP_BG_COLOR             DISP_PIXEL_BLUE

#define DISP_PIXEL_GRAPH_ON       DISP_PIXEL_WHITE
#define DISP_PIXEL_GRAPH_WARNING  DISP_PIXEL_YELLOW
#define DISP_PIXEL_GRAPH_DANGER   DISP_PIXEL_RED
#define DISP_PIXEL_GRAPH_OFF      DISP_PIXEL_BLACK

#ifndef MAX_WRITE_ID
    #define MAX_WRITE_ID 12
#endif

void disp_init(void);
void disp_set_pos(uint8_t x, uint8_t y);
void disp_write_pixel(uint16_t color);
void disp_write_pixel_at(uint8_t x, uint8_t y, uint8_t color);
void disp_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void disp_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void disp_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void disp_clear_screen(uint8_t color);
void disp_write(uint8_t c);
void disp_write_str(char *str);
void disp_write_str_group(char *str, uint8_t replace_last);
void disp_remove_str_group(uint8_t replace_last);
void disp_end_group(void);
void disp_get_text_bounds(char *str, uint8_t x, uint8_t y, uint8_t *x1, uint8_t *y1, uint8_t *w, uint8_t *h);
void disp_set_font(GFXfont *font);
void disp_set_font_scale(uint8_t scale);
void disp_set_wrap(uint8_t val);
void disp_set_cp437(uint8_t val);
void disp_set_color(uint8_t text, uint8_t bg);
void disp_commit();
void disp_sleep_enable();
void disp_sleep_disable();

#endif
