#include "TinyScreen.h"

#define DISPLAY_WIDTH   96 // pixels
#define DISPLAY_HEIGHT  64 // pixels

#define DISPLAY_GRAPH_WIDTH             60 // pixels
#define DISPLAY_GRAPH_HEIGHT            60 // pixels
#define DISPLAY_GRAPH_BAR_WIDTH         2  // pixels
#define DISPLAY_GRAPH_MIN_BAR_HEIGHT    1  // pixels
#define DISPLAY_GRAPH_MAX_BAR_HEIGHT    60 // pixels
#define DISPLAY_GRAPH_BAR_PADDING_LOW   1
#define DISPLAY_GRAPH_BAR_PADDING_HIGH  1

#define DISPLAY_GRAPH_X 32
#define DISPLAY_GRAPH_Y 2

#define PIXEL_WHITE   TS_8b_White
#define PIXEL_YELLOW  TS_8b_Yellow
#define PIXEL_RED     TS_8b_Red
#define PIXEL_OFF     TS_8b_Black

void setDisplay(TinyScreen display);
void display_set_graph_pixel(int x, int y, int color);
void writeText(int x, int y, char* text, unsigned char replace_last);
void display_reset_graph();
