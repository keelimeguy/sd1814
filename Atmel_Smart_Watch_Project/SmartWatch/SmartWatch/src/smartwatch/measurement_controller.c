/* UConn Senior Design Team 1814, November 2017
*/

#include "measurement_controller.h"

static volatile uint8_t new_measurement;
static volatile float glucose;

void measurement_controller_init(void) {
    new_measurement = 0;
    glucose = 0;
}

void take_measurement(void) {
    new_measurement = 1;
    glucose++;
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
