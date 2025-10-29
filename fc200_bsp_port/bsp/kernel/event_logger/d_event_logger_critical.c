/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger_critical.c

  Abstract           : Event Logger for critical errors

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

/* Critical log entries are written to the MAC Flash staring at address 0,
   The last 16 sectors are used for testing thus there are 496 sectors
   available. Each entry uses 256 bytes, although the entries themselves
   occupy less space. The 256 byte alignment is used to ensure all writes
   can be written with a single page write command.
   If the log is full, at initialisation or during normal operation, then
   a non-critical error is logged and the initialised state set to FALSE
   to prevent further writes. */
   
/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/timer/d_timer.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/crc32/d_crc32.h"
#include "sru/flash_mac/d_flash_mac.h"
#include "sru/flash_mac/d_flash_mac_cfg.h"
#include "d_event_logger.h"
#include "d_event_logger_cfg.h"
#include "d_event_logger_critical.h"

/***** Constants ********************************************************/

#define TEST_SECTORS     16u

/* Entry size set to 256 so entries do not span across page writes and therefore only need one command to write */
#define ENTRY_SIZE        256u

#define MAX_ENTRIES (Uint32_t)((d_FLASH_MAC_SIZE / ENTRY_SIZE) - (TEST_SECTORS * (d_FLASH_MAC_SECTOR_SIZE / ENTRY_SIZE)))

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static Uint32_t eventNumber;

static Uint32_t flashMacDevice;

static Uint32_t logEntryCount;
static Uint32_t nextWriteAddress;

// cppcheck-suppress misra-c2012-8.9; Defining this large variable at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Uint8_t buffer[ENTRY_SIZE];

/***** Function Declarations ********************************************/

static d_Status_t waitNotBusy(const Uint32_t timeout);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_EVENT_CriticalInitialise -->

  Initialise the Event Logger CSC
*************************************************************************/
d_Status_t                   /** \return Success or Failure */
d_EVENT_CriticalInitialise
(
const Uint32_t device        /**> [in] Flash MAC device number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  flashMacDevice = device;
  
  status = d_FLASH_MAC_Initialise();
    
  if (status == d_STATUS_SUCCESS)
  {
    eventNumber = 0u;
    /* Scan through looking for the first empty entry */
    logEntryCount = 0u;
    nextWriteAddress = 0u;
    Bool_t found = d_FALSE;
    do
    {
      status = d_FLASH_MAC_Read(flashMacDevice,
                                nextWriteAddress,
                                sizeof(buffer),
                                &buffer[0],
                                sizeof(buffer));
      if (status == d_STATUS_SUCCESS)
      {
        /* Check for empty entry */
        found = d_TRUE;
        for (Uint32_t index = 0; index < sizeof(buffer); index++)
        {
          if (buffer[index] != 0xFFu)
          {
            found = d_FALSE;
          }
          ELSE_DO_NOTHING
        }
        
        if (found != d_TRUE)
        {
          /* Not empty, go onto next entry */
          logEntryCount++;
          nextWriteAddress += ENTRY_SIZE;
          if (logEntryCount >= MAX_ENTRIES)
          {
            // gcov-jst 1 It is not practical to generate this failure during bench testing.
            d_ERROR_Logger(d_STATUS_BUFFER_FULL, d_ERROR_CRITICALITY_NON_CRITICAL, logEntryCount, MAX_ENTRIES, 0, 0);
            /* Do not return failure status else all error logging is disabled */
          }
          ELSE_DO_NOTHING
        }
        ELSE_DO_NOTHING
      }
      
    } while ((status == d_STATUS_SUCCESS) && (found != d_TRUE));
  }
  
  if (status == d_STATUS_SUCCESS)
  {
    (void)d_FLASH_MAC_Unlock(flashMacDevice);
    initialised = d_TRUE;
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  return status;
}

/*********************************************************************//**
  <!-- d_EVENT_Critical -->

  Log a critical error.
  There is no point in attempting to log errors here.
*************************************************************************/
d_Status_t                   /** \return Success or Failure */
d_EVENT_Critical
(
d_EVENT_LogEvent_t * const pEvent  /**< [in] Critical error to be logged */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (pEvent == NULL)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  pEvent->eventNumber = eventNumber;
  
  status = d_FLASH_MAC_Write(flashMacDevice,
                             nextWriteAddress,
                             sizeof(d_EVENT_LogEvent_t),
                             (const Uint8_t *)pEvent,
                             sizeof(d_EVENT_LogEvent_t));

  eventNumber++;
  
  if (status == d_STATUS_SUCCESS)
  {
    status = waitNotBusy(10);

    logEntryCount++;
    nextWriteAddress += ENTRY_SIZE;
    if (logEntryCount >= MAX_ENTRIES)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_BUFFER_FULL, d_ERROR_CRITICALITY_NON_CRITICAL, logEntryCount, MAX_ENTRIES, 0, 0);
      status = d_STATUS_BUFFER_FULL;
      initialised = d_FALSE;
    }
    else
    {
      DO_NOTHING();
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_EVENT_CriticalCount -->

  Get the number of logged critical errors.
*************************************************************************/
d_Status_t                   /** \return Success or Failure */
d_EVENT_CriticalCount
(
Uint32_t * const pCount      /**< [out] Storage for error count */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  *pCount = logEntryCount;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_EVENT_CriticalClear -->

  Clear the critical error log.
*************************************************************************/
d_Status_t                   /** \return Success or Failure */
d_EVENT_CriticalClear
(
void
)
{
  d_Status_t status;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  status = d_FLASH_MAC_EraseDevice(flashMacDevice);
  if (status == d_STATUS_SUCCESS)
  {
    status = waitNotBusy(200);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  if (status == d_STATUS_SUCCESS)
  {
    eventNumber = 0u;
    logEntryCount = 0u;
    nextWriteAddress = 0u;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_EVENT_ReadEventCritical -->

  Read a critical error
*************************************************************************/
d_Status_t                   /** \return Success or Failure */
d_EVENT_CriticalRead
(
const Uint32_t entryNumber,        /**< [in]  Event number to read */
d_EVENT_LogEvent_t * const pEvent  /**< [out] Pointer to storage for the event */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t entry;
  
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  entry = entryNumber;
  if (entry == LAST_LOGGED_EVENT)
  {
    /* Get last logged entry number. This will give -1 if nothing logged which will give parameter error */
    entry = logEntryCount - 1u;
  }
  else
  {
    DO_NOTHING();
  }
  
  /* Check the parameters */
  if (entry >= logEntryCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_NON_CRITICAL, 1, entry, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (pEvent == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  Uint32_t address = entry * ENTRY_SIZE;

  returnValue = d_FLASH_MAC_Read(flashMacDevice,
                                 address,
                                 sizeof(d_EVENT_LogEvent_t),
                                 (Uint8_t *)pEvent,
                                 sizeof(d_EVENT_LogEvent_t));

  return returnValue;
}

/*********************************************************************//**
  <!-- waitNotBusy -->

  Wait until the device is not busy.
*************************************************************************/
static d_Status_t waitNotBusy(const Uint32_t timeout)
{
  d_Status_t status;
  Uint32_t startTime;
  Uint32_t elapsedTime;

  startTime = d_TIMER_ReadValueInTicks();
  do
  {
    elapsedTime = d_TIMER_ElapsedMilliseconds(startTime, NULL);
  	status = d_FLASH_MAC_CheckFlashNotBusy(flashMacDevice);
  } while ((elapsedTime < timeout) && (status != d_STATUS_SUCCESS));

  return status;
}


