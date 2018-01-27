/* UConn Senior Design Team 1814, November 2017
*/

#ifndef BLUETOOTH_DRIVER_H
#define BLUETOOTH_DRIVER_H

#include "conf_bluetooth.h"
#ifndef CONSOLE_VERSION
	#include "UART.h"
#else
	#include <string.h>
	#define HAL_ACI_MAX_LENGTH 16
#endif

#define BT_CMD_TEST                                 0x01
#define BT_CMD_ECHO                                 0x02
#define BT_CMD_DTMCOMMAND                           0x03
#define BT_CMD_SLEEP                                0x04
#define BT_CMD_WAKEUP                               0x05
#define BT_CMD_SETUP                                0x06
#define BT_CMD_READ_DYN_DATA                        0x07
#define BT_CMD_WRITE_DYN_DATA                       0x08
#define BT_CMD_GET_DEVICE_VERSION                   0x09
#define BT_CMD_GET_DEVICE_ADDRESS                   0x0A
#define BT_CMD_GET_BATTERY                          0x0B
#define BT_CMD_GET_TEMPERATURE                      0x0C
#define BT_CMD_RADIO_RESET                          0x0E
#define BT_CMD_CONNECT                              0x0F
#define BT_CMD_BOND                                 0x10
#define BT_CMD_DISCONNECT                           0x11
#define BT_CMD_SET_TX_POWER                         0x12
#define BT_CMD_CHANGE_TIMING_REQUEST                0x13
#define BT_CMD_OPEN_REMOTE_PIPE                     0x14
#define BT_CMD_SET_APP_LATENCY                      0x19
#define BT_CMD_SET_KEY                              0x1A
#define BT_CMD_OPEN_ADV_PIPE                        0x1B
#define BT_CMD_BROADCAST                            0x1C
#define BT_CMD_BOND_SEC_REQUEST                     0x1D
#define BT_CMD_DIRECTED_CONNECT                     0x1E
#define BT_CMD_CLOSE_REMOTE_PIPE                    0x1F

#define BT_EVENT_DEVICE_STARTED                     0x81
#define BT_EVENT_ECHO                               0x82
#define BT_EVENT_HARDWARE_ERROR                     0x83
#define BT_EVENT_COMMAND_RESPONSE                   0x84
#define BT_EVENT_CONNECTED                          0x85
#define BT_EVENT_DISCONNECTED                       0x86
#define BT_EVENT_BOND_STATUS                        0x87
#define BT_EVENT_PIPE_STATUS                        0x88
#define BT_EVENT_TIMING                             0x89
#define BT_EVENT_DISPLAY_KEY                        0x8E
#define BT_EVENT_KEY_REQUEST                        0x8F

#define BT_MAX_MSG_LENGTH         HAL_ACI_MAX_LENGTH+1   // Maximum length, in bytes, of BT packet
#define BT_MAX_BUFFER_LENGTH      10                     // Maximum amount of queued messages

#define BT_DISCONNECTED        0
#define BT_CONNECTED           1

void bluetooth_driver_init(void);
uint8_t is_bt_active_soft(void);
uint8_t is_bt_active(void);
void bt_task(void);
uint8_t bt_write(uint8_t *tx_data, uint16_t length);
uint8_t bt_amt_notifications(void);
void bt_clear_amt_notifications(void);
char* bt_get_notification_1(void);
char* bt_get_notification_2(void);
void bt_set_notification_1(char* str);
void bt_set_notification_2(char* str);
uint8_t bt_connection_state(void);
void bt_set_connection_state(uint8_t state);
void set_ble_rx_buffer(int i, uint8_t val);
void bt_read_callback();
void bt_write_callback();

#endif
