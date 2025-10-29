/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_ioc_eeprom.c

  Abstract           : IOC EEPROM interface..

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-753
                       SDD References: 136T-2200-131100-001-D22 SWDES-2424
                                                                SWDES-2425
\note
  CSC ID             : SWDES-2419
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_status.h"           /* Error status */
#include "soc/timer/d_timer.h"                     /* Timer */
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "kernel/general/d_gen_register.h"         /* Register functions */

#include "sru/ioc_eeprom/d_ioc_eeprom.h"           /* Discrete driver */
#include "sru/ioc_eeprom/d_ioc_eeprom_cfg.h"       /* Discrete driver config */

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t WRITE_START = 0x01u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t READ_START  = 0x02u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t OFFSET_STATUS      = 0x00u;
static const Uint32_t OFFSET_CONTROL     = 0x04u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t OFFSET_PAGE_BUFFER = 0x40u;

/* Read and write timeout in microseconds. Note that the read time is to read all the Metadata */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t READ_TIMEOUT  = 50000;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t WRITE_TIMEOUT = 10000;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_IOC_EEPROM_Initialise -->

  Verify configuration data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_IOC_EEPROM_Initialise
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint32_t index;
  
  status = d_ERROR_Initialise();
  
  index = 0;
  while ((index < d_IOC_EEPROM_Count) && (status == d_STATUS_SUCCESS))
  {
    if (d_IOC_EEPROM_BaseAddress[index] == 0x00000000u)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, index, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }

    index++;
  }

  initialised = d_TRUE;
  
  return status;
}

/*********************************************************************//**
  <!-- d_IOC_EEPROM_Status -->

  Read the interface status.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_IOC_EEPROM_Status
(
Uint32_t device,                  /**< [in] device number */
Uint32_t * const pStatus           /**< [out] Pointer to storage for interface ststus */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (device >= d_IOC_EEPROM_Count)
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

  *pStatus = d_GEN_RegisterRead(d_IOC_EEPROM_BaseAddress[device] + OFFSET_STATUS);
  (void)d_TIMER_DelayMicroseconds(100);
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_IOC_EEPROM_Read -->

  Read a page from the EEPROM.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_IOC_EEPROM_Read
(
const Uint32_t device             /**< [in] device number */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t status;
  
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (device >= d_IOC_EEPROM_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Check not busy */
  (void)d_IOC_EEPROM_Status(device, &status);
  if ((status & (d_IOC_EEPROM_READ_BUSY | d_IOC_EEPROM_WRITE_BUSY)) != 0u)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_DEVICE_NOT_READY;
  }
  else
  {
    /* Send read command */
    d_GEN_RegisterWrite(d_IOC_EEPROM_BaseAddress[device] + OFFSET_CONTROL, READ_START);

    /* Wait for command to complete */
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    Uint32_t elapsedTime;
    do
    {
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
      (void)d_IOC_EEPROM_Status(device, &status);
    } while (((status & d_IOC_EEPROM_READ_BUSY) != 0u) && ((status & d_IOC_EEPROM_READ_ERROR) == 0u) && (elapsedTime < READ_TIMEOUT));

    if ((status & d_IOC_EEPROM_READ_ERROR) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_DEVICE_ERROR;
    }
    else if ((status & d_IOC_EEPROM_READ_BUSY) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_DEVICE_BUSY;
    }
    else
    {
      DO_NOTHING();
    }
  }

  return returnValue; 
}

/*********************************************************************//**
  <!-- d_IOC_EEPROM_Write -->

  Write a page to the EEPROM.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_IOC_EEPROM_Write
(
const Uint32_t device,            /**< [in] device number */
const Uint32_t page,              /**< [in] page number */
const d_IOC_EEPROM_PageBuffer_t pBuffer    /**< [in] pointer to data to be written */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t status;
  
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (device >= d_IOC_EEPROM_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (page >= d_IOC_EEPROM_PAGE_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, page, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Check not busy */
  (void)d_IOC_EEPROM_Status(device, &status);
  if ((status & (d_IOC_EEPROM_READ_BUSY | d_IOC_EEPROM_WRITE_BUSY)) != 0u)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_DEVICE_NOT_READY;
  }
  else
  {
    /* Copy data to page buffer */
    for (Uint32_t index = 0u; index < (d_IOC_EEPROM_PAGE_SIZE / 4u); index++)
    {
      // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
      d_GEN_RegisterWrite(d_IOC_EEPROM_BaseAddress[device] + OFFSET_PAGE_BUFFER + (index * 4u), *(const Uint32_t *)&pBuffer[index * 4u]);
    }

    /* Send command to write */
    d_GEN_RegisterWrite(d_IOC_EEPROM_BaseAddress[device] + OFFSET_CONTROL, WRITE_START | (page << 8u));

    /* Wait for command to complete */
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    Uint32_t elapsedTime;
    do
    {
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
      (void)d_IOC_EEPROM_Status(device, &status);
    } while (((status & d_IOC_EEPROM_WRITE_BUSY) != 0u) && ((status & d_IOC_EEPROM_WRITE_ERROR) == 0u) && (elapsedTime < WRITE_TIMEOUT));
  
    if ((status & d_IOC_EEPROM_WRITE_ERROR) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_DEVICE_ERROR;
    }
    else if ((status & d_IOC_EEPROM_WRITE_BUSY) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_DEVICE_BUSY;
    }
    else
    {
      DO_NOTHING();
    }
  }
  
  return returnValue;
}


