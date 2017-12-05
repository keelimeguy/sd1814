/* UConn Senior Design Team 1814, November 2017
*/

#ifndef SMARTWATCH_H
#define SMARTWATCH_H

#include <ASF.h>

#ifndef PROGMEM
#define PROGMEM
#endif

#include "smartwatch/clock_driver.h"
#include "smartwatch/display_manager.h"
#include "smartwatch/button_listener.h"
#include "smartwatch/bluetooth_driver.h"
#include "smartwatch/measurement_controller.h"

void init_all(void);
void smartwatch_task(void);
uint8_t is_active(void);
uint8_t is_screen_active(void);
uint8_t is_screen_active_soft(void);
void sleep(void);
void wakeup(void);

#endif
