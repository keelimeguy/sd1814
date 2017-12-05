/* UConn Senior Design Team 1814, November 2017
*/

#include "bluetooth_driver.h"

static struct usart_module usart_instance;
static volatile uint8_t rx_buffer[BT_MAX_RX_BUFFER_LENGTH];
static volatile uint8_t data_available, write_busy;
static volatile uint8_t num_notifications, connection_state;

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
    write_busy = 0;
    connection_state = BT_DISCONNECTED;
    for (int i=0; i< BT_MAX_RX_BUFFER_LENGTH; i++)
        rx_buffer[i] = 0;

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
        num_notifications++;
        data_available--;
    }
}

void bt_write(uint8_t *tx_data, uint16_t length) {
    write_busy = 1;
    usart_write_buffer_job(&usart_instance, tx_data, length);
}

uint8_t bt_amt_notifications(void) {
    return num_notifications;
}

void bt_clear_amt_notifications(void) {
    num_notifications = 0;
}

uint8_t bt_connection_state(void) {
    return connection_state;
}

static void bt_read_callback(struct usart_module *const usart_module) {
    data_available++;
    usart_read_buffer_job(&usart_instance, rx_buffer, BT_MAX_RX_BUFFER_LENGTH);
    connection_state = BT_CONNECTED;
}

static void bt_write_callback(struct usart_module *const usart_module) {
    write_busy = 0;
}
