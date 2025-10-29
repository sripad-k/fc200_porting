/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_ds1682.c

  Abstract           : Elapsed time power cycle counter interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-154
                                                                SWREQ-155
                       SDD References: 136T-2200-131000-001-D22 SWDES-548
                                                                SWDES-859
\note
  CSC ID             : SWDES-63
*************************************************************************/

/***** Includes *********************************************************/

#include "sru/ds1682/d_ds1682.h"                   /* Discrete driver */
#include "sru/ds1682/d_ds1682_cfg.h"               /* Discrete driver config */
#include "sru/fcu/d_fcu_cfg.h"                      /* FCU CSC */
#include "soc/defines/d_common_status.h"           /* Error status */
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "kernel/general/d_gen_register.h"         /* Register functions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_DS1682_Initialise -->

  Verify configuration data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DS1682_Initialise
(
void
)
{
  for (Uint32_t index = 0; index < d_DS1682_Count; index++)
  {
    if (d_DS1682_BaseAddress[index] == 0x00000000u)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, index, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }
  }
  
  initialised = d_TRUE;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DS1682_Read -->

  Read the power cycles and elapsed time, in units of 0.25 seconds.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DS1682_Read
(
const Uint32_t device,            /**< [in] Device number */
Uint32_t * const pPowerCount,     /**< [out] Pointer: power cycles */
Uint32_t * const pElapsedTime     /**< [out] Pointer: elapsed time, 250ms resolution */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_DS1682_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pPowerCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pElapsedTime == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t iocStatus = d_FCU_IocAddressCheck(d_DS1682_BaseAddress[device]);
  if (iocStatus == d_STATUS_SUCCESS)
  {
    /* Read register values */
    *pPowerCount = d_GEN_RegisterRead(d_DS1682_BaseAddress[device]) & 0x0000FFFFu;

    *pElapsedTime = d_GEN_RegisterRead(d_DS1682_BaseAddress[device] + 0x04u);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return iocStatus;
}

