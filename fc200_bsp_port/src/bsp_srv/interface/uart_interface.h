/****************************************************
 *  uart_interface.h
 *  Created on: 10-Jul-2025 11:49:39 AM
 *  Implementation of the Interface uart_interface
 *  Copyright: LODD (c) 2025
 ****************************************************/
#ifndef H_UART_INTERFACE
#define H_UART_INTERFACE

#include "type.h"

#include "uart_peripherals_t.h" /* Include for 'relation to classifier' 'UART_peripherals_t' */

/* The function initializes the VALID UART peripherals that are passed as argument */
bool uart_init(UART_peripherals_t uart_channel);

/* Implementation of operation 'uart_read' from interface 'UART_interface' */
uint16_t uart_read(UART_peripherals_t uart_channel, uint8_t* ptr_rx_data, uint16_t max_buf_data_size);

/* Implementation of operation 'uart_write' from interface 'UART_interface' */
uint16_t uart_write(UART_peripherals_t uart_channel, uint8_t *ptr_tx_data, uint16_t max_buf_data_size);

#endif /* H_UART_INTERFACE */
