/* UConn Senior Design Team 1814, November 2017
     Using code modified from: conf_ssd1306.h
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#ifndef CONF_ST7735S_H
#define CONF_ST7735S_H

#include <asf.h>

#define ST7735S_SPI                 BOARD_DISP_SERCOM
#define CONFIG_ST7735S_FRAMEBUFFER

#define ST7735S_DC_PIN              BOARD_DISP_DC_PIN
#define ST7735S_RES_PIN             BOARD_DISP_RES_PIN
#define ST7735S_CS_PIN              BOARD_DISP_CS_PIN

#define ST7735S_X_OFF               2
#define ST7735S_Y_OFF               3

#define ST7735S_SPI_PINMUX_SETTING  BOARD_DISP_SERCOM_SETTING
#define ST7735S_SPI_PINMUX_PAD0     PINMUX_PA16C_SERCOM1_PAD0
#define ST7735S_SPI_PINMUX_PAD1     PINMUX_PA17C_SERCOM1_PAD1
#define ST7735S_SPI_PINMUX_PAD2     PINMUX_DEFAULT
#define ST7735S_SPI_PINMUX_PAD3     PINMUX_DEFAULT

/* Minimum clock period is 150ns@3.3V -> max frequency is 6.66MHz */
#define ST7735S_CLOCK_SPEED           4000000UL

#endif /* CONF_ST7735S_H */
