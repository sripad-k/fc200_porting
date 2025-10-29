/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SBUS CFG

  Abstract           : SBUS interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-331
                                                                SWREQ-332
                       SDD References: 136T-2200-131000-001-D22 SWDES-893
                                                                SWDES-894

\note
  CSC ID             : SWDES-890
*************************************************************************/

#ifndef D_SBUS_CFG_H
#define D_SBUS_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "sru/sbus/d_sbus.h"

/***** Literals *********************************************************/

/***** Type Definitions *************************************************/

/***** Constants ********************************************************/

extern const Uint32_t d_SBUS_BaseAddress[d_SBUS_IOC_COUNT];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_SBUS_CFG_H */
