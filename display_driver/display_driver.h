#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#define NHD_128160EF 0
#define OLED4D_282815 1
#define TINY_SCREEN 2

#ifndef DISPLAY_SCREEN_TYPE
    #define DISPLAY_SCREEN_TYPE TINY_SCREEN
#endif

#if DISPLAY_SCREEN_TYPE==NHD_128160EF
    #include "displays/NHD_128160EF.h"

#elif DISPLAY_SCREEN_TYPE==OLED4D_282815
    #include "displays/OLED4D_282815.h"

#elif DISPLAY_SCREEN_TYPE==TINY_SCREEN
    #include "displays/TinyScreen_driver.h"

#endif

#endif
