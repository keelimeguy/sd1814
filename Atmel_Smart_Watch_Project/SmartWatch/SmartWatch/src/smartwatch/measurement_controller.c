/* UConn Senior Design Team 1814, January 2018
*/

#include "measurement_controller.h"

#define BG_CAL 160

static float x_result[6] = {125,125,6,0,70,1000};
static volatile uint8_t new_measurement, buttonFlag;
static volatile uint8_t measure_busy, pulseState;
static volatile float glucose;
static uint32_t readingTimeout, pulseOne, pulseTwo, pulseThree;
static uint16_t freq;

static unsigned long do_measurement(void);

void measurement_controller_init(void) {
    struct port_config pin;
    port_get_config_defaults(&pin);
    pin.direction = PORT_PIN_DIR_OUTPUT;

    port_pin_set_config(LED_R1, &pin);
    port_pin_set_config(LED_R2, &pin);
    port_pin_set_config(LED_R3, &pin);
    port_pin_set_config(LED_C1, &pin);
    port_pin_set_config(LED_C2, &pin);
    port_pin_set_config(LED_C3, &pin);

    pin.direction = PORT_PIN_DIR_INPUT;
    port_pin_set_config(PHOTODIODE_PIN, &pin);

    new_measurement = 0;
    buttonFlag = 0;
    measure_busy = 0;
    pulseState = 0;
    glucose = 125;
    pulseOne = 1;    // 1 s
    pulseTwo = 1;    // 1 s
    pulseThree = 6;  // 6 s
    freq = 0;//3000;

    // Time until next reading
    readingTimeout  = 10; // s
}

void take_measurement(uint8_t button) {
    buttonFlag = button;
    measure_busy = 1;
}

uint8_t is_measure_busy(void) {
    return measure_busy;
}

void measure_set_reading_timeout(uint32_t timeout) {
    readingTimeout = timeout;
}

uint32_t measure_get_reading_timeout(void) {
    return readingTimeout;
}

void measure_set_pulse_one(uint32_t pulse) {
    pulseOne = pulse;
}

void measure_set_pulse_two(uint32_t pulse) {
    pulseTwo = pulse;
}

void measure_set_pulse_three(uint32_t pulse) {
    pulseThree = pulse;
}

void measurement_task(void) {
    if (measure_busy) {
        #if DEBUG_MODE != DEBUG_NONE
        glucose++;
        new_measurement = 1;
        measure_busy = 0;
        return;
        #else

        switch (pulseState) {
            case 0:
                port_pin_set_output_level(LED_R1, true);
                set_pulse_timeout(pulseOne);
                pulseState = 1;
                break;
            case 1:
                if (is_pulse_timeout()) {
                    port_pin_set_output_level(LED_R1, false);
                    set_pulse_timeout(pulseTwo);
                    pulseState = 2;
                }
                break;
            case 2:
                if (is_pulse_timeout()) {
                    port_pin_set_output_level(LED_R1, true);
                    set_pulse_timeout(pulseThree);
                    pulseState = 3;
                }
                break;
            case 3:
                freq = 0.8*(0.5/(do_measurement()*pow(10,-6))) + 0.2*freq; // weigh new measurements more than previous

                if (is_pulse_timeout()) {
                    port_pin_set_output_level(LED_R1, false);
                    pulseState = 0;

                    if (buttonFlag) {
                        // Calculate bg without kalman algorithm
                        float Gs = x_result[4]*freq + x_result[5]; // rough estimate of Gs
                        glucose = x_result[3]*(1/x_result[2]) + Gs;
                    } else {
                        do_kalman(freq, 1);
                        glucose = x_result[1];
                    }

                    measure_busy = 0;
                    new_measurement = 1;
                }
                break;
            default:
                pulseState = 0;
        }
        #endif
    }
}

// The function do_measurement() is modified from:
/*
  wiring_pulse.c - pulseIn() function
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis
*/

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
* or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
* to 3 minutes in length, but must be called at least a few dozen microseconds
* before the start of the pulse. */
static unsigned long do_measurement(void) {
    // cache the port and bit of the pin in order to speed up the
    // pulse width measuring loop and achieve finer resolution.  calling
    // digitalRead() instead yields much coarser resolution.
    unsigned long width = 0; // keep initialization out of time critical area

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes 16 clock cycles per iteration.
    unsigned long numloops = 0;
    unsigned long maxloops = microsecondsToClockCycles(LONGEST_FREQ_PERIOD_us) / 16;

    // wait for any previous pulse to end
    while (port_pin_get_output_level(PHOTODIODE_PIN) == true)
        if (numloops++ == maxloops)
            return 0;

    // wait for the pulse to start
    while (port_pin_get_output_level(PHOTODIODE_PIN) != true)
        if (numloops++ == maxloops)
            return 0;

    // wait for the pulse to stop
    while (port_pin_get_output_level(PHOTODIODE_PIN) == true) {
        if (numloops++ == maxloops)
            return 0;
        width++;
    }

    // convert the reading to microseconds. The loop has been determined
    // to be 20 clock cycles long and have about 16 clocks between the edge
    // and the start of the loop. There will be some error introduced by
    // the interrupt handlers.
    return clockCyclesToMicroseconds(width * 21 + 16);
}

float get_measurement(void) {
    return glucose;
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
    kalman_CGM((float)calibration, 0.0025*x_result[1]*x_result[1], 0, x_result);
}

void do_kalman(unsigned long freq, uint8_t sensorNum) {
    kalman_CGM((float)freq, (float)400, sensorNum, x_result);
}

void cal_kalman(void) {
    kalman_CGM((float)BG_CAL, (float)(0.05*BG_CAL), 0, x_result);
}
