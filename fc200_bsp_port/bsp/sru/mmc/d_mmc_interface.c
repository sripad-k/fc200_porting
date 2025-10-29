/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : MMC Memory Interface Header File

  Abstract           : Wrapper for the MMC Xilinx driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-270
                                                                SWREQ-271
                                                                SWREQ-272
                                                                SWREQ-273
                       SDD References: 136T-2200-131100-001-D22 SWDES-254
                                                                SWDES-255
                                                                SWDES-256
                                                                SWDES-257
                                                                SWDES-258
\note
  CSC ID             : SWDES-61
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/date_time/d_date_time.h"
#include "xparameters.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"

#include "xsdps.h"
#include "xsdps_core.h"

#include "d_mmc_interface.h"

/***** Constants ********************************************************/

#define SECTOR_SIZE    512u
#define BUFFER_SIZE    (SECTOR_SIZE / 2u)

#define MMC_READY  0x11111111u

/* NB Cache only used for single sector reads and writes */
#define CACHE_ENTRIES  8u

#define CMD35   0x2300u
#define CMD36   0x2400u
#define CMD38   0x2600u

/***** Type Definitions *************************************************/

typedef enum
{
  PATTERN_0 = 0,
  PATTERN_1
} Pattern_t;

typedef struct
{
  Bool_t used;
  Uint32_t sector;
  Uint64_t time;
  Uint8_t data[SECTOR_SIZE];
} sectorCache_t;
  
/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/* Instance of the MMC driver */
static d_MMC_Instance_t mmcInstance;

// cppcheck-suppress misra-c2012-8.9; Defining this large variable at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static  __attribute__((aligned(64))) Uint16_t buffer[BUFFER_SIZE];

static sectorCache_t sectorCache[CACHE_ENTRIES];

/***** Function Declarations ********************************************/

static void cacheFlush(void);
static Uint32_t getNextCacheEntry(const Uint32_t sector);
static Uint32_t getCacheEntry(const Uint32_t sector);
static d_Status_t SectorCheck(const Uint32_t sector, const Pattern_t pattern);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_MMC_Initialise -->

  Initialise the MMC interface.
  Can return a pointer to the MMC instance structure if required. Use of a
  pointer to a pointer here is the most effective way of integrating the BSP
  with the Xilinx SPI/MMC code.
*************************************************************************/
d_Status_t                          /** \return Function status */
d_MMC_Initialise
(
d_MMC_Instance_t ** ppMmcInstance   /**< [out] Return pointer to MMC instance structure. */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  
  if ((initialised != d_TRUE) || (mmcInstance.IsReady != MMC_READY))
  {
    Int32_t status;
    XSdPs_Config * pSdConfig;
    pSdConfig = XSdPs_LookupConfig(XPAR_XSDPS_0_DEVICE_ID);
    if (pSdConfig == NULL)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
    ELSE_DO_NOTHING
  
    if (returnValue == d_STATUS_SUCCESS)
    {
      /* Return pointer to instance structure */
      if (ppMmcInstance != NULL)
      {
        *ppMmcInstance = &mmcInstance;
      }
      // gcov-jst 1 It is not practical to cover both paths of the above if statement during bench testing.
      ELSE_DO_NOTHING

      // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
      status = XSdPs_CfgInitialize((XSdPs *)&mmcInstance, pSdConfig,
                                   pSdConfig->BaseAddress);
      if (status != XST_SUCCESS)
      {
        // gcov-jst 1 It is not practical to generate this failure during bench testing.
        returnValue = d_STATUS_FAILURE;
      }
      ELSE_DO_NOTHING
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING
  
    if (returnValue == d_STATUS_SUCCESS)
    {
      // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
      status = XSdPs_CardInitialize((XSdPs *)&mmcInstance);
      if (status == XST_SUCCESS)
      {
        initialised = d_TRUE;
      }
      else
      {
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        mmcInstance.IsReady = ~MMC_READY;
        returnValue = d_STATUS_FAILURE;
      }
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING
    
    /* Set all cache entries as unused */
    cacheFlush();
  }
  else
  {
    /* previously initialised, return pointer to instanc structure */
    if (ppMmcInstance != NULL)
    {
      *ppMmcInstance = &mmcInstance;
    }
    ELSE_DO_NOTHING
  }
  
  return returnValue;
}


/*********************************************************************//**
  <!-- d_MMC_SectorCount -->

  Read the number of sectors.
************************************************************************/
d_Status_t                          /** \return Function status */
d_MMC_SectorCount
(
Uint32_t * const pSectors           /**< [out] Return number of sectors */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pSectors == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  *pSectors = mmcInstance.SectorCount;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_MMC_Bit -->

  Perform a BIT test on the specified sector.
************************************************************************/
d_Status_t                          /** \return BIT result */
d_MMC_Bit
(
const Uint32_t sector               /**< [in] Sector to test */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (mmcInstance.IsReady != MMC_READY)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_DEVICE_NOT_READY, d_ERROR_CRITICALITY_UNKNOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_DEVICE_NOT_READY;
  }

  if (sector >= mmcInstance.SectorCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, sector, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  status = SectorCheck(sector, PATTERN_0);

  if (status == d_STATUS_SUCCESS)
  {
    status = SectorCheck(sector, PATTERN_1);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_MMC_SectorRead -->

  Read a sector from the MMC. This wrapper of the Xilinx code is a 
  blocking function and does not use interrupts.
  Note that the buffer must be 16 byte aligned for DMA access.
************************************************************************/
d_Status_t                          /** \return Function status */
d_MMC_SectorRead
(
const Uint32_t sector,              /**< [in] First sector number to read */
const Uint32_t count,               /**< [in] Number of sectors to read */
Uint8_t * const pBuffer             /**< [out] Pointer to storage for data read */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Bool_t done = d_FALSE;
  Uint32_t cacheEntry;
  
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (mmcInstance.IsReady != MMC_READY)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_DEVICE_NOT_READY, d_ERROR_CRITICALITY_UNKNOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_DEVICE_NOT_READY;
  }

  if (sector >= mmcInstance.SectorCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, sector, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((sector + count) > mmcInstance.SectorCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, count, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  // cppcheck-suppress misra-c2012-11.4; Conversion required to check for invalid parameter. Violation of 'Advisory' rule does not present a risk
  if (((Pointer_t)pBuffer & 0x0000000Fu) != 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (count == 1u)
  {
    cacheEntry = getCacheEntry(sector);
    if (cacheEntry < CACHE_ENTRIES)
    {
      d_GEN_MemoryCopy(pBuffer, &sectorCache[cacheEntry].data[0], SECTOR_SIZE);
      done = d_TRUE;
    }
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (done != d_TRUE)
  {
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    Int32_t status  = XSdPs_ReadPolled((XSdPs *)&mmcInstance, sector, count, pBuffer);
    if (status == XST_SUCCESS)
    {
      if (count == 1u)
      {
        cacheEntry = getNextCacheEntry(sector);
        d_GEN_MemoryCopy(&sectorCache[cacheEntry].data[0], pBuffer, SECTOR_SIZE);
        sectorCache[cacheEntry].sector = sector;
        sectorCache[cacheEntry].time = d_DATE_TIME_Timestamp();
        sectorCache[cacheEntry].used = d_TRUE;
      }
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      ELSE_DO_NOTHING
    }
    else
    {  
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
  }
  ELSE_DO_NOTHING
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_MMC_SectorWrite -->

  Write a sector to the MMC. This wrapper of the Xilinx code is a 
  blocking function and does not use interrupts.
  Note that the buffer must be 16 byte aligned for DMA access.
*************************************************************************/
d_Status_t                          /** \return Function status */
d_MMC_SectorWrite
(
const Uint32_t sector,              /**< [in] First sector number to write */
const Uint32_t count,               /**< [in] Number of sectors to write */
const Uint8_t * const pBuffer       /**< [in] Pointer to data to write */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (mmcInstance.IsReady != MMC_READY)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_DEVICE_NOT_READY, d_ERROR_CRITICALITY_UNKNOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_DEVICE_NOT_READY;
  }

  if (sector >= mmcInstance.SectorCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, sector, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((sector + count) > mmcInstance.SectorCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  // cppcheck-suppress misra-c2012-11.4; Conversion required to check for invalid parameter. Violation of 'Advisory' rule does not present a risk
  if (((Pointer_t)pBuffer & 0x0000000Fu) != 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  Int32_t status  = XSdPs_WritePolled((XSdPs *)&mmcInstance, sector, count, pBuffer);
  if (status == XST_SUCCESS)
  {
    if (count == 1u)
    {
      Uint32_t cacheEntry = getNextCacheEntry(sector);
      d_GEN_MemoryCopy(&sectorCache[cacheEntry].data[0], pBuffer, SECTOR_SIZE);
      sectorCache[cacheEntry].sector = sector;
      sectorCache[cacheEntry].time = d_DATE_TIME_Timestamp();
      sectorCache[cacheEntry].used = d_TRUE;
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_FAILURE;
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_MMC_Erase -->

  Erase the MMC.
************************************************************************/
d_Status_t                          /** \return Function status */
d_MMC_Erase
(
const Uint32_t sectorStart,         /**< [in] First sector number to erase */
const Uint32_t sectorEnd            /**< [in] Last sector number to erase */
)
{
  Int32_t Status;

  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Send CMD35 start erase command */
  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  Status = XSdPs_CmdTransfer((XSdPs *)&mmcInstance, CMD35, sectorStart, 0u);
  if (Status != XST_SUCCESS)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    /* Send CMD36 end erase command */
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    Status = XSdPs_CmdTransfer((XSdPs *)&mmcInstance, CMD36, sectorEnd, 0u);
    if (Status != XST_SUCCESS)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_FAILURE;
    }
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    /* Send CMD38 erase command with Trim option */
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    Status = XSdPs_CmdTransfer((XSdPs *)&mmcInstance, CMD38, 1u, 0u);
    if (Status != XST_SUCCESS)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_FAILURE;
    }
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  /* Ensure anything erased is not still in the cache */
  cacheFlush();

  return status;
}

/*********************************************************************//**
  <!-- cacheFlush -->

  Mark all cache entries as unused.
************************************************************************/
static void                         /** \return None */
cacheFlush
(
void
)
{
  Uint32_t index;
  
  for (index = 0; index < CACHE_ENTRIES; index++)
  {
    sectorCache[index].used = d_FALSE;
    sectorCache[index].time = 0;
  }
  
  return;
}

/*********************************************************************//**
  <!-- getNextCacheEntry -->

  Get empty or oldest cache entry to use next.
*************************************************************************/
static Uint32_t                     /** \return Cache entry number */ 
getNextCacheEntry
(
const Uint32_t sector               /**< [in] Sector number required */
)
{
  Uint32_t index;
  Uint32_t latestIndex = 0;
  Bool_t found = d_FALSE;

  /* Check if there is already an entry for this sector */
  index = 0;
  while ((index < CACHE_ENTRIES) && (found == d_FALSE))
  {
    if (sectorCache[index].sector == sector)
    {
      found = d_TRUE;
      latestIndex = index;
    }
    else
    {
      index++;
    }
  }

  /* Check for an unused entry */
  index = 0;
  while ((index < CACHE_ENTRIES) && (found == d_FALSE))
  {
    if (sectorCache[index].used == d_FALSE)
    {
      found = d_TRUE;
      latestIndex = index;
    }
    else
    {
      index++;
    }
  }

  /* Check for the oldest entry */
  if (found == d_FALSE)
  {
    Uint32_t latestTime = 0xFFFFFFFFu;
    for (index = 0; index < CACHE_ENTRIES; index++)
    {
      /* Note empty entries have time of zero so they will be oldest */
      if (sectorCache[index].time < latestTime)
      {
        latestTime = sectorCache[index].time;
        latestIndex = index;
      }
      else
      {
        DO_NOTHING();
      }
    }
  }

  return latestIndex;
}

/*********************************************************************//**
  <!-- getCacheEntry -->

  Get cache entry containing specified sector.
*************************************************************************/
static Uint32_t                     /** \return Cache entry number */
getCacheEntry
(
const Uint32_t sector               /**< [in] Sector number to find */
)
{
  Uint32_t index = 0;
  Bool_t found = d_FALSE;

  do
  {
    if ((sectorCache[index].used == d_TRUE) && (sectorCache[index].sector == sector))
    {
      found = d_TRUE;
    }
    else
    {
      index++;
    }
  } while ((found == d_FALSE) && (index < CACHE_ENTRIES));
  
  return index;
}

/*********************************************************************//**
  <!-- SectorCheck -->

  Write / Read sector test.
*************************************************************************/
static d_Status_t                   /** \return BIT result */
SectorCheck
(
const Uint32_t sector,              /**< [in] Sector to test */
const Pattern_t pattern             /**< [in] Pattern to write */
)
{
  Uint32_t index;
  d_Status_t status;
  d_Status_t returnValue = d_STATUS_SUCCESS;

  /* Write a sector with specified pattern */
  for (index = 0; index < BUFFER_SIZE; index++)
  {
    if (pattern == PATTERN_0)
    {
      buffer[index] = (Uint16_t)(index * 3u);
    }
    else
    {
      buffer[index] = (Uint16_t)~(index * 3u);
    }
  }
  
  status = d_MMC_SectorWrite(sector, 1, (Uint8_t *)buffer);

  if (status == d_STATUS_SUCCESS)
  {    
    /* Remove from cache */
    cacheFlush();
    /* Read back the sector */
    d_GEN_MemorySet((Uint8_t *)buffer, 0, SECTOR_SIZE);
    status = d_MMC_SectorRead(sector, 1, (Uint8_t *)buffer);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  /* Verify the sector */
  if (status == d_STATUS_SUCCESS)
  {    
    for (index = 0; index < BUFFER_SIZE; index++)
    {
      if (((pattern == PATTERN_0) && (buffer[index] != (Uint16_t)(index * 3u))) ||
          ((pattern == PATTERN_1) && (buffer[index] != (Uint16_t)~(index * 3u))))
      {
        // gcov-jst 1 It is not practical to generate this failure during bench testing.
        returnValue = d_STATUS_BIT_FAILURE;
      }
      ELSE_DO_NOTHING
    }
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status != d_STATUS_SUCCESS)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_BIT_FAILURE;
  }
  ELSE_DO_NOTHING
  
  return returnValue;
}

