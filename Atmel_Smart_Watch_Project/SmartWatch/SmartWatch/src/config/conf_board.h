/* UConn Senior Design Team 1814, January 2018
     Using code adapted from Atmel:

/**
 * \file
 *
 * \brief User board configuration template
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define BOARD_BT_SERCOM            SERCOM3                  // PA22:31, PA23:32, PA24:33, PA25:34
#define BOARD_BT_SERCOM_SETTING    SPI_SIGNAL_MUX_SETTING_D //   MOSI ,   SCLK ,    -   ,   MISO

#define BOARD_BT_RES_PIN           PIN_PA28 // 41
#define BOARD_BT_REQ_PIN           PIN_PA24 // 33
#define BOARD_BT_ACT_PIN           PIN_PA27 // 39

#define BOARD_BT_RDY_PIN           PIN_PA21 // 30
#define BOARD_BT_RDY_MUX           MUX_PA21A_EIC_EXTINT5
#define BOARD_EIC_BLE              5

#define BOARD_DISP_SERCOM          SERCOM1                  // PA16:25, PA17:26, PA18:27, PA19:28
#define BOARD_DISP_SERCOM_SETTING  SPI_SIGNAL_MUX_SETTING_A //   MOSI ,   SCLK ,    -   ,    -

#define BOARD_DISP_CS_PIN          PIN_PA20 // 29
#define BOARD_DISP_DC_PIN          PIN_PA18 // 27
#define BOARD_DISP_RES_PIN         PIN_PA19 // 28
#define BOARD_DISP_BACKLIGHT_PIN   PIN_PA15 // 24

#define BOARD_BUTTON_L_PIN         PIN_PA10 // 15
#define BOARD_BUTTON_L_MUX         MUX_PA10A_EIC_EXTINT10
#define BOARD_EIC_L_BUTTON         10

#define BOARD_BUTTON_R_PIN         PIN_PA11 // 16
#define BOARD_BUTTON_R_MUX         MUX_PA11A_EIC_EXTINT11
#define BOARD_EIC_R_BUTTON         11

#define BOARD_BATTERY_DIVIDER_PIN  PIN_PA05 // 10

#define BOARD_PHOTODIODE_PIN       PIN_PA07 // 12
#define BOARD_PHOTODIODE_MUX       MUX_PA07A_EIC_EXTINT7
#define BOARD_PHOTODIODE_GEN       EVSYS_ID_GEN_EIC_EXTINT_7
#define BOARD_EIC_PHOTODIODE       7

#define BOARD_LED_PIN              PIN_PA08 // 13

#define BOARD_DEBUG_LED            BOARD_LED_PIN

#endif // CONF_BOARD_H
