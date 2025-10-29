/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_spi.c

  Abstract           : SPI Interface.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-246
                       SDD References: 136T-2200-131100-001-D22 SWDES-111
                                                                SWDES-112
                                                                SWDES-113
                                                                SWDES-114
                                                                SWDES-115
\note
CSC_ID             : SWDES-45
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/discrete/d_discrete.h"            /* Discrete driver */
#include "soc/defines/d_common_status.h"        /* Error status */
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "kernel/general/d_gen_register.h"         /* Register functions */
#include "soc/timer/d_timer.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"

#include "soc/spi/d_spi.h"
#include "soc/spi/d_spi_cfg.h"

#include "xparameters.h"
#include "xspips_hw.h"

/***** Type Definitions *************************************************/

/* The maximum number of devices per interface */
#define MAX_DEVICES   3u

typedef struct
{
  Uint32_t baseAddress;
  Uint32_t clkFrequency;
} spiConfig_t;

typedef struct
{
  d_SPI_State_t state;
  Uint32_t cmdCount;
  Uint32_t dataCount;
} spiStatus_t;

/***** Constants ********************************************************/

#define SPI_FIFO_SIZE    128u

/* Value to write to SSCTRL field of config register */
static const Uint32_t ssctrl[MAX_DEVICES] = {0u, 1u, 3u};

/***** Variables ********************************************************/

static spiStatus_t spiStatus[XPAR_XSPIPS_NUM_INSTANCES];

static Bool_t initialised[XPAR_XSPIPS_NUM_INSTANCES] =
{
  d_FALSE, d_FALSE
};

/***** Function Declarations ********************************************/

static d_Status_t validatePolled(const Uint32_t channel,
                                 const Uint32_t device,
                                 const Uint8_t * const cmdBuffer,
                                 const Uint32_t cmdCount,
                                 const Uint32_t dataCount);

static void interruptEnable(const Uint32_t channel, const Uint32_t mask);

static void interruptDisable(const Uint32_t channel, const Uint32_t mask);

static d_Status_t transfer(const Uint32_t channel,
                           const Uint32_t device,
                           const Uint8_t * const cmdBuffer,
                           const Uint32_t cmdCount,
                           const Uint8_t * const txBuffer,
                           const Uint32_t dataCount);

static void resetDevice(const Uint32_t channel);

static Uint32_t spiRegisterRead(const Uint32_t channel,
                                const Uint32_t regOffset);

static void spiRegisterWrite(const Uint32_t channel,
                             const Uint32_t regOffset,
                             const Uint32_t value);

static void spiRegisterMaskWrite(const Uint32_t channel,
                                 const Uint32_t regOffset,
                                 const Uint32_t mask,
                                 const Uint32_t value);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SPI_Initialise -->

  Configure SPI channel master.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_Initialise
(
const Uint32_t channel            /**< [in] Channel number to initialise */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_SPI_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (d_SPI_Config[channel].baseAddress == 0u)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 1, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (d_SPI_Config[channel].clkInactiveState >= d_SPI_CLK_INACTIVE_STATE_COUNT)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 4, d_SPI_Config[channel].clkInactiveState, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (d_SPI_Config[channel].clkPhase >= d_SPI_CLK_PHASE_COUNT)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 5, d_SPI_Config[channel].clkPhase, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  resetDevice(channel);
  spiRegisterWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_RESET_STATE);
  initialised[channel] = d_TRUE; /* Set initialised here so setting baud rate and delays does not fail */
  status = d_SPI_SetBaudrate(channel, d_SPI_Config[channel].baud);
  (void)d_SPI_SetDelays(channel, 0, 0, 0, 0);
  initialised[channel] = d_FALSE;

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t configRegister = spiRegisterRead(channel, XSPIPS_CR_OFFSET);

    configRegister = configRegister & ~(XSPIPS_CR_MODF_GEN_EN_MASK |
                                          XSPIPS_CR_MANSTRTEN_MASK |
                                          XSPIPS_CR_SSFORCE_MASK |
                                          XSPIPS_CR_SSCTRL_MASK |
                                          XSPIPS_CR_SSDECEN_MASK |
                                          XSPIPS_CR_CPHA_MASK |
                                          XSPIPS_CR_CPOL_MASK |
                                          XSPIPS_CR_MSTREN_MASK);

    configRegister = configRegister | (XSPIPS_CR_MODF_GEN_EN_MASK |
                                      XSPIPS_CR_MANSTRTEN_MASK |
                                       XSPIPS_CR_SSFORCE_MASK |
                                       XSPIPS_CR_SSCTRL_MASK |
                                       XSPIPS_CR_MSTREN_MASK);

    configRegister = configRegister | ((Uint32_t)d_SPI_Config[channel].clkInactiveState << 1u) |
                                        ((Uint32_t)d_SPI_Config[channel].clkPhase << 2u);

    spiRegisterWrite(channel, XSPIPS_CR_OFFSET, configRegister);

    /* Enable channel interrupt */
    status = d_INT_IrqEnable(d_SPI_Config[channel].interruptNumber);

    spiStatus[channel].state = d_SPI_STATUS_READY;

    initialised[channel] = d_TRUE;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_SetBaudrate -->

  Set baudrate.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_SetBaudrate
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t baud               /**< [in] Baudrate */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_SPI_Count)
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

  /* Determine required prescaler value */
  Uint32_t frequency = d_SPI_Config[channel].clkFrequency / 4u;
  Uint32_t prescaler = 1;
  while (frequency > baud)
  {
    frequency = frequency / 2u;
    prescaler++;
  }

  if (prescaler < 8u)
  {
    spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_PRESC_MASK, prescaler << XSPIPS_CR_PRESC_SHIFT);
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, baud, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_SetDelays -->

  Set delays, in units of clock cycles.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_SetDelays
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t delayNss,          /**< [in] Delay between words when CPHA = 0 */
const Uint32_t delayBtwn,         /**< [in] Delay between chip selects */
const Uint32_t delayAfter,        /**< [in] Delay between words */
const Uint32_t delayInit          /**< [in] Delay between chip select and first word */
)
{
  if (channel >= d_SPI_Count)
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

  if (delayNss > 255u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, delayNss, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (delayBtwn > 255u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, delayBtwn, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (delayAfter > 255u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, delayAfter, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (delayInit > 255u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, delayInit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t spiRegisterDelay =  (delayNss << 24u) | (delayBtwn << 16u) | (delayAfter << 8u) | delayInit;
  spiRegisterWrite(channel, XSPIPS_DR_OFFSET, spiRegisterDelay);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_SPI_Transfer -->

  Perform an SPI data transfer.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_Transfer
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

  if (channel >= d_SPI_Count)
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

  if (device >= MAX_DEVICES)
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

  if ((cmdCount < 1u) || (cmdCount > 8u))  /* 8 bytes is the limit of the command transfer in the PL implementation */
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, cmdCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((cmdCount + dataCount) > SPI_FIFO_SIZE)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 6, dataCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (spiStatus[channel].state != d_SPI_STATUS_READY)
  {
    status = d_STATUS_DEVICE_BUSY;
  }
  else
  {
    status = transfer(channel, device, cmdBuffer, cmdCount, txBuffer, dataCount);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_Result -->

  Get result of transfer request.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_Result
(
const Uint32_t channel,           /**< [in]  Channel number */
d_SPI_State_t * const pStatus,    /**< [out] Transfer status */
Uint8_t * const rxBuffer,         /**< [out] Storage for received data */
const Uint32_t bufferLength,      /**< [in]  Length of the receive buffer */
Uint32_t * const pCount           /**< [out] Received byte count */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_SPI_Count)
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

  *pStatus = spiStatus[channel].state;
  *pCount = 0;

  if (spiStatus[channel].state == d_SPI_STATUS_COMPLETE)
  {
    Uint32_t statusReg = spiRegisterRead(channel, XSPIPS_SR_OFFSET);
    if ((statusReg & XSPIPS_IXR_MODF_MASK) != 0u)
    {
      /* Clear the mode fail bit */
      // gcov-jst 5 It is not practical to generate this failure during bench testing.
      spiRegisterWrite(channel, XSPIPS_SR_OFFSET, XSPIPS_IXR_MODF_MASK);

      spiStatus[channel].state = d_SPI_STATUS_READY;

      status = d_STATUS_FAILURE;
    }
    ELSE_DO_NOTHING

    if ((statusReg & XSPIPS_IXR_TXOW_MASK) == XSPIPS_IXR_TXOW_MASK)
    {
      Uint32_t index;

      /* Read received data */
      for (index = 0; index < (spiStatus[channel].cmdCount + spiStatus[channel].dataCount); index++)
      {
        Uint8_t value = spiRegisterRead(channel, XSPIPS_RXD_OFFSET);
        if ((rxBuffer != NULL) && (index >= spiStatus[channel].cmdCount) && (index < (spiStatus[channel].cmdCount + bufferLength)))
        {
          rxBuffer[index - spiStatus[channel].cmdCount] = value;
        }
        ELSE_DO_NOTHING
      }
      /* Save the number of received bytes */
      *pCount = index - spiStatus[channel].cmdCount;

      spiStatus[channel].state = d_SPI_STATUS_READY;
    } /* end "if ((statusReg & 0x010000u) == 0u)" */
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    *pStatus = spiStatus[channel].state;
  } /* end "if (spiStatus[channel] == d_SPI_STATUS_COMPLETE)" */

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_Polled -->

  Perform an SPI transfer in polling mode.
*************************************************************************/
d_Status_t d_SPI_Polled
(
const Uint32_t channel,           /**< [in]  Channel number */
const Uint32_t device,            /**< [in]  Device to transfer with */
const Uint8_t * const cmdBuffer,  /**< [in]  Command data */
const Uint32_t cmdCount,          /**< [in]  Command byte count */
const Uint8_t * const txBuffer,   /**< [in]  Transmit data */
const Uint32_t dataCount,         /**< [in]  Data byte count */
Uint8_t * const rxBuffer          /**< [in]  Received data */
)
{
  d_Status_t status;
  Uint32_t statusReg;

  status = validatePolled(channel, device, cmdBuffer, cmdCount, dataCount);
  if (status != d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return status;
  }
  
  spiStatus[channel].state = d_SPI_STATUS_BUSY;

  (void)d_INT_IrqDisable(d_SPI_Config[channel].interruptNumber);

  /* Select the requested slave */
  spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_SSCTRL_MASK, ssctrl[device] << XSPIPS_CR_SSCTRL_SHIFT);

  /* Enable the device */
  spiRegisterWrite(channel, XSPIPS_ER_OFFSET, XSPIPS_ER_ENABLE_MASK);

  /* Write the command data to the FIFO */
  for (Uint32_t index = 0; index < cmdCount; index++)
  {
    spiRegisterWrite(channel, XSPIPS_TXD_OFFSET, cmdBuffer[index]);
  }

  /* Write the transmit data to the FIFO */
  if (txBuffer == NULL)
  {
    for (Uint32_t index = 0; index < dataCount; index++)
    {
      spiRegisterWrite(channel, XSPIPS_TXD_OFFSET, 0xFFu);
    }
  }
  else
  {
    for (Uint32_t index = 0; index < dataCount; index++)
    {
     spiRegisterWrite(channel, XSPIPS_TXD_OFFSET, txBuffer[index]);
    }
  }

  /* Clear all interrupts */
  spiRegisterWrite(channel, XSPIPS_SR_OFFSET, XSPIPS_IXR_WR_TO_CLR_MASK);

  interruptEnable(channel, XSPIPS_IXR_TXOW_MASK);

  /* Start transmission */
  spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_MANSTRT_MASK, XSPIPS_CR_MANSTRT_MASK);

  /* Determine time in us for transfer */
  Uint32_t timeout = ((1000000u * 8u * (cmdCount + dataCount)) / d_SPI_Config[channel].baud) + 1u;

  Uint32_t startTime = d_TIMER_ReadValueInTicks();
  Uint32_t elapsedTime;
  do
  {
    elapsedTime =  d_TIMER_ElapsedMicroseconds(startTime, NULL);
    statusReg = spiRegisterRead(channel, XSPIPS_SR_OFFSET);

  } while (((statusReg & XSPIPS_IXR_TXOW_MASK) != XSPIPS_IXR_TXOW_MASK) && (elapsedTime < (2u * timeout)));

  /* Clear those that specifically need to be cleared */
  spiRegisterWrite(channel, XSPIPS_SR_OFFSET, (statusReg & XSPIPS_IXR_WR_TO_CLR_MASK));

  if ((statusReg & XSPIPS_IXR_TXOW_MASK) == 0u)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
	  status = d_STATUS_FAILURE;
  }
  ELSE_DO_NOTHING

  /* Turn off the chip select */
  spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_SSCTRL_MASK, XSPIPS_CR_SSCTRL_MASK);

  /* Read received data */
  for (Uint32_t index = 0; index < (cmdCount + dataCount); index++)
  {
    Uint8_t value = spiRegisterRead(channel, XSPIPS_RXD_OFFSET);
    if ((rxBuffer != NULL) && (index >= cmdCount))
    {
      rxBuffer[index - cmdCount] = value;
    }
    ELSE_DO_NOTHING
  }

  spiStatus[channel].state = d_SPI_STATUS_READY;
  (void)d_INT_IrqEnable(d_SPI_Config[channel].interruptNumber);

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_Status -->

  Get interface status.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_Status
(
const Uint32_t channel,           /**< [in]  Channel number */
d_SPI_State_t * const pStatus     /**< [out] Pointer to storage for status */
)
{
  if (channel >= d_SPI_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  *pStatus = spiStatus[channel].state;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_SPI_SelfTest -->

  Test the selected device.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_SPI_SelfTest
(
const Uint32_t channel            /**< [in] Channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_SPI_Count)
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

  /* Reset */
  resetDevice(channel);

  /* Check config register */
  Uint32_t controlReg = spiRegisterRead(channel, XSPIPS_CR_OFFSET);
  if ((controlReg & XSPIPS_CR_MODF_GEN_EN_MASK) != XSPIPS_CR_MODF_GEN_EN_MASK)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    /* Check Tx FIFO empty */
    Uint32_t statusReg = spiRegisterRead(channel, XSPIPS_SR_OFFSET);
    if ((statusReg & XSPIPS_IXR_TXFULL_MASK) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_FAILURE;
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

  if (status == d_STATUS_SUCCESS)
  {
    /* Check read and write register */
    spiRegisterWrite(channel, XSPIPS_DR_OFFSET, 0x5AA5AA55u);
    Uint32_t delayReg = spiRegisterRead(channel, XSPIPS_DR_OFFSET);
    if (delayReg != 0x5AA5AA55u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_FAILURE;
    }
    else
    {
      DO_NOTHING();
    }
    spiRegisterWrite(channel, XSPIPS_DR_OFFSET, 0x0u);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  /* Reset */
  resetDevice(channel);

  return status;
}

/*********************************************************************//**
  <!-- d_SPI_InterruptHandler -->

  Interrupt handler.
*************************************************************************/
void                              /** \return None */
d_SPI_InterruptHandler
(
const Uint32_t channel            /**< [in] Channel number */
)
{
  if (channel >= d_SPI_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  else
  {
    DO_NOTHING();
  }

  /* Read the interrupt status register */
  Uint32_t isr = spiRegisterRead(channel, XSPIPS_SR_OFFSET);

  /* Clear those that specifically need to be cleared */
  spiRegisterWrite(channel, XSPIPS_SR_OFFSET, (isr & XSPIPS_IXR_WR_TO_CLR_MASK));

  /* check for end of transmit */
  if ((isr & XSPIPS_IXR_TXOW_MASK) != 0u)
  {
  /* Disable the interrupt */
    interruptDisable(channel, XSPIPS_IXR_TXOW_MASK);
    /* Turn off the chip select */
    spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_SSCTRL_MASK, XSPIPS_CR_SSCTRL_MASK);
   /* Set transfer complete */
    spiStatus[channel].state = d_SPI_STATUS_COMPLETE;
    if (d_SPI_Config[channel].eventHandler != NULL)
    {
      d_SPI_Config[channel].eventHandler(channel, spiStatus[channel].state);
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

  return;
}

/*********************************************************************//**
  <!-- validatePolled -->

  Validate the parameters to the d_SPI_Polled function.
*************************************************************************/
static d_Status_t
validatePolled
(
const Uint32_t channel,           /**< [in]  Channel number */
const Uint32_t device,            /**< [in]  Device to transfer with */
const Uint8_t * const cmdBuffer,  /**< [in]  Command data */
const Uint32_t cmdCount,          /**< [in]  Command byte count */
const Uint32_t dataCount          /**< [in]  Data byte count */
)
{
  if (channel >= d_SPI_Count)
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

  if (device >= MAX_DEVICES)
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

  if ((cmdCount < 1u) || (cmdCount > 8u))  /* 8 bytes is the limit of the command transfer in the PL implementation */
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, cmdCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((cmdCount + dataCount) > SPI_FIFO_SIZE)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 6, dataCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- interruptEnable -->

  Enable SPI Interrupt for channel.
*************************************************************************/
static void                       /** \return None */
interruptEnable
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t mask               /**< [in] interrupts to enable */
)
{
  spiRegisterWrite(channel, XSPIPS_IER_OFFSET, mask);

  return;
}

/*********************************************************************//**
  <!-- interruptDisable -->

  Disable SPI Interrupt for channel.
*************************************************************************/
static void                       /** \return None */
interruptDisable
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t mask               /**< [in] interrupts to disable */
)
{
  spiRegisterWrite(channel, XSPIPS_IDR_OFFSET, mask);

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
  d_Status_t status = d_STATUS_SUCCESS;

  spiStatus[channel].state = d_SPI_STATUS_BUSY;

  /* Select the requested slave */
  spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_SSCTRL_MASK, ssctrl[device] << XSPIPS_CR_SSCTRL_SHIFT);

  /* Enable the device */
  spiRegisterWrite(channel, XSPIPS_ER_OFFSET, XSPIPS_ER_ENABLE_MASK);

  /* Clear all interrupts */
  spiRegisterWrite(channel, XSPIPS_SR_OFFSET, XSPIPS_IXR_WR_TO_CLR_MASK);

  /* Write the command data to the FIFO */
  for (index = 0; index < cmdCount; index++)
  {
    spiRegisterWrite(channel, XSPIPS_TXD_OFFSET, cmdBuffer[index]);
  }

  /* Write the transmit data to the FIFO */
  if (txBuffer == NULL)
  {
    for (index = 0; index < dataCount; index++)
    {
      spiRegisterWrite(channel, XSPIPS_TXD_OFFSET, 0xFFu);
    }
  }
  else
  {
    for (index = 0; index < dataCount; index++)
    {
     spiRegisterWrite(channel, XSPIPS_TXD_OFFSET, txBuffer[index]);
    }
  }

  spiStatus[channel].cmdCount = cmdCount;
  spiStatus[channel].dataCount = dataCount;

  interruptEnable(channel, XSPIPS_IXR_TXOW_MASK);

  /* Start transmission */
  spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_MANSTRT_MASK, XSPIPS_CR_MANSTRT_MASK);

  return status;
}

/*********************************************************************//**
  <!-- resetDevice -->

  Reset the device.
*************************************************************************/
static void                       /** \return None */
resetDevice
(
const Uint32_t channel            /**< [in] Channel number */
)
{
  Uint32_t index;

  /* Disable the device */
  spiRegisterWrite(channel, XSPIPS_ER_OFFSET, 0u);

  /* Clear Rx FIFO */
  for (index = 0; index < SPI_FIFO_SIZE; index++)
  {
    (void)spiRegisterRead(channel, XSPIPS_RXD_OFFSET);
  }

  /* Clear mode fault */
  spiRegisterWrite(channel, XSPIPS_SR_OFFSET, XSPIPS_IXR_MODF_MASK);

  /* Enable mode fail generation */
  spiRegisterMaskWrite(channel, XSPIPS_CR_OFFSET, XSPIPS_CR_MODF_GEN_EN_MASK, XSPIPS_CR_MODF_GEN_EN_MASK);

  return;
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

  value = d_GEN_RegisterRead(d_SPI_Config[channel].baseAddress + regOffset);

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
  d_GEN_RegisterWrite(d_SPI_Config[channel].baseAddress + regOffset, value);

  return;
}

/*********************************************************************//**
  <!-- spiRegisterMaskWrite -->

  Write the masked bits of an SPI register.
*************************************************************************/
static void                       /** \return None */
spiRegisterMaskWrite
(
const Uint32_t channel,           /**< [in] Channel number */
const Uint32_t regOffset,         /**< [in] Register address offset */
const Uint32_t mask,              /**< [in] Mask of bits to write */
const Uint32_t value              /**< [in] Value to write */
)
{
  d_GEN_RegisterWriteMask(d_SPI_Config[channel].baseAddress + regOffset, mask, value);

  return;
}
