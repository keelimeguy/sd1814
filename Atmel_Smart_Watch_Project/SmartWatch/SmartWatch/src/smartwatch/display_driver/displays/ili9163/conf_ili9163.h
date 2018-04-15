/* UConn Senior Design Team 1814, November 2017
     Using code modified from: conf_ssd1306.h
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#ifndef CONF_ILI9163_H
#define CONF_ILI9163_H

#include "../../display_driver.h"

#define ILI9163_SPI                 BOARD_DISP_SERCOM
#define CONFIG_ILI9163_FRAMEBUFFER

#define ILI9163_DC_PIN              BOARD_DISP_DC_PIN
#define ILI9163_RES_PIN             BOARD_DISP_RES_PIN
#define ILI9163_CS_PIN              BOARD_DISP_CS_PIN

#define ILI9163_X_OFF               0
#define ILI9163_Y_OFF               0

#define ILI9163_SPI_PINMUX_SETTING  BOARD_DISP_SERCOM_SETTING
#define ILI9163_SPI_PINMUX_PAD0     PINMUX_PA16C_SERCOM1_PAD0
#define ILI9163_SPI_PINMUX_PAD1     PINMUX_PA17C_SERCOM1_PAD1
#define ILI9163_SPI_PINMUX_PAD2     PINMUX_DEFAULT
#define ILI9163_SPI_PINMUX_PAD3     PINMUX_DEFAULT

/* Minimum clock period is 150ns@3.3V -> max frequency is 6.66MHz */
#define ILI9163_CLOCK_SPEED           1000000UL

#endif /* CONF_ILI9163_H */
