#include "TinyScreen_driver.h"

static TinyScreen disp(TinyScreenPlus);
int lastx, lasty, lastwidth, lastheight;

void setDisplay(TinyScreen display) {
    disp = display;
    lastx = 0;
    lasty = 0;
    lastwidth = 0;
    lastheight = 0;
}

void display_set_graph_pixel(int x, int y, int color) {
    disp.drawPixel(x+DISPLAY_GRAPH_X, DISPLAY_HEIGHT-y-DISPLAY_GRAPH_Y, color);
}

void writeText(int x, int y, char* text, unsigned char replace_last) {
    disp.setFont(thinPixel7_10ptFontInfo);
    if (replace_last)
        disp.clearWindow(lastx, lasty, lastwidth, lastheight);
    lastwidth=disp.getPrintWidth(text);
    lastheight=disp.getFontHeight();
    disp.clearWindow(x, y, lastwidth, lastheight);
    disp.setCursor(x,y);
    disp.fontColor(TS_8b_Green,TS_8b_Black);
    disp.print(text);
    lastx = x;
    lasty = y;
}

void display_reset_graph() {
}

