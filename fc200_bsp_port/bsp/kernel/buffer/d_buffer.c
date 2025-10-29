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
#include "kernel/general/d_gen_memory.h"
#include "kernel/error_handler/d_error_handler.h"
#include "soc/interrupt_manager/d_int_critical.h"

#include "d_buffer_cfg.h"
#include "d_buffer.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static void messageCopyOut(Uint8_t * const pDestination, const Uint8_t * const pSource, const Uint32_t bufferSize, const Uint32_t bufferIndex, const Uint32_t length);
static void messageCopyIn(Uint8_t * const pDestination, const Uint32_t bufferSize, const Uint32_t bufferIndex, const Uint8_t * const pSource, const Uint32_t length);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_BUFFER_FixedInitialise -->

  Initialise the fixed message length buffers.
*************************************************************************/
void                      /** \return None */
d_BUFFER_FixedInitialise
(
void
)
{
  Uint32_t index;

  /* Initialise buffers */
  for (index = 0; index < bufferFixedCount; index++)
  {
    (void)d_BUFFER_FixedFlush(index);
  }

  return;
}

/*********************************************************************//**
  <!-- d_BUFFER_FixedFlush -->

  Flush a fixed message length buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_BUFFER_FixedFlush
(
const Uint32_t id              /**< [in]  Buffer ID */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (id >= bufferFixedCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    /* Initialise buffer */
    d_BUFFER_FixedDef[id].pBufferFixedStatus->indexIn = 0;
    d_BUFFER_FixedDef[id].pBufferFixedStatus->indexOut = 0;
    d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount = 0;
    d_BUFFER_FixedDef[id].pBufferFixedStatus->highWater = 0;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_BUFFER_FixedWrite -->

  Write a number of items to a fixed item length buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_BUFFER_FixedWrite
(
const Uint32_t id,             /**< [in]  Buffer ID */
const Uint8_t * const pData,   /**< [in]  Pointer to message data */
const Uint32_t count,          /**< [in]  Number of items to write */
Uint32_t * const pActual       /**< [out] Pointer to storage for actual number of items written */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t written = 0;
  Uint32_t readIndex;

  if (id >= bufferFixedCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pData == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if ((count == 0u) || (count > d_BUFFER_FixedDef[id].entryMax))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, count, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pActual == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    readIndex = 0;
    while ((written < count) && (d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount < d_BUFFER_FixedDef[id].entryMax))
    {
// cppcheck-suppress misra-c2012-11.8; Erroneous reporting of removal of pointer data qualification. Implementation does not violate the rule.
      d_GEN_MemoryCopy(&d_BUFFER_FixedDef[id].pBuffer[d_BUFFER_FixedDef[id].pBufferFixedStatus->indexIn * d_BUFFER_FixedDef[id].entrySize],
                       (const Uint8_t * const)&pData[readIndex * d_BUFFER_FixedDef[id].entrySize],
                       d_BUFFER_FixedDef[id].entrySize);
      d_BUFFER_FixedDef[id].pBufferFixedStatus->indexIn = (d_BUFFER_FixedDef[id].pBufferFixedStatus->indexIn + 1u) % d_BUFFER_FixedDef[id].entryMax;
      Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
      d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount++;
      if (d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount > d_BUFFER_FixedDef[id].pBufferFixedStatus->highWater)
      {
        d_BUFFER_FixedDef[id].pBufferFixedStatus->highWater = d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount;
      }
      else
      {
        DO_NOTHING();
      }
      d_INT_CriticalSectionLeave(interruptFlags);
      readIndex++;
      written++;
    }
    *pActual = written;
    if (written < count)
    {
      returnValue = d_STATUS_BUFFER_FULL;
    }
    else
    {
      DO_NOTHING();
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_BUFFER_FixedRead -->

  Read a number of items from a fixed item length buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_BUFFER_FixedRead
(
const Uint32_t id,             /**< [in]  Buffer ID */
Uint8_t * const pData,         /**< [out] Pointer to storage for message data */
const Uint32_t count,          /**< [in]  Number of items to read */
Uint32_t * const pActual       /**< [out] Pointer to storage for actual number of items read */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t read = 0u;
  Uint32_t writeIndex;

  if (id >= bufferFixedCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pData == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if ((count == 0u) || (count > d_BUFFER_FixedDef[id].entryMax))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, count, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pActual == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    writeIndex = 0;
    while ((read < count) && (d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount > 0u))
    {
      d_GEN_MemoryCopy(&pData[writeIndex * d_BUFFER_FixedDef[id].entrySize],
                       &d_BUFFER_FixedDef[id].pBuffer[d_BUFFER_FixedDef[id].pBufferFixedStatus->indexOut * d_BUFFER_FixedDef[id].entrySize],
                       d_BUFFER_FixedDef[id].entrySize);
      d_BUFFER_FixedDef[id].pBufferFixedStatus->indexOut = (d_BUFFER_FixedDef[id].pBufferFixedStatus->indexOut + 1u) % d_BUFFER_FixedDef[id].entryMax;
      Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
      d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount--;
      d_INT_CriticalSectionLeave(interruptFlags);
      writeIndex++;
      read++;
    }
    *pActual = read;
    if (read < count)
    {
      returnValue = d_STATUS_BUFFER_EMPTY;
    }
    else
    {
      DO_NOTHING();
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_BUFFER_FixedCount -->

  Read the number of items in a fixed item length buffer.
*************************************************************************/
d_Status_t
d_BUFFER_FixedCount
(
const Uint32_t id,
Uint32_t * const pCount
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (id >= bufferFixedCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    *pCount = d_BUFFER_FixedDef[id].pBufferFixedStatus->entryCount;  
  }
  
  return returnValue;
}
  
/*********************************************************************//**
  <!-- d_BUFFER_VarInitialise -->

  Initialise the variable message length buffers.
*************************************************************************/
void                      /** \return None */
d_BUFFER_VarInitialise
(
void
)
{
  Uint32_t index;

  /* Initialise buffers */
  for (index = 0; index < bufferVarCount; index++)
  {
    (void)d_BUFFER_VarFlush(index);
  }

  return;
}

/*********************************************************************//**
  <!-- d_BUFFER_VarFlush -->

  Flush a variable message length buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_BUFFER_VarFlush
(
const Uint32_t id              /**< [in] Buffer ID */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (id >= bufferVarCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    d_BUFFER_VarDef[id].pBufferVarStatus->bufferIndexIn = 0;
    d_BUFFER_VarDef[id].pBufferVarStatus->bufferUsedBytes = 0;
    d_BUFFER_VarDef[id].pBufferVarStatus->indexIn = 0;
    d_BUFFER_VarDef[id].pBufferVarStatus->indexOut = 0;
    d_BUFFER_VarDef[id].pBufferVarStatus->entryCount = 0;
    d_BUFFER_VarDef[id].pBufferVarStatus->highWater = 0;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_BUFFER_VarWrite -->

  Write a variable length message to a buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_BUFFER_VarWrite
(
const Uint32_t id,             /**< [in] Buffer ID */
const Uint8_t * const pData,   /**< [in] Pointer to message data */
const Uint32_t length          /**< [in] Length of message to write */
)
{
  d_Status_t returnValue = d_STATUS_BUFFER_FULL;

  if (id >= bufferVarCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pData == NULL)
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
    if ((d_BUFFER_VarDef[id].pBufferVarStatus->entryCount < d_BUFFER_VarDef[id].entryMax) &&
        (length <= (d_BUFFER_VarDef[id].totalSize - d_BUFFER_VarDef[id].pBufferVarStatus->bufferUsedBytes)))
    {
// cppcheck-suppress misra-c2012-11.8; Erroneous reporting of removal of pointer data qualification. Implementation does not violate the rule.
      messageCopyIn(&d_BUFFER_VarDef[id].pBuffer[0],
                    d_BUFFER_VarDef[id].totalSize,
                    d_BUFFER_VarDef[id].pBufferVarStatus->bufferIndexIn,
                    &pData[0],
                    length);
      d_BUFFER_VarDef[id].pBufferVarItem[d_BUFFER_VarDef[id].pBufferVarStatus->indexIn].itemIndex = d_BUFFER_VarDef[id].pBufferVarStatus->bufferIndexIn;
      d_BUFFER_VarDef[id].pBufferVarItem[d_BUFFER_VarDef[id].pBufferVarStatus->indexIn].itemSize = length;
      d_BUFFER_VarDef[id].pBufferVarStatus->bufferIndexIn = (d_BUFFER_VarDef[id].pBufferVarStatus->bufferIndexIn + length) % d_BUFFER_VarDef[id].totalSize;
      d_BUFFER_VarDef[id].pBufferVarStatus->indexIn = (d_BUFFER_VarDef[id].pBufferVarStatus->indexIn + 1u) % d_BUFFER_VarDef[id].entryMax;
      Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
      d_BUFFER_VarDef[id].pBufferVarStatus->bufferUsedBytes = d_BUFFER_VarDef[id].pBufferVarStatus->bufferUsedBytes + length;
      d_BUFFER_VarDef[id].pBufferVarStatus->entryCount++;
      if (d_BUFFER_VarDef[id].pBufferVarStatus->entryCount > d_BUFFER_VarDef[id].pBufferVarStatus->highWater)
      {
        d_BUFFER_VarDef[id].pBufferVarStatus->highWater = d_BUFFER_VarDef[id].pBufferVarStatus->entryCount;
      }
      else
      {
        DO_NOTHING();
      }
      d_INT_CriticalSectionLeave(interruptFlags);
      returnValue = d_STATUS_SUCCESS;
    }
    else
    {
      DO_NOTHING();
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_BUFFER_VarRead -->

  Read a variable length message from a buffer.
*************************************************************************/
d_Status_t                     /** \return Function status */
d_BUFFER_VarRead
(
const Uint32_t id,             /**< [in]  Buffer ID */
Uint8_t * const pData,         /**< [out] Pointer to storage for message data */
Uint32_t * const pLength       /**< [out] Pointer to storage for message length */
)
{
  d_Status_t returnValue = d_STATUS_BUFFER_EMPTY;

  if (id >= bufferVarCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  if ((returnValue == d_STATUS_BUFFER_EMPTY) && (pData == NULL))
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
    if (d_BUFFER_VarDef[id].pBufferVarStatus->entryCount > 0u)
    {
      entrySize = d_BUFFER_VarDef[id].pBufferVarItem[d_BUFFER_VarDef[id].pBufferVarStatus->indexOut].itemSize;
      messageCopyOut(&pData[0],
                     &d_BUFFER_VarDef[id].pBuffer[0],
                     d_BUFFER_VarDef[id].totalSize,
                     d_BUFFER_VarDef[id].pBufferVarItem[d_BUFFER_VarDef[id].pBufferVarStatus->indexOut].itemIndex,
                     entrySize);
      d_BUFFER_VarDef[id].pBufferVarStatus->indexOut = (d_BUFFER_VarDef[id].pBufferVarStatus->indexOut + 1u) % d_BUFFER_VarDef[id].entryMax;
      Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
      d_BUFFER_VarDef[id].pBufferVarStatus->bufferUsedBytes = d_BUFFER_VarDef[id].pBufferVarStatus->bufferUsedBytes - entrySize;
      d_BUFFER_VarDef[id].pBufferVarStatus->entryCount--;
      d_INT_CriticalSectionLeave(interruptFlags);
      returnValue = d_STATUS_SUCCESS;
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
  <!-- d_BUFFER_VarCount -->

  Read the number of messages in a variable length buffer.
*************************************************************************/
d_Status_t
d_BUFFER_VarCount
(
const Uint32_t id,
Uint32_t * const pCount
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (id >= bufferVarCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, id, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else if (pCount == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    *pCount = d_BUFFER_VarDef[id].pBufferVarStatus->entryCount;  
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
const Uint8_t * const pSource,   /**< [in]  Source pointer */
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
Uint8_t * const pDestination,    /**< [out] Destination pointer */
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
