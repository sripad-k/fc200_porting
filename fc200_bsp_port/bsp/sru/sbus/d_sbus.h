/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SBUS

  Abstract           : SBUS interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-331
                                                                SWREQ-332
                       SDD References: 136T-2200-131000-001-D22 SWDES-893
                                                                SWDES-894

\note
  CSC ID             : SWDES-890
*************************************************************************/

#ifndef D_SBUS_H
#define D_SBUS_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

#define d_SBUS_IOC_COUNT      2u

#define d_SBUS_MAX_CHANNELS  16u


/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the CSC */
d_Status_t d_SBUS_Initialise(void);

/* Read a SBUS channel */
d_Status_t d_SBUS_ReadValue(const Uint32_t ioc, const Uint32_t channel, Uint32_t * const pValue);

/* Read the flags data */
d_Status_t d_SBUS_ReadFlags(const Uint32_t ioc, Uint32_t * const pFlags);

#endif /* D_SBUS_H */
