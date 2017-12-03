/* UConn Senior Design Team 1814, November 2017
*/

#ifndef BLUETOOTH_DRIVER_H
#define BLUETOOTH_DRIVER_H

#include <SmartWatch.h>

#define BT_MAX_RX_BUFFER_LENGTH   2

#define BT_USART                 BOARD_BT_SERCOM

#define BT_USART_PINMUX_SETTING  BOARD_BT_SERCOM_SETTING
#define BT_USART_PINMUX_PAD0     PINMUX_DEFAULT
#define BT_USART_PINMUX_PAD1     PINMUX_DEFAULT
#define BT_USART_PINMUX_PAD2     PINMUX_DEFAULT
#define BT_USART_PINMUX_PAD3     PINMUX_DEFAULT

#define BT_USART_BAUDRATE        9600UL

void bluetooth_driver_init(void);
uint8_t is_bt_active_soft(void);
uint8_t is_bt_active(void);
void bt_task(void);
void bt_write(uint8_t *tx_data, uint16_t length);

#endif
