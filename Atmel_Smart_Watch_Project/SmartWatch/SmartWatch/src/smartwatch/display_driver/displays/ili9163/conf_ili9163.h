/* UConn Senior Design Team 1814, November 2017
     Using code modified from: conf_ssd1306.h
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#ifndef CONF_ILI9163_H
#define CONF_ILI9163_H

#include <board.h>

#define ILI9163_SPI                 SERCOM0
#define CONFIG_ILI9163_FRAMEBUFFER

#define ILI9163_DC_PIN              0
#define ILI9163_RES_PIN             0
#define ILI9163_CS_PIN              0

#define ILI9163_SPI_PINMUX_SETTING  SPI_SIGNAL_MUX_SETTING_D
#define ILI9163_SPI_PINMUX_PAD0     PINMUX_DEFAULT
#define ILI9163_SPI_PINMUX_PAD1     PINMUX_DEFAULT
#define ILI9163_SPI_PINMUX_PAD2     PINMUX_DEFAULT
#define ILI9163_SPI_PINMUX_PAD3     PINMUX_DEFAULT

/* Minimum clock period is 150ns@3.3V -> max frequency is 6.66MHz */
#define ILI9163_CLOCK_SPEED           1000000UL

#endif /* CONF_ILI9163_H */
