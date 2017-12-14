/* UConn Senior Design Team 1814, November 2017
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

#define BOARD_BT_SERCOM            SERCOM0                  // PA08:13, PA09:14, PA10:15, PA11:16
#define BOARD_BT_SERCOM_SETTING    SPI_SIGNAL_MUX_SETTING_D //   MOSI ,   SCLK ,    -   ,   MISO

#define BOARD_BT_RES_PIN           PIN_PB10 // 19
#define BOARD_BT_CS_PIN            PIN_PB11 // 20

#define BOARD_DISP_SERCOM          SERCOM1                  // PA16:25, PA17:26, PA18:27, PA19:28
#define BOARD_DISP_SERCOM_SETTING  SPI_SIGNAL_MUX_SETTING_A //   MOSI ,   SCLK ,    -   ,    -

#define BOARD_DISP_CS_PIN          PIN_PA20 // 29
#define BOARD_DISP_DC_PIN          PIN_PA21 // 30
#define BOARD_DISP_RES_PIN         PIN_PA22 // 31

#define BOARD_BUTTON_L_PIN         PIN_PA00 // 1
#define BOARD_BUTTON_R_PIN         PIN_PA01 // 2

//#define BOARD_LED_PIN_1            PIN_PB10 // 19
// etc

//#define BOARD_PHOTODIODE_PIN_1     PIN_PB11 // 20
// etc

// TODO: Other pins as they turn up

#endif // CONF_BOARD_H
