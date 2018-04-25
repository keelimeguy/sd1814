## Glucose Monitoring Smart Watch - UConn Senior Design Team 1814

### Notes:
- Board configuration/pinout: "src/config/conf_board.h"
- Configure startup time in: "src/config/conf_clocks.h"
- Warning levels for glucose defined in "src/smartwatch/display\_driver/display_driver.h"
- Application starts at "src/main.c"

### Firmware:
:alien: = COMPLETE
:raised_hand: = UNVERIFIED
:x: = INCOMPLETE

- :alien: **smartwatch/display_driver/**

- :raised_hand: **smartwatch/lib_aci/**

- **main.c**
    - **_SmartWatch.h_**
    - :alien: main()

- **SmartWatch.c**
    - **_SmartWatch.h_**
    - :alien: init_all()
    - :alien: smartwatch_task()
    - :alien: is_active()
    - :alien: is_screen_active_soft()
    - :alien: is_screen_active()
    - :alien: request_screen_on()
    - :alien: sleep()
    - :alien: wakeup()

- **smartwatch/battery_reader.c**
    - **_battery\_reader.h_**
    - :alien: battery_reader_init()
    - :raised_hand: battery_reader_task()
    - :alien: start_battery_read()
    - :alien: is_battery_active()
    - :raised_hand: get_battery_level()

- **smartwatch/bluetooth_driver.c**
    - **_bluetooth\_driver.h_**
    - :alien: bluetooth_driver_init()
    - :alien: is_bt_active_soft()
    - :alien: is_bt_active()
    - :alien: bt_task()
    - :alien: bt_write()
    - :alien: bt_amt_notifications()
    - :alien: bt_new_notifications_soft()
    - :alien: bt_new_notifications()
    - :alien: bt_clear_amt_notifications()
    - :alien: bt_get_notification_1()
    - :alien: bt_get_notification_2()
    - :alien: bt_connection_state()
    - :alien: bt_set_connection_state()
    - :alien: set_ble_rx_buffer()
    - :alien: bt_read_callback()
    - :alien: bt_write_callback()

- **smartwatch/button_listener.c**
    - **_button\_listener.h_**
    - :alien: button_listener_init()
    - :alien: button_listener_callback_L()
    - :alien: button_listener_callback_R()
    - :alien: is_button_interrupt_soft()
    - :alien: is_button_interrupt()
    - :alien: get_buttons_soft()
    - :alien: get_buttons()

- **smartwatch/clock_driver.c**
    - **_clock\_driver.h_**
    - **_date\_calc.h_**
    - :alien: clock_driver_init()
    - :alien: rtc_get_time()
    - :alien: rtc_update_time()
    - :alien: calendar_day_str()
    - :alien: is_reading_timeout_soft()
    - :alien: is_reading_timeout()
    - :alien: is_screen_timeout_soft()
    - :alien: is_screen_timeout()
    - :alien: set_screen_timeout()
    - :alien: screen_timer_callback()
    - :alien: is_pulse_timeout_soft()
    - :alien: is_pulse_timeout()
    - :alien: set_pulse_timeout()
    - :alien: pulse_timer_callback()
    - :alien: is_battery_timeout_soft()
    - :alien: is_battery_timeout()
    - :alien: set_battery_timeout()
    - :alien: battery_timer_callback()
    - :alien: rtc_alarm_callback()

- **smartwatch/display_manager.c**
    - **_display\_manager.h_**
    - :alien: display_manager_init()
    - :alien: display_ui_task()
    - :alien: updateMainDisplay()
    - :alien: viewNotifications()
    - :alien: showGraphView()
    - :alien: updateGraph()
    - :alien: updateDateDisplay()
    - :alien: updateGlucoseDisplay()
    - :alien: updateTimeDisplay()
    - :alien: updateBLEstatusDisplay()
    - :alien: displayBattery()

- **smartwatch/measurement_controller.c**
    - **_measurement\_controller.h_**
    - :alien: measurement_controller_init()
    - :alien: take_measurement()
    - :alien: is_measure_busy()
    - :alien: measure_set_reading_timeout()
    - :alien: measure_get_reading_timeout()
    - :alien: measure_set_pulse_one()
    - :alien: measure_set_pulse_two()
    - :alien: measure_set_pulse_three()
    - :alien: measurement_task()
    - :alien: do_measurement()
    - :alien: get_measurement()
    - :alien: is_new_measurement_soft()
    - :alien: is_new_measurement()
    - :alien: do_kalman_bt_cmd()
    - :alien: do_kalman()
    - :alien: cal_kalman()

- **smartwatch/UART.c**
    - **_UART.h_**
    - :alien: BLEsetup()
    - :raised_hand: uart_tx()
    - :alien: uart_process_control_point_rx()
    - :alien: aci_loop()
