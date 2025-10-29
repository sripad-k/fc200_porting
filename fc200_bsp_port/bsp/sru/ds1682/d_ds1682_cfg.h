/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_ds1682_cfg.h.c

  Abstract           : Elapsed time power cycle counter interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-154
                                                                SWREQ-155
                       SDD References: 136T-2200-131000-001-D22 SWDES-548
                                                                SWDES-859
\note
  CSC ID             : SWDES-63
*************************************************************************/

#ifndef D_DS1682_CFG_H
#define D_DS1682_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_DS1682_COUNT const Uint32_t d_DS1682_Count = (sizeof(d_DS1682_BaseAddress) / sizeof(Pointer_t))

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const Pointer_t d_DS1682_BaseAddress[];

extern const Uint32_t d_DS1682_Count;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_DS1682_CFG_H */
