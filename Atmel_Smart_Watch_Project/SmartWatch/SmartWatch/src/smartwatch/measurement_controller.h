/* UConn Senior Design Team 1814, November 2017
*/

#ifndef MEASUREMENT_CONTROLLER_H
#define MEASUREMENT_CONTROLLER_H

#include <SmartWatch.h>

void measurement_controller_init(void);
void take_measurement(void);
float get_measurement(void);
uint8_t is_new_measurement_soft(void);
uint8_t is_new_measurement(void);

#endif
