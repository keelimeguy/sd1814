## Glucose Monitoring Smart Watch - UConn Senior Design Team 1814

### TODO:
- Complete board pinout configuration
- Implement bluetooth_driver (UART communication with BT chip and protocol)
- Implement button_listener (extint)
- Implement clock_driver (RTC/alarm and timeout timers)
- Implement display_manager (smartwatch GUI)
- Implement measurement_controller (communication with implant and data interpretation)

### Notes:
- Board configuration/pinout: "src/config/conf_board.h"
- Warning levels for glucose defined in "src/smartwatch/display\_driver/display_driver.h"
- Application starts at "src/main.c"
