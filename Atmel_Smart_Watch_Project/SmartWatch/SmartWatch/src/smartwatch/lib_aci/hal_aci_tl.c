/* UConn Senior Design Team 1814, December 2017
     Using code adapted from:

/* Copyright (c) 2014, Nordic Semiconductor ASA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** @file
@brief Implementation of the ACI transport layer module
*/

#include <compiler.h>
#include <port.h>
#include <spi.h>
#include <delay.h>

#include "hal_platform.h"
#include "hal_aci_tl.h"
#include "aci_queue.h"
/*
PIC32 supports only MSbit transfer on SPI and the nRF8001 uses LSBit
Use the REVERSE_BITS macro to convert from MSBit to LSBit
The outgoing command and the incoming event needs to be converted
*/
//Board dependent defines
// #if defined (__AVR__)
//     //For Arduino add nothing
// #elif defined(ARDUINO_ARCH_SAMD)
//     //For Arduino add nothing
// #elif defined(__PIC32MX__)
//     //For ChipKit as the transmission has to be reversed, the next definitions have to be added
//     #define REVERSE_BITS(byte) (((reverse_lookup[(byte & 0x0F)]) << 4) + reverse_lookup[((byte & 0xF0) >> 4)])
//     static const uint8_t reverse_lookup[] = { 0, 8,  4, 12, 2, 10, 6, 14,1, 9, 5, 13,3, 11, 7, 15 };
// #endif

#define digitalWrite port_pin_set_output_level
#define digitalRead port_pin_get_input_level

static void m_aci_data_print(hal_aci_data_t *p_data);
static void m_aci_event_check(void);
static void m_aci_isr(void);
static void m_aci_pins_set(aci_pins_t *a_pins_ptr);
static inline void m_aci_reqn_disable (void);
static inline void m_aci_reqn_enable (void);
static void m_aci_q_flush(void);

static uint8_t        spi_readwrite(uint8_t aci_byte);

struct spi_module bt_master;
struct spi_slave_inst bt_slave;

#define aci_debug_print false

aci_queue_t    aci_tx_q;
aci_queue_t    aci_rx_q;

static aci_pins_t	 *a_pins_local_ptr;

// void m_aci_data_print(hal_aci_data_t *p_data)
// {
//   const uint8_t length = p_data->buffer[0];
//   uint8_t i;
//   Serial.print(length, DEC);
//   Serial.print(" :");
//   for (i=0; i<=length; i++)
//   {
//     Serial.print(p_data->buffer[i], HEX);
//     Serial.print(F(", "));
//   }
//   Serial.println(F(""));
// }

void* get_aci_rx_q_ptr() {
	return (void*)&aci_rx_q;
}

/*
  Interrupt service routine called when the RDYN line goes low. Runs the SPI transfer.
*/
static void m_aci_isr(void)
{
  hal_aci_data_t data_to_send;
  hal_aci_data_t received_data;

  // Receive from queue
  if (!aci_queue_dequeue_from_isr(&aci_tx_q, &data_to_send))
  {
    /* queue was empty, nothing to send */
    data_to_send.status_byte = 0;
    data_to_send.buffer[0] = 0;
  }

  // Receive and/or transmit data
  m_aci_spi_transfer(&data_to_send, &received_data);

  if (!aci_queue_is_full_from_isr(&aci_rx_q) && !aci_queue_is_empty_from_isr(&aci_tx_q))
  {
    m_aci_reqn_enable();
  }

  // Check if we received data
  if (received_data.buffer[0] > 0)
  {
    if (!aci_queue_enqueue_from_isr(&aci_rx_q, &received_data))
    {
      /* Receive Buffer full.
         Should never happen.
         Spin in a while loop.
      */
      while(1);
    }

    // Disable ready line interrupt until we have room to store incoming messages
//     if (aci_queue_is_full_from_isr(&aci_rx_q))
//     {
//       detachInterrupt(a_pins_local_ptr->interrupt_number);
//     }
  }

  return;
}

/*
  Checks the RDYN line and runs the SPI transfer if required.
*/
static void m_aci_event_check(void)
{
  hal_aci_data_t data_to_send;
  hal_aci_data_t received_data;

  // No room to store incoming messages
  if (aci_queue_is_full(&aci_rx_q))
  {
    return;
  }

  // If the ready line is disabled and we have pending messages outgoing we enable the request line
  if (digitalRead(a_pins_local_ptr->rdyn_pin))
  {
    if (!aci_queue_is_empty(&aci_tx_q))
    {
      m_aci_reqn_enable();
    }

    return;
  }

  // Receive from queue
  if (!aci_queue_dequeue(&aci_tx_q, &data_to_send))
  {
    /* queue was empty, nothing to send */
    data_to_send.status_byte = 0;
    data_to_send.buffer[0] = 0;
  }

  // Receive and/or transmit data
  m_aci_spi_transfer(&data_to_send, &received_data);

  /* If there are messages to transmit, and we can store the reply, we request a new transfer */
  if (!aci_queue_is_full(&aci_rx_q) && !aci_queue_is_empty(&aci_tx_q))
  {
    m_aci_reqn_enable();
  }

  // Check if we received data
  if (received_data.buffer[0] > 0)
  {
    if (!aci_queue_enqueue(&aci_rx_q, &received_data))
    {
      /* Receive Buffer full.
         Should never happen.
         Spin in a while loop.
      */
      while(1);
    }
  }

  return;
}

/** @brief Point the low level library at the ACI pins specified
 *  @details
 *  The ACI pins are specified in the application and a pointer is made available for
 *  the low level library to use
 */
static void m_aci_pins_set(aci_pins_t *a_pins_ptr)
{
  a_pins_local_ptr = a_pins_ptr;
}

static inline void m_aci_reqn_disable (void)
{
  digitalWrite(a_pins_local_ptr->reqn_pin, 1);
}

static inline void m_aci_reqn_enable (void)
{
  digitalWrite(a_pins_local_ptr->reqn_pin, 0);
}

static void m_aci_q_flush(void)
{
  noInterrupts();
  /* re-initialize aci cmd queue and aci event queue to flush them*/
  aci_queue_init(&aci_tx_q);
  aci_queue_init(&aci_rx_q);
  interrupts();
}

bool m_aci_spi_transfer(hal_aci_data_t * data_to_send, hal_aci_data_t * received_data)
{
  uint8_t byte_cnt;
  uint8_t byte_sent_cnt;
  uint8_t max_bytes;

  m_aci_reqn_enable();

  // Send length, receive header
  byte_sent_cnt = 0;
  received_data->status_byte = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
  // Send first byte, receive length from slave
  received_data->buffer[0] = spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
  if (0 == data_to_send->buffer[0])
  {
    max_bytes = received_data->buffer[0];
  }
  else
  {
    // Set the maximum to the biggest size. One command byte is already sent
    max_bytes = (received_data->buffer[0] > (data_to_send->buffer[0] - 1))
                                          ? received_data->buffer[0]
                                          : (data_to_send->buffer[0] - 1);
  }

  if (max_bytes > HAL_ACI_MAX_LENGTH)
  {
    max_bytes = HAL_ACI_MAX_LENGTH;
  }

  // Transmit/receive the rest of the packet
  for (byte_cnt = 0; byte_cnt < max_bytes; byte_cnt++)
  {
    received_data->buffer[byte_cnt+1] =  spi_readwrite(data_to_send->buffer[byte_sent_cnt++]);
  }

  // RDYN should follow the REQN line in approx 100ns
  m_aci_reqn_disable();

  return (max_bytes > 0);
}

// void hal_aci_tl_debug_print(bool enable)
// {
// 	//aci_debug_print = enable;
// }

void hal_aci_tl_pin_reset(void)
{
    if (HAL_UNUSED != a_pins_local_ptr->reset_pin)
    {
        if ((REDBEARLAB_SHIELD_V1_1     == a_pins_local_ptr->board_name) ||
            (REDBEARLAB_SHIELD_V2012_07 == a_pins_local_ptr->board_name))
        {
            //The reset for the Redbearlab v1.1 and v2012.07 boards are inverted and has a Power On Reset
            //circuit that takes about 100ms to trigger the reset
            digitalWrite(a_pins_local_ptr->reset_pin, 1);
            delay_cycles_ms(100);
            digitalWrite(a_pins_local_ptr->reset_pin, 0);
        }
        else
        {
            digitalWrite(a_pins_local_ptr->reset_pin, 1);
            digitalWrite(a_pins_local_ptr->reset_pin, 0);
            delay_cycles_ms(10);
            digitalWrite(a_pins_local_ptr->reset_pin, 1);
        }
    }
}

bool hal_aci_tl_event_peek(hal_aci_data_t *p_aci_data)
{
  // if (!a_pins_local_ptr->interface_is_interrupt)
  // {
  //   m_aci_event_check();
  // }

  if (aci_queue_peek(&aci_rx_q, p_aci_data))
  {
    return true;
  }

  return false;
}

bool hal_aci_tl_event_get(hal_aci_data_t *p_aci_data)
{
  bool was_full;

  if (!a_pins_local_ptr->interface_is_interrupt && !aci_queue_is_full(&aci_rx_q))
  {
    m_aci_event_check();
  }

  was_full = aci_queue_is_full(&aci_rx_q);

  if (aci_queue_dequeue(&aci_rx_q, p_aci_data))
  {
//     #if aci_debug_print
//       Serial.print(" E");
//       m_aci_data_print(p_aci_data);
//     #endif

    // if (was_full && a_pins_local_ptr->interface_is_interrupt)
    // {
      /* Enable RDY line interrupt again */
    //   attachInterrupt(a_pins_local_ptr->interrupt_number, m_aci_isr, LOW);
    // }

    /* Attempt to pull REQN LOW since we've made room for new messages */
    if (!aci_queue_is_full(&aci_rx_q) && !aci_queue_is_empty(&aci_tx_q))
    {
      m_aci_reqn_enable();
    }

    return true;
  }

  return false;
}

void hal_aci_tl_init(aci_pins_t *a_pins, bool debug)
{
  /* Needs to be called as the first thing for proper intialization*/
  m_aci_pins_set(a_pins);

  struct spi_config config;
  struct spi_slave_inst_config slave_config;

  spi_slave_inst_get_config_defaults(&slave_config);
  slave_config.ss_pin = a_pins->reqn_pin;
  spi_attach_slave(&bt_slave, &slave_config);

  spi_get_config_defaults(&config);

  config.mux_setting = a_pins->mux_setting;
  config.pinmux_pad0 = a_pins->pinmux_pad0;
  config.pinmux_pad1 = a_pins->pinmux_pad1;
  config.pinmux_pad2 = a_pins->pinmux_pad2;
  config.pinmux_pad3 = a_pins->pinmux_pad3;
  config.mode_specific.master.baudrate = a_pins->baudrate;
  config.data_order = a_pins->dord;
  config.receiver_enable = true;

  spi_init(&bt_master, a_pins->spi, &config);
  spi_enable(&bt_master);

  //Configure the IO lines

  struct port_config pin;
  port_get_config_defaults(&pin);
  pin.direction = PORT_PIN_DIR_OUTPUT;

  port_pin_set_config(a_pins->reset_pin, &pin);
  digitalWrite(a_pins->reset_pin, true);

  port_get_config_defaults(&pin);
  pin.input_pull = PORT_PIN_PULL_UP;

  port_pin_set_config(a_pins->rdyn_pin, &pin);

  /* Initialize the ACI Command queue. This must be called after the delay above. */
  aci_queue_init(&aci_tx_q);
  aci_queue_init(&aci_rx_q);

  /* Pin reset the nRF8001, required when the nRF8001 setup is being changed */
  hal_aci_tl_pin_reset();
  delay_cycles_ms(30); //Wait for the nRF8001 to get hold of its lines - the lines float for a few ms after the reset

  /* Attach the interrupt to the RDYN line as requested by the caller */
  // if (a_pins->interface_is_interrupt)
  // {
  //   // We use the LOW level of the RDYN line as the atmega328 can wakeup from sleep only on LOW
  //   attachInterrupt(a_pins->interrupt_number, m_aci_isr, LOW);
  // }
}

bool hal_aci_tl_send(hal_aci_data_t *p_aci_cmd)
{
  const uint8_t length = p_aci_cmd->buffer[0];
  bool ret_val = false;

  if (length > HAL_ACI_MAX_LENGTH)
  {
    return false;
  }

  ret_val = aci_queue_enqueue(&aci_tx_q, p_aci_cmd);
  if (ret_val)
  {
    if(!aci_queue_is_full(&aci_rx_q))
    {
      // Lower the REQN only when successfully enqueued
      m_aci_reqn_enable();
    }

//     #if aci_debug_print
//       Serial.print("C"); //ACI Command
//       m_aci_data_print(p_aci_cmd);
//     #endif
  }

  return ret_val;
}

static uint8_t spi_readwrite(const uint8_t aci_byte)
{
	uint16_t ret;
	spi_transceive_wait(&bt_master, &aci_byte, &ret);
	return (uint8_t)ret;
}

bool hal_aci_tl_rx_q_empty (void)
{
  return aci_queue_is_empty(&aci_rx_q);
}

bool hal_aci_tl_rx_q_full (void)
{
  return aci_queue_is_full(&aci_rx_q);
}

bool hal_aci_tl_tx_q_empty (void)
{
  return aci_queue_is_empty(&aci_tx_q);
}

bool hal_aci_tl_tx_q_full (void)
{
  return aci_queue_is_full(&aci_tx_q);
}

void hal_aci_tl_q_flush (void)
{
  m_aci_q_flush();
}
