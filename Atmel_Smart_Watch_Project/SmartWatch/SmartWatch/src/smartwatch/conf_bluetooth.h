/* UConn Senior Design Team 1814, November 2017
*/

#ifndef CONF_BLUETOOTH_H
#define CONF_BLUETOOTH_H

#include <SmartWatch.h>

#define BT_SPI                 BOARD_BT_SERCOM

#define BT_RES_PIN             BOARD_BT_RES_PIN
#define BT_REQ_PIN             BOARD_BT_REQ_PIN
#define BT_RDY_PIN             BOARD_BT_RDY_PIN
#define BT_RDY_MUX             BOARD_BT_RDY_MUX
#define BT_RDY_EIC             BOARD_EIC_BLE

#define BT_SPI_PINMUX_SETTING  BOARD_BT_SERCOM_SETTING
#define BT_SPI_PINMUX_PAD0     PINMUX_PA08C_SERCOM0_PAD0
#define BT_SPI_PINMUX_PAD1     PINMUX_PA09C_SERCOM0_PAD1
#define BT_SPI_PINMUX_PAD2     PINMUX_DEFAULT
#define BT_SPI_PINMUX_PAD3     PINMUX_PA11C_SERCOM0_PAD3

#define BT_CLOCK_SPEED         1000000UL
#define BT_DATA_ORDER		   SPI_DATA_ORDER_LSB

#endif
