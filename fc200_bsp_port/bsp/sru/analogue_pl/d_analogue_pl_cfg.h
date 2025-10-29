/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Analogue PL

  Abstract           : ANALOGUE PL interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-127
                       SDD References: 136T-2200-131000-001-D22 SWDES-189
                                                                SWDES-190
                                                                SWDES-191
                                                                SWDES-543
                       SRS References: 136T-2200-131000-001-D20 SWREQ-222
                       SDD References: 136T-2200-131000-001-D22 SWREQ-???
\note
  CSC ID             : SWDES-56
*************************************************************************/

#ifndef D_ANALOGUE_PL_CFG_H
#define D_ANALOGUE_PL_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_ANALOGUE_PL_COUNT const Uint32_t d_ANALOGUE_PL_Count = (sizeof(d_ANALOGUE_PL_Configuration) / sizeof(d_ANALOGUE_PL_Configuration_t))

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t baseAddress;
  Uint32_t bitCount;
  Float32_t scalingGain;
  Float32_t scalingOffset;
  Float32_t minimum;
  Float32_t maximum;
} d_ANALOGUE_PL_Configuration_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_ANALOGUE_PL_Configuration_t d_ANALOGUE_PL_Configuration[];

extern const Uint32_t d_ANALOGUE_PL_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_ANALOGUE_PL_CFG_H */
