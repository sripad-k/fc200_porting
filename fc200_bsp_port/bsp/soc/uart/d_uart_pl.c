/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART_PL

  Abstract           : SOC UART PL interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-215
                       SDD References: 136T-2200-131000-001-D22 SWDES-544
                                                                SWDES-592
                                                                SWDES-593
                                                                SWDES-594
                                                                SWDES-595
                                                                SWDES-596
                                                                SWDES-597
                                                                SWDES-598
                                                                SWDES-599
                                                                SWDES-600
                                                                SWDES-601
                                                                SWDES-602
                                                                SWDES-603
\note
  CSC ID             : SWDES-57
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_register.h"
#include "soc/interrupt_manager/d_int_critical.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/error_handler/d_error_handler.h"
#include "sru/fcu/d_fcu_cfg.h"

#include "xparameters.h"
#include "xuartps.h"

#include "d_uart_pl_cfg.h"
#include "d_uart_pl.h"

/***** Constants ********************************************************/

#define TRANSMIT_BUFFER_LENGTH 2048u
#define RECEIVE_BUFFER_LENGTH  2048u

/* The following constant defines the amount of error that is allowed for
 * a specified baud rate. This error is the difference between the actual
 * baud rate that will be generated using the specified clock and the
 * desired baud rate.
 */
#define XUARTPS_MAX_BAUD_ERROR_RATE     3u  /* max % error allowed */

#define UART_FIFO_SIZE   16u

/* Register offset addresses */
#define UART_RBR 0x0000u    /* Receive buffer */
#define UART_THR 0x0000u    /* Transmit holding */
#define UART_IER 0x0004u    /* Interrupt enable */
#define UART_IIR 0x0008u    /* Interrupt identification */
#define UART_FCR 0x0008u    /* FIFO control */
#define UART_LCR 0x000Cu    /* Line control */
#define UART_MCR 0x0010u    /* Modem control */
#define UART_LSR 0x0014u    /* Line status */
#define UART_MSR 0x0018u    /* Modem status */
#define UART_SCR 0x001Cu    /* Scratch */
#define UART_DLL 0x0000u    /* Divisor latch least significant */
#define UART_DLM 0x0004u    /* Divisor latch most significant */

/* Interrupt identification */
#define INTERRUPT_MODEM_STATUS     0x00u
#define INTERRUPT_TRANSMIT_EMPTY   0x02u
#define INTERRUPT_RECEIVE_READY    0x04u
#define INTERRUPT_LINE_STATUS      0x06u
#define INTERRUPT_CHAR_TIMEOUT     0x0Cu

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t dataBitsTable[d_UART_DATA_BITS_COUNT] =
{
  0x02u,  /* 7 bits */
  0x03u   /* 8 bits */
};

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t parityTable[d_UART_PARITY_COUNT] =
{
  0x00u,  /* None */
  0x08u,  /* Odd */
  0x18u   /* Even */
};

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint8_t stopBitsTable[d_UART_STOP_BITS_COUNT] =
{
  0x00u,  /* 1 bit */
  0x04u   /* 2 bits */
};


/***** Type Definitions *************************************************/

typedef enum
{
  FUNC_d_UART_PlConfigure,
  FUNC_d_UART_PlSetBaudRate,
  FUNC_d_UART_PlTransmit,
  FUNC_d_UART_PlReceive,
  FUNC_d_UART_PlDiscard,
  FUNC_d_UART_PlFlush,
  FUNC_d_UART_PlLoopback,
  FUNC_d_UART_PlInterruptHandler
} function_t;

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
static transmitBuffer_t transmitBuffer[d_UART_PL_MAX_INTERFACES];

/* Receive buffer */
static receiveBuffer_t receiveBuffer[d_UART_PL_MAX_INTERFACES];

/***** Function Declarations ********************************************/

static void outputChar(const Uint32_t uart);
static void inputChar(const Uint32_t uart);
static void bufferInitialise(const Uint32_t uart);
static Uint8_t uartRegisterRead(const Uint32_t uart, const Uint32_t reg);
static void uartRegisterWrite(const Uint32_t uart, const Uint32_t reg, const Uint8_t value);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_UART_PlConfigure -->

  Configure a UART interface.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_UART_PlConfigure
(
const Uint32_t uart,              /**< [in] UART channel */
const Uint32_t baud,              /**< [in] Baudrate */
const d_UART_DataBits_t dataBits, /**< [in] Number of data bits */
const d_UART_Parity_t parity,     /**< [in] Parity selection */
const d_UART_StopBits_t stopBits  /**< [in] Number of stop bits */
)
{
  d_Status_t status;

  status = d_FCU_IocAddressCheck(d_UART_PL_Configuration[uart].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    /* Setting the baudrate verifies the uart and baud parameters */
    status = d_UART_PlSetBaudRate(uart, baud);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  if (status == d_STATUS_SUCCESS)
  {
    bufferInitialise(uart);

    /* Disable all UART interrupts */
    uartRegisterWrite(uart, UART_IER, 0x00u);
  
    /* Enable FIFO with 14 byte trigger level, DMA mode 0 */
    /* It has been found that the first write is not always effective and a second is necessary */
    uartRegisterWrite(uart, UART_FCR, 0xC7u);
    uartRegisterWrite(uart, UART_FCR, 0xC7u);

    /* No loopback */
    uartRegisterWrite(uart, UART_MCR, 0x00u);

    /* Setup line characteristics */
    uartRegisterWrite(uart, UART_LCR, (Uint32_t)dataBitsTable[dataBits] |
                                      (Uint32_t)parityTable[parity] |
                                      (Uint32_t)stopBitsTable[stopBits]);
  
    /* Dummy reads to clear possible interrupt */
    (void)uartRegisterRead(uart, UART_LSR);
    (void)uartRegisterRead(uart, UART_MSR);
    (void)uartRegisterRead(uart, UART_IIR);

    /* Enable receive interrupt */
    uartRegisterWrite(uart, UART_IER, 0x01u);

    status = d_INT_IrqEnable(d_UART_PL_Configuration[uart].interruptNumber);
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PlSetBaudRate -->

  Set the UART baudrate.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PlSetBaudRate
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t baud     /**< [in] Baudrate */
)
{
  Uint32_t divisor;
  uint32_t uartBaudClock;
  d_Status_t status;

  if (uart >= d_UART_PlCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  uartBaudClock = d_UART_PL_Configuration[uart].clockFrequency / 16u;

  if (baud > uartBaudClock)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, baud, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = d_FCU_IocAddressCheck(d_UART_PL_Configuration[uart].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t actual;
    Uint32_t error;
    Uint32_t percentError;

    bufferInitialise(uart);

    Float32_t fdivisor = ((Float32_t)uartBaudClock / (Float32_t)baud) + 0.5f;
    divisor = (Uint32_t)fdivisor;
  
    actual = uartBaudClock / divisor;
  
    if (baud > actual)
    {
      error = baud - actual;
    }
    else
    {
      error = actual - baud;
    }

    /* Make sure the error is not too large. */
    percentError = (error * 100u) / baud;
    if (percentError > XUARTPS_MAX_BAUD_ERROR_RATE)
    {
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
    Uint8_t lcr = uartRegisterRead(uart, UART_LCR);
    /* enable access to baud rate registers */
    uartRegisterWrite(uart, UART_LCR, lcr | 0x80u);
    /* Write divisor */
    uartRegisterWrite(uart, UART_DLL, divisor & 0xFFu);
    uartRegisterWrite(uart, UART_DLM, divisor >> 8u);
    /* revert to original LCR value */
    uartRegisterWrite(uart, UART_LCR, lcr);
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PlTransmit -->

  Transmit a message.
*************************************************************************/
d_Status_t                     /** \return Success or Failure */
d_UART_PlTransmit
(
const Uint32_t uart,           /**< [in] UART channel */
const Uint8_t * const buffer,  /**< [in] Pointer to data to transmit */
const Uint32_t length          /**< [in] Number of bytes to transmit */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart >= d_UART_PlCount)
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
    transmitBuffer[uart].dropped = transmitBuffer[uart].dropped + length;
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_BUFFER_FULL;
  }

  status = d_FCU_IocAddressCheck(d_UART_PL_Configuration[uart].baseAddress);
  if (status != d_STATUS_SUCCESS)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return status;
  }

  Uint32_t index;
  Uint32_t stat;
  Uint32_t sendNowCount = UART_FIFO_SIZE;
  Uint64_t interruptFlags;

  if (sendNowCount > length)
  {
    sendNowCount = length;
  }
  ELSE_DO_NOTHING

  /* If transmit buffer empty then write whatever we can directly to UART */
  interruptFlags = d_INT_CriticalSectionEnter();
  stat = uartRegisterRead(uart, UART_LSR);
  index = 0;
  if ((transmitBuffer[uart].count == 0u) && ((stat & 0x20u) != 0u))
  {
    for (index = 0; index < sendNowCount; index++)
    {
      uartRegisterWrite(uart, UART_THR, buffer[index]);
    }
  }
  else
  {
    sendNowCount = 0;
  }
  d_INT_CriticalSectionLeave(interruptFlags);

  /* Add remaining characters to the buffer */
  for (; index < length; index++)
  {
    transmitBuffer[uart].buffer[transmitBuffer[uart].indexIn] = buffer[index];
    transmitBuffer[uart].indexIn = (transmitBuffer[uart].indexIn + 1u) % TRANSMIT_BUFFER_LENGTH;
  }

  interruptFlags = d_INT_CriticalSectionEnter();
  transmitBuffer[uart].count = transmitBuffer[uart].count + length - sendNowCount;
  if (transmitBuffer[uart].count > transmitBuffer[uart].maxUsed)
  {
    transmitBuffer[uart].maxUsed = transmitBuffer[uart].count;
  }
  ELSE_DO_NOTHING
  d_INT_CriticalSectionLeave(interruptFlags);

  /* If interrupts were disabled then flush the buffer */
  if ((interruptFlags & 0x80u) != 0u)
  {
    (void)d_UART_PlFlushTx(uart);
  }
  else
  {
    /* Ensure transmit empty interrupt enabled */
    uartRegisterWrite(uart, UART_IER, uartRegisterRead(uart, UART_IER) | 0x02u);
    /* and UART interrupts are enabled */
    status = d_INT_IrqEnable(d_UART_PL_Configuration[uart].interruptNumber);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PlReceive -->

  Receive a message.
*************************************************************************/
d_Status_t                    /** \return Success or Failure */
d_UART_PlReceive
(
const Uint32_t uart,          /**< [in]  UART channel */
Uint8_t * const buffer,       /**< [out] Pointer to buffer for received data */
const Uint32_t length,        /**< [in]  Maximum number of bytes to receive */
Uint32_t * const pBytesRead   /**< [out] Pointer to storage for number of bytes read */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (uart >= d_UART_PlCount)
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
  <!-- d_UART_PlDiscard -->

  Discard characters from the receive buffer.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PlDiscard
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t length   /**< [in] Number of bytes to discard */
)
{
  if (uart >= d_UART_PlCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t discardCount;
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
  <!-- d_UART_PlFlushRx -->

  Clear receive buffer.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PlFlushRx
(
const Uint32_t uart
)
{
  if (uart >= d_UART_PlCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_UART_PlDiscard(uart, RECEIVE_BUFFER_LENGTH);

  return status;
}

/*********************************************************************//**
}
  <!-- d_UART_PlFlushTx -->

  Output any remaining characters in queue without interrupts.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PlFlushTx
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  if (uart >= d_UART_PlCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_UART_PL_Configuration[uart].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    /* Write characters until buffer empty, or FIFO full */
    while (transmitBuffer[uart].count > 0u)
    {
      Uint32_t lsr = uartRegisterRead(uart, UART_LSR);
      if ((lsr & 0x20u) != 0u)
      {
        Uint32_t sendNowCount = UART_FIFO_SIZE;
        if (transmitBuffer[uart].count < sendNowCount)
        {
          sendNowCount = transmitBuffer[uart].count;
        }
        for (Uint32_t index = 0; index < sendNowCount; index++)
        {
          outputChar(uart);
        }
      }
      else
      {
        DO_NOTHING();
      }
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
  <!-- d_UART_PlLoopback -->

  Enable or disable loopback.
*************************************************************************/
d_Status_t              /** \return Success or Failure */
d_UART_PlLoopback
(
const Uint32_t uart,    /**< [in] UART channel */
const Bool_t enable     /**< [in] Enable or disable */
)
{
  if (uart >= d_UART_PlCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, uart, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_UART_PL_Configuration[uart].baseAddress);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t registerMcr = uartRegisterRead(uart, UART_MCR);
    if (enable == d_TRUE)
    {
      registerMcr = registerMcr | 0x10u;
    }
    else
    {
      registerMcr = registerMcr & 0xEFu;
    }
    uartRegisterWrite(uart, UART_MCR, registerMcr);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_UART_PlInterruptHandler -->

  Process a UART interrupt.
*************************************************************************/
void                    /** \return None */
d_UART_PlInterruptHandler
(
const Uint32_t uart     /**< [in] UART channel */
)
{
  Uint32_t registerLsr;
  Uint32_t registerIir;
  Uint32_t sendNowCount;

  d_Status_t status = d_FCU_IocAddressCheck(d_UART_PL_Configuration[uart].baseAddress);
  if (status != d_STATUS_SUCCESS)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  
  registerIir = uartRegisterRead(uart, UART_IIR);

  do
  {
    switch (registerIir & 0x0Eu)
    {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    case INTERRUPT_MODEM_STATUS:
      (void)uartRegisterRead(uart, UART_MSR);
      break;

    case INTERRUPT_TRANSMIT_EMPTY:
      /* Write characters until buffer empty, or FIFO full */
      sendNowCount = UART_FIFO_SIZE;
      if (sendNowCount > transmitBuffer[uart].count)
      {
        sendNowCount = transmitBuffer[uart].count;
      }
      else
      {
        DO_NOTHING();
      }
      for (Uint32_t index = 0; index < sendNowCount; index++)
      {
        outputChar(uart);
      }
      /* If buffer empty then turn off transmit FIFO empty interrupt */
      if (transmitBuffer[uart].count == 0u)
      {
        uartRegisterWrite(uart, UART_IER, uartRegisterRead(uart, UART_IER) & 0xFDu);
      }
      else
      {
        DO_NOTHING();
      }
      break;

    case INTERRUPT_RECEIVE_READY:
    case INTERRUPT_CHAR_TIMEOUT:
      /* Read all characters until buffer full, or FIFO empty */
      registerLsr = uartRegisterRead(uart, UART_LSR);
      while (((registerLsr & 0x01u) != 0u) && (receiveBuffer[uart].count < RECEIVE_BUFFER_LENGTH))
      {
        inputChar(uart);
        registerLsr = uartRegisterRead(uart, UART_LSR);
      }
      /* discard any other characters when buffer full */
      while ((registerLsr & 0x01u) != 0u)
      {
        (void)uartRegisterRead(uart, UART_RBR);
        registerLsr = uartRegisterRead(uart, UART_LSR);
      }
      break;

    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    case INTERRUPT_LINE_STATUS:
      registerLsr = uartRegisterRead(uart, UART_LSR);
      break;

    // gcov-jst 2 Default case for robustness purposes only. It will never be executed.
    default:
      break;
    }

    registerIir = uartRegisterRead(uart, UART_IIR);
  } while ((registerIir & 0x01u) == 0u);

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
  uartRegisterWrite(uart, UART_THR, transmitBuffer[uart].buffer[transmitBuffer[uart].indexOut]);
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
  receiveBuffer[uart].buffer[receiveBuffer[uart].indexIn] = uartRegisterRead(uart, UART_RBR);
  receiveBuffer[uart].indexIn = (receiveBuffer[uart].indexIn + 1u) % RECEIVE_BUFFER_LENGTH;
  receiveBuffer[uart].count++;
  if (receiveBuffer[uart].count > receiveBuffer[uart].maxUsed)
  {
    receiveBuffer[uart].maxUsed = receiveBuffer[uart].count;
  }
  else
  {
    DO_NOTHING();
  }

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

/*********************************************************************//**
  <!-- uartRegisterRead -->

  Read a UART register.
*************************************************************************/
static Uint8_t          /** \return Register value */
uartRegisterRead
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t reg      /**< [in] Register to read */
)
{
  return d_GEN_RegisterRead(d_UART_PL_Configuration[uart].baseAddress + reg);
}

/*********************************************************************//**
  <!-- uartRegisterWrite -->

  Write a UART register.
*************************************************************************/
static void             /** \return None */
uartRegisterWrite
(
const Uint32_t uart,    /**< [in] UART channel */
const Uint32_t reg,     /**< [in] Register to write */
const Uint8_t value     /**< [in] Value to write */
)
{
  d_GEN_RegisterWrite(d_UART_PL_Configuration[uart].baseAddress + reg, value);
  
  return;
}

