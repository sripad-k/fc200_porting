/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : DMA CFG

  Abstract           : SOC DMA configuration.

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

#ifndef D_DMA_CFG_H
#define D_DMA_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "xparameters.h"

/***** Literals *********************************************************/

/***** Type Definitions *************************************************/

/* Callback type for Completion of all data transfers */
typedef void (*d_DMA_DoneHandler) (const Uint32_t channel);

/* Callback type for all error interrupts */
typedef void (*d_DMA_ErrorHandler) (const Uint32_t channel, const Uint32_t ErrorMask);

typedef struct
{
  d_DMA_DoneHandler doneHandler;
  d_DMA_ErrorHandler errorHandler;
} d_DMA_Config_t;

/***** Constants ********************************************************/

extern const d_DMA_Config_t d_DMA_Config[XPAR_XZDMA_NUM_INSTANCES];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_DMA_CFG_H */
