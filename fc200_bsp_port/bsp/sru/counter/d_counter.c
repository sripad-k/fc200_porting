/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Counter

  Abstract           : Counter interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-
                       SDD References: 136T-2200-131000-001-D22 SWDES-
\note
  CSC ID             : SWDES-
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"
#include "soc/discrete/d_discrete.h"
#include "soc/timer/d_timer.h"
#include "kernel/general/d_gen_register.h"
#include "d_counter.h"
#include "d_counter_cfg.h"

/***** Constants ********************************************************/

static const Uint32_t COUNTER_OFFSET = 0x40u;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

typedef struct ChannelValues_t
{
  Uint32_t lastReading;
  Uint32_t counter;
}
ChannelValues_t; /**< Used to maintain values per channel.*/

typedef struct IocChannelValues_t
{
  ChannelValues_t channelValues[d_COUNTER_MAX_CHANNELS];
}
IocChannelValues_t; /**< Used to maintain values relevant to each of the channels.*/

static IocChannelValues_t iocChannelValues[d_COUNTER_MAX_IOCS]; /**< Used to maintain values for each channel relevant to each of the IOCs.*/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_COUNTER_Initialise -->

  Initialise the Counter CSC.
*************************************************************************/
d_Status_t         /** \return Function status */
d_COUNTER_Initialise
(
void
)
{
  if (d_COUNTER_CountIoc > d_COUNTER_MAX_IOCS)
  {
		// gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 1, d_COUNTER_CountIoc, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (d_COUNTER_CountChannel > d_COUNTER_MAX_CHANNELS)
  {
		// gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 2, d_COUNTER_CountChannel, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  for (Uint32_t ioc = 0; ioc < d_COUNTER_CountIoc; ioc++)
  {
    for (Uint32_t channel = 0; channel < d_COUNTER_CountChannel; channel++)
    {
      iocChannelValues[ioc].channelValues[channel].lastReading = d_GEN_RegisterRead(d_COUNTER_BaseAddress[ioc] + COUNTER_OFFSET + (channel * 0x04u));
      iocChannelValues[ioc].channelValues[channel].counter = 0u;
    }
  }
  
  initialised = d_TRUE;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_COUNTER_Read -->

  Read a counter.
************************************************************************/
d_Status_t         /** \return Function status */
d_COUNTER_Read
(
const Uint32_t ioc,
const Uint32_t channel,
Uint32_t * const pValue
)
{
  if (ioc >= d_COUNTER_CountIoc)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, ioc, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (channel >= d_COUNTER_CountChannel)
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

  d_Status_t status = d_STATUS_SUCCESS;
  
  if (initialised == d_TRUE)
  {
    Uint32_t regValue = d_GEN_RegisterRead(d_COUNTER_BaseAddress[ioc] + COUNTER_OFFSET + (channel * 0x04u));
    Uint32_t increment;
    if (regValue >= iocChannelValues[ioc].channelValues[channel].lastReading)
    {
      increment = regValue - iocChannelValues[ioc].channelValues[channel].lastReading;
    }
    else
    {
      increment = regValue + 256u - iocChannelValues[ioc].channelValues[channel].lastReading;
    }
      
    iocChannelValues[ioc].channelValues[channel].lastReading = regValue;
    
    iocChannelValues[ioc].channelValues[channel].counter += increment;
    *pValue = iocChannelValues[ioc].channelValues[channel].counter;
  }
  else
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    status = d_STATUS_NOT_INITIALISED;
  }
  
  return status;
}



