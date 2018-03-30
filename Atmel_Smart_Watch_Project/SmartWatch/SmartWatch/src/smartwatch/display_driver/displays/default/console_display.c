/* UConn Senior Design Team 1814, November 2017
*/

#include "console_display.h"
#include "../../display_driver.h"

static uint8_t xs, ys, xe, ye;
static char fb[CONSOLE_DISPLAY_WIDTH][CONSOLE_DISPLAY_HEIGHT];

void console_display_init(void) {
    xs = 0;
    ys = 0;
    xe = CONSOLE_DISPLAY_WIDTH-1;
    ye = CONSOLE_DISPLAY_HEIGHT-1;
    int x, y;
    for (y=0; y<CONSOLE_DISPLAY_HEIGHT; y++)
        for (x=0; x<CONSOLE_DISPLAY_WIDTH; x++)
            fb[x][y] = '_';
}

void console_display_write_command(uint8_t command) {
}

static int fix_2write_16_bit = 0;
static int r = 0, g = 0, b = 0;
static uint8_t last_data;
void console_display_write_data(uint8_t data) {
    if (fix_2write_16_bit) {
        char char_out;
        if ((data|COLOR_ARRAY[DISP_BG_COLOR]) == (COLOR_ARRAY[DISP_BG_COLOR]&0xff) && (last_data|(COLOR_ARRAY[DISP_BG_COLOR]>>8)) == ((COLOR_ARRAY[DISP_BG_COLOR]>>8)&0xff)) {
            char_out = '_';
        } else {
            if ((data&COLOR_ARRAY[DISP_PIXEL_GREEN]) == (COLOR_ARRAY[DISP_PIXEL_GREEN]&0xff)) {
                g = 1;
            } else {
                g = 0;
            }

            if ((data&COLOR_ARRAY[DISP_PIXEL_BLUE]) == (COLOR_ARRAY[DISP_PIXEL_BLUE]&0xff)) {
                b = 1;
            } else {
                b = 0;
            }
            switch ((r<<2)|(g<<1)|(b)) {
                case 4:
                    char_out = 'R';
                    break;
                case 2:
                    char_out = 'G';
                    break;
                case 1:
                    char_out = 'U';
                    break;
                case 6:
                    char_out = 'Y';
                    break;
                case 5:
                    char_out = 'M';
                    break;
                case 3:
                    char_out = 'C';
                    break;
                case 7:
                    char_out = 'W';
                    break;
                case 0:
                    char_out = 'B';
                    break;
                default:
                    char_out = 'X';
            }
        }
        fb[xs][ys] = char_out;
        if (++xs>xe) {
            xs = 0;
            if (++ys>ye)
                ys = 0;
        }
        fix_2write_16_bit = 0;
        r = 0;
        g = 0;
        b = 0;
    } else {
        fix_2write_16_bit = 1;
        if ((data&(COLOR_ARRAY[DISP_PIXEL_RED]>>8)) == ((COLOR_ARRAY[DISP_PIXEL_RED]>>8)&0xff)) {
            r = 1;
        } else {
            r = 0;
        }

        if ((data&(COLOR_ARRAY[DISP_PIXEL_GREEN]>>8)) == ((COLOR_ARRAY[DISP_PIXEL_GREEN]>>8)&0xff)) {
            g = 1;
        } else {
            g = 0;
        }

        last_data = data;
    }
}

void console_display_end_write(void) {
    int x, y;
    for (y=0; y<CONSOLE_DISPLAY_HEIGHT; y++) {
        for (x=0; x<CONSOLE_DISPLAY_WIDTH; x++)
            printf("%c", fb[x][y]);
        printf("\n");
    }
    printf("\n");
}

void console_display_set_row_address(uint8_t start_address, uint8_t end_address) {
    ys = start_address;
    ye = end_address;
}

void console_display_set_column_address(uint8_t start_address, uint8_t end_address) {
    xs = start_address;
    xe = end_address;
}
