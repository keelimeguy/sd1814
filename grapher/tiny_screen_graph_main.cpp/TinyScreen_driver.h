#include <TinyScreen.h>

#define DISPLAY_WIDTH   96 // pixels
#define DISPLAY_HEIGHT  64 // pixels

#define DISPLAY_GRAPH_WIDTH             60 // pixels
#define DISPLAY_GRAPH_HEIGHT            38 // pixels
#define DISPLAY_GRAPH_BAR_WIDTH         2  // pixels
#define DISPLAY_GRAPH_MIN_BAR_HEIGHT    1  // pixels
#define DISPLAY_GRAPH_MAX_BAR_HEIGHT    38 // pixels
#define DISPLAY_GRAPH_BAR_PADDING_LOW   1
#define DISPLAY_GRAPH_BAR_PADDING_HIGH  1

#define DANGER_HIGH 300.0
#define WARNING_HIGH 180.0
#define WARNING_LOW 70.0
#define DANGER_LOW 50.0

#define DISPLAY_GRAPH_X 32
#define DISPLAY_GRAPH_Y 14

#define PIXEL_WHITE   TS_8b_White
#define PIXEL_YELLOW  TS_8b_Yellow
#define PIXEL_RED     TS_8b_Red
#define PIXEL_OFF     TS_8b_Black

#define MAX_WRITE_ID 10

void setDisplay(TinyScreen display);
void screen_on();
void screen_off();
void display_set_graph_pixel(int x, int y, int color);
void writeText(int x, int y, char* text, unsigned int replace_last);
void display_reset_graph();
