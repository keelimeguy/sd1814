/* UConn Senior Design Team 1814, November 2017
*/

#ifndef BLUETOOTH_DRIVER_H
#define BLUETOOTH_DRIVER_H

#include <SmartWatch.h>

void bluetooth_driver_init(void);
uint8_t is_bt_active(void);
void bt_task(void);

#endif
