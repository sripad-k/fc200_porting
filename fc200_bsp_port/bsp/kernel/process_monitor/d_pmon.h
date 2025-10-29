/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Process Monitor

  Abstract : Diagnostic reading of any memory location.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-189
                       SDD References: 136T-2200-131100-001-D22 SWDES-2442
                                                                SWDES-2443
                                                                SWDES-2444
                                                                SWDES-2444
\note
  CSC ID             : SWDES-2421
*************************************************************************/

#ifndef D_PMON_H_
#define D_PMON_H_

/***** Includes *********************************************************/

#include "soc/defines/d_common_status.h"

/***** Literals ********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise process monitoring. Should only be called after d_ETH_Initialise has returned */
d_Status_t d_PMON_Initialise(void);

/* Cyclic process monitor function */
void d_PMON_Cyclic(void);

#endif /* D_PMON_H_ */
