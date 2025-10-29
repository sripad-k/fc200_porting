/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : IRQ Split Handler

  Abstract : Handles interrupt indication source with flag word.

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

#ifndef D_INT_IRQ_SPLIT_CFG_H
#define D_INT_IRQ_SPLIT_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "d_int_irq_table.h"

/***** Literals *********************************************************/

#define d_INT_IRQ_SPLIT_MAX_ENTRIES   8u
#define d_INT_IRQ_SPLIT_MAX_FLAGS    16u

#define d_INT_IRQ_SPLIT_COUNT const Uint32_t d_INT_IrqSplitCount = (sizeof(d_INT_IrqSplit) / sizeof(d_INT_IrqSplit_t))

/***** Type Definitions *************************************************/

typedef struct
{
  d_INT_IrqHandler_t pHandler;
  Uint32_t parameter;
} handlerEntry_t;

typedef struct
{
  Uint32_t flagAddress;
  handlerEntry_t handlerEntry[d_INT_IRQ_SPLIT_MAX_FLAGS];
} d_INT_IrqSplit_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_INT_IrqSplit_t d_INT_IrqSplit[];

extern const Uint32_t d_INT_IrqSplitCount;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_INT_IRQ_SPLIT_CFG_H */
