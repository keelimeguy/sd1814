/* UConn Senior Design Team 1814, January 2018
*/

#ifndef GRAPHER_H
#define GRAPHER_H

#include <stdio.h>

#include "../display_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Comment out if not used */
// #define DEFAULT_MIN 0.0
// #define DEFAULT_MAX 10.0

#define GRAPH_WIDTH         DISP_GRAPH_WIDTH // pixels
#define BAR_WIDTH           DISP_GRAPH_BAR_WIDTH // pixels
#define MIN_BAR_HEIGHT      DISP_GRAPH_MIN_BAR_HEIGHT // pixels
#define MAX_BAR_HEIGHT      DISP_GRAPH_MAX_BAR_HEIGHT // pixels
#define BAR_PADDING_LOW     DISP_GRAPH_BAR_PADDING_LOW
#define BAR_PADDING_HIGH    DISP_GRAPH_BAR_PADDING_HIGH

#define GRAPH_PIXEL_ON(i, j)        disp_write_pixel_at(DISP_GRAPH_X+i, DISP_GRAPH_Y+DISP_GRAPH_HEIGHT-j-1, DISP_PIXEL_GRAPH_ON)
#define GRAPH_PIXEL_WARNING(i, j)   disp_write_pixel_at(DISP_GRAPH_X+i, DISP_GRAPH_Y+DISP_GRAPH_HEIGHT-j-1, DISP_PIXEL_GRAPH_WARNING)
#define GRAPH_PIXEL_DANGER(i, j)    disp_write_pixel_at(DISP_GRAPH_X+i, DISP_GRAPH_Y+DISP_GRAPH_HEIGHT-j-1, DISP_PIXEL_GRAPH_DANGER)
#define GRAPH_PIXEL_OFF(i, j)       disp_write_pixel_at(DISP_GRAPH_X+i, DISP_GRAPH_Y+DISP_GRAPH_HEIGHT-j-1, DISP_PIXEL_GRAPH_OFF)
#define GRAPH_NEXT_ROW()            0

#define GRAPH_RESET()               disp_fill_rect(DISP_GRAPH_X,DISP_GRAPH_Y,GRAPH_WIDTH-1,DISP_GRAPH_HEIGHT-1, DISP_PIXEL_GRAPH_OFF)

void reset_graph();
int add_to_graph(int val);
void graph(int clear);

unsigned char graph_changed();
int graph_length();
double graph_min();
double graph_max();

#ifdef __cplusplus
}
#endif

#endif
