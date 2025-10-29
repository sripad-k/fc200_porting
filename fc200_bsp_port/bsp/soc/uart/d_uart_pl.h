/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART_PL Header File

  Abstract           : SOC UART PL interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-215
                       SDD References: 136T-2200-131000-001-D22 SWDES-544
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
  CSC ID             : SWDES-57
*************************************************************************/

#ifndef D_UART_PL_H
#define D_UART_PL_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/uart/d_uart.h"

/***** Literals *********************************************************/

#define PL_CHANNEL_OFFSET   2u

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Configure a UART interface */
d_Status_t d_UART_PlConfigure(const Uint32_t uart,
                              const Uint32_t baud,
                              const d_UART_DataBits_t dataBits,
                              const d_UART_Parity_t parity,
                              const d_UART_StopBits_t stopBits);

/* Set the UART baudrate */
d_Status_t d_UART_PlSetBaudRate(const Uint32_t uart,
                                const Uint32_t baud);

/* Transmit a message */
d_Status_t d_UART_PlTransmit(const Uint32_t uart,
                             const Uint8_t * const buffer,
                             const Uint32_t length);

/* Receive a message */
d_Status_t d_UART_PlReceive(const Uint32_t uart,
                            Uint8_t * const buffer,
                            const Uint32_t length,
                            Uint32_t * const pBytesRead);

/* Discard characters from the receive buffer */
d_Status_t d_UART_PlDiscard(const Uint32_t uart,
                            const Uint32_t length);

/* Clear receive buffer */
d_Status_t d_UART_PlFlushRx(const Uint32_t uart);

/* Output any remaining characters in transmit buffer without interrupts */
d_Status_t d_UART_PlFlushTx(const Uint32_t uart);

/* Enable or disable loopback */
d_Status_t d_UART_PlLoopback(const Uint32_t uart,
                             const Bool_t enable);

/* Interrupt handlers */
void d_UART_PlInterruptHandler(const Uint32_t channel);

#endif /* D_UART_PL_H */
