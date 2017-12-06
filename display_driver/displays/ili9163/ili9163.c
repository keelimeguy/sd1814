/* UConn Senior Design Team 1814, November 2017
     Using code modified from: ssd1306.c
        which was provided by Atmel:

   Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
*/

#include "ili9163.h"

struct spi_module ili9163_master;
struct spi_slave_inst ili9163_slave;

static void ili9163_interface_init(void) {
    struct spi_config config;
    struct spi_slave_inst_config slave_config;

    spi_slave_inst_get_config_defaults(&slave_config);
    slave_config.ss_pin = ILI9163_CS_PIN;
    spi_attach_slave(&ili9163_slave, &slave_config);

    spi_get_config_defaults(&config);

    config.mux_setting = ILI9163_SPI_PINMUX_SETTING;
    config.pinmux_pad0 = ILI9163_SPI_PINMUX_PAD0;
    config.pinmux_pad1 = ILI9163_SPI_PINMUX_PAD1;
    config.pinmux_pad2 = ILI9163_SPI_PINMUX_PAD2;
    config.pinmux_pad3 = ILI9163_SPI_PINMUX_PAD3;
    config.mode_specific.master.baudrate = ILI9163_CLOCK_SPEED;

    spi_init(&ili9163_master, ILI9163_SPI, &config);
    spi_enable(&ili9163_master);

    struct port_config pin;
    port_get_config_defaults(&pin);
    pin.direction = PORT_PIN_DIR_OUTPUT;

    port_pin_set_config(ILI9163_DC_PIN, &pin);
    port_pin_set_config(ILI9163_RES_PIN, &pin);
}

void ili9163_init(void) {
    // Initialize delay routine
    delay_init();
    // Do a hard reset of the OLED display controller
    ili9163_hard_reset();

    // Initialize the interface
    ili9163_interface_init();

    // Set the reset pin to the default state
    port_pin_set_output_level(ILI9163_RES_PIN, true);

    // Exit sleep modes
    ili9163_sleep_disable();

    ili9163_write_command(ILI9163_CMD_GAMMA_SELECT);  // Set Default Gamma
    ili9163_write_data(0x04);
    ili9163_write_command(ILI9163_CMD_FRAME_CTRL_1);
    ili9163_write_data(0x08); // 10
    ili9163_write_data(0x10); // 08
    ili9163_write_command(ILI9163_CMD_POWER_CTRL_1);  // Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
    ili9163_write_data(0x0C);
    ili9163_write_data(0x05);
    ili9163_write_command(ILI9163_CMD_POWER_CTRL_2);  // Set BT[2:0] for AVDD & VCL & VGH & VGL
    ili9163_write_data(0x02);
    ili9163_write_command(ILI9163_CMD_VCOM_CTRL_1);  // Set VMH[6:0] & VML[6:0] for VOMH & VCOML
    ili9163_write_data(0x4E);
    ili9163_write_data(0x30);
    ili9163_write_command(ILI9163_CMD_VCOM_OFFSET_CTRL);
    ili9163_write_data(0xc0);     // offset=0 // C0
    ili9163_write_command(ILI9163_CMD_SET_PIXEL_FORMAT);  // Set Color Format
    ili9163_write_data(0x05);
    ili9163_write_command(ILI9163_CMD_SET_COLUMN_ADDRESS);  // Set Column Address
    ili9163_write_data(0x00);
    ili9163_write_data(0x00);
    ili9163_write_data(0x00);
    ili9163_write_data(0x7F);
    ili9163_write_command(ILI9163_CMD_SET_ROW_ADDRESS);  // Set Page Address
    ili9163_write_data(0x00);
    ili9163_write_data(0x00);
    ili9163_write_data(0x00);
    ili9163_write_data(0x9F);
    // ili9163_write_command(ILI9163_CMD_INVERSION_CTRL);  // frame inversion
    // ili9163_write_data(0x07);
    ili9163_write_command(ILI9163_CMD_MEM_DATA_ACCESS_CONTROL);  // Set Scanning Direction
    ili9163_write_data(0xC0);
    // ili9163_write_command(ILI9163_CMD_SET_GATE_PUMP_CLK_FREQ);  // Set pumping clock frequency
    // ili9163_write_data(0x0B);
    ili9163_write_command(ILI9163_CMD_GAMMA_R_SEL);  // Enable Gamma bit
    ili9163_write_data(0x01);

    ili9163_write_command(ILI9163_CMD_GAMMA_P_CTRL);
    ili9163_write_data(0x3F); // p1        // xx VP63[5:0]
    ili9163_write_data(0x31); // p2        // xx VP62[5:0]
    ili9163_write_data(0x2D); // p3        // xx VP61[5:0]
    ili9163_write_data(0x2F); // p4        // xx VP59[5:0]
    ili9163_write_data(0x28); // p5        // xx VP57[5:0]
    ili9163_write_data(0x0D); // p6        // xxx VP50[4:0]
    ili9163_write_data(0x59); // p7        // x VP43[6:0]
    ili9163_write_data(0xA8); // p8        // VP36[3:0] VP27[3:0]
    ili9163_write_data(0x44); // p9        // x VP20[6:0]
    ili9163_write_data(0x18); // p10       // xxx VP13[4:0]
    ili9163_write_data(0x1F); // p11       // xx VP6[5:0]
    ili9163_write_data(0x10); // p12       // xx VP4[5:0]
    ili9163_write_data(0x07); // p13       // xx VP2[5:0]
    ili9163_write_data(0x02); // p14       // xx VP1[5:0]
    ili9163_write_data(0x00); // p15       // xx VP0[5:0]
    ili9163_write_command(ILI9163_CMD_GAMMA_N_CTRL);
    ili9163_write_data(0x00); // p1        // xx VN0[5:0]
    ili9163_write_data(0x0E); // p2        // xx VN1[5:0]
    ili9163_write_data(0x12); // p3        // xx VN2[5:0]
    ili9163_write_data(0x10); // p4        // xx VN4[5:0]
    ili9163_write_data(0x17); // p5        // xx VN6[5:0]
    ili9163_write_data(0x12); // p6        // xxx VN13[4:0]
    ili9163_write_data(0x26); // p7        // x VN20[6:0]
    ili9163_write_data(0x57); // p8        // VN36[3:0] VN27[3:0]
    ili9163_write_data(0x3B); // p9        // x VN43[6:0]
    ili9163_write_data(0x07); // p10       // xxx VN50[4:0]
    ili9163_write_data(0x20); // p11       // xx VN57[5:0]
    ili9163_write_data(0x2F); // p12       // xx VN59[5:0]
    ili9163_write_data(0x38); // p13       // xx VN61[5:0]
    ili9163_write_data(0x3D); // p14       // xx VN62[5:0]
    ili9163_write_data(0x3f); // p15       // xx VN63[5:0]

    // ili9163_write_command(ILI9163_CMD_GAMMA_P_CTRL);
    // ili9163_write_data(0x36); // p1
    // ili9163_write_data(0x29); // p2
    // ili9163_write_data(0x12); // p3
    // ili9163_write_data(0x22); // p4
    // ili9163_write_data(0x1C); // p5
    // ili9163_write_data(0x15); // p6
    // ili9163_write_data(0x42); // p7
    // ili9163_write_data(0xB7); // p8
    // ili9163_write_data(0x2F); // p9
    // ili9163_write_data(0x13); // p10
    // ili9163_write_data(0x12); // p11
    // ili9163_write_data(0x0A); // p12
    // ili9163_write_data(0x11); // p13
    // ili9163_write_data(0x0B); // p14
    // ili9163_write_data(0x06); // p15
    // ili9163_write_command(ILI9163_CMD_GAMMA_N_CTRL);
    // ili9163_write_data(0x09); // p1
    // ili9163_write_data(0x16); // p2
    // ili9163_write_data(0x2D); // p3
    // ili9163_write_data(0x0D); // p4
    // ili9163_write_data(0x13); // p5
    // ili9163_write_data(0x15); // p6
    // ili9163_write_data(0x40); // p7
    // ili9163_write_data(0x48); // p8
    // ili9163_write_data(0x53); // p9
    // ili9163_write_data(0x0C); // p10
    // ili9163_write_data(0x1D); // p11
    // ili9163_write_data(0x25); // p12
    // ili9163_write_data(0x2E); // p13
    // ili9163_write_data(0x34); // p14
    // ili9163_write_data(0x39); // p15
}

void ili9163_write_command(uint8_t command) {
    spi_select_slave(&ili9163_master, &ili9163_slave, true);
    port_pin_set_output_level(ILI9163_DC_PIN, false);
    spi_write_buffer_wait(&ili9163_master, &command, 1);
    spi_select_slave(&ili9163_master, &ili9163_slave, false);
}

void ili9163_write_data(uint8_t data) {
    spi_select_slave(&ili9163_master, &ili9163_slave, true);
    port_pin_set_output_level(ILI9163_DC_PIN, true);
    spi_write_buffer_wait(&ili9163_master, &data, 1);
    spi_select_slave(&ili9163_master, &ili9163_slave, false);
}
