/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger_mmc

  Abstract           : Event Logger driver module for the MMC Flash

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
#include "d_event_logger.h"
#include "d_event_logger_cfg.h"
#include "d_event_logger_mmc.h"
#include "d_event_logger_mmc_event.h"
#include "kernel/buffer/d_buffer.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/crc32/d_crc32.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* Event allocation table */
typedef struct
{
  Uint32_t maximumEntries;
  Uint32_t entrySize;
  Uint32_t startSector;
  Uint32_t entryCount;
  Uint32_t currentSectorNumber;
  Uint32_t currentSectorIndex;
  Uint32_t crc;
} d_EVENT_AllocationEvent_t;

typedef struct
{
  d_EVENT_AllocationEvent_t allocation;
  Uint8_t fill[MMC_SECTOR_SIZE - sizeof(d_EVENT_AllocationEvent_t)];
} allocationEventSector_t;

/***** Variables ********************************************************/

Uint32_t missedEvents;

/* Sector containing the allocation table */
static __attribute__((aligned(32))) allocationEventSector_t allocationEventSector;

/* Current sector which may have data appended */
static __attribute__((aligned(32))) Uint8_t eventCurrentSector[MMC_SECTOR_SIZE];

/* Temperary sector */
static __attribute__((aligned(32))) Uint8_t tempSector[MMC_SECTOR_SIZE];

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

static Bool_t ReadAllocationTableAndVerify(const Uint32_t sector, d_EVENT_AllocationEvent_t * allocation);

static void writeEventAllocationTable(void);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- EventMmcInitialise -->

  Initialise the event log on the MMC
*************************************************************************/
d_Status_t             /** \return Success or Failure */
EventMmcInitialise
(
void
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  d_EVENT_AllocationEvent_t alloc_1;
  d_EVENT_AllocationEvent_t alloc_2;
  Bool_t allocValid_1;
  Bool_t allocValid_2;
  
  /* Clear the unused portion of the allocation sector */
  d_GEN_MemorySet((Uint8_t *)&allocationEventSector.fill[0], 0xFFu, sizeof(allocationEventSector.fill));

  allocValid_1 = ReadAllocationTableAndVerify(LOG_EVENT_ALLOC_SECTOR_1, &alloc_1);
  allocValid_2 = ReadAllocationTableAndVerify(LOG_EVENT_ALLOC_SECTOR_2, &alloc_2);

  if ((allocValid_1 == d_FALSE) && (allocValid_2 == d_FALSE))
  {
    /* Set defaults */
    allocationEventSector.allocation.maximumEntries = d_EVENT_MAX_EVENT_ENTRIES;
    allocationEventSector.allocation.entrySize = sizeof(d_EVENT_LogEvent_t);
    allocationEventSector.allocation.startSector = LOG_EVENT_START_SECTOR;
    allocationEventSector.allocation.entryCount = 0;
    allocationEventSector.allocation.currentSectorNumber = LOG_EVENT_START_SECTOR;
    allocationEventSector.allocation.currentSectorIndex = 0;
    writeEventAllocationTable();
    returnValue = d_STATUS_SUCCESS;
  }
  else if ((allocValid_2 == d_TRUE) && ((allocValid_1 == d_FALSE) || (alloc_2.entryCount > alloc_1.entryCount)))
  {
    d_GEN_MemoryCopy((Uint8_t *)&allocationEventSector.allocation, (Uint8_t *)&alloc_2, sizeof(d_EVENT_AllocationEvent_t));
    writeEventAllocationTable();
  }
  else
  {
    d_GEN_MemoryCopy((Uint8_t *)&allocationEventSector.allocation, (Uint8_t *)&alloc_1, sizeof(d_EVENT_AllocationEvent_t));
  }

  /* If part way through writing a sector, then read current state */
  if (allocationEventSector.allocation.currentSectorIndex > 0u)
  {
    returnValue = d_MMC_SectorRead(allocationEventSector.allocation.currentSectorNumber, 1, &eventCurrentSector[0]);
  }
  else
  {
    DO_NOTHING();
  }

  missedEvents = 0;
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    initialised = d_TRUE;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- EventMmcCount -->

  Read the number of logged events.
*************************************************************************/
d_Status_t               /** \return Success or Failure */
EventMmcCount
(
Uint32_t * const pCount  /**< [out] Pointer to storage for number of events */
)
{
  *pCount = allocationEventSector.allocation.entryCount;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- EventMmcClear -->

  Clear the event log on the MMC
*************************************************************************/
d_Status_t             /** \return Success or Failure */
EventMmcClear
(
void
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  
  /* Set defaults */
  allocationEventSector.allocation.maximumEntries = d_EVENT_MAX_EVENT_ENTRIES;
  allocationEventSector.allocation.entrySize = sizeof(d_EVENT_LogEvent_t);
  allocationEventSector.allocation.startSector = LOG_EVENT_START_SECTOR;
  allocationEventSector.allocation.entryCount = 0;
  allocationEventSector.allocation.currentSectorNumber = LOG_EVENT_START_SECTOR;
  allocationEventSector.allocation.currentSectorIndex = 0;
  writeEventAllocationTable();

  missedEvents = 0;
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_EVENT_ReadEventMmc -->

  Read an event from the MMC
*************************************************************************/
d_Status_t                          /** \return Success or Failure */
d_EVENT_ReadEventMmc
(
const Uint32_t entryNumber,         /**< [in]  Event number to read */
d_EVENT_LogEvent_t * const pEvent   /**< [out] Pointer to storage for the event */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t entry;
  
  entry = entryNumber;
  if (entry == LAST_LOGGED_EVENT)
  {
    /* Get last logged entry number. This will give -1 if nothing logged which will give parameter error */
    entry = allocationEventSector.allocation.entryCount - 1u;
  }
  else
  {
    DO_NOTHING();
  }
  
  /* Check the parameters */
  if (entry >= allocationEventSector.allocation.entryCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1, entry, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pEvent == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t offset = entry * sizeof(d_EVENT_LogEvent_t);
    Uint32_t sector = LOG_EVENT_START_SECTOR + (offset / MMC_SECTOR_SIZE);
    offset = offset % MMC_SECTOR_SIZE;
    Uint32_t bytesToRead = sizeof(d_EVENT_LogEvent_t);
    if (bytesToRead > (MMC_SECTOR_SIZE - offset))
    {
      bytesToRead = MMC_SECTOR_SIZE - offset;
      returnValue = d_MMC_SectorRead(sector, 1, &tempSector[0]);
      d_GEN_MemoryCopy((Uint8_t *)pEvent, (Uint8_t *)&tempSector[offset], bytesToRead);
      bytesToRead = sizeof(d_EVENT_LogEvent_t) - bytesToRead;
      sector++;
      offset = 0;
    }
    else
    {
      DO_NOTHING();
    }

    if ((bytesToRead > 0u) && (returnValue == d_STATUS_SUCCESS))
    {
      returnValue = d_MMC_SectorRead(sector, 1, &tempSector[0]);
      // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
      d_GEN_MemoryCopy((Uint8_t *)((Uint32_t)pEvent + sizeof(d_EVENT_LogEvent_t) - bytesToRead), &tempSector[offset], bytesToRead);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }
  return returnValue;
}

/*********************************************************************//**
  <!-- ProcessMmcEvent -->

  Background write of event data to MMC
*************************************************************************/
void         /** \return None */
ProcessMmcEvent
(
void
)
{
  d_Status_t status;
  d_EVENT_LogEvent_t event;
  Uint32_t count;
  Bool_t done = d_FALSE;
  Bool_t updated = d_FALSE;
  
  if (initialised == d_TRUE)
  {
    while (done == d_FALSE)
    {
      status = d_BUFFER_FixedRead(d_EVENT_Fixed_Events_Buffer_Index, (Uint8_t *)&event, 1, &count);
      if ((status == d_STATUS_SUCCESS) && (count >= 1u))
      {
        Uint32_t bytesToWrite = sizeof(d_EVENT_LogEvent_t);
        if (bytesToWrite > (MMC_SECTOR_SIZE - allocationEventSector.allocation.currentSectorIndex))
        {
          bytesToWrite = MMC_SECTOR_SIZE - allocationEventSector.allocation.currentSectorIndex;
          d_GEN_MemoryCopy(&eventCurrentSector[allocationEventSector.allocation.currentSectorIndex], (Uint8_t *)&event, bytesToWrite);
          /* No point in checking if write successful as cannot log anything if it fails */
          (void)d_MMC_SectorWrite(allocationEventSector.allocation.currentSectorNumber, 1, &eventCurrentSector[0]);
          bytesToWrite = sizeof(d_EVENT_LogEvent_t) - bytesToWrite;
          allocationEventSector.allocation.currentSectorNumber++;
          allocationEventSector.allocation.currentSectorIndex = 0;
        }
        ELSE_DO_NOTHING
        
        if (bytesToWrite > 0u)
        {
          // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
          d_GEN_MemoryCopy(&eventCurrentSector[allocationEventSector.allocation.currentSectorIndex], (Uint8_t *)((Uint32_t)&event + sizeof(d_EVENT_LogEvent_t) - bytesToWrite), bytesToWrite);
          /* No point in checking if write successful as cannot log anything if it fails */
          (void)d_MMC_SectorWrite(allocationEventSector.allocation.currentSectorNumber, 1, &eventCurrentSector[0]);
          allocationEventSector.allocation.currentSectorIndex += bytesToWrite;
          if (allocationEventSector.allocation.currentSectorIndex >= MMC_SECTOR_SIZE)
          {
            allocationEventSector.allocation.currentSectorNumber++;
            allocationEventSector.allocation.currentSectorIndex = 0;
          }
        }
        // gcov-jst 1 It is not practical to execute this code during bench testing.
        ELSE_DO_NOTHING
        
        allocationEventSector.allocation.entryCount++;
        updated = d_TRUE;
      }
      else
      {
        done = d_TRUE;
      }
    } /* end "while (done == d_FALSE)" */

    /* Add event if any events were missed due to buffer queue being full */
    if (missedEvents > 0u)
    {
      /* At this stage buffer will just have been emptied */
      (void)d_EVENT_Log_Event(d_EVENT_NON_CRITICAL, (const Char_t *)"Missed events due to buffer full", 32u, missedEvents);
      missedEvents = 0u;
    }
    ELSE_DO_NOTHING

    if (updated == d_TRUE)
    {
      writeEventAllocationTable();
    }
    ELSE_DO_NOTHING
    
  } /* end "if (initialisd == d_TRUE)" */
  
  return;
}

/*********************************************************************//**
  <!-- ReadAllocationTableAndVerify -->

  Read the event allocation table and verify CRC
*************************************************************************/
static Bool_t                                   /** \return Allocation table validity */
ReadAllocationTableAndVerify
(
const Uint32_t sector,                          /**< [in]  Sector number containing the allocation table */
d_EVENT_AllocationEvent_t * const pAllocation   /**< [out] Pointer to storage for allocation table */
)
{
  Bool_t returnValue = d_FALSE;
	
  /* Read the allocation table */
  d_Status_t status = d_MMC_SectorRead(sector, 1, &tempSector[0]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Copy to local structure */
    d_GEN_MemoryCopy((Uint8_t *)pAllocation, &tempSector[0], sizeof(d_EVENT_AllocationEvent_t));
    /* Check CRC */
    if (d_CRC32_Calculate((Uint8_t *)pAllocation, sizeof(d_EVENT_AllocationEvent_t) - 4u) == pAllocation->crc)
    {
      returnValue = d_TRUE;
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

  return returnValue;
}

/*********************************************************************//**
  <!-- writeEventAllocationTable -->

  Write the event allocation table to the MMC
*************************************************************************/
static void         /** \return None */
writeEventAllocationTable
(
void
)
{
  allocationEventSector.allocation.crc = d_CRC32_Calculate((Uint8_t *)&allocationEventSector.allocation, sizeof(d_EVENT_AllocationEvent_t) - 4u);
  (void)d_MMC_SectorWrite(LOG_EVENT_ALLOC_SECTOR_1, 1, (Uint8_t *)&allocationEventSector.allocation);
  (void)d_MMC_SectorWrite(LOG_EVENT_ALLOC_SECTOR_2, 1, (Uint8_t *)&allocationEventSector.allocation);

  return;
}

