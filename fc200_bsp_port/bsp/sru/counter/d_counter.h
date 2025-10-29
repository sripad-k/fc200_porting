/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Counter

  Abstract           : Counter interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-
                       SDD References: 136T-2200-131000-001-D22 SWDES-
\note
  CSC ID             : SWDES-
*************************************************************************/

#ifndef D_COUNTER_H
#define D_COUNTER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the watchdog interface */
d_Status_t d_COUNTER_Initialise(void);

/* Strobe the watchdog */
d_Status_t d_COUNTER_Read(const Uint32_t ioc, const Uint32_t channel, Uint32_t * const pValue);

#endif /* D_COUNTER_H */
