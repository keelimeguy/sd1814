/* UConn Senior Design Team 1814, November 2017
*/

#include "console_display.h"
#include "../../display_driver.h"

static uint8_t xs, ys, curx, cury, xe, ye;
static char fb[CONSOLE_DISPLAY_WIDTH][CONSOLE_DISPLAY_HEIGHT];

void console_display_init(void) {
    xs = 0;
    ys = 0;
    curx = xs;
    cury = ys;
    xe = CONSOLE_DISPLAY_WIDTH-1;
    ye = CONSOLE_DISPLAY_HEIGHT-1;
    int x, y;
    for (y=0; y<CONSOLE_DISPLAY_HEIGHT; y++)
        for (x=0; x<CONSOLE_DISPLAY_WIDTH; x++)
            fb[x][y] = '_';
}

void console_display_write_command(uint8_t command) {
}

// static int fix_2write_16_bit = 0;
// static int r = 0, g = 0, b = 0;
// static uint8_t last_data;
// // 16-bit mode
// void console_display_write_data(uint8_t data) {
//     if (fix_2write_16_bit) {
//         char char_out;
//         if ((data|DISP_BG_COLOR) == (DISP_BG_COLOR&0xff) && (last_data|(DISP_BG_COLOR>>8)) == ((DISP_BG_COLOR>>8)&0xff)) {
//             char_out = '_';
//         } else {
//             if ((data&DISP_PIXEL_GREEN) == (DISP_PIXEL_GREEN&0xff)) {
//                 g = 1;
//             } else {
//                 g = 0;
//             }

//             if ((data&DISP_PIXEL_BLUE) == (DISP_PIXEL_BLUE&0xff)) {
//                 b = 1;
//             } else {
//                 b = 0;
//             }
//             switch ((r<<2)|(g<<1)|(b)) {
//                 case 4:
//                     char_out = 'R';
//                     break;
//                 case 2:
//                     char_out = 'G';
//                     break;
//                 case 1:
//                     char_out = 'U';
//                     break;
//                 case 6:
//                     char_out = 'Y';
//                     break;
//                 case 5:
//                     char_out = 'M';
//                     break;
//                 case 3:
//                     char_out = 'C';
//                     break;
//                 case 7:
//                     char_out = 'W';
//                     break;
//                 case 0:
//                     char_out = 'B';
//                     break;
//                 default:
//                     char_out = 'X';
//             }
//         }
//         fb[curx][cury] = char_out;
//         if (++curx>xe) {
//             curx = xs;
//             if (++cury>ye)
//                 cury = ys;
//         }
//         fix_2write_16_bit = 0;
//         r = 0;
//         g = 0;
//         b = 0;
//     } else {
//         fix_2write_16_bit = 1;
//         if ((data&(DISP_PIXEL_RED>>8)) == ((DISP_PIXEL_RED>>8)&0xff)) {
//             r = 1;
//         } else {
//             r = 0;
//         }

//         if ((data&(DISP_PIXEL_GREEN>>8)) == ((DISP_PIXEL_GREEN>>8)&0xff)) {
//             g = 1;
//         } else {
//             g = 0;
//         }

//         last_data = data;
//     }
// }

static int fix_2write_12_bit = 0;
static int r = 0, g = 0, b = 0;
static uint8_t last_data;
// 16-bit mode
void console_display_write_data(uint8_t data) {
    if (fix_2write_12_bit == 1) {
        char char_out;
        if (((data>>4) == (DISP_BG_COLOR&0xf)) && (last_data == (DISP_BG_COLOR>>4))) {
            char_out = '_';
        } else {
            if ((data&0xf0) == 0xf0) {
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
        fb[curx][cury] = char_out;
        if (++curx>xe) {
            curx = xs;
            if (++cury>ye)
                cury = ys;
        }
        fix_2write_12_bit = 2;
        r = 0;
        g = 0;
        b = 0;

        if ((data&0xf) == 0xf) {
            r = 1;
        } else {
            r = 0;
        }
        last_data = data;

    } else if (fix_2write_12_bit == 2) {
        char char_out;
        if ((data == (DISP_BG_COLOR&0xff)) && ((last_data&0xf) == DISP_BG_COLOR>>8)) {
            char_out = '_';
        } else {
            if ((data&0xf0) == 0xf0) {
                g = 1;
            } else {
                g = 0;
            }

            if ((data&0xf) == 0xf) {
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
        fb[curx][cury] = char_out;
        if (++curx>xe) {
            curx = xs;
            if (++cury>ye)
                cury = ys;
        }
        fix_2write_12_bit = 0;
        r = 0;
        g = 0;
        b = 0;
    } else {
        fix_2write_12_bit = 1;
        if ((data&0xf0) == 0xf0) {
            r = 1;
        } else {
            r = 0;
        }

        if ((data&0x0f) == 0x0f) {
            g = 1;
        } else {
            g = 0;
        }

        last_data = data;
    }
}

void console_display_write_multiple_data(uint8_t* data, uint16_t length) {
    for (int i = 0; i < length; i++)
        console_display_write_data(data[i]);
}

void console_display_end_write(void) {
    int x, y;
    #if BOARD_DISPLAY_ROTATE
    for (y=CONSOLE_DISPLAY_HEIGHT-1; y>=0; y--) {
        for (x=0; x<CONSOLE_DISPLAY_WIDTH; x++)
            printf("%c", fb[y][x]);
    #else
    for (y=0; y<CONSOLE_DISPLAY_HEIGHT; y++) {
        for (x=0; x<CONSOLE_DISPLAY_WIDTH; x++)
            printf("%c", fb[x][y]);
    #endif
        printf("\n");
    }
    printf("\n");

    for (y=0; y<CONSOLE_DISPLAY_HEIGHT; y++)
        for (x=0; x<CONSOLE_DISPLAY_WIDTH; x++)
            fb[x][y] = 'X';

}

void console_display_set_row_address(uint8_t start_address, uint8_t end_address) {
    ys = start_address;
    cury = ys;
    ye = end_address;
}

void console_display_set_column_address(uint8_t start_address, uint8_t end_address) {
    xs = start_address;
    curx = xs;
    xe = end_address;
}
