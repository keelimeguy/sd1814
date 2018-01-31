/* UConn Senior Design Team 1814, December 2017
     Using code adapted from:

/* Copyright (c) 2014, Nordic Semiconductor ASA

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/


/** @defgroup ble_uart_project_template ble_uart_project_template
  @{
  @ingroup projects
  @brief Empty project that can be used as a template for new projects.

  @details
  This project is a firmware template for new projects.
  The project will run correctly in its current state.
  It can send data on the UART TX characteristic
  It can receive data on the UART RX characterisitc.
  With this project you have a starting point for adding your own application functionality.

  The following instructions describe the steps to be made on the Windows PC:

  -# Install the Master Control Panel on your computer. Connect the Master Emulator
    (nRF2739) and make sure the hardware drivers are installed.

  -# You can use the nRF UART app in the Apple iOS app store and Google Play for Android 4.3 for Samsung Galaxy S4
   with this UART template app

  -# You can send data from the Arduino serial monitor, maximum length of a string is 19 bytes
   Set the line ending to "Newline" in the Serial monitor (The newline is also sent over the air


   Click on the "Serial Monitor" button on the Arduino IDE to reset the Arduino and start the application.
   The setup() function is called first and is called only once for each reset of the Arduino.
   The loop() function as the name implies is called in a loop.

   The setup() and loop() function are called in this way.
   main()
    {
     setup();
     while(1)
     {
       loop();
     }
   }

  sendLength */

#include "conf_bluetooth.h"
#include "bluetooth_driver.h"

#include "lib_aci/lib_aci.h"
#include "lib_aci/aci_setup.h"
#include "lib_aci/uart_over_ble.h"
#include "lib_aci/services.h"

#define BLE_DEBUG false

void __ble_assert(const char *file, uint16_t line);
void BLEsetup(void);
void uart_over_ble_init(void);
bool uart_tx(uint8_t *buffer, uint8_t buffer_len);
bool uart_process_control_point_rx(uint8_t *byte, uint8_t length);
void aci_loop();
