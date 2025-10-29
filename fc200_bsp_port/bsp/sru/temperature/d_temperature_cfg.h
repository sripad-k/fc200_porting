/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Temperature configuration

  Abstract           : TMP1075 Temperature sensor interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-125
                       SDD References: 136T-2200-131000-001-D22 SWDES-2426
\note
  CSC ID             : SWDES-2420
*************************************************************************/

#ifndef D_TEMPERATURE_CFG_H
#define D_TEMPERATURE_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_TEMPERATURE_MAX_INTERFACES  10u

#define d_TEMPERATURE_COUNT const Uint32_t d_TEMPERATURE_Count = (sizeof(d_TEMPERATURE_Configuration) / sizeof(d_TEMPERATURE_Configuration_t))

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t baseAddress;
  Float32_t minimum;
  Float32_t maximum;
} d_TEMPERATURE_Configuration_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_TEMPERATURE_Configuration_t d_TEMPERATURE_Configuration[];

extern const Uint32_t d_TEMPERATURE_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_TEMPERATURE_CFG_H */
