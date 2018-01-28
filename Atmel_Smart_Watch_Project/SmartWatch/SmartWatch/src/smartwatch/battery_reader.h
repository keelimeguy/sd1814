/* UConn Senior Design Team 1814, January 2018
*/

#ifndef BATTERY_READER_H
#define BATTERY_READER_H

#include <SmartWatch.h>

void battery_reader_init(void);
void battery_task(void);
int get_battery_level(int max);

#endif
