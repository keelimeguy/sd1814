## Glucose Monitoring Smart Watch - UConn Senior Design Team 1814

### TODO:
- Implement measurement_controller (communication with implant and data interpretation)
- Implement interrupt-driven bluetooth
- Make battery ADC read into interrupt and nonblocking
- Test and verify each component of code

### Notes:
- Board configuration/pinout: "src/config/conf_board.h"
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
    - :raised_hand: main()

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
    - :x: battery_reader_init()
    - :x: battery_reader_task()
    - :x: get_battery_level()

- **smartwatch/bluetooth_driver.c**
    - **_bluetooth\_driver.h_**
    - :raised_hand: bluetooth_driver_init()
    - :alien: is_bt_active_soft()
    - :alien: is_bt_active()
    - :raised_hand: bt_task()
    - :alien: bt_write()
    - :alien: bt_amt_notifications()
    - :alien: bt_clear_amt_notifications()
    - :alien: bt_get_notification_1()
    - :alien: bt_get_notification_2()
    - :alien: bt_connection_state()
    - :alien: bt_set_connection_state()
    - :alien: set_ble_rx_buffer()
    - :raised_hand: bt_read_callback()
    - :alien: bt_write_callback()

- **smartwatch/button_listener.c**
    - **_button\_listener.h_**
    - :raised_hand: button_listener_init()
    - :alien: button_listener_callback_L()
    - :alien: button_listener_callback_R()
    - :alien: is_button_interrupt_soft()
    - :alien: is_button_interrupt()
    - :alien: get_buttons_soft()
    - :alien: get_buttons()

- **smartwatch/clock_driver.c**
    - **_clock\_driver.h_**
    - **_date\_calc.h_**
    - :raised_hand: clock_driver_init()
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
    - :alien: rtc_alarm_callback()

- **smartwatch/display_manager.c**
    - **_display\_manager.h_**
    - :alien: display_manager_init()
    - :alien: display_ui_task()
    - :raised_hand: updateMainDisplay()
    - :raised_hand: viewNotifications()
    - :raised_hand: showGraphView()
    - :alien: updateGraph()
    - :alien: updateDateDisplay()
    - :alien: updateGlucoseDisplay()
    - :raised_hand: updateTimeDisplay()
    - :raised_hand: updateBLEstatusDisplay()
    - :x: displayBattery()

- **smartwatch/kalman.c**
    - **_kalman.h_**
    - :alien: setPointer()
    - :alien: kalman_setT()
    - :raised_hand: kalman_CGM()

- **smartwatch/measurement_controller.c**
    - **_measurement\_controller.h_**
    - :x: measurement_controller_init()
    - :alien: take_measurement()
    - :alien: is_measure_busy()
    - :alien: measure_set_reading_timeout()
    - :alien: measure_get_reading_timeout()
    - :alien: measure_set_pulse_one()
    - :alien: measure_set_pulse_two()
    - :alien: measure_set_pulse_three()
    - :x: measurement_task()
    - :alien: get_measurement()
    - :alien: is_new_measurement_soft()
    - :alien: is_new_measurement()
    - :alien: do_kalman_bt_cmd()
    - :alien: do_kalman()
    - :alien: cal_kalman()

- **smartwatch/UART.c**
    - **_UART.h_**
    - :raised_hand: BLEsetup()
    - :raised_hand: uart_tx()
    - :raised_hand: uart_process_control_point_rx()
    - :raised_hand: aci_loop()
