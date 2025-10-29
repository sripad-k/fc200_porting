/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART

  Abstract           : SOC & PL UART interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-215
                                                                SWREQ-233
                       SDD References: 136T-2200-131000-001-D22 SWDES-98
                                                                SWDES-99
                                                                SWDES-100
                                                                SWDES-101
                                                                SWDES-102
                                                                SWDES-103
                                                                SWDES-104
                                                                SWDES-105
                                                                SWDES-106
                                                                SWDES-107
                                                                SWDES-108
                                                                SWDES-109
                                                                SWDES-110
                                                                SWDES-544
                                                                SWDES-592
                                                                SWDES-593
                                                                SWDES-594
                                                                SWDES-595
                                                                SWDES-596
                                                                SWDES-597
                                                                SWDES-598
                                                                SWDES-599
                                                                SWDES-600
                                                                SWDES-601
                                                                SWDES-602
                                                                SWDES-603
\note
  CSC ID             : SWDES-44
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_register.h"
#include "soc/interrupt_manager/d_int_critical.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "soc/timer/d_timer.h"
#include "kernel/error_handler/d_error_handler.h"

#include "xparameters.h"
#include "xuartps.h"

#include "d_uart_ps.h"
#include "d_uart_pl.h"
#include "d_uart_ps_cfg.h"
#include "d_uart_pl_cfg.h"
#include "d_uart.h"

/***** Constants ********************************************************/

#define TEST_PATTERN_SIZE          256u
/* Allow extra length for receiving test pattern to check for extraneous bytes */
#define RX_BUFFER_SIZE             (TEST_PATTERN_SIZE + 1u)

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_UART_Configure -->

  Configure a UART interface.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_UART_Configure
(
const Uint32_t uart,              /**< [in] UART channel */
const Uint32_t baud,              /**< [in] Baudrate */
const d_UART_DataBits_t dataBits, /**< [in] Number of data bits */
const d_UART_Parity_t parity,     /**< [in] Parity selection */
const d_UART_StopBits_t stopBits  /**< [in] Number of stop bits */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (dataBits >= d_UART_DATA_BITS_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, dataBits, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (parity >= d_UART_PARITY_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, parity, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (stopBits >= d_UART_STOP_BITS_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, stopBits, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsConfigure(uart, baud, dataBits, parity, stopBits);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlConfigure(uart - PL_CHANNEL_OFFSET, baud, dataBits, parity, stopBits);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_SetBaudRate -->

  Set the UART baudrate.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_SetBaudRate
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t baud     /**< [in] Baudrate */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsSetBaudRate(uart, baud);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlSetBaudRate(uart - PL_CHANNEL_OFFSET, baud);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_Transmit -->

  Transmit a message.
*************************************************************************/
d_Status_t                     /** \return Success or Failure */
d_UART_Transmit
(
const Uint32_t uart,           /**< [in] UART channel */
const Uint8_t * const buffer,  /**< [in] Pointer to data to transmit */
const Uint32_t length          /**< [in] Number of bytes to transmit */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsTransmit(uart, buffer, length);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlTransmit(uart - PL_CHANNEL_OFFSET, buffer, length);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_Receive -->

  Receive a message.
*************************************************************************/
d_Status_t                    /** \return Success or Failure */
d_UART_Receive
(
const Uint32_t uart,          /**< [in]  UART channel */
Uint8_t * const buffer,       /**< [out] Pointer to buffer for received data */
const Uint32_t length,        /**< [in]  Maximum number of bytes to receive */
Uint32_t * const pBytesRead   /**< [out] Pointer to storage for number of bytes read */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsReceive(uart, buffer, length, pBytesRead);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlReceive(uart - PL_CHANNEL_OFFSET, buffer, length, pBytesRead);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_Discard -->

  Discard characters from the receive buffer.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_Discard
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t length   /**< [in] Number of bytes to discard */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsDiscard(uart, length);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlDiscard(uart - PL_CHANNEL_OFFSET, length);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_FlushRx -->

  Clear receive buffer.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_FlushRx
(
const Uint32_t uart
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsFlushRx(uart);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlFlushRx(uart - PL_CHANNEL_OFFSET);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_FlushTx -->

  Output any remaining characters in transmit buffer without interrupts.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_FlushTx
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsFlushTx(uart);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlFlushTx(uart - PL_CHANNEL_OFFSET);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_Bit -->

  Perform loopback BIT on the UART channel.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_Bit
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart < PL_CHANNEL_OFFSET)
  {
    status = d_UART_PsLoopback(uart, d_TRUE);
  }
  else if (uart < (PL_CHANNEL_OFFSET + d_UART_PL_MAX_INTERFACES))
  {
    status = d_UART_PlLoopback(uart - PL_CHANNEL_OFFSET, d_TRUE);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t index;
    Uint32_t bytesReceived;
    Uint8_t rxBuffer[RX_BUFFER_SIZE];
    static Uint8_t testPattern[TEST_PATTERN_SIZE];

    /* Create test pattern */
    for (index = 0; index < TEST_PATTERN_SIZE; index++)
    {
      testPattern[index] = (Uint8_t)index;
    }

    /* Discard any existing received data */
    (void)d_UART_Discard(uart, 2048u);

    /* Transmit it */
    (void)d_UART_Transmit(uart, &testPattern[0], TEST_PATTERN_SIZE);

    /* Allow time for receipt */
    (void)d_TIMER_DelayMilliseconds(25u);

    /* Receive data */
    (void)d_UART_Receive(uart, &rxBuffer[0], RX_BUFFER_SIZE, &bytesReceived);

    /* Verify */
    if (bytesReceived != TEST_PATTERN_SIZE)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_BIT_FAILURE;
    }
    else
    {
      for (index = 0; index < TEST_PATTERN_SIZE; index++)
      {
        if (rxBuffer[index] != testPattern[index])
        {
          // gcov-jst 1 It is not practical to generate this failure during bench testing.
          status = d_STATUS_BIT_FAILURE;
        }
        ELSE_DO_NOTHING
      }
    }

    /* Discard all data */
    (void)d_UART_Discard(uart, 2048u);

    if (uart < PL_CHANNEL_OFFSET)
    {
      (void)d_UART_PsLoopback(uart, d_FALSE);
    }
    else
    {
      (void)d_UART_PlLoopback(uart - PL_CHANNEL_OFFSET, d_FALSE);
    }
  }
  ELSE_DO_NOTHING

  return status;
}
