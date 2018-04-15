/* UConn Senior Design Team 1814, January 2018
*/

#include "display_manager.h"
#include "display_driver/grapher/util.h"

#include "display_driver/display_driver.h"
#include "display_driver/grapher/grapher.h"

#include "display_driver/GFXfont/Fonts/Org_01.h"
#include "display_driver/GFXfont/Fonts/FreeSans9pt7b.h"

#define FONT_SMALL               &Org_01
#define FONT_MEDIUM              &FreeSans9pt7b

#define FONT_DEFAULT             FONT_MEDIUM
#define round(f) ( (f-(float)((int)f)) > 0.5 ? (int)f+1 : (int)f )

static volatile uint8_t startup;
static volatile uint8_t rewriteMain;
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
static uint16_t lastGlucoseVal;
static int last_battery;
static uint8_t last_bat_x, last_ble_x, last_date_x, last_hour_x, last_min_x, last_sec_x, last_ampm_x;

static uint8_t last_drawn[MAX_WRITE_ID+1];

static char buffer[12];
#define headerTextY 8
const static uint8_t menuTextY[7] = { 2*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1, 4*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1,
                                2*DISP_DIVISION_HEIGHT+DISP_HEADER_HEIGHT-1, 8*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1,
                                10*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1, 4*DISP_DIVISION_HEIGHT+DISP_HEADER_HEIGHT-1,
                                14*DISP_DIVISION_HEIGHT/3+DISP_HEADER_HEIGHT-1 };
static void initScreen(void);
static void updateMainDisplay(uint8_t button);
static void showGraphView(uint8_t button);
static void viewNotifications(uint8_t button);
static void updateGlucoseDisplay(uint16_t glucose);
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
    rewriteMain = 1;
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
    last_notifications = -1;
    last_battery = -1;

    for (int i = 0; i <= MAX_WRITE_ID; i++)
        last_drawn[i] = 0;
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
            currentDisplayState = DISP_STATE_HOME;
            rewriteMain = 1;
            initScreen();
    }

    if (startup) {
        disp_commit();
        startup = 0;
    }
}

static void initScreen() {
    disp_fill_rect(0, DISP_HEADER_HEIGHT, DISP_WIDTH, DISP_HEIGHT, DISP_PIXEL_BLACK);
}

static void updateMainDisplay(uint8_t button) {
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    if (startup) {
        currentDisplayState = DISP_STATE_HOME;
        rewriteMain = 1;
        initScreen();
    } else if (currentDisplayState != DISP_STATE_HOME) {
        currentDisplayState = DISP_STATE_HOME;
        rewriteMain = 1;
        // Assuming button IDs are only IDs greater than graph ID
        for (int i = 0; i <= GRAPH_ID; i++) {
            if (last_drawn[i]) disp_remove_str_group(i);
            last_drawn[i] = 0;
        }
    }
    if (button & VIEW_BUTTON) {
        viewNotifications(0);
    } else if (button & GRAPH_BUTTON) {
        showGraphView(0);
    } else {
        if (rewriteMain || newGlucose) {
            updateGlucoseDisplay(lastGlucoseVal);
            newGlucose = 0;
        }
        if (rewriteMain || lastAmtNotificationsShown != bt_amt_notifications()) {
            lastAmtNotificationsShown = bt_amt_notifications();
            disp_set_font(FONT_MEDIUM);
            disp_set_pos(1, (menuTextY[4]+menuTextY[5])/2);
            if (!bt_amt_notifications()) {
                disp_write_str_group("No notifications.", NOTIFICATION_NUM_ID);
                disp_end_group();
                last_drawn[NOTIFICATION_NUM_ID] = 1;
            } else {
                disp_write_str_group(itoa(bt_amt_notifications(), buffer, 10), NOTIFICATION_NUM_ID);
                disp_write_str_group(" notification", NOTIFICATION_NUM_ID);
                if (bt_amt_notifications() > 1)
                    disp_write_str_group("s", NOTIFICATION_NUM_ID);
                disp_write_str_group(".", NOTIFICATION_NUM_ID);
                disp_end_group();
                last_drawn[NOTIFICATION_NUM_ID] = 1;
            }
            if (!startup)
                disp_commit();
            disp_set_font(FONT_SMALL);
            disp_set_pos(1, menuTextY[6]);
            disp_write_str_group("< Graph", LEFT_BUTTON_ID);
            disp_end_group();
            if (!startup)
                disp_commit();
            last_drawn[LEFT_BUTTON_ID] = 1;
            uint8_t x1;
            uint8_t w;
            uint8_t y1;
            uint8_t h;
            disp_get_text_bounds("View >", 0, 0, &x1, &y1, &w, &h);
            disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
            disp_write_str_group("View >", RIGHT_BUTTON_ID);
            disp_end_group();
            if (!startup)
                disp_commit();
            last_drawn[RIGHT_BUTTON_ID] = 1;
            rewriteMain = 0;
        }
    }
}

static void viewNotifications(uint8_t button) {
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    if (startup) {
        currentDisplayState = DISP_STATE_NOTIFICATION;
        last_notifications = -1;
        initScreen();
    } else if (currentDisplayState != DISP_STATE_NOTIFICATION) {
        currentDisplayState = DISP_STATE_NOTIFICATION;
        // Assuming button IDs are only IDs greater than graph ID
        for (int i = 0; i <= GRAPH_ID; i++) {
            if (last_drawn[i]) disp_remove_str_group(i);
            last_drawn[i] = 0;
        }
        last_notifications = -1;
    }
    if (button == VIEW_BUTTON) {
        updateMainDisplay(0);
    } else if (bt_amt_notifications() && button == CLR_BUTTON) {
        bt_clear_amt_notifications();
        updateMainDisplay(0);
    } else {
        if (bt_new_notifications() || last_notifications != bt_amt_notifications()) {
            disp_set_font(FONT_MEDIUM);
            last_notifications = bt_amt_notifications();
            if (bt_amt_notifications()) {
                disp_set_pos(1, menuTextY[0]);
                disp_write_str_group(bt_get_notification_1(), NOTIFICATION_1_ID);
                disp_end_group();
                last_drawn[NOTIFICATION_1_ID] = 1;
                disp_set_pos(1, menuTextY[1]);
                disp_write_str_group(bt_get_notification_2(), NOTIFICATION_2_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[NOTIFICATION_2_ID] = 1;
                disp_set_pos(1, menuTextY[6]);
                disp_set_font(FONT_SMALL);
                disp_write_str_group("< Clear", LEFT_BUTTON_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[LEFT_BUTTON_ID] = 1;
                uint8_t x1;
                uint8_t w;
                uint8_t y1;
                uint8_t h;
                disp_get_text_bounds("Back >", 0, 0, &x1, &y1, &w, &h);
                disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
                disp_write_str_group("Back >", RIGHT_BUTTON_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[RIGHT_BUTTON_ID] = 1;
            } else {
                disp_set_pos(1, menuTextY[0]);
                disp_write_str_group("No notifications.", NOTIFICATION_1_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[NOTIFICATION_1_ID] = 1;
                disp_remove_str_group(NOTIFICATION_2_ID);
                uint8_t x1;
                uint8_t w;
                uint8_t y1;
                uint8_t h;
                disp_set_font(FONT_SMALL);
                disp_get_text_bounds("Back >", 0, 0, &x1, &y1, &w, &h);
                disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
                disp_write_str_group("Back >", RIGHT_BUTTON_ID);
                disp_end_group();
                disp_remove_str_group(LEFT_BUTTON_ID);
                if (!startup)
                    disp_commit();
                last_drawn[RIGHT_BUTTON_ID] = 1;
            }
        }
    }
}

static void showGraphView(uint8_t button) {
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    if (startup) {
        currentDisplayState = DISP_STATE_GRAPH;
        graph_refresh = 1;
        initScreen();
    } else if (currentDisplayState != DISP_STATE_GRAPH) {
        currentDisplayState = DISP_STATE_GRAPH;
        graph_refresh = 1;
        // Assuming button IDs are only IDs greater than graph ID
        for (int i = 0; i <= GRAPH_ID; i++) {
            if (last_drawn[i]) disp_remove_str_group(i);
            last_drawn[i] = 0;
        }
    }
    if (button == GRAPH_BUTTON) {
        updateMainDisplay(0);
    } else {
        if (graph_changed() || graph_refresh) {
            disp_set_font(FONT_SMALL);
            if (graph_length()==0) {
                disp_set_pos(1, menuTextY[2]);
                disp_write_str_group("No Data.", DATA_TOP_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[DATA_TOP_ID] = 1;
                disp_remove_str_group(DATA_BOTTOM_ID);
                disp_remove_str_group(GLUCOSE_VAL_GRAPH_ID);
                disp_remove_str_group(GRAPH_ID);
            } else {
                if (first_data) {
                    first_data = 0;
                    disp_remove_str_group(DATA_TOP_ID);
                    disp_commit();
                }
                uint16_t x, y, width, height;
                disp_remove_str_group(GRAPH_ID);
                graph_smart_sizing(&x, &y, &width, &height);
                disp_set_str_group(x, y, width, height, GRAPH_ID);
                if (!startup)
                    disp_commit();
                last_drawn[GRAPH_ID] = 1;
                disp_set_font(FONT_SMALL);
                ftoa(buffer, graph_max(), 1);
                disp_set_pos(1, menuTextY[0]-7);
                disp_write_str_group(buffer, DATA_TOP_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[DATA_TOP_ID] = 1;
                ftoa(buffer, graph_min(), 1);
                disp_set_pos(1, menuTextY[3]+11);
                disp_write_str_group(buffer, DATA_BOTTOM_ID);
                disp_end_group();
                if (!startup)
                    disp_commit();
                last_drawn[DATA_BOTTOM_ID] = 1;

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
                if (!startup)
                    disp_commit();
                last_drawn[GLUCOSE_VAL_GRAPH_ID] = 1;
            }
        }

        if (graph_refresh) {
            disp_set_font(FONT_SMALL);
            disp_set_pos(1, menuTextY[6]);
            disp_write_str_group("< Back", LEFT_BUTTON_ID);
            disp_end_group();
            if (!startup)
                disp_commit();
            last_drawn[LEFT_BUTTON_ID] = 1;

            uint8_t x1;
            uint8_t w;
            uint8_t y1;
            uint8_t h;
            disp_get_text_bounds("Measure >", 0, 0, &x1, &y1, &w, &h);
            disp_set_pos(DISP_WIDTH-w-1, menuTextY[6]);
            disp_write_str_group("Measure >", RIGHT_BUTTON_ID);
            if (!startup)
                disp_commit();
            disp_end_group();
            last_drawn[RIGHT_BUTTON_ID] = 1;

            graph_refresh = 0;
        }

        if (button == VIEW_BUTTON) {
            take_measurement(1);
        }
    }
}

void updateGraph(uint16_t glucose) {
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
    disp_commit();
    return last_date_x;
}

static void updateGlucoseDisplay(uint16_t glucose) {
    if (currentDisplayState != DISP_STATE_HOME)
        return;

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

    #if DEBUG_MODE == DEBUG_MEASURE_FREQ
    disp_set_font(FONT_SMALL);
    disp_get_text_bounds(itoa(round(get_freq()), buffer, 10), 0, menuTextY[0], &x1, &y1, &w, &h);
    disp_set_pos(1, (menuTextY[0]+menuTextY[1])/2 + 4);
    disp_write_str_group(buffer, GLUCOSE_VAL_ID);
    #endif

    disp_set_font(FONT_MEDIUM);
    disp_set_font_scale(3);
    disp_get_text_bounds(itoa(glucose, buffer, 10), 0, menuTextY[1], &x1, &y1, &w, &h);
    disp_set_pos(0.80*DISP_WIDTH - w - x1, (menuTextY[1]+menuTextY[2])/2 + 4);
    disp_write_str_group(buffer, GLUCOSE_VAL_ID);
    disp_end_group();
    last_drawn[GLUCOSE_VAL_ID] = 1;

    disp_set_font(FONT_SMALL);
    disp_set_font_scale(2);
    disp_set_color(DISP_PIXEL_WHITE, DISP_PIXEL_BLACK);
    disp_set_pos(0.80*DISP_WIDTH+5,menuTextY[1]-3);
    disp_write_str_group("mg", GLUCOSE_UNIT_ID);

    disp_get_text_bounds("mg", 0.80*DISP_WIDTH+5, menuTextY[1]-3, &x1, &y1, &w, &h);
    disp_draw_line(0.80*DISP_WIDTH+5, y1+h+1, 0.80*DISP_WIDTH+5+w, y1+h+1, DISP_PIXEL_WHITE);
    disp_set_pos(0.80*DISP_WIDTH+5, menuTextY[1]+h+2);
    disp_write_str_group("dL", GLUCOSE_UNIT_ID);
    disp_end_group();
    disp_set_font_scale(1);
    disp_draw_line(x1+w-8, y1+h-1, x1+w-3, y1+h-1, DISP_PIXEL_WHITE);
    if (!startup)
        disp_commit();
    last_drawn[GLUCOSE_UNIT_ID] = 1;
}

static int updateTimeDisplay(int xoff) {
    uint8_t commit = 0;
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
        commit = 1;
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
        commit = 1;
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
        commit = 1;
    }

    if (rewriteTime || lastHourDisplayed != time.hour) {
        uint8_t x1, y, w1, h;
        disp_get_text_bounds(itoa(time.hour, buffer, 10), 0, 0, &x1, &y ,&w1, &h);
        last_hour_x = last_sec_x-x1-w1-2;
        disp_set_pos(last_hour_x, headerTextY);
        disp_write_str_group(buffer, HOUR_ID);
        disp_end_group();
        commit = 1;
    }

    lastHourDisplayed = time.hour;
    lastMinuteDisplayed = time.minute;
    lastSecondDisplayed = time.second;
    lastAMPMDisplayed = time.pm;
    if (commit)
        disp_commit();
    rewriteTime = 0;
    return last_hour_x;
}

#define disp_ble_s  2
#define disp_ble_y  6
static int updateBLEstatusDisplay(int xoff) {
    int x = xoff - 8;
    if (bt_connection_state() == ble_connection_displayed_state && last_ble_x == x)
        return x-disp_ble_s-4;
    if (last_ble_x >= disp_ble_s)
        disp_fill_rect(last_ble_x-disp_ble_s, disp_ble_y-2*disp_ble_s, 2*disp_ble_s+1, 9, DISP_BG_COLOR);
    last_ble_x = x;
    ble_connection_displayed_state = bt_connection_state();
    uint16_t color = DISP_PIXEL_RED;
    if (ble_connection_displayed_state)
        color = DISP_PIXEL_GREEN;
    disp_draw_line(x, disp_ble_y + 2*disp_ble_s, x, disp_ble_y - 2*disp_ble_s, color);
    disp_draw_line(x - disp_ble_s, disp_ble_y + disp_ble_s, x + disp_ble_s, disp_ble_y - disp_ble_s, color);
    disp_draw_line(x + disp_ble_s, disp_ble_y + disp_ble_s, x - disp_ble_s, disp_ble_y - disp_ble_s, color);
    disp_draw_line(x, disp_ble_y + 2*disp_ble_s, x + disp_ble_s, disp_ble_y + disp_ble_s, color);
    disp_draw_line(x, disp_ble_y - 2*disp_ble_s, x + disp_ble_s, disp_ble_y - disp_ble_s, color);
    disp_commit();
    return x-disp_ble_s-4;
}

// TODO: make battery ADC read into interrupt and nonblocking
#define disp_bat_height  6
#define disp_bat_length  20
#define disp_bat_y       3
static int displayBattery(int xoff) {
    uint8_t x = xoff - disp_bat_length - 8;
    uint8_t force = 0;
    if (last_bat_x != x && last_bat_x > 0) {
        disp_fill_rect(last_bat_x-1, disp_bat_y-1, disp_bat_length+3, disp_bat_height+3, DISP_BG_COLOR);
        force = 1;
    }
    last_bat_x = x;
    int battery = get_battery_level(disp_bat_length);
    if (force || battery!=last_battery) {
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
        disp_commit();
    }
    #if DEBUG_MODE==DEBUG_BATTERY
    disp_fill_rect(0,2*headerTextY,30,30-headerTextY,DISP_PIXEL_MAGENTA);
    disp_set_pos(5, menuTextY[0]);
    disp_write_str(ftoa(buffer, battery, 1));
    disp_commit();
    #endif
    return x-1;
}

void display_off(void) {
    disp_sleep_enable();
}

void display_on(void) {
    disp_sleep_disable();
}
