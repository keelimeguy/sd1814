#include <Wire.h>
#include <SPI.h>
#include "TinyScreen_driver.h"
#include "grapher.h"
#include "util.h"

TinyScreen display = TinyScreen(TinyScreenPlus);

const int size = GRAPH_WIDTH/BAR_WIDTH;
int i = 0, length = 0, diff = 1.1;
double data[size], k = 0;
char buffer[12];

void setup(void) {
    Wire.begin();
    display.begin();
    display.setBrightness(10);
    display.setFlip(true);
    setDisplay(display);
}

void loop() {
    data[i] = k*i;
    ftoa(buffer, data[i], 1);

    k+=diff;
    if (k*i>DANGER_HIGH || k*i<DANGER_LOW) {
            diff = -diff;
            k/=(double)(i)/3.0;
    }
    if (length < size) length++;
    if (++i >= size) i = 0;

    graph(data, length, length!=size?0:i, BAR_WIDTH);
    writeText(0, 20, buffer, 1);
    delay(500);
}
