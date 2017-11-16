#include <Wire.h>
#include <SPI.h>
#include "TinyScreen_driver.h"
#include "grapher.h"
#include "util.h"

TinyScreen display = TinyScreen(TinyScreenPlus);

int i = 0, diff = 1.1;
double data, k = 0;
char buffer[12];

void setup(void) {
    Wire.begin();
    display.begin();
    display.setBrightness(10);
    display.setFlip(true);
    setDisplay(display);
}

void loop() {
    data = k*i;
    k+=diff;
    if (data>DANGER_HIGH || data<DANGER_LOW) {
            diff = -diff;
            k/=(double)(i)/3.0;
    }
    ftoa(buffer, data, 1);
    i = add_to_graph(data);
    graph(0);
    writeText(0, 20, buffer, 1);
    delay(500);
}
