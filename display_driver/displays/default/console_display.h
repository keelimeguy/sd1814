/* UConn Senior Design Team 1814, November 2017
*/

#ifndef CONSOLE_DISPLAY_H
#define CONSOLE_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define CONSOLE_DISPLAY_WIDTH 100
#define CONSOLE_DISPLAY_HEIGHT 100

#define Abs(a) (((a) <  0 ) ? -(a) : (a))
#define PROGMEM

static inline void console_display_on(void) {
}

static inline void console_display_hard_reset(void) {
}

static inline void console_display_sleep_enable(void) {
}

static inline void console_display_sleep_disable(void) {
}

static inline void console_display_start_write(void) {
}

void console_display_set_row_address(uint8_t start_address, uint8_t end_address);
void console_display_set_column_address(uint8_t start_address, uint8_t end_address);
void console_display_write_command(uint8_t command);
void console_display_write_data(uint8_t data);
void console_display_init(void);
void console_display_end_write(void);

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_DISPLAY_H */
