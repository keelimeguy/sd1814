/* UConn Senior Design Team 1814, January 2018
*/

#include "battery_reader.h"

void battery_reader_init(void) {

}

void battery_task(void) {
/*    SYSCTRL->VREF.reg |= SYSCTRL_VREF_BGOUTEN;
//    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->SAMPCTRL.bit.SAMPLEN = 0x1;
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->INPUTCTRL.bit.MUXPOS = 0x19;         // Internal bandgap input
//    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->CTRLA.bit.ENABLE = 0x01;             // Enable ADC
    // Start conversion
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->SWTRIG.bit.START = 1;
    // Clear the Data Ready flag
    ADC->INTFLAG.bit.RESRDY = 1;
    // Start conversion again, since The first conversion after the reference is changed must not be used.
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->SWTRIG.bit.START = 1;
    // Store the value
    while ( ADC->INTFLAG.bit.RESRDY == 0 );   // Waiting for conversion to complete
    uint32_t valueRead = ADC->RESULT.reg;
    while (ADC->STATUS.bit.SYNCBUSY == 1);
    ADC->CTRLA.bit.ENABLE = 0x00;             // Disable ADC
//    while (ADC->STATUS.bit.SYNCBUSY == 1);
    SYSCTRL->VREF.reg &= ~SYSCTRL_VREF_BGOUTEN;
    result = (((1100L * 1024L) / valueRead) + 5L) / 10L;
*/

}

int get_battery_level(int max) {

}

#endif
