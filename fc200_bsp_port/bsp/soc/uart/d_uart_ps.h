/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART_PS Header File

  Abstract           : SOC UART PS interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-233
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
\note
  CSC ID             : SWDES-44
*************************************************************************/

#ifndef D_UART_PS_H
#define D_UART_PS_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/uart/d_uart.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Configure a UART interface */
d_Status_t d_UART_PsConfigure(const Uint32_t uart,
                              const Uint32_t baud,
                              const d_UART_DataBits_t dataBits,
                              const d_UART_Parity_t parity,
                              const d_UART_StopBits_t stopBits);

/* Set the UART baudrate */
d_Status_t d_UART_PsSetBaudRate(const Uint32_t uart,
                                const Uint32_t baud);

/* Transmit a message */
d_Status_t d_UART_PsTransmit(const Uint32_t uart,
                             const Uint8_t * const buffer,
                             const Uint32_t length);

/* Receive a message */
d_Status_t d_UART_PsReceive(const Uint32_t uart,
                            Uint8_t * const buffer,
                            const Uint32_t length,
                            Uint32_t * const pBytesRead);

/* Discard characters from the receive buffer */
d_Status_t d_UART_PsDiscard(const Uint32_t uart,
                            const Uint32_t length);

/* Clear receive buffer */
d_Status_t d_UART_PsFlushRx(const Uint32_t uart);

/* Output any remaining characters in transmit buffer without interrupts */
d_Status_t d_UART_PsFlushTx(const Uint32_t uart);

/* Enable or disable loopback */
d_Status_t d_UART_PsLoopback(const Uint32_t uart,
                             const Bool_t enable);

/* Interrupt handlers */
void d_UART_PsInterruptHandler(const Uint32_t uart);

#endif /* D_UART_PS_H */
