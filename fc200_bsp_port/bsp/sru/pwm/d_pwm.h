/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : d_pwm.h

  Abstract           : Pulse Width Modulation interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-327
                       SDD References: 136T-2200-131000-001-D22 SWDES-554
                                                                SWDES-605
\note
  CSC ID             : SWDES-71
*************************************************************************/

#ifndef D_PWM_H
#define D_PWM_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Set PWM outputs to their default values */
d_Status_t d_PWM_Initialise(void);

/* Set the PWM output */
d_Status_t d_PWM_Output(const Uint32_t ioc, const Uint32_t channel, const Float32_t value);

/* Read the PWM input */
d_Status_t d_PWM_Input(const Uint32_t ioc, const Uint32_t channel, Float32_t * const pValue);

#endif /* D_PWM_H */
