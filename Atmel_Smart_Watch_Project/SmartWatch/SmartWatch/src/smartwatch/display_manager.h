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

#define DISP_HEADER_HEIGHT   24
#define DISP_DIVISIONS       4
#define DISP_DIVISION_HEIGHT (DISP_HEIGHT-DISP_HEADER_HEIGHT)/DISP_DIVISIONS

 // IDs must be > 0 and <= MAX_WRITE_ID defined in display_driver.h
#define NOTIFICATION_NUM_ID  1
#define NOTIFICATION_1_ID    2
#define NOTIFICATION_2_ID    3
#define HOUR_ID              4
#define MIN_ID               5
#define SEC_ID               6
#define DATE_ID              7
#define GLUCOSE_VAL_ID       8
#define DATA_TOP_ID          9
#define DATA_BOTTOM_ID       10

void display_manager_init(void);
void display_ui_task(uint8_t button);

#endif
