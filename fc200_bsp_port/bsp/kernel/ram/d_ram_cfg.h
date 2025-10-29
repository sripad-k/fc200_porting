/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM 

  Abstract           : Memory test functionality configuration items

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-237
                       SDD References: 136T-2200-131100-001-D22 SWDES-656
                                                                SWDES-662
\note
  CSC ID             : SWDES-74
*************************************************************************/

#ifndef D_RAM_CFG_H
#define D_RAM_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/* All times in milliseconds */
extern const Uint32_t d_RAM_CyclicExecutionTime;
extern const Uint32_t d_RAM_CodeCheckTime;
extern const Uint32_t d_RAM_StackCheckTime;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_RAM_CFG_H */
