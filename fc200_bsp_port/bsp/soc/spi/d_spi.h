/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_spi.h

  Abstract           : SPI Interface

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-246
                       SDD References: 136T-2200-131100-001-D22 SWDES-111
                                                                SWDES-112
                                                                SWDES-113
                                                                SWDES-114
                                                                SWDES-115
\note
CSC_ID             : SWDES-45
*************************************************************************/

#ifndef D_SPI_H
#define D_SPI_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

typedef enum
{
  d_SPI_STATUS_INIT = 0,
  d_SPI_STATUS_READY,
  d_SPI_STATUS_BUSY,
  d_SPI_STATUS_COMPLETE
} d_SPI_State_t;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

d_Status_t d_SPI_Initialise(const Uint32_t channel);

d_Status_t d_SPI_SetBaudrate(const Uint32_t channel,
                             const Uint32_t baud);

d_Status_t d_SPI_SetDelays(const Uint32_t channel,
                           const Uint32_t delayNss,
                           const Uint32_t delayBtwn,
                           const Uint32_t delayAfter,
                           const Uint32_t delayInit);

d_Status_t d_SPI_Transfer(const Uint32_t channel,
                          const Uint32_t device,
                          const Uint8_t * const cmdBuffer,
                          const Uint32_t cmdCount,
                          const Uint8_t * const txBuffer,
                          const Uint32_t dataCount);

d_Status_t d_SPI_Result(const Uint32_t channel,
                        d_SPI_State_t * const pStatus,
                        Uint8_t * const rxBuffer,
                        const Uint32_t bufferLength,
                        Uint32_t * pCount);

d_Status_t d_SPI_Polled(const Uint32_t channel,
                        const Uint32_t device,
                        const Uint8_t * const cmdBuffer,
                        const Uint32_t cmdCount,
                        const Uint8_t * const txBuffer,
                        const Uint32_t dataCount,
                        Uint8_t * const rxBuffer);


/* Get interface status */
d_Status_t d_SPI_Status(const Uint32_t channel,
                        d_SPI_State_t * const pStatus);

d_Status_t d_SPI_SelfTest(const Uint32_t channel);

/* Interrupt handler */
void d_SPI_InterruptHandler(const Uint32_t channel);

#endif /* D_SPI_H */
