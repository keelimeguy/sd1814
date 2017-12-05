/* UConn Senior Design Team 1814, November 2017
*/

#include "display_manager.h"
#include "display_driver/display_driver.h"
#include "display_driver/grapher/util.h"
/*
#include "display_driver/GFXfont/Fonts/FreeSans9pt7b.h"
#include "display_driver/GFXfont/Fonts/FreeSans12pt7b.h"
#include "display_driver/GFXfont/Fonts/FreeSans24pt7b.h"
*/
#include "display_driver/grapher/grapher.h"

#define FONT_9PT                 0//&FreeSans9pt7b
#define FONT_12PT                0//&FreeSans12pt7b
#define FONT_24PT                0//&FreeSans24pt7b

#define FONT_DEFAULT             FONT_9PT

static volatile uint8_t rewriteMenu;
static volatile uint8_t rewriteTime;
static uint8_t lastDisplayedDay;
static uint8_t currentDisplayState;
static uint8_t lastAmtNotificationsShown;
static uint8_t lastAMPMDisplayed;
static uint8_t lastHourDisplayed;
static uint8_t lastMinuteDisplayed;
static uint8_t lastSecondDisplayed;
static uint8_t ble_connection_displayed_state;
static float lastGlucoseVal;

static int diff = 1, i = 0;
static double k = DISP_WARNING_LOW;
static char buffer[12], min_max_buffer[12];

static void initHomeScreen(void);
static void updateMainDisplay(uint8_t button);
static void updateGraph(float glucose);
static void showGraphView(uint8_t button);
static void viewNotifications(uint8_t button);
static void updateDateDisplay(void);
static void updateGlucoseDisplay(float glucose);
// static void updateFreqDisplay(void);
static void updateTimeDisplay(void);
static void updateBLEstatusDisplay(void);
static void displayBattery(void);

void display_manager_init(void) {
    disp_init();
    disp_set_font(FONT_DEFAULT);

    lastDisplayedDay = 0;
    currentDisplayState = DISP_STATE_HOME;
    rewriteMenu = 1;
    rewriteTime = 1;
    lastAmtNotificationsShown = 0;
    lastAMPMDisplayed = 0;
    lastHourDisplayed = 0;
    lastMinuteDisplayed = 0;
    lastSecondDisplayed = 0;
    ble_connection_displayed_state = 2;
    lastGlucoseVal = 0;
}

void display_ui_task(uint8_t button) {
    // k+=diff*i;
    // if (k>DISP_DANGER_HIGH || k<DISP_DANGER_LOW) {
    //         diff = -diff;
    // }
    // updateGraph(k);

    if (is_new_measurement()) {
        updateGraph(get_measurement());
    }

    switch(currentDisplayState) {
        case DISP_STATE_HOME:
            updateMainDisplay(button);
            break;

        case DISP_STATE_GRAPH:
            showGraphView(button);
            break;

        case DISP_STATE_NOTIFICATION:
            viewNotifications(button);
            break;

        default:
            initHomeScreen();
    }
}

static void initHomeScreen() {
    disp_fill_rect(0, 12, 96, 64, DISP_PIXEL_BLACK);
    rewriteTime = 1;
    rewriteMenu = 1;
}

static void updateMainDisplay(uint8_t button) {
    uint8_t menuTextY[4] = {12, 25, 38, 51};
    // uint8_t brightness = 3;
    // uint8_t lastSetBrightness = 100;
    // if (lastSetBrightness != brightness) {
    //     disp_setBrightness(brightness);
    //     lastSetBrightness = brightness;
    // }

    if (button & VIEW_BUTTON) {
        currentDisplayState == DISP_STATE_NOTIFICATION;
    } else if (button & GRAPH_BUTTON) {
        currentDisplayState == DISP_STATE_GRAPH;
    }

    updateGlucoseDisplay(lastGlucoseVal);
    displayBattery();
    updateBLEstatusDisplay();
    updateDateDisplay();
    if (currentDisplayState == DISP_STATE_HOME) {
        updateTimeDisplay();
        if (rewriteMenu || lastAmtNotificationsShown != bt_amt_notifications()) {
            lastAmtNotificationsShown = bt_amt_notifications();
            disp_set_pos(0, menuTextY[2]);
            if (!bt_amt_notifications()) {
                disp_write_str(" No notifications.  ");
            } else {
                disp_write_str("  ");
                disp_write_str(itoa(bt_amt_notifications(), buffer, 10));
                disp_write_str(" notification");
                if (bt_amt_notifications() > 1)
                    disp_write('s');
                disp_write_str(".   ");
            }
            disp_set_font(FONT_12PT);
            disp_set_pos(0, menuTextY[3]);
            disp_write_str("< Graph");
            disp_set_pos(56, menuTextY[3]);
            disp_write_str("View >");
            rewriteMenu = 0;
        }
    }
}

static void viewNotifications(uint8_t button) {
    uint8_t menuTextY[4] = {12, 25, 38, 51};
    if (!button) {
        currentDisplayState = DISP_STATE_NOTIFICATION;
        disp_fill_rect(0, 12, 96, 64, DISP_PIXEL_BLACK);
        disp_set_font(FONT_12PT);
        disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_WHITE);

        if (bt_amt_notifications()) {
            disp_set_pos(0, menuTextY[0]);
            //disp_write_str(bt_get_notification(0));
            disp_set_pos(0, menuTextY[1]);
            //disp_write_str(bt_get_notification(1));
            disp_set_pos(56, menuTextY[3]);
            disp_write_str("clear >");
        } else {
            disp_set_pos(0, menuTextY[0]);
            disp_write_str(" No notifications.   ");
            disp_set_pos(59, menuTextY[3]);
            disp_write_str("back >");
        }
    } else {
        if (button == CLR_BUTTON) {
            bt_clear_amt_notifications();
            currentDisplayState = DISP_STATE_HOME;
            initHomeScreen();
        }
    }
}

static void showGraphView(uint8_t button) {
    uint8_t menuTextY[4] = {12, 25, 38, 51};
    if (currentDisplayState != DISP_STATE_GRAPH) {
        disp_fill_rect(0, 12, 96, 64, DISP_PIXEL_BLACK);
        currentDisplayState = DISP_STATE_GRAPH;
    }
    if (!button) {
        disp_set_font(FONT_12PT);
        disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);

        if (graph_length()==0) {
            disp_set_pos(0, menuTextY[0]);
            disp_write_str(" No Data.   ");
        } else {
            if (graph_length()==1) {
                disp_fill_rect(0, 12, 96, 64, DISP_PIXEL_BLACK);
            }
            graph(0);
            ftoa(min_max_buffer, graph_max(), 1);
			disp_set_pos(0, menuTextY[0]);
            disp_write_str(min_max_buffer);
            ftoa(min_max_buffer, graph_min(), 1);
            disp_set_pos(0, menuTextY[2]);
			disp_write_str(min_max_buffer);

            disp_set_font(FONT_9PT);
            disp_set_pos(42, menuTextY[3]);
            disp_write_str(buffer);
            disp_write_str("mg/dL    ");
        }

        disp_set_pos(0, menuTextY[3]);
        disp_write_str("< back");
    } else {
        if (button == GRAPH_BUTTON) {
            currentDisplayState = DISP_STATE_HOME;
            initHomeScreen();
        }
    }
}

static void updateGraph(float glucose) {
    lastGlucoseVal = glucose;
    i = add_to_graph(glucose);
}

static void updateDateDisplay(void) {
    struct rtc_calendar_time time;
    rtc_get_time(&time);

    if (lastDisplayedDay == time.day)
        return;
    lastDisplayedDay = time.day;
    disp_set_font(FONT_9PT);
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    disp_fill_rect(2, 1, 50, 20, DISP_PIXEL_BLACK);
    disp_set_pos(2, 1);
    disp_write_str(calendar_day_str(buffer, &time));
    disp_write(' ');
    disp_write_str(itoa(time.month, buffer, 10));
    disp_write('/');
    disp_write_str(itoa(time.day, buffer, 10));
    disp_write_str("  ");
}

#define round(f) ( (f-(float)((int)f)) > 0.5 ? (int)f+1 : (int)f )

static void updateGlucoseDisplay(float glucose) {
    uint8_t timeY = 14;
    if (currentDisplayState != DISP_STATE_HOME)
        return;
    disp_set_font(FONT_24PT);

    if (glucose < DISP_DANGER_LOW || glucose > DISP_DANGER_HIGH)
         disp_set_color(DISP_PIXEL_RED, DISP_PIXEL_BLACK);
    else if (glucose < DISP_WARNING_LOW || glucose > DISP_WARNING_HIGH)
         disp_set_color(DISP_PIXEL_YELLOW, DISP_PIXEL_BLACK);
    else
         disp_set_color(DISP_PIXEL_GREEN, DISP_PIXEL_BLACK);

    disp_fill_rect(46, timeY, 37, 20, DISP_PIXEL_BLACK);
    if(round(glucose)<10)
        disp_set_pos(70, timeY);
    else if(round(glucose < 100))
        disp_set_pos(58, timeY);
    else
        disp_set_pos(46, timeY);
    disp_write_str(itoa(round(glucose), buffer, 10));
    disp_set_font(FONT_9PT);
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    disp_set_pos(83,timeY-4);
    disp_write_str("mg");
    disp_set_pos(83,timeY+7);
    disp_write_str("----");
    disp_set_pos(83,timeY+15);
    disp_write_str("dL");
}

// static void updateFreqDisplay(void) {
//     disp_fill_rect(0, menuTextY[3], 50, 20, DISP_PIXEL_BLACK);
//     disp_set_font(FONT_12PT);
//     disp_set_pos(0, menuTextY[3]);
//     disp_write_str(itoa(freq, buffer, 10));
//     disp_write_str("Hz");
// }

static void updateTimeDisplay(void) {
    struct rtc_calendar_time time;
    rtc_get_time(&time);

    uint8_t timeY = 14;
    if (currentDisplayState != DISP_STATE_HOME)
        return;
    uint8_t displayX;
    uint8_t hour12 = time.hour;
    uint8_t AMPM = 0;
    if (hour12 > 12) {
        AMPM = 1;
        hour12 -= 12;
    }
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    if (rewriteTime || lastHourDisplayed != hour12) {
        disp_set_font(FONT_12PT);
        lastHourDisplayed = hour12;
        displayX = 0;
        disp_set_pos(displayX, timeY);
        if (lastHourDisplayed < 10) disp_write('0');
        disp_write_str(itoa(lastHourDisplayed, buffer, 10));
        disp_write(':');
        if (rewriteTime || lastAMPMDisplayed != AMPM) {
            if (AMPM)
                disp_set_color(DISP_PIXEL_GREY, DISP_PIXEL_BLACK);
            disp_set_font(FONT_9PT);
            disp_set_pos(0, timeY + 13);
            disp_write_str("AM");
            if (AMPM) {
                disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
            } else {
                disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
            }
            disp_set_pos(22, timeY + 13);
            disp_write_str("PM");
            disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
        }
    }

    if (rewriteTime || lastMinuteDisplayed != time.minute) {
        disp_set_font(FONT_12PT);
        lastMinuteDisplayed = time.minute;
        displayX = 16;
        disp_set_pos(displayX, timeY);
        if (lastMinuteDisplayed < 10) disp_write('0');
        disp_write_str(itoa(lastMinuteDisplayed, buffer, 10));
        disp_write(':');
    }

    if (rewriteTime || lastSecondDisplayed != time.second) {
        disp_set_font(FONT_12PT);
        lastSecondDisplayed = time.second;
        displayX = 32;
        disp_set_pos(displayX, timeY);
        if (lastSecondDisplayed < 10) disp_write('0');
        disp_write_str(itoa(lastSecondDisplayed, buffer, 10));
    }
    rewriteTime = 0;
}

static void updateBLEstatusDisplay(void) {
    if (bt_connection_state() == ble_connection_displayed_state)
        return;
    ble_connection_displayed_state = bt_connection_state();
    int x = 62;
    int y = 6;
    int s = 2;
    uint16_t color = DISP_PIXEL_RED;
    if (ble_connection_displayed_state)
        color = DISP_PIXEL_BLUE;
    disp_draw_line(x, y + s + s, x, y - s - s, color);
    disp_draw_line(x - s, y + s, x + s, y - s, color);
    disp_draw_line(x + s, y + s, x - s, y - s, color);
    disp_draw_line(x, y + s + s, x + s, y + s, color);
    disp_draw_line(x, y - s - s, x + s, y - s, color);
}

// TODO: make battery ADC read into interrupt and nonblocking
static void displayBattery(void) {
    int result = 0;
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
*/    uint8_t x = 70;
    uint8_t y = 3;
    uint8_t height = 5;
    uint8_t length = 20;
    uint8_t red, green;
    if (result > 325) {
        red = 0;
        green = 63;
    } else {
        red = 63;
        green = 0;
    }
    disp_draw_line(x - 1, y, x - 1, y + height, DISP_PIXEL_WHITE); //left boarder
    disp_draw_line(x - 1, y - 1, x + length, y - 1, DISP_PIXEL_WHITE); //top border
    disp_draw_line(x - 1, y + height + 1, x + length, y + height + 1, DISP_PIXEL_WHITE); //bottom border
    disp_draw_line(x + length, y - 1, x + length, y + height + 1, DISP_PIXEL_WHITE); //right border
    disp_draw_line(x + length + 1, y + 2, x + length + 1, y + height - 2, DISP_PIXEL_WHITE); //right border
    for (uint8_t q = 0; q < length; q++) {
        disp_draw_line(x + q, y, x + q, y + height, DISP_FORMAT_COLOR(red, green, 0));
    }
}
