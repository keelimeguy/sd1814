/* UConn Senior Design Team 1814, January 2018
*/

#include "bluetooth_driver.h"

static volatile uint8_t rx_buffer[BT_MAX_BUFFER_LENGTH][BT_MAX_MSG_LENGTH];
static volatile uint8_t tx_buffer[BT_MAX_MSG_LENGTH];
static volatile uint8_t rx_buffer_len, write_busy;
static volatile uint8_t connection_state;
static volatile uint8_t cur_rxindx;
static uint8_t num_notifications, cur_rindx, new_notifications;
static char notificationLine1[BT_MAX_MSG_LENGTH];
static char notificationLine2[BT_MAX_MSG_LENGTH];
static char paramData[BT_MAX_MSG_LENGTH];

void bluetooth_driver_init(void) {

    rx_buffer_len = 0;
    cur_rxindx = 0;
    cur_rindx = 0;
    write_busy = 0;
    new_notifications = 1;
    connection_state = BT_DISCONNECTED;
    for (int i=0; i< BT_MAX_BUFFER_LENGTH; i++)
        for (int j=0; j< BT_MAX_MSG_LENGTH; j++)
            rx_buffer[i][j] = 0;

	#if BLE_ENABLED
		BLEsetup();
		//start_read();
	#endif
}

uint8_t is_bt_active_soft(void) {
    return rx_buffer_len|write_busy;
}

uint8_t is_bt_active(void) {
    return rx_buffer_len|write_busy;
}

void bt_task(void) {
    aci_loop();

    if (rx_buffer_len) {
        rx_buffer_len--;

        // Commands copied over from 2016-17 project

        // Set date and time
        if (rx_buffer[cur_rindx][0] == 'D') {
            // expect date/time string- example: DYYYY MM DD HH MM SS (D[Year] [Month] [Day] [Hr] [Min] [Sec])
            struct rtc_calendar_time time;
            rtc_get_time(&time);
            char *ptr;
            time.year = strtol(&rx_buffer[cur_rindx][1],&ptr,10);
            time.month = strtol(ptr,&ptr,10);
            time.day = strtol(ptr,&ptr,10);
            time.hour = strtol(ptr,&ptr,10);
            time.minute = strtol(ptr,&ptr,10);
            time.second = strtol(ptr,&ptr,10);
            rtc_update_time(&time);
            request_screen_on();
        }

        // Set notification 1
        else if (rx_buffer[cur_rindx][0] == '1') {
            bt_set_notification_1(&rx_buffer[cur_rindx][1]);
            request_screen_on();
            new_notifications = 1;
        }

        // Set notification 2
        else if (rx_buffer[cur_rindx][0] == '2') {
            bt_set_notification_2(&rx_buffer[cur_rindx][1]);
            request_screen_on();
            new_notifications = 1;
        }

        // Perform kalman calculation
        else if (rx_buffer[cur_rindx][0] == 'C') {
            memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            char *ptr;
            do_kalman_bt_cmd(strtol(paramData, &ptr, 10));
            request_screen_on();
        }

        // Set measurement pulse timeout in seconds
        else if (rx_buffer[cur_rindx][0] == 'P') {
            memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            char *ptr;
            measure_set_pulse_one((uint32_t)strtol(paramData, &ptr, 10));
            measure_set_pulse_two((uint32_t)strtol(ptr, &ptr, 10));
            measure_set_pulse_three((uint32_t)strtol(ptr, &ptr, 10));
            request_screen_on();
        }

        // Set measurement reading timeout in seconds
        else if (rx_buffer[cur_rindx][0] == 'R') {
            memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            char *ptr;
            long val = strtol(paramData, &ptr, 10);

            // TODO: Verify these changes with app interface
            kalman_setT(val/360000);
            measure_set_reading_timeout(val);
            // kalman_setT(val/60);
            // measure_set_reading_timeout(val*60*1000);
            request_screen_on();
        }

        #if DEBUG_MODE==DEBUG_BLE
            // Debug command to change LED
            else if (rx_buffer[cur_rindx][0] == 'L') {
                memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
                char *ptr;
                long val = strtol(paramData, &ptr, 10);
                port_pin_set_output_level(BOARD_DEBUG_LED, (val>0));
            }
        #endif

        // if (cur_rindx == cur_rxindx) {
        //     start_read();
        // }
        if (++cur_rindx >= BT_MAX_BUFFER_LENGTH) cur_rindx=0;
    }
}

uint8_t bt_write(uint8_t *tx_data, uint16_t length) {
    if (length > BT_MAX_MSG_LENGTH-1)
        return 0;
    write_busy = 1;
    uart_tx(tx_data, length);
    return 1;
}

uint8_t bt_amt_notifications(void) {
    #if DEBUG_MODE == DEBUG_CLOCK
    return 2;
    #else
    return num_notifications>1 ? (num_notifications==3) + 1 : num_notifications;
    #endif
}

uint8_t bt_new_notifications(void) {
    if (new_notifications) {
        new_notifications = 0;
        return 1;
    }
    return 0;
}

uint8_t bt_new_notifications_soft(void) {
    return new_notifications;
}

void bt_clear_amt_notifications(void) {
    num_notifications = 0;
}

char* bt_get_notification_1(void) {
    #if DEBUG_MODE == DEBUG_CLOCK
    return __DATE__;
    #else
    return notificationLine1;
    #endif
}

char* bt_get_notification_2(void) {
    #if DEBUG_MODE == DEBUG_CLOCK
    return __TIME__;
    #else
    return notificationLine2;
    #endif
}

void bt_set_notification_1(char* str) {
    num_notifications|=1;
    memcpy(notificationLine1, str, BT_MAX_MSG_LENGTH - 1);
    notificationLine1[BT_MAX_MSG_LENGTH - 1] = '\0';
}

void bt_set_notification_2(char* str) {
    num_notifications|=2;
    memcpy(notificationLine2, str, BT_MAX_MSG_LENGTH - 1);
    notificationLine2[BT_MAX_MSG_LENGTH - 1] = '\0';
}

uint8_t bt_connection_state(void) {
    return connection_state;
}

void bt_set_connection_state(uint8_t state) {
    connection_state = state;
}

void set_ble_rx_buffer(int i, uint8_t val) {
    if (i < BT_MAX_MSG_LENGTH)
        rx_buffer[cur_rxindx][i] = val;
}

void bt_read_callback() {
    if (++rx_buffer_len < BT_MAX_BUFFER_LENGTH) {
        if(++cur_rxindx >= BT_MAX_BUFFER_LENGTH) cur_rxindx = 0;
        //start_read();
    }
}

void bt_write_callback() {
    write_busy = 0;
}
