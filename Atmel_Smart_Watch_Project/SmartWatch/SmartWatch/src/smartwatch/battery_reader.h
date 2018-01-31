/* UConn Senior Design Team 1814, January 2018
*/

#ifndef BATTERY_READER_H
#define BATTERY_READER_H

#include <SmartWatch.h>

#define MAX_ADC     0xFF

void battery_reader_init(void);
void battery_task(void);
void start_battery_read(void);
uint8_t is_battery_active(void);
int get_battery_level(int max);

#endif
