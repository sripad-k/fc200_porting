/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_file

  Abstract           : File handler interface to PC

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/timer/d_timer.h"
#include "soc/uart/d_uart.h"
#include "soc/interrupt_manager/d_int_critical.h"
#include "kernel/crc32/d_crc32.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/general/d_gen_string.h"

#include "d_file.h"

/***** Constants ********************************************************/

/* Mesage ID, command, data count, data, crc */
static Uint8_t RESPONSE_LENGTH = 4u + 2u + 2u + 4u + 4u;

/***** Type Definitions *************************************************/

typedef enum
{
  COMMAND_FOPEN = 0,
  COMMAND_FCLOSE,
  COMMAND_FWRITE
} FileCommand_t;

/***** Variables ********************************************************/

static Uint32_t channel;

/***** Function Declarations ********************************************/

static void SendMessage(const FileCommand_t command, const Uint8_t * const pData, const Uint16_t length);
static d_Status_t GetResponse(Uint8_t * const pData, const Uint8_t expectedLength);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_FILE_Initialise -->

  Initialise file handling.
*************************************************************************/
Uint32_t                         /** \return Returns the CRC */
d_FILE_Initialise
(
const Uint32_t serialChannel     /**< [in] Serial channel for interface to PC */
)
{
  d_Status_t status;
  
  channel = serialChannel;
  
  d_TIMER_Initialise();

  status = d_UART_Configure(channel, 921600, d_UART_DATA_BITS_8, d_UART_PARITY_NONE, d_UART_STOP_BITS_1);

  return status;
}

/*********************************************************************//**
  <!-- d_FILE_fopen -->

  Adds block of memory to existing CRC32.
*************************************************************************/
Int32_t                         /** \return File handle */
d_FILE_fopen
(
const Char_t * const filename,   /**< [in] Name of file to open */
const Char_t * const mode        /**< [in] Mode, [r][w][a] */
)
{
  Int32_t returnValue;
  Uint8_t buffer[80];
  Int32_t filenameLength = d_GEN_StringLength(filename, 80u);
  Int32_t modeLength = d_GEN_StringLength(mode, 8u);

  (void)d_GEN_StringCopy((Char_t *)&buffer[0], 80u, filename, filenameLength);
  (void)d_GEN_StringCopy((Char_t *)&buffer[filenameLength], 80u - (Uint32_t)filenameLength, "|", 1u);
  (void)d_GEN_StringCopy((Char_t *)&buffer[(Uint32_t)filenameLength + 1u], 79u - (Uint32_t)filenameLength, mode, 8u);

  SendMessage(COMMAND_FOPEN, &buffer[0], (Uint32_t)filenameLength + 1u + (Uint32_t)modeLength);
  
  d_Status_t status = GetResponse(&buffer[0], RESPONSE_LENGTH);
  if (status == d_STATUS_SUCCESS)
  {
// cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
    returnValue = *(Int32_t *)&buffer[8];
  }
  else
  {
    returnValue = -1;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FILE_fclose -->

  Adds block of memory to existing CRC32.
*************************************************************************/
Int32_t                          /** \return File handle */
d_FILE_fclose
(
const Uint32_t fileId
)
{
  Int32_t returnValue = 0;
  Uint8_t buffer[16];

  SendMessage(COMMAND_FCLOSE, (const Uint8_t *)&fileId, sizeof(fileId));
  
  d_Status_t status = GetResponse(&buffer[0], RESPONSE_LENGTH);
  if (status != d_STATUS_SUCCESS)
  {
    returnValue = -1;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_FILE_fwrite -->

  Adds block of memory to existing CRC32.
*************************************************************************/
Int32_t                          /** \return File handle */
d_FILE_fwrite
(
const Uint32_t fileId,   /**< [in] Name of file to open */
const Uint8_t * const pData,
const Uint32_t length
)
{
  Int32_t returnValue = 0;
  Uint8_t buffer[1500];

// cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  *(Uint32_t *)&buffer[0] = fileId;
  d_GEN_MemoryCopy(&buffer[4], pData, length);
  SendMessage(COMMAND_FWRITE, buffer, length + 4u);
  
  d_Status_t status = GetResponse(&buffer[0], RESPONSE_LENGTH);
  if (status != d_STATUS_SUCCESS)
  {
    returnValue = -1;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- SendMessage -->

  Adds block of memory to existing CRC32.
*************************************************************************/
static void
SendMessage(const FileCommand_t command, const Uint8_t * const pData, const Uint16_t length)
{
  Uint8_t buffer[1500];

  d_GEN_MemoryCopy(&buffer[0], (const Uint8_t *)"FILE", 4);
// cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  *(Uint16_t *)&buffer[4] = command;
// cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  *(Uint16_t *)&buffer[6] = length;
  d_GEN_MemoryCopy(&buffer[8], pData, length);
// cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers. Violation of 'Advisory' rule does not present a risk.
  *(Uint32_t *)&buffer[8u + length] = d_CRC32_Calculate(&buffer[0], 8u + length);

  (void)d_UART_Transmit(channel, &buffer[0], 12u + length);
  
  return;
}

/*********************************************************************//**
  <!-- GetResponse -->

  Adds block of memory to existing CRC32.
*************************************************************************/
static d_Status_t
GetResponse(Uint8_t * const pData, const Uint8_t expectedLength)
{
  Bool_t done = d_FALSE;
  Uint32_t start;
  Uint32_t bytesRead;
  d_Status_t status = d_STATUS_FAILURE;
  Uint32_t elapsed;

  (void)d_TIMER_ElapsedMilliseconds(0, &start);
  while (done == d_FALSE)
  {
    status = d_UART_Receive(channel, pData, expectedLength, &bytesRead);
    if (bytesRead >= expectedLength)
    {
      done = d_TRUE;
    }
    else
    {
      elapsed = d_TIMER_ElapsedMilliseconds(start, NULL);
      if (elapsed > 2000u)
      {
        status = d_STATUS_TIMEOUT;
        done = d_TRUE;
      }
    }
  }
  (void)d_UART_FlushRx(channel);

  return status;
}
