/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Discrete Header File

  Abstract           : SOC PL Discrete interface configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-112
                       SDD References: 136T-2200-131100-001-D22 SWDES-165
                                                                SWDES-166
                                                                SWDES-167
                                                                SWDES-168
                                                                SWDES-169
                                                                SWDES-170
                                                                SWDES-171
                                                                SWDES-192
\note
  CSC ID             : SWDES-55
*************************************************************************/

#ifndef D_DISC_PL_CFG_H
#define D_DISC_PL_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_DISC_PL_MAX_INTERFACES  10u

#define d_DISC_PL_COUNT const Uint32_t d_DISC_PL_Count = (sizeof(d_DISC_PL_BaseAddress) / sizeof(Pointer_t))

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const Pointer_t d_DISC_PL_BaseAddress[];

extern const Uint32_t d_DISC_PL_Count;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_DISC_PL_CFG_H */
