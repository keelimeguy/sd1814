/* UConn Senior Design Team 1814, November 2017
     Using code modified from: ssd1306.h
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#ifndef ILI9163_H
#define ILI9163_H

#include <compiler.h>
#include <port.h>
#include <spi.h>
#include <delay.h>

#include "conf_ili9163.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define ILI9163_CMD_COL_ADD_SET_LSB(column)         (0x00 | (column))
// #define ILI9163_CMD_COL_ADD_SET_MSB(column)         (0x10 | (column))
#define ILI9163_CMD_NOP                             0x00
#define ILI9163_CMD_SOFT_RESET                      0x01
#define ILI9163_CMD_READ_DISP_ID                    0x04
#define ILI9163_CMD_READ_DISP_STATUS                0x09
#define ILI9163_CMD_READ_DISP_POWER_MODE            0x0A
#define ILI9163_CMD_READ_DISP_MADCTL                0x0B
#define ILI9163_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ILI9163_CMD_READ_DISP_IMAGE_MODE            0x0D
#define ILI9163_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ILI9163_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ILI9163_CMD_SLEEP_IN                        0x10
#define ILI9163_CMD_SLEEP_OUT                       0x11
#define ILI9163_CMD_PARTIAL_MODE_ON                 0x12
#define ILI9163_CMD_PARTIAL_MODE_OFF                0x13
#define ILI9163_CMD_SET_NORMAL_DISPLAY              0x20
#define ILI9163_CMD_SET_INVERSE_DISPLAY             0x21
#define ILI9163_CMD_GAMMA_SELECT                    0x26
#define ILI9163_CMD_SET_DISPLAY_OFF                 0x28
#define ILI9163_CMD_SET_DISPLAY_ON                  0x29
#define ILI9163_CMD_SET_COLUMN_ADDRESS              0x2A
#define ILI9163_CMD_SET_ROW_ADDRESS                 0x2B
#define ILI9163_CMD_WRITE_RAM                       0x2C
#define ILI9163_CMD_SET_RGB_LUT                     0x2D
#define ILI9163_CMD_READ_RAM                        0x2E
#define ILI9163_CMD_SET_PARTIAL_ADDRESS             0x30
#define ILI9163_CMD_SET_SCROLL_AREA                 0x33
#define ILI9163_CMD_TEARING_EFFECT_OFF              0x34
#define ILI9163_CMD_TEARING_EFFECT_MODE_SET         0x35
#define ILI9163_CMD_MEM_DATA_ACCESS_CONTROL         0x36
#define ILI9163_CMD_SET_SCROLL_START_ADDRESS        0x37
#define ILI9163_CMD_IDLE_MODE_OFF                   0x38
#define ILI9163_CMD_IDLE_MODE_ON                    0x39
#define ILI9163_CMD_SET_PIXEL_FORMAT                0x3A
#define ILI9163_CMD_FRAME_CTRL_1                    0xB1
#define ILI9163_CMD_FRAME_CTRL_2                    0xB2
#define ILI9163_CMD_FRAME_CTRL_3                    0xB3
#define ILI9163_CMD_INVERSION_CTRL                  0xB4
#define ILI9163_CMD_RGB_INTERFACE_CTRL              0xB5
#define ILI9163_CMD_SET_DISPLAY_FUNCTION            0xB6
#define ILI9163_CMD_SRC_DRIVER_DIR_CTRL             0xB7
#define ILI9163_CMD_GATE_DRIVER_DIR_CTRL            0xB8
#define ILI9163_CMD_POWER_CTRL_1                    0xC0
#define ILI9163_CMD_POWER_CTRL_2                    0xC1
#define ILI9163_CMD_POWER_CTRL_3                    0xC2
#define ILI9163_CMD_POWER_CTRL_4                    0xC3
#define ILI9163_CMD_POWER_CTRL_5                    0xC4
#define ILI9163_CMD_VCOM_CTRL_1                     0xC5
#define ILI9163_CMD_VCOM_CTRL_2                     0xC6
#define ILI9163_CMD_VCOM_OFFSET_CTRL                0xC7
#define ILI9163_CMD_WRITE_ID4                       0xD3
#define ILI9163_CMD_NVM_CTRL_1                      0xD5
#define ILI9163_CMD_NVM_CTRL_2                      0xD6
#define ILI9163_CMD_NVM_CTRL_3                      0xD7
#define ILI9163_CMD_READ_ID1                        0xDA
#define ILI9163_CMD_READ_ID2                        0xDB
#define ILI9163_CMD_READ_ID3                        0xDC
#define ILI9163_CMD_GAMMA_P_CTRL                    0xE0
#define ILI9163_CMD_GAMMA_N_CTRL                    0xE1
#define ILI9163_CMD_SET_GATE_PUMP_CLK_FREQ          0xEC
#define ILI9163_CMD_GAMMA_R_SEL                     0xF2

extern struct spi_module ili9163_master;
extern struct spi_slave_inst ili9163_slave;

void ili9163_write_command(uint8_t command);

void ili9163_write_data(uint8_t data);
void ili9163_begin_write_data(void);
void ili9163_write_data_continue(uint8_t data);
void ili9163_write_multiple_data_continue(uint8_t* data, uint16_t length);
void ili9163_end_write_data(void);

static inline void ili9163_display_on(void) {
    ili9163_write_command(ILI9163_CMD_SET_DISPLAY_ON); // Display On
    ili9163_write_command(ILI9163_CMD_WRITE_RAM);
}

static inline void ili9163_hard_reset(void) {
    port_pin_set_output_level(ILI9163_RES_PIN, false);
    delay_cycles_us(10); // At least 10us
    port_pin_set_output_level(ILI9163_RES_PIN, true);
    delay_cycles_ms(5); // At least 5ms
}

static inline void ili9163_sleep_enable(void) {
    ili9163_write_command(ILI9163_CMD_SLEEP_IN);
    delay_cycles_ms(5); // At least 5ms
}

static inline void ili9163_sleep_disable(void) {
    ili9163_write_command(ILI9163_CMD_SLEEP_OUT);
    delay_cycles_ms(120); // At least 120ms
}

static inline void ili9163_set_row_address(uint8_t start_address, uint8_t end_address) {
    // Max address 160 : 0x00-0x9f
    start_address+=ILI9163_Y_OFF;
    end_address+=ILI9163_Y_OFF;
    ili9163_write_command(ILI9163_CMD_SET_ROW_ADDRESS);
    ili9163_write_data(0);
    ili9163_write_data(start_address);
    ili9163_write_data(0);
    ili9163_write_data(end_address);
    ili9163_write_command(ILI9163_CMD_WRITE_RAM);
}

static inline void ili9163_set_column_address(uint8_t start_address, uint8_t end_address) {
    // Max address 128 : 0x00-0x7f
    start_address+=ILI9163_X_OFF;
    end_address+=ILI9163_X_OFF;
    ili9163_write_command(ILI9163_CMD_SET_COLUMN_ADDRESS);
    ili9163_write_data(0);
    ili9163_write_data(start_address);
    ili9163_write_data(0);
    ili9163_write_data(end_address);
    ili9163_write_command(ILI9163_CMD_WRITE_RAM);
}

void ili9163_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ILI9163_H */
