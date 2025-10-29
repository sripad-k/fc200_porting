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

#ifndef D_TEMPERATURE_H
#define D_TEMPERATURE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

#define d_TEMPERATURE_MAXIMUM_CHANNELS   16u

/***** Type Definitions *************************************************/

typedef enum
{
  d_TEMPERATURE_BIT_PASS = 0,
  d_TEMPERATURE_BIT_BELOW_MINIMUM,
  d_TEMPERATURE_BIT_ABOVE_MAXIMUM,
  d_TEMPERATURE_BIT_NOT_CHECKED,
  d_TEMPERATURE_BIT_IOC_OFFLINE
} d_TEMPERATURE_BitStatus_t;

typedef struct
{
  d_TEMPERATURE_BitStatus_t bitStatus;
  Float32_t value;
} d_TEMPERATURE_Result_t;

typedef d_TEMPERATURE_Result_t d_TEMPERATURE_Results_t[d_TEMPERATURE_MAXIMUM_CHANNELS];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Verify configuration data */
d_Status_t d_TEMPERATURE_Initialise(void);

/* Read the status of a discrete pin */
d_Status_t d_TEMPERATURE_Read(const Uint32_t device, Float32_t * const pTemperature);

/* Read all channels and verify values are within tolerance */
d_Status_t d_TEMPERATURE_Bit(d_TEMPERATURE_Results_t results);

#endif /* D_TEMPERATURE_H */
