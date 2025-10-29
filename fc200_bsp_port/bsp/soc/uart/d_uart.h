/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART Header File

  Abstract           : SOC UART interface.

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

#ifndef D_UART_H
#define D_UART_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum
{
  d_UART_DATA_BITS_7 = 0,
  d_UART_DATA_BITS_8,

  d_UART_DATA_BITS_COUNT
} d_UART_DataBits_t;

typedef enum
{
  d_UART_PARITY_NONE = 0,
  d_UART_PARITY_ODD,
  d_UART_PARITY_EVEN,

  d_UART_PARITY_COUNT
} d_UART_Parity_t;

typedef enum
{
  d_UART_STOP_BITS_1 = 0,
  d_UART_STOP_BITS_2,
  
  d_UART_STOP_BITS_COUNT
} d_UART_StopBits_t;
  
/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Configure a UART interface */
d_Status_t d_UART_Configure(const Uint32_t uart,
                            const Uint32_t baud,
                            const d_UART_DataBits_t dataBits,
                            const d_UART_Parity_t parity,
                            const d_UART_StopBits_t stopBits);

/* Set the UART baudrate */
d_Status_t d_UART_SetBaudRate(const Uint32_t uart,
                              const Uint32_t baud);

/* Transmit a message */
d_Status_t d_UART_Transmit(const Uint32_t uart,
                           const Uint8_t * const buffer,
                           const Uint32_t length);

/* Receive a message */
d_Status_t d_UART_Receive(const Uint32_t uart,
                          Uint8_t * const buffer,
                          const Uint32_t length,
                          Uint32_t * const pBytesRead);

/* Discard characters from the receive buffer */
d_Status_t d_UART_Discard(const Uint32_t uart,
                          const Uint32_t length);

/* Clear receive buffer */
d_Status_t d_UART_FlushRx(const Uint32_t uart);

/* Output any remaining characters in transmit buffer without interrupts */
d_Status_t d_UART_FlushTx(const Uint32_t uart);

/* Perform loopback BIT on the UART channel */
d_Status_t d_UART_Bit(const Uint32_t uart);

#endif /* D_UART_H */
