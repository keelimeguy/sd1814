/* UConn Senior Design Team 1814, January 2018
*/

#include "measurement_controller.h"

#define BG_CAL 160.0f
#define MAX_CAP 256

static float x_result[6] = {125,125,6,0,70,1000}, freq = 0.0f;
static volatile uint8_t new_measurement, buttonFlag;
static volatile uint8_t measure_busy, pulseState;
static volatile uint16_t glucose;
static uint16_t readingTimeout, pulseOne, pulseTwo, pulseThree;

#if DEBUG_MODE != DEBUG_MEASURE_SIM

static struct tc_module capture_instance;
static struct extint_events eic_events;
static volatile uint16_t periods[MAX_CAP]; // Period with PPW capture
// static volatile uint16_t pulse_widths[MAX_CAP]; // Pulse width with PPW capture
static volatile size_t nCap;

static void do_measurement(void);
static void capture_event_callback(struct tc_module *const module);
static void disable_capture(void);
static void enable_capture(void);

#endif

void measurement_controller_init(void) {
    struct port_config pin;
    port_get_config_defaults(&pin);
    pin.direction = PORT_PIN_DIR_OUTPUT;

    port_pin_set_config(LED_PIN, &pin);

    new_measurement = 0;
    buttonFlag = 0;
    measure_busy = 0;
    pulseState = 0;
    glucose = 125;
    pulseOne = 1;    // 1 s
    pulseTwo = 1;    // 1 s
    pulseThree = 6;  // 6 s

#if DEBUG_MODE != DEBUG_MEASURE_SIM
    nCap = 0;

    struct tc_config config_tc;
    tc_get_config_defaults(&config_tc);
    config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
    config_tc.enable_capture_on_channel[0] = 1;
    config_tc.enable_capture_on_channel[1] = 1;

    tc_init(&capture_instance, TC4, &config_tc);

    struct tc_events events_tc;

    // PPW: T captured in CC0, tp captured in CC1
    // f = 1/T, dutyCycle = tp / T
    events_tc.event_action = TC_EVENT_ACTION_PPW;

    // Enable the event action
    events_tc.on_event_perform_action = 1;

    tc_enable_events(&capture_instance, &events_tc);

    tc_register_callback(&capture_instance, capture_event_callback, TC_CALLBACK_CC_CHANNEL0);
    tc_enable_callback(&capture_instance, TC_CALLBACK_CC_CHANNEL0);

    struct extint_chan_conf config_extint_chan;
    extint_chan_get_config_defaults(&config_extint_chan);

    config_extint_chan.gpio_pin           = BOARD_PHOTODIODE_PIN;
    config_extint_chan.gpio_pin_mux       = BOARD_PHOTODIODE_MUX;
    config_extint_chan.gpio_pin_pull      = EXTINT_PULL_NONE;
    config_extint_chan.detection_criteria = EXTINT_DETECT_HIGH;
    config_extint_chan.wake_if_sleeping   = 0;
    extint_chan_set_config(PHOTODIODE_EIC, &config_extint_chan);

    eic_events.generate_event_on_detect[PHOTODIODE_EIC] = 1;
    extint_enable_events(&eic_events);

    struct events_resource capture_event;
    struct events_config config_evt;
    events_get_config_defaults(&config_evt);
    config_evt.generator      = BOARD_PHOTODIODE_GEN;
    config_evt.edge_detect    = EVENTS_EDGE_DETECT_NONE;
    config_evt.path           = EVENTS_PATH_ASYNCHRONOUS;
    events_allocate(&capture_event, &config_evt);
    events_attach_user(&capture_event, EVSYS_ID_USER_TC4_EVU);

    tc_enable(&capture_instance);

    disable_capture();
#endif

    // Time until next reading
    readingTimeout  = 10; // s
}

#if DEBUG_MODE != DEBUG_MEASURE_SIM

static void disable_capture(void) {
	system_interrupt_disable_global();
    tc_stop_counter(&capture_instance);
    tc_set_count_value(&capture_instance, 0);
    extint_disable_events(&eic_events);
	system_interrupt_enable_global();
}

static void enable_capture(void) {
    tc_start_counter(&capture_instance);
    extint_enable_events(&eic_events);
}

#endif

void take_measurement(uint8_t button) {
    buttonFlag = button;
    measure_busy = 1;
}

uint8_t is_measure_busy(void) {
    return measure_busy;
}

void measure_set_reading_timeout(uint16_t timeout) {
    readingTimeout = timeout;
}

uint16_t measure_get_reading_timeout(void) {
    return readingTimeout;
}

void measure_set_pulse_one(uint16_t pulse) {
    pulseOne = pulse;
}

void measure_set_pulse_two(uint16_t pulse) {
    pulseTwo = pulse;
}

void measure_set_pulse_three(uint16_t pulse) {
    pulseThree = pulse;
}

void measurement_task(void) {
    if (measure_busy) {
        #if DEBUG_MODE == DEBUG_MEASURE_SIM
        glucose++;
        new_measurement = 1;
        measure_busy = 0;
        return;
        #else

        // Mimicking blinking initialization from last year code
        switch (pulseState) {
            case 0:
                port_pin_set_output_level(LED_PIN, true);
                set_pulse_timeout(pulseOne);
                pulseState = 1;
                break;
            case 1:
                if (is_pulse_timeout()) {
                    port_pin_set_output_level(LED_PIN, false);
                    set_pulse_timeout(pulseTwo);
                    pulseState = 2;
                }
                break;
            case 2:
                if (is_pulse_timeout()) {
                    port_pin_set_output_level(LED_PIN, true);
                    set_pulse_timeout(pulseThree);
                    pulseState = 3;
                    enable_capture();
                }
                break;
            case 3:
                if (is_pulse_timeout()) {
                    port_pin_set_output_level(LED_PIN, false);
                    do_measurement();
                    pulseState = 0;
                    measure_busy = 0;
                }
                break;

            default:
                pulseState = 0;
                break;
        }
        #endif
    }
}

#if DEBUG_MODE != DEBUG_MEASURE_SIM

static void capture_event_callback(struct tc_module *const module) {
    // The interrupt flag is cleared by reading CC
    periods[nCap] = TC4->COUNT16.CC[0].bit.CC;
    // pulse_widths[nCap] = TC4->COUNT16.CC[1].bit.CC;
    if (++nCap == MAX_CAP) {
        disable_capture();
        set_pulse_timeout(0);
    }
}

static void do_measurement(void) {
    disable_capture();

    if (nCap < 1)
        return; // Failure

    new_measurement = 1;
    uint32_t sum = 0;
    // Start at 1, to ignore first read value
    for (uint16_t i = 1; i < nCap; i ++) {
        sum += (uint32_t)periods[i];
    }

    // Assumes 8MHz clock
    freq = 8000000.0f * (float)(nCap-1) / (float)sum;
    nCap = 0;

    // if (buttonFlag) {
    //     // Calculate bg without kalman algorithm
    //     float Gs = x_result[4]*freq + x_result[5]; // rough estimate of Gs
    //     glucose = x_result[3]*(1.0f/x_result[2]) + Gs;
    //     buttonFlag = 0;
    // } else {
    //     do_kalman(freq, 1);
    //     glucose = x_result[1];
    // }
    do_kalman(freq, 1);
    glucose = x_result[1];
}

#endif

uint16_t get_measurement(void) {
    return glucose;
}

float get_freq(void) {
    return freq;
}

uint8_t is_new_measurement_soft(void) {
    return new_measurement;
}

uint8_t is_new_measurement(void) {
    if (new_measurement) {
        new_measurement = 0;
        return 1;
    }
    return 0;
}

void do_kalman_bt_cmd(long calibration) {
    kalman_CGM((float)calibration, 0.0025f*x_result[1]*x_result[1], 0, x_result);
}

void do_kalman(float freq, uint8_t sensorNum) {
    kalman_CGM(freq, 400.0f, sensorNum, x_result);
}

void cal_kalman(void) {
    kalman_CGM(BG_CAL, 0.05f*BG_CAL, 0, x_result);
}
