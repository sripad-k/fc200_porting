/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SBUS

  Abstract           : SBUS interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-331
                                                                SWREQ-332
                       SDD References: 136T-2200-131000-001-D22 SWDES-893
                                                                SWDES-894

\note
  CSC ID             : SWDES-890
*************************************************************************/

/***** Includes *********************************************************/

#include "sru/sbus/d_sbus.h"                       /* SBUS CSC */
#include "sru/sbus/d_sbus_cfg.h"                   /* SBUS CSC Config */
#include "sru/fcu/d_fcu.h"                         /* FCU CSC */
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
  <!-- d_SBUS_Initialise -->

  Initialise the CSC 
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SBUS_Initialise
(
void
)
{
  for (Uint32_t ioc = 0; ioc < d_SBUS_IOC_COUNT; ioc++)
  {
    if (d_SBUS_BaseAddress[ioc] == 0x00000000u)
    {
      // gcov-jst 3 It is not practical to generate this error during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 1, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }
  }
  
  initialised = d_TRUE;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_SBUS_ReadValue -->

  Read an SBUS channel.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SBUS_ReadValue
(
const Uint32_t ioc,               /**< [in]  IOC number */
const Uint32_t channel,           /**< [in]  SBUS channel */
Uint32_t * const pValue           /**< [out] Pointer to storage for read value */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (ioc >= d_SBUS_IOC_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, ioc, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (channel >= d_SBUS_MAX_CHANNELS)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue ==NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (d_FCU_IocOnline(ioc) != d_TRUE)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    // cppcheck-suppress misra-c2012-10.4; Adding integer to error code is the most efficient method of implementing this
    return d_STATUS_OFFLINE_IOCA + ioc;
  }
  
  *pValue = d_GEN_RegisterRead(d_SBUS_BaseAddress[ioc] + (channel * 0x04u));

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_SBUS_ReadFlags -->

  Read the flags data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SBUS_ReadFlags
(
const Uint32_t ioc,               /**< [in]  IOC number */
Uint32_t * const pFlags           /**< [out] Pointer to storage for flags value */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (ioc >= d_SBUS_IOC_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, ioc, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFlags == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (d_FCU_IocOnline(ioc) != d_TRUE)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    // cppcheck-suppress misra-c2012-10.4; Adding integer to error code is the most efficient method of implementing this
    return d_STATUS_OFFLINE_IOCA + ioc;
  }
  
  *pFlags = d_GEN_RegisterRead(d_SBUS_BaseAddress[ioc] + 0x40u);

  return d_STATUS_SUCCESS;
}

