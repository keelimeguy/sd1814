/* UConn Senior Design Team 1814, January 2018
*/

#ifndef MEASUREMENT_CONTROLLER_H
#define MEASUREMENT_CONTROLLER_H

#include "../SmartWatch.h"

#define BUTTON_L_EIC     BOARD_EIC_L_BUTTON
#define BUTTON_R_EIC     BOARD_EIC_R_BUTTON

#define BUTTON_L_VAL     0x1
#define BUTTON_R_VAL     0x2

void button_listener_init(void);
uint8_t is_button_interrupt_soft(void);
uint8_t is_button_interrupt(void);
uint8_t get_buttons_soft(void);
uint8_t get_buttons(void);
uint8_t get_buttons_held(void);

#endif
