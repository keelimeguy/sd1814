/* UConn Senior Design Team 1814, November 2017
*/

#include "bluetooth_driver.h"

static struct usart_module usart_instance;
static volatile uint8_t rx_buffer[BT_MAX_RX_BUFFER_LENGTH][BT_MAX_MSG_LENGTH];
static volatile uint8_t data_available, write_busy;
static volatile uint8_t connection_state;
static volatile uint8_t cur_windx;
static uint8_t num_notifications, cur_rindx;
static char notificationLine1[BT_MAX_MSG_LENGTH];
static char notificationLine2[BT_MAX_MSG_LENGTH];
static char paramData[BT_MAX_MSG_LENGTH];

static void bt_read_callback(struct usart_module *const usart_module);
static void bt_write_callback(struct usart_module *const usart_module);

void bluetooth_driver_init(void) {
    struct usart_config config_usart;
    usart_get_config_defaults(&config_usart);

    config_usart.mux_setting = BT_USART_PINMUX_SETTING;
    config_usart.pinmux_pad0 = BT_USART_PINMUX_PAD0;
    config_usart.pinmux_pad1 = BT_USART_PINMUX_PAD1;
    config_usart.pinmux_pad2 = BT_USART_PINMUX_PAD2;
    config_usart.pinmux_pad3 = BT_USART_PINMUX_PAD3;
    config_usart.baudrate    = BT_USART_BAUDRATE;

    while (usart_init(&usart_instance, BT_USART, &config_usart) != STATUS_OK) {
    }

    usart_enable(&usart_instance);

    usart_register_callback(&usart_instance, bt_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_register_callback(&usart_instance, bt_read_callback, USART_CALLBACK_BUFFER_RECEIVED);

    usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
    usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);

    data_available = 0;
    cur_windx = 0;
    cur_rindx = 0;
    write_busy = 0;
    connection_state = BT_DISCONNECTED;
    for (int i=0; i< BT_MAX_RX_BUFFER_LENGTH; i++)
        for (int j=0; j< BT_MAX_MSG_LENGTH; j++)
            rx_buffer[i][j] = 0;

    usart_read_buffer_job(&usart_instance, rx_buffer, BT_MAX_RX_BUFFER_LENGTH);
}

uint8_t is_bt_active_soft(void) {
    return data_available|write_busy;
}

uint8_t is_bt_active(void) {
    return data_available|write_busy;
}

void bt_task(void) {
    if (data_available) {
        data_available--;

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
            long val = strtol(paramData, &ptr, 10);
            char *ptr;
            kalman_setT(val/60);
            set_pulse_timeout(val*60*1000);
            request_screen_on();
        }

        if (cur_rindx == cur_windx) {
            usart_read_buffer_job(usart_instance, rx_buffer[cur_windx], BT_MAX_MSG_LENGTH);
        }
        if (++cur_rindx >= BT_MAX_RX_BUFFER_LENGTH) cur_rindx=0;
    }
}

void bt_write(uint8_t *tx_data, uint16_t length) {
    write_busy = 1;
    usart_write_buffer_job(&usart_instance, tx_data, length);
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

static void bt_read_callback(struct usart_module *const usart_module) {
    connection_state = BT_CONNECTED;
    if (++data_available < BT_MAX_RX_BUFFER_LENGTH) {
        if(++cur_windx >= BT_MAX_RX_BUFFER_LENGTH) cur_windx = 0;
        usart_read_buffer_job(&usart_instance, rx_buffer[cur_windx], BT_MAX_MSG_LENGTH);
    }
}

static void bt_write_callback(struct usart_module *const usart_module) {
    write_busy = 0;
}
