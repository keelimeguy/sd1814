/* UConn Senior Design Team 1814, November 2017
*/

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <SmartWatch.h>
#include "display_driver/display_driver.h"

#define CLR_BUTTON               BUTTON_L_VAL
#define GRAPH_BUTTON             BUTTON_L_VAL
#define VIEW_BUTTON              BUTTON_R_VAL

#define DISP_STATE_HOME          0
#define DISP_STATE_GRAPH         1
#define DISP_STATE_NOTIFICATION  2

void display_manager_init(void);
void display_ui_task(uint8_t button);

#endif
