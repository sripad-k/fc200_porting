/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Watchdog

  Abstract           : Watchdog interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-162
                       SDD References: 136T-2200-131000-001-D22 SWDES-251
                                                                SWDES-252
                                                                SWDES-253
\note
  CSC ID             : SWDES-65
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"
#include "soc/discrete/d_discrete.h"
#include "soc/timer/d_timer.h"
#include "d_watchdog.h"
#include "d_watchdog_cfg.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;
static Bool_t enabled = d_FALSE;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_WDOG_Initialise -->

  Initialise the watchdog interface.
*************************************************************************/
d_Status_t         /** \return Function status */
d_WDOG_Initialise
(
void
)
{
  d_Status_t returnValue = d_DISC_SetAsOutputPin(watchdogPin);

  if (returnValue == d_STATUS_SUCCESS)
  {
    initialised = d_TRUE;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, watchdogPin, 0, 0);
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_WDOG_Service -->

  Strobe the watchdog.
************************************************************************/
d_Status_t         /** \return Function status */
d_WDOG_Service
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  
  if (initialised == d_TRUE)
  {
    if (enabled == d_TRUE)
    {
      (void)d_DISC_ClearPin(watchdogPin);

      (void)d_TIMER_DelayMicroseconds(pulseLength);

      (void)d_DISC_SetPin(watchdogPin);
    }
    else
    {
      status = d_STATUS_INVALID_MODE;
    }
  }
  else
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    status = d_STATUS_NOT_INITIALISED;
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_WDOG_Start -->

  Start strobing the watchdog.
************************************************************************/
d_Status_t         /** \return Function status */
d_WDOG_Start
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised == d_TRUE)
  {
    enabled = d_TRUE;
  }
  else
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    status = d_STATUS_NOT_INITIALISED;
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_WDOG_Stop -->

  Stop strobing the watchdog.
************************************************************************/
d_Status_t         /** \return Function status */
d_WDOG_Stop
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised == d_TRUE)
  {
    enabled = d_FALSE;
  }
  else
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    status = d_STATUS_NOT_INITIALISED;
  }
  
  return status;
}


