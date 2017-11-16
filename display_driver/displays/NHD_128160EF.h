#ifndef NHD_128160EF_H
#define NHD_128160EF_H

/* Pin connections (Serial mode):
    1 (GND)
    2 (IOVDD) = VDD
    3 (VDD) = 2.8V, 3mA (2.5V-3.3V)
    4 (/CS), active low chip select
    5 (/RST), active low reset
    6 (D/C), data(1)/command(0) select
    7 (/WR), active low write
    8 (/RD), active low read
    9-16 (DB0-DB7), 8-bit bus
    17 (LED-A), backlight = 3.2V, 30mA (3.0V-3.4V)
    18 (LED-K1) = GND
    19 (LED-K2) = GND
    20 (GND)
    21 (YU) touch up
    22 (XL) touch left
    23 (YD) touch down
    24 (XR) touch right
*/

#define NHD_MAX_ROW                     0x7F
#define NHD_MAX_COL                     0x9F

#define NHD_NOP                         0x00
#define NHD_SOFT_RST                    0x01
#define NHD_GET_RED_CHANNEL             0x06
#define NHD_GET_GREEN_CHANNEL           0x07
#define NHD_GET_BLUE_CHANNEL            0x08
#define NHD_GET_PIXEL_FORMAT            0x0C
#define NHD_GET_POWER_MODE              0x0A
#define NHD_GET_ADDRESS_MODE            0x0B
#define NHD_GET_DISPLAY_MODE            0x0D
#define NHD_GET_SIGNAL_MODE             0x0E
#define NHD_GET_DIAGNOSTIC_RESULT       0x0F
#define NHD_ENTER_SLEEP_MODE            0x10 // wait 5ms before next command
#define NHD_EXIT_SLEEP_MODE             0x11 // wait 5ms before next command/ wait 120ms after exiting before entering sleep again
#define NHD_ENTER_PARTIAL_MODE          0x12
#define NHD_ENTER_NORMAL_MODE           0x13
#define NHD_EXIT_INVERT_MODE            0x20
#define NHD_ENTER_INVERT_MODE           0x21
#define NHD_SET_GAMMA_CURVE             0x26
#define NHD_SET_DISPLAY_OFF             0x28
#define NHD_SET_DISPLAY_ON              0x29
#define NHD_SET_COLUMN_ADDRESS          0x2A // 16-bit XStart, XEnd : XS15-XS8 -> XS7-XS0 -> XE15-XE8 -> XE7-XE0
#define NHD_SET_PAGE_ADDRESS            0x2B // 16-bit YStart, YEnd : YS15-YS8 -> YS7-YS0 -> YE15-YE8 -> YE7-YE0
#define NHD_WRITE_MEMORY_START          0x2C // Follow by memory writes: 18-bits:R5R4R3R2R1R0XX -> G -> B | 16-bits:R4R3R2R1R0G5G4G3 -> G2G1G0B4B3B2B1B0 | 12-bits:RG -> BR -> GB
#define NHD_WRITE_LUT                   0x2D
#define NHD_READ_MEMORY_START           0x2E
#define NHD_SET_PARTIAL_AREA            0x30
#define NHD_SET_TEAR_OFF                0x34
#define NHD_SET_TEAR_ON                 0x35
#define NHD_SET_ADDRESS_MODE            0x36
#define NHD_SET_SCROLL_START            0x37
#define NHD_EXIT_IDLE_MODE              0x38
#define NHD_ENTER_IDLE_MODE             0x39
#define NHD_SET_PIXEL_FORMAT            0x3A // xxxx0ABC : 011=12bit/pixel, 101=16bit, 110=18bit
#define NHD_WRITE_MEMORY_CONTINUE       0x3C
#define NHD_READ_MEMORY_CONTINUE        0x3E
#define NHD_SET_TEAR_SCANLINE           0x44
#define NHD_GET_SCANLINE                0x45
#define NHD_READ_ID1                    0xDA
#define NHD_READ_ID2                    0xDB
#define NHD_READ_ID3                    0xDC

/*
void NHD_Init(void) {
    GPIO_ResetBits(GPIOC, CS1);
    GPIO_SetBits(GPIOC, nRD);
    GPIO_ResetBits(GPIOC, nWR);
    GPIO_WriteBit(GPIOC, RES, Bit_RESET);
    delay(5);
    NHD_delay(10);
    GPIO_WriteBit(GPIOC, RES, Bit_SET);
    delay(100);
    NHD_delay(10);
    NHD_Write_Command(0x11);
    NHD_delay(100);
    NHD_Write_Command(0x26);NHD_(0x04);
    NHD_Write_Command(0xF2);NHD_(0x00);
    NHD_Write_Command(0xB1);NHD_(0x0A);NHD_Write_Data(0x14);
    NHD_Write_Command(0xC0);NHD_Write_Data(0x0A);NHD_Write_Data(0x00);
    NHD_Write_Command(0xC1);NHD_Write_Data(0x02);
    NHD_Write_Command(0xC5);NHD_Write_Data(0x2F);NHD_Write_Data(0x3E);
    NHD_Write_Command(0xC7);NHD_Write_Data(0x40);
    NHD_Write_Command(0x2A);
    NHD_Write_Data(0x00);
    NHD_Write_Data(0x00);
    NHD_Write_Data(0x00);
    NHD_Write_Data(0x7F);
    NHD_Write_Command(0x2B);
    NHD_Write_Data(0x00);
    NHD_Write_Data(0x00);
    NHD_Write_Data(0x00);
    NHD_Write_Data(0x9F);
    NHD_Write_Command(0x36);NHD_Write_Data(0x48);
    NHD_Write_Command(0x3A);NHD_Write_Data(0xC5);
    NHD_Write_Command(0x29);
    NHD_Write_Command(0x2C);
}

void NHD_Write_Command(unsigned char command) {
    GPIO_ResetBits(GPIOC, RS);
    GPIO_Write(GPIOB, command);
    GPIO_ResetBits(GPIOC, nWR);
    GPIO_SetBits(GPIOC, nWR);
}

void NHD_Write_Data(unsigned char data1) {
    GPIO_SetBits(GPIOC, RS);
    GPIO_Write(GPIOB, data1);
    GPIO_ResetBits(GPIOC, nWR);
    GPIO_SetBits(GPIOC, nWR);
}
*/

#endif
