/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_spi_pl

  Abstract           : SPI PL Interface

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-758
                       SDD References: 136T-2200-131100-001-D22 SWDES-2427
                                                                SWDES-2428
                                                                SWDES-2429
                                                                SWDES-2430
                                                                SWDES-2431
                                                                SWDES-2432
\note
  CSC ID             : SWDES-2422
*************************************************************************/

#ifndef D_SPI_PL_H
#define D_SPI_PL_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

typedef enum
{
  d_SPI_PL_STATUS_INIT = 0,
  d_SPI_PL_STATUS_READY,
  d_SPI_PL_STATUS_BUSY,
  d_SPI_PL_STATUS_COMPLETE
} d_SPI_PL_State_t;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise SPI channel, set to IDLE */
d_Status_t d_SPI_PL_Initialise(const Uint32_t channel);

/* Clear the receive FIFO */
d_Status_t d_SPI_PL_Reset(const Uint32_t channel);

/* Perform an SPI data transfer */
d_Status_t d_SPI_PL_Transfer(const Uint32_t channel,
                             const Uint32_t device,
                             const Uint8_t * const cmdBuffer,
                             const Uint32_t cmdCount,
                             const Uint8_t * const txBuffer,
                             const Uint32_t dataCount);

/* Get result of transfer request */
d_Status_t d_SPI_PL_Result(const Uint32_t channel,
                           d_SPI_PL_State_t * const pStatus,
                           Uint8_t * const rxBuffer,
                           const Uint32_t bufferLength,
                           Uint32_t * pCount);

/* Get interface status */
d_Status_t d_SPI_PL_Status(const Uint32_t channel,
                           d_SPI_PL_State_t * const pStatus);

/* Interrupt handler */
void d_SPI_PL_InterruptHandler(const Uint32_t channel);

#endif /* D_SPI_PL_H */
