/* UConn Senior Design Team 1814, November 2017
*/

#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define NONE            0

#define DISP_ST7735S    1 // 1.44" TFT LCD
#define DISP_ILI9163    2 // 1.8" TFT LCD

#ifndef DISP_SCREEN
    #define DISP_SCREEN NONE
#endif

/* All configurations:
     Color format is 16bit (RRRRRGGGGGGBBBBB)
     Interface mode is 4-wire serial
*/

#if DISP_SCREEN == DISP_ST7735S
    #include "displays/st7735s/st7735s.h"
    #define DISP_WIDTH    128
    #define DISP_HEIGHT   128
    #define disp_write_command       st7735s_write_command
    #define disp_write_data          st7735s_write_data
    #define disp_hard_reset          st7735s_hard_reset
    #define disp_sleep_enable        st7735s_sleep_enable
    #define disp_sleep_disable       st7735s_sleep_disable
    #define disp_set_row_address     st7735s_set_row_address
    #define disp_set_column_address  st7735s_set_column_address
    #define disp_init                st7735s_init
#elif DISP_SCREEN == DISP_ILI9163
    #include "displays/ili9163/ili9163.h"
    #define DISP_WIDTH               128
    #define DISP_HEIGHT              160
    #define disp_write_command       ili9163_write_command
    #define disp_write_data          ili9163_write_data
    #define disp_hard_reset          ili9163_hard_reset
    #define disp_sleep_enable        st7735s_sleep_enable
    #define disp_sleep_disable       st7735s_sleep_disable
    #define disp_set_row_address     ili9163_set_row_address
    #define disp_set_column_address  ili9163_set_column_address
    #define disp_init                ili9163_init
#endif

static inline void disp_setPos(uint8_t x, uint8_t y) {
    disp_set_column_address(x, DISP_WIDTH);
    disp_set_column_address(y, DISP_HEIGHT);
}

static inline void disp_write_pixel(uint16_t bColor) {
    disp_write_data(bColor>>8);
    disp_write_data(bColor&0xff);
}

static inline void disp_clear_screen(uint16_t bColor) {
    unsigned int i,j;
    disp_setPos(0, 0);
    for (i=0;i<DISP_WIDTH;i++)
        for (j=0;j<DISP_HEIGHT;j++) {
            disp_write_pixel(bColor);
        }
}

#endif
