/****************************************************
 *  uart_main.c                                         
 *  Created on: 25-Aug-2025 10:22:24 AM                      
 *  Implementation of the Class uart_main       
 *  Copyright: LODD (c) 2025                     
 ****************************************************/

#include "uart_main.h"
#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"
#include "soc/uart/d_uart.h"

typedef struct
{
    /* data */
    UART_peripherals_t uart_ch;
    uint32_t baud_rate;
    uint8_t data_bits;
    char parity;
    uint8_t stop_bits;
} uart_config_t;

static const uart_config_t UART_Config[UART_MAX_PERIPHERAL] = {

    {UART_DEBUG_CONSOLE,  115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1},
    {UART_RADALT,         115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1},
    {UART_SBUS,           115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1},
    {UART_INS,            115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1},
    {UART_SERVO,          115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1},
    {UART_ADS,            115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}
};



bool uart_init(UART_peripherals_t uart_ch)
{
    d_Status_t debugUart = d_STATUS_SUCCESS;
    bool status = true;
	debugUart = d_UART_Configure(UART_Config[uart_ch].uart_ch,
                        UART_Config[uart_ch].baud_rate,
                        UART_Config[uart_ch].data_bits,
                        UART_Config[uart_ch].parity,
                        UART_Config[uart_ch].stop_bits);

    if (debugUart != d_STATUS_SUCCESS)
    {
        status = false;
    }
    return status;
}

uint16_t uart_read(UART_peripherals_t uart_ch, uint8_t* ptr_rx_data, uint16_t max_buf_data_size)
{
	return  0;
}

uint16_t uart_write(UART_peripherals_t uart_channel, uint8_t* ptr_tx_data, uint16_t max_buf_data_size)
{
	return  0;
} 

