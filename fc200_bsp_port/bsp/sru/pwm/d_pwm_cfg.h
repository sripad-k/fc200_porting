/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : d_pwm_cfg.h

  Abstract           : Pulse Width Modulation interface configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-327
                       SDD References: 136T-2200-131000-001-D22 SWDES-554
                                                                SWDES-605
\note
  CSC ID             : SWDES-71
*************************************************************************/

#ifndef D_PWM_CFG_H
#define D_PWM_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_PWM_MAX_IOCS       2u

/* Not that the FC-200 IOC only supports 8 channels */
#define d_PWM_MAX_CHANNELS  16u

#define d_PWM_COUNT_IOC const Uint32_t d_PWM_CountIoc = (sizeof(d_PWM_AddressDefinition) / sizeof(d_PWM_AddressDefinition_t))

#define d_PWM_COUNT_CHANNEL const Uint32_t d_PWM_CountChannel = (sizeof(d_PWM_Definition) / sizeof(d_PWM_Definition_t))

/***** Type Definitions *************************************************/

typedef struct
{
  Float32_t minimumValue;
  Float32_t maximumValue;
  Float32_t defaultValue;
} d_PWM_Definition_t;

typedef struct
{
  Uint32_t pwmInputBaseAddress;
  Uint32_t pwmOutputBaseAddress;
} d_PWM_AddressDefinition_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_PWM_AddressDefinition_t d_PWM_AddressDefinition[];

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_PWM_Definition_t d_PWM_Definition[];

extern const Uint32_t d_PWM_CountIoc;
extern const Uint32_t d_PWM_CountChannel;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_PWM_CFG_H */
