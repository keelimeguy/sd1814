/* UConn Senior Design Team 1814, November 2017
*/

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <SmartWatch.h>

#include "display_driver/GFXfont/Fonts/FreeSans9pt7b.h"
#include "display_driver/GFXfont/Fonts/FreeSans12pt7b.h"
#include "display_driver/GFXfont/Fonts/FreeSans24pt7b.h"

#define FONT_9PT                 FreeSans9pt7b
#define FONT_12PT                FreeSans12pt7b
#define FONT_24PT                FreeSans24pt7b

#define FONT_DEFAULT             FONT_9PT

#define CLR_BUTTON               BUTTON_L_VAL
#define GRAPH_BUTTON             BUTTON_L_VAL
#define VIEW_BUTTON              BUTTON_R_VAL

#define DISP_STATE_HOME          0
#define DISP_STATE_GRAPH         1
#define DISP_STATE_NOTIFICATION  2

void display_manager_init(void);
void display_ui_task(uint8_t button);

#endif
