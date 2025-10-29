/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Error Handling

  Abstract           : Error handling driver

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-183
                                                                SWREQ-184
                                                                SWREQ-185
                       SDD References: 136T-2200-131000-001-D22 SWDES-238
                                                                SWDES-239
                                                                SWDES-240
                                                                SWDES-241
                                                                SWDES-242
                                                                SWDES-243
                                                                SWDES-244
                                                                SWDES-245
                                                                SWDES-246
                                                                SWDES-247
                                                                SWDES-248
                                                                SWDES-249
\note
  CSC ID             : SWDES-74
*************************************************************************/

#define JENKINS_TEST d_TRUE

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/error_handler/d_error_handler_cfg.h"
#include "kernel/event_logger/d_event_logger.h"
#include "kernel/general/d_gen_string.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "soc/uart/d_uart.h"
#include "sru/watchdog/d_watchdog.h"
#include "sru/fcu/d_fcu.h"

/***** Constants ********************************************************/

/* Length of string accepted by the event logger for event description */
#define LOG_STRING_LENGTH   128u

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

static Uint32_t generateString(const d_ERROR_LogData_t * const errorData,
                               Char_t * const buffer,
                               const Uint32_t bufferLength);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_ERROR_Initialise -->

  Initialise the Error Handling CSC.
*************************************************************************/
d_Status_t                     /** \return Succes or failure */
d_ERROR_Initialise
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  
  if (initialised != d_TRUE)
  {
    status = d_EVENT_Initialise();

    if (status == d_STATUS_SUCCESS)
    {
      initialised = d_TRUE;
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }
  else
  {
    DO_NOTHING();
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_ERROR_Log -->

  Log an error.
*************************************************************************/
void                                  /** \return None */
d_ERROR_Log
(
d_ERROR_LogData_t * const errorData   /**< [in] Error data structure */
)
{
  d_ERROR_Criticality_t criticality;
  d_EVENT_EventCode_t eventCode;
  
  if (initialised != d_TRUE)
  {
    /* Cannot log error */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  
  if (errorData == NULL)
  {
    /* Force a shudown */
    // gcov-jst 7 It is not practical to generate this error during bench testing.
    const Char_t * const tbuffer = "Invalid parameter to d_ERROR_Log function\r\n";
    if (d_ERROR_UartChannel != UART_LOGGING_NOT_REQUIRED)
    {
      (void)d_UART_Transmit(d_ERROR_UartChannel, (const Uint8_t *)tbuffer, 43u);
    }
    ELSE_DO_NOTHING
    criticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  }
  else
  {
    /* Get criticality from application */
    criticality = d_ERROR_Criticality(errorData);

    /* Define buffer for the error message, plus 1 to allow for zero termination */
    Char_t buffer[LOG_STRING_LENGTH + 1u];
  
    /* Generate error string */
    Uint32_t stringLength = generateString(errorData, buffer, LOG_STRING_LENGTH + 1u);

    /* Log the error */
    if (criticality == d_ERROR_CRITICALITY_NON_CRITICAL)
    {
      eventCode = d_EVENT_NON_CRITICAL;
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this condition during coverage bench testing as all errors are considered non-critical.
      eventCode = d_EVENT_CRITICAL;
    }
    (void)d_EVENT_Log_Event(eventCode, buffer, stringLength, errorData->Edata[0]);

    /* Send diagnostic serial message */
    if (d_ERROR_UartChannel != UART_LOGGING_NOT_REQUIRED)
    {
      (void)d_UART_Transmit(d_ERROR_UartChannel, (Uint8_t *)buffer, stringLength);
      (void)d_UART_Transmit(d_ERROR_UartChannel, (const Uint8_t *)"\r\n", 2);
    }
    // gcov-jst 1 It is not practical to generate this condition during bench testing.
    ELSE_DO_NOTHING
  }

  /* Process criticality */
  // clang-tidy-jst misc-redundant-expression 1 Comparison will always be true during CI testing.
  if ((criticality != d_ERROR_CRITICALITY_NON_CRITICAL) && (JENKINS_TEST != d_TRUE))
  {
    /* Ensure all characters are output after interrupts disabled */
    if (d_ERROR_UartChannel != UART_LOGGING_NOT_REQUIRED)
    {
      (void)d_UART_FlushTx(d_ERROR_UartChannel);
    }
    ELSE_DO_NOTHING

    d_INT_Disable();

    Uint32_t slot = d_FCU_SlotNumber();
    (void)d_FCU_NoGo(slot);

    while (1)
    {
      if (criticality == d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN)
      {
        /* Strobe watchdog so board does not reset */
        (void)d_WDOG_Service();
      }
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      ELSE_DO_NOTHING
    }
  }
  ELSE_DO_NOTHING
  
  return;
}

/*********************************************************************//**
  <!-- d_ERROR_LogRaw -->

  Log error, specifying the raw data.
*************************************************************************/
void                                        /** \return None */
d_ERROR_LogRaw
(
const Char_t * const eModule,               /**< [in] Error module path string */
const Uint32_t eLine,                       /**< [in] Error line number */
const d_Status_t eType,                     /**< [in] Error type */
const d_ERROR_Criticality_t eCriticality,   /**< [in] Error criticality */
const Uint32_t eData0,                      /**< [in] Error additional data 0 */
const Uint32_t eData1,                      /**< [in] Error additional data 1 */
const Uint32_t eData2,                      /**< [in] Error additional data 2 */
const Uint32_t eData3                       /**< [in] Error additional data 3 */
)
{
  d_ERROR_LogData_t errorData;

  if (initialised != d_TRUE)
  {
    /* Cannot log error */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  
  if (eModule != NULL)
  {
    /* Check if string too long */
    Uint32_t moduleLength = d_GEN_StringLength(eModule, 80);
    if (moduleLength > (d_ERROR_PATH_SIZE - 1u))
    {
      Uint32_t index = moduleLength - d_ERROR_PATH_SIZE + 4u;
      while ((index < moduleLength) && (eModule[index] != '/'))
      {
        index++;
      }
      (void)d_GEN_StringCopy(errorData.Emodule, sizeof(errorData.Emodule), "...", 3u);
      (void)d_GEN_StringCopy(&errorData.Emodule[3u], sizeof(errorData.Emodule) - 3u, &eModule[index], d_ERROR_PATH_SIZE - 4u);
    }
    else
    {
      (void)d_GEN_StringCopy(errorData.Emodule, sizeof(errorData.Emodule), eModule, d_ERROR_PATH_SIZE - 1u);
    }
  }
  else
  {
    errorData.Emodule[0] = 0;
  }
  errorData.Eline = eLine;
  errorData.Etype = eType;
  errorData.Ecriticality = eCriticality;
  errorData.Edata[0] = eData0;
  errorData.Edata[1] = eData1;
  errorData.Edata[2] = eData2;
  errorData.Edata[3] = eData3;

  d_ERROR_Log(&errorData);
  
  return;
}

/*********************************************************************//**
  <!-- d_ERROR_Criticality -->

  'weak' function to determine criticality if function not provided
  by the application.
*************************************************************************/
// gcov-jst 20 This is a weak function overridden by a function in the test application code.
__attribute__((weak))
d_ERROR_Criticality_t           /** \return Error criticality */
d_ERROR_Criticality
(
d_ERROR_LogData_t * errorData   /**< [in] Error data structure */
)
{
  UNUSED_PARAMETER(errorData);
  d_ERROR_Criticality_t criticality = errorData->Ecriticality;

  if (criticality == d_ERROR_CRITICALITY_UNKNOWN)
  {
    criticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  }
  else
  {
    DO_NOTHING();
  }
  
  return criticality;
}

/*********************************************************************//**
  <!-- generateString -->

  Generate a string describing the error.
*************************************************************************/
static Uint32_t                                 /** \return Length of the generated string */
generateString
(
const d_ERROR_LogData_t * const errorData,      /**< [in]  Error data structure */
Char_t * const buffer,                          /**< [out] Buffer for the generated string */
const Uint32_t bufferLength                     /**< [in]  Length of the buffer */
)
{
  Char_t localBuffer[16];
  Uint32_t index;

  (void)d_GEN_StringCopy(buffer, bufferLength, "ERROR: ", 7);
  if (errorData->Emodule[0] != (Char_t)0)
  {
    (void)d_GEN_StringConcatenate(buffer, bufferLength, "Module ", 7u);
    (void)d_GEN_StringConcatenate(buffer, bufferLength, errorData->Emodule, sizeof(errorData->Emodule));
    (void)d_GEN_StringConcatenate(buffer, bufferLength, ", Line ", 7u);
    (void)d_GEN_ConvertIntegerToString((Int32_t)errorData->Eline, localBuffer, sizeof(localBuffer), 0u, 10u);
    (void)d_GEN_StringConcatenate(buffer, bufferLength, localBuffer, sizeof(localBuffer));
    (void)d_GEN_StringConcatenate(buffer, bufferLength, ", ", 2u);
  }
  else
  {
    DO_NOTHING();
  }

  Uint32_t messageLength = 0u;
  const Char_t * message = d_StatusMessage(errorData->Etype, &messageLength);
  if (message != NULL)
  {
    (void)d_GEN_StringConcatenate(buffer, bufferLength, message, messageLength);
  }
  else
  {
    (void)d_GEN_ConvertIntegerToString(errorData->Etype, localBuffer, sizeof(localBuffer), 0u, 10u);
    (void)d_GEN_StringConcatenate(buffer, bufferLength, localBuffer, sizeof(localBuffer));
  }

  for (index = 0; index < d_ERROR_DATA_ITEM_COUNT; index++)
  {
    /* Check if space for value */
    if ((LOG_STRING_LENGTH - (Uint32_t)d_GEN_StringLength(buffer, bufferLength)) >= 12u)
    {
      (void)d_GEN_StringConcatenate(buffer, bufferLength, ", 0x", 4u);
      (void)d_GEN_ConvertIntegerToString((Int32_t)errorData->Edata[index], localBuffer, sizeof(localBuffer), 8u, 16u);
      (void)d_GEN_StringConcatenate(buffer, bufferLength, localBuffer, sizeof(localBuffer));
    }
    else
    {
      DO_NOTHING();
    }
  }

  Uint32_t stringLength = d_GEN_StringLength(buffer, bufferLength);

  return stringLength;
}

