/* UConn Senior Design Team 1814, November 2017
*/

#include "button_listener.h"

static volatile uint8_t button_pressed, button_interrupt_flag;

static void button_listener_callback_L(void);
static void button_listener_callback_R(void);

void button_listener_init(void) {
    struct extint_chan_conf config_extint_chan;
    extint_chan_get_config_defaults(&config_extint_chan);

    config_extint_chan.gpio_pin           = BOARD_BUTTON_L_PIN;
    config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
    config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
    extint_chan_set_config(BUTTON_L_EIC, &config_extint_chan);

    extint_register_callback(button_listener_callback_L, BUTTON_L_EIC, EXTINT_CALLBACK_TYPE_DETECT);
    extint_chan_enable_callback(BUTTON_L_EIC, EXTINT_CALLBACK_TYPE_DETECT);

    config_extint_chan.gpio_pin           = BOARD_BUTTON_R_PIN;
    config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
    config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
    extint_chan_set_config(BUTTON_R_EIC, &config_extint_chan);

    extint_register_callback(button_listener_callback_R, BUTTON_R_EIC, EXTINT_CALLBACK_TYPE_DETECT);
    extint_chan_enable_callback(BUTTON_R_EIC, EXTINT_CALLBACK_TYPE_DETECT);

    button_pressed = 0;
    button_interrupt_flag = 1;
}

static void button_listener_callback_L(void) {
    button_interrupt_flag = 1;
    button_pressed |= BUTTON_L_VAL;
}

static void button_listener_callback_R(void) {
    button_interrupt_flag = 1;
    button_pressed |= BUTTON_R_VAL;
}

uint8_t is_button_interrupt_soft(void) {
    return button_interrupt_flag;
}

uint8_t is_button_interrupt(void) {
    if (button_interrupt_flag) {
        button_interrupt_flag = 0;
        return 1;
    }
    return 0;
}

uint8_t get_buttons_soft(void) {
    return button_pressed;
}

uint8_t get_buttons(void) {
    if (button_pressed) {
        uint8_t ret = button_pressed;
        button_pressed = 0;
        return ret;
    }
    return 0;
}
