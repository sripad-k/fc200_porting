/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : d_counter_cfg.h

  Abstract           : Counter interface configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-
                       SDD References: 136T-2200-131000-001-D22 SWDES-
\note
  CSC ID             : SWDES-
*************************************************************************/

#ifndef D_COUNTER_CFG_H
#define D_COUNTER_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_COUNTER_MAX_IOCS       2u

/* Not that the FC-200 IOC only supports 8 channels */
#define d_COUNTER_MAX_CHANNELS  16u

#define d_COUNTER_COUNT_IOC const Uint32_t d_COUNTER_CountIoc = (sizeof(d_COUNTER_BaseAddress) / sizeof(Uint32_t))

/***** Type Definitions *************************************************/

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const Uint32_t d_COUNTER_BaseAddress[];

extern const Uint32_t d_COUNTER_CountIoc;
extern const Uint32_t d_COUNTER_CountChannel;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_COUNTER_CFG_H */
