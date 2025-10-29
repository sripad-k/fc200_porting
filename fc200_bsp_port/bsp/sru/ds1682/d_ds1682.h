/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_ds1682.h

  Abstract           : Elapsed time power cycle counter interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-154
                                                                SWREQ-155
                       SDD References: 136T-2200-131000-001-D22 SWDES-548
                                                                SWDES-859
\note
  CSC ID             : SWDES-63
*************************************************************************/

#ifndef D_DS1682_H
#define D_DS1682_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Verify configuration */
d_Status_t d_DS1682_Initialise(void);

/* Read the power cycles and elapsed time, in units of 0.25 seconds. */
d_Status_t d_DS1682_Read(const Uint32_t device, Uint32_t * const pPowerCount, Uint32_t * const pElapsedTime);

#endif /* D_DS1682_H */
