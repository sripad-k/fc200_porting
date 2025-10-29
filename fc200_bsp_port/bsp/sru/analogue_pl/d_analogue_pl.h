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

#ifndef D_ANALOGUE_PL_H
#define D_ANALOGUE_PL_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

#define d_ANALOGUE_PL_MAXIMUM_CHANNELS   128u

/***** Type Definitions *************************************************/

typedef enum
{
  d_ANALOGUE_PL_BIT_PASS = 0,
  d_ANALOGUE_PL_BIT_BELOW_MINIMUM,
  d_ANALOGUE_PL_BIT_ABOVE_MAXIMUM,
  d_ANALOGUE_PL_BIT_NOT_CHECKED,
  d_ANALOGUE_PL_BIT_IOC_OFFLINE
} d_ANALOGUE_PL_BitStatus_t;

typedef struct
{
  d_ANALOGUE_PL_BitStatus_t bitStatus;
  Float32_t value;
} d_ANALOGUE_PL_Result_t;

typedef d_ANALOGUE_PL_Result_t d_ANALOGUE_PL_Results_t[d_ANALOGUE_PL_MAXIMUM_CHANNELS];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Verify configuration and initialise data */
d_Status_t d_ANALOGUE_PL_Initialise(void);

/* Read the status of a discrete pin */
d_Status_t d_ANALOGUE_PL_ReadValue(const Uint32_t device, Float32_t * const pValue);

/* Read all channels and verify values are within tolerance */
d_Status_t d_ANALOGUE_PL_Bit(d_ANALOGUE_PL_Results_t results);

#endif /* D_ANALOGUE_PL_H */
