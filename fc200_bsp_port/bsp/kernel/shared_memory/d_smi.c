/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Buffer

  Abstract           : Buffer driver module

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-145
                       SDD References: 136T-2200-131100-001-D22 SWDES-200
                                                                SWDES-201
                                                                SWDES-202
                                                                SWDES-203
                                                                SWDES-204
                                                                SWDES-205
                                                                SWDES-206
                                                                SWDES-207
                                                                SWDES-208
                                                                SWDES-209

\note
  CSC ID             : SWDES-80
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/timer/d_timer.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/error_handler/d_error_handler.h"
#include "soc/interrupt_manager/d_int_critical.h"

#include "d_smi.h"

/***** Constants ********************************************************/

/* Maximum time, in microseconds, to wait for semaphore access */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t TIMEOUT_TIME = 50;

#define MAX_ENTRIES      50u
#define TOTAL_SIZE      500u

/***** Type Definitions *************************************************/

typedef enum
{
  INTERFACE_TRANSMIT = 0,
  INTERFACE_RECEIVE
} Interface_t;

#pragma pack(1)
/* Buffer status */
typedef struct
{
  volatile Uint32_t bufferIndexIn;       /* Index in buffer for next write */
  volatile Uint32_t bufferUsedBytes;     /* Number of bytes of buffer used */
  volatile Uint32_t indexIn;             /* Index in entries for next write */
  volatile Uint32_t indexOut;            /* Index in entries for next read */
  volatile Uint32_t entryCount;          /* Number of entries in buffer */
  volatile Uint32_t highWater;           /* Largest number of entries in buffer */
} BufferVarStatus_t;

/* Item status */
typedef struct
{
  volatile Uint32_t itemIndex;           /* Index in buffer of the message */
  volatile Uint32_t itemSize;            /* Length of the message */
} BufferVarItem_t;

typedef struct
{
  volatile Uint32_t user[2]; /* We are not using Bool_t as that gives different size variables between RPU and APU */
  volatile Uint32_t turn;
} Semaphore_t;

typedef struct
{
  volatile BufferVarStatus_t varStatus;
  volatile BufferVarItem_t varItems[MAX_ENTRIES];
  volatile Semaphore_t semaphore;
  volatile Uint8_t buffer[TOTAL_SIZE];
} buffer_cfg_t;
#pragma pack()

/***** Variables ********************************************************/

static buffer_cfg_t bufferTx __attribute__ ((section (".sharedTx")));  \
static buffer_cfg_t bufferRx __attribute__ ((section (".sharedRx")));

/***** Function Declarations ********************************************/

static void messageCopyOut(Uint8_t * const pDestination, const volatile Uint8_t * const pSource, const Uint32_t bufferSize, const Uint32_t bufferIndex, const Uint32_t length);
static void messageCopyIn(volatile Uint8_t * const pDestination, const Uint32_t bufferSize, const Uint32_t bufferIndex, const Uint8_t * const pSource, const Uint32_t length);
static d_Status_t semaphoreLock(const Interface_t interface);
static void semaphoreFree(const Interface_t interface);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SMI_Initialise -->

  Initialise the variable message length buffers.
*************************************************************************/
void                      /** \return None */
d_SMI_Initialise
(
void
)
{
  /* Initialise buffers */
  d_SMI_ReceiveFlush();
  d_SMI_TransmitFlush();

  /* Initialise semaphores */
  bufferRx.semaphore.user[0] = d_FALSE;
  bufferRx.semaphore.user[1] = d_FALSE;
  bufferRx.semaphore.turn = 0;

  return;
}

/*********************************************************************//**
  <!-- d_SMI_ReceiveFlush -->

  Flush a variable message length buffer.
*************************************************************************/
void
d_SMI_ReceiveFlush
(
void
)
{
  bufferRx.varStatus.bufferIndexIn = 0;
  bufferRx.varStatus.bufferUsedBytes = 0;
  bufferRx.varStatus.indexIn = 0;
  bufferRx.varStatus.indexOut = 0;
  bufferRx.varStatus.entryCount = 0;
  bufferRx.varStatus.highWater = 0;

  return;
}

/*********************************************************************//**
  <!-- d_SMI_TransmitFlush -->

  Flush a variable message length buffer.
*************************************************************************/
void
d_SMI_TransmitFlush
(
void
)
{
  bufferTx.varStatus.bufferIndexIn = 0;
  bufferTx.varStatus.bufferUsedBytes = 0;
  bufferTx.varStatus.indexIn = 0;
  bufferTx.varStatus.indexOut = 0;
  bufferTx.varStatus.entryCount = 0;
  bufferTx.varStatus.highWater = 0;

  return;
}

/*********************************************************************//**
  <!-- d_SMI_Transmit -->

  Write a variable length message to a buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_SMI_Transmit
(
const Uint8_t * const pData,   /**< [in] Pointer to message data */
const Uint32_t length          /**< [in] Length of message to write */
)
{
  d_Status_t returnValue = d_STATUS_BUFFER_FULL;

  if (pData == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (length == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    if ((bufferTx.varStatus.entryCount < MAX_ENTRIES) &&
        (length <= (TOTAL_SIZE - bufferTx.varStatus.bufferUsedBytes)))
    {
      messageCopyIn(&bufferTx.buffer[0],
                    TOTAL_SIZE,
                    bufferTx.varStatus.bufferIndexIn,
                    &pData[0],
                    length);
      bufferTx.varItems[bufferTx.varStatus.indexIn].itemIndex = bufferTx.varStatus.bufferIndexIn;
      bufferTx.varItems[bufferTx.varStatus.indexIn].itemSize = length;
      bufferTx.varStatus.bufferIndexIn = (bufferTx.varStatus.bufferIndexIn + length) % TOTAL_SIZE;
      bufferTx.varStatus.indexIn = (bufferTx.varStatus.indexIn + 1u) % MAX_ENTRIES;
      returnValue = semaphoreLock(INTERFACE_TRANSMIT);
      if (returnValue == d_STATUS_SUCCESS)
      {
        bufferTx.varStatus.bufferUsedBytes = bufferTx.varStatus.bufferUsedBytes + length;
        bufferTx.varStatus.entryCount++;
        semaphoreFree(INTERFACE_TRANSMIT);
        if (bufferTx.varStatus.entryCount > bufferTx.varStatus.highWater)
        {
          bufferTx.varStatus.highWater = bufferTx.varStatus.entryCount;
        }
        else
        {
          DO_NOTHING();
        }
        /* Send interrupt to other processor */
        d_INT_Ipi();
      }
      else
      {
        /* Semaphore lock failed */
        semaphoreFree(INTERFACE_TRANSMIT);
      }
    }
    else
    {
      DO_NOTHING();
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SMI_Receive -->

  Read a variable length message from a buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_SMI_Receive
(
Uint8_t * const pData,         /**< [out] Pointer to storage for message data */
Uint32_t * const pLength       /**< [out] Pointer to storage for message length */
)
{
  d_Status_t returnValue = d_STATUS_BUFFER_EMPTY;

  if (pData == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  if ((returnValue == d_STATUS_BUFFER_EMPTY) && (pLength == NULL))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  if (returnValue == d_STATUS_BUFFER_EMPTY)
  {
    Uint32_t entrySize = 0u;
    if (bufferRx.varStatus.entryCount > 0u)
    {
      entrySize = bufferRx.varItems[bufferRx.varStatus.indexOut].itemSize;
      messageCopyOut(&pData[0],
                     &bufferRx.buffer[0],
                     TOTAL_SIZE,
                     bufferRx.varItems[bufferRx.varStatus.indexOut].itemIndex,
                     entrySize);
      bufferRx.varStatus.indexOut = (bufferRx.varStatus.indexOut + 1u) % MAX_ENTRIES;
      returnValue = semaphoreLock(INTERFACE_RECEIVE);
      if (returnValue == d_STATUS_SUCCESS)
      {
        bufferRx.varStatus.bufferUsedBytes = bufferRx.varStatus.bufferUsedBytes - entrySize;
        bufferRx.varStatus.entryCount--;
      }
      semaphoreFree(INTERFACE_RECEIVE);
    }
    else
    {
      DO_NOTHING();
    }
    *pLength = entrySize;
  }
  else
  {
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SMI_ReceiveCount -->

  Read the number of messages in a variable length buffer.
*************************************************************************/
d_Status_t
d_SMI_ReceiveCount
(
Uint32_t * const pCount
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (pCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    returnValue = semaphoreLock(INTERFACE_RECEIVE);
    if (returnValue == d_STATUS_SUCCESS)
    {
      *pCount = bufferRx.varStatus.entryCount;
    }
    else
    {
      *pCount = 0;
    }
    semaphoreFree(INTERFACE_RECEIVE);
  }
  
  return returnValue;
}
  
/*********************************************************************//**
  <!-- messageCopyOut -->

  Copy a message out of the buffer to the application provided memory location.
*************************************************************************/
static void              /** \return None */
messageCopyOut
(
Uint8_t * const pDestination,    /**< [out] Destination pointer */
const volatile Uint8_t * const pSource,   /**< [in]  Source pointer */
const Uint32_t bufferSize,       /**< [in]  Source buffer size */
const Uint32_t bufferStart,      /**< [in]  Source buffer start index */
const Uint32_t length            /**< [in]  Length of message to copy */
)
{
  Uint32_t index;

  Uint32_t bufferIndex = bufferStart;
  for (index = 0u; index < length; index++)
  {
    pDestination[index] = pSource[bufferIndex];
    bufferIndex = (bufferIndex + 1u) % bufferSize;
  }

  return;
}

/*********************************************************************//**
  <!-- messageCopyIn -->

  Copy a message into the buffer from the application provided memory location.
*************************************************************************/
static void                      /** \return None */
messageCopyIn
(
volatile Uint8_t * const pDestination,    /**< [out] Destination pointer */
const Uint32_t bufferSize,       /**< [in] Destination buffer size */
const Uint32_t bufferStart,      /**< [in] Destination buffer start index */
const Uint8_t * const pSource,   /**< [in] Source pointer */
const Uint32_t length            /**< [in] Length of message to copy */
)
{
  Uint32_t index;

  Uint32_t bufferIndex = bufferStart;
  for (index = 0u; index < length; index++)
  {
    pDestination[bufferIndex] = pSource[index];
    bufferIndex = (bufferIndex + 1u) % bufferSize;
  }

  return;
}

/*********************************************************************//**
  <!-- semaphoreLock -->

  Lock semaphore for exclusive access.
*************************************************************************/
static d_Status_t semaphoreLock(const Interface_t interface)
{
  volatile Semaphore_t * pSemaphore;
  d_Status_t returnValue = d_STATUS_SUCCESS;
  
  if (interface == INTERFACE_TRANSMIT)
  {
    pSemaphore = (volatile Semaphore_t *)&bufferTx.semaphore;
  }
  else
  {
    pSemaphore = (volatile Semaphore_t *)&bufferRx.semaphore;
  }
  Uint32_t otherInterface = ((Uint32_t)interface + 1u) & 0x01u;

  pSemaphore->user[interface] = d_TRUE;
  pSemaphore->turn = otherInterface;

  Bool_t timeout = d_FALSE;
  Uint32_t startTime;
  (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
  
  // cppcheck-suppress knownConditionTrueFalse; The structure pointed to pSemaphore is defined as 'volatile' and may be changed by another processor.
  while ((pSemaphore->turn == otherInterface) && (pSemaphore->user[otherInterface] == (Uint32_t)d_TRUE) && (timeout == d_FALSE))
  {
    Uint32_t elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    if (elapsedTime > TIMEOUT_TIME)
    {
      timeout = d_TRUE;
      returnValue = d_STATUS_TIMEOUT;
    }
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- semaphoreFree -->

  Free the semaphore.
*************************************************************************/
static void semaphoreFree(const Interface_t interface)
{
  volatile Semaphore_t * pSemaphore;

  if (interface == INTERFACE_TRANSMIT)
  {
    pSemaphore = &bufferTx.semaphore;
  }
  else
  {
    pSemaphore = &bufferRx.semaphore;
  }

  pSemaphore->user[interface] = d_FALSE;
  
  return;
}
