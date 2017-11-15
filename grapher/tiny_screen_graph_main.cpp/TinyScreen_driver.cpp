#include "TinyScreen_driver.h"

static TinyScreen disp(TinyScreenPlus);
int lastx[MAX_WRITE_ID], lasty[MAX_WRITE_ID], lastwidth[MAX_WRITE_ID], lastheight[MAX_WRITE_ID];

void setDisplay(TinyScreen display) {
    disp = display;
    int i;
    for (i = 0; i<MAX_WRITE_ID; i++) {
        lastx[i] = 0;
        lasty[i] = 0;
        lastwidth[i] = 0;
        lastheight[i] = 0;
    }
}

void screen_on() {
    disp.on();
}

void screen_off() {
    disp.off();
}

void display_set_graph_pixel(int x, int y, int color) {
    disp.drawPixel(x+DISPLAY_GRAPH_X, DISPLAY_HEIGHT-y-DISPLAY_GRAPH_Y, color);
}

void writeText(int x, int y, char* text, unsigned int replace_last) {
    disp.setFont(thinPixel7_10ptFontInfo);
    if (replace_last>0 && replace_last<=MAX_WRITE_ID && lastwidth[replace_last-1]>0)
        disp.clearWindow(lastx[replace_last-1], lasty[replace_last-1], lastwidth[replace_last-1], lastheight[replace_last-1]);
    lastwidth[replace_last-1]=disp.getPrintWidth(text);
    lastheight[replace_last-1]=disp.getFontHeight();
    disp.clearWindow(x, y, lastwidth[replace_last-1], lastheight[replace_last-1]);
    disp.setCursor(x,y);
    disp.fontColor(TS_8b_Green,TS_8b_Black);
    disp.print(text);
    lastx[replace_last-1] = x;
    lasty[replace_last-1] = y;
}

void display_reset_graph() {
}
