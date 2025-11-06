#include "uart_interface.h"

#define CONSOLE_UART 0

void _putchar(char ch)
{
    uart_write(CONSOLE_UART, (uint8_t *)&ch, 1);
}