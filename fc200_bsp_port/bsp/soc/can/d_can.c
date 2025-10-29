/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : CAN

  Abstract           : CAN transfers

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-325
                       SDD References: 136T-2200-131100-001-D22 SWDES-117
                                                                SWDES-541
                                                                SWDES-606
                                                                SWDES-607
                                                                SWDES-608
                                                                SWDES-609
                                                                SWDES-610
                                                                SWDES-611
                                                                SWDES-612
                                                                SWDES-613
                                                                SWDES-614

\note
  CSC ID             : SWDES-47
*************************************************************************/

#define  CAN_PUBLIC_VARIABLES

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/defines/d_common_endian.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "soc/timer/d_timer.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */

#include "d_can.h"
#include "d_can_cfg.h"
#include "d_can_hw.h"

/***** Macros ***********************************************************/

/* Read a CAN register */
#define canReadReg(channel, RegOffset) \
    d_GEN_RegisterRead(d_CAN_Config[(channel)].baseAddress + (Uint32_t)(RegOffset))

/* Write a CAN register */
#define canWriteReg(channel, RegOffset, Data) \
    d_GEN_RegisterWrite((d_CAN_Config[(channel)].baseAddress + (Uint32_t)(RegOffset)), (Uint32_t)(Data))

/***** Type Definitions *************************************************/

typedef struct
{
  Bool_t initialised;
} ChannelStatus_t;

/***** Constants ********************************************************/

/***** Variables ********************************************************/

/* Buffers to hold messages to send and receive during loopback test */
static d_CAN_Message_t txMessage;
static d_CAN_Message_t rxMessage;

static ChannelStatus_t ChannelStatus[d_CAN_MAX_INTERFACES];

/***** Function Declarations ********************************************/

static d_Status_t canSend(const Uint32_t channel, const Uint32_t * const pFrame);
static d_Status_t canReceive(const Uint32_t channel, Uint32_t * const pFrame);

static d_Status_t messageValidate(const d_CAN_Message_t * const pMessage);

static d_Status_t modeSet(const Uint32_t channel, const d_CAN_Mode_t mode);
static d_Status_t loopback(const Uint32_t channel);
static d_Status_t messageCompare(void);
static d_Status_t canReset(const Uint32_t channel);

static d_Status_t setBaudRatePrescaler(const Uint32_t channel, const Uint32_t value);
static d_Status_t setTiming(const Uint32_t channel, const Uint32_t timeSegment1, const Uint32_t timeSegment2, const Uint32_t syncJumpWidth);

static Bool_t IsTxFifoFull(const Uint32_t channel);
static Bool_t IsRxEmpty(const Uint32_t channel);

static void interruptClear(const Uint32_t channel, Uint32_t mask);
static Uint32_t interruptStatus(const Uint32_t channel);
static Uint32_t interruptsEnabled(const Uint32_t channel);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_CAN_Initialise -->

  Initialise a CAN channel.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_Initialise
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = canReset(channel);

  if (status == d_STATUS_SUCCESS)
  {
    ChannelStatus[channel].initialised = d_TRUE;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_CAN_ModeSet -->

  Set operating mode.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_ModeSet
(
const Uint32_t channel,           /**< [in] CAN channel number */
const d_CAN_Mode_t mode           /**< [in] Required mode */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  d_CAN_Mode_t currentMode;

  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* No 'initialised' check here as this function is used to perform the initialisation */

  if ((mode != d_CAN_MODE_CONFIG) &&
      (mode != d_CAN_MODE_SLEEP) &&
      (mode != d_CAN_MODE_NORMAL) &&
      (mode != d_CAN_MODE_LOOPBACK) &&
      (mode != d_CAN_MODE_SNOOP))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, mode, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  (void)d_CAN_ModeGet(channel, &currentMode);

  /* If current mode is Normal Mode and the mode to enter is Sleep Mode,
     or if current mode is Sleep Mode and the mode to enter is Normal
     Mode, no transition through Configuration Mode is needed. */
  if ((currentMode == d_CAN_MODE_NORMAL) && (mode == d_CAN_MODE_SLEEP))
  {
    /* Normal Mode ---> Sleep Mode */
    canWriteReg(channel, d_CAN_REGISTER_MSR_OFFSET, d_CAN_MSR_SLEEP_MASK);
  }
  else if ((currentMode == d_CAN_MODE_SLEEP) && (mode == d_CAN_MODE_NORMAL))
  {
    /* Sleep Mode ---> Normal Mode */
    canWriteReg(channel, d_CAN_REGISTER_MSR_OFFSET, 0U);
  }
  else
  {
    status = modeSet(channel, mode);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_ModeGet -->

  Get operating mode.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_ModeGet
(
const Uint32_t channel,           /**< [in] CAN channel number */
d_CAN_Mode_t * const pMode        /**< [out] Pointer to storage for mode */
)
{
  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* No 'initialised' check here as this function is used to perform the initialisation */

  if (pMode == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  *pMode = d_CAN_MODE_UNKNOWN;
  Uint32_t statusRegister = canReadReg(channel, d_CAN_REGISTER_SR_OFFSET);
  if ((statusRegister & d_CAN_SR_CONFIG_MASK) != 0u)
  {
    *pMode = d_CAN_MODE_CONFIG;
  }
  else if ((statusRegister & d_CAN_SR_SLEEP_MASK) != 0u)
  {
    *pMode = d_CAN_MODE_SLEEP;
  }
  else if ((statusRegister & d_CAN_SR_NORMAL_MASK) != 0u)
  {
    if ((statusRegister & d_CAN_SR_SNOOP_MASK) != 0u)
    {
      *pMode = d_CAN_MODE_SNOOP;
    }
    else
    {
      *pMode = d_CAN_MODE_NORMAL;
    }
  }
  else if ((statusRegister & d_CAN_SR_LBACK_MASK) != 0u)
  {
    *pMode = d_CAN_MODE_LOOPBACK;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    *pMode = d_CAN_MODE_UNKNOWN;
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_CAN_SoftwareReset -->

  Perform a software reset.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_SoftwareReset
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, d_CAN_SRR_SRST_MASK);
  d_Status_t status = canReset(channel);
  
  return status;
}

/*********************************************************************//**
  <!-- d_CAN_SelfTest -->

  Perform a CAN loopback test.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_SelfTest
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  d_CAN_Mode_t mode;

  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  (void)canReset(channel);

  /* Ensure that controller went to CONFIG mode */
  (void)d_CAN_ModeGet(channel, &mode);
  if (mode != d_CAN_MODE_CONFIG)
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
    /* Setup Baud Rate Prescaler Register (BRPR) and Bit Timing Register */
    (void)setBaudRatePrescaler(channel, d_CAN_Config[channel].clockDivisor - 1u);
    (void)setTiming(channel, d_CAN_Config[channel].firstTimeSegment, d_CAN_Config[channel].secondTimeSegment, d_CAN_Config[channel].syncJumpWidth);

    /* Enter the loop back mode. */
    status = d_CAN_ModeSet(channel, d_CAN_MODE_LOOPBACK);
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

  /* Reset device again before returning to the caller. */
  (void)canReset(channel);

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_SendMessage -->

  Send a message frame.
*************************************************************************/
d_Status_t                              /** \return Status of operation */
d_CAN_SendMessage
(
const Uint32_t channel,                 /**< [in] CAN channel number */
const d_CAN_Message_t * const pMessage  /**< [in] Pointer to message to send */
)
{
  d_Status_t status;
  Uint32_t frame[4];
  
  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
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

  frame[0] = (Uint32_t)pMessage->remoteTxRequest |
             ((Uint32_t)pMessage->exId << 1u) |
             ((Uint32_t)pMessage->extended << 19u) |
             ((Uint32_t)pMessage->substituteRemoteTxRequest << 20u) |
             ((Uint32_t)pMessage->id << 21u);
  frame[1] = pMessage->dataLength << 28u;
  
  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  frame[2] = *(const Uint32_t *)&pMessage->data[0];
  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  frame[3] = *(const Uint32_t *)&pMessage->data[4];

  status = canSend(channel, &frame[0]);
  
  return status;
}

/*********************************************************************//**
  <!-- d_CAN_ReceiveMessage -->

  Receive a message frame.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_CAN_ReceiveMessage
(
const Uint32_t channel,           /**< [in] CAN channel number */
d_CAN_Message_t * const pMessage  /**< [out] Pointer to storage for received message */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint32_t frame[4];
  
  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
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

  status = canReceive(channel, &frame[0]);

  if (status == d_STATUS_SUCCESS)
  {
    pMessage->extended = (frame[0] >> 19u) & 0x01u;
    pMessage->id = (frame[0] >> 21u) & 0x7FFu;
    pMessage->exId = (frame[0] >> 1u) & 0x3FFFu;
    pMessage->substituteRemoteTxRequest = (frame[0] >> 20u) & 0x01u;;
    pMessage->remoteTxRequest = frame[0] & 0x01u;;
    pMessage->dataLength = (frame[1] >> 28u) & 0x0Fu;
    if (pMessage->dataLength > 8u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      pMessage->dataLength = 8u;
    }

    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    *(Uint32_t *)&pMessage->data[0] = frame[2];
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    *(Uint32_t *)&pMessage->data[4] = frame[3];
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_CAN_InterruptEnable -->

   Enable CAN Interrupt for channel.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_InterruptEnable
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t mask               /**< [in] Interrupt mask */
)
{
  Uint32_t IntrValue;

  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  IntrValue = canReadReg(channel, d_CAN_REGISTER_IER_OFFSET);
  IntrValue |= mask;
  canWriteReg(channel, d_CAN_REGISTER_IER_OFFSET, IntrValue);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_CAN_InterruptDisable -->

   Disable CAN Interrupt for channel.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_InterruptDisable
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t mask               /**< [in] Interrupt mask */
)
{
  Uint32_t IntrValue;

  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Write to the IER to disable the specified interrupts. */
  IntrValue = interruptsEnabled(channel);
  IntrValue &= ~mask;
  canWriteReg(channel,  d_CAN_REGISTER_IER_OFFSET, IntrValue);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_CAN_InterruptHandler -->

   Interrupt handler for the CAN CSC.
*************************************************************************/
void                              /** \return None */
d_CAN_InterruptHandler
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  static Bool_t has1stUnhandledErrorBeenLogged = d_FALSE;
  Uint32_t PendingIntr;
  Uint32_t EventIntr;

  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  PendingIntr = interruptStatus(channel);
  PendingIntr &= interruptsEnabled(channel);
  Uint32_t ErrorStatus = canReadReg(channel, d_CAN_REGISTER_ESR_OFFSET);

  /* Clear all pending interrupts. Rising Edge interrupt */
  interruptClear(channel, PendingIntr);

  /* Error interrupt handling */
  if ((PendingIntr & d_CAN_IXR_ERROR_MASK) != (Uint32_t)0)
  {
    if (d_CAN_Config[channel].errorHandler != NULL)
    {
      // gcov-jst 4 It is not practical to generate this failure during bench testing.
      d_CAN_Config[channel].errorHandler(channel, ErrorStatus);
    }
    else if (has1stUnhandledErrorBeenLogged != d_TRUE)
    {
      d_ERROR_Logger(d_STATUS_DEVICE_ERROR, d_ERROR_CRITICALITY_NON_CRITICAL, 3, channel, ErrorStatus, 0);
      has1stUnhandledErrorBeenLogged = d_TRUE; /**< NB: Only the very first unhandled error will be logged to avoid interfering with interrupt handling.*/
    }

    /* Clear Error Status Register. */
    canWriteReg(channel, d_CAN_REGISTER_ESR_OFFSET, ErrorStatus);
  }
  ELSE_DO_NOTHING

  /*
   * Check if any following event interrupts are pending:
   *    - RX FIFO Overflow
   *    - RX FIFO Underflow
   *    - TX High Priority Buffer full
   *    - TX FIFO Full
   *    - Wake up from sleep mode
   *    - Enter sleep mode
   *    - Enter Bus off status
   *    - Arbitration is lost
   *
   * If so, call event callback provided by upper level.
   */
  EventIntr = PendingIntr & (d_CAN_IXR_RXOFLW_MASK |
                             d_CAN_IXR_RXUFLW_MASK |
                             d_CAN_IXR_TXBFLL_MASK |
                             d_CAN_IXR_TXFLL_MASK  |
                             d_CAN_IXR_WKUP_MASK   |
                             d_CAN_IXR_SLP_MASK    |
                             d_CAN_IXR_BSOFF_MASK  |
                             d_CAN_IXR_ARBLST_MASK);
  if ((EventIntr != (Uint32_t)0) && (d_CAN_Config[channel].eventHandler != NULL))
  {
    d_CAN_Config[channel].eventHandler(channel, EventIntr);

    if ((EventIntr & d_CAN_IXR_BSOFF_MASK) != (Uint32_t)0)
    {
      /* On a BUS OFF event device should be reinitialised */
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      PendingIntr = 0u;
    }
    ELSE_DO_NOTHING
  }
  ELSE_DO_NOTHING


  if (((PendingIntr & (d_CAN_IXR_RXFWMFLL_MASK | d_CAN_IXR_RXNEMP_MASK)) != (Uint32_t)0) &&
      (d_CAN_Config[channel].receiveHandler != NULL))
  {

    /*
     * This case happens when
     * A number of frames depending on the Rx FIFO Watermark
     * threshold are received.
     * And also when frame was received and is sitting in RX FIFO.
     *
     * d_CAN_IXR_RXOK_MASK is not used because the bit is set
     * just once even if there are multiple frames sitting
     * in the RX FIFO.
     *
     * d_CAN_IXR_RXNEMP_MASK is used because the bit can be
     * set again and again automatically as long as there is
     * at least one frame in RX FIFO.
     */
    d_CAN_Config[channel].receiveHandler(channel);
  }
  ELSE_DO_NOTHING

  /* A frame was transmitted successfully */
  if (((PendingIntr & (d_CAN_IXR_TXOK_MASK | d_CAN_IXR_TXFWMEMP_MASK)) != (Uint32_t)0) &&
      (d_CAN_Config[channel].sendHandler != NULL))
  {
    d_CAN_Config[channel].sendHandler(channel);
  }
  ELSE_DO_NOTHING

  return;
}

/*********************************************************************//**
  <!-- d_CAN_RegisterRead -->

  Read a CAN register.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_RegisterRead
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t regOffset,         /**< [in] Register address offset */
Uint32_t * const pValue           /**< [out] Pointer to storage for register value */
)
{
  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (regOffset > d_CAN_REGISTER_AFIR4_OFFSET)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, regOffset, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  *pValue = d_GEN_RegisterRead(d_CAN_Config[channel].baseAddress + regOffset);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_CAN_RegisterWrite -->

  Write a CAN register.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_CAN_RegisterWrite
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t regOffset,         /**< [in] Register address offset */
const Uint32_t value              /**< [in] Value to write */
)
{
  if (channel >= d_CAN_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (ChannelStatus[channel].initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (regOffset > d_CAN_REGISTER_AFIR4_OFFSET)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, regOffset, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_GEN_RegisterWrite(d_CAN_Config[channel].baseAddress + regOffset, value);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- canSend -->

  Send a frame.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
canSend
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t * const pFrame     /**< [in] Pointer to frame to be transmitted */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  /* Check for FIFO full */
  if (IsTxFifoFull(channel) == d_TRUE)
  {
  status = d_STATUS_BUFFER_FULL;
  }
  else
  {
    /* Write IDR, DLC, Data Word 1 and Data Word 2 to the CAN device */
    canWriteReg(channel, d_CAN_REGISTER_TXFIFO_ID_OFFSET, pFrame[0]);
    canWriteReg(channel, d_CAN_REGISTER_TXFIFO_DLC_OFFSET, pFrame[1]);
    canWriteReg(channel, d_CAN_REGISTER_TXFIFO_DW1_OFFSET, Xil_EndianSwap32(pFrame[2]));
    canWriteReg(channel, d_CAN_REGISTER_TXFIFO_DW2_OFFSET, Xil_EndianSwap32(pFrame[3]));
  }

  return status;
}

/*********************************************************************//**
  <!-- canReceive -->

  Receive a frame.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
canReceive
(
const Uint32_t channel,           /**< [in] CAN channel number */
Uint32_t * const pFrame           /**< [out] Pointer to storage for received frame */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (IsRxEmpty(channel) == d_TRUE)
  {
    status = d_STATUS_NO_DATA;
  }
  else
  {
    /* Read IDR, DLC, Data Word 1 and Data Word 2 from the CAN device */
    pFrame[0] = canReadReg(channel, d_CAN_REGISTER_RXFIFO_ID_OFFSET);
    pFrame[1] = canReadReg(channel, d_CAN_REGISTER_RXFIFO_DLC_OFFSET);
    pFrame[2] = Xil_EndianSwap32(canReadReg(channel, d_CAN_REGISTER_RXFIFO_DW1_OFFSET));
    pFrame[3] = Xil_EndianSwap32(canReadReg(channel, d_CAN_REGISTER_RXFIFO_DW2_OFFSET));

    /* Clear RXNEMP bit in ISR. This allows future IsRxEmpty() call
       to return correct RX FIFO occupancy/empty condition */
    interruptClear(channel, d_CAN_IXR_RXNEMP_MASK);
  }

  return status;
}

/*********************************************************************//**
  <!-- messageValidate -->

  Validate the message elements.
*************************************************************************/
static d_Status_t
messageValidate
(
const d_CAN_Message_t * const pMessage
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
  status = d_CAN_SendMessage(channel, &txMessage);

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime;
    Uint32_t elapsedTime;
    Uint32_t RxEmptyResult;

    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);

    /* Wait until the frame arrives RX FIFO via internal loop back. */
    do
    {
      RxEmptyResult = canReadReg(channel, d_CAN_REGISTER_ISR_OFFSET) & d_CAN_IXR_RXNEMP_MASK;
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((RxEmptyResult == 0u) && (elapsedTime < 200u));

    /* Receive the frame. */
    status = d_CAN_ReceiveMessage(channel, &rxMessage);
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
static d_Status_t                 /** \return Success or Failure */
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
  <!-- canReset -->

  Reset the CAN controller.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
canReset
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  d_Status_t status;

  canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, d_CAN_SRR_SRST_MASK);

  status = d_CAN_ModeSet(channel, d_CAN_MODE_CONFIG);

  if (status == d_STATUS_SUCCESS)
  {
    (void)setBaudRatePrescaler(channel, d_CAN_Config[channel].clockDivisor - 1u);
    (void)setTiming(channel, d_CAN_Config[channel].firstTimeSegment, d_CAN_Config[channel].secondTimeSegment, d_CAN_Config[channel].syncJumpWidth);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- modeSet -->

  Change mode via the mode select register.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
modeSet
(
const Uint32_t channel,           /**< [in] CAN channel number */
const d_CAN_Mode_t mode           /**< [in] Required mode */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  d_CAN_Mode_t currentMode;
  Uint32_t startTime;

  (void)d_TIMER_ElapsedMicroseconds(0, &startTime);

  /* Write to the Software Reset Register to select config mode */
  canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, 0u);

  /* Check if the device has entered Configuration Mode, if not, return to the caller. */
  (void)d_CAN_ModeGet(channel, &currentMode);

  if (currentMode != d_CAN_MODE_CONFIG)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  else
  {
    switch (mode)
    {
    case d_CAN_MODE_CONFIG:
      /* As CAN is in Configuration Mode already. */
      /* Nothing is needed to be done here. */
      break;

    case d_CAN_MODE_SLEEP:
      canWriteReg(channel, d_CAN_REGISTER_MSR_OFFSET, d_CAN_MSR_SLEEP_MASK);
      canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, d_CAN_SRR_CEN_MASK);
      break;

    case d_CAN_MODE_NORMAL:
      canWriteReg(channel, d_CAN_REGISTER_MSR_OFFSET, 0u);
      canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, d_CAN_SRR_CEN_MASK);
      break;

    case d_CAN_MODE_SNOOP:
      canWriteReg(channel, d_CAN_REGISTER_MSR_OFFSET, d_CAN_MSR_SNOOP_MASK);
      canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, d_CAN_SRR_CEN_MASK);
      break;

    default:
      canWriteReg(channel, d_CAN_REGISTER_MSR_OFFSET, d_CAN_MSR_LBACK_MASK);
      canWriteReg(channel, d_CAN_REGISTER_SRR_OFFSET, d_CAN_SRR_CEN_MASK);
      break;

    }
  }

  Uint32_t elapsedTime;
  do
  {
    (void)d_CAN_ModeGet(channel, &currentMode);
    elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
  } while ((currentMode != mode) && (elapsedTime < 200u));

  if (currentMode != mode)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- setTiming -->

  Set CAN bit timing.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
setTiming
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t timeSegment1,      /**< [in] Time segment 1 */
const Uint32_t timeSegment2,      /**< [in] Time segment 2 */
const Uint32_t syncJumpWidth      /**< [in] Sync jump width */
)
{
  d_CAN_Mode_t mode;
  d_Status_t status = d_STATUS_SUCCESS;

  (void)d_CAN_ModeGet(channel, &mode);
  if (mode != d_CAN_MODE_CONFIG)
  {
    // gcov-jst 1 This function is only called when in d_CAN_MODE_CONFIG. The check is for robustness only.
    status = d_STATUS_INVALID_MODE;
  }
  else
  {
    Uint32_t value = timeSegment1 & d_CAN_BTR_TS1_MASK;
    value = value | ((timeSegment2 << d_CAN_BTR_TS2_SHIFT) & d_CAN_BTR_TS2_MASK);
    value = value | ((syncJumpWidth << d_CAN_BTR_SJW_SHIFT) & d_CAN_BTR_SJW_MASK);

    canWriteReg(channel, d_CAN_REGISTER_BTR_OFFSET, value);
  }

  return status;
}

/*********************************************************************//**
  <!-- setBaudRatePrescaler -->

  Set CAN baud rate prescaler.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
setBaudRatePrescaler
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t value              /**< [in] Prescaler value to write */
)
{
  d_CAN_Mode_t mode;
  d_Status_t status = d_STATUS_SUCCESS;

  (void)d_CAN_ModeGet(channel, &mode);
  if (mode != d_CAN_MODE_CONFIG)
  {
    // gcov-jst 1 This function is only called when in d_CAN_MODE_CONFIG. The check is for robustness only.
    status = d_STATUS_INVALID_MODE;
  }
  else
  {
    canWriteReg(channel, d_CAN_REGISTER_BRPR_OFFSET, value & d_CAN_BRPR_BRP_MASK);
  }

  return status;
}

/*********************************************************************//**
  <!-- IsTxFifoFull -->

  Check if the transmit FIFO is full.
*************************************************************************/
static Bool_t                     /** \return Transmitter FIFO full status */
IsTxFifoFull
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  Bool_t result = d_FALSE;

  Uint32_t srReg = canReadReg(channel, d_CAN_REGISTER_SR_OFFSET);
  if ((srReg & d_CAN_SR_TXFLL_MASK) != 0u)
  {
    result = d_TRUE;
  }
  else
  {
    DO_NOTHING();
  }

  return result;
}

/*********************************************************************//**
  <!-- IsRxEmpty -->

  Check if the receive FIFO is empty.
*************************************************************************/
static Bool_t                     /** \return Receiver empty status */
IsRxEmpty
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  Bool_t result = d_TRUE;

  Uint32_t isrReg = canReadReg(channel, d_CAN_REGISTER_ISR_OFFSET);
  if ((isrReg & d_CAN_IXR_RXNEMP_MASK) != 0u)
  {
    result = d_FALSE;
  }
  else
  {
    DO_NOTHING();
  }

  return result;
}

/*********************************************************************//**
  <!-- interruptClear -->

  Clear the currently pending interrupts.
*************************************************************************/
static void                       /** \return None */
interruptClear
(
const Uint32_t channel,           /**< [in] CAN channel number */
const Uint32_t mask               /**< [in] Interrupt mask */
)
{
  Uint32_t IntrValue;

  IntrValue = interruptStatus(channel);
  IntrValue &= mask;
  canWriteReg(channel, d_CAN_REGISTER_ICR_OFFSET, IntrValue);

  return;
}

/*********************************************************************//**
  <!-- interruptStatus -->

  Read the interrupt status register.
*************************************************************************/
static Uint32_t                   /** \return Interrupt status register */
interruptStatus
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  return canReadReg(channel, d_CAN_REGISTER_ISR_OFFSET);
}

/*********************************************************************//**
  <!-- interruptsEnabled -->

  Read the interrupt enabled register.
*************************************************************************/
static Uint32_t                   /** \return Interrupt enabled register */
interruptsEnabled
(
const Uint32_t channel            /**< [in] CAN channel number */
)
{
  return canReadReg(channel, d_CAN_REGISTER_IER_OFFSET);
}
