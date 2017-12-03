/* UConn Senior Design Team 1814, November 2017
     Using code modified from: ssd1306.c
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#include "st7735s.h"

struct spi_module st7735s_master;
struct spi_slave_inst st7735s_slave;

static void st7735s_interface_init(void) {
    struct spi_config config;
    struct spi_slave_inst_config slave_config;

    spi_slave_inst_get_config_defaults(&slave_config);
    slave_config.ss_pin = ST7735S_CS_PIN;
    spi_attach_slave(&st7735s_slave, &slave_config);

    spi_get_config_defaults(&config);

    config.mux_setting = ST7735S_SPI_PINMUX_SETTING;
    config.pinmux_pad0 = ST7735S_SPI_PINMUX_PAD0;
    config.pinmux_pad1 = ST7735S_SPI_PINMUX_PAD1;
    config.pinmux_pad2 = ST7735S_SPI_PINMUX_PAD2;
    config.pinmux_pad3 = ST7735S_SPI_PINMUX_PAD3;
    config.mode_specific.master.baudrate = ST7735S_CLOCK_SPEED;

    spi_init(&st7735s_master, ST7735S_SPI, &config);
    spi_enable(&st7735s_master);

    struct port_config pin;
    port_get_config_defaults(&pin);
    pin.direction = PORT_PIN_DIR_OUTPUT;

    port_pin_set_config(ST7735S_DC_PIN, &pin);
    port_pin_set_config(ST7735S_RES_PIN, &pin);
}

void st7735s_init(void) {
    // Initialize delay routine
    delay_init();
    // Do a hard reset of the OLED display controller
    st7735s_hard_reset();

    // Initialize the interface
    st7735s_interface_init();

    // Set the reset pin to the default state
    port_pin_set_output_level(ST7735S_RES_PIN, true);

    // Exit sleep modes
    st7735s_sleep_disable();

    st7735s_write_command(ST7735S_CMD_FRAME_CTRL_1); // Frame rate 80Hz
    st7735s_write_data(0x02);
    st7735s_write_data(0x35);
    st7735s_write_data(0x36);
    st7735s_write_command(ST7735S_CMD_FRAME_CTRL_2);
    st7735s_write_data(0x02);
    st7735s_write_data(0x35);
    st7735s_write_data(0x36);
    st7735s_write_command(ST7735S_CMD_FRAME_CTRL_3);
    st7735s_write_data(0x02);
    st7735s_write_data(0x35);
    st7735s_write_data(0x36);
    st7735s_write_data(0x02);
    st7735s_write_data(0x35);
    st7735s_write_data(0x36);

    st7735s_write_command(ST7735S_CMD_INVERSION_CTRL); // Column inversion
    st7735s_write_data(0x07);

    st7735s_write_command(ST7735S_CMD_POWER_CTRL_1); // Power Sequence
    st7735s_write_data(0xa2);
    st7735s_write_data(0x02);
    st7735s_write_data(0x84);
    st7735s_write_command(ST7735S_CMD_POWER_CTRL_2);
    st7735s_write_data(0x45);
    st7735s_write_command(ST7735S_CMD_POWER_CTRL_3);
    st7735s_write_data(0x0a);
    st7735s_write_data(0x00);
    st7735s_write_command(ST7735S_CMD_POWER_CTRL_4);
    st7735s_write_data(0x8a);
    st7735s_write_data(0x2a);
    st7735s_write_command(ST7735S_CMD_POWER_CTRL_5);
    st7735s_write_data(0x8a);
    st7735s_write_data(0xee);

    st7735s_write_command(ST7735S_CMD_VCOM_CTRL); // VCOM
    st7735s_write_data(0x03);

    st7735s_write_command(ST7735S_CMD_MEM_DATA_ACCESS_CONTROL);
    st7735s_write_data(0xC8);

    st7735s_write_command(ST7735S_CMD_GAMMA_P_CTRL); // Gamma Sequence
    st7735s_write_data(0x12);
    st7735s_write_data(0x1c);
    st7735s_write_data(0x10);
    st7735s_write_data(0x18);
    st7735s_write_data(0x33);
    st7735s_write_data(0x2c);
    st7735s_write_data(0x25);
    st7735s_write_data(0x28);
    st7735s_write_data(0x28);
    st7735s_write_data(0x27);
    st7735s_write_data(0x2f);
    st7735s_write_data(0x3c);
    st7735s_write_data(0x00);
    st7735s_write_data(0x03);
    st7735s_write_data(0x03);
    st7735s_write_data(0x10);
    st7735s_write_command(ST7735S_CMD_GAMMA_N_CTRL);
    st7735s_write_data(0x12);
    st7735s_write_data(0x1c);
    st7735s_write_data(0x10);
    st7735s_write_data(0x18);
    st7735s_write_data(0x2d);
    st7735s_write_data(0x28);
    st7735s_write_data(0x23);
    st7735s_write_data(0x28);
    st7735s_write_data(0x28);
    st7735s_write_data(0x26);
    st7735s_write_data(0x2f);
    st7735s_write_data(0x3d);
    st7735s_write_data(0x00);
    st7735s_write_data(0x03);
    st7735s_write_data(0x03);
    st7735s_write_data(0x10);

    st7735s_write_command(ST7735S_CMD_SET_PIXEL_FORMAT); // 65k mode
    st7735s_write_data(0x05);

    st7735s_write_command(ST7735S_CMD_SET_DISPLAY_ON); // Display on

    st7735s_write_command(ST7735S_CMD_WRITE_RAM);
}

void st7735s_write_command(uint8_t command) {
    spi_select_slave(&st7735s_master, &st7735s_slave, true);
    port_pin_set_output_level(ST7735S_DC_PIN, false);
    spi_write_buffer_wait(&st7735s_master, &command, 1);
    spi_select_slave(&st7735s_master, &st7735s_slave, false);
}

void st7735s_write_data(uint8_t data) {
    spi_select_slave(&st7735s_master, &st7735s_slave, true);
    port_pin_set_output_level(ST7735S_DC_PIN, true);
    spi_write_buffer_wait(&st7735s_master, &data, 1);
    spi_select_slave(&st7735s_master, &st7735s_slave, false);
}
