/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : Analogue PL

  Abstract           : Analogue interface to PL measured values.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-210
                                                                SWREQ-211
                       SDD References: 136T-2200-131000-001-D22 SWDES-94
                                                                SWDES-95
                                                                SWDES-96
                                                                SWDES-97
\note
  CSC ID             : SWDES-56
*************************************************************************/

#ifndef D_ANALOGUE_H
#define D_ANALOGUE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* Following enumeration follows the same sequence as the structure array
   defined in the c file */
typedef enum
{
  d_ANALOG_PS_TEMPERATURE_LPD,
  d_ANALOG_PS_VCC_INTLP,
  d_ANALOG_PS_VCC_INTFP,
  d_ANALOG_PS_VCC_PSAUX,
  d_ANALOG_PS_VCC_PSDDR,
  d_ANALOG_PS_VCC_PSIO3,
  d_ANALOG_PS_VCC_PSIO0,
  d_ANALOG_PS_VCC_PSIO1,
  d_ANALOG_PS_VCC_PSIO2,
  d_ANALOG_PS_VCC_PSGTR,
  d_ANALOG_PS_VIT_PSGTR,
  d_ANALOG_PS_VCC_PSADC,
  d_ANALOG_PS_TEMPERATURE_FPD,

  d_ANALOG_PL_TEMPERATURE,
  d_ANALOG_PL_VCC_INT,
  d_ANALOG_PL_VCC_AUX,
  d_ANALOG_PL_VREFP,
  d_ANALOG_PL_VREFN,
  d_ANALOG_PL_VCC_BRAM,
  d_ANALOG_PL_VCC_PSINTLP,
  d_ANALOG_PL_VCC_PSINTFP,
  d_ANALOG_PL_VCC_PSAUX,

  d_ANALOG_CHANNEL_COUNT
} d_ANALOG_Channel_t;

typedef enum
{
  d_ANALOG_BIT_PASS = 0,
  d_ANALOG_BIT_BELOW_MINIMUM,
  d_ANALOG_BIT_ABOVE_MAXIMUM,
  d_ANALOG_BIT_NOT_CHECKED
} d_ANALOG_BitStatus_t;
  
typedef struct
{
  d_ANALOG_BitStatus_t bitStatus;
  Float32_t value;
} d_ANALOG_Result_t;

typedef d_ANALOG_Result_t d_ANALOG_Results_t[d_ANALOG_CHANNEL_COUNT];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the analogue interface */
d_Status_t d_ANALOG_Initialise(void);

/* Read an analogue value */
d_Status_t d_ANALOG_Read(d_ANALOG_Channel_t signal, Float32_t * const pValue);

/* Read all channels and verify values are within tolerance */
d_Status_t d_ANALOG_Bit(d_ANALOG_Results_t results);

#endif /* D_ANALOGUE_H */
