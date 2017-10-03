#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define DISPLAY_WIDTH                       128
#define DISPLAY_HEIGHT                      160

#define DISPLAY_GRAPH_WIDTH                 120
#define DISPLAY_GRAPH_HEIGHT                120
#define DISPLAY_GRAPH_BAR_WIDTH             2
#define DISPLAY_GRAPH_MIN_BAR_HEIGHT        1
#define DISPLAY_GRAPH_MAX_BAR_HEIGHT        120
#define DISPLAY_GRAPH_BAR_PADDING_LOW       1
#define DISPLAY_GRAPH_BAR_PADDING_HIGH      1

#define PIXEL_OFF       0x000
#define PIXEL_WHITE     0xfff
#define PIXEL_RED       0xf00
#define PIXEL_ORANGE    0xfa0

void display_reset_graph();
void display_set_graph_pixel(int x, int y, int value);

#endif
