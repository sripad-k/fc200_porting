/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Temperature

  Abstract           : TMP1075 Temperature sensor interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-125
                       SDD References: 136T-2200-131000-001-D22 SWDES-2426
\note
  CSC ID             : SWDES-2420
*************************************************************************/

/***** Includes *********************************************************/
#include "sru/temperature/d_temperature.h"            /* Discrete driver */
#include "sru/temperature/d_temperature_cfg.h"        /* Discrete driver config */
#include "soc/defines/d_common_status.h"              /* Error status */
#include "kernel/error_handler/d_error_handler.h"     /* Error handler */
#include "kernel/general/d_gen_register.h"            /* Register functions */
#include "sru/fcu/d_fcu_cfg.h"                        /* FCU CSC */

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Float32_t SCALING_FACTOR = 0.0625f;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t DEVICE_ID_OFFSET = 0x3Cu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t DEVICE_ID = 0x7500u;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_TEMPERATURE_Initialise -->

  Verify configuration  data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_TEMPERATURE_Initialise
(
void
)
{
  if (d_TEMPERATURE_Count > d_TEMPERATURE_MAXIMUM_CHANNELS)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, d_TEMPERATURE_Count, d_TEMPERATURE_MAXIMUM_CHANNELS);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }


  for (Uint32_t index = 0; index < d_TEMPERATURE_Count; index++)
  {
    if (d_TEMPERATURE_Configuration[index].baseAddress == 0x00000000u)
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
  <!-- d_TEMPERATURE_Read -->

  Read the temperature
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_TEMPERATURE_Read
(
const Uint32_t device,            /**< [in] Firmware block number */
Float32_t * const pTemperature    /**< [out] Pointer: return temperature */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_TEMPERATURE_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pTemperature == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  returnValue = d_FCU_IocAddressCheck(d_TEMPERATURE_Configuration[device].baseAddress);
  if (returnValue == d_STATUS_SUCCESS)
  {
    /* Read register value */
    Uint32_t regValue = d_GEN_RegisterRead(d_TEMPERATURE_Configuration[device].baseAddress + DEVICE_ID_OFFSET);
    if (regValue == DEVICE_ID)
    {
      regValue = d_GEN_RegisterRead(d_TEMPERATURE_Configuration[device].baseAddress);
      regValue = (regValue & 0xFFFFu) >> 4u;
      if ((regValue & 0x0800u) != 0u)
      {
        // gcov-jst 1 It is not practical to produce a negative reading during bench testing.
        regValue = regValue | 0xFFFFF000u;
      }
      Int32_t tempValue = (Int32_t)regValue;
      *pTemperature = (Float32_t)tempValue * SCALING_FACTOR;
    }
    else
    {
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      *pTemperature = 0.0f;
      returnValue = d_STATUS_HARDWARE_ERROR;
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_TEMPERATURE_Bit -->

  Read all channels and verify values are within tolerance
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_TEMPERATURE_Bit
(
d_TEMPERATURE_Results_t results   /**< [out] Pointer: return results */
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

  for (Uint32_t device = 0; device < d_TEMPERATURE_Count; device++)
  {
    if ((d_TEMPERATURE_Configuration[device].minimum != 0.0f) || (d_TEMPERATURE_Configuration[device].maximum != 0.0f))
    {
      d_Status_t channelStatus = d_TEMPERATURE_Read(device, &value);
      if (channelStatus == d_STATUS_SUCCESS)
      {
        results[device].value = value;
        if (value < d_TEMPERATURE_Configuration[device].minimum)
        {
          // gcov-jst 2 It is not possible to generate this failure during bench testing.
          results[device].bitStatus = d_TEMPERATURE_BIT_BELOW_MINIMUM;
          status = d_STATUS_FAILURE;
        }
        else if (value > d_TEMPERATURE_Configuration[device].maximum)
        {
          // gcov-jst 2 It is not possible to generate this failure during bench testing.
          results[device].bitStatus = d_TEMPERATURE_BIT_ABOVE_MAXIMUM;
          status = d_STATUS_FAILURE;
        }
        else
        {
          results[device].bitStatus = d_TEMPERATURE_BIT_PASS;
        }
      }
      else
      {
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        results[device].bitStatus = d_TEMPERATURE_BIT_IOC_OFFLINE;
        status = d_STATUS_FAILURE;
      }
    }
    else
    {
      // gcov-jst 1 It is not possible to execute this code with current configuration.
      results[device].bitStatus = d_TEMPERATURE_BIT_NOT_CHECKED;
    }
  }
  
  return status;
}
