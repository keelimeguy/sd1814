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

void measurement_controller_init(void) {
    new_measurement = 0;
    buttonFlag = 0;
    measure_busy = 0;
    pulseState = 0;
    glucose = 125;
    pulseOne = 1;    // 1 s
    pulseTwo = 1;    // 1 s
    pulseThree = 6;  // 6 s
    freq = 3000;

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
        // For testing
        glucose++;
        new_measurement = 1;
        measure_busy = 0;
        return;

        // // TODO: Finish Implementation
        // switch (pulseState) {
        //     case 0:
        //         digitalWrite(6, HIGH);
        //         set_pulse_timeout(pulseOne);
        //         pulseState = 1;
        //         break;
        //     case 1:
        //         if (is_pulse_timeout()) {
        //             digitalWrite(6, LOW);
        //             set_pulse_timeout(pulseTwo);
        //             pulseState = 2;
        //         }
        //         break;
        //     case 2:
        //         if (is_pulse_timeout()) {
        //             digitalWrite(6, HIGH);
        //             set_pulse_timeout(pulseThree);
        //             pulseState = 3;
        //         }
        //         break;
        //     case 3:
        //         freq = 0.8*(0.5/(pulseIn(5,HIGH)*pow(10,-6))) + 0.2*freq; // weigh new measurements more than previous

        //         if (is_pulse_timeout()) {
        //             digitalWrite(6, LOW);
        //             pulseState = 0;

        //             if (buttonFlag) {
        //                 // Calculate bg without kalman algorithm
        //                 float Gs = x_result[4]*freq + x_result[5]; // rough estimate of Gs
        //                 glucose = x_result[3]*(1/x_result[2]) + Gs;
        //             } else {
        //                 do_kalman(freq, 1);
        //                 glucose = x_result[1];
        //             }

        //             measure_busy = 0;
        //             new_measurement = 1;
        //         }
        //         break;
        //     default:
        //         pulseState = 0;
        // }
    }
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
