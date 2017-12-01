/* UConn Senior Design Team 1814, November 2017
*/

#ifndef CLOCK_DRIVER_H
#define CLOCK_DRIVER_H

#include <SmartWatch.h>

#define READING_TIMEOUT 0
#define SCREEN_TIMEOUT 0

void clock_driver_init(void);
uint8_t is_reading_timeout(void);
void set_reading_timeout(uint32_t val);
uint8_t is_screen_timeout(void);
void set_screen_timeout(uint32_t val);

#endif
