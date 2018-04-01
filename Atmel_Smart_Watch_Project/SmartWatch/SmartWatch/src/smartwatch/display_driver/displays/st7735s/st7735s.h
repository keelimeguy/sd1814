/* UConn Senior Design Team 1814, November 2017
     Using code modified from: ssd1306.h
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#ifndef ST7735S_H
#define ST7735S_H

#include <compiler.h>
#include <port.h>
#include <spi.h>
#include <delay.h>

#include "conf_st7735s.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define ST7735S_CMD_COL_ADD_SET_LSB(column)         (0x00 | (column))
// #define ST7735S_CMD_COL_ADD_SET_MSB(column)         (0x10 | (column))
#define ST7735S_CMD_NOP                             0x00
#define ST7735S_CMD_SOFT_RESET                      0x01
#define ST7735S_CMD_READ_DISP_ID                    0x04
#define ST7735S_CMD_READ_DISP_STATUS                0x09
#define ST7735S_CMD_READ_DISP_POWER_MODE            0x0A
#define ST7735S_CMD_READ_DISP_MADCTL                0x0B
#define ST7735S_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ST7735S_CMD_READ_DISP_IMAGE_MODE            0x0D
#define ST7735S_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ST7735S_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ST7735S_CMD_SLEEP_IN                        0x10
#define ST7735S_CMD_SLEEP_OUT                       0x11
#define ST7735S_CMD_PARTIAL_MODE_ON                 0x12
#define ST7735S_CMD_PARTIAL_MODE_OFF                0x13
#define ST7735S_CMD_SET_NORMAL_DISPLAY              0x20
#define ST7735S_CMD_SET_INVERSE_DISPLAY             0x21
#define ST7735S_CMD_GAMMA_SELECT                    0x26
#define ST7735S_CMD_SET_DISPLAY_OFF                 0x28
#define ST7735S_CMD_SET_DISPLAY_ON                  0x29
#define ST7735S_CMD_SET_COLUMN_ADDRESS              0x2A
#define ST7735S_CMD_SET_ROW_ADDRESS                 0x2B
#define ST7735S_CMD_WRITE_RAM                       0x2C
#define ST7735S_CMD_SET_RGB_LUT                     0x2D
#define ST7735S_CMD_READ_RAM                        0x2E
#define ST7735S_CMD_SET_PARTIAL_ADDRESS             0x30
#define ST7735S_CMD_SET_SCROLL_AREA                 0x33
#define ST7735S_CMD_TEARING_EFFECT_OFF              0x34
#define ST7735S_CMD_TEARING_EFFECT_MODE_SET         0x35
#define ST7735S_CMD_MEM_DATA_ACCESS_CONTROL         0x36
#define ST7735S_CMD_SET_SCROLL_START_ADDRESS        0x37
#define ST7735S_CMD_IDLE_MODE_OFF                   0x38
#define ST7735S_CMD_IDLE_MODE_ON                    0x39
#define ST7735S_CMD_SET_PIXEL_FORMAT                0x3A
#define ST7735S_CMD_FRAME_CTRL_1                    0xB1
#define ST7735S_CMD_FRAME_CTRL_2                    0xB2
#define ST7735S_CMD_FRAME_CTRL_3                    0xB3
#define ST7735S_CMD_INVERSION_CTRL                  0xB4
#define ST7735S_CMD_POWER_CTRL_1                    0xC0
#define ST7735S_CMD_POWER_CTRL_2                    0xC1
#define ST7735S_CMD_POWER_CTRL_3                    0xC2
#define ST7735S_CMD_POWER_CTRL_4                    0xC3
#define ST7735S_CMD_POWER_CTRL_5                    0xC4
#define ST7735S_CMD_VCOM_CTRL                       0xC5
#define ST7735S_CMD_VCOM_OFFSET_CTRL                0xC7
#define ST7735S_CMD_WRITE_ID2                       0xD1 // LCM Version Code
#define ST7735S_CMD_WRITE_ID3                       0xD2 // Customer Project Code
#define ST7735S_CMD_NVM_CTRL_1                      0xD9
#define ST7735S_CMD_READ_ID1                        0xDA
#define ST7735S_CMD_READ_ID2                        0xDB
#define ST7735S_CMD_READ_ID3                        0xDC
#define ST7735S_CMD_NVM_CTRL_2                      0xDE
#define ST7735S_CMD_NVM_CTRL_3                      0xDF
#define ST7735S_CMD_GAMMA_P_CTRL                    0xE0
#define ST7735S_CMD_GAMMA_N_CTRL                    0xE1
#define ST7735S_CMD_SET_GATE_PUMP_CLK_FREQ          0xFC

extern struct spi_module st7735s_master;
extern struct spi_slave_inst st7735s_slave;

void st7735s_write_command(uint8_t command);

void st7735s_write_data(uint8_t data);
void st7735s_begin_write_data(void);
void st7735s_write_data_continue(uint8_t data);
void st7735s_write_multiple_data_continue(uint8_t* data, uint16_t length);
void st7735s_end_write_data(void);

static inline void st7735s_display_on(void) {
    st7735s_write_command(ST7735S_CMD_SET_DISPLAY_ON); // Display On
    st7735s_write_command(ST7735S_CMD_WRITE_RAM);
}

static inline void st7735s_hard_reset(void) {
    port_pin_set_output_level(ST7735S_RES_PIN, false);
    delay_cycles_us(10); // At least 10us
    port_pin_set_output_level(ST7735S_RES_PIN, true);
    delay_cycles_ms(5); // At least 5ms
}

static inline void st7735s_sleep_enable(void) {
    st7735s_write_command(ST7735S_CMD_SLEEP_IN);
}

static inline void st7735s_sleep_disable(void) {
    st7735s_write_command(ST7735S_CMD_SLEEP_OUT);
    delay_cycles(500); // At least 120ms
}

static inline void st7735s_set_row_address(uint8_t start_address, uint8_t end_address) {
    // Max address 128 : 0x00-0x7f
    start_address+=ST7735S_Y_OFF;
    end_address+=ST7735S_Y_OFF;
    st7735s_write_command(ST7735S_CMD_SET_ROW_ADDRESS);
    st7735s_write_data(0);
    st7735s_write_data(start_address);
    st7735s_write_data(0);
    st7735s_write_data(end_address);
    st7735s_write_command(ST7735S_CMD_WRITE_RAM);
}

static inline void st7735s_set_column_address(uint8_t start_address, uint8_t end_address) {
    // Max address 128 : 0x00-0x7f
    start_address+=ST7735S_X_OFF;
    end_address+=ST7735S_X_OFF;
    st7735s_write_command(ST7735S_CMD_SET_COLUMN_ADDRESS);
    st7735s_write_data(0);
    st7735s_write_data(start_address);
    st7735s_write_data(0);
    st7735s_write_data(end_address);
    st7735s_write_command(ST7735S_CMD_WRITE_RAM);
}

void st7735s_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ST7735S_H */
