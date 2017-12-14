/* UConn Senior Design Team 1814, November 2017
*/

#ifndef CONF_BLUETOOTH_H
#define CONF_BLUETOOTH_H

#include <SmartWatch.h>

#define BT_SPI                 BOARD_BT_SERCOM

#define BT_RES_PIN             BOARD_BT_RES_PIN
#define BT_CS_PIN              BOARD_BT_CS_PIN

#define BT_SPI_PINMUX_SETTING  BOARD_BT_SERCOM_SETTING
#define BT_SPI_PINMUX_PAD0     PINMUX_DEFAULT
#define BT_SPI_PINMUX_PAD1     PINMUX_DEFAULT
#define BT_SPI_PINMUX_PAD2     PINMUX_DEFAULT
#define BT_SPI_PINMUX_PAD3     PINMUX_DEFAULT

#define BT_CLOCK_SPEED         9600UL

#endif
