/* UConn Senior Design Team 1814, November 2017
*/

#include "display_driver.h"

// Font library
#include "GFXfont/gfxfont.h"

// Utility function definitions
#ifndef _swap_uint8_t
 #define _swap_uint8_t(a, b) { uint8_t t = a; a = b; b = t; }
#endif
#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

// Static variables
static GFXfont *gfxFont;
static uint8_t wrap;   // If set, 'wrap' text at right edge of display
static uint8_t _cp437; // If set, use correct CP437 charset (default is off)
static uint8_t _width, _height, // Display w/h as modified by current rotation
                cursor_x, cursor_y;
static uint16_t textcolor, textbgcolor;
static uint8_t textsize, rotation;

#include "GFXfont/Fonts/FreeSans9pt7b.h"

void disp_init() {
    gfxFont = &FreeSans9pt7b;
    wrap = 1;
    _cp437 = 1;
    _width = DISP_WIDTH;
    _height = DISP_HEIGHT;
    cursor_x = 0;
    cursor_y = 0;
    textcolor = 1;
    textbgcolor = 0;
    textsize = 9;
    rotation = 0;
    disp_sub_init();
}

void disp_setPos(uint8_t x, uint8_t y) {
    disp_set_column_address(x, DISP_WIDTH-1);
    disp_set_row_address(y, DISP_HEIGHT-1);
}

void disp_write_pixel(uint16_t color) {
    disp_write_data(color>>8);
    disp_write_data(color&0xff);
}

void disp_write_pixel_at(uint8_t x, uint8_t y, uint16_t color) {
    disp_setPos(x, y);
    disp_write_pixel(color);
}

// Bresenham's algorithm
void disp_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color) {
    uint8_t steep = Abs(y1 - y0) > Abs(x1 - x0);
    if (steep) {
        _swap_uint8_t(x0, y0);
        _swap_uint8_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_uint8_t(x0, x1);
        _swap_uint8_t(y0, y1);
    }

    int8_t dx, dy;
    dx = x1 - x0;
    dy = Abs(y1 - y0);

    int8_t err = dx / 2;
    int8_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            disp_write_pixel_at(y0, x0, color);
        } else {
            disp_write_pixel_at(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void disp_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    disp_draw_line(x, y, x+w-1, y, color);
    disp_draw_line(x, y+h-1, x+w-1, y+h-1, color);
    disp_draw_line(x, y, x, y+h-1, color);
    disp_draw_line(x+w-1, y, x+w-1, y+h-1, color);
}

void disp_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    for (uint8_t i=x; i<x+w; i++) {
        disp_draw_line(i, y, i, y+h-1, color);
    }
}

void disp_clear_screen(uint16_t color) {
    disp_fill_rect(0, 0, DISP_WIDTH, DISP_HEIGHT, color);
}

static void disp_draw_char(uint8_t x, uint8_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
    // Can only draw character if font is loaded
    if (gfxFont) {
        c -= (uint8_t)pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                 h  = pgm_read_byte(&glyph->height);
        uint8_t  xo = pgm_read_byte(&glyph->xOffset),
                 yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        uint8_t  xo16 = 0, yo16 = 0;

        if(size > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size == 1) {
                        disp_write_pixel_at(x+xo+xx, y+yo+yy, color);
                    } else {
                        disp_fill_rect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
                    }
                }
                bits <<= 1;
            }
        }
    }
}

void disp_write(uint8_t c) {
    if(gfxFont) {
        if(c == '\n') {
            cursor_x  = 0;
            cursor_y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if(c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
                  &gfxFont->glyph))[c - first]);
                uint8_t   w     = pgm_read_byte(&glyph->width),
                          h     = pgm_read_byte(&glyph->height);
                if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    uint8_t xo = (uint8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if(wrap && ((cursor_x + textsize * (xo + w)) > _width)) {
                        cursor_x  = 0;
                        cursor_y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                    }
                    disp_draw_char(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
                }
                cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * textsize;
            }
        }

    }
}

void disp_write_str(char *str) {
    char c;
    while((c = *str++))
        disp_write((uint8_t)c);
}

static void disp_char_bounds(char c, uint8_t *x, uint8_t *y, uint8_t *minx, uint8_t *miny, uint8_t *maxx, uint8_t *maxy) {
    if (gfxFont) {
        if (c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if (c != '\r') { // Not a carriage return; is normal char
            uint8_t first = pgm_read_byte(&gfxFont->first),
                    last  = pgm_read_byte(&gfxFont->last);
            if ((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c - first]);
                uint8_t gw = pgm_read_byte(&glyph->width),
                        gh = pgm_read_byte(&glyph->height),
                        xa = pgm_read_byte(&glyph->xAdvance);
                int8_t  xo = pgm_read_byte(&glyph->xOffset),
                        yo = pgm_read_byte(&glyph->yOffset);
                if (wrap && ((*x+((int16_t)xo+gw)*textsize)) > _width) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += textsize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                }
                uint8_t x1 = *x + xo * textsize,
                        y1 = *y + yo * textsize,
                        x2 = x1 + gw * textsize - 1,
                        y2 = y1 + gh * textsize - 1;
                if (x1 < *minx) *minx = x1;
                if (y1 < *miny) *miny = y1;
                if (x2 > *maxx) *maxx = x2;
                if (y2 > *maxy) *maxy = y2;
                *x += xa * textsize;
            }
        }
    }
}

// Pass string and a cursor position, returns UL corner and W,H.
void disp_get_text_bounds(char *str, uint8_t x, uint8_t y, uint8_t *x1, uint8_t *y1, uint8_t *w, uint8_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    uint8_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while((c = *str++))
        disp_char_bounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}
