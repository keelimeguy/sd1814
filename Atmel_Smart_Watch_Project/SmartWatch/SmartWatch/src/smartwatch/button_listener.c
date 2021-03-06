/* UConn Senior Design Team 1814, January 2018
*/

#include "button_listener.h"

static volatile uint8_t button_pressed, button_held, button_interrupt_flag;
#if DEBUG_MODE==DEBUG_BUTTON_EXTINT
    static uint8_t level;
#endif

static void button_listener_callback_L(void);
static void button_listener_callback_R(void);

void button_listener_init(void) {
    struct port_config pin;
    port_get_config_defaults(&pin);
    pin.direction = PORT_PIN_DIR_INPUT;
    pin.input_pull = PORT_PIN_PULL_UP;

    port_pin_set_config(BOARD_BUTTON_L_PIN, &pin);
    port_pin_set_config(BOARD_BUTTON_R_PIN, &pin);

    struct extint_chan_conf config_extint_chan;
    extint_chan_get_config_defaults(&config_extint_chan);

    config_extint_chan.gpio_pin           = BOARD_BUTTON_L_PIN;
    config_extint_chan.gpio_pin_mux       = BOARD_BUTTON_L_MUX;
    config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
    config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
    config_extint_chan.wake_if_sleeping   = 1;
    extint_chan_set_config(BUTTON_L_EIC, &config_extint_chan);

    extint_register_callback(button_listener_callback_L, BUTTON_L_EIC, EXTINT_CALLBACK_TYPE_DETECT);
    extint_chan_enable_callback(BUTTON_L_EIC, EXTINT_CALLBACK_TYPE_DETECT);

    config_extint_chan.gpio_pin           = BOARD_BUTTON_R_PIN;
    config_extint_chan.gpio_pin_mux       = BOARD_BUTTON_R_MUX;
    extint_chan_set_config(BUTTON_R_EIC, &config_extint_chan);

    extint_register_callback(button_listener_callback_R, BUTTON_R_EIC, EXTINT_CALLBACK_TYPE_DETECT);
    extint_chan_enable_callback(BUTTON_R_EIC, EXTINT_CALLBACK_TYPE_DETECT);

    #if DEBUG_MODE==DEBUG_BUTTON_EXTINT
        level = false;
        port_pin_set_output_level(BOARD_DEBUG_LED, level);
    #endif

    button_pressed = 0;
    button_held = 0;
    button_interrupt_flag = 1;
}

static void button_listener_callback_L(void) {
    if (!port_pin_get_input_level(BOARD_BUTTON_L_PIN)) {
        if (is_button_timeout()) {
            button_interrupt_flag = 1;
            button_pressed |= BUTTON_L_VAL;
            #if DEBUG_MODE==DEBUG_BUTTON_EXTINT
                level = !level;
                port_pin_set_output_level(BOARD_DEBUG_LED, level);
            #endif
           set_button_timeout(BUTTON_TIMEOUT);
        }
        button_held |= BUTTON_L_VAL;
    } else {
        button_held &= ~BUTTON_L_VAL;
    }
}

static void button_listener_callback_R(void) {
    if (!port_pin_get_input_level(BOARD_BUTTON_R_PIN)) {
        if (is_button_timeout()) {
            button_interrupt_flag = 1;
            button_pressed |= BUTTON_R_VAL;
            #if DEBUG_MODE==DEBUG_BUTTON_EXTINT
                level = !level;
                port_pin_set_output_level(BOARD_DEBUG_LED, level);
            #endif
            set_button_timeout(BUTTON_TIMEOUT);
        }
        button_held |= BUTTON_R_VAL;
    } else {
        button_held &= ~BUTTON_R_VAL;
    }
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

uint8_t get_buttons_held(void) {
    return button_held;
}
