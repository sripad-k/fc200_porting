/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger

  Abstract           : Event Logger driver module

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-72
                                                                SWREQ-73
                       SDD References: 136T-2200-131000-001-D22 SWDES-221
                                                                SWDES-222
                                                                SWDES-223
                                                                SWDES-224
                                                                SWDES-226
                                                                SWDES-227
                                                                SWDES-228
                                                                SWDES-229
                                                                SWDES-230
                                                                SWDES-231
\note
  CSC ID             : SWDES-77
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "d_event_logger.h"
#include "d_event_logger_cfg.h"
#include "d_event_logger_mmc_event.h"
#include "d_event_logger_mmc.h"
#include "kernel/buffer/d_buffer.h"
#include "kernel/date_time/d_date_time.h"
#include "kernel/crc32/d_crc32.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static Uint32_t eventNumber;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_EVENT_Initialise -->

  Initialise the Event Logger CSC
*************************************************************************/
d_Status_t             /** \return Success or Failure */
d_EVENT_Initialise
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_BUFFER_FixedInitialise();
  
    status = InitialiseMmc();
  
    if ((status == d_STATUS_SUCCESS) && (d_EVENT_LogCriticalInitialise != NULL))
    {
      status = d_EVENT_LogCriticalInitialise(d_EVENT_LogCriticalDevice);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
    
    /* Always start at event zero at startup */
    eventNumber = 0;

    if (status == d_STATUS_SUCCESS)
    {
	    initialised = d_TRUE;
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }
  else
  {
    DO_NOTHING();
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_EVENT_Count -->

  Read the number of logged events
*************************************************************************/
d_Status_t
d_EVENT_Count
(
Uint32_t * const pCount
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  /* Check the parameters */
  if (pCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  d_Status_t status = EventMmcCount(pCount); 
  
  return status;
 }

/*********************************************************************//**
  <!-- d_EVENT_Clear -->

  Clear events in MMC
*************************************************************************/
d_Status_t             /** \return Success or Failure */
d_EVENT_Clear
(
void
)
{
  d_Status_t status = EventMmcClear();

  eventNumber = 0;

  return status;
}

/*********************************************************************//**
  <!-- d_EVENT_Log_Event -->

  Log an event
*************************************************************************/
d_Status_t             /** \return Success or Failure */
d_EVENT_Log_Event
(
const d_EVENT_EventCode_t eventCode,   /**< [in] Event code */
const Char_t * const pTextField,       /**< [in] Event text message */
const Uint32_t textLength,             /**< [in] Length of text message */
const Uint32_t eventValue              /**< [in] Event value to be stored */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  d_EVENT_LogEvent_t event;
  Uint32_t count;
  
  /* Check the parameters */
  if (pTextField == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if ((textLength == 0u) || (textLength > TEXT_FIELD_SIZE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 3, textLength, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (initialised == d_TRUE)
  {
    /* Construct the log entry */
    event.eventNumber = eventNumber;
    event.timestamp = d_DATE_TIME_Timestamp();
    event.eventCode = eventCode;
    d_GEN_MemoryCopy(&event.textField[0], (const Uint8_t *)pTextField, textLength);
    event.textLength = textLength;
    event.eventValue = eventValue;
    // cppcheck-suppress misra-c2012-11.4; Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    event.crc =  d_CRC32_Calculate((Uint8_t *)&event, (Uint32_t)&event.crc - (Uint32_t)&event);

    /* Store in buffer queue */
    if (eventCode == d_EVENT_CRITICAL)
    {
      if (d_EVENT_LogCritical != NULL)
      {
        /* Write straight to Flash with critical errors */
        returnValue = d_EVENT_LogCritical(&event);
      }
      else
      {
        // gcov-jst 1 It is not practical to execute this code during bench testing.
        DO_NOTHING();
      }
    }
    else
    {
      returnValue = d_BUFFER_FixedWrite(d_EVENT_Fixed_Events_Buffer_Index, (Uint8_t *)&event, 1, &count);
      if (returnValue == d_STATUS_BUFFER_FULL)
      {
        missedEvents++;
      }
      else
      {
        DO_NOTHING();
      }
      eventNumber++;
    }
  }
  else
  {
    /* Unable to process event, ignore */
    returnValue = d_STATUS_NOT_INITIALISED;
  }
  
  return returnValue;
}

