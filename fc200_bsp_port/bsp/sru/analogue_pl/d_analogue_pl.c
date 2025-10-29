/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Analogue PL

  Abstract           : ANALOGUE PL interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-127
                       SDD References: 136T-2200-131000-001-D22 SWDES-189
                                                                SWDES-190
                                                                SWDES-191
                                                                SWDES-543
\note
  CSC ID             : SWDES-56
*************************************************************************/

/***** Includes *********************************************************/

#include "sru/analogue_pl/d_analogue_pl.h"            /* Discrete driver */
#include "sru/analogue_pl/d_analogue_pl_cfg.h"        /* Discrete driver config */
#include "soc/defines/d_common_status.h"              /* Error status */
#include "kernel/error_handler/d_error_handler.h"     /* Error handler */
#include "kernel/general/d_gen_register.h"            /* Register functions */
#include "sru/fcu/d_fcu_cfg.h"                        /* FCU CSC */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_ANALOGUE_PL_Initialise -->

  Verify configuration data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_ANALOGUE_PL_Initialise
(
void
)
{
  if (d_ANALOGUE_PL_Count > d_ANALOGUE_PL_MAXIMUM_CHANNELS)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, d_ANALOGUE_PL_Count, d_ANALOGUE_PL_MAXIMUM_CHANNELS);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  for (Uint32_t index = 0; index < d_ANALOGUE_PL_Count; index++)
  {
    if (d_ANALOGUE_PL_Configuration[index].baseAddress == 0x00000000u)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 1, index, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }

    if (d_ANALOGUE_PL_Configuration[index].bitCount == 0x00000000u)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 2, index, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }

    if (d_ANALOGUE_PL_Configuration[index].scalingGain == 0.0f)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 3, index, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }
  }
  initialised = d_TRUE;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_ANALOGUE_PL_ReadValue -->

  Read the analogue valuen
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_ANALOGUE_PL_ReadValue
(
const Uint32_t device,            /**< [in] Device number */
Float32_t * const pValue          /**< [out] Pointer: return value */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_ANALOGUE_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_ANALOGUE_PL_Configuration[device].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    Uint32_t regValue = d_GEN_RegisterRead(d_ANALOGUE_PL_Configuration[device].baseAddress);
  
    Uint32_t mask = 0x00000001u << d_ANALOGUE_PL_Configuration[device].bitCount;
    mask = mask - 1u;
    regValue = regValue & mask;
  
    *pValue = ((Float32_t)regValue * d_ANALOGUE_PL_Configuration[device].scalingGain) + d_ANALOGUE_PL_Configuration[device].scalingOffset;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_ANALOGUE_PL_Bit -->

  Read all channels and verify values are within tolerance.
************************************************************************/
d_Status_t
d_ANALOGUE_PL_Bit
(
  d_ANALOGUE_PL_Results_t results
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Float32_t value = 0.0f;

  if (results == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  for (Uint32_t device = 0; device < d_ANALOGUE_PL_Count; device++)
  {
    if ((d_ANALOGUE_PL_Configuration[device].minimum != 0.0f) || (d_ANALOGUE_PL_Configuration[device].maximum != 0.0f))
    {
      d_Status_t channelStatus = d_ANALOGUE_PL_ReadValue(device, &value);
      if (channelStatus == d_STATUS_SUCCESS)
      {
        results[device].value = value;
        if (value < d_ANALOGUE_PL_Configuration[device].minimum)
        {
          // gcov-jst 2 It is not possible to generate this failure during bench testing.
          results[device].bitStatus = d_ANALOGUE_PL_BIT_BELOW_MINIMUM;
          status = d_STATUS_FAILURE;
        }
        else if (value > d_ANALOGUE_PL_Configuration[device].maximum)
        {
          // gcov-jst 2 It is not possible to generate this failure during bench testing.
          results[device].bitStatus = d_ANALOGUE_PL_BIT_ABOVE_MAXIMUM;
          status = d_STATUS_FAILURE;
        }
        else
        {
          results[device].bitStatus = d_ANALOGUE_PL_BIT_PASS;
        }
      }
      else
      {
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        results[device].bitStatus = d_ANALOGUE_PL_BIT_IOC_OFFLINE;
        status = d_STATUS_FAILURE;
      }
    }
    else
      {
        results[device].bitStatus = d_ANALOGUE_PL_BIT_NOT_CHECKED;
      }
  }
  
  return status;
}

