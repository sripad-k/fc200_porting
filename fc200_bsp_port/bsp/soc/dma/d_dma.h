/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : DMA

  Abstract           : DMA transfers

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-146
                       SDD References: 136T-2200-131000-001-D22 SWDES-120
                                                                SWDES-121
                                                                SWDES-122
                                                                SWDES-123
                                                                SWDES-124
                                                                SWDES-125
                                                                SWDES-126
                                                                SWDES-127
                                                                SWDES-128
\note
  CSC ID             : SWDES-49
*************************************************************************/

#ifndef DMA_H
#define DMA_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum
{
  d_DMA_STATE_IDLE,    /**< Idle state */
  d_DMA_STATE_PAUSE,   /**< Paused state */
  d_DMA_STATE_BUSY     /**< Busy state */
} d_DMA_State_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the DMA driver */
d_Status_t d_DMA_Initialise(const Uint32_t channel);

/* Perform check of DMA register access */
d_Status_t d_DMA_SelfTest(const Uint32_t channel);

/* Test if DMA transfer is complete, function is non-blocking */
d_DMA_State_t d_DMA_ChannelState(const Uint32_t channel);

/* Start a DMA PS to PS memory transfer */
d_Status_t d_DMA_StartMemToMemTransfer(const Uint32_t channel, const Uint8_t * const destination, const Uint8_t * const source, const Uint32_t byteCount);

/* Enable DMA Interrupt for channel */
d_Status_t d_DMA_InterruptEnable(Uint32_t dmaChannel);

/* Disable DMA Interrupt for channel */
d_Status_t d_DMA_InterruptDisable(Uint32_t dmaChannel);

/* Interrupt handlers */
void d_DMA_InterruptHandler(const Uint32_t channel);

#endif /* DMA_H */
