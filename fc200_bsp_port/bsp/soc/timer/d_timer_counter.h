/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Timer Counter

  Abstract : Control of Zynq UltraScale triple timer counters

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-328
                       SDD References: 136T-2200-131000-001-D22 SWDES-148
                                                                SWDES-149
                                                                SWDES-150
                                                                SWDES-151
                                                                SWDES-152
                                                                SWDES-153
                                                                SWDES-154
                                                                SWDES-155
                                                                SWDES-156
                                                                SWDES-157
\note
  CSC ID             : SWDES-53
*************************************************************************/

#ifndef TIMER_COUNTER_H
#define TIMER_COUNTER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/* Timer identification, 4 x triple timer counters */
typedef enum
{
	d_TIMER_TTC0_0,
	d_TIMER_TTC0_1,
	d_TIMER_TTC0_2,
	d_TIMER_TTC1_0,
	d_TIMER_TTC1_1,
	d_TIMER_TTC1_2,
	d_TIMER_TTC2_0,
	d_TIMER_TTC2_1,
	d_TIMER_TTC2_2,
	d_TIMER_TTC3_0,
	d_TIMER_TTC3_1,
	d_TIMER_TTC3_2,

	d_TIMER_COUNT
} d_Timer_t;

/* Interrupt selection */
typedef enum
{
	d_TIMER_INTERRUPT_INTERVAL,
	d_TIMER_INTERRUPT_MATCH_1,
	d_TIMER_INTERRUPT_MATCH_2,
	d_TIMER_INTERRUPT_MATCH_3,
	d_TIMER_INTERRUPT_OVERFLOW_COUNTER,
	d_TIMER_INTERRUPT_OVERFLOW_TIMER,

	d_TIMER_INTERRUPT_COUNT
} d_Timer_Interrupt_t;

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Configure timer */
d_Status_t d_TIMER_Configure(const d_Timer_t timer, const Bool_t prescalerEnable, const Uint32_t prescalerValue);

/* Setup options */
d_Status_t d_TIMER_Options(const d_Timer_t timer, const Bool_t intervalMode);

/* Set timer interval */
d_Status_t d_TIMER_Interval(const d_Timer_t timer, const Uint32_t interval);

/* Start timer */
d_Status_t d_TIMER_Start(const d_Timer_t timer);

/* Stop timer */
d_Status_t d_TIMER_Stop(const d_Timer_t timer);

/* Reset timer */
d_Status_t d_TIMER_Reset(const d_Timer_t timer);

/* Read timer value */
d_Status_t d_TIMER_Read(const d_Timer_t timer, Uint32_t * const pValue);

/* Enable timer interrupt */
d_Status_t d_TIMER_InterruptEnable(const d_Timer_t timer, d_Timer_Interrupt_t interrupt);

/* Disable timer interrupt */
d_Status_t d_TIMER_InterruptDisable(const d_Timer_t timer, d_Timer_Interrupt_t interrupt);

/* Read interrupt status */
d_Status_t d_TIMER_InterruptStatus(const d_Timer_t timer, Uint32_t * const pValue);


#endif /* TIMER_COUNTER_H */
