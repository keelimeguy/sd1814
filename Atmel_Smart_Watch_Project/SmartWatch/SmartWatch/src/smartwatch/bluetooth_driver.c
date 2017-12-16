/* UConn Senior Design Team 1814, November 2017
*/

#include "bluetooth_driver.h"

static volatile uint8_t rx_buffer[BT_MAX_BUFFER_LENGTH][BT_MAX_MSG_LENGTH];
static volatile uint8_t tx_buffer[BT_MAX_MSG_LENGTH];
static volatile uint8_t rx_buffer_len, write_busy;
static volatile uint8_t connection_state;
static volatile uint8_t cur_rxindx;
static uint8_t num_notifications, cur_rindx;
static char notificationLine1[BT_MAX_MSG_LENGTH];
static char notificationLine2[BT_MAX_MSG_LENGTH];
static char paramData[BT_MAX_MSG_LENGTH];

void bluetooth_driver_init(void) {

    rx_buffer_len = 0;
    cur_rxindx = 0;
    cur_rindx = 0;
    write_busy = 0;
    connection_state = BT_DISCONNECTED;
    for (int i=0; i< BT_MAX_BUFFER_LENGTH; i++)
        for (int j=0; j< BT_MAX_MSG_LENGTH; j++)
            rx_buffer[i][j] = 0;

    BLEsetup();
  //usart_read_buffer_job(&usart_instance, rx_buffer, BT_MAX_BUFFER_LENGTH);
}

uint8_t is_bt_active_soft(void) {
    return rx_buffer_len|write_busy;
}

uint8_t is_bt_active(void) {
    return rx_buffer_len|write_busy;
}

void bt_task(void) {
    if (rx_buffer_len) {
        rx_buffer_len--;

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
        } else if (rx_buffer[cur_rindx][0] == '1') {
            num_notifications|=1;
            memcpy(notificationLine1, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            notificationLine1[BT_MAX_MSG_LENGTH - 1] = '\0';
            request_screen_on();
        } else if (rx_buffer[cur_rindx][0] == '2') {
            num_notifications|=2;
            memcpy(notificationLine2, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            notificationLine2[BT_MAX_MSG_LENGTH - 1] = '\0';
            request_screen_on();
        }

        // Not sure what the following commands specifically do.. copied over from 2016-17 project
        else if (rx_buffer[cur_rindx][0] == 'C') {
            memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            char *ptr;
            do_kalman_bt_cmd(strtol(paramData, &ptr, 10));
            request_screen_on();
        } else if (rx_buffer[cur_rindx][0] == 'P') {
            memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            char *ptr;
            measure_set_pulse_one(1000*(uint32_t)strtol(paramData, &ptr, 10));
            measure_set_pulse_two(1000*(uint32_t)strtol(ptr, &ptr, 10));
            measure_set_pulse_three(1000*(uint32_t)strtol(ptr, &ptr, 10));
            request_screen_on();
        } else if (rx_buffer[cur_rindx][0] == 'R') {
            memcpy(paramData, &rx_buffer[cur_rindx][1], BT_MAX_MSG_LENGTH - 1);
            char *ptr;
            long val = strtol(paramData, &ptr, 10);
            kalman_setT(val/60);
            set_pulse_timeout(val*60*1000);
            request_screen_on();
        }

        if (cur_rindx == cur_rxindx) {
            //start_read();
        }
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
    return num_notifications>1 ? (num_notifications==3) + 1 : num_notifications;
}

void bt_clear_amt_notifications(void) {
    num_notifications = 0;
}

char* bt_get_notification_1(void) {
    return notificationLine1;
}

char* bt_get_notification_2(void) {
    return notificationLine2;
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
