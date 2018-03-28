/* UConn Senior Design Team 1814, January 2018
*/

#include "display_manager.h"
#include "display_driver/grapher/util.h"

#include "display_driver/GFXfont/Fonts/Org_01.h"
#include "display_driver/GFXfont/Fonts/FreeSans9pt7b.h"
#include "display_driver/GFXfont/Fonts/FreeSans12pt7b.h"

#include "display_driver/grapher/grapher.h"

#define FONT_SMALL               &Org_01
#define FONT_MEDIUM              &FreeSans9pt7b
#define FONT_LARGE               &FreeSans12pt7b

#define FONT_DEFAULT             FONT_MEDIUM
#define round(f) ( (f-(float)((int)f)) > 0.5 ? (int)f+1 : (int)f )

static volatile uint8_t startup;
static volatile uint8_t rewriteMenu;
static volatile uint8_t rewriteTime;
static uint8_t last_notifications;
static uint8_t lastDisplayedDay;
static uint8_t currentDisplayState;
static uint8_t lastAmtNotificationsShown;
static uint8_t lastAMPMDisplayed;
static uint8_t lastHourDisplayed;
static uint8_t lastMinuteDisplayed;
static uint8_t lastSecondDisplayed;
static uint8_t ble_connection_displayed_state;
static uint8_t first_data, graph_refresh, newGlucose;
static float lastGlucoseVal;
static int last_battery;
static uint8_t last_bat_x, last_ble_x, last_date_x, last_hour_x, last_min_x, last_sec_x, last_ampm_x;

static char buffer[12], min_max_buffer[12];
#define headerTextY 8
static const uint8_t menuTextY[7] = { 2*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1, 4*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1,
                                2*DISP_DIVISION_HEIGHT+DISP_HEADER_HEIGHT-1, 8*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1,
                                10*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1, 4*DISP_DIVISION_HEIGHT+DISP_HEADER_HEIGHT-1,
                                14*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1 };
static void initHomeScreen(void);
static void updateMainDisplay(uint8_t button);
static void showGraphView(uint8_t button);
static void viewNotifications(uint8_t button);
static void updateGlucoseDisplay(float glucose);
// static void updateFreqDisplay(void);
static int updateDateDisplay(int xoff);
static int updateTimeDisplay(int xoff);
static int updateBLEstatusDisplay(int xoff);
static int displayBattery(int xoff);

void display_manager_init(void) {
    disp_init();
    disp_set_font(FONT_DEFAULT);
    disp_set_font_scale(1);

    startup = 1;
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
    first_data = 0;
    graph_refresh = 1;
    lastGlucoseVal = 0;
    newGlucose = 0;
    last_ampm_x = 0;
    last_sec_x = 0;
    last_min_x = 0;
    last_hour_x = 0;
    last_date_x = 0;
    last_ble_x = 0;
    last_bat_x = 0;
    last_notifications = 0;
    last_battery = -1;
}

void display_ui_task(uint8_t button) {
    // Display header
    int offset = updateTimeDisplay(DISP_WIDTH);
    offset = displayBattery(offset);
    offset = updateBLEstatusDisplay(offset);
    updateDateDisplay(-1);

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

    disp_commit();
}

static void initHomeScreen() {
    currentDisplayState = DISP_STATE_HOME;
    disp_fill_rect(0, DISP_HEADER_HEIGHT, DISP_WIDTH, DISP_HEIGHT, DISP_PIXEL_BLACK);
    rewriteTime = 1;
    rewriteMenu = 1;
}

static int my_bt_amt_notifications() {return 2;}
static void updateMainDisplay(uint8_t button) {
    if (currentDisplayState != DISP_STATE_HOME || startup) {
        initHomeScreen();
        startup = 0;
    }
    if (button & VIEW_BUTTON) {
        viewNotifications(0);
    } else if (button & GRAPH_BUTTON) {
        showGraphView(0);
    } else {
        if (rewriteMenu || newGlucose) {
            updateGlucoseDisplay(lastGlucoseVal);
            newGlucose = 0;
        }
        if (rewriteMenu || lastAmtNotificationsShown != my_bt_amt_notifications()) {
            lastAmtNotificationsShown = my_bt_amt_notifications();
            disp_set_font(FONT_MEDIUM);
            disp_set_pos(0, menuTextY[3]);
            if (!my_bt_amt_notifications()) {
                disp_write_str_group("No notifications.", NOTIFICATION_NUM_ID);
                disp_end_group();
            } else {
                disp_write_str_group(itoa(my_bt_amt_notifications(), buffer, 10), NOTIFICATION_NUM_ID);
                disp_write_str_group(" notification", NOTIFICATION_NUM_ID);
                if (my_bt_amt_notifications() > 1)
                    disp_write_str_group("s", NOTIFICATION_NUM_ID);
                disp_write_str_group(".", NOTIFICATION_NUM_ID);
                disp_end_group();
            }
            disp_set_font(FONT_SMALL);
            disp_set_pos(0, menuTextY[6]);
            disp_write_str("< Graph");
            uint8_t x1;
            uint8_t w;
            uint8_t y1;
            uint8_t h;
            disp_get_text_bounds("View >", 0, 0, &x1, &y1, &w, &h);
            disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
            disp_write_str("View >");
            rewriteMenu = 0;
        }
    }
}

static void viewNotifications(uint8_t button) {
    if (currentDisplayState != DISP_STATE_NOTIFICATION || startup) {
        currentDisplayState = DISP_STATE_NOTIFICATION;
        disp_fill_rect(0, DISP_HEADER_HEIGHT, DISP_WIDTH, DISP_HEIGHT, DISP_PIXEL_BLACK);
        startup = 0;
    }
    if (button == VIEW_BUTTON) {
        updateMainDisplay(0);
    } else if (my_bt_amt_notifications() && button == CLR_BUTTON) {
        // bt_clear_amt_notifications();
        updateMainDisplay(0);
    } else {
        if (last_notifications != my_bt_amt_notifications()) {
            disp_set_font(FONT_MEDIUM);
            disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
            if (my_bt_amt_notifications()) {
                last_notifications = my_bt_amt_notifications();
                disp_set_pos(0, menuTextY[0]);
                disp_write_str_group("test1", NOTIFICATION_1_ID);
                disp_end_group();
                disp_set_pos(0, menuTextY[1]);
                disp_write_str_group("test2", NOTIFICATION_2_ID);
                disp_end_group();
                disp_set_pos(0, menuTextY[6]);
                disp_set_font(FONT_SMALL);
                disp_write_str("< Clear");
                uint8_t x1;
                uint8_t w;
                uint8_t y1;
                uint8_t h;
                disp_get_text_bounds("Back >", 0, 0, &x1, &y1, &w, &h);
                disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
                disp_write_str("Back >");
            } else {
                disp_set_pos(0, menuTextY[0]);
                disp_write_str_group("No notifications.", NOTIFICATION_1_ID);
                disp_end_group();
                disp_remove_str_group(NOTIFICATION_2_ID);
                uint8_t x1;
                uint8_t w;
                uint8_t y1;
                uint8_t h;
                disp_set_font(FONT_SMALL);
                disp_get_text_bounds("Back >", 0, 0, &x1, &y1, &w, &h);
                disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
                disp_write_str("Back >");
            }
        }
    }
}

static void showGraphView(uint8_t button) {
    if (currentDisplayState != DISP_STATE_GRAPH || startup) {
        currentDisplayState = DISP_STATE_GRAPH;
        graph_refresh = 1;
        disp_fill_rect(0, DISP_HEADER_HEIGHT, DISP_WIDTH, DISP_HEIGHT, DISP_PIXEL_BLACK);
        startup = 0;
    }
    if (button == GRAPH_BUTTON) {
        updateMainDisplay(0);
    } else {
        // if (button == VIEW_BUTTON) {
        //     take_measurement(1);
        // }
        disp_set_font(FONT_MEDIUM);
        disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);

        if (graph_length()==0) {
            disp_set_pos(0, menuTextY[2]);
            disp_write_str_group("No Data.", DATA_TOP_ID);
            disp_end_group();
            disp_remove_str_group(DATA_BOTTOM_ID);
            disp_remove_str_group(GLUCOSE_VAL_GRAPH_ID);
        } else if (graph_changed() || graph_refresh) {
            graph_refresh = 0;
            if (first_data) {
                first_data = 0;
                disp_fill_rect(0, DISP_HEADER_HEIGHT, DISP_WIDTH, DISP_HEIGHT, DISP_PIXEL_BLACK);
            }
            graph(0);
            disp_set_font(FONT_SMALL);
            ftoa(min_max_buffer, graph_max(), 1);
            disp_set_pos(0, menuTextY[0]-7);
            disp_write_str_group(min_max_buffer, DATA_TOP_ID);
            disp_end_group();
            ftoa(min_max_buffer, graph_min(), 1);
            disp_set_pos(0, menuTextY[3]+7);
            disp_write_str_group(min_max_buffer, DATA_BOTTOM_ID);
            disp_end_group();

            uint8_t x1, x2;
            uint8_t w1, w2;
            uint8_t y1;
            uint8_t h;
            ftoa(buffer, lastGlucoseVal, 1);
            disp_get_text_bounds(buffer, 0, 0, &x1, &y1, &w1, &h);
            disp_get_text_bounds("mg/dL", 0, 0, &x2, &y1, &w2, &h);
            disp_set_pos(DISP_WIDTH-w1-w2-x1-x2-1, menuTextY[4]);
            disp_write_str_group(buffer, GLUCOSE_VAL_GRAPH_ID);
            disp_write_str_group("mg/dL", GLUCOSE_VAL_GRAPH_ID);
            disp_end_group();
        }

        disp_set_font(FONT_SMALL);
        disp_set_pos(0, menuTextY[6]);
        disp_write_str("< Back");

        uint8_t x1;
        uint8_t w;
        uint8_t y1;
        uint8_t h;
        disp_get_text_bounds("Measure >", 0, 0, &x1, &y1, &w, &h);
        disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
        disp_write_str("Measure >");
    }
}

void updateGraph(float glucose) {
    if (!graph_length()) {
        first_data = 1;
    }
    if (lastGlucoseVal != glucose) {
        lastGlucoseVal = glucose;
        newGlucose = 1;
    }
    add_to_graph(glucose);
}

static int updateDateDisplay(int xoff) {
    struct rtc_calendar_time time;
    rtc_get_time(&time);

    if (lastDisplayedDay == time.day)
        return last_date_x;
    lastDisplayedDay = time.day;
    disp_set_font(FONT_SMALL);
    disp_set_color(DISP_PIXEL_WHITE, DISP_BG_COLOR);
    if (xoff>=0) {
        uint8_t x0, x1, x2, x3;
        uint8_t w0, w1, w2, w3;
        uint8_t y;
        uint8_t h;
        disp_get_text_bounds(calendar_day_str(buffer, &time), 0, menuTextY[0], &x0, &y, &w0, &h);
        disp_get_text_bounds(itoa(time.month, buffer, 10), 0, menuTextY[0], &x1, &y, &w1, &h);
        disp_get_text_bounds(" /", 0, menuTextY[0], &x2, &y, &w2, &h);
        disp_get_text_bounds(itoa(time.day, buffer, 10), 0, menuTextY[0], &x3, &y, &w3, &h);
        last_date_x = xoff-(x0 + w0 + x1 + w1 + x2 + w2 + x3 + w3)-3;
    } else last_date_x = 2;
    disp_set_pos(last_date_x, headerTextY);
    disp_write_str_group(calendar_day_str(buffer, &time), DATE_ID);
    disp_write_str_group(" ", DATE_ID);
    disp_write_str_group(itoa(time.month, buffer, 10), DATE_ID);
    disp_write_str_group("/", DATE_ID);
    disp_write_str_group(itoa(time.day, buffer, 10), DATE_ID);
    disp_end_group();
    return last_date_x;
}

static void updateGlucoseDisplay(float glucose) {
    if (currentDisplayState != DISP_STATE_HOME)
        return;
    disp_set_font(FONT_LARGE);

    if (glucose < DISP_DANGER_LOW || glucose > DISP_DANGER_HIGH)
         disp_set_color(DISP_PIXEL_RED, DISP_PIXEL_BLACK);
    else if (glucose < DISP_WARNING_LOW || glucose > DISP_WARNING_HIGH)
         disp_set_color(DISP_PIXEL_YELLOW, DISP_PIXEL_BLACK);
    else
         disp_set_color(DISP_PIXEL_GREEN, DISP_PIXEL_BLACK);

    uint8_t x1;
    uint8_t w;
    uint8_t y1;
    uint8_t h;
    disp_get_text_bounds(itoa(round(glucose), buffer, 10), 0, menuTextY[0], &x1, &y1, &w, &h);
    disp_set_pos(0.80*DISP_WIDTH - w - x1, (menuTextY[0]+menuTextY[1])/2 + 4);
    disp_write_str_group(buffer, GLUCOSE_VAL_ID);
    disp_end_group();

    disp_set_font(FONT_SMALL);
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    disp_set_pos(0.80*DISP_WIDTH+12,menuTextY[0]);
    disp_write_str("mg");

    disp_get_text_bounds("mg", 0.80*DISP_WIDTH, menuTextY[0], &x1, &y1, &w, &h);
    disp_draw_line(0.80*DISP_WIDTH+12, y1+h+1, 0.80*DISP_WIDTH+12+w, y1+h+1, DISP_PIXEL_WHITE);
    disp_set_pos(0.80*DISP_WIDTH+12, menuTextY[0]+h+4);
    disp_write_str("dL");
}

// static void updateFreqDisplay(void) {
//     disp_fill_rect(0, menuTextY[3], 50, 20, DISP_PIXEL_BLACK);
//     disp_set_font(FONT_MEDIUM);
//     disp_set_pos(0, menuTextY[3]);
//     disp_write_str(itoa(freq, buffer, 10));
//     disp_write_str("Hz");
// }

static int updateTimeDisplay(int xoff) {
    struct rtc_calendar_time time;
    rtc_get_time(&time);

    disp_set_color(DISP_PIXEL_WHITE, DISP_BG_COLOR);
    disp_set_font(FONT_SMALL);

    if (rewriteTime || lastAMPMDisplayed != time.pm) {
        uint8_t x0, y, w0, h;
        if (time.pm) disp_get_text_bounds("PM", 0, 0, &x0, &y ,&w0, &h);
        else disp_get_text_bounds("AM", 0, 0, &x0, &y ,&w0, &h);
        uint8_t t = xoff - x0 - w0 - 2;
        if (t != last_ampm_x) {
            last_ampm_x = t;
            rewriteTime = 1;
        }
        disp_set_pos(last_ampm_x, headerTextY);
        if (time.pm) disp_write_str_group("PM", AMPM_ID);
        else disp_write_str_group("AM", AMPM_ID);
        disp_end_group();
    }

    if (rewriteTime || lastMinuteDisplayed != time.minute) {
        uint8_t x0=0, x1, y, w0=0, w1, h;
        if (time.minute < 10) disp_get_text_bounds("0", 0, 0, &x0, &y ,&w0, &h);
        disp_get_text_bounds(itoa(time.minute, buffer, 10), 0, 0, &x1, &y ,&w1, &h);
        uint8_t t = last_ampm_x-x0-w0-x1-w1-2;
        if (t != last_min_x) {
            last_min_x = t;
            rewriteTime = 1;
        }
        disp_set_pos(last_min_x, headerTextY);
        if (time.minute < 10) disp_write_str_group("0", MIN_ID);
        disp_write_str_group(buffer, MIN_ID);
        disp_end_group();
    }

    if (rewriteTime || lastSecondDisplayed != time.second) {
        uint8_t x0, y, w0, h;
        disp_get_text_bounds(":", 0, 0, &x0, &y ,&w0, &h);
        uint8_t t = last_min_x-x0-w0-2;
        if (t != last_sec_x) {
            last_sec_x = t;
            rewriteTime = 1;
        }
        disp_set_pos(last_sec_x, headerTextY);
        if (!(time.second%2)) {
            disp_write_str_group(":", SEC_ID);
            disp_end_group();
        } else {
            disp_remove_str_group(SEC_ID);
        }
    }

    if (rewriteTime || lastHourDisplayed != time.hour) {
        uint8_t x1, y, w1, h;
        disp_get_text_bounds(itoa(time.hour, buffer, 10), 0, 0, &x1, &y ,&w1, &h);
        last_hour_x = last_sec_x-x1-w1-2;
        disp_set_pos(last_hour_x, headerTextY);
        disp_write_str_group(buffer, HOUR_ID);
        disp_end_group();
    }

    lastHourDisplayed = time.hour;
    lastMinuteDisplayed = time.minute;
    lastSecondDisplayed = time.second;
    lastAMPMDisplayed = time.pm;
    rewriteTime = 0;
    return last_hour_x;
}

#define disp_ble_s  2
#define disp_ble_y  6
static uint8_t my_bt_connection_state() {return 1;}
static int updateBLEstatusDisplay(int xoff) {
    int x = xoff - 8;
    if (my_bt_connection_state() == ble_connection_displayed_state && last_ble_x == x)
        return x-disp_ble_s-4;
    if (last_ble_x >= disp_ble_s)
        disp_fill_rect(last_ble_x-disp_ble_s, disp_ble_y-2*disp_ble_s, 2*disp_ble_s+1, 9, DISP_BG_COLOR);
    last_ble_x = x;
    ble_connection_displayed_state = my_bt_connection_state();
    uint16_t color = DISP_PIXEL_RED;
    if (ble_connection_displayed_state)
        color = DISP_PIXEL_GREEN;
    disp_draw_line(x, disp_ble_y + 2*disp_ble_s, x, disp_ble_y - 2*disp_ble_s, color);
    disp_draw_line(x - disp_ble_s, disp_ble_y + disp_ble_s, x + disp_ble_s, disp_ble_y - disp_ble_s, color);
    disp_draw_line(x + disp_ble_s, disp_ble_y + disp_ble_s, x - disp_ble_s, disp_ble_y - disp_ble_s, color);
    disp_draw_line(x, disp_ble_y + 2*disp_ble_s, x + disp_ble_s, disp_ble_y + disp_ble_s, color);
    disp_draw_line(x, disp_ble_y - 2*disp_ble_s, x + disp_ble_s, disp_ble_y - disp_ble_s, color);
    return x-disp_ble_s-4;
}

// TODO: make battery ADC read into interrupt and nonblocking
#define disp_bat_height  6
#define disp_bat_length  20
#define disp_bat_y       3
static int displayBattery(int xoff) {
    uint8_t x = xoff - disp_bat_length - 8;
    if (last_bat_x != x && last_bat_x > 0) {
        disp_fill_rect(last_bat_x-1, disp_bat_y-1, disp_bat_length+3, disp_bat_height+3, DISP_BG_COLOR);
    }
    last_bat_x = x;
    int battery = 15;//get_battery_level(disp_bat_length);
    if (battery!=last_battery) {
        uint16_t color;
        if (battery > disp_bat_length/3) {
            color = DISP_PIXEL_GREEN;
        } else {
            color = DISP_PIXEL_RED;
        }
        disp_draw_line(x - 1, disp_bat_y, x - 1, disp_bat_y + disp_bat_height, DISP_PIXEL_WHITE); //left boarder
        disp_draw_line(x - 1, disp_bat_y - 1, x + disp_bat_length, disp_bat_y - 1, DISP_PIXEL_WHITE); //top border
        disp_draw_line(x - 1, disp_bat_y + disp_bat_height + 1, x + disp_bat_length, disp_bat_y + disp_bat_height + 1, DISP_PIXEL_WHITE); //bottom border
        disp_draw_line(x + disp_bat_length, disp_bat_y - 1, x + disp_bat_length, disp_bat_y + disp_bat_height + 1, DISP_PIXEL_WHITE); //right border
        disp_draw_line(x + disp_bat_length + 1, disp_bat_y + 2, x + disp_bat_length + 1, disp_bat_y + disp_bat_height - 2, DISP_PIXEL_WHITE); //right border
        for (uint8_t q = 0; q < battery; q++) {
            disp_draw_line(x + q, disp_bat_y, x + q, disp_bat_y + disp_bat_height, color);
        }
        for (uint8_t q = battery; q < last_battery; q++) {
            disp_draw_line(x + q, disp_bat_y, x + q, disp_bat_y + disp_bat_height, DISP_BG_COLOR);
        }
        last_battery = battery;
    }
    #if DEBUG_MODE==DEBUG_BATTERY
    disp_fill_rect(0,2*headerTextY,30,30-headerTextY,DISP_PIXEL_MAGENTA);
    disp_set_pos(5, menuTextY[0]);
    disp_write_str(ftoa(buffer, battery, 1));
    #endif
    return x-1;
}
