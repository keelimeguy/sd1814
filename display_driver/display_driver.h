#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define NONE          0

#define NHD_128160EF  1
#define OLED4D_282815 2
#define TINY_SCREEN   3

#define PARALLEL_8080 1
#define PARALLEL_6800 2
#define SERIAL_3_WIRE 3
#define SERIAL_4_WIRE 4

#ifndef DISPLAY_SCREEN_TYPE
    #define DISPLAY_SCREEN_TYPE NONE
#endif

#if DISPLAY_SCREEN_TYPE==NHD_128160EF
    #include "displays/NHD_128160EF.h"
    #define INTERFACE_MODE PARALLEL_8080

#elif DISPLAY_SCREEN_TYPE==OLED4D_282815
    #include "displays/OLED4D_282815.h"
    #define INTERFACE_MODE SERIAL_4_WIRE

#elif DISPLAY_SCREEN_TYPE==TINY_SCREEN
    #include "displays/TinyScreen_driver.h"
    #define INTERFACE_MODE NONE

#else
    #define INTERFACE_MODE NONE

#endif

#endif
