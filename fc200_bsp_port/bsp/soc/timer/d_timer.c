/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Timer

  Abstract           : Module providing basic timing functions

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

/***** Includes *********************************************************/

#include "xparameters.h"                 /** Xilinx parameter definition */
#include "soc/defines/d_common_asm.h"
#include "kernel/general/d_gen_register.h"
#include "soc/timer/d_timer_counter.h"
#include "soc/timer/d_timer.h"
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */

/***** Constants ********************************************************/

/* Timer used for process timing */
#define TIMER d_TIMER_TTC0_2

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_TIMER_Initialise -->

  Initialise the timer for 100MHz / 64 clock.
  Thus the 32 bit timer will wrap in approx 45 minutes.
*************************************************************************/
void
d_TIMER_Initialise
(
void
)
{
  /* Set clock to 100MHz / 2^6 */
  (void)d_TIMER_Configure(TIMER, d_TRUE, 5u);

  /* Enable timer */
  (void)d_TIMER_Start(TIMER);

  return;
}

/*********************************************************************//**
  <!-- d_TIMER_ReadValueInTicks -->

  Reads the value of the global timer
*************************************************************************/
Uint32_t                     /** \return The value of the global timer in ticks */
d_TIMER_ReadValueInTicks
(
void
)
{
  Uint32_t value;

  (void)d_TIMER_Read(TIMER, &value);

  return value;
}

/*********************************************************************//**
  <!-- d_TIMER_ElapsedMilliseconds -->

  Measure the elapsed time in milliseconds.
*************************************************************************/
Uint32_t                     /** \return Elapsed time in milliseconds */
d_TIMER_ElapsedMilliseconds
(
const Uint32_t start,        /**< Value of timer at start of period to be measured */
Uint32_t * const now         /**< Pointer to storage for the current timer value */
)
{
  Uint32_t current;
  Uint32_t elapsed;
  
  current = d_TIMER_ReadValueInTicks();

  if (current >= start)
  {
    elapsed = (Uint32_t)((((Uint64_t)current - (Uint64_t)start) * 2uL) / 3125uL);
  }
  else
  {
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    elapsed = (Uint32_t)(((0xFFFFFFFFuL - ((Uint64_t)start - (Uint64_t)current)) * 2uL) / 3125uL);
  }

  /* provide current time value if requested */
  if (now != NULL)
  {
    *now = current;
  }
  else
  {
    DO_NOTHING();
  }
  
  return elapsed;
}

/*********************************************************************//**
  <!-- d_TIMER_ElapsedMicroseconds -->

  Measure the elapsed time in microseconds.
*************************************************************************/
Uint32_t                     /** \return Elapsed time in microseconds */
d_TIMER_ElapsedMicroseconds
(
const Uint32_t start,        /**< Value of timer at start of period to be measured */
Uint32_t * const now         /**< Pointer to storage for the current timer value */
)
{
  Uint32_t current;
  Uint32_t elapsed;
  
  current = d_TIMER_ReadValueInTicks();

  if (current >= start)
  {
    elapsed = (Uint32_t)((((Uint64_t)current - (Uint64_t)start) * 16uL) / 25uL);
  }
  else
  {
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    elapsed = (Uint32_t)(((0xFFFFFFFFuL - ((Uint64_t)start - (Uint64_t)current)) * 16uL) / 25uL);
  }

  /* provide current time value if requested */
  if (now != NULL)
  {
    *now = current;
  }
  else
  {
    DO_NOTHING();
  }

  return elapsed;
}

/*********************************************************************//**
  <!-- d_TIMER_DelayMilliseconds -->

  Delay for a number of milliseconds. Maximum 2748779 milliseconds.
*************************************************************************/
d_Status_t                   /** \Succes or failure None */
d_TIMER_DelayMilliseconds
(
const Uint32_t delay         /**< delay number of milliseconds */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (delay > 2748779u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, delay, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t elapsed;
  Uint32_t start = d_TIMER_ReadValueInTicks();
  Uint32_t complete = (Uint32_t)(((Uint64_t)delay * 3125uL) / 2uL);
  do
  {
    Uint32_t now = d_TIMER_ReadValueInTicks();
    elapsed = now - start;
  } while (elapsed < complete);

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_DelayMicroseconds -->

  Delay for a number of microseconds. Maximum 2748779068 microseconds.
*************************************************************************/
d_Status_t                   /** \Succes or failure None */
d_TIMER_DelayMicroseconds
(
const Uint32_t delay         /**< delay number of microseconds */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (delay > 2748779068u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, delay, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t elapsed;
  Uint32_t start = d_TIMER_ReadValueInTicks();
  Uint32_t complete = (Uint32_t)(((Uint64_t)delay * 25uL) / 16uL);
  do
  {
    Uint32_t now = d_TIMER_ReadValueInTicks();
    elapsed = now - start;
  } while (elapsed < complete);

  return status;
}

