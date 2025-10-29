/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_date_time

  Abstract           : Date and Time driver module

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-275
                                                                SWREQ-276
                                                                SWREQ-277
                       SDD References: 136T-2200-131100-001-D22 SWDES-217
                                                                SWDES-218
                                                                SWDES-219
                                                                SWDES-220

\note
  CSC ID             : SWDES-78
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/timer/d_timer.h"
#include "soc/interrupt_manager/d_int_critical.h"

#include "d_date_time.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/* Time in milliseonds since 1980-01-06 (GNSS epoch) */
/* excluding leap seconds */
static Uint64_t timestamp;

static Uint32_t timerRef = 0;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_DATE_TIME_Initialise -->

  Initialise the Date and Time CSC
*************************************************************************/
d_Status_t                   /** \return Success or Failure */
d_DATE_TIME_Initialise
(
void
)
{
  d_DATE_TIME_TimestampSet(0);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DATE_TIME_Timestamp -->

  Get the timestamp
*************************************************************************/
Uint64_t                     /** \return Timestamp */
d_DATE_TIME_Timestamp
(
void
)
{
  Uint32_t flags =  d_INT_CriticalSectionEnter();
  Uint64_t tstamp = timestamp;
  d_INT_CriticalSectionLeave(flags);

  return tstamp;
}

/*********************************************************************//**
  <!-- d_DATE_TIME_TimestampSet -->

  Set the timestamp
*************************************************************************/
void                         /** \return None */
d_DATE_TIME_TimestampSet
(
const Uint64_t newTimestamp  /**< {in] Timestamp value to set */
)
{
  timestamp = newTimestamp;
  (void)d_TIMER_ElapsedMicroseconds(0, &timerRef);

  return;
}

/*********************************************************************//**
  <!-- d_DATE_TIME_TimestampUpdate -->

  Add milliseconds to timestamp
*************************************************************************/
void                         /** \return None */
d_DATE_TIME_TimestampUpdate
(
void
)
{
  Uint32_t flags =  d_INT_CriticalSectionEnter();
  timestamp = timestamp + (d_TIMER_ElapsedMicroseconds(timerRef, &timerRef) + 500u) / 1000u;
  d_INT_CriticalSectionLeave(flags);

  return;
}

