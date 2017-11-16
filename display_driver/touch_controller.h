#ifndef TOUCH_CONTROLLER_H
#define TOUCH_CONTROLLER_H

/*
    4-wire touch controller:                                XL    XR        YU        YD
        standby (waits for interrupt for touch activity) : GND   HI-Z      HI-Z      Pull-Up/Interrupt
                            X-coordinate (reads via ADC) : GND   Vcc       HI-Z      HI-Z/ADC
                            Y-coordinate (reads via ADC) : HI-Z  HI-Z/ADC  GND       Vcc
                            Z1 for force (reads via ADC) : GND   HI-Z/ADC  HI-Z      Vcc
                            Z2 for force (reads via ADC) : GND   HI-Z      HI-Z/ADC  Vcc

            then Rtouch can be calculted as: Rtouch = Rx-plate * ADCX/2^ADC_resolution * (ADCZ2/ADCZ1-1)

            recommend sampling: 70 points per second (200 points per second if motion is regarded)

    touch calibration:    display=(Xd, Yd)  touch=(Xt, Yt)
        ---------------------------------  Xd = A*Xt + B*Yt + C
        |   (3)                         |  Yd = D*Xt + E*Yt + F
        |   X   10%^                    |
        |   <-10%                  50%^ |  A = [(Xd1-Xd3)(Yt2-Yt3)-(Xd2-Xd3)(Yt1-Yt3)] / [(Xt1-Xt3)(Yt2-Yt3)-(Xt2-Xt3)(Yt1-Yt3)]
        |                       (1)  X  |  B = [A*(Xt3-Xt2)+Xd2-Xd3] / [Yt2-Yt3]
        |                         10%-> |  C = Xd3 - A*Xt3 - B*Yt3
        |              (2)              |  D = [(Yd1-Yd3)(Yt2-Yt3)-(Yd2-Yd3)(Yt1-Yt3)] / [(Xt1-Xt3)(Yt2-Yt3)-(Xt2-Xt3)(Yt1-Yt3)]
        |        10%\/  X  50%->        |  E = [D*(Xt3-Xt2)+Yd2-Yd3] / [Yt2-Yt3]
        ---------------------------------  F = Yd3 - D*Xt3 - E*Yt3
*/

#endif
