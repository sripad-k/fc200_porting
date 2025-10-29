/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_can_holt

  Abstract           : CAN interface using Holt HI-3110 device via SPI.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-213
                       SDD References: 136T-2200-131100-001-D22 SWDES-549
                                                                SWDES-625
                                                                SWDES-626
                                                                SWDES-627
                                                                SWDES-628
                                                                SWDES-629
                                                                SWDES-630
                                                                SWDES-631
                                                                SWDES-632
                                                                SWDES-633
                                                                SWDES-634
                                                                SWDES-635
                                                                SWDES-636
                                                                SWDES-637
                                                                SWDES-638

\note
  CSC ID             : SWDES-64
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"               /* Data types */
#include "soc/defines/d_common_status.h"              /* Error status */
#include "sru/can_holt/d_can_holt.h"                  /* Discrete driver */
#include "sru/can_holt/d_can_holt_cfg.h"              /* Discrete driver config */
#include "soc/timer/d_timer.h"                        /* Timer */
#include "soc/spi/d_spi.h"                            /* SPI interface */
#include "soc/spi/d_spi_cfg.h"                        /* SPI configuration */
#include "soc/interrupt_manager/d_int_irq_handler.h"  /* Interrupt handler */
#include "soc/interrupt_manager/d_int_critical.h"     /* Critical error definitions */
#include "kernel/error_handler/d_error_handler.h"     /* Error handler */
#include "kernel/general/d_gen_register.h"            /* Register functions */

#include "xparameters.h"
#include "xscugic.h"

/***** Macros ***********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised[d_CAN_HOLT_MAX_INTERFACES] =
{
  d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE, d_FALSE
};

/* Buffers to hold messages to send and receive during loopback test */
static d_CAN_HOLT_Message_t txMessage;
static d_CAN_HOLT_Message_t rxMessage;

static Bool_t interruptEnabled[d_CAN_HOLT_MAX_INTERFACES];

/***** Function Declarations ********************************************/

static d_Status_t canSend(const Uint32_t channel, const Uint8_t * const pFrame, const Uint32_t frameLength);
static d_Status_t canReceive(const Uint32_t channel, Uint8_t * const pFrame);
static d_Status_t messageValidate(const d_CAN_HOLT_Message_t * const pMessage);
static d_Status_t filterValidate(const d_CAN_HOLT_Filter_t * const pFilter);
static d_Status_t loopback(const Uint32_t channel);
static d_Status_t messageCompare(void);
static d_Status_t commandWrite(const Uint32_t channel, const Uint8_t command);
static d_Status_t registerRead(const Uint32_t channel, const Uint8_t command, const Uint32_t count, Uint8_t * const pValue);
static d_Status_t registerWrite(const Uint32_t channel, const Uint8_t command, const Uint32_t count, const Uint8_t * const pValue);
static d_Status_t registerByteWrite(const Uint32_t channel, const Uint8_t command, const Uint8_t value);
static Uint8_t interruptsEnabled(const Uint32_t channel);
static d_Status_t validateConfiguration(const Uint32_t channel);
static void extractFilter(const Uint8_t * const pData, d_CAN_HOLT_Filter_t * const pFilter);
static void insertFilter(const d_CAN_HOLT_Filter_t * const pFilter, Uint8_t * const pData);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_CAN_HOLT_Initialise -->

  Verify configuration and initialise data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_HOLT_Initialise
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  d_Status_t status;

  if (d_CAN_HOLT_Count > d_CAN_HOLT_MAX_INTERFACES)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_UNKNOWN, 0, 1, d_CAN_HOLT_Count, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = validateConfiguration(channel);

  if (status == d_STATUS_SUCCESS)
  {
    status = d_SPI_Initialise(d_CAN_HOLT_Configuration[channel].spiChannel);
  }
  else
  {
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    Uint8_t statf = 0x00u;

    /* Set initialised to true to allow master reset to complete */
    initialised[channel] = d_TRUE;
    status = d_CAN_HOLT_MasterReset(channel);
    initialised[channel] = d_FALSE;
    
    if (status == d_STATUS_SUCCESS)
    {
      status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_STATF, 1, &statf);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }

    if ((status == d_STATUS_SUCCESS) && (statf != 0x82u))
    {
      status = d_STATUS_DEVICE_NOT_FOUND;
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

  if (status == d_STATUS_SUCCESS)
  {
    interruptEnabled[channel] = d_FALSE;
    initialised[channel] = d_TRUE;
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_MasterReset -->

  Reset the CAN controller.
*************************************************************************/
d_Status_t        /** \return Success or Failure */
d_CAN_HOLT_MasterReset
(
const Uint32_t channel   /**< [in] CAN channel number */
)
{
  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  d_Status_t status = commandWrite(channel, d_CAN_HOLT_COMMAND_MASTER_RESET);

  (void)registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_BTR0, (d_CAN_HOLT_Configuration[channel].clockDivisor - 1u) |
                                                                  ((d_CAN_HOLT_Configuration[channel].syncJumpWidth - 1u) << 6u));
  (void)registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_BTR1, 0x00u |
                                                                  ((d_CAN_HOLT_Configuration[channel].secondTimeSegment - 1u) << 4u) |
                                                                  (d_CAN_HOLT_Configuration[channel].firstTimeSegment - 1u));
  (void)registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_CTRL1, 0x80u);

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_ModeSet -->

  Set operating mode.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_HOLT_ModeSet
(
const Uint32_t channel,           /**< [in] CAN channel number */
const d_CAN_HOLT_Mode_t mode      /**< [in] Required mode */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint8_t ctrl0;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (mode >= d_CAN_HOLT_MODE_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, mode, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_CTRL0, 1, &ctrl0);
  if (status == d_STATUS_SUCCESS)
  {
    ctrl0 = ctrl0 & 0x1Fu;
    ctrl0 = (Uint8_t)(ctrl0 | ((Uint32_t)mode << 5u));
    status = registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_CTRL0, ctrl0);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_ModeGet -->

  Get operating mode.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_HOLT_ModeGet
(
const Uint32_t channel,           /**< [in]  CAN channel number */
d_CAN_HOLT_Mode_t * const pMode   /**< [out] Pointer to storage for mode */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint8_t ctrl0;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pMode == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  ctrl0 = 0xFFu;
  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_CTRL0, 1, &ctrl0);
  if (status == d_STATUS_SUCCESS)
  {
    ctrl0 = (Uint8_t)(((Uint32_t)ctrl0 >> 5u) & 0x07u);
    if (ctrl0 < 4u)
    {
      *pMode = ctrl0;
    }
    else
    {
      *pMode = d_CAN_HOLT_MODE_INITIALISE;
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
  <!-- d_CAN_HOLT_FilterSet -->

  Set filter registers.
*************************************************************************/
d_Status_t                                  /** \return Status of operation */
d_CAN_HOLT_FilterSet
(
const Uint32_t channel,                     /**< [in] CAN channel number */
const Uint32_t entry,                       /**< [in] Filter entry number */
const d_CAN_HOLT_Filter_t * const pFilter,  /**< [in] Filter definition */
const d_CAN_HOLT_Filter_t * const pMask     /**< [in] Mask definition */
)
{
  d_Status_t status;
  Uint8_t canData[6];
  Uint8_t command;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (entry >= 8u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, entry, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = filterValidate(pFilter);

  if (status == d_STATUS_SUCCESS)
  {
    status = filterValidate(pMask);
  }
  else
  {
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    command = (Uint8_t)(d_CAN_HOLT_COMMAND_WRITE_FILTER_0 + (entry * 2u));
    /* Fix anomaly in addressing */
    if (entry == 7u)
    {
      command = command + 2u;
    }
    else
    {
      DO_NOTHING();
    }
    insertFilter(pFilter, &canData[0]);
    status = registerWrite(channel, command, 6, &canData[0]);
  }
  else
  {
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    command = (Uint8_t)(d_CAN_HOLT_COMMAND_WRITE_MASK_0 + (entry * 2u));
    /* Fix anomaly in addressing */
    if (entry > 5u)
    {
      command = command + 2u;
    }
    else
    {
      DO_NOTHING();
    }
    insertFilter(pMask, &canData[0]);
    status = registerWrite(channel, command, 6, &canData[0]);
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_FilterGet -->

  Get filter registers.
*************************************************************************/
d_Status_t                            /** \return Status of operation */
d_CAN_HOLT_FilterGet
(
const Uint32_t channel,               /**< [in]  CAN channel number */
const Uint32_t entry,                 /**< [in]  Filter entry number */
d_CAN_HOLT_Filter_t * const pFilter,  /**< [out] Pointer to storage for the filter definition */
d_CAN_HOLT_Filter_t * const pMask     /**< [out] Pointer to storage for the filter mask */
)
{
  d_Status_t status;
  Uint8_t canData[6];

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (entry >= 8u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, entry, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFilter == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pMask == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint8_t command = (Uint8_t)(d_CAN_HOLT_COMMAND_READ_FILTER_0 + (entry * 2u));
  /* Fix anomaly in addressing */
  if (entry == 7u)
  {
    command = command + 2u;
  }
  else
  {
    DO_NOTHING();
  }
  status = registerRead(channel, command, 6, &canData[0]);

  if (status == d_STATUS_SUCCESS)
  {
    extractFilter(&canData[0], pFilter);

    command = (Uint8_t)(d_CAN_HOLT_COMMAND_READ_MASK_0 + (entry * 2u));
    /* Fix anomaly in addressing */
    if (entry > 5u)
    {
      command = command + 2u;
    }
    else
    {
      DO_NOTHING();
    }
    status = registerRead(channel, command, 6, &canData[0]);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    extractFilter(&canData[0], pMask);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_FilterEnable -->

  Enable filteringt.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_HOLT_FilterEnable
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  Uint8_t ctrl1Value;
  d_Status_t status;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_CTRL1, 1, &ctrl1Value);
  if (status == d_STATUS_SUCCESS)
  {
    ctrl1Value |= 0x10u;
    status = registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_CTRL1, ctrl1Value);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_FilterDisable -->

  Disable filtering.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_HOLT_FilterDisable
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  Uint8_t ctrl1Value;
  d_Status_t status;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_CTRL1, 1, &ctrl1Value);
  if (status == d_STATUS_SUCCESS)
  {
    ctrl1Value &= 0xEFu;
    status = registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_CTRL1, ctrl1Value);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}


/*********************************************************************//**
  <!-- d_CAN_HOLT_SelfTest -->

  Perform a CAN loopback test.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_HOLT_SelfTest
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  d_Status_t status;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  status = d_CAN_HOLT_MasterReset(channel);

  if (status == d_STATUS_SUCCESS)
  {
    status = d_CAN_HOLT_ModeSet(channel, d_CAN_HOLT_MODE_LOOPBACK);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = loopback(channel);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  (void)d_CAN_HOLT_MasterReset(channel);

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_SendMessage -->

  Send a message frame.
*************************************************************************/
d_Status_t                                   /** \return Status of operation */
d_CAN_HOLT_SendMessage
(
const Uint32_t channel,                      /**< [in] CAN channel number */
const d_CAN_HOLT_Message_t * const pMessage  /**> [in] Message to send */
)
{
  d_Status_t status;
  Uint8_t canFrame[14];
  Uint32_t index;
  Uint32_t sendLength;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pMessage == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = messageValidate(pMessage);
  if (status != d_STATUS_SUCCESS)
  {
    /* Error will already have been logged */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return status;
  }

  if (pMessage->extended == d_FALSE)
  {
    canFrame[0] = 0;
    canFrame[1] = (pMessage->id >> 3u) & 0xFFu;
    canFrame[2] = ((Uint32_t)pMessage->extended << 3u) | ((Uint32_t)pMessage->remoteTxRequest << 4u) | (((Uint32_t)pMessage->id << 5u) & 0xE0u);
    canFrame[3] = pMessage->dataLength;

    for (index = 0; index < pMessage->dataLength; index++)
    {
      canFrame[index + 4u] = pMessage->data[index];
    }
    sendLength = pMessage->dataLength + 4u;
  }
  else
  {
    canFrame[0] = 0;
    canFrame[1] = (pMessage->id >> 3u) & 0xFFu;
    canFrame[2] = (((Uint32_t)pMessage->exId >> 15u) & 0x07u) |
                   ((Uint32_t)pMessage->extended << 3u) |
                   ((Uint32_t)pMessage->substituteRemoteTxRequest << 4u) |
                   (((Uint32_t)pMessage->id << 5u) & 0xE0u);
    canFrame[3] = (pMessage->exId >> 7u) & 0xFFu;
    canFrame[4] = (Uint8_t)pMessage->remoteTxRequest | ((pMessage->exId << 1u) & 0xFEu);
    canFrame[5] = pMessage->dataLength;

    for (index = 0; index < pMessage->dataLength; index++)
    {
      canFrame[index + 6u] = pMessage->data[index];
    }
    sendLength = pMessage->dataLength + 6u;
  }

  status = canSend(channel, &canFrame[0], sendLength);

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_ReceiveMessage -->

  Receive a message frame.
*************************************************************************/
d_Status_t                             /** \return Status of operation */
d_CAN_HOLT_ReceiveMessage
(
const Uint32_t channel,                /**< [in] CAN channel number */
d_CAN_HOLT_Message_t * const pMessage  /**< [out] Pointer to storage for the received message */
)
{
  d_Status_t status;
  Uint8_t canFrame[16];

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pMessage == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = canReceive(channel, &canFrame[0]);

  // Not sure if get full 16 bytes if no time tag. To be tested
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t index;
    pMessage->extended = ((Uint32_t)canFrame[4] >> 3u) & 0x01u;
    pMessage->id = ((Uint32_t)canFrame[3] << 3u) | (((Uint32_t)canFrame[4] >> 5u) & 0x07u);
    pMessage->exId = ((Uint32_t)canFrame[6] >> 1u) | ((Uint32_t)canFrame[5] << 7u) | ((Uint32_t)(canFrame[4] & 0x07u) << 15u);
    pMessage->substituteRemoteTxRequest = (Uint32_t)(canFrame[4] >> 4u) & 0x01u;;
    pMessage->remoteTxRequest = canFrame[6] & 0x01u;;
    pMessage->dataLength = canFrame[7] & 0x0Fu;
    if (pMessage->dataLength > 8u)
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing.
      pMessage->dataLength = 8u;
    }
    else
    {
      DO_NOTHING();
    }

    for (index = 0; index < pMessage->dataLength; index++)
    {
      pMessage->data[index] = canFrame[index + 8u];
    }
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_InterruptEnable -->

  Enable CAN Interrupt for channel.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_HOLT_InterruptEnable
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint8_t mask                /**< [in] Interrupt mask */
)
{
  Uint8_t inteValue;
  d_Status_t status;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_INTE, 1, &inteValue);
  if (status == d_STATUS_SUCCESS)
  {
    inteValue |= mask;
    status = registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_INTE, inteValue);
    if (inteValue != 0u)
    {
       (void)d_INT_IrqEnable(d_CAN_HOLT_Configuration[channel].interruptNumber);
       interruptEnabled[channel] = d_TRUE;
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
  <!-- d_CAN_HOLT_InterruptDisable -->

  Disable CAN Interrupt for channel.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_HOLT_InterruptDisable
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint8_t mask                /**< [in] Interrupt mask */
)
{
  Uint8_t inteValue;
  d_Status_t status;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_INTE, 1, &inteValue);
  if (status == d_STATUS_SUCCESS)
  {
    inteValue &= (Uint8_t)~mask;
    status = registerByteWrite(channel, d_CAN_HOLT_COMMAND_WRITE_INTE, inteValue);
    if (inteValue == 0u)
    {
      interruptEnabled[channel] = d_FALSE;
    }
    else
    {
      // gcov-jst 1 It is not practical to cover this path during bench testing.
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
  <!-- d_CAN_HOLT_InterruptHandler -->

   Interrupt handler for the CAN CSC.
*************************************************************************/
void                              /** \return None */
d_CAN_HOLT_InterruptHandler
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  Uint8_t PendingIntr;

  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  if (initialised[channel] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  (void)registerRead(channel, d_CAN_HOLT_COMMAND_READ_INTF, 1, &PendingIntr);
  PendingIntr &= interruptsEnabled(channel);

  /* Error interrupt handling */
  if (((PendingIntr & d_CAN_HOLT_BUSERR_MASK) != (Uint32_t)0) && (d_CAN_HOLT_Configuration[channel].errorHandler != NULL))
  // gcov-jst 4 It is not practical to generate this failure during bench testing.
  {
    Uint8_t errorStatus;
    (void)registerRead(channel, d_CAN_HOLT_COMMAND_READ_ERR, 1, &errorStatus);
    d_CAN_HOLT_Configuration[channel].errorHandler(channel, errorStatus);
  }
  ELSE_DO_NOTHING

  /*
   * Check if any of the following event interrupts are pending:
   *    - Mode Change
   *    - Wake-Up detected
   *    - Filter 1 passed a valid message
   *    - Filter 0 passed a valid message
   *
   * If so, call event callback provided by upper level.
   */
  Uint8_t EventIntr = PendingIntr & (d_CAN_HOLT_MCHG_MASK   |
                                     d_CAN_HOLT_WAKEUP_MASK |
                                     d_CAN_HOLT_F1MESS_MASK |
                                     d_CAN_HOLT_F0MESS_MASK);
  if ((EventIntr != (Uint32_t)0) && (d_CAN_HOLT_Configuration[channel].eventHandler != NULL))
  {
    d_CAN_HOLT_Configuration[channel].eventHandler(channel, EventIntr);
  }
  ELSE_DO_NOTHING

  if (((PendingIntr & d_CAN_HOLT_RXFIFO_MASK) != (Uint32_t)0) &&
      (d_CAN_HOLT_Configuration[channel].receiveHandler != NULL))
  {
    d_CAN_HOLT_Configuration[channel].receiveHandler(channel);
  }
  ELSE_DO_NOTHING

  /* A frame was transmitted successfully */
  if (((PendingIntr & d_CAN_HOLT_TXCPLT_MASK) != (Uint32_t)0u) &&
      (d_CAN_HOLT_Configuration[channel].sendHandler != NULL))
  {
    d_CAN_HOLT_Configuration[channel].sendHandler(channel);
  }
  ELSE_DO_NOTHING

  return;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_RegisterRead -->

  Read a CAN register.
*************************************************************************/
d_Status_t               /** \return Status of operation */
d_CAN_HOLT_RegisterRead
(
const Uint32_t channel,  /**< [in]  CAN channel number */
const Uint8_t command,   /**< [in]  Register read command */
Uint8_t * const pValue   /**< [out] Pointer to storage for the register value */
)
{
  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = registerRead(channel, command, 1, pValue);

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_HOLT_RegisterWrite -->

  Write a CAN register.
*************************************************************************/
d_Status_t                /** \return Status of operation */
d_CAN_HOLT_RegisterWrite
(
const Uint32_t channel,   /**< [in] CAN channel number */
const Uint8_t command,    /**< [in] Register write command */
const Uint8_t value       /**< [in] Value to write to the register */
)
{
  if (channel >= d_CAN_HOLT_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = registerWrite(channel, command, 1, &value);

  return status;
}

/*********************************************************************//**
  <!-- canSend -->

  Send a frame.
*************************************************************************/
static d_Status_t              /** \return Status of operation */
canSend
(
const Uint32_t channel,        /**< [in] CAN channel number */
const Uint8_t * const pFrame,  /**< [in] Frame to send */
const Uint32_t frameLength     /**< [in] Frame length */
)
{
  d_Status_t status;
  Uint8_t statf;

  /* Check Tx FIFO is not full */
  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_STATF, 1, &statf);
  
  if (status == d_STATUS_SUCCESS)
  {
    if ((statf & 0x40u) == 0u)
    {
      status = registerWrite(channel, d_CAN_HOLT_COMMAND_WRITE_MESSAGE, frameLength, pFrame); /* hard coded for standard frame 8 data bytes */
    }
    else
    {
      status = d_STATUS_BUFFER_FULL;
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
  <!-- canReceive -->

  Receive a frame.
*************************************************************************/
static d_Status_t        /** \return Status of operation */
canReceive
(
const Uint32_t channel,  /**< [in]  CAN channel number */
Uint8_t * const pFrame   /**< [out] Pointer to storage for reveived frame */
)
{
  d_Status_t status;
  Uint8_t statf;

  status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_STATF, 1, &statf);
  if (status == d_STATUS_SUCCESS)
  {
    if ((statf & 0x02u) == 0u)
    {
      status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_MESSAGE, 16, pFrame); /* this is read without time tag */
    }
    else
    {
      status = d_STATUS_NO_DATA;
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
  <!-- messageValidate -->

  Validate the message elements.
*************************************************************************/
static d_Status_t                            /** \return Status of operation */
messageValidate
(
const d_CAN_HOLT_Message_t * const pMessage  /**< [in] Message definition */
)
{
  if ((pMessage->extended != d_TRUE) && (pMessage->extended != d_FALSE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 1, pMessage->extended, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pMessage->id > 0x7FFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 2, pMessage->id, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (((pMessage->extended == d_FALSE) && (pMessage->exId > 0x0u)) || ((pMessage->extended == d_TRUE) && (pMessage->exId > 0x3FFFFu)))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 3, pMessage->exId, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((pMessage->substituteRemoteTxRequest != d_TRUE) && (pMessage->substituteRemoteTxRequest != d_FALSE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 4, pMessage->substituteRemoteTxRequest, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((pMessage->remoteTxRequest != d_TRUE) && (pMessage->remoteTxRequest != d_FALSE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 5, pMessage->remoteTxRequest, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pMessage->dataLength > 8u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 6, pMessage->dataLength, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- filterValidate -->

  Validate the filter structure elements.
*************************************************************************/
static d_Status_t                          /** \return Status of operation */
filterValidate
(
const d_CAN_HOLT_Filter_t * const pFilter  /**< [in] Filter definition */
)
{
  if (pFilter == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((pFilter->extended != d_TRUE) && (pFilter->extended != d_FALSE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 1, pFilter->extended, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFilter->id > 0x7FFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 2, pFilter->id, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFilter->exId > 0x3FFFFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 3, pFilter->exId, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- loopback -->

  Perform the loopback.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
loopback
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint32_t index;
  Uint8_t statf;

  txMessage.extended = d_FALSE;
  txMessage.id = 1234;
  txMessage.exId = 0;
  txMessage.substituteRemoteTxRequest = d_FALSE;
  txMessage.remoteTxRequest = d_FALSE;
  txMessage.dataLength = 8;
  for (index = 0; index < txMessage.dataLength; index++)
  {
    txMessage.data[index] = index;
  }
  status = d_CAN_HOLT_SendMessage(channel, &txMessage);

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime;
    Uint32_t elapsedTime;

    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);

    /* Wait until the frame arrives RX FIFO via internal loop back. */
    do
    {
      status = registerRead(channel, d_CAN_HOLT_COMMAND_READ_STATF, 1, &statf);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && ((statf & 0x02u) != 0u) && (elapsedTime < 500u));

    /* Receive the frame. */
    status = d_CAN_HOLT_ReceiveMessage(channel, &rxMessage);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = messageCompare();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- messageCompare -->

  Compare transmit and receive messages.
*************************************************************************/
static d_Status_t    /** \return Status of operation */
messageCompare
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint32_t index;

  if ((rxMessage.extended != txMessage.extended) ||
      (rxMessage.id != txMessage.id) ||
      (rxMessage.exId != txMessage.exId) ||
      (rxMessage.substituteRemoteTxRequest != txMessage.substituteRemoteTxRequest) ||
      (rxMessage.remoteTxRequest != txMessage.remoteTxRequest) ||
      (rxMessage.dataLength != txMessage.dataLength))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }

  for (index = 0; index < rxMessage.dataLength; index++)
  {
    if (rxMessage.data[index] != txMessage.data[index])
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_FAILURE;
    }
    else
    {
      DO_NOTHING();
    }
  }

  return status;
}

/*********************************************************************//**
  <!-- commandWrite -->

  Send a single byte command.
*************************************************************************/
static d_Status_t        /** \return Status of operation */
commandWrite
(
const Uint32_t channel,  /**< [in] CAN channel number */
const Uint8_t command    /**< [in] Command to send */
)
{
  d_Status_t status = d_SPI_Polled(d_CAN_HOLT_Configuration[channel].spiChannel, d_CAN_HOLT_Configuration[channel].device, &command, 1, NULL, 0, NULL);

  return status;
}

/*********************************************************************//**
  <!-- registerRead -->

  Read multiple register values.
*************************************************************************/
static d_Status_t        /** \return Status of operation */
registerRead
(
const Uint32_t channel,  /**< [in]  CAN channel number */
const Uint8_t command,   /**< [in]  Register read command */
const Uint32_t count,    /**< [in]  Number of registers to read */
Uint8_t * const pValue   /**< [out] Pointer to storage for register values  */
)
{
  d_Status_t status = d_SPI_Polled(d_CAN_HOLT_Configuration[channel].spiChannel, d_CAN_HOLT_Configuration[channel].device, &command, 1, NULL, count, pValue);

  return status;
}

/*********************************************************************//**
  <!-- registerWrite -->

  Write multiple register values.
*************************************************************************/
static d_Status_t             /** \return Status of operation */
registerWrite
(
const Uint32_t channel,       /**< [in] CAN channel number */
const Uint8_t command,        /**< [in] Register write command */
const Uint32_t count,         /**< [in] Number of registers to write */
const Uint8_t * const pValue  /**< [in] Pointer to values to write */
)
{
  d_Status_t status = d_SPI_Polled(d_CAN_HOLT_Configuration[channel].spiChannel, d_CAN_HOLT_Configuration[channel].device, &command, 1, pValue, count, NULL);

  return status;
}

/*********************************************************************//**
  <!-- registerByteWrite -->

  Write multiple register values.
*************************************************************************/
static d_Status_t             /** \return Status of operation */
registerByteWrite
(
const Uint32_t channel,       /**< [in] CAN channel number */
const Uint8_t command,        /**< [in] Register write command */
const Uint8_t value           /**< [in] Value to write */
)
{
  d_Status_t status = d_SPI_Polled(d_CAN_HOLT_Configuration[channel].spiChannel, d_CAN_HOLT_Configuration[channel].device, &command, 1, &value, 1, NULL);

  return status;
}

/*********************************************************************//**
  <!-- interruptsEnabled -->

  Read the interrupt enabled register.
*************************************************************************/
static Uint8_t           /** \return Interrupt enabled register */
interruptsEnabled
(
const Uint32_t channel   /**< [in] CAN channel number */
)
{
  Uint8_t inte;

  (void)registerRead(channel, d_CAN_HOLT_COMMAND_READ_INTE, 1, &inte);

  return inte;
}

/*********************************************************************//**
  <!-- validateConfiguration -->

  Validate configuration data. Note that the SPI related items will be
  validated when the SPI initialisation function is executed.
*************************************************************************/
static d_Status_t        /** \return Status of operation */
validateConfiguration
(
const Uint32_t channel   /**< [in] CAN channel number */
)
{
  if ((d_CAN_HOLT_Configuration[channel].clockDivisor < 1u) ||
      (d_CAN_HOLT_Configuration[channel].clockDivisor > 4u))
  {
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, d_CAN_HOLT_Configuration[channel].clockDivisor, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if ((d_CAN_HOLT_Configuration[channel].firstTimeSegment < 2u) ||
      (d_CAN_HOLT_Configuration[channel].firstTimeSegment > 16u))
  {
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, d_CAN_HOLT_Configuration[channel].firstTimeSegment, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if ((d_CAN_HOLT_Configuration[channel].secondTimeSegment < 2u) ||
      (d_CAN_HOLT_Configuration[channel].secondTimeSegment > 8u))
  {
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, channel, d_CAN_HOLT_Configuration[channel].secondTimeSegment, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if ((d_CAN_HOLT_Configuration[channel].syncJumpWidth < 1u) ||
      (d_CAN_HOLT_Configuration[channel].syncJumpWidth > 4u))
  {
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, channel, d_CAN_HOLT_Configuration[channel].syncJumpWidth, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (d_CAN_HOLT_Configuration[channel].interruptNumber >= MAXIMUM_IRQ)
  {
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, channel, d_CAN_HOLT_Configuration[channel].interruptNumber, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- extractFilter -->

  Extract the filter elements from the CAN registers.
*************************************************************************/
static void                          /** \return None */
extractFilter
(
const Uint8_t * const pData,         /**< [in]  Pointer to register values */
d_CAN_HOLT_Filter_t * const pFilter  /**< [out] Filter definition */
)
{
  pFilter->id = (((Uint32_t)pData[0] << 3u) & 0x7f8u) | (((Uint32_t)pData[1] >> 5u) & 0x07u);
  pFilter->exId = (((Uint32_t)pData[1] << 15u) & 0x38000u) | (((Uint32_t)pData[2] << 7u) & 0x7F80u) | (((Uint32_t)pData[3] >> 1u) & 0x7Fu);
  pFilter->data[0] = pData[4];
  pFilter->data[1] = pData[5];
  if ((pData[1] & 0x08u) != 0u)
  {
    pFilter->extended = d_TRUE;
  }
  else
  {
    pFilter->extended = d_FALSE;
  }

  return;
}

/*********************************************************************//**
  <!-- insertFilter -->

  Insert the filter elements into the CAN registers.
*************************************************************************/
static void                                 /** \return None */
insertFilter
(
const d_CAN_HOLT_Filter_t * const pFilter,  /**< [in]  Filter definition */
Uint8_t * const pData                       /**< [out] Pointer to data registers */
)
{
  pData[0] = (pFilter->id >> 3u) & 0xFFu;
  pData[1] = ((pFilter->id << 5u) & 0xE0u) | ((pFilter->exId >> 15u) & 0x07u);
  if (pFilter->extended == d_TRUE)
  {
    pData[1] = pData[1] | 0x08u;
  }
  else
  {
    DO_NOTHING();
  }
  pData[2] = (pFilter->exId >> 7u) & 0xFFu;
  pData[3] = (pFilter->exId << 1u) & 0xFEu;
  pData[4] = pFilter->data[0];
  pData[5] = pFilter->data[1];

  return;
}
