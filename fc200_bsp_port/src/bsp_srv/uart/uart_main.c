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
    uint32_t uart_ch;
    uint32_t baud_rate;
    d_UART_DataBits_t data_bits;
    d_UART_Parity_t parity;
    d_UART_StopBits_t stop_bits;
} uart_config_t;

static const uart_config_t UART_Config[UART_MAX_PERIPHERAL] = {

    {0,  115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}, /* DEBUG_CONSOLE */
    {14,  115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}, /* RADALT */
    {20,  100000,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}, /* SBUS */
    {26,  460800,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}, /* INS */
    {22,  115200,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}, /* SERVO */
    {5,  500000,  d_UART_DATA_BITS_8,  d_UART_PARITY_NONE,  d_UART_STOP_BITS_1}  /* ADS */
};

/**
 * @brief Initializes a UART peripheral with specified configuration parameters
 * 
 * This function configures a UART channel using the parameters stored in the
 * UART_Config array for the specified peripheral. It sets up baud rate, data bits,
 * parity, and stop bits according to the configuration.
 * 
 * @param uart_ch The UART peripheral identifier specifying which UART channel to initialize
 * 
 * @return true if UART initialization was successful
 * @return false if UART initialization failed
 * 
 * @note The function relies on the global UART_Config array to contain valid
 *       configuration parameters for the specified UART channel
 * 
 * @see d_UART_Configure() for the underlying UART configuration function
 * @see UART_peripherals_t for valid UART channel identifiers
 */
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

/**
 * @brief Reads data from the specified UART peripheral
 * 
 * This function attempts to receive data from the given UART channel and stores
 * it in the provided buffer. The function validates input parameters before
 * attempting to read data.
 * 
 * @param uart_ch The UART peripheral channel to read from (must be < UART_MAX_PERIPHERAL)
 * @param ptr_rx_data Pointer to the buffer where received data will be stored (must not be NULL)
 * @param max_buf_data_size Maximum number of bytes that can be stored in the receive buffer
 * 
 * @return uint16_t Number of bytes actually read from the UART peripheral
 *                  Returns 0 if invalid parameters are provided or if no data is available
 * 
 * @note This function performs parameter validation before attempting to read data
 * @note The actual number of bytes read may be less than max_buf_data_size
 */
uint16_t uart_read(UART_peripherals_t uart_channel, uint8_t* ptr_rx_data, uint16_t max_buf_data_size)
{
    uint32_t bytes_read = 0;
    uint16_t retval = 0; 
    d_Status_t status = d_STATUS_SUCCESS;
    /* If pointers passed are not equal to NULL and the given uart channel is in range */
    if ((ptr_rx_data != NULL) && (uart_channel < UART_MAX_PERIPHERAL))
    {
        /* Call BSP function to receive the data */
        status = d_UART_Receive((Uint32_t)UART_Config[uart_channel].uart_ch, ptr_rx_data, max_buf_data_size, &bytes_read);
        
        /* Check if the transmission was successful */
        if(status != d_STATUS_SUCCESS)
        {
            /* Update Bytes Written upon successful transmission */
            bytes_read = 0;
        }
    }

    if(bytes_read <= UINT16_MAX)
    {
        retval = (uint16_t)bytes_read;
    }
    else
    {
        retval = UINT16_MAX;
    }

	return retval;  //TBD: The prototype can be changed to return uint32_t
}

/**
 * @brief Writes data to the specified UART channel
 * 
 * This function transmits data through the specified UART peripheral channel.
 * It validates the input parameters and returns the number of bytes successfully written.
 * 
 * @param uart_channel The UART peripheral channel to write to (must be < UART_MAX_PERIPHERAL)
 * @param ptr_tx_data Pointer to the data buffer to transmit (must not be NULL)
 * @param max_buf_data_size Maximum number of bytes to transmit from the buffer
 * 
 * @return uint16_t Number of bytes successfully written
 *         - Returns max_buf_data_size on successful transmission
 *         - Returns 0 if transmission fails or invalid parameters are provided
 * 
 * @note The function performs parameter validation before attempting transmission
 * @note If either ptr_tx_data is NULL or uart_channel is out of range, transmission is skipped
 */
uint16_t uart_write(UART_peripherals_t uart_channel, uint8_t* ptr_tx_data, uint16_t max_buf_data_size)
{

    d_Status_t status = d_STATUS_SUCCESS;
    uint16_t bytes_written = 0;

    /* If pointers passed are not equal to NULL and the given uart channel is in range */
    if ((ptr_tx_data != NULL) && (uart_channel < UART_MAX_PERIPHERAL))
    {
        /* Call BSP function to transmit data */
        status = d_UART_Transmit((Uint32_t)UART_Config[uart_channel].uart_ch, ptr_tx_data, (Uint32_t)max_buf_data_size);

        /* Check if the transmission was successful */
        if(status == d_STATUS_SUCCESS)
        {
            /* Update Bytes Written upon successful transmission */
            bytes_written = max_buf_data_size;
        }
    }

	return (bytes_written);
}

