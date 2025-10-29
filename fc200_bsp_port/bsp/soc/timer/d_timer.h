/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Timer

  Abstract : Module providing basic timing functions

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-170
                                                                SWREQ-171
                       SDD References: 136T-2200-131000-001-D22 SWDES-158
                                                                SWDES-159
                                                                SWDES-160
                                                                SWDES-161
                                                                SWDES-162
                                                                SWDES-163
                                                                SWDES-164
\note
  CSC ID             : SWDES-53
*************************************************************************/

#ifndef TIMER_H
#define TIMER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the timer */
void d_TIMER_Initialise(void);

/* Read the global timer value in ticks */
Uint32_t d_TIMER_ReadValueInTicks(void);

/* Measure the elapsed time in milliseconds */
Uint32_t d_TIMER_ElapsedMilliseconds
(
const Uint32_t start,           /** Value of timer at start of period to be measured */
Uint32_t * const now            /** Pointer to storage for the current timer value */
);

/* Measure the elapsed time in microseconds */
Uint32_t d_TIMER_ElapsedMicroseconds
(
const Uint32_t start,           /** Value of timer at start of period to be measured */
Uint32_t * const now            /** Pointer to storage for the current timer value */
);

/* Delay for a number of milliseconds. Maximum 2748779 milliseconds. */
d_Status_t d_TIMER_DelayMilliseconds(const Uint32_t delay);  /** delay number of milliseconds */

/* Delay for a number of microseconds. Maximum 2748779068 microseconds. */
d_Status_t d_TIMER_DelayMicroseconds(const Uint32_t delay);  /** delay number of microseconds */


#endif /* TIMER_H */
