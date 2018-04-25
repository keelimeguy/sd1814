/* UConn Senior Design Team 1814, January 2018
*/

#include "measurement_controller.h"

#define BG_CAL 160.0f
#define MAX_CAP 256

static float freq = 0.0f;
static volatile uint8_t new_measurement;
static volatile uint8_t measure_busy, pulseState;
static volatile uint16_t glucose, glucoseTemp;
static uint16_t readingTimeout, pulseOne, pulseTwo, pulseThree, pulseCounts, numPoints;

#if DEBUG_MODE != DEBUG_MEASURE_SIM

#define MAX_PULSE_COUNTS 9

static struct tc_module capture_instance;
static struct extint_events eic_events;
static volatile uint16_t periods[MAX_CAP]; // Period with PPW capture
// static volatile uint16_t pulse_widths[MAX_CAP]; // Pulse width with PPW capture
static volatile size_t nCap;

static void do_measurement(void);
static void capture_event_callback(struct tc_module *const module);
static void disable_capture(void);
static void enable_capture(void);

#else

static uint8_t sim_glucose[] = {152, 154, 156, 157, 158, 159, 160, 161, 162, 163, 163, 164, 165, 166, 166, 167, 168, 168, 169, 170, 170, 171, 172, 172, 173, 174, 174, 175, 175, 176, 177, 177, 178, 178, 179, 179, 180, 180, 181, 182, 182, 183, 183, 184, 184, 185, 185, 186, 186, 187, 187, 188, 188, 189, 189, 190, 190, 191, 191, 192, 192, 193, 193, 194, 194, 195, 195, 196, 196, 196, 197, 197, 198, 198, 199, 199, 200, 200, 201, 201, 202, 202, 202, 203, 203, 204, 204, 205, 205, 206, 206, 206, 207, 207, 207, 207, 206, 206, 205, 205, 205, 204, 203, 203, 202, 202, 201, 201, 200, 199, 199, 198, 198, 197, 196, 196, 195, 194, 194, 193, 192, 191, 191, 190, 189, 189, 188, 187, 186, 186, 185, 184, 183, 183, 182, 181, 180, 180, 179, 178, 177, 176, 176, 175, 174, 173, 172, 172, 171, 170, 169, 168, 167, 167, 166, 165, 164, 163, 162, 162, 161, 160, 159, 158, 157, 156, 156, 155, 154, 153, 152, 151, 150, 149, 148, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 139, 138, 137, 136, 136, 135, 135, 134, 133, 132, 131, 130, 129, 129, 127, 126, 125, 124, 123, 122, 121, 120, 118, 117, 116, 115, 113, 112, 111, 109, 108, 107, 105, 104, 102, 101, 100, 98, 97, 96, 95, 95, 94, 94, 94, 93, 93, 93, 93, 92, 92, 92, 92, 91, 91, 91, 91, 91, 90, 90, 90, 90, 90, 89, 89, 89, 90, 90, 91, 92, 0};
static int idx = 0;

#endif

void measurement_controller_init(void) {
    struct port_config pin;
    port_get_config_defaults(&pin);
    pin.direction = PORT_PIN_DIR_OUTPUT;

    port_pin_set_config(LED_PIN, &pin);

    new_measurement = 0;
    pulseState = 0;
    #if DEBUG_MODE == DEBUG_MEASURE_SIM
        measure_busy = 1;
    #else
        measure_busy = 0;
    #endif
    glucose = 0;
    glucoseTemp = 0;
    pulseOne = 1;    // 1 s
    pulseTwo = 1;    // 1 s
    pulseThree = 6;  // 6 s
    pulseCounts = 0;
    numPoints = 0;

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

    readingTimeout  = READING_TIMEOUT; // s
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

void take_measurement(void) {
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
        if (sim_glucose[idx]) {
            new_measurement = 1;
            glucose = sim_glucose[idx];
            idx++;
        } else{
            measure_busy = 0;
        }
        return;
        #else
        switch (pulseState) {
            case 0:
                pulseCounts = 0;
                glucoseTemp = 0;
                numPoints = 0;

                port_pin_set_output_level(LED_PIN, true);
                set_pulse_timeout(pulseOne);
                pulseState = 1;
                break;
            case 1:
                if (is_pulse_timeout()) {
                    set_pulse_timeout(pulseTwo);
                    enable_capture();
                    pulseState = 2;
                }
                break;
            case 2:
                if (is_pulse_timeout()) {
                    do_measurement();
                    pulseCounts++;
                    if (pulseCounts < MAX_PULSE_COUNTS) {
                        set_pulse_timeout(pulseTwo);
                        enable_capture();
                    } else {
                        port_pin_set_output_level(LED_PIN, false);
                        if (numPoints > 0) {
                            glucose = (glucoseTemp / numPoints);
                            new_measurement = 1;
                        }
                        measure_busy = 0;
                        pulseState = 0;
                    }
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
        //set_pulse_timeout(0);
    }
}

static void do_measurement(void) {
    disable_capture();

    if (nCap < 1)
        return; // Failure
    numPoints++;
    uint32_t sum = 0;
    // Start at 1, to ignore first read value
    for (uint16_t i = 1; i < nCap; i ++) {
        sum += (uint32_t)periods[i];
    }

    // Assumes 8MHz clock
    freq = 8000000.0f * (float)(nCap-1) / (float)sum;
    nCap = 0;

    // 1kHz -> 80, 2kHz -> 85
    // m = 0.005
    // glucose = m(freq-1000)+80
    glucoseTemp += 0.005f*(freq-1000.0f)+80;
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
