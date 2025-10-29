/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_apu.c

  Abstract           : Start the APU processor core 0.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-
                       SDD References: 136T-2200-131100-001-D22 SWDES-

\note
  CSC ID             : SWDES-
*************************************************************************/

#ifndef APU_H
#define APU_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Start the APU */
d_Status_t d_APU_Start(const Uint32_t address);

#endif /* APU_H */
