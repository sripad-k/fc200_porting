/**
 * @file printf_support.c
 * @brief Platform-specific printf support for FC200 ARM bare-metal environment
 * 
 * This file implements the low-level character output function (_putchar) that
 * is used by the printf implementation to output characters to the UART debug console.
 * 
 * The printf implementation (printf.c) calls _putchar() for each character that
 * needs to be output. This function sends the character to the UART hardware.
 * 
 * Configuration:
 * - CONSOLE_UART is set to UART_DEBUG_CONSOLE (channel 0)
 * - UART must be initialized with uart_init(UART_DEBUG_CONSOLE) before using printf
 * - Default UART settings: 115200 baud, 8N1 (configured in uart_ps_cfg.c or uart_pl_cfg.c)
 * 
 * @see printf.h for printf function declarations
 * @see printf.c for printf implementation
 * @see uart_interface.h for UART interface functions
 */

#include "uart_interface.h"

// UART channel used for printf debug output
// Maps to UART_DEBUG_CONSOLE from uart_peripherals_t enum
#define CONSOLE_UART 0

/**
 * @brief Output a single character to the debug UART
 * 
 * This function is called by printf() to output each character.
 * It writes the character to the UART debug console using the uart_write() interface.
 * 
 * @param ch Character to output to the debug console
 * 
 * @note This function is blocking and will wait until the character is transmitted
 * @note Make sure to call uart_init(UART_DEBUG_CONSOLE) before using printf
 */
void _putchar(char ch)
{
    uart_write(CONSOLE_UART, (uint8_t *)&ch, 1);
}