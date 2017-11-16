#ifndef OLED4D_282815_H
#define OLED4D_282815_H

/* Pin connections (Serial mode):
    27 (VCI) = +V (-.3V -> 4V) typ:2.8V,240uA
    26 (VDD), cap between this and VSS (-.5V -> 2.75V) [internally regulated]
    4 (VDDIO) = VCI (-.5V -> VCI)
    28 (VSS) = GND (0V)
    2 (VCC) = +V (-.5V -> 16V) typ:13V, 30%=13.3mA 50%=23.2mA 100%=33.4mA

    22 (IREF), resister between this and VSS, 12.5uA max
    3 (VCOMH), tantalum cap between this and VSS
    5 (VSL), resistor and diode to ground (see command B4h for reference)

    24-23 (GPIO0-1), open
    17-18 (BS0-1), 3-wire=10, 4-wire=00

    21 (RES#), reset enabled low, keep high
    19 (CS#), chip select enabled low, keep low to enable MCU communication

    15 (E/RD#), Vss
    16 (R/W#), Vss
    20 (D/C#), data/command select, 3-wire=Vss
    7-11 (D7-3), Vss
    12 (D2), open
    13 (D1), SDIN
    14 (D0), SCLK

    1, 30 (N.C.) =GND
    6, 25, 29 (N.C.) open

    1uF cap:    Vdd to Vss
    1uF cap:    Vddio to Vss
    0.1uF cap:  Vss to Vci
    4.7uF cap:  Vss to Vci
    4.7uF /25V tantulum cap: Vcomh to Vss
    10uF cap:   Vcc to Vss
    0.1uF cap:  Vcc to Vss
    560kΩ [(Voltage_at_IREF – VSS) / IREF, where Voltage_at_IREF = Vcc - 6V] resistor: Iref to Vss
    50Ω, 1/4W resistor: Vsl to input of diode (~1.4V, .5W) that outputs to Vss

    serial timing:
    CS# low -> on SCLK rising edge reads:
        D7-D0 (4-wire) from SDIN
        or
        D/C#,D7-D0 (3-wire) from SDIN
    then CS# high to end
    [~20ns setup/hold times all around]


    note:
    When RES# input is pulled LOW, the chip is initialized with the following status:
    1. Display is OFF
    2. 128 MUX Display Mode
    3. Normal segment and display data column address and row address mapping (SEG0 mapped to
       address 00h and COM0 mapped to address 00h)
    4. Display start line is set at display RAM address 0
    5. Column address counter is set at 0
    6. Normal scan direction of the COM outputs
    7. Command A2h,B1h,B3h,BBh,BEh are locked by command FDh


    Power ON sequence:
    1. Power ON VCI, VDDIO.
    2. After VCI, VDDIO become stable, set wait time at least 1ms [300ms recommended] (t0) for internal VDD become stable. Then set
       RES# pin LOW (logic low) for at least 2us (t1) and then HIGH (logic high).
    3. After set RES# pin LOW (logic low), wait for at least 2us (t2). Then Power ON VCC.
    4. After VCC become stable, send command AFh for display ON. SEG/COM will be ON after 200ms (tAF).
    5. After VCI become stable, wait for at least 300ms to send command

    Power OFF sequence:
    1. Send command AEh for display OFF.
    2. Power OFF VCC.
    3. Wait for tOFF. Power OFF VCI, VDDIO. (where Minimum tOFF=0ms, Typical tOFF=100ms)

    Writing:
    color depth=65k(default): 1) command(write) 2) write(C4C3CC1C0B5B4B3) 3) write(B2B1B0A4A3A2A1A0)
    color depth=262k: 1) command(write) 2) write(xxC5C4C3C2C1C0) 3) write(xxB5B4B3B2B1B0) 4) write(xxA5A4A3A2A1A0)
*/

#define OLED4D_SET_COLUMN_ADDRESS      0x15
#define OLED4D_SET_ROW_ADDRESS         0x75
#define OLED4D_WRITE_GRAM              0x5C
#define OLED4D_READ_GRAM               0x5D
#define OLED4D_STOP_MOVING             0x9E
#define OLED4D_START_MOVING            0x9F
#define OLED4D_REMAP_COLOUR_SETTINGS   0xA0
#define OLED4D_DISPLAY_START_LINE      0xA1
#define OLED4D_DISPLAY_OFFSET          0xA2
#define OLED4D_DISPLAY_OFF             0xA4 // Entire Display Off, All Pixels Turn Off
#define OLED4D_DISPLAY_ALL_ON          0xA5 // Entire Display On, All Pixels Turn On at GS Level 63
#define OLED4D_DISPLAY_NORMAL          0xA6 // Normal Display
#define OLED4D_DISPLAY_INVERSE         0xA7 // Inverse Display
#define OLED4D_FUNCTION_SELECTION      0xAB // Regulator enable: ABh A[0]=1b.
#define OLED4D_DISPLAY_NOP             0xAD
#define OLED4D_DISPLAY_SLEEP_OFF       0xAE
#define OLED4D_DISPLAY_SLEEP_ON        0xAF // Can disable Vdd regulator after entering sleep using AB command, to save more power
#define OLED4D_DISPLAY_NOP2            0xB0
#define OLED4D_PHASE_PRECHARGE         0xB1
#define OLED4D_DISPLAY_ENHANCE         0xB2
#define OLED4D_CLOCK_FREQUENCY         0xB3
#define OLED4D_SEGMENT_LOW_VOLTAGE     0xB4
#define OLED4D_SET_GPIO                0xB5
#define OLED4D_SECOND_PRECHARGE        0xB6
#define OLED4D_SET_GRAYSCALE_LUT       0xB8
#define OLED4D_RESET_GRAYSCALE_LUT     0xB9
#define OLED4D_PRECHARGE_VOLTAGE_RGB   0xBB
#define OLED4D_SET_VCOMH               0xBE
#define OLED4D_CONTRAST_RGB            0xC1
#define OLED4D_CONTRAST_MASTER         0xC7
#define OLED4D_DUTY_CYCLE              0xCA // same (set mux ration)
#define OLED4D_OLED_NOP2               0xD1
#define OLED4D_OLED_NOP3               0xE3
#define OLED4D_COMMAND_LOCK            0xFD

#endif
