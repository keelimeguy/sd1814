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

#define BT_DISCONNECTED        0
#define BT_CONNECTED           1

void bluetooth_driver_init(void);
uint8_t is_bt_active_soft(void);
uint8_t is_bt_active(void);
void bt_task(void);
void bt_write(uint8_t *tx_data, uint16_t length);
uint8_t bt_amt_notifications(void);
void bt_clear_amt_notifications(void);
uint8_t bt_connection_state(void);

#endif
