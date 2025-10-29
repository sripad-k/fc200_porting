/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_memory_cfg.h

  Abstract           : MEMORY MPU configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-153
                       SDD References: 136T-2200-131000-001-D22 SWDES-129
                                                                SWDES-130
                                                                SWDES-131
                                                                SWDES-132
                                                                SWDES-133
                                                                SWDES-134
                                                                SWDES-135
                                                                SWDES-136
                                                                SWDES-137
                                                                SWDES-138

\note
  CSC ID             : SWDES-51
*************************************************************************/

#ifndef D_MEMORY_CFG_H
#define D_MEMORY_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_MEMORY_REGION_COUNT const Uint32_t d_MEMORY_RegionCount = (sizeof(d_MEMORY_Configuration) / sizeof(d_MEMORY_Configuration_t))

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t baseAddress;  /* MPU region base address */
  Uint32_t sizeCode;     /* MPU region size code */
  Uint32_t attribute;    /* MPU region attribute */
} d_MEMORY_Configuration_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_MEMORY_Configuration_t d_MEMORY_Configuration[];

extern const Uint32_t d_MEMORY_RegionCount;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_MEMORY_CFG_H */
