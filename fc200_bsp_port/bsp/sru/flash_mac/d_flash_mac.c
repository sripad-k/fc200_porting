/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_flash_mac.c

  Abstract           : Flash MAC device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-177
                                                                SWREQ-178
                                                                SWREQ-179
                                                                SWREQ-180
                                                                SWREQ-214
                       SDD References: 136T-2200-131100-001-D22 SWDES-295
                                                                SWDES-615
                                                                SWDES-616
                                                                SWDES-617
                                                                SWDES-618
                                                                SWDES-619
                                                                SWDES-620
                                                                SWDES-621
                                                                SWDES-622
                                                                SWDES-623
                                                                SWDES-624
                       
\note
  CSC ID             : SWDES-59
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "soc/timer/d_timer.h"
#include "sru/spi_pl/d_spi_pl.h"
#include "sru/spi_pl/d_spi_pl_cfg.h"

#include "d_flash_mac.h"
#include "d_flash_mac_cfg.h"

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SECTOR_COUNT                          = d_FLASH_MAC_SIZE / d_FLASH_MAC_SECTOR_SIZE;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SPI_TRANSACTION_TIMEOUT_USEC          = 500u; /* Timeout period for SPI transaction to complete */

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_STATUS_BUSY_MASK                = 0x01u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_READ_COMMAND                    = 0x03u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_WRITE_COMMAND                   = 0x02u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_ERASE_SECTOR_COMMAND            = 0x20u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_ERASE_CHIP_COMMAND              = 0xC7u;

static const Uint32_t FLASH_READ_STATUS_COMMAND             = 0x05u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_READ_CONFIG_COMMAND             = 0x35u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_WRITE_CONFIG_COMMAND            = 0x01u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_BLOCK_PROTECTION_READ           = 0x72u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk. 
static const Uint32_t FLASH_BLOCK_PROTECTION_WRITE          = 0x42u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t FLASH_UNLOCK_COMMAND                  = 0x98u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t FLASH_WRITE_ENABLE_COMMAND            = 0x06u;

static const Uint32_t FLASH_READ_DISCOVERABLE_COMMAND       = 0x5Au;

/* Address value when no address required in command */
static const Uint32_t ADDRESS_NOT_REQUIRED                  = 0xFFFFFFFFu;

/* Address of MAC in discoverable parameters */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t ADDRESS_MAC                           = 0x00000260u;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

static d_Status_t writeEnable(const Uint32_t device);

static Bool_t blockProtected(const Uint32_t device, const Uint32_t address);

static d_Status_t waitForSpiTransactionComplete(const Uint32_t device,
                                                const Uint32_t timeoutPeriodInUsec);

static d_Status_t processCommand(const Uint32_t device,
                                 const Uint8_t command,
                                 const Uint32_t address,
                                 const Uint8_t * const txBuffer,
                                 Uint8_t * const rxBuffer,
                                 const Uint32_t dataCount);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_FLASH_MAC_Initialise -->

  Initialises the FLASH MAC CSC.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_Initialise
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    /* Check number of devices is valid */
    if ((d_FLASH_MAC_Count == 0u) || (d_FLASH_MAC_Count > d_FLASH_MAC_MAX_INTERFACES))
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, d_FLASH_MAC_Count, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }

    /* Check configured SPI interface numbers are valid */
   	Uint32_t channel = 0;
    while ((status == d_STATUS_SUCCESS) && (channel < d_FLASH_MAC_Count))
    {
      if (d_FLASH_MAC_Config[channel].channel >= d_SPI_PL_Count)
      {
        // gcov-jst 3 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, d_FLASH_MAC_Config[channel].channel, 0);
        // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
        return d_STATUS_INVALID_CONFIGURATION;
      }

      if (d_FLASH_MAC_Config[channel].device >= d_SPI_PL_MAX_DEVICES)
      {
        // gcov-jst 3 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, channel, d_FLASH_MAC_Config[channel].device, 0);
        // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
        return d_STATUS_INVALID_CONFIGURATION;
      }

      status = d_SPI_PL_Initialise(d_FLASH_MAC_Config[channel].channel);

      channel++;
    }

    d_TIMER_Initialise();

    initialised = d_TRUE;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_Unlock -->

  Unlock the FLASH MAC device.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_Unlock
(
const Uint32_t device             /**< [in] Device number */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = writeEnable(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_UNLOCK_COMMAND, ADDRESS_NOT_REQUIRED, NULL, NULL, 0);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_Read -->

  Read up t 256 bytes from the FLASH MAC device.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_Read
(
const Uint32_t device,            /**< [in] Device number */
Uint32_t addressToRead,           /**< [in] Memory address in device */
const Uint32_t bytesToRead,       /**< [in] The number of bytes to be read from the flash  */
Uint8_t * const pDataBuffer,      /**< [in] Pointer to the buffer for the read data */
const Uint32_t bufferSizeInBytes  /**< [in] The size of the buffer for the data */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((addressToRead + bytesToRead) > d_FLASH_MAC_SIZE)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Limit to page size as that is the size of the SPI FIFO */
  if ((bytesToRead == 0u) || (bytesToRead > d_FLASH_MAC_PAGE_SIZE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, bytesToRead, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pDataBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bufferSizeInBytes < bytesToRead)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, bufferSizeInBytes, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = d_FLASH_MAC_CheckFlashNotBusy(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_READ_COMMAND, addressToRead, NULL, pDataBuffer, bytesToRead);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_Write -->

  Write data to the FLASH MAC device.
  The data must fall entirely within a 256 byte page.
*************************************************************************/
d_Status_t                          /** \return function status */
d_FLASH_MAC_Write
(
const Uint32_t device,              /**< [in] Device number */
const Uint32_t addressToWrite,      /**< [in] Memory address in device */
const Uint32_t bytesToWrite,        /**< [in] The number of bytes to be write to the flash  */
const Uint8_t * const pDataBuffer,  /**< [in] Pointer to the buffer containing the data to be written */
const Uint32_t bufferSizeInBytes    /**< [in] The size of the buffer containing the data */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((addressToWrite + bytesToWrite) > d_FLASH_MAC_SIZE)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((bytesToWrite == 0u) || (((addressToWrite & 0xFFu) + bytesToWrite) > d_FLASH_MAC_PAGE_SIZE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, bytesToWrite, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pDataBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;  
  }

  if (bufferSizeInBytes < bytesToWrite)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, bufferSizeInBytes, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;  
  }

  d_Status_t returnValue = writeEnable(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    if (blockProtected(device, addressToWrite) == d_TRUE)
    {
      returnValue = d_STATUS_WRITE_PROTECTED;
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

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_WRITE_COMMAND, addressToWrite, pDataBuffer, NULL, bytesToWrite);
  }
  else
  {
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_EraseSector -->

  Erases a 4k sector of the FLASH MAC device.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_EraseSector
(
const Uint32_t device,            /**< [in] Device number */
const Uint32_t sectorIndex)       /**< [in] The index of the sector to be erased */
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (sectorIndex >= SECTOR_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, sectorIndex, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = writeEnable(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    if (blockProtected(device, sectorIndex * d_FLASH_MAC_SECTOR_SIZE) == d_TRUE)
    {
      returnValue = d_STATUS_WRITE_PROTECTED;
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

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint32_t addressToErase = sectorIndex * d_FLASH_MAC_SECTOR_SIZE;

    returnValue = processCommand(device, FLASH_ERASE_SECTOR_COMMAND, addressToErase, NULL, NULL, 0);
  }
  else
  {
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_EraseDevice -->

  Erases the entire FLASH MAC device.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_EraseDevice
(
const Uint32_t device             /**< [in] Device number */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = writeEnable(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    d_FLASH_MAC_Protection_t protectionStatus;

    (void)d_FLASH_MAC_BlockProtectionRead(device, protectionStatus);
  
    for (Uint32_t index = 0; index < d_FLASH_MAC_PROTECTION_SIZE; index++)
    {
      // clang-tidy-jst clang-analyzer-core.UndefinedBinaryOperatorResult 1 The variable is set by the d_FLASH_MAC_BlockProtectionRead function.
      if (protectionStatus[index] != 0u)
      {
        returnValue = d_STATUS_WRITE_PROTECTED;
      }
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_ERASE_CHIP_COMMAND, ADDRESS_NOT_REQUIRED, NULL, NULL, 0);
  }
  else
  {
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_MacAddress -->

  Read the MAC address.
*************************************************************************/
d_Status_t                          /** \return function status */
d_FLASH_MAC_MacAddress
(
const Uint32_t device,              /**< [in]  Device number */
d_MacAddress_t macAddress  /**< [out] Pointer to storage for MAC address */
)
{
  Uint8_t buffer[8];
  
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (macAddress == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = d_FLASH_MAC_CheckFlashNotBusy(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_READ_DISCOVERABLE_COMMAND, ADDRESS_MAC, NULL, buffer, 8);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    for (Uint32_t index = 0; index < d_MAC_ADDRESS_LENGTH; index++)
    {
      macAddress[index] = buffer[index + 2u];
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
  <!-- d_FLASH_MAC_StatusRead -->

  Read the device status register
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_StatusRead
(
const Uint32_t device,            /**< [in] Device number */
Uint8_t * const pStatus           /**< [out] Pointer to storage for the status */
)
{
  d_Status_t returnValue;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  returnValue = processCommand(device, FLASH_READ_STATUS_COMMAND, ADDRESS_NOT_REQUIRED, NULL, pStatus, 1);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_ConfigRead -->

  Read the device config register.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_ConfigRead
(
const Uint32_t device,            /**< [in] Device number */
Uint8_t * const pConfig           /**< [out] Pointer to storage for the configuration */
)
{
  d_Status_t returnValue;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pConfig == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  returnValue = processCommand(device, FLASH_READ_CONFIG_COMMAND, ADDRESS_NOT_REQUIRED, NULL, pConfig, 1);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_ConfigWrite -->

  Write the device config register.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_ConfigWrite
(
const Uint32_t device,            /**< [in]  Device number */
const Uint8_t config              /**< [out] Configuration */
)
{
  d_Status_t returnValue;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  returnValue = writeEnable(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    Uint16_t configWrite = (Uint16_t)config << 8u;
    returnValue = processCommand(device, FLASH_WRITE_CONFIG_COMMAND, ADDRESS_NOT_REQUIRED, (Uint8_t *)&configWrite, NULL, 2);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_BlockProtectionRead -->

  Read the block protection register.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_BlockProtectionRead
(
const Uint32_t device,            /**< [in] Device number */
d_FLASH_MAC_Protection_t protect  /**< [out] Pointer to storage for the protection register */
)
{
  d_Status_t returnValue;
  Uint8_t protection[d_FLASH_MAC_PROTECTION_SIZE];

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (protect == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  for (Uint32_t index = 0; index < d_FLASH_MAC_PROTECTION_SIZE; index++)
  {
    protection[index] = 0;
  }

  returnValue = processCommand(device, FLASH_BLOCK_PROTECTION_READ, ADDRESS_NOT_REQUIRED, NULL, &protection[0], d_FLASH_MAC_PROTECTION_SIZE);

  if (returnValue == d_STATUS_SUCCESS)
  {
    for (Uint32_t index = 0; index < d_FLASH_MAC_PROTECTION_SIZE; index++)
    {
	    protect[index] = protection[d_FLASH_MAC_PROTECTION_SIZE - 1u - (Uint8_t)index];
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
  <!-- d_FLASH_MAC_BlockProtectionWrite -->

  Write the block protection register.
*************************************************************************/
d_Status_t                              /** \return function status */
d_FLASH_MAC_BlockProtectionWrite
(
const Uint32_t device,                  /**< [in] Device number */
const d_FLASH_MAC_Protection_t protect  /**< [out] Pointer to register data */
)
{
  Uint8_t protection[d_FLASH_MAC_PROTECTION_SIZE];

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (protect == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  for (Uint32_t index = 0; index < d_FLASH_MAC_PROTECTION_SIZE; index++)
  {
    protection[index] = protect[d_FLASH_MAC_PROTECTION_SIZE - 1u - (Uint8_t)index];
  }
  
  d_Status_t returnValue = writeEnable(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_BLOCK_PROTECTION_WRITE, ADDRESS_NOT_REQUIRED, &protection[0], NULL, d_FLASH_MAC_PROTECTION_SIZE);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_CheckFlashNotBusy -->

  Read the device status register to determine if it is busy.
*************************************************************************/
d_Status_t                        /** \return function status */
d_FLASH_MAC_CheckFlashNotBusy
(
const Uint32_t device             /**< [in] Device number */
)
{
  d_Status_t returnValue;
  Uint8_t status = 0x00u;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  returnValue = processCommand(device, FLASH_READ_STATUS_COMMAND, ADDRESS_NOT_REQUIRED, NULL, &status, 1);

  if (returnValue == d_STATUS_SUCCESS)
  {
    if ((status & FLASH_STATUS_BUSY_MASK) == FLASH_STATUS_BUSY_MASK)
    {
      returnValue = d_STATUS_DEVICE_BUSY;
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
  <!-- d_FLASH_MAC_SfdpByte -->

  Read a byte from the Serial Flash Discoverable Parameters.
*************************************************************************/
d_Status_t
d_FLASH_MAC_SfdpByte
(
const Uint32_t device,            /**< [in] Device number */
const Uint32_t address,
Uint8_t * const pValue
)
{
  Uint8_t buffer[8];

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (address >= 0xFFFFFFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, address, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = d_FLASH_MAC_CheckFlashNotBusy(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_READ_DISCOVERABLE_COMMAND, address, NULL, buffer, 8);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    *pValue = buffer[1];
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_FLASH_MAC_SfdpWord -->

  Read a word from the Serial Flash Discoverable Parameters.
*************************************************************************/
d_Status_t
d_FLASH_MAC_SfdpWord
(
const Uint32_t device,            /**< [in] Device number */
const Uint32_t address,
Uint32_t * const pValue
)
{
  Uint8_t buffer[8];

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_FLASH_MAC_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (address >= 0xFFFFFFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, address, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t returnValue = d_FLASH_MAC_CheckFlashNotBusy(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_READ_DISCOVERABLE_COMMAND, address, NULL, buffer, 8);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    *pValue = *(Uint32_t *)&buffer[1];
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- writeEnable -->

  Enable writing to the device.
*************************************************************************/
static d_Status_t                 /** \return function status */
writeEnable
(
const Uint32_t device             /**< [in] Device number */
)
{
  d_Status_t returnValue = d_FLASH_MAC_CheckFlashNotBusy(device);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = processCommand(device, FLASH_WRITE_ENABLE_COMMAND, ADDRESS_NOT_REQUIRED, NULL, NULL, 0);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- blockProtected -->

  Check if block is write protected.
*************************************************************************/
static Bool_t                     /** \return Protected state */
blockProtected
(
const Uint32_t device,            /**< [in] Device number */
const Uint32_t address            /**< [in] Address to be written */
)
{
  Bool_t returnValue;
  d_FLASH_MAC_Protection_t protectionStatus;
  Uint32_t protectBit;

  (void)d_FLASH_MAC_BlockProtectionRead(device, protectionStatus);

  if (address < 0x10000u)
  {
    if (address < 0x8000u)
    {
      protectBit = ((address / 0x2000u) * 2u) + 32u;
    }
    else
    {
      protectBit = 30;
    }
  }
  else if (address >= 0x1F0000u)
  {
    if (address >= 0x1F8000u)
    {
      protectBit = (((address - 0x1f8000u) / 0x2000u) * 2u) + 40u;
    }
    else
    {
      protectBit = 31;
    }
  }
  else
  {
    protectBit = (address / 0x10000u) - 1u;
  }

  if ((protectionStatus[protectBit / 8u] & (0x01u << (protectBit % 8u))) != 0x00u)
  {
    returnValue = d_TRUE;
  }
  else
  {
    returnValue = d_FALSE;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- waitForSpiTransactionComplete -->

  Wait for the SPI transaction to complete, function will timeout after specified timeout period and return with error

*************************************************************************/
static d_Status_t                   /** \return function status */
waitForSpiTransactionComplete
(
const Uint32_t device,              /**< [in] Device number */
const Uint32_t timeoutPeriodInUsec  /**< [in] The timeout period in microseconds */
)
{
  d_Status_t returnValue;
  Uint32_t startTime;
  Uint32_t elapsedTime;
  d_SPI_PL_State_t spiStatus;

  (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
  do
  {
    elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    returnValue = d_SPI_PL_Status(d_FLASH_MAC_Config[device].channel, &spiStatus);
  } while  ((spiStatus != d_SPI_PL_STATUS_COMPLETE) && (returnValue == d_STATUS_SUCCESS) && (elapsedTime < timeoutPeriodInUsec));

  if ((spiStatus != d_SPI_PL_STATUS_COMPLETE) && (returnValue == d_STATUS_SUCCESS))
  {
    /* FLASH response timeout */
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_TIMEOUT, d_ERROR_CRITICALITY_NON_CRITICAL, 0, 0, 0, 0);
    returnValue = d_STATUS_TIMEOUT;
  }
  else
  {
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- processCommand -->

  Send SPI command and wait for completion.
*************************************************************************/
static d_Status_t                 /** \return function status */
processCommand
(
const Uint32_t device,            /**< [in] Device number */
const Uint8_t command,
const Uint32_t address,
const Uint8_t * const txBuffer,
Uint8_t * const rxBuffer,
const Uint32_t dataCount
)
{
  d_Status_t returnValue;
  Uint8_t commandBuffer[4];
  d_SPI_PL_State_t spiStatus;
  Uint32_t count;

  commandBuffer[0] = command;
  Uint32_t commandCount = 1;
  if (address != 0xFFFFFFFFu)
  {
    Uint32_t index;
    const Uint8_t * aptr = (const Uint8_t *)&address;
    for (index = 0u; index < 3u; index++)
    {
      // cppcheck-suppress objectIndex; The individual bytes of the 32 bit address are accessed requiring non-zero index.
      commandBuffer[index + 1u] = aptr[2u - index];
    }
    commandCount = 4;
  }
  else
  {
    DO_NOTHING();
  }

  returnValue = d_SPI_PL_Transfer(d_FLASH_MAC_Config[device].channel,
                                  d_FLASH_MAC_Config[device].device,
                                  &commandBuffer[0],
                                  commandCount,
                                  txBuffer,
                                  dataCount);

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = waitForSpiTransactionComplete(device, SPI_TRANSACTION_TIMEOUT_USEC);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SPI_PL_Result(d_FLASH_MAC_Config[device].channel, &spiStatus, rxBuffer, dataCount, &count);
  }
  else
  {
    /* Reset SPI channel */
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    (void)d_SPI_PL_Reset(d_FLASH_MAC_Config[device].channel);
  }
  
return returnValue;
}
