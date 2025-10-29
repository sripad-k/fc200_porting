/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_sata_ops

  Abstract           : SOC SATA driver operations file

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_register.h"        /* Register functions */
#include "kernel/general/d_gen_memory.h"
#include "soc/memory_manager/d_memory_cache.h"
#include "soc/timer/d_timer.h"

#include "d_sata_ops.h"



/***** Macros ***********************************************************/

#define AtaIdHasLba(id)		((id)[49u] & (Uint16_t)((Uint16_t)1u << 9u))

#define AtaIdU32(id,n)	\
	(((u32) (id)[(n) + 1u] << 16u) | ((u32) (id)[(n)]))

#define AtaIdU64(id,n)	\
	( ((Uint64_t) (id)[(n) + 3u] << 48u) | \
	  ((Uint64_t) (id)[(n) + 2u] << 32u) | \
	  ((Uint64_t) (id)[(n) + 1u] << 16u) | \
	  ((Uint64_t) (id)[(n) + 0u]) )


#define SWAP16(x,n) ((((Uint16_t)(x)[(n) + 0u]) << 8u) | \
                    (((Uint16_t)(x)[(n) + 1u])))


#define SWAP32(x,n) ((((Uint32_t)(x)[(n) + 0u]) << 24u) | \
                    (((Uint32_t)(x)[(n) + 1u]) << 16u) | \
                    (((Uint32_t)(x)[(n) + 2u]) << 8u) | \
                    (((Uint32_t)(x)[(n) + 3u])))

/***** Constants ********************************************************/

#define FC_TPER		  0x0001
#define FC_LOCKING    0x0002
#define FC_GEOMETRY   0x0003
#define FC_ENTERPRISE 0x0100
#define FC_DATASTORE  0x0202
#define FC_SINGLEUSER 0x0201
#define FC_OPALV100   0x0200
#define FC_OPALV200   0x0203

static const Uint32_t ATA_ID_LBA48_SECTORS	= 100u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SATA_PORT_COUNT = 2u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Char_t DEFAULT_PASSWORD[] = "MSIDMSIDMSIDMSID";

/***** Type Definitions *************************************************/

typedef struct {
    Uint16_t featureCode; /* 0x0002 in 2.00.100 */
    Uint8_t reserved_v : 4;
    Uint8_t version : 4;
    Uint8_t length;

    Uint8_t lockingSupported : 1;
    Uint8_t lockingEnabled : 1;
    Uint8_t locked : 1;
    Uint8_t mediaEncryption : 1;
    Uint8_t MBREnabled : 1;
    Uint8_t MBRDone : 1;
    Uint8_t reserved01 : 1;
    Uint8_t reserved02 : 1;

    Uint32_t reserved03;
    Uint32_t reserved04;
    Uint32_t reserved05;
} Discovery0LockingFeatures;

/***** Variables ********************************************************/

AhciCmdInfo_t cmdInfo;

static Uint16_t __attribute__ ((aligned (1024))) readBuff[1024u];

Uint8_t __attribute__ ((aligned (1024))) dmaMem[AHCI_PORT_DMA_SIZE]; // DMA memory must be 1K-byte aligned

d_SATA_DeviceInfo_t d_SATA_DeviceInfo;

/***** Function Declarations ********************************************/  

static d_Status_t d_SATAOP_EncTerminateSession(const Uint32_t port);
static d_Status_t d_SATAOP_EncStartSession(const Uint32_t port, const Char_t * password, const Uint32_t pwdLength, const d_SATAENC_uidPair_t uidPair);
static d_Status_t d_SATAOP_SetSIDPassword(const Uint32_t port, const Char_t * const password, const Uint32_t pwdLength);
static d_Status_t d_SATAOP_SetLockingRange(const Uint32_t port, const Bool_t lock);
static d_Status_t d_SATAOP_RevertLockingSP(const Uint32_t port);
static d_Status_t d_SATAOP_ConfigureLockingRange(const Uint32_t port, const Bool_t lockingEnabled);
static d_Status_t d_SATAOP_EncActivateLockingSP(const Uint32_t port);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SATA_RegisterRead -->

  Reads a register value from the SATA controller

*************************************************************************/
Uint32_t                      /** \return The contents of the register */
d_SATA_RegisterRead
(Uint32_t offset)             /**< [in] The offset of the register to read */
{
  return d_GEN_RegisterRead(XPAR_PSU_SATA_S_AXI_BASEADDR + offset);
}

/*********************************************************************//**
  <!-- d_SATA_RegisterWrite -->

  Write a register value in the SATA controller

*************************************************************************/
void                    /** \return None */
d_SATA_RegisterWrite
(
Uint32_t offset,        /**< [in] The offset of the register to write */
Uint32_t value          /**< [in] The value to write to the register */
)
{
  d_GEN_RegisterWrite(XPAR_PSU_SATA_S_AXI_BASEADDR + offset, value);
}

/*********************************************************************//**
  <!-- AhciFillCmdSlot -->

  Fills the AHCI Command slot

*************************************************************************/
static void              /** \return None */
AhciFillCmdSlot
(Uint32_t opts)         /**< [in] options to be set in the command slot */
{
  cmdInfo.cmdSlot->opts = opts;
  cmdInfo.cmdSlot->status = 0u;
  Uint64_t addr = (Uint64_t)cmdInfo.cmdTbl;
  cmdInfo.cmdSlot->tbl_addr = (Uint32_t)addr;
  cmdInfo.cmdSlot->tbl_addr_hi = (Uint32_t)(addr >> 32u);
}

/*********************************************************************//**
  <!-- AhciFillSg -->

  Fills the AHCI scatter-gather table

*************************************************************************/
static Uint32_t                    /** \return The amount of entries in the SG table */
AhciFillSg
(
const Uint8_t * const pBuffer,              /**< [Out] buffer to write the response */
const Uint32_t bufferSize)      /**< [in] size of the buffer */
{
  const Uint32_t MAX_DATA_BYTE_COUNT =  (4u*1024u*1024u);

  Uint32_t buffLength = bufferSize;
  Uint32_t sgCount = ((buffLength - 1u) / MAX_DATA_BYTE_COUNT) + 1u;
  AhciSg_t * ahciSg = cmdInfo.cmdTblSg;
  for (Uint32_t i = 0u; i < sgCount; i++)
  {
    // cppcheck-suppress misra-c2012-11.4; Conversion between pointer and integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    Uint32_t tmpAddr = (Uint32_t)&pBuffer[MAX_DATA_BYTE_COUNT * i]; // First cast pointer to 32bit integer
    Uint64_t addr = (Uint64_t)tmpAddr; // now cast 32 bit int to 64bit int
    ahciSg->addr = (Uint32_t)addr;
    ahciSg->addr_hi = (Uint32_t)(addr >> 32u);
    
    Uint32_t flagSize = (MAX_DATA_BYTE_COUNT - 1u);
    if (buffLength < MAX_DATA_BYTE_COUNT)
    {
      flagSize = buffLength - 1u;
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    ahciSg->flags_size = (0x3fffffu & flagSize);

    ahciSg++;
    buffLength -= MAX_DATA_BYTE_COUNT;
  }

  return sgCount;
}

/*********************************************************************//**
  <!-- AhciDataIo -->

  IO operation for AHCI port

*************************************************************************/
static d_Status_t               /** \return 0 upon success, error code on failure */
AhciDataIo
(const Uint8_t * const pFis,    /**< [in] pointer to the FIS array */
const Uint32_t fisLength,       /**< [in] length of the FIS array */
const Uint8_t * const pBuffer,              /**< [Out] buffer to write the response */
const Uint32_t bufferSize,      /**< [in] size of the buffer */
const Bool_t isWrite,           /**< [in] Transaction read/write flag */
const Uint32_t port)            /**< [in] Sata port to use */
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  
  // cppcheck-suppress misra-c2012-11.4; Conversion between pointer and integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
  d_GEN_MemoryCopy((Uint8_t*)cmdInfo.cmdTbl, pFis, fisLength);

  Uint32_t sgCount = 0u;
  if (pBuffer != NULL)
  {
    sgCount = AhciFillSg(pBuffer, bufferSize);
  }
  ELSE_DO_NOTHING

  Uint32_t opts = (fisLength >> 2u) | (sgCount << 16u);
  if (isWrite == d_TRUE)
  {
    opts |= ((Uint32_t)1u << 6u);
  }
  ELSE_DO_NOTHING

  AhciFillCmdSlot(opts);

  // Ensure data for SATA controller is flushed out of dcache and written to physical memory.
  // cppcheck-suppress misra-c2012-11.4; Conversion from pointer to integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
  d_MEMORY_DCacheFlushRange((Pointer_t)cmdInfo.cmdSlot, AHCI_PORT_DMA_SIZE);
  if (pBuffer != NULL)
  {
    // cppcheck-suppress misra-c2012-11.4; Conversion from pointer to integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    d_MEMORY_DCacheFlushRange((Pointer_t)&pBuffer[0], bufferSize);
  }
  ELSE_DO_NOTHING

  d_SATA_RegisterWrite(PxCI_REGISTER + (port * PORT_REG_OFFSET), PORT_CMD_ST); // issue start command
  Uint32_t registerValue = d_SATA_RegisterRead(PxCI_REGISTER + (port * PORT_REG_OFFSET)); // flush

  // wait for cmd to complete
  const Uint32_t DATA_IO_TIMEOUT_IN_MSEC = 10000u;
  Uint32_t startTime = d_TIMER_ReadValueInTicks();
  do
  {
    registerValue = d_SATA_RegisterRead(PxCI_REGISTER + (port * PORT_REG_OFFSET));    
  } while (((registerValue & PORT_CMD_ST) != 0u) && (d_TIMER_ElapsedMilliseconds(startTime, NULL) < DATA_IO_TIMEOUT_IN_MSEC));

  if ((registerValue & PORT_CMD_ST) != 0u) // timeout occurred
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    xil_printf("Data IO Timeout occured\n\r");
    returnValue = d_STATUS_TIMEOUT;
  }
  ELSE_DO_NOTHING

  if ((returnValue == d_STATUS_SUCCESS) && (pBuffer != NULL))
  {
    // cppcheck-suppress misra-c2012-11.4; Conversion from pointer to integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    d_MEMORY_DCacheInvalidateRange((Pointer_t)&pBuffer[0], bufferSize);
  }
  ELSE_DO_NOTHING
  
  return returnValue;
}

/*********************************************************************//**
  <!-- AtaIdStrCpy -->

  Copies a device ID while swapping 16bit integers

*************************************************************************/
static void                    /** \return none */
AtaIdStrCpy
(Uint16_t * dest,              /**< [out] pointer to the destination to copy to */
const Uint16_t * const src,    /**< [in] pointer to the source being copied from */ 
const Uint32_t len)            /**< [in] length of the data to copy */
{
  for (Uint32_t i = 0u; i < len; i++)
  {
    dest[i] = ((Uint16_t)(src[i] << 8u)) | ((Uint16_t)(src[i] >> 8u));
  }
}

/*********************************************************************//**
  <!-- ScsiStrTrimCpy -->

  Copies a scsi identity field string to a desitnation, 
  trimming the leading and trailing whitspaces

*************************************************************************/
static void                 /** \return none */
ScsiStrTrimCpy
(Uint8_t *dest,             /**< [out] pointer to the destination to copy to */
const Uint8_t * const src,  /**< [in] pointer to the source being copied from */
const Uint32_t len)         /**< [in] length of the data to copy */
{
	Uint32_t start;
  Uint32_t end;

	start = 0u;
	while (start < len) 
  {
	  if (src[start] != (Uint8_t)' ')
    {
			break;
    }
	  // gcov-jst 3 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

		start++;
	}

	end = len - 1u;
	while (end > start) 
  {
		if (src[end] != (Uint8_t)' ')
    {
			break;
    }
		// gcov-jst 3 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

		end--;
	}

  Uint32_t destIndex = 0;
	for (; start <= end; start++)
  {
		dest[destIndex] = src[start];
    destIndex++;
  }

	dest[destIndex] = (Uint8_t)'\0';
}

/*********************************************************************//**
  <!-- AtaIdHasLba48 -->

  Checks if the device ID supports LBA48

*************************************************************************/
static Uint32_t            /** \return 0 if LBA48 is not supported  */
AtaIdHasLba48
(const Uint16_t* const id) /**< [in] pointer to the buffer containing the ID data */
{
  Uint32_t returnValue;
  if (((id[83] & 0xC000u) != 0x4000u) || (AtaIdU64(id, ATA_ID_LBA48_SECTORS) == 0u))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
		returnValue = 0u;
  }
  else 
  {
    returnValue = (id[83] & (Uint16_t)((Uint16_t)1u << 10u));
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- ExtractCapacity -->

  Extract the drive capacity from the response

*************************************************************************/
static Uint32_t               /** \return Capacity in number of sectors */
ExtractCapacity
(void)                        /**< [in] None*/
{
  Uint32_t returnValue = 0u;
  if (AtaIdHasLba(readBuff) != 0u)
  {
    if (AtaIdHasLba48(readBuff) != 0u)
    {
      returnValue = (Uint32_t)AtaIdU64(readBuff, ATA_ID_LBA48_SECTORS);
    }
    else
    {
      // gcov-jst 5 It is not practical to generate this failure during bench testing.
      const Uint32_t ATA_ID_LBA_SECTORS	= 60u;
      returnValue = AtaIdU32(readBuff, ATA_ID_LBA_SECTORS);
    }
  }
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- ExtractSmartDataField -->

  Extracts the value of a field in the S.M.A.R.T Data info from the read buffer

*************************************************************************/
static Uint64_t                       /** \return The value contained in the specified field */
ExtractSmartDataField
(Uint32_t id,                         /**< [in] the ID of the field requested*/
const Uint8_t * const pRawBuffer)     /**< [in] pointer to the raw data buffer*/
{
  if (pRawBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (id > 249u) // max SMART id field
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  Uint64_t fieldValue = 0;
  for (Uint32_t i = 2u; i < 361u; i += 12u)
  {
    Uint32_t currentID =  pRawBuffer[i];
    if (currentID == id)
    {
      fieldValue = ((Uint64_t)pRawBuffer[i+5u]) | 
                  ((Uint64_t)pRawBuffer[i+6u] << 8u) | 
                  ((Uint64_t)pRawBuffer[i+7u] << 16u) | 
                  ((Uint64_t)pRawBuffer[i+8u] << 24u)| 
                  ((Uint64_t)pRawBuffer[i+9u] << 32u) | 
                  ((Uint64_t)pRawBuffer[i+10u] << 40u);                 

      break;
    }
    ELSE_DO_NOTHING
  }

  return fieldValue;
}


/*********************************************************************//**
  <!-- d_SATAOP_ReadSmartData -->

  SATA operation function for reading SMART Data from device
  Reads the device SMART Data

*************************************************************************/
d_Status_t                     /** \return 0 upon success, error code on failure */
d_SATAOP_ReadSmartData
(const Uint32_t port,          /**< [in] Port to perform the operation*/
d_SATA_SmartInfo_t *pSmartInfo) /**< [out] pointer to structure for read data */
{
  const Uint8_t ATA_CMD_SMART		    = 0xB0u;  // SMART command
  const Uint8_t SMART_READ_DATA     = 0xD0u;  // SMART Sub command
  Uint8_t fis[20u];

  if (pSmartInfo == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_NON_CRITICAL, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (port >= SATA_PORT_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_NON_CRITICAL, 1, port, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  // construct the FIS
  d_GEN_MemorySet(fis, 0u, sizeof(fis));
  fis[0u] = 0x27u;		        /* Host to device FIS. */
	fis[1u] = 1u << 7u;	      /* Command FIS. */
	fis[2u] = ATA_CMD_SMART;  /* Command byte. */
  fis[3u] = SMART_READ_DATA;  /* SMART sub command goes in the Features field */
  fis[5u] = 0x4fu;            /* lba mid */
  fis[6u] = 0xC2u;          /* lba high */
  fis[12u] = 1u;            /* Sector Count */

  d_Status_t returnValue = AhciDataIo(fis, sizeof(fis), (Uint8_t*)readBuff, ATA_SECT_SIZE, d_FALSE, port);
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    const Uint32_t SMART_POWER_ON_HOURS_ID = 9u;
    pSmartInfo->powerOnHours = (Uint32_t)ExtractSmartDataField(SMART_POWER_ON_HOURS_ID, (Uint8_t*)readBuff);

    const Uint32_t SMART_NORMAL_POWER_ON_OFF_COUNT_ID = 12u;
    pSmartInfo->normalPowerOnOffCount = (Uint32_t)ExtractSmartDataField(SMART_NORMAL_POWER_ON_OFF_COUNT_ID, (Uint8_t*)readBuff);

    const Uint32_t SMART_DEVICE_USER_CAPACITY_ID = 15u;
    pSmartInfo->deviceUserCapacity = (Uint32_t)ExtractSmartDataField(SMART_DEVICE_USER_CAPACITY_ID, (Uint8_t*)readBuff);

    const Uint32_t SMART_DEVICE_TEMPERATURE_ID = 194u;
    pSmartInfo->deviceTemperature = (Int8_t)ExtractSmartDataField(SMART_DEVICE_TEMPERATURE_ID, (Uint8_t*)readBuff);

    const Uint32_t SMART_REMAINING_LIFE_PERCENTAGE_ID = 248u;
    pSmartInfo->remainingLifePercentage = (Uint8_t)ExtractSmartDataField(SMART_REMAINING_LIFE_PERCENTAGE_ID, (Uint8_t*)readBuff);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return returnValue;
}



/*********************************************************************//**
  <!-- d_SATAOP_Inquiry -->

  SATA operation function for inquiry
  Reads the device information

*************************************************************************/
d_Status_t                    /** \return 0 upon success, error code on failure */
d_SATAOP_Inquiry
(const Uint32_t port)         /**< [in] Port to perform the operation*/
{
  const Uint8_t ATA_CMD_ID_ATA		    = 0xECu;  // identify device
  Uint8_t fis[20u];
  
  // construct the FIS
  d_GEN_MemorySet(fis, 0u, sizeof(fis));
  fis[0u] = 0x27u;		        /* Host to device FIS. */
	fis[1u] = 1u << 7u;	      /* Command FIS. */
	fis[2u] = ATA_CMD_ID_ATA;  /* Command byte. */

  d_Status_t returnValue = AhciDataIo(fis, sizeof(fis), (Uint8_t*)readBuff, ATA_SECT_SIZE, d_FALSE, port);
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    // extract the device info
    d_GEN_MemoryCopy((Uint8_t*)d_SATA_DeviceInfo.vendor, (const Uint8_t*)"ATA", 4u);

    Uint16_t tempBuff[9];
    d_GEN_MemorySet((Uint8_t*)tempBuff, 0u, sizeof(tempBuff));

    AtaIdStrCpy(tempBuff, &readBuff[27u], (sizeof(d_SATA_DeviceInfo.product) - 1u)/2u);
    ScsiStrTrimCpy((Uint8_t*)d_SATA_DeviceInfo.product, (Uint8_t*)tempBuff, sizeof(d_SATA_DeviceInfo.product) - 1u);

    AtaIdStrCpy(tempBuff, &readBuff[23u], (sizeof(d_SATA_DeviceInfo.revision) - 1u)/2u);
    ScsiStrTrimCpy((Uint8_t*)d_SATA_DeviceInfo.revision, (Uint8_t*)tempBuff, sizeof(d_SATA_DeviceInfo.revision) - 1u);

    AtaIdStrCpy(tempBuff, &readBuff[10u], (sizeof(d_SATA_DeviceInfo.serial) - 1u)/2u);
    ScsiStrTrimCpy((Uint8_t*)d_SATA_DeviceInfo.serial, (Uint8_t*)tempBuff, sizeof(d_SATA_DeviceInfo.serial) - 1u);

    d_SATA_DeviceInfo.capacity = ExtractCapacity();
    d_SATA_DeviceInfo.sectorSize = ATA_SECT_SIZE;
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_Flush -->

  SATA operation function to flush the drive cache to disk

*************************************************************************/
d_Status_t                    /** \return 0 upon success, error code on failure */
d_SATAOP_Flush
(const Uint32_t port)         /**< [in] Port to perform the operation*/
{
  const Uint8_t ATA_CMD_FLUSH_EXT	  = 0xEAu;  // Flush command
  Uint8_t fis[20u];
  
  // construct the FIS
  d_GEN_MemorySet(fis, 0u, sizeof(fis));
  fis[0] = 0x27u;		        /* Host to device FIS. */
	fis[1] = 1u << 7u;	      /* Command FIS. */
	fis[2] = ATA_CMD_FLUSH_EXT;  /* Command byte. */

  return AhciDataIo(fis, sizeof(fis), NULL, 0u, d_FALSE, port);
}

/*********************************************************************//**
  <!-- d_SATAOP_ReadWrite -->

  SATA operation function for Read or write
  Functions constructs the FIS command and performs IO to the device
  
*************************************************************************/
d_Status_t                    /** \return 0 upon success, error code on failure */
d_SATAOP_ReadWrite
(const Uint32_t port,         /**< [in] Port to perform the operation */
const Uint32_t startingLba,   /**< [in] logical block to read */            
const Uint8_t * const pBuffer,            /**< [out] pointer to buffer for the data being read/written */
const Uint32_t length,        /**< [in] the amount of bytes to read/write */
const Bool_t writeFlag)       /**< [in] Flag indicating direction of transaction */
{
  // TODO: check buffer pointer and buffer size
  // Also check lba range and port number  

  d_Status_t returnValue = d_STATUS_SUCCESS;

  Uint8_t fis[20u];
  Uint32_t blockCount = length / ATA_SECT_SIZE; 
  if ((length % ATA_SECT_SIZE) != 0u)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    blockCount ++;
  }
  ELSE_DO_NOTHING

  Uint32_t lba = startingLba;
  Uint32_t index = 0;

  while (blockCount > 0u)
  {
    const Uint8_t ATA_CMD_READ_EXT     = 0x25u;  // Read command
    const Uint8_t ATA_CMD_WRITE_EXT	  = 0x35u;  // write command
    const Uint32_t MAX_SATA_BLOCKS_READ_WRITE	= 0x80;
    Uint32_t blocksInTransaction = (blockCount > MAX_SATA_BLOCKS_READ_WRITE) ? MAX_SATA_BLOCKS_READ_WRITE: blockCount;
    Uint32_t transferSize = blocksInTransaction * ATA_SECT_SIZE;

    // construct the FIS
    d_GEN_MemorySet(fis, 0u, sizeof(fis));

    fis[0u] = 0x27u;		  /* Host to device FIS. */
    fis[1u] = 1u << 7u;	    /* Command FIS. */
    fis[2u] = (writeFlag == d_TRUE) ? ATA_CMD_WRITE_EXT : ATA_CMD_READ_EXT;

    fis[4u] = (lba >> 0u) & 0xffu;
    fis[5u] = (lba >> 8u) & 0xffu;
    fis[6u] = (lba >> 16u) & 0xffu;
    fis[7u] = 1u << 6u; /* device reg: set LBA mode */
    fis[8u] = ((lba >> 24u) & 0xffu);

    fis[3u] = 0xe0u; /* features */
    
    /* Block (sector) count */
    fis[12u] = (blockCount >> 0u) & 0xffu;
    fis[13u] = (blockCount >> 8u) & 0xffu;

    returnValue = AhciDataIo(fis, sizeof(fis), &pBuffer[index], transferSize, writeFlag, port);
    if (returnValue != d_STATUS_SUCCESS)
    {
       // gcov-jst 1 It is not practical to generate this failure during bench testing.
      break;
    }
    ELSE_DO_NOTHING

    index += transferSize;
    blockCount -= blocksInTransaction;
    lba += blocksInTransaction;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_Discovery0 -->

  SATA operation function for Discovery0
  Reads extended device info to find encryption / locking capabilities and status

*************************************************************************/
d_Status_t                                        /** \return 0 upon success, error code on failure */
d_SATAOP_Discovery0
(const Uint32_t port,                             /**< [in] Port to perform the operation */
d_SATAENC_SelfEncryptionStatus_t * pLockStatus)   /**< [in] Pointer to a structure to store the drive locking status */
{
  d_Status_t returnValue;
  if (pLockStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    const Uint8_t ATA_CMD_TRUSTED_RCV		    = 0x5Cu;  // Trusted receive
    Uint8_t fis[20u];

    // construct the FIS
    d_GEN_MemorySet(fis, 0u, sizeof(fis));

    fis[0u] = 0x27u;		        /* Host to device FIS. */
    fis[1u] = 1u << 7u;	      /* Command FIS. */
    fis[2u] = ATA_CMD_TRUSTED_RCV;  /* Command byte. */

    fis[3u] = 0x01u; /* ATA features / TRUSTED S/R security protocol */
    
    const Uint16_t COM_ID = 0x0001u;

    fis[5u] = (COM_ID & 0x00ffu);
    fis[6u] = ((COM_ID & 0xff00u) >> 8u);

    const Uint32_t SECTOR_COUNT = 4u;
    fis[12u] = SECTOR_COUNT; 

    fis[15u] = 0x08u; // ATA_DEVCTL_OBS

    d_GEN_MemorySet((Uint8_t*)readBuff, 0u, sizeof(readBuff));

    returnValue = AhciDataIo(fis, sizeof(fis), (Uint8_t*)readBuff, ATA_SECT_SIZE * SECTOR_COUNT, d_FALSE, port);
    
    if (returnValue == d_STATUS_SUCCESS)
    {
      Uint8_t *cpos = (Uint8_t*)&readBuff[0u];
      Uint8_t *epos = (Uint8_t*)&readBuff[0u];

      Uint32_t headerLength = SWAP32(cpos, 0u);

      // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
      epos = (Uint8_t *)((Uint32_t)epos + headerLength);
      // cppcheck-suppress misra-c2012-18.4;  Operation on Uint8_t pointer. Violation of 'Advisory' rule does not present a risk.
      cpos = cpos + 48u; // TODO: check header version

      d_GEN_MemorySet((Uint8_t *)pLockStatus, 0u, sizeof(d_SATAENC_SelfEncryptionStatus_t));

      do 
      {
        Uint16_t featureCode = SWAP16(cpos, 0u);
        switch (featureCode)
        {
          case FC_LOCKING:
          {
            // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
            Discovery0LockingFeatures * lockingFeatures = (Discovery0LockingFeatures*)cpos;
            if (lockingFeatures->lockingSupported)
            {
              pLockStatus->lockingSupported = d_TRUE;
            }
            else
            {
              // gcov-jst 1 It is not practical to generate this failure during bench testing.
              pLockStatus->lockingSupported = d_FALSE;
            }

            if (lockingFeatures->lockingEnabled)
            {
              pLockStatus->lockingEnabled = d_TRUE;
            }
            else
            {
              // gcov-jst 1 It is not practical to generate this failure during bench testing.
              pLockStatus->lockingEnabled = d_FALSE;
            }

            if (lockingFeatures->locked)
            {
              pLockStatus->locked = d_TRUE;
            }
            else
            {
              pLockStatus->locked = d_FALSE;
            }

            break;
          }
          // clang-tidy-jst bugprone-branch-clone 1 Separate statements for clarity although they perform the same operation.
          case FC_TPER:
          case FC_GEOMETRY:
          case FC_ENTERPRISE:
          case FC_OPALV100:
          case FC_SINGLEUSER:
          case FC_DATASTORE:
          case FC_OPALV200:
            break;
          default:
            break; 
        }

        Uint8_t length = cpos[3u]; 
        // cppcheck-suppress misra-c2012-18.4;  Operation on Uint8_t pointer. Violation of 'Advisory' rule does not present a risk.
        cpos = cpos + length + 4u;

      } while (cpos < epos);

    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAENC_TxRxOpalPacket -->

  SATA Tx Rx operation function for Standard OPAL messages
  the message should already be packed into readBuff
  The response will also be written to ReadBuff

*************************************************************************/
static d_Status_t             /** \return 0 upon success, error code on failure */
d_SATAENC_TxRxOpalPacket
(const Uint32_t port,         /**< [in] Port to perform the operation */
const Uint32_t byteCount)     /**< [in] Number of bytes loaded to the Tx Rx buffer */
{
  const Uint16_t COM_ID = 0x1005u;
  const Uint8_t ATA_CMD_TRUSTED_SEND		    = 0x5Eu;  // Trusted send
  Uint8_t fis[20u];

  Uint32_t sectorCount = byteCount / ATA_SECT_SIZE;
  if ((byteCount % ATA_SECT_SIZE) != 0u)
  {
    sectorCount ++;
  }
  // construct the FIS
  d_GEN_MemorySet(fis, 0u, sizeof(fis));

  fis[0u] = 0x27u;		        /* Host to device FIS. */
  fis[1u] = 1u << 7u;	      /* Command FIS. */
  fis[2u] = ATA_CMD_TRUSTED_SEND;  /* Command byte. */
  fis[3u] = 0x01u; /* ATA features / TRUSTED S/R security protocol */
  fis[5u] = (COM_ID & 0x00ffu);
  fis[6u] = ((COM_ID & 0xff00u) >> 8u);
  fis[12u] = sectorCount; 
  fis[15u] = 0x08u; // ATA_DEVCTL_OBS

  d_Status_t returnValue = AhciDataIo(fis, sizeof(fis), (Uint8_t*)readBuff, sectorCount * ATA_SECT_SIZE, d_TRUE, port);
  if (returnValue == d_STATUS_SUCCESS)
  {
    do 
    {
      (void)d_TIMER_DelayMilliseconds(25u); // Sleep before reading the response
      const Uint8_t ATA_CMD_TRUSTED_RCV		    = 0x5Cu;  // Trusted receive
      d_GEN_MemorySet((Uint8_t*)readBuff, 0, sizeof(readBuff));
      d_GEN_MemorySet(fis, 0u, sizeof(fis));

      fis[0u] = 0x27u;		        /* Host to device FIS. */
      fis[1u] = 1u << 7u;	      /* Command FIS. */
      fis[2u] = ATA_CMD_TRUSTED_RCV;  /* Command byte. */
      fis[3u] = 0x01u; /* ATA features / TRUSTED S/R security protocol */
      fis[5u] = (COM_ID & 0x00ffu);
      fis[6u] = ((COM_ID & 0xff00u) >> 8u);

      sectorCount = sizeof(readBuff) / ATA_SECT_SIZE;
      if ((sizeof(readBuff) % ATA_SECT_SIZE) != 0u)
      {
        sectorCount ++;
      }

      fis[12u] = sectorCount; 
      fis[15u] = 0x08u; // ATA_DEVCTL_OBS

      returnValue = AhciDataIo(fis, sizeof(fis), (Uint8_t*)readBuff, sizeof(readBuff), d_FALSE, port);
    }
    while (d_SATAENC_IsOperationComplete((Uint8_t*)readBuff, sizeof(readBuff)) == d_FALSE);
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_EncTerminateSession -->

  SATA operation function for terminating a session with the encryption feature

*************************************************************************/
static d_Status_t                 /** \return 0 upon success, error code on failure */
d_SATAOP_EncTerminateSession
(const Uint32_t port)             /**< [in] Port to perform the operation */
{
  Uint32_t byteCount = d_SATAENC_BuildSessionTerminatePayload((Uint8_t*)readBuff, sizeof(readBuff));
  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_EncStartSession -->

  SATA operation function for starting a session with the encryption feature

*************************************************************************/
static d_Status_t                        /** \return 0 upon success, error code on failure */
d_SATAOP_EncStartSession
(const Uint32_t port,             /**< [in] Port to perform the operation */
const Char_t * password,          /**< [in] Password to use for session, can be NULL */
const Uint32_t pwdLength,         /**< [in] length of the given password */
const d_SATAENC_uidPair_t uidPair) /**< [in] UID pair for security provider and signing authority / User */
{
  Uint32_t byteCount = d_SATAENC_BuildSessionStartPayload((Uint8_t*)readBuff, sizeof(readBuff), password, pwdLength, uidPair);
  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint8_t ret = d_SATAENC_GetTokenFromResponse((Uint8_t*)readBuff, sizeof(readBuff), 9u);
    if (ret != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
    else
    {
      d_SATAENC_UpdateSessionNumbersFromResponse((Uint8_t*)readBuff, sizeof(readBuff));
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_SetSIDPassword -->

  SATA operation function to set the SID password

*************************************************************************/
static d_Status_t                   /** \return 0 upon success, error code on failure */
d_SATAOP_SetSIDPassword
(const Uint32_t port,               /**< [in] Port to perform the operation */
const Char_t * const password,      /**< [in] New password to use */
const Uint32_t pwdLength)           /**< [in] length of the password string */
{
  Uint32_t byteCount = d_SATAENC_BuildSetTablePayload((Uint8_t*)readBuff, sizeof(readBuff), password, pwdLength);
  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint8_t ret = d_SATAENC_GetTokenFromResponse((Uint8_t*)readBuff, sizeof(readBuff), 4u);
    if (ret != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_SetLockingRange -->

  SATA operation function to Set locking of range 0 (Entire drive)
  This will either lock or unlock the range 0

*************************************************************************/
static d_Status_t               /** \return 0 upon success, error code on failure */
d_SATAOP_SetLockingRange
(const Uint32_t port,           /**< [in] Port to perform the operation */
const Bool_t lock)              /**< [in] flag whether to lock or unlock */ 
{
  Uint32_t byteCount = d_SATAENC_BuildSetLockRangePayload((Uint8_t*)readBuff, sizeof(readBuff), lock);
  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint8_t ret = d_SATAENC_GetTokenFromResponse((Uint8_t*)readBuff, sizeof(readBuff), 4u);
    if (ret != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_RevertLockingSP -->

  SATA operation function to Revert the locking SP to the default state.
  Locking is disabled, disk contents should be safe

*************************************************************************/
static d_Status_t               /** \return 0 upon success, error code on failure */
d_SATAOP_RevertLockingSP
(const Uint32_t port)           /**< [in] Port to perform the operation */
{ 
  Uint32_t byteCount = d_SATAENC_BuildRevertLockingSPPayload((Uint8_t*)readBuff, sizeof(readBuff)); 
  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint8_t ret = d_SATAENC_GetTokenFromResponse((Uint8_t*)readBuff, sizeof(readBuff), 4u);
    if (ret != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_ConfigureLockingRange -->

  SATA operation function to Configure locking of range 0 (Entire drive)
  Only configures locking of this range but leaves the device in the unlocked state

*************************************************************************/
static d_Status_t               /** \return 0 upon success, error code on failure */
d_SATAOP_ConfigureLockingRange
(const Uint32_t port,           /**< [in] Port to perform the operation */
const Bool_t lockingEnabled)    /**< [in] flag whether to lock or unlock */ 
{
  Uint32_t byteCount = d_SATAENC_BuildLockingRangePayload((Uint8_t*)readBuff, sizeof(readBuff), lockingEnabled); 

  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint8_t ret = d_SATAENC_GetTokenFromResponse((Uint8_t*)readBuff, sizeof(readBuff), 4u);
    if (ret != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_EncActivateLockingSP -->

  SATA operation function to Activate locking SP

*************************************************************************/
static d_Status_t                  /** \return 0 upon success, error code on failure */
d_SATAOP_EncActivateLockingSP
(const Uint32_t port)       /**< [in] Port to perform the operation */
{
  Uint32_t byteCount = d_SATAENC_BuildActivateLockingSPPayload((Uint8_t*)readBuff, sizeof(readBuff));
  d_Status_t returnValue = d_SATAENC_TxRxOpalPacket(port, byteCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint8_t ret = d_SATAENC_GetTokenFromResponse((Uint8_t*)readBuff, sizeof(readBuff), 4u);
    if (ret != 0u)
    {
      returnValue = d_STATUS_FAILURE;
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_SetLockingUnlocking -->

  Either locks or unlocks range 0 (entire drive) on the drive

*************************************************************************/
d_Status_t                      /** \return 0 upon success, error code on failure */
d_SATAOP_SetLockingUnlocking
(const Uint32_t port,          /**< [in] Port to perform the operation */
const Char_t * const password, /**< [in] pointer to the encryption password to use */
const Uint32_t pwdLength,      /**< [in] length of the encryption password */
const Bool_t lock)             /**< [in] flag whether to lock or unlock */ 
{
  d_SATAENC_uidPair_t uidPair;
  uidPair.secProvider = OPAL_LOCKINGSP_UID;
  uidPair.signAuth = OPAL_ADMIN1_UID;
  d_Status_t returnValue = d_SATAOP_EncStartSession(port, password, pwdLength, uidPair);
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_SetLockingRange(port, lock);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_EncTerminateSession(port);
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_EnableDisableLockingRange -->

  Enable or disable locking on Range 0 (Entire drive)

*************************************************************************/
d_Status_t                        /** \return 0 upon success, error code on failure */
d_SATAOP_EnableDisableLockingRange
(const Uint32_t port,             /**< [in] Port to perform the operation */
const Char_t * const password,    /**< [in] Locking password to use */
const Uint32_t pwdLength,         /**< [in] Length of the password string */
const Bool_t lockingEnabled)      /**< [in] flag to indicate whether to enable or disable locking */
{
  d_Status_t returnValue;
  d_SATAENC_uidPair_t uidPair;
  uidPair.secProvider = OPAL_LOCKINGSP_UID;
  uidPair.signAuth = OPAL_ADMIN1_UID;
  returnValue = d_SATAOP_EncStartSession(port, password, pwdLength, uidPair);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_ConfigureLockingRange(port, lockingEnabled);
  }

  if ((returnValue == d_STATUS_SUCCESS) && (lockingEnabled == d_FALSE))
  {
    // the previous functions only disables locking for the range 0
    // we also need to revert the locking SP 
    returnValue = d_SATAOP_RevertLockingSP(port);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_EncTerminateSession(port);
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAOP_EncInitialSetup -->

  Perform Initial setup of Encryption on SED drive

*************************************************************************/
d_Status_t                      /** \return 0 upon success, error code on failure */
d_SATAOP_EncInitialSetup
(const Uint32_t port,           /**< [in] Port to perform the operation */
const Char_t * const password,  /**< [in] Password to use for locking */
const Uint32_t pwdLength)       /**< [in] Length of the password string */
{
  d_SATAENC_uidPair_t uidPair;
  uidPair.secProvider = OPAL_ADMINSP_UID;
  uidPair.signAuth = OPAL_SID_UID;

  d_Status_t returnValue = d_SATAOP_EncStartSession(port, DEFAULT_PASSWORD, sizeof(DEFAULT_PASSWORD) - 1u, uidPair); 
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_SetSIDPassword(port, password, pwdLength);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_EncActivateLockingSP(port);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_EncTerminateSession(port);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_EnableDisableLockingRange(port, password, pwdLength, d_TRUE);
  }

  return returnValue;
}


