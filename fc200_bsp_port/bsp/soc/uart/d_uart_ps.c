/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART_PS

  Abstract           : SOC UART PS interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-233
                       SDD References: 136T-2200-131000-001-D22 SWDES-98
                                                                SWDES-99
                                                                SWDES-100
                                                                SWDES-101
                                                                SWDES-102
                                                                SWDES-103
                                                                SWDES-104
                                                                SWDES-105
                                                                SWDES-106
                                                                SWDES-107
                                                                SWDES-108
                                                                SWDES-109
                                                                SWDES-110
\note
  CSC ID             : SWDES-44
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_register.h"
#include "soc/interrupt_manager/d_int_critical.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/error_handler/d_error_handler.h"

#include "xparameters.h"
#include "xuartps.h"

#include "d_uart_ps.h"
#include "d_uart_ps_cfg.h"

/***** Constants ********************************************************/

#define TRANSMIT_BUFFER_LENGTH 2048u
#define RECEIVE_BUFFER_LENGTH  2048u

/* The following constant defines the amount of error that is allowed for
 * a specified baud rate. This error is the difference between the actual
 * baud rate that will be generated using the specified clock and the
 * desired baud rate.
 */
#define XUARTPS_MAX_BAUD_ERROR_RATE     3U  /* max % error allowed */

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t dataBitsTable[d_UART_DATA_BITS_COUNT] =
{
  0x04u,  /* 7 bits */
  0x00u   /* 8 bits */
};

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t parityTable[d_UART_PARITY_COUNT] =
{
  0x20u,  /* None */
  0x08u,  /* Odd */
  0x00u   /* Even */
};

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t stopBitsTable[d_UART_STOP_BITS_COUNT] =
{
  0x00u,  /* 1 bit */
  0x80u   /* 2 bits */
};

/***** Type Definitions *************************************************/

typedef struct
{
  Uint8_t  buffer[TRANSMIT_BUFFER_LENGTH];
  Uint32_t indexIn;
  Uint32_t indexOut;
  Uint32_t count;
  Uint32_t maxUsed;
  Uint32_t dropped;
} transmitBuffer_t;

typedef struct
{
  Uint8_t  buffer[RECEIVE_BUFFER_LENGTH];
  Uint32_t indexIn;
  Uint32_t indexOut;
  Uint32_t count;
  Uint32_t maxUsed;
  Uint32_t dropped;
} receiveBuffer_t;

/***** Variables ********************************************************/

/* Transmit buffer */
static transmitBuffer_t transmitBuffer[2];

/* Receive buffer */
static receiveBuffer_t receiveBuffer[2];

/***** Function Declarations ********************************************/

static void outputChar(const Uint32_t uart);
static void inputChar(const Uint32_t uart);
static void bufferInitialise(const Uint32_t uart);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_UART_PsConfigure -->

  Configure a UART interface.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_UART_PsConfigure
(
const Uint32_t uart,              /**< [in] UART channel */
const Uint32_t baud,              /**< [in] Baudrate */
const d_UART_DataBits_t dataBits, /**< [in] Number of data bits */
const d_UART_Parity_t parity,     /**< [in] Parity selection */
const d_UART_StopBits_t stopBits  /**< [in] Number of stop bits */
)
{
  d_Status_t status;

  /* Setting the baudrate verifies the uart and baud parameters */
  status = d_UART_PsSetBaudRate(uart, baud);
  
  if (status == d_STATUS_SUCCESS)
  {
    bufferInitialise(uart);

    Uint32_t registerMr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_MR_OFFSET);

    /* Mask all but mode settings */
    registerMr = registerMr & ~(XUARTPS_MR_CHARLEN_MASK | XUARTPS_MR_PARITY_MASK | XUARTPS_MR_STOPMODE_MASK);

    /* Insert settings */
    registerMr = registerMr | (Uint32_t)dataBitsTable[dataBits] |
                              (Uint32_t)parityTable[parity] |
                              (Uint32_t)stopBitsTable[stopBits];

    /* Write back  mode register */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_MR_OFFSET, registerMr);

    /* Disable all UART interrupts */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_IDR_OFFSET, XUARTPS_IXR_MASK);

    /* Output a character to get the transmit FIFO empty flag set */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_FIFO_OFFSET, 0);

    /* Set for 20 bit times timeout, that is about 2 characters */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_RXTOUT_OFFSET, 5);

    /* Enable receive interrupt */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_IER_OFFSET, XUARTPS_IXR_RXFULL | XUARTPS_IXR_TOUT);

    status = d_INT_IrqEnable(d_UART_PS_Configuration[uart].interruptNumber);
  }
  else
  {
    DO_NOTHING();
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_UART_PsSetBaudRate -->

  Set the UART baudrate.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PsSetBaudRate
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t baud     /**< [in] Baudrate */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint32_t inputClock;
  Uint32_t Best_BRGR = 0;
  Uint8_t Best_BAUDDIV = 0;
  
  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  bufferInitialise(uart);

  /* Check whether the input clock is divided by 8 */
  Uint32_t registerMr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_MR_OFFSET);

  inputClock = d_UART_PS_Configuration[uart].clockFrequency;
  if ((registerMr & (Uint32_t)XUARTPS_MR_CLKSEL) != 0u)
  {
    // gcov-jst 1 It is not practical to execute this statement during bench testing.
    inputClock = d_UART_PS_Configuration[uart].clockFrequency / 8u;
  }
  ELSE_DO_NOTHING

  if (baud > (inputClock / 5u))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, baud, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t BaudError;
    Uint32_t Best_Error = 0xFFFFFFFFU;

    /* Determine the Baud divider. It can be 4 to 254. Loop through all possible combinations */
    for (Uint32_t IterBAUDDIV = 4u; IterBAUDDIV < 255u; IterBAUDDIV++)
    {
      /* Calculate the value for BRGR register */
      Uint32_t BRGR_Value = inputClock / (baud * (IterBAUDDIV + 1u));

      /* Calculate the baud rate from the BRGR value */
      Uint32_t CalcBaudRate = inputClock / (BRGR_Value * (IterBAUDDIV + 1u));

      /* Avoid unsigned integer underflow */
      if (baud > CalcBaudRate)
      {
        BaudError = baud - CalcBaudRate;
      }
      else
      {
        BaudError = CalcBaudRate - baud;
      }

      /* Find the calculated baud rate closest to requested baud rate. */
      if (Best_Error > BaudError)
      {
        Best_BRGR = BRGR_Value;
        Best_BAUDDIV = (Uint8_t)IterBAUDDIV;
        Best_Error = BaudError;
      }
      ELSE_DO_NOTHING
    }

    /* Make sure the best error is not too large. */
    Uint32_t PercentError = (Best_Error * 100u) / baud;
    if (PercentError > XUARTPS_MAX_BAUD_ERROR_RATE)
    {
      status = d_STATUS_FAILURE;
    }
    ELSE_DO_NOTHING
  }

  if (status == d_STATUS_SUCCESS)
  {
    /* Disable TX and RX to avoid glitches when setting the baud rate. */
    Uint32_t registerCr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_CR_OFFSET);
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_CR_OFFSET,
                        (registerCr & ~XUARTPS_CR_EN_DIS_MASK) | XUARTPS_CR_RX_DIS | XUARTPS_CR_TX_DIS);

    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_BAUDGEN_OFFSET, Best_BRGR);
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_BAUDDIV_OFFSET, Best_BAUDDIV);

    /* RX and TX SW reset */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_CR_OFFSET, XUARTPS_CR_TXRST | XUARTPS_CR_RXRST);

    /* Enable device */
    registerCr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_CR_OFFSET);
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_CR_OFFSET,
                        (registerCr & ~XUARTPS_CR_EN_DIS_MASK) | XUARTPS_CR_RX_EN | XUARTPS_CR_TX_EN);

    /* Set for 20 bit times timeout, that is about 2 characters */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_RXTOUT_OFFSET, 5);

    /* Enable receive interrupt */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_IER_OFFSET, XUARTPS_IXR_RXFULL | XUARTPS_IXR_TOUT);
  }
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PsTransmit -->

  Transmit a message.
*************************************************************************/
d_Status_t                     /** \return Success or Failure */
d_UART_PsTransmit
(
const Uint32_t uart,           /**< [in] UART channel */
const Uint8_t * const buffer,  /**< [in] Pointer to data to transmit */
const Uint32_t length          /**< [in] Number of bytes to transmit */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  
  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (buffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (length > (TRANSMIT_BUFFER_LENGTH - transmitBuffer[uart].count))
  {
    transmitBuffer[uart].dropped =  transmitBuffer[uart].dropped + length;
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_BUFFER_FULL;
  }

  Uint32_t index;
  Uint32_t bufferCount;
  Uint32_t stat;

  bufferCount = length;
  /* If transmit buffer empty then write whatever we can directly to UART */
  Uint64_t interruptFlags = d_INT_CriticalSectionEnter();
  stat = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);
  index = 0;
  if (transmitBuffer[uart].count == 0u)
  {
    while (((stat & XUARTPS_SR_TXFULL) == 0u) && (bufferCount > 0u))
    {
      d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_FIFO_OFFSET, buffer[index]);
      index++;
      stat = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);
      bufferCount--;
    }
  }
  else
  {
    DO_NOTHING();
  }
  d_INT_CriticalSectionLeave(interruptFlags);

  /* Add remaining characters to the buffer */
  for (; index < length; index++)
  {
    transmitBuffer[uart].buffer[transmitBuffer[uart].indexIn] = buffer[index];
    transmitBuffer[uart].indexIn = (transmitBuffer[uart].indexIn + 1u) % TRANSMIT_BUFFER_LENGTH;
  }

  interruptFlags = d_INT_CriticalSectionEnter();
  transmitBuffer[uart].count = transmitBuffer[uart].count + bufferCount;
  if (transmitBuffer[uart].count > transmitBuffer[uart].maxUsed)
  {
    transmitBuffer[uart].maxUsed = transmitBuffer[uart].count;
  }
  else
  {
    DO_NOTHING();
  }
  d_INT_CriticalSectionLeave(interruptFlags);

  /* If interrupts were disabled then flush the buffer */
  if ((interruptFlags & 0x80u) != 0u)
  {
    (void)d_UART_PsFlushTx(uart);
  }
  else
  {
    /* Ensure transmit empty interrupt enabled */
    d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_IER_OFFSET, XUARTPS_IXR_TXEMPTY);
    /* and UART interrupts are enabled */
    status = d_INT_IrqEnable(d_UART_PS_Configuration[uart].interruptNumber);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PsReceive -->

  Receive a message.
*************************************************************************/
d_Status_t                    /** \return Success or Failure */
d_UART_PsReceive
(
const Uint32_t uart,          /**< [in]  UART channel */
Uint8_t * const buffer,       /**< [out] Pointer to buffer for received data */
const Uint32_t length,        /**< [in]  Maximum number of bytes to receive */
Uint32_t * const pBytesRead   /**< [out] Pointer to storage for number of bytes read */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  
  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (buffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (length == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pBytesRead == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t readCount;
  Uint32_t index;
  if (length < receiveBuffer[uart].count)
  {
    readCount = length;
  }
  else
  {
    readCount = receiveBuffer[uart].count;
  }
  for (index = 0; index < readCount; index++)
  {
    buffer[index] = receiveBuffer[uart].buffer[(receiveBuffer[uart].indexOut + index) % RECEIVE_BUFFER_LENGTH];
  }
  *pBytesRead = readCount;
  if (readCount == 0u)
  {
    status = d_STATUS_BUFFER_EMPTY;
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PsDiscard -->

  Discard characters from the receive buffer.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PsDiscard
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t length   /**< [in] Number of bytes to discard */
)
{
  Uint32_t discardCount;

  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  Uint64_t interruptFlags = d_INT_CriticalSectionEnter();
  if (length < receiveBuffer[uart].count)
  {
    discardCount = length;
  }
  else
  {
    discardCount = receiveBuffer[uart].count;
  }
  receiveBuffer[uart].count = receiveBuffer[uart].count - discardCount;
  d_INT_CriticalSectionLeave(interruptFlags);
  receiveBuffer[uart].indexOut = (receiveBuffer[uart].indexOut + discardCount) % RECEIVE_BUFFER_LENGTH;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_UART_PsFlushRx -->

  Clear receive buffer.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PsFlushRx
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  d_Status_t status;

  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  status = d_UART_PsDiscard(uart, RECEIVE_BUFFER_LENGTH);

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PsFlushTx -->

  Output any remaining characters in queue without interrupts.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PsFlushTx
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  /* Write characters until buffer empty, or FIFO full */
  while (transmitBuffer[uart].count > 0u)
  {
    Uint32_t sr_reg = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);
    if ((sr_reg & (Uint32_t)XUARTPS_SR_TXFULL) == 0u)
    {
      outputChar(uart);
    }
    else
    {
      DO_NOTHING();
    }
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_UART_PsLoopback -->

  Enable or disable loopback.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PsLoopback
(
const Uint32_t uart,    /**< [in] UART channel */
const Bool_t enable     /**< [in] Enable or disable */
)
{
  if (uart >= d_UART_PsCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  Uint32_t registerMr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_MR_OFFSET);
  if (enable == d_TRUE)
  {
    registerMr = registerMr | XUARTPS_MR_CHMODE_L_LOOP;
  }
  else
  {
    registerMr = registerMr & ~XUARTPS_MR_CHMODE_L_LOOP;
  }
  d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_MR_OFFSET, registerMr);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_UART_PsInterruptHandler -->

  Process a UART interrupt.
*************************************************************************/
void                    /** \return None */
d_UART_PsInterruptHandler
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  Uint32_t registerIsr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_ISR_OFFSET);
  Uint32_t registerImr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_IMR_OFFSET);
  Uint32_t registerSr  = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);

  /* Check for transmit FIFO empty */
  if (((registerIsr & registerImr) & (Uint32_t)XUARTPS_IXR_TXEMPTY) != 0u)
  {
    /* Write characters until buffer empty, or FIFO full */
    while (((registerSr & XUARTPS_SR_TXFULL) == 0u) && (transmitBuffer[uart].count > 0u))
    {
      outputChar(uart);
      registerSr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);
    }
    /* If buffer empty then turn off transmit FIFO empty interrupt */
    if (transmitBuffer[uart].count == 0u)
    {
      d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_IDR_OFFSET, XUARTPS_IXR_TXEMPTY);
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

  /* Check for receive FIFO full or receive timeout */
  if (((registerIsr & registerImr) & ((Uint32_t)XUARTPS_IXR_TOUT | (Uint32_t)XUARTPS_IXR_RXFULL)) != 0u)
  {
    /* Read all characters until buffer full, or FIFO empty */
    while (((registerSr & (Uint32_t)XUARTPS_SR_RXEMPTY) == 0u) && (receiveBuffer[uart].count < RECEIVE_BUFFER_LENGTH))
    {
      inputChar(uart);
      registerSr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);
    }
    /* discard any other characters when buffer full */
    while ((registerSr & (Uint32_t)XUARTPS_SR_RXEMPTY) == 0u)
    {
      (void)d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_FIFO_OFFSET);
      registerSr = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_SR_OFFSET);
    }
  }
  else
  {
    DO_NOTHING();
  }

  /* Ack all interrupts */
  d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_ISR_OFFSET, registerIsr);

  return;
}

/*********************************************************************//**
  <!-- outputChar -->

  Output a character to the UART.
*************************************************************************/
static void             /** \return None */
outputChar
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  d_GEN_RegisterWrite(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_FIFO_OFFSET, transmitBuffer[uart].buffer[transmitBuffer[uart].indexOut]);
  transmitBuffer[uart].indexOut = (transmitBuffer[uart].indexOut + 1u) % TRANSMIT_BUFFER_LENGTH;
  transmitBuffer[uart].count--;

  return;
}

/*********************************************************************//**
  <!-- inputChar -->

  Read a character from the UART.
*************************************************************************/
static void             /** \return None */
inputChar
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  receiveBuffer[uart].buffer[receiveBuffer[uart].indexIn] = d_GEN_RegisterRead(d_UART_PS_Configuration[uart].baseAddress + XUARTPS_FIFO_OFFSET);
  receiveBuffer[uart].indexIn = (receiveBuffer[uart].indexIn + 1u) % RECEIVE_BUFFER_LENGTH;
  receiveBuffer[uart].count++;

  return;
}

/*********************************************************************//**
  <!-- bufferInitialise -->

  Initialise the transmit and receive buffers.
*************************************************************************/
static void             /** \return None */
bufferInitialise
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  transmitBuffer[uart].indexIn = 0;
  transmitBuffer[uart].indexOut = 0;
  transmitBuffer[uart].count = 0;
  transmitBuffer[uart].maxUsed = 0;
  transmitBuffer[uart].dropped = 0;

  receiveBuffer[uart].indexIn = 0;
  receiveBuffer[uart].indexOut = 0;
  receiveBuffer[uart].count = 0;
  receiveBuffer[uart].maxUsed = 0;
  receiveBuffer[uart].dropped = 0;

  return;
}

