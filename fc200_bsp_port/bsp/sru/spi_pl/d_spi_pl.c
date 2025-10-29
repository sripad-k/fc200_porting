/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_spi_pl

  Abstract           : SPI PL Interface.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-758
                       SDD References: 136T-2200-131100-001-D22 SWDES-2427
                                                                SWDES-2428
                                                                SWDES-2429
                                                                SWDES-2430
                                                                SWDES-2431
                                                                SWDES-2432
\note
  CSC ID             : SWDES-2422
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_status.h"              /* Error status */
#include "soc/interrupt_manager/d_int_irq_handler.h"  /* Interrupt manager */
#include "kernel/error_handler/d_error_handler.h"     /* Error handler */
#include "kernel/general/d_gen_register.h"            /* Register functions */
#include "sru/fcu/d_fcu_cfg.h"                            /* FCU CSC */

#include "sru/spi_pl/d_spi_pl.h"
#include "sru/spi_pl/d_spi_pl_cfg.h"

/***** Type Definitions *************************************************/

typedef struct
{
  d_SPI_PL_State_t state;
  Uint32_t dataCount;
} spiStatus_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t CMD_COUNT_MAX = 8u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t DATA_COUNT_MAX = 256u;

enum
{
  REGISTER_OFFSET_STATUS = 0x0000u,
  REGISTER_OFFSET_CTRL   = 0x0004u,
  REGISTER_OFFSET_CMD0   = 0x0008u,
  REGISTER_OFFSET_CMD1   = 0x000Cu,
  REGISTER_OFFSET_IRQ    = 0x0010u,
  REGISTER_OFFSET_FIFO   = 0x0100u
};

/***** Variables ********************************************************/

static spiStatus_t spiStatus[d_SPI_PL_MAX_INTERFACES];
static Bool_t initialised[d_SPI_PL_MAX_INTERFACES] =
{
  d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE
};

/***** Function Declarations ********************************************/

static void DeviceInterrupt(const Uint32_t channel,
                            const Uint32_t isr);

static d_Status_t transfer(const Uint32_t channel,
                           const Uint32_t device,
                           const Uint8_t * const cmdBuffer,
                           const Uint32_t cmdCount,
                           const Uint8_t * const txBuffer,
                           const Uint32_t dataCount);

static Uint32_t spiRegisterRead(const Uint32_t channel,
                                const Uint32_t regOffset);

static void spiRegisterWrite(const Uint32_t channel,
                             const Uint32_t regOffset,
                             const Uint32_t value);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SPI_PL_Initialise -->

  Initialise SPI channel, set to IDLE.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_PL_Initialise
(
const Uint32_t channel            /**< [in]  Channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if ((channel >= d_SPI_PL_Count) || (channel >= d_SPI_PL_MAX_INTERFACES))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  spiStatus[channel].state = d_SPI_PL_STATUS_READY;
  (void)d_SPI_PL_Reset(channel);
  initialised[channel] = d_TRUE;
  /* Enable channel interrupt */
  status = d_INT_IrqEnable(d_SPI_PL_Definition[channel].interruptNumber);

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_PL_Reset -->

  Clear the receive FIFO.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_PL_Reset
(
const Uint32_t channel            /**< [in]  Channel number */
)
{
  d_Status_t returnValue;
  
  if (channel >= d_SPI_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  returnValue = d_FCU_IocAddressCheck(d_SPI_PL_Definition[channel].baseAddress);

  if (returnValue == d_STATUS_SUCCESS)
  {
    /* Read maximum 256 / 4 Uint32_t entries in the FIFO */
    for (Uint32_t index = 0u; index < 64u; index++)
    {
      (void)spiRegisterRead(channel, REGISTER_OFFSET_FIFO);
    }
    spiRegisterWrite(channel, REGISTER_OFFSET_IRQ, 0u);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_SPI_PL_Transfer -->

  Perform an SPI data transfer.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_PL_Transfer
(
const Uint32_t channel,           /**< [in]  Channel number */
const Uint32_t device,            /**< [in]  Device to transfer with */
const Uint8_t * const cmdBuffer,  /**< [in]  Command data */
const Uint32_t cmdCount,          /**< [in]  Command byte count */
const Uint8_t * const txBuffer,   /**< [in]  Transmit data */
const Uint32_t dataCount          /**< [in]  Data byte count */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_SPI_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (device >= d_SPI_PL_MAX_DEVICES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (cmdBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((cmdCount < 1u) || (cmdCount > CMD_COUNT_MAX))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, cmdCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (dataCount > DATA_COUNT_MAX)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 6, dataCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (spiStatus[channel].state == d_SPI_PL_STATUS_INIT)
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    status = d_STATUS_NOT_INITIALISED;
  }
  else if (spiStatus[channel].state != d_SPI_PL_STATUS_READY)
  {
    status = d_STATUS_DEVICE_BUSY;
  }
  else
  {
    status = d_FCU_IocAddressCheck(d_SPI_PL_Definition[channel].baseAddress);
    if (status == d_STATUS_SUCCESS)
    {
      status = transfer(channel, device, cmdBuffer, cmdCount, txBuffer, dataCount);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_PL_Result -->

  Get result of transfer request.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_PL_Result
(
const Uint32_t channel,           /**< [in]  Channel number */
d_SPI_PL_State_t * const pStatus, /**< [out] Transfer status */
Uint8_t * const rxBuffer,         /**< [out] Storage for received data */
const Uint32_t bufferLength,      /**< [in]  Length of the receive buffer */
Uint32_t * const pCount           /**< [out] Received byte count */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_SPI_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = d_FCU_IocAddressCheck(d_SPI_PL_Definition[channel].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t statusReg;

    *pStatus = spiStatus[channel].state;
    *pCount = 0;

    if (spiStatus[channel].state == d_SPI_PL_STATUS_BUSY)
    {
      statusReg = spiRegisterRead(channel, REGISTER_OFFSET_STATUS);
      if ((statusReg & 0x010000u) == 0u)
      {
   	    spiStatus[channel].state = d_SPI_PL_STATUS_COMPLETE;
      }
      ELSE_DO_NOTHING
    }
    ELSE_DO_NOTHING

    if (spiStatus[channel].state == d_SPI_PL_STATUS_COMPLETE)
    {
      Uint32_t index;

      statusReg = spiRegisterRead(channel, REGISTER_OFFSET_STATUS);
      Uint32_t rxcount = (statusReg >> 8u) & 0x7Fu;
      *pCount = spiStatus[channel].dataCount;
      if (bufferLength < spiStatus[channel].dataCount)
      {
  	    spiStatus[channel].dataCount = bufferLength;
      }
      ELSE_DO_NOTHING
      /* Read received data */
      for (index = 0; index < rxcount; index++)
      {
        Uint32_t readVal = spiRegisterRead(channel, REGISTER_OFFSET_FIFO);
        if (rxBuffer != NULL)
        {
          Uint32_t ref = 0;
          while ((((index * 4u) + ref) < spiStatus[channel].dataCount) && (ref < 4u))
          {
            rxBuffer[(index * 4u) + ref] = (Uint8_t)(readVal & 0xFFu);
            readVal = readVal >> 8u;
            ref++;
          }
        }
        ELSE_DO_NOTHING
      }
      spiStatus[channel].state = d_SPI_PL_STATUS_READY;
      *pStatus = spiStatus[channel].state;
    } /* end "if (spiStatus[channel] == d_SPI_PL_STATUS_BUSY)" */
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_PL_Status -->

  Get interface status.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_PL_Status
(
const Uint32_t channel,           /**< [in]  Channel number */
d_SPI_PL_State_t * const pStatus  /**< [out] Pointer to storage for status */
)
{
  d_Status_t status;

  if (channel >= d_SPI_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = d_FCU_IocAddressCheck(d_SPI_PL_Definition[channel].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    *pStatus = spiStatus[channel].state;

    if (spiStatus[channel].state == d_SPI_PL_STATUS_BUSY)
    {
      Uint32_t statusReg = spiRegisterRead(channel, REGISTER_OFFSET_STATUS);
      if ((statusReg & 0x010000u) == 0u)
      {
        spiStatus[channel].state = d_SPI_PL_STATUS_COMPLETE;
        *pStatus = spiStatus[channel].state;
      }
      else
      {
        DO_NOTHING();
      }
    }
    else
    {
      DO_NOTHING();
    }
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  return status;
}

/*********************************************************************//**
  <!-- d_SPI_PL_InterruptHandler -->

  Interrupt handler.
*************************************************************************/
void                              /** \return None */
d_SPI_PL_InterruptHandler
(
const Uint32_t channel            /**< [in]  Channel number */
)
{
  if (channel >= d_SPI_PL_Count)
  {
    // gcov-jst 3 It is not practical to generate this error during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  if (initialised[channel] != d_TRUE)
  {
    // gcov-jst 3 It is not practical to generate this error during bench testing.
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_SPI_PL_Definition[channel].baseAddress);
  if (status != d_STATUS_SUCCESS)
  {
    // gcov-jst 2 It is not practical to generate this error during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  
  /* Read the interrupt status register */
  Uint32_t isr = spiRegisterRead(channel, REGISTER_OFFSET_IRQ);

  /* Clear those that specifically need to be cleared */
  spiRegisterWrite(channel, REGISTER_OFFSET_IRQ, 0u);  // need literals for IRQ bits

  /* Reset if FIFO overflow */
  if ((isr & 0x04u) != 0u)
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    (void)d_SPI_PL_Reset(channel);
  }
  else
  {
    DO_NOTHING();
  }

  /* check for end of transmit */
  if ((isr & 0x01u) != 0u)
  {
    if (d_SPI_PL_Definition[channel].eventHandler != NULL)
    {
    	d_SPI_PL_Definition[channel].eventHandler(channel);
    }
    else
    {
      DO_NOTHING();
    }
  }
  else
  {
    DO_NOTHING();
  }

  /* check for device interrupt */
  if (((isr & 0xFF0000u) != 0u) && (d_SPI_PL_Definition[channel].deviceHandler != NULL))
  {
    DeviceInterrupt(channel, isr);
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- DeviceInterrupt -->

  Process a device interrupt.
*************************************************************************/
static void
DeviceInterrupt
(
const Uint32_t channel,
const Uint32_t isr
)
{
  for (Uint32_t device = 0; device < d_SPI_PL_MAX_DEVICES; device++)
  {
    if ((isr & (0x10000u << device)) != 0u)
    {
      d_SPI_PL_Definition[channel].deviceHandler(channel, device);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing. The MAC Flash does not have a device interrupt.
      DO_NOTHING();
    }
  }

  return;
}
 
/*********************************************************************//**
  <!-- transfer -->

  Perform an SPI data transfer.
*************************************************************************/
static d_Status_t                 /** \return Status of operation */
transfer
(
const Uint32_t channel,           /**< [in]  Channel number */
const Uint32_t device,            /**< [in]  Device to transfer with */
const Uint8_t * const cmdBuffer,  /**< [in]  Command data */
const Uint32_t cmdCount,          /**< [in]  Command byte count */
const Uint8_t * const txBuffer,   /**< [in]  Transmit data */
const Uint32_t dataCount          /**< [in]  Data byte count */
)
{
  Uint32_t index;
  
  spiStatus[channel].state = d_SPI_PL_STATUS_BUSY;

  for (index = 0; index < (cmdCount + 3u) / 4u; index++)
  {
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    spiRegisterWrite(channel, REGISTER_OFFSET_CMD0 + (index * 4u), *(const Uint32_t *)&cmdBuffer[index * 4u]);
  }

  /* Write the transmit data to the FIFO */
  if (txBuffer == NULL)
  {
    for (index = 0; index < (dataCount + 3u) / 4u; index++)
    {
      spiRegisterWrite(channel, REGISTER_OFFSET_FIFO, 0xFFFFFFFFu);
    }
  }
  else
  {
    for (index = 0; index < (dataCount + 3u) / 4u; index++)
    {
      // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
      spiRegisterWrite(channel, REGISTER_OFFSET_FIFO, *(const Uint32_t *)&txBuffer[index * 4u]);
    }
  }

  spiStatus[channel].dataCount = dataCount;
  if (d_SPI_PL_Definition[channel].excludeCommandResponse == d_FALSE)
  {
    spiStatus[channel].dataCount = spiStatus[channel].dataCount + cmdCount;
  }
  else
  {
    DO_NOTHING();
  }

  Uint32_t startCmd = (dataCount << 20u) | ((cmdCount - 1u) << 16u) | (device << 5u) | 0x07u | ((Uint32_t)d_SPI_PL_Definition[channel].excludeCommandResponse << 7u);

  /* Start transmission */
  spiRegisterWrite(channel, REGISTER_OFFSET_CTRL, startCmd);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- spiRegisterRead -->

  Read an SPI register.
*************************************************************************/
static Uint32_t                   /** \return Register value */
spiRegisterRead
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t regOffset          /**< [in] Register address offset */
)
{
  Uint32_t value;

  value = d_GEN_RegisterRead(d_SPI_PL_Definition[channel].baseAddress + regOffset);

  return value;
}

/*********************************************************************//**
  <!-- spiRegisterWrite -->

  Write an SPI register.
*************************************************************************/
static void                       /** \return None */
spiRegisterWrite
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t regOffset,         /**< [in] Register address offset */
const Uint32_t value              /**< [in] Value to write */
)
{
  d_GEN_RegisterWrite(d_SPI_PL_Definition[channel].baseAddress + regOffset, value);

  return;
}

