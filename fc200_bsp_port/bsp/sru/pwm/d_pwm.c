/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : d_pwm.c

  Abstract           : Pulse Width Modulation interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-327
                       SDD References: 136T-2200-131000-001-D22 SWDES-554
                                                                SWDES-605
\note
  CSC ID             : SWDES-71
*************************************************************************/

/***** Includes *********************************************************/

#include "sru/pwm/d_pwm.h"                         /* Discrete driver */
#include "sru/pwm/d_pwm_cfg.h"                     /* Discrete driver config */
#include "sru/fcu/d_fcu.h"                         /* FCU CSC */
#include "soc/defines/d_common_status.h"           /* Error status */
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "kernel/general/d_gen_register.h"         /* Register functions */

/***** Constants ********************************************************/

/* These values were determined by experimentation to match the fixed input scaling */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Float32_t MINIMUM_REG_VALUE = 177.0f;
static const Float32_t MAXIMUM_REG_VALUE = 1824.0f;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Float32_t OUTPUT_REG_SPAN = (MAXIMUM_REG_VALUE - MINIMUM_REG_VALUE);

/* Input values are read in microseconds */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t MINIMUM_UINT_READ_VALUE = 1000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t MAXIMUM_UINT_READ_VALUE = 2000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t INPUT_REG_SPAN = (MAXIMUM_UINT_READ_VALUE - MINIMUM_UINT_READ_VALUE);

/* PWM input registers */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t RESOLUTION_OFFSET = 0x04u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t TIMER_OFFSET      = 0x20u;

/* PWM output registers */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t ENABLE_OFFSET     = 0x20u;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static Float32_t valueSpan[d_PWM_MAX_CHANNELS];

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_PWM_Initialise -->

  Set PWM outputs to their default values 
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_PWM_Initialise
(
void
)
{
  Uint32_t channel;
  
  if (d_PWM_CountIoc > d_PWM_MAX_IOCS)
  {
		// gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 1, d_PWM_CountIoc, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  for (Uint32_t ioc = 0; ioc < d_PWM_CountIoc; ioc++)
  {
    if (d_PWM_AddressDefinition[ioc].pwmInputBaseAddress == 0x00000000u)
    {
  		// gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 2, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }
    if (d_PWM_AddressDefinition[ioc].pwmOutputBaseAddress == 0x00000000u)
    {
  		// gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 3, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }
  }
  
  if (d_PWM_CountChannel > d_PWM_MAX_CHANNELS)
  {
		// gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 4, d_PWM_CountChannel, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  /* Set initialised TRUE before setting the default outputs */
  initialised = d_TRUE;
  
  d_Status_t returnValue = d_STATUS_SUCCESS;
  
  for (Uint32_t ioc = 0; ioc < d_PWM_CountIoc; ioc++)
  {
    if (d_FCU_IocOnline(ioc) == d_TRUE)
    {
      for (channel = 0; channel < d_PWM_CountChannel; channel++)
      {
        /* Enable PWM outputs, setting 16 bits in case available channels changes in future firmware */
        d_GEN_RegisterWrite(d_PWM_AddressDefinition[ioc].pwmOutputBaseAddress + ENABLE_OFFSET, 0xFFFFu);
        /* Set input resolution to 1us, setting 16 bits in case available channels changes in future firmware */
        d_GEN_RegisterWrite(d_PWM_AddressDefinition[ioc].pwmInputBaseAddress + RESOLUTION_OFFSET, 0xFFFFu);
        valueSpan[channel] = d_PWM_Definition[channel].maximumValue - d_PWM_Definition[channel].minimumValue;
        (void)d_PWM_Output(ioc, channel, d_PWM_Definition[channel].defaultValue);
      }
    }
    else
    {
      // gcov-jst 7 It is not practical to generate this failure during bench testing.
      if (ioc == 0u)
      {
        returnValue = d_STATUS_OFFLINE_IOCA;
      }
      else
      {
        returnValue = d_STATUS_OFFLINE_IOCB;
      }
    }
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_PWM_Output -->

  Set the PWM output
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_PWM_Output
(
const Uint32_t ioc,               /**< [in] IOC number */
const Uint32_t channel,           /**< [in] PWM channel */
const Float32_t value             /**< [in] Value to set */
)
{
  Float32_t outputValue = value;

  if (ioc >= d_PWM_CountIoc)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, ioc, 0, 0);
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

  if (channel >= d_PWM_CountChannel)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (value < d_PWM_Definition[channel].minimumValue)
  {
    // gcov-jst 2 It is not practical to ensure coverage of this path during bench testing.
    outputValue = d_PWM_Definition[channel].minimumValue;
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_NON_CRITICAL, 3, (Uint32_t)(value * 1000.0f), 0, 0);
  }

  if (value > d_PWM_Definition[channel].maximumValue)
  {
    // gcov-jst 2 It is not practical to ensure coverage of this path during bench testing.
    outputValue = d_PWM_Definition[channel].maximumValue;
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_NON_CRITICAL, 3, (Uint32_t)(value * 1000.0f), 0, 0);
  }

  Float32_t regValue = (outputValue - d_PWM_Definition[channel].minimumValue) / valueSpan[channel];
  regValue = (regValue * OUTPUT_REG_SPAN) + MINIMUM_REG_VALUE + 0.05f;
  d_GEN_RegisterWrite(d_PWM_AddressDefinition[ioc].pwmOutputBaseAddress + (channel * 0x04u), (Uint32_t)regValue);
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_PWM_Input -->

  Read the PWM input
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_PWM_Input
(
const Uint32_t ioc,               /**< [in] IOC number */
const Uint32_t channel,           /**< [in] PWM channel */
Float32_t * const pValue          /**< [out] Value read */
)
{
  if (ioc >= d_PWM_CountIoc)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, ioc, 0, 0);
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

  if (channel >= d_PWM_CountChannel)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  Uint32_t regValue = d_GEN_RegisterRead(d_PWM_AddressDefinition[ioc].pwmInputBaseAddress + TIMER_OFFSET + (channel * 0x04u));
  if (regValue < MINIMUM_UINT_READ_VALUE)
  {
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    regValue = MINIMUM_UINT_READ_VALUE;
  }
  else if (regValue > MAXIMUM_UINT_READ_VALUE)
  {
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    regValue = MAXIMUM_UINT_READ_VALUE;
  }
  else
  {
    /* Value is in range */
  }
  
  regValue = regValue - MINIMUM_UINT_READ_VALUE;
  Float32_t fRegValue = (Float32_t)regValue;
  Float32_t value = fRegValue / (Float32_t)INPUT_REG_SPAN;
  *pValue = (value * valueSpan[channel]) + d_PWM_Definition[channel].minimumValue;

  return d_STATUS_SUCCESS;
}

