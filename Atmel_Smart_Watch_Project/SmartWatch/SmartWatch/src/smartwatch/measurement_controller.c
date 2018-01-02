/* UConn Senior Design Team 1814, November 2017
*/

#include "measurement_controller.h"

#define BG_CAL 160

static float x_result[6] = {125,125,6,0,70,1000};
static volatile uint8_t new_measurement;
static volatile uint8_t measure_busy;
static volatile float glucose;
static uint32_t pulseOne, pulseTwo, pulseThree;

void measurement_controller_init(void) {
    new_measurement = 0;
    glucose = 0;
	pulseOne = 1000;
	pulseTwo = 1000;
	pulseThree = 6000;
}

void take_measurement(void) {
    new_measurement = 1;
    glucose++;
    measure_busy = 1;
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
        // Copied from 2016-17 code, not sure about it yet

        // if (((millis() > pulseTimer + (unsigned long)pulseTimeout) && !pulseFlag) || (buttonFlag == 1 && !pulseFlag)) {
        //     pulseFlag = 1;
        //     pulseTimer = millis();
        //     // turn LED array on
        //     digitalWrite(6, HIGH);
        // }

        // if (((pulseFlag == 1)&& millis() > pulseTimer + (unsigned long)pulseOne)) {
        //     // if first pulse is over
        //     digitalWrite(6, LOW);
        //     // turn LED array off
        //     pulseFlag = 2;
        //     pulseTimer = millis();
        // }

        // if (((pulseFlag == 2)&& millis() > pulseTimer + (unsigned long)pulseTwo)) {
        //     // if off pulse is over
        //     digitalWrite(6, HIGH);
        //     // turn LED array on
        //     pulseFlag = 3;
        //     pulseTimer = millis();
        // }

        // if (pulseFlag == 3) {
        //     // if during third pulse
        //     freq = 0.8*(0.5/(pulseIn(5,HIGH)*pow(10,-6))) + 0.2*freq; // weigh new measurements more than previous

        //     if (millis() > pulseTimer + (unsigned long)pulseThree) {
        //         // if third pulse is over
        //         digitalWrite(6, LOW);
        //         // turn LED array off
        //         pulseFlag = 0;
        //         if(buttonFlag) {
        //             SerialMonitorInterface.println("Measuring...");
        //             // calculate bg without kalman algorithm
        //             float Gs = x_result[4]*freq + x_result[5]; // rough estimate of Gs
        //             float Gb = x_result[3]*(1/x_result[2]) + Gs;
        //             updateGlucoseDisplay(Gb); // update display
        //         }
        //         else
        //             doKalman(freq, 1);

        //         buttonFlag = 0;
        //         sendFlag = 1;
        //         pulseTimer = millis();
        //     }
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
