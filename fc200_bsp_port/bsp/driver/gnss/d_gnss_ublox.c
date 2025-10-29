/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : GNSS Ublox Configuration

  Abstract           : Driver for the u-blox F9 GNSS

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-333
                       SDD References: 136T-2200-131100-001-D22 SWDES-296
                                                                SWDES-297
                                                                SWDES-298
                                                                SWDES-299
                                                                SWDES-300
                                                                SWDES-301
\note
  CSC ID             : SWDES-72
*************************************************************************/

#define GNSS_UBLOX_PUBLIC_VARIABLES

/***** Includes *********************************************************/

#include "soc/uart/d_uart.h"
#include "soc/timer/d_timer.h"
#include "soc/discrete/d_discrete.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_register.h"
#include "d_gnss_ublox.h"
#include "d_gnss_ublox_cfg.h"

/***** Constants ********************************************************/

/* Width of reset pulse in milliseconds */
#define RESET_PULSE_WIDTH   200u

/* Messages with length entry above this will be ignored */
#define MAX_MESSAGE_LENGTH  100u

/* Allow space for all messages plus a contingency of 142 bytes */
#define RX_BUFFER_LENGTH   (500u)

/* Minimum message length. Header, checksum */
#define MESSAGE_LENGTH_MINIMUM    8u

#define PREAMBLE_SYNC_1 (Uint8_t)0xB5u
#define PREAMBLE_SYNC_2 (Uint8_t)0x62u

/***** Type Definitions *************************************************/

/* Function prototypes for the message process and invalidate functions */
typedef void (*processFunction_t)(const Uint8_t * const pData);
typedef void (*invalidateFunction_t)(void);

/* Constant message data */
typedef struct
{
  Uint8_t class;
  Uint8_t id;
  processFunction_t processFunction;
  invalidateFunction_t invalidateFunction;
} messageConfig_t;

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;
static Uint32_t uartChannel;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Uint8_t rxBuffer[RX_BUFFER_LENGTH];

static Uint32_t unknownMessageCount;

static Uint32_t resetStartTime;

/***** Function Declarations ********************************************/

static Uint32_t processMessage(const Uint8_t * const pData, const Uint32_t messageLength);
static void resetCheck(void);
static void checkStale(void);
static Uint32_t getMessageIndex(const Uint8_t class, const Uint8_t id);

/* Message processing functions */
static void processMonHw(const Uint8_t * const pData);
static void processTimTp(const Uint8_t * const pData);
static void processNavPvt(const Uint8_t * const pData);
static void processNavCov(const Uint8_t * const pData);
static void processNavDop(const Uint8_t * const pData);
static void processNavStatus(const Uint8_t * const pData);
static void processNavVelecef(const Uint8_t * const pData);
static void processMonSys(const Uint8_t * const pData);
static void processSecSig(const Uint8_t * const pData);

/* Message invalidate functions */
static void invalidateMonHw(void);
static void invalidateTimTp(void);
static void invalidateNavPvt(void);
static void invalidateNavCov(void);
static void invalidateNavDop(void);
static void invalidateNavStatus(void);
static void invalidateNavVelecef(void);
static void invalidateMonSys(void);
static void invalidateSecSig(void);

/* Functions to extract data from received messages */
static Uint8_t   getBool(const Uint8_t * const pData, const Uint32_t ref, const Uint32_t bit);
static Uint8_t   getEnum(const Uint8_t * const pData, const Uint32_t ref, const Uint32_t shift, const Uint8_t mask);
static Int8_t    getInt8(const Uint8_t * const pData, const Uint32_t ref);
static Uint8_t   getUint8(const Uint8_t * const pData, const Uint32_t ref);
static Uint16_t  getUint16(const Uint8_t * const pData, const Uint32_t ref);
static Uint32_t  getUint32(const Uint8_t * const pData, const Uint32_t ref);
static Int32_t   getInt32(const Uint8_t * const pData, const Uint32_t ref);
static Float32_t getFloat32(const Uint8_t * const pData, const Uint32_t ref);

/* Definition of supported messages */
static const messageConfig_t messageConfig[GNSS_UBLOX_MESSSAGE_COUNT] =
{
  {0x0Au, 0x09u, processMonHw,  invalidateMonHw},
  {0x0Du, 0x01u, processTimTp,  invalidateTimTp},
  {0x01u, 0x07u, processNavPvt, invalidateNavPvt},
  {0x01u, 0x36u, processNavCov, invalidateNavCov},
  {0x01u, 0x04u, processNavDop, invalidateNavDop},
  {0x01u, 0x03u, processNavStatus, invalidateNavStatus},
  {0x01u, 0x11u, processNavVelecef, invalidateNavVelecef},
  {0x0Au, 0x39u, processMonSys,  invalidateMonSys},
  {0x27u, 0x09u, processSecSig,  invalidateSecSig}
};

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_GNSS_Ublox_Initialise -->

  Initialise the GNSS interface.
*************************************************************************/
d_Status_t                        /** \return Success of Failure */
d_GNSS_Ublox_Initialise
(
const Uint32_t channel            /** [in] Selected GNSS UART channel */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t index;
  
  if ((channel >= d_GNSS_UART_CHANNEL_COUNT) || ((channel > 0u) && (d_GNSS_Ublox_UartChannel[channel] == 0u)))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  /* Save selected uartChannel */
  uartChannel = d_GNSS_Ublox_UartChannel[channel];
  
  /* Check that defined messages are supported */
  for (index = 0; index < d_GNSS_Ublox_MessageCount; index++)
  {
    Uint32_t messageIndex = getMessageIndex(d_GNSS_Ublox_Messages[index].class, d_GNSS_Ublox_Messages[index].id);
    if (messageIndex < GNSS_UBLOX_MESSSAGE_COUNT)
    {
      d_GNSS_Ublox_MessageStatus[messageIndex].period = d_GNSS_Ublox_Messages[index].period;
    }
    else
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, index, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_PARAMETER;
    }
  }
  
  for (index = 0; index < GNSS_UBLOX_MESSSAGE_COUNT; index++)
  {
    d_GNSS_Ublox_MessageStatus[index].lastReceiveTime = 0;
    d_GNSS_Ublox_MessageStatus[index].counter = 0;
    d_GNSS_Ublox_MessageStatus[index].interval = 0;
    d_GNSS_Ublox_MessageStatus[index].valid = d_FALSE;
    messageConfig[index].invalidateFunction();
  }

  unknownMessageCount++;

  resetStartTime = 0;
  
  returnValue = d_UART_Configure(uartChannel, 115200, d_UART_DATA_BITS_8, d_UART_PARITY_NONE, d_UART_STOP_BITS_1);

  if (returnValue == d_STATUS_SUCCESS)
  {
    /* Initialise Power output */
    (void)d_DISC_SetAsOutputPin(d_GNSS_Ublox_DiscretePower);
    returnValue = d_DISC_SetPin(d_GNSS_Ublox_DiscretePower);  /* Set output high to turn on power */
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (returnValue == d_STATUS_SUCCESS)
  {
    /* Initialise Reset output */
    (void)d_DISC_SetAsOutputPin(d_GNSS_Ublox_DiscreteReset);
    returnValue = d_DISC_ClearPin(d_GNSS_Ublox_DiscreteReset);  /* Set output low for no reset */
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_INT_IrqSetPriorityTriggerType(d_GNSS_Ublox_Interrupt, 128, d_INT_RISING_EDGE);
    (void)d_INT_IrqEnable(d_GNSS_Ublox_Interrupt);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    initialised = d_TRUE;
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_GNSS_Ublox_ProcessMessages -->

  Process messages received from the GNSS.
*************************************************************************/
void                              /** \return None */
d_GNSS_Ublox_ProcessMessages
(
void
)
{
  if (initialised == d_TRUE)
  {
    Uint32_t bytesReceived;
    Uint32_t processIndex = 0;
    Uint32_t messageLength;

    do
    {
      Uint32_t bytesToProcess;

      (void)d_UART_Receive(uartChannel, rxBuffer, RX_BUFFER_LENGTH, &bytesReceived);
      bytesToProcess = bytesReceived;
      Bool_t waiting = d_FALSE;
      while ((bytesToProcess >= MESSAGE_LENGTH_MINIMUM) && (waiting == d_FALSE))
      {
        Uint32_t discard = 1;
        /* Check for expected header */
        if ((rxBuffer[processIndex] == PREAMBLE_SYNC_1) && (rxBuffer[processIndex + 1u] == PREAMBLE_SYNC_2))
        {
          discard = 0;
          /* Get length */
          messageLength = (Uint32_t)getUint16(rxBuffer, processIndex + 4u);
          /* Check id message length is valid */
          if (messageLength < MAX_MESSAGE_LENGTH)
          {
            /* See if we have enough data to complete */
            if (bytesToProcess >= (messageLength + MESSAGE_LENGTH_MINIMUM))
            {
              discard = processMessage(&rxBuffer[processIndex], messageLength);
            }
            else
            {
              waiting = d_TRUE;
            }
          } /* end "if (messageLength < MAX_MESSAGE_LENGTH)" */
          else
          {
            /* invalid length, assume preamble bytes were part of data */
            // gcov-jst 1 It is not practical to generate this failure during bench testing.
            discard = 2;
          }
        } /* end "if ((rxBuffer[processIndex] == PREAMBLE_SYNC_1) && (rxBuffer[processIndex + 1u] == PREAMBLE_SYNC_2))" */
        processIndex = processIndex + discard;
        bytesToProcess = bytesToProcess - discard;
      } /* end "while (bytesToProcess >= MESSAGE_LENGTH_MINIMUM)" */
      (void)d_UART_Discard(uartChannel, processIndex);
    } while (bytesReceived >= RX_BUFFER_LENGTH);

    /* Check for stale messages */
    checkStale();
    
    /* Check if reset signal needs to be switched off */
    resetCheck();
    
  } /* end "if (initialised == d_TRUE)" */

  return;
}

/*********************************************************************//**
  <!-- processMessage -->

  Process a detected message from the GNSS.
*************************************************************************/
static Uint32_t                   /** \return Number of bytes to discard */
processMessage
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t messageLength      /**< [in] Length of the message */
)
{
  Uint32_t discard;
  Uint32_t index;

  if (d_GNSS_Ublox_RawReceived != NULL)
  {
    d_GNSS_Ublox_RawReceived(pData, messageLength + 8u);
  }
  else
  {
    // gcov-jst 1 It is not practical to execute this code during bench testing.
    DO_NOTHING();
  }
  
  /* check checksum */
  Uint8_t ckA = 0;
  Uint8_t ckB = 0;
  for (index = 2u; index < (messageLength + 6u); index++)
  {
    ckA = ckA + pData[index];
    ckB = ckB + ckA;
  }
  if ((pData[index] == ckA) && (pData[index + 1u] == ckB))
  {
    /* Valid message received */
    Uint32_t messageIndex = getMessageIndex(pData[2u], pData[3u]);
    if (messageIndex < GNSS_UBLOX_MESSSAGE_COUNT)
    {
      d_GNSS_Ublox_MessageStatus[messageIndex].counter++;
      d_GNSS_Ublox_MessageStatus[messageIndex].interval = d_TIMER_ElapsedMilliseconds(d_GNSS_Ublox_MessageStatus[messageIndex].lastReceiveTime, &d_GNSS_Ublox_MessageStatus[messageIndex].lastReceiveTime);
      messageConfig[messageIndex].processFunction(&pData[6u]);
      d_GNSS_Ublox_MessageStatus[messageIndex].valid = d_TRUE;
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      unknownMessageCount++;
    }
    discard = messageLength + MESSAGE_LENGTH_MINIMUM;
  }
  else
  {
    /* checksum error, assume preamble bytes were part of data */
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    discard = 2;
  }
 
  return discard;
}

/*********************************************************************//**
  <!-- d_GNSS_Ublox_1ppsInterrupt -->

  1pps interrupt handler.
*************************************************************************/
void                              /** \return None */
d_GNSS_Ublox_1ppsInterrupt
(
const Uint32_t parameter          /**< [in] Unused interrupt handler parameter */
)
{
  UNUSED_PARAMETER(parameter);
  
  if ((pD_GNSS_Ublox->timTp.valid == d_TRUE) && (d_GNSS_Ublox_1pps != NULL))
  {
    d_GNSS_Ublox_1pps();
  }
  ELSE_DO_NOTHING

  return;
}

/*********************************************************************//**
  <!-- d_GNSS_Ublox_Reset -->

  Reset the GNSS.
*************************************************************************/
void                              /** \return None */
d_GNSS_Ublox_Reset
(
void
)
{
  (void)d_DISC_SetPin(d_GNSS_Ublox_DiscreteReset);   /* Set output high to reset */
  (void)d_TIMER_ElapsedMilliseconds(0, &resetStartTime);
 
  return;
}

/*********************************************************************//**
  <!-- resetCheck -->

  Check if reset signal needs to be switched off.
*************************************************************************/
static void                       /** \return None */
resetCheck
(
void
)
{
  if (resetStartTime > 0u)
  {
    Uint32_t resetTime = d_TIMER_ElapsedMilliseconds(resetStartTime, NULL);
    if (resetTime >= RESET_PULSE_WIDTH)
    {
      (void)d_DISC_ClearPin(d_GNSS_Ublox_DiscreteReset);   /* Set output low to stop reset */
      resetStartTime = 0u;

      for (Uint32_t index = 0; index < GNSS_UBLOX_MESSSAGE_COUNT; index++)
      {
        d_GNSS_Ublox_MessageStatus[index].lastReceiveTime = 0;
        d_GNSS_Ublox_MessageStatus[index].counter = 0;
        d_GNSS_Ublox_MessageStatus[index].interval = 0;
        d_GNSS_Ublox_MessageStatus[index].valid = d_FALSE;
        messageConfig[index].invalidateFunction();
      }
    }
    ELSE_DO_NOTHING
  }
  ELSE_DO_NOTHING

  return;
}

/*********************************************************************//**
  <!-- checkStale -->

  Check for stale messages.
*************************************************************************/
static void                       /** \return None */
checkStale
(
void
)
{
  Uint32_t index;

  for (index = 0; index < GNSS_UBLOX_MESSSAGE_COUNT; index++)
  {
    if (d_GNSS_Ublox_MessageStatus[index].valid == d_TRUE)
    {
      Uint32_t elapsedTime = d_TIMER_ElapsedMilliseconds(d_GNSS_Ublox_MessageStatus[index].lastReceiveTime, NULL);
      if (elapsedTime > (d_GNSS_Ublox_MessageStatus[index].period * 2u))
      {
        // gcov-jst 2 It is not practical to ensure coverage of this path during bench testing.
        messageConfig[index].invalidateFunction();
        d_GNSS_Ublox_MessageStatus[index].valid = d_FALSE;
      }
      ELSE_DO_NOTHING
    }
    ELSE_DO_NOTHING
  }

  return;
}

/*********************************************************************//**
  <!-- getMessageIndex -->

  Get array index for message based on class and ID.
*************************************************************************/
static Uint32_t                   /** \return Index into message arrays */
getMessageIndex
(
const Uint8_t class,              /**< [in] Message class */
const Uint8_t id                  /**< [in] Message ID */
)
{
  Uint32_t index = 0;
  
  while (index < GNSS_UBLOX_MESSSAGE_COUNT)
  {
    if ((class == messageConfig[index].class) && (id == messageConfig[index].id))
    {
      break;
    }
    index++;
  }
  
  return index;
}

/*********************************************************************//**
  <!-- processMonHw -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processMonHw
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->monHw.antennaStatus = getUint8(pData, 20u);
  pD_GNSS_Ublox->monHw.powerStatus = getUint8(pData, 21u);
  pD_GNSS_Ublox->monHw.jammingState = getEnum(pData, 22u, 2u, 0x03u);
  pD_GNSS_Ublox->monHw.jammingIndicator = getUint8(pData, 45u);
  pD_GNSS_Ublox->monHw.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateMonHw -->

  Invalidate MON-HW data.
*************************************************************************/
static void                       /** \return None */
invalidateMonHw
(
void
)
{
  pD_GNSS_Ublox->monHw.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processTimTp -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processTimTp
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->timTp.towMs = getUint32(pData, 0u);
  pD_GNSS_Ublox->timTp.towSubMs = getUint32(pData, 4u);
  pD_GNSS_Ublox->timTp.week = getUint16(pData, 12u);
  pD_GNSS_Ublox->timTp.timebase = getEnum(pData, 14u, 0u, 0x01u);
  pD_GNSS_Ublox->timTp.utc = getEnum(pData, 14u, 1u, 0x01u);
  pD_GNSS_Ublox->timTp.raim = getEnum(pData, 14u, 2u, 0x03u);
  pD_GNSS_Ublox->timTp.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateTimTp -->

  Invalidate TIM-TP data.
*************************************************************************/
static void                       /** \return None */
invalidateTimTp
(
void
)
{
  pD_GNSS_Ublox->timTp.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processNavPvt -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processNavPvt
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->navPvt.iTow = getUint32(pData, 0u);
  pD_GNSS_Ublox->navPvt.fixType = getEnum(pData, 20u, 0u, 0x07u);
  pD_GNSS_Ublox->navPvt.gnssFixOk = getBool(pData, 21u, 0u);
  pD_GNSS_Ublox->navPvt.diffSoln = getBool(pData, 21u, 1u);
  pD_GNSS_Ublox->navPvt.carrierPhaseSolution = getEnum(pData, 21u, 6u, 0x03u);
  pD_GNSS_Ublox->navPvt.numSv = getUint8(pData, 23u);
  pD_GNSS_Ublox->navPvt.longitude = getInt32(pData, 24u);
  pD_GNSS_Ublox->navPvt.latitude = getInt32(pData, 28u);
  pD_GNSS_Ublox->navPvt.height = getInt32(pData, 32u);
  pD_GNSS_Ublox->navPvt.hMsl = getInt32(pData, 36u);
  pD_GNSS_Ublox->navPvt.hAcc = getUint32(pData, 40u);
  pD_GNSS_Ublox->navPvt.vAcc = getUint32(pData, 44u);
  pD_GNSS_Ublox->navPvt.velN = getInt32(pData, 48u);
  pD_GNSS_Ublox->navPvt.velE = getInt32(pData, 52u);
  pD_GNSS_Ublox->navPvt.velD = getInt32(pData, 56u);
  pD_GNSS_Ublox->navPvt.gSpeed = getInt32(pData, 60u);
  pD_GNSS_Ublox->navPvt.sAcc = getUint32(pData, 68u);
  pD_GNSS_Ublox->navPvt.pDop = getUint16(pData, 76u);
  pD_GNSS_Ublox->navPvt.invalidLlh = getBool(pData, 78u, 0u);
  pD_GNSS_Ublox->navPvt.year = getUint16(pData, 4u);
  pD_GNSS_Ublox->navPvt.month = getUint8(pData, 6u);
  pD_GNSS_Ublox->navPvt.day = getUint8(pData, 7u);
  pD_GNSS_Ublox->navPvt.hour = getUint8(pData, 8u);
  pD_GNSS_Ublox->navPvt.min = getUint8(pData, 9u);
  pD_GNSS_Ublox->navPvt.sec = getUint8(pData, 10u);
  pD_GNSS_Ublox->navPvt.validDate = getBool(pData, 11u, 0u);
  pD_GNSS_Ublox->navPvt.validTime = getBool(pData, 11u, 1u);
  pD_GNSS_Ublox->navPvt.fullyResolved = getBool(pData, 11u, 2u);
  pD_GNSS_Ublox->navPvt.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateNavPvt -->

  Invalidate NAV-PVT data.
*************************************************************************/
static void                       /** \return None */
invalidateNavPvt
(
void
)
{
  pD_GNSS_Ublox->navPvt.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processNavCov -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processNavCov
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  Uint32_t index;
  
  for (index = 0; index < GNSS_UBLOX_COVARIANCE_COUNT; index++)
  {
    pD_GNSS_Ublox->navCov.posCov[index] = getFloat32(pData, 16u + (index * 4u));
    pD_GNSS_Ublox->navCov.velCov[index] = getFloat32(pData, 40u + (index * 4u));
  }
  pD_GNSS_Ublox->navCov.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateNavCov -->

  Invalidate NAV-COV data.
*************************************************************************/
static void                       /** \return None */
invalidateNavCov
(
void
)
{
  pD_GNSS_Ublox->navCov.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processNavDop -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processNavDop
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->navDop.pDop = getUint16(pData, 6u);
  pD_GNSS_Ublox->navDop.vDop = getUint16(pData, 10u);
  pD_GNSS_Ublox->navDop.hDop = getUint16(pData, 12u);
  pD_GNSS_Ublox->navDop.nDop = getUint16(pData, 14u);
  pD_GNSS_Ublox->navDop.eDop = getUint16(pData, 16u);
  pD_GNSS_Ublox->navDop.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateNavDop -->

  Invalidate NAV-DOP data.
*************************************************************************/
static void                       /** \return None */
invalidateNavDop
(
void
)
{
  pD_GNSS_Ublox->navDop.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processNavStatus -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processNavStatus
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->navStatus.fixType = getEnum(pData, 4u, 0u, 0x07u);
  pD_GNSS_Ublox->navStatus.gpsFixOk = getBool(pData, 5u, 0u);
  pD_GNSS_Ublox->navStatus.diffSoln = getBool(pData, 5u, 1u);
  pD_GNSS_Ublox->navStatus.diffCorr = getBool(pData, 6u, 0u);
  pD_GNSS_Ublox->navStatus.spoofDetState = getEnum(pData, 7u, 3u, 0x03u);
  pD_GNSS_Ublox->navStatus.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateNavStatus -->

  Invalidate NAV-STATUS data.
*************************************************************************/
static void                       /** \return None */
invalidateNavStatus
(
void
)
{
  pD_GNSS_Ublox->navStatus.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processNavVelecef -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processNavVelecef
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->navVelecef.iTow = getUint32(pData, 0u);
  pD_GNSS_Ublox->navVelecef.ecefVX = getInt32(pData, 4u);
  pD_GNSS_Ublox->navVelecef.ecefVY = getInt32(pData, 8u);
  pD_GNSS_Ublox->navVelecef.ecefVZ = getInt32(pData, 12u);
  pD_GNSS_Ublox->navVelecef.sAcc = getUint32(pData, 16u);
  pD_GNSS_Ublox->navVelecef.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateNavVelecef -->

  Invalidate NAV-STATUS data.
*************************************************************************/
static void                       /** \return None */
invalidateNavVelecef
(
void
)
{
  pD_GNSS_Ublox->navVelecef.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processMonSys -->

  Process messages received from the GNSS.
*************************************************************************/
static void                       /** \return None */
processMonSys
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->monSys.bootType = getEnum(pData, 1u, 0u, 0x0Fu);
  pD_GNSS_Ublox->monSys.cpuLoad = getUint8(pData, 2u);
  pD_GNSS_Ublox->monSys.cpuLoadMax = getUint8(pData, 3u);
  pD_GNSS_Ublox->monSys.memUsage = getUint8(pData, 4u);
  pD_GNSS_Ublox->monSys.memUsageMax = getUint8(pData, 5u);
  pD_GNSS_Ublox->monSys.ioUsage = getUint8(pData, 6u);
  pD_GNSS_Ublox->monSys.ioUsageMax = getUint8(pData, 7u);
  pD_GNSS_Ublox->monSys.runTime = getUint32(pData, 8u);
  pD_GNSS_Ublox->monSys.noticeCount = getUint16(pData, 12u);
  pD_GNSS_Ublox->monSys.warnCount = getUint16(pData, 14u);
  pD_GNSS_Ublox->monSys.errorCount = getUint16(pData, 16u);
  pD_GNSS_Ublox->monSys.tempValue = getInt8(pData, 18u);
  pD_GNSS_Ublox->monSys.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateMonSys -->

  Invalidate MON-SYS data.
*************************************************************************/
static void                       /** \return None */
invalidateMonSys
(
void
)
{
  pD_GNSS_Ublox->monSys.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- processSecSig -->

  Process messages received from the GNSS.
*************************************************************************/
// gcov-jst 14 It is not possible to execute this code on the FC-200.
static void                       /** \return None */
processSecSig
(
const Uint8_t * const pData       /**< [in] Pointer to message */
)
{
  pD_GNSS_Ublox->secSig.jamEnabled = getEnum(pData, 4u, 0u, 0x01u);
  pD_GNSS_Ublox->secSig.jammingState = getEnum(pData, 4u, 1u, 0x03u);
  pD_GNSS_Ublox->secSig.spoofDetEnabled = getEnum(pData, 8u, 0u, 0x01u);
  pD_GNSS_Ublox->secSig.spoofDetState = getEnum(pData, 8u, 1u, 0x07u);
  pD_GNSS_Ublox->secSig.valid = d_TRUE;

  return;
}

/*********************************************************************//**
  <!-- invalidateSecSig -->

  Invalidate SEC-SIG data.
*************************************************************************/
// gcov-jst 10 It is not possible to execute this code on the FC-200.
static void                       /** \return None */
invalidateSecSig
(
void
)
{
  pD_GNSS_Ublox->secSig.valid = d_FALSE;

  return;
}

/*********************************************************************//**
  <!-- getBool -->

  Extract bool from received message.
*************************************************************************/
static Uint8_t                    /** \return Extracted variable */
getBool
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref,               /**< [in] Index of data to extract */
const Uint32_t bit                /**< [in] Bit number containing the bool */
)
{
  return (Uint8_t)(pData[ref] >> bit) & 0x01u;
}

/*********************************************************************//**
  <!-- getEnum -->

  Extract enum from received message.
*************************************************************************/
static Uint8_t                    /** \return Extracted variable */
getEnum
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref,               /**< [in] Index of data to extract */
const Uint32_t shift,             /**< [in] Number of bits to shift */
const Uint8_t mask)               /**< [in] Bit mask to apply */
{
  return (Uint8_t)(pData[ref] >> shift) & mask;
}

/*********************************************************************//**
  <!-- getInt8 -->

  Extract Int8 from received message.
*************************************************************************/
static Int8_t                    /** \return Extracted variable */
getInt8
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref                /**< [in] Index of data to extract */
)
{
  return (Int8_t)pData[ref];
}

/*********************************************************************//**
  <!-- getUint8 -->

  Extract Uint8 from received message.
*************************************************************************/
static Uint8_t                    /** \return Extracted variable */
getUint8
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref                /**< [in] Index of data to extract */
)
{
  return pData[ref];
}

/*********************************************************************//**
  <!-- getUint16 -->

  Extract Uint16 from received message.
*************************************************************************/
static Uint16_t                   /** \return Extracted variable */
getUint16
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref                /**< [in] Index of data to extract */
)
{
  // cppcheck-suppress misra-c2012-11.3; Pointer is 2 byte aligned assuring validity of cast. Violation of 'Advisory' rule does not present a risk.
  return *(const Uint16_t *)&pData[ref];
}

/*********************************************************************//**
  <!-- getUint32 -->

  Extract Uint32 from received message.
*************************************************************************/
static Uint32_t                   /** \return Extracted variable */
getUint32
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref                /**< [in] Index of data to extract */
)
{
  // cppcheck-suppress misra-c2012-11.3; Pointer is 4 byte aligned assuring validity of cast. Violation of 'Advisory' rule does not present a risk.
  return *(const Uint32_t *)&pData[ref];
}

/*********************************************************************//**
  <!-- getInt32 -->

  Extract Int32 from received message.
*************************************************************************/
static Int32_t                    /** \return Extracted variable */
getInt32
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref                /**< [in] Index of data to extract */
)
{
  // cppcheck-suppress misra-c2012-11.3; Pointer is 4 byte aligned assuring validity of cast. Violation of 'Advisory' rule does not present a risk.
  return *(const Int32_t *)&pData[ref];
}

/*********************************************************************//**
  <!-- getFloat32 -->

  Extract float from received message.
*************************************************************************/
static Float32_t                  /** \return Extracted variable */
getFloat32
(
const Uint8_t * const pData,      /**< [in] Pointer to message */
const Uint32_t ref                /**< [in] Index of data to extract */
)
{
  // cppcheck-suppress misra-c2012-11.3; Pointer is 4 byte aligned assuring validity of cast. Violation of 'Advisory' rule does not present a risk.
  // cppcheck-suppress invalidPointerCast; This is a standard method to extract floating point data from a byte array.
  return *(const Float32_t *)&pData[ref];
}
