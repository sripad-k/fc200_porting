/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Memory test configuration

  Abstract           : This is a the memory test definition.

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-237
                       SDD References: 136T-2200-131100-001-D22 SWDES-656
                                                                SWDES-662
\note
  CSC ID             : SWDES-74
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "kernel/ram/d_ram_cfg.h"

/***** Constants ********************************************************/

/* All times in milliseconds */

/* Time between invocations of the d_RAM_Cyclic function */
__attribute__((weak)) const Uint32_t d_RAM_CyclicExecutionTime = 20;

/* Time in which the code CRRC check must be completed */
__attribute__((weak)) const Uint32_t d_RAM_CodeCheckTime = 60000;

/* Time in which the stack usage check must be completed */
__attribute__((weak)) const Uint32_t d_RAM_StackCheckTime = 10000;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

