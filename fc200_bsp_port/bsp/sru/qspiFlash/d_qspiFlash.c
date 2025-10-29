/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : QSPI Flash

  Abstract           : QSPI Flash driver

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-141
                                                                SWREQ-142
                                                                SWREQ-143
                       SDD References: 136T-2200-131000-001-D22 SWDES-254
                                                                SWDES-255
                                                                SWDES-256
                                                                SWDES-257
                                                                SWDES-258
\note
  CSC ID             : SWDES-66
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"               /* Common type definitions */
#include "soc/defines/d_common_status.h"              /* Common status and error definitions */
#include "kernel/error_handler/d_error_handler.h"     /* Error handler */
#include "kernel/general/d_gen_register.h"            /* Register functions */
#include "soc/memory_manager/d_memory_cache.h"        /* Memory Manager: Cache functionality */
#include "soc/timer/d_timer.h"                        /* Timer driver module */
#include "xparameters.h"                              /* Xilinx definitions for Zynq Ultra-scale */

#include "d_qspiFlash.h"                              /* QSPI Flash module header file */

/***** Literals *********************************************************/

/* Discrete used to reset the QSPI device */
#define RESET_DISCRETE  d_DISC_MIO_7
/* Duration of the reset signal in microseconds */
#define RESET_TIME      10

/* QSPI module mode */
typedef enum
{
  d_QSPI_UNINITIALISED = 0u, /* QPSI is not initialised */
  d_QSPI_DMA_MODE      = 1u, /* QPSI is set to DMA mode */
  d_QSPI_IO_MODE       = 2u  /* QPSI is set to IO mode */
} d_QSPI_Mode_t;

/***** Constants ********************************************************/
/* QSPI Hardware configuration dependable constants */
const static Uint32_t d_QSPI_FLASH_SIZE_IN_BYTES     = 0x04000000u; /* 512 Megabits = 64 Megabytes (16384 Number of 4KB sub sectors) */
const static Uint32_t d_QSPI_WORD_SHIFT = 0x00000002u; /* Constant to shift between words and bytes */

/* QSPI Base address */
const static Uint32_t QSPI_BASEADDR = 0xFF0F0000u;      /* QSPI controller base address */
const static Uint32_t QSPI_DMA_WORDS_MAX = 0x04000000u; /* QSPI DMA maximum number of words (Bytes: 0x10000000u) */

/* Register: GQSPI_CFG */
const static Uint32_t GQSPI_CFG_OFFSET = 0x00000100u;
const static Uint32_t GQSPI_CFG_MODE_EN_MASK = 0xC0000000u;
const static Uint32_t GQSPI_CFG_MODE_EN_DMA_MASK = 0x80000000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_GEN_FIFO_START_MODE_MASK = 0x20000000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_START_GEN_FIFO_MASK = 0x10000000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_ENDIAN_MASK = 0x04000000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_EN_POLL_TO_MASK = 0x00100000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_WP_HOLD_MASK = 0x00080000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_BAUD_RATE_DIV_MASK = 0x00000038u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_BAUD_RATE_DIV_2 = 0x00000000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_CLK_PHA_MASK = 0x00000004u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_CFG_CLK_POL_MASK = 0x00000002u;

/* Register: GQSPI_ISR */
const static Uint32_t GQSPI_ISR_OFFSET = 0x00000104u;
const static Uint32_t GQSPI_ISR_RXEMPTY_MASK = 0x00000800u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_ISR_TXEMPTY_MASK = 0x00000100u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_ISR_GENFIFOEMPTY_MASK = 0x00000080u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_ISR_WR_TO_CLR_MASK = 0x00000002u;

/* Register: GQSPI_IDR */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_IDR_OFFSET = 0x0000010Cu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_IDR_ALL_MASK = 0x0FBEu;

/* Register: GQSPI_EN_REG */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_EN_OFFSET = 0x00000114u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_EN_MASK = 0x00000001u;

/* Register: GQSPI_TXD */
const static Uint32_t GQSPI_TXD_OFFSET = 0x0000011Cu;

/* Register: XQSPIPSU_RXD */
const static Uint32_t GQSPI_RXD_OFFSET = 0x00000120u;

/* Register: GQSPI_TX_THRESHOLD */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_TX_THRESHOLD_OFFSET = 0x00000128u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_TX_FIFO_THRESHOLD_RESET_VAL = 0x01u;

/* Register: GQSPI_RX_THRESHOLD */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_RX_THRESHOLD_OFFSET = 0x0000012Cu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_RX_FIFO_THRESHOLD_RESET_VAL = 0x01u;

/* Register: GQSPI_LPBK_DLY_ADJ */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_LPBK_DLY_ADJ_OFFSET = 0x00000138u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_LPBK_DLY_ADJ_USE_LPBK_MASK = 0x00000020u;

/* Register: GQSPI_GEN_FIFO */
const static Uint32_t GQSPI_GENFIFO_OFFSET            = 0x00000140u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GENFIFO_CS_HOLD_TIME      = 0x00000004u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GENFIFO_CS_SETUP_TIME     = 0x00000005u;
const static Uint32_t GQSPI_GENFIFO_MODE_SPI          = 0x00000400u;
const static Uint32_t GQSPI_GENFIFO_MODE_QUADSPI      = 0x00000C00u;
const static Uint32_t GQSPI_GENFIFO_CS_LOWER          = 0x00001000u;
const static Uint32_t GQSPI_GENFIFO_BUS_LOWER         = 0x00004000u;
const static Uint32_t GQSPI_GENFIFO_DATA_XFER         = 0x00000100u;
const static Uint32_t GQSPI_GENFIFO_TX                = 0x00010000u;
const static Uint32_t GQSPI_GENFIFO_RX                = 0x00020000u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GENFIFO_DUMMY_CLOCKS_LEN  = 0x00000008u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GENFIFO_IMM_DATA_MASK     = 0x000000FFu;
const static Uint32_t GQSPI_GENFIFO_EXP               = 0x00000200u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GENFIFO_EXP_MASK          = 0x00000001u;

/* Register: GQSPI_SEL */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_SEL_OFFSET = 0x00000144u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_SEL_GQSPI = 0x00000001u;

/* Register: GQSPI_FIFO_CTRL */
const static Uint32_t GQSPI_FIFO_CTRL_OFFSET = 0x0000014Cu;
const static Uint32_t GQSPI_FIFO_CTRL_RST_RX_FIFO_MASK = 0x00000004u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_FIFO_CTRL_RST_TX_FIFO_MASK = 0x00000002u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_FIFO_CTRL_RST_GEN_FIFO_MASK = 0x00000001u;

/* Register: GQSPI_GF_THRESHOLD */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GF_THRESHOLD_OFFSET = 0x00000150u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_GEN_FIFO_THRESHOLD_RESET_VAL = 0x10u;

/* Register: GQSPI_DMA_DST_ADDR */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_ADDR_OFFSET = 0x00000800u;
const static Uint32_t GQSPI_DMA_DST_ADDR_MASK = 0xFFFFFFFCu;

/* Register: GQSPI_DMA_DST_SIZE */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_SIZE_OFFSET = 0x00000804u;

/* Register: GQSPI_DMA_DST_STS */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_STS_OFFSET = 0x00000808u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_STS_WTC    = 0x0000E000u;

/* Register: GQSPI_DMA_DST_CTRL */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_CTRL_OFFSET = 0x0000080Cu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_CTRL_RESET_VAL = 0x403FFA00u;

/* Register: GQSPI_DMA_DST_I_STS */
const static Uint32_t GQSPI_DMA_DST_I_STS_OFFSET = 0x00000814u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_I_STS_DONE_MASK = 0x00000002u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_INTR_ERR_MASK = 0x000000FCu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_INTR_ALL_MASK = 0x000000FEu;

/* Register: GQSPI_DMA_DST_I_DIS */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_I_DIS_OFFSET = 0x0000081Cu;

/* Register: GQSPI_DMA_DST_ADDR_MSB */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
const static Uint32_t GQSPI_DMA_DST_ADDR_MSB_OFFSET = 0x00000828u;

/* General */
const static Uint32_t TX_COMMAND_TIME_OUT_MICROSECONDS = 100u;

/***** Type Definitions *************************************************/

/* The QSPI driver instance data. */
typedef struct
{
  volatile d_QSPI_Mode_t qspiMode;              /* QSPI mode */
  volatile d_QSPI_ModuleStatus_t qspiModStatus; /* QSPI status */
  volatile Bool_t isBusy;                       /* A transfer is in progress */
  volatile Uint32_t* pProgramBuffer;            /* Pointer to buffer used for sending data to flash device for programming */
  volatile Uint32_t numWordsToProgram;          /* Number of 32bit words to program */
  volatile Uint32_t txFifoProgramWords;         /* Number of 32bit words transmitted (written to TX FIFO) for programming */
  volatile Uint32_t programmedWords;            /* Number of 32bit words transfer completed for programming */
  volatile Uint32_t qspiAddress;                /* QSPI flash memory address used for programming */
  volatile Uint32_t pageIndex;                  /* QSPI flash memory page index used for programming */
  volatile Uint32_t rxedWords;                  /* Number of 32bit words received (read from RX FIFO) */
  volatile Uint32_t* pRxBuffer;                 /* Pointer to buffer used for receiving data from flash device */
} d_QSPIO_handle_t;

/***** Variables ********************************************************/

static d_QSPIO_handle_t qspioHandle = {d_QSPI_UNINITIALISED,  /* qspiMode */
                                       d_QSPI_STATUS_UNKNOWN, /* qspiModStatus */
                                       d_FALSE,               /* isBusy */
                                       NULL,                  /* pProgramBuffer */
                                       0u,                    /* numWordsToProgram */
                                       0u,                    /* txFifoProgramWords */
                                       0u,                    /* programmedWords */
                                       0u,                    /* qspiAddress */
                                       0u,                    /* pageIndex */
                                       0u,                    /* rxedWords */
                                       NULL};                 /* pRxBuffer */

/***** Function Declarations ********************************************/

static d_Status_t performRead(const Uint32_t qspiAddress, const Uint32_t numWordsToRead, Uint32_t * const pReadBuffer);
static d_Status_t readStatusComplete(void);
static void setToDMAmode(void);
static void setToIOmode(void);
static void startGenericFifoCommandExecution(void);
static d_Status_t waitTransferCompleteWithTimeout(const Uint32_t timeoutMilliseconds);
static void chipSelectAssert(void);
static d_Status_t chipSelectDeassert(void);
static d_Status_t writeEnable(void);
static d_Status_t transferDummyClocks(void);
static d_Status_t transferCommand(const Uint32_t command, const Uint32_t qspiAddress);
static void populateGenFifoEntriesWithDataLength(Uint32_t numWords, Uint32_t* pGenFifoEntry);
static d_Status_t programPage(void);
static void setDefaultConfig(void);
static void qspiAbort(void);
static void qspiReset(void);

static d_Status_t readStatusRegister(Uint8_t * const pValue);
static d_Status_t writeStatusRegister(const Uint8_t value);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_QSPI_Initialise -->

  Initialise the QSPI flash driver.
*************************************************************************/
d_Status_t    /** \return QSPI flash driver initialisation status */
d_QSPI_Initialise
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  qspioHandle.qspiMode = d_QSPI_UNINITIALISED;
  qspioHandle.qspiModStatus = d_QSPI_STATUS_UNKNOWN;
  qspioHandle.isBusy = d_FALSE;
  qspioHandle.pProgramBuffer = NULL;
  qspioHandle.numWordsToProgram = 0u;
  qspioHandle.txFifoProgramWords = 0u;
  qspioHandle.programmedWords = 0u;
  qspioHandle.qspiAddress = 0u;
  qspioHandle.pageIndex = 0u;
  qspioHandle.rxedWords = 0u;
  qspioHandle.pRxBuffer = NULL;

  /* Select Generic QSPI mode */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_SEL_OFFSET, GQSPI_SEL_GQSPI);

  /* Reset to known state */
  qspiReset();

  /* Enable GQSPI controller */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_EN_OFFSET, GQSPI_EN_MASK);

  /* Update Mode and status of QSPI module */
  qspioHandle.qspiMode = d_QSPI_DMA_MODE;
  qspioHandle.qspiModStatus = d_QSPI_STATUS_IDLE;

  Uint8_t statusRegister = 0x00u;
  (void)readStatusRegister(&statusRegister);
  /* Check QSPI mode enabled */
  if ((statusRegister & 0x40u) == 0x00u)
  {
    /* If not then enable it */
    // gcov-jst 1 It is not practical to get this coverage. Requires a new device which has never been initialised.
    status = writeStatusRegister(0x40u);
  }
  else
  {
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_QSPI_Read -->

  Read QSPI flash memory with DMA into buffer.
*************************************************************************/
d_Status_t                            /** \return QSPI flash read initiation status */
d_QSPI_Read
(
const Uint32_t qspiAddress,           /**< [in] QSPI flash memory start address to read */
const Uint32_t numWordsToRead,        /**< [in] Number of words to read from QSPI flash memory */
Uint32_t* const pReadBuffer,          /**< [in] Read data will be stored at this pointer */
const Uint32_t readBufferSizeInWords  /**< [in] Read buffer size in words - QSPI module will check sufficiency */
)
{
  d_Status_t status = d_STATUS_SUCCESS;  /* Return parameter: Status of sub-sector read initiation */

  /* Out of bounds address and size */
  Uint32_t numBytesToRead = numWordsToRead << d_QSPI_WORD_SHIFT;
  if ((qspiAddress + numBytesToRead) > d_QSPI_FLASH_SIZE_IN_BYTES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, qspiAddress + numBytesToRead, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Nothing to do - 0 words to read OR bigger than the DMA can handle*/
  if ((numWordsToRead == 0u) || (numWordsToRead > QSPI_DMA_WORDS_MAX))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, numWordsToRead, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Invalid readBuffer OR not 32 bit aligned address */
  // cppcheck-suppress misra-c2012-11.4; Conversion necessary to check alignment validity. Violation of 'Advisory' rule does not present a risk.
  if ((pReadBuffer == NULL) || (((Uint32_t)pReadBuffer & GQSPI_DMA_DST_ADDR_MASK) != (Uint32_t)pReadBuffer))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Inadequate readBuffer size */
  if (readBufferSizeInWords < numWordsToRead)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, readBufferSizeInWords, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Initialisation check */
  if (qspioHandle.qspiMode == d_QSPI_UNINITIALISED)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Transfer in progress check */
  if (qspioHandle.isBusy == d_TRUE)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    status = d_STATUS_DEVICE_BUSY;
    d_ERROR_Logger(d_STATUS_DEVICE_BUSY, d_ERROR_CRITICALITY_NON_CRITICAL, 0, 0, 0, 0);
  }
  else
  {
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)  /* All checks passed - Initiate reading */
  {
    status = performRead(qspiAddress, numWordsToRead, pReadBuffer);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return (status);
}

/*********************************************************************//**
  <!-- d_QSPI_Write -->

  Program QSPI flash memory from buffer.
*************************************************************************/
d_Status_t                            /** \return QSPI program initiation status */
d_QSPI_Write
(
const Uint32_t qspiAddress,           /**< [in] QSPI flash memory start address to read */
const Uint32_t numWordsToWrite,       /**< [in] Number of words to read from QSPI flash memory */
Uint32_t* const pWriteBuffer,         /**< [in] Read data will be stored at this pointer */
const Uint32_t writeBufferSizeInWords /**< [in] Read buffer size in words - QSPI module will check sufficiency */
)
{
  d_Status_t status = d_STATUS_SUCCESS;  /* Return parameter: Status of sub-sector read initiation */

  /* Out of bounds address and size */
  Uint32_t numBytesToWrite = numWordsToWrite << d_QSPI_WORD_SHIFT;
  if ((qspiAddress + numBytesToWrite) >= d_QSPI_FLASH_SIZE_IN_BYTES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Nothing to do - 0 words to read OR bigger than the DMA can handle*/
  if ((numWordsToWrite == 0u) || (numWordsToWrite > QSPI_DMA_WORDS_MAX))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Invalid readBuffer OR not 32 bit aligned address */
  // cppcheck-suppress misra-c2012-11.4; Conversion necessary to check alignment validity. Violation of 'Advisory' rule does not present a risk.
  if ((pWriteBuffer == NULL) || (((Uint32_t)pWriteBuffer & GQSPI_DMA_DST_ADDR_MASK) != (Uint32_t)pWriteBuffer))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Inadequate readBuffer size */
  if (writeBufferSizeInWords < numWordsToWrite)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Initialisation check */
  if (qspioHandle.qspiMode == d_QSPI_UNINITIALISED)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Transfer in progress check */
  if (qspioHandle.isBusy == d_TRUE)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    status = d_STATUS_DEVICE_BUSY;
    d_ERROR_Logger(d_STATUS_DEVICE_BUSY, d_ERROR_CRITICALITY_NON_CRITICAL, 2, 0, 0, 0);
  }
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)  /* All checks passed - Initiate reading */
  {
    qspioHandle.isBusy = d_TRUE;                              /* This thread is busy */
    qspioHandle.qspiModStatus = d_QSPI_STATUS_PROGRAM_BUSY; /* Update status to busy programming */
    qspioHandle.pProgramBuffer = pWriteBuffer;              /* Pointer to start of data to program */
    qspioHandle.numWordsToProgram = numWordsToWrite;        /* Number of words to program */
    qspioHandle.txFifoProgramWords = 0u;                    /* Start with 0 - no words sent yet */
    qspioHandle.programmedWords = 0u;                       /* Start with 0 - no words programmed yet */
    qspioHandle.qspiAddress = qspiAddress;                  /* QSPI address programming start address */
    qspioHandle.pageIndex = 0u;                             /* Start with page index 0 */

    do
    {
      status = programPage();
      qspioHandle.pageIndex++;
    } while ((status == d_STATUS_SUCCESS) && (qspioHandle.txFifoProgramWords < qspioHandle.numWordsToProgram));

    if (status != d_STATUS_SUCCESS) /* Initial program page command failed */
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      qspioHandle.qspiModStatus = d_QSPI_STATUS_PROGRAM_FAIL; /* Set status to programming fail */
    }
    else
    {
      qspioHandle.qspiModStatus = d_QSPI_STATUS_PROGRAM_COMPLETE;
    }
    qspioHandle.isBusy = d_FALSE;   /* Program complete */
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return (status);
}

/*********************************************************************//**
  <!-- d_QSPI_EraseSubSector4K -->

  Erase 4KB sub-sector at QSPI flash memory address.
*************************************************************************/
d_Status_t                  /** \return QSPI erase sub-sector status */
d_QSPI_EraseSubSector4K
(
const Uint32_t qspiAddress  /**< [in] QSPI flash memory address in sub-sector to erase */
)
{
  d_Status_t status = d_STATUS_SUCCESS;  /* Return parameter: Status of sub-sector read initiation */

  /* Out of bounds address */
  if (qspiAddress  >= d_QSPI_FLASH_SIZE_IN_BYTES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Initialisation check */
  if (qspioHandle.qspiMode == d_QSPI_UNINITIALISED)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Transfer in progress check */
  if (qspioHandle.isBusy == d_TRUE)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    status = d_STATUS_DEVICE_BUSY;
    d_ERROR_Logger(d_STATUS_DEVICE_BUSY, d_ERROR_CRITICALITY_NON_CRITICAL, 2, 0, 0, 0);
  }
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)  /* All checks passed - Initiate reading */
  {
    qspioHandle.isBusy = d_TRUE;                            /* This thread is busy */
    qspioHandle.qspiModStatus = d_QSPI_STATUS_ERASE_BUSY; /* Update status to busy programming */

    status = writeEnable();

    if (status == d_STATUS_SUCCESS) /* Write enable successful */
    {
      const Uint32_t ERSE_4KB_COMMAND_4B = 0x00000021u; /* 4-byte erase 4KB sub-sector command */
      status = transferCommand(ERSE_4KB_COMMAND_4B, qspiAddress);
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    if (status == d_STATUS_SUCCESS) /* Erase command successful */
    {
      status = chipSelectDeassert();
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    if (status == d_STATUS_SUCCESS) /* Chip select de-assert successful */
    {
      Uint32_t startTime = d_TIMER_ReadValueInTicks();
      Uint32_t elapsedTime = 0u;
      do
      {
        status = readStatusComplete();
        elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
      } while ((status != d_STATUS_SUCCESS) && (elapsedTime < 400000u));
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    if (status != d_STATUS_SUCCESS) /* Something failed in the erase process */
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      qspioHandle.qspiModStatus = d_QSPI_STATUS_ERASE_FAIL; /* Set status to erase fail */
    }
    else
    {
      qspioHandle.qspiModStatus = d_QSPI_STATUS_ERASE_COMPLETE; /* Set status to erase complete */
    }

    qspioHandle.isBusy = d_FALSE;   /* Erase complete/failed, thread is not busy anymore */
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- d_QSPI_GetQspiModuleStatus -->

  Get the current QSPI driver module status.
*************************************************************************/
d_QSPI_ModuleStatus_t          /** \return QSPI module status */
d_QSPI_GetQspiModuleStatus
(
void
)
{
  return qspioHandle.qspiModStatus;
}

/*********************************************************************//**
  <!-- d_QSPI_SfdpByte -->

  Read a byte from the Serial Flash Discoverable Parameters
*************************************************************************/
d_Status_t                 /** \return Operation status */
d_QSPI_SfdpByte
(
const Uint32_t address,    /**< [in]  SFDP address */
Uint8_t * const pValue    /**< [out] pointer to storage for SFDP value */
)
{
  /* Initialisation check */
  if (qspioHandle.qspiMode == d_QSPI_UNINITIALISED)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Address check */
  if (address > 0xFFFFFFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t value = 0u;
  d_Status_t status = d_QSPI_SfdpWord(address, &value);
  
  if (status == d_STATUS_SUCCESS)
  {
    *pValue = (Uint8_t)value;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_QSPI_SfdpWord -->

  Read a word from the Serial Flash Discoverable Parameters
*************************************************************************/
d_Status_t                 /** \return Operation status */
d_QSPI_SfdpWord
(
const Uint32_t address,    /**< [in]  SFDP address */
Uint32_t * const pValue   /**< [out] pointer to storage for SFDP value */
)
{
  /* Initialisation check */
  if (qspioHandle.qspiMode == d_QSPI_UNINITIALISED)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  /* Address check */
  if (address > 0xFFFFFFu)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  chipSelectAssert();

  /* Setup and write command to TX FIFO */
  const Uint32_t READ_STATUS_COMMAND = 0x0000005Au |
                                       ((address & 0x0000FFu) << 24u) |
                                       ((address & 0x00FF00u) << 8u) |
                                       ((address & 0xFF0000u) >> 8u);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, READ_STATUS_COMMAND);  /* Write command */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, 0);

  /* Generic FIFO write command entry */
  const static Uint32_t GQSPI_GENFIFO_TX_READ_STATUS_COMMAND_LEN = 0x00000005u;
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_DATA_XFER
                        | GQSPI_GENFIFO_TX
                        | GQSPI_GENFIFO_TX_READ_STATUS_COMMAND_LEN;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  /* Start Generic FIFO */
  startGenericFifoCommandExecution();

  /* Wait for command to be transferred with TX timeout */
  d_Status_t status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

  if (status == d_STATUS_SUCCESS)
  {
    /* Read bytes */
    setToIOmode();

    /* Generic FIFO read command entry */
    const static Uint32_t GQSPI_GENFIFO_RX_READ_STATUS_COMMAND_LEN = 0x00000004u;
    genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                 | GQSPI_GENFIFO_BUS_LOWER
                 | GQSPI_GENFIFO_CS_LOWER
                 | GQSPI_GENFIFO_DATA_XFER
                 | GQSPI_GENFIFO_RX
                 | GQSPI_GENFIFO_RX_READ_STATUS_COMMAND_LEN;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* One Dummy entry for IO mode */
    genFifoEntry = 0x00000000u;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* Start Generic FIFO */
    startGenericFifoCommandExecution();

    /* Read status */
    const Uint32_t RX_COMMAND_TIME_OUT_MICROSECONDS = 100u;
    Uint32_t qspiStatusReg;
    Uint32_t elapsedTime;
    Uint32_t startTimeRx = d_TIMER_ReadValueInTicks();
    do
    {
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTimeRx, NULL);
      qspiStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_ISR_OFFSET);
    } while (((qspiStatusReg & GQSPI_ISR_RXEMPTY_MASK) != 0u) && (elapsedTime < RX_COMMAND_TIME_OUT_MICROSECONDS));

    if ((qspiStatusReg & GQSPI_ISR_RXEMPTY_MASK) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_TIMEOUT;
    }
    else
    {
      Uint32_t flashStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_RXD_OFFSET);
      *pValue = (Uint32_t)flashStatusReg;
    }

    (void)chipSelectDeassert(); /* Execute ChipSelectDeassert */

    /* Reset RX FIFO - just in case */
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_FIFO_CTRL_OFFSET, GQSPI_FIFO_CTRL_RST_RX_FIFO_MASK);

    setToDMAmode();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_QSPI_GetQspiModuleStatus -->

  Get the current QSPI driver module status
*************************************************************************/
static d_Status_t               /** \return status */
performRead
(
const Uint32_t qspiAddress,     /**< [in] QSPI flash memory start address to read */
const Uint32_t numWordsToRead,  /**< [in] Number of bytes to read from QSPI flash memory */
Uint32_t* const pReadBuffer     /**< [in] Read data will be stored at this pointer */
)
{
  d_Status_t status;

  Uint32_t numBytesToRead = numWordsToRead << d_QSPI_WORD_SHIFT;

  qspioHandle.isBusy = d_TRUE;  /* This thread is busy */

  /* Populate the READ command */
  const Uint32_t READ_COMMAND_4B = 0x0000006Cu; /* 4-byte Quadrature output fast read */
  status = transferCommand(READ_COMMAND_4B, qspiAddress);

  if (status == d_STATUS_SUCCESS)  /* Command transferred successful - transfer dummy clocks */
  {
    status = transferDummyClocks();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)  /* Command and dummy clocks transferred successful - Initiate Read */
  {
    /* Write address information */
    qspioHandle.pRxBuffer = pReadBuffer;
    // cppcheck-suppress misra-c2012-11.4; Conversion necessary for register write function. Violation of 'Advisory' rule does not present a risk.
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_ADDR_OFFSET, (Uint32_t)pReadBuffer);
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_ADDR_MSB_OFFSET, 0u);

    /* Write number of bytes to DMA DST SIZE */
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_SIZE_OFFSET, (numWordsToRead << d_QSPI_WORD_SHIFT));

    /* Generic FIFO general read setup (for exponent) without the data size */
    Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_QUADSPI
                          | GQSPI_GENFIFO_BUS_LOWER
                          | GQSPI_GENFIFO_CS_LOWER
                          | GQSPI_GENFIFO_DATA_XFER
                          | GQSPI_GENFIFO_RX
                          | GQSPI_GENFIFO_EXP;

    /* Populate one or more generic FIFO entries for the read */
    populateGenFifoEntriesWithDataLength(numWordsToRead, &genFifoEntry);

    /* Cache invalidate range */
    // cppcheck-suppress misra-c2012-11.4; Conversion necessary for memory cache function. Violation of 'Advisory' rule does not present a risk.
    d_MEMORY_DCacheInvalidateRange((Pointer_t)pReadBuffer, numBytesToRead);

    /* Start the read */
    qspioHandle.qspiModStatus = d_QSPI_STATUS_READ_BUSY;
    startGenericFifoCommandExecution();

    /* Wait for completion */
    Uint32_t qspiDmaInterruptStatus = 0u;
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    Uint32_t elapsedTime = 0;
    const Uint32_t MAX_READ_TIME = 1000000;
    do
    {
      qspiDmaInterruptStatus = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_DMA_DST_I_STS_OFFSET); /* Read DMA interrupt status...*/
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while (((qspiDmaInterruptStatus & GQSPI_DMA_DST_I_STS_DONE_MASK) != GQSPI_DMA_DST_I_STS_DONE_MASK) && (elapsedTime < MAX_READ_TIME));
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_I_STS_OFFSET, qspiDmaInterruptStatus);          /* ...and write to clear */

    status = chipSelectDeassert();

    if ((status != d_STATUS_SUCCESS) || ((qspiDmaInterruptStatus & GQSPI_DMA_DST_INTR_ERR_MASK) != 0u)) /* Something went wrong */
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      qspioHandle.qspiModStatus = d_QSPI_STATUS_ERROR;
    }
    else
    {
      qspioHandle.qspiModStatus = d_QSPI_STATUS_READ_COMPLETE;
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    qspioHandle.qspiModStatus = d_QSPI_STATUS_READ_FAIL; /* Set module status to read fail */
  }

  qspioHandle.isBusy = d_FALSE;  /* Read initiation fail, thread is not busy anymore */

  return status;
}

/*********************************************************************//**
  <!-- readStatusComplete -->

  Check if a write/erase completed successfully
*************************************************************************/
static d_Status_t             /** \return QSPI Flash write/erase complete status */
readStatusComplete
(
void
)
{
  chipSelectAssert();

  /* Setup and write command to TX FIFO */
  const Uint8_t READ_STATUS_COMMAND = 0x00000005u;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, READ_STATUS_COMMAND);  /* Write command: one word only*/

  /* Generic FIFO write command entry */
  const static Uint32_t GQSPI_GENFIFO_TX_READ_STATUS_COMMAND_LEN = 0x00000001u;
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_DATA_XFER
                        | GQSPI_GENFIFO_TX
                        | GQSPI_GENFIFO_TX_READ_STATUS_COMMAND_LEN;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  /* Start Generic FIFO */
  startGenericFifoCommandExecution();

  /* Wait for command to be transferred with TX timeout */
  d_Status_t status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

  if (status == d_STATUS_SUCCESS)
  {
    /* Read bytes */
    setToIOmode();

    /* Generic FIFO read command entry */
    const static Uint32_t GQSPI_GENFIFO_RX_READ_STATUS_COMMAND_LEN = 0x00000001u;
    genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                 | GQSPI_GENFIFO_BUS_LOWER
                 | GQSPI_GENFIFO_CS_LOWER
                 | GQSPI_GENFIFO_DATA_XFER
                 | GQSPI_GENFIFO_RX
                 | GQSPI_GENFIFO_RX_READ_STATUS_COMMAND_LEN;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* One Dummy entry for IO mode */
    genFifoEntry = 0x00000000u;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* Start Generic FIFO */
    startGenericFifoCommandExecution();

    /* Read status */
    const Uint32_t RX_COMMAND_TIME_OUT_MICROSECONDS = 100u;
    Uint32_t qspiStatusReg;
    Uint32_t elapsedTime;
    Uint32_t startTimeRx = d_TIMER_ReadValueInTicks();
    do
    {
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTimeRx, NULL);
      qspiStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_ISR_OFFSET);
    } while (((qspiStatusReg & GQSPI_ISR_RXEMPTY_MASK) != 0u) && (elapsedTime < RX_COMMAND_TIME_OUT_MICROSECONDS));

    if ((qspiStatusReg & GQSPI_ISR_RXEMPTY_MASK) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_TIMEOUT;
    }
    else
    {
      Uint32_t flashStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_RXD_OFFSET);

      if ((flashStatusReg & 0x00000001u) > 0u) /* Check for busy */
      {
        status = d_STATUS_FAILURE;
      }
      else
      {
        status = d_STATUS_SUCCESS;
      }
    }

    (void)chipSelectDeassert(); /* Execute ChipSelectDeassert */

    /* Reset RX FIFO - just in case */
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_FIFO_CTRL_OFFSET, GQSPI_FIFO_CTRL_RST_RX_FIFO_MASK);

    setToDMAmode();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- setToDMAmode -->

  Set to DMA mode
*************************************************************************/
static void            /** \return None */
setToDMAmode
(
void
)
{
  qspioHandle.qspiMode = d_QSPI_DMA_MODE;
  Uint32_t genericConfigReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_CFG_OFFSET);
  genericConfigReg |= GQSPI_CFG_MODE_EN_DMA_MASK;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_CFG_OFFSET, genericConfigReg);

  return;
}

/*********************************************************************//**
  <!-- setToIOmode -->

  Set to IO mode
*************************************************************************/
static void            /** \return None */
setToIOmode
(
void
)
{
  qspioHandle.qspiMode = d_QSPI_IO_MODE;
  Uint32_t genericConfigReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_CFG_OFFSET);
  genericConfigReg &= ~GQSPI_CFG_MODE_EN_MASK;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_CFG_OFFSET, genericConfigReg);

  return;
}

/*********************************************************************//**
  <!-- startGenericFifoCommandExecution -->

  Start (Trigger) generic FIFO command execution
*************************************************************************/
static void            /** \return None */
startGenericFifoCommandExecution
(
void
)
{
  Uint32_t genericConfigReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_CFG_OFFSET);
  genericConfigReg |= GQSPI_CFG_START_GEN_FIFO_MASK;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_CFG_OFFSET, genericConfigReg);

  return;
}

/*********************************************************************//**
  <!-- waitTransferCompleteWithTimeout -->

  Check if a transmit completes within the timeout limit
*************************************************************************/
static d_Status_t                   /** \return QSPI Flash transmit complete status */
waitTransferCompleteWithTimeout
(
const Uint32_t timeoutMicroseconds  /**< [in] Timeout for waiting */
)
{
  Uint32_t qspiStatusReg;
  Uint32_t elapsedTime;
  Uint32_t startTimeTx = d_TIMER_ReadValueInTicks();
  do
  {
    elapsedTime = d_TIMER_ElapsedMicroseconds(startTimeTx, NULL);
    qspiStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_ISR_OFFSET);
  } while (((qspiStatusReg & GQSPI_ISR_GENFIFOEMPTY_MASK) == 0u) && ((qspiStatusReg & GQSPI_ISR_TXEMPTY_MASK) == 0u) && (elapsedTime < timeoutMicroseconds));

  d_Status_t status;
  if (((qspiStatusReg & GQSPI_ISR_GENFIFOEMPTY_MASK) == 0u) && ((qspiStatusReg & GQSPI_ISR_TXEMPTY_MASK) == 0u))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_TIMEOUT;
  }
  else
  {
    status = d_STATUS_SUCCESS;
  }

  return status;
}

/*********************************************************************//**
  <!-- chipSelectAssert -->

  Assert the QSPI flash chip select
*************************************************************************/
static void               /** \return None */
chipSelectAssert
(
void
)
{
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_CS_SETUP_TIME;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  return;
}

/*********************************************************************//**
  <!-- chipSelectDeassert -->

  De-assert the QSPI flash chip select
*************************************************************************/
static d_Status_t         /** \return ChipSelect DeAssert execution status */
chipSelectDeassert
(
void
)
{
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_HOLD_TIME;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  startGenericFifoCommandExecution();

  /* Wait for execution to complete with TX timeout */
  d_Status_t status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

  return status;
}

/*********************************************************************//**
  <!-- writeEnable -->

  Execute write enable sequence to be able to edit QSPI flash memory
*************************************************************************/
static d_Status_t            /** \return QSPI flash write enable command transfer status */
writeEnable
(
void
)
{
  chipSelectAssert();  /* Start with generic FIFO entry: ChipSelectAssert */

  /* Setup and write command to TX FIFO */
  const Uint32_t WRITE_ENABLE_COMMAND = 0x00000006u;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, WRITE_ENABLE_COMMAND);  /* Write command: one word only*/

  /* Generic FIFO write command entry */
  const static Uint32_t GQSPI_GENFIFO_WRITE_ENABLE_COMMAND_LEN = 0x00000001u;
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_DATA_XFER
                        | GQSPI_GENFIFO_TX
                        | GQSPI_GENFIFO_WRITE_ENABLE_COMMAND_LEN;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  /* Start Generic FIFO */
  startGenericFifoCommandExecution();

  /* Wait for command to be transferred with TX timeout */
  d_Status_t status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

  if (status == d_STATUS_SUCCESS)
  {
    status = chipSelectDeassert(); /* Execute ChipSelectDeassert */
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- transferDummyClocks -->

  Transfer dummy clocks to/from QSPI flash device
*************************************************************************/
static d_Status_t           /** \return QSPI flash command transfer status */
transferDummyClocks
(
void
)
{
  /* GenFifo write command */
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_QUADSPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_DATA_XFER
                        | GQSPI_GENFIFO_DUMMY_CLOCKS_LEN;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  startGenericFifoCommandExecution();

  /* Poll TxFifo with 5ms timeout - to determine if command transfer is complete */
  const Uint32_t DUMMY_TIME_OUT_MILLISECONDS = 5u;
  d_Status_t status = waitTransferCompleteWithTimeout(DUMMY_TIME_OUT_MILLISECONDS);

  return status;
}

/*********************************************************************//**
  <!-- transferCommand -->

  Transfer a command to QSPI flash device
*************************************************************************/
static d_Status_t           /** \return QSPI flash command transfer status */
transferCommand
(
const Uint32_t command,     /**< [in] Command to transfer */
const Uint32_t qspiAddress  /**< [in] Address to be used with command */
)
{
  chipSelectAssert(); /* Start with generic FIFO entry: ChipSelectAssert */

  Uint32_t commandAddress[2u];
  commandAddress[0u] = command
                     | ((qspiAddress & 0xFF000000u) >> 16u)
                     | (qspiAddress & 0x00FF0000u)
                     | ((qspiAddress & 0x0000FF00u) << 16u);
  commandAddress[1u] = (qspiAddress & 0x000000FFu);

  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, commandAddress[0]);  /* Write command: first word */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, commandAddress[1]);  /* Write command: second word */

  /* Generic FIFO command entry */
  const static Uint32_t GQSPI_GENFIFO_COMMAND_ADDRESS_LEN = 0x00000005u;
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_DATA_XFER
                        | GQSPI_GENFIFO_TX
                        | GQSPI_GENFIFO_COMMAND_ADDRESS_LEN;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  startGenericFifoCommandExecution();

  /* Wait for command to be transferred with TX timeout */
  d_Status_t status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

  return status;
}

/*********************************************************************//**
  <!-- populateGenFifoEntriesWithDataLength -->

  Generic FIFO entries populated with data length and written to
  generic FIFO register
*************************************************************************/
static void               /** \return None */
populateGenFifoEntriesWithDataLength
(
Uint32_t numWords,        /**< [in] Number of words to transfer */
Uint32_t* pGenFifoEntry   /**< [in] Read data will be stored at this pointer */
)
{
  Uint32_t numBytes = numWords << d_QSPI_WORD_SHIFT;

  /* GenFifo exponent data read */
  const Uint32_t EXPONENT_START = 0x00000008u; /* Exponent start for transfers of length larger than immediate data size */
  Uint32_t exponent = EXPONENT_START;
  Uint32_t exponentBytes = numBytes >> EXPONENT_START;

  while (exponentBytes != 0u)
  {
    if ((exponentBytes & GQSPI_GENFIFO_EXP_MASK) == GQSPI_GENFIFO_EXP_MASK)
    {
      *pGenFifoEntry &= (~GQSPI_GENFIFO_IMM_DATA_MASK); /* Clear data bits */
      *pGenFifoEntry |= exponent;                       /* Populate data bits with exponent */
      d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, *pGenFifoEntry);
    }
    else
    {
      DO_NOTHING();
    }
    exponentBytes = exponentBytes >> 1u;
    exponent++;
  }

  /* GenFifo immediate data read */
  Uint32_t immediateData = numBytes & GQSPI_GENFIFO_IMM_DATA_MASK;
  if (immediateData > 0u)
  {
    *pGenFifoEntry &= (~GQSPI_GENFIFO_EXP);           /* Setup for immediate data (not exponent) */
    *pGenFifoEntry &= (~GQSPI_GENFIFO_IMM_DATA_MASK); /* Clear data bits */
    *pGenFifoEntry |= immediateData;                  /* Populate data bits with immediate number of data bytes */
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, *pGenFifoEntry);
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- programPage -->

  Program QSPI flash memory page
*************************************************************************/
static d_Status_t     /** \return QSPI flash program page initiation status */
programPage
(
void
)
{
  d_Status_t status = writeEnable();

  if (status == d_STATUS_SUCCESS)  /* Write enable successful */
  {
    /* Populate the program page command */
    const Uint32_t QSPI_PAGE_SIZE_BYTES = 256u;           /* 256 Bytes per Page */
    Uint32_t qspiPageAddress = qspioHandle.qspiAddress + (qspioHandle.pageIndex * QSPI_PAGE_SIZE_BYTES);
    const Uint32_t PROGRAM_PAGE_COMMAND_4B = 0x00000012u; /* 4-byte program page command */
    status = transferCommand(PROGRAM_PAGE_COMMAND_4B, qspiPageAddress);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)  /* Program page command successful */
  {
    Uint32_t wordCount = 0u;
    static const Uint32_t QSPI_TX_FIFO_DEPTH = 64u;

    /* Write words to program to TX buffer */
    while ((wordCount < QSPI_TX_FIFO_DEPTH) && (qspioHandle.txFifoProgramWords < qspioHandle.numWordsToProgram))
    {
      d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, qspioHandle.pProgramBuffer[qspioHandle.txFifoProgramWords]);
      qspioHandle.txFifoProgramWords++;
      wordCount++;
    }

    /* GenFifo general write setup (for exponent) without the data size */
    Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                          | GQSPI_GENFIFO_BUS_LOWER
                          | GQSPI_GENFIFO_CS_LOWER
                          | GQSPI_GENFIFO_DATA_XFER
                          | GQSPI_GENFIFO_TX
                          | GQSPI_GENFIFO_EXP;

    /* Populate one or more generic FIFO entries for the write */
    populateGenFifoEntriesWithDataLength(wordCount, &genFifoEntry);

    /* Start the generic FIFO sequence to program a page */
    startGenericFifoCommandExecution();

    /* Wait for execution to complete with TX timeout */
    status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

    (void)chipSelectDeassert();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  /* Wait for completion of page programming */
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    Uint32_t elapsedTime = 0u;
    do
    {
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
      status = readStatusComplete(); /* Check success of page program */
    } while ((status != d_STATUS_SUCCESS) && (elapsedTime < 20000u));
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- setDefaultConfig -->

  Set the QSPI flash module to default configuration
*************************************************************************/
static void   /** \return None */
setDefaultConfig
(
void
)
{
  Uint32_t configReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_CFG_OFFSET);
  configReg &= ~GQSPI_CFG_MODE_EN_MASK;             /* Clear default mode */
  configReg |= GQSPI_CFG_MODE_EN_DMA_MASK;          /* DMA mode */
  configReg |= GQSPI_CFG_GEN_FIFO_START_MODE_MASK;  /* Manual start */
  configReg &= ~GQSPI_CFG_ENDIAN_MASK;              /* Little Endian */
  configReg &= ~GQSPI_CFG_EN_POLL_TO_MASK;          /* Disable poll timeout */
  configReg |= GQSPI_CFG_WP_HOLD_MASK;              /* Set hold bit */
  configReg &= ~GQSPI_CFG_BAUD_RATE_DIV_MASK;       /* Clear prescalar */
  configReg |= GQSPI_CFG_BAUD_RATE_DIV_2;           /* Set prescalar divide by 2 */
  configReg &= ~GQSPI_CFG_CLK_PHA_MASK;             /* CPOL 0 */
  configReg &= ~GQSPI_CFG_CLK_POL_MASK;             /* CPHA 0 */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_CFG_OFFSET, configReg);

  /* Set by default to allow for high frequencies */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_LPBK_DLY_ADJ_OFFSET, d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_LPBK_DLY_ADJ_OFFSET) | GQSPI_LPBK_DLY_ADJ_USE_LPBK_MASK);

  /* Reset thresholds */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TX_THRESHOLD_OFFSET, GQSPI_TX_FIFO_THRESHOLD_RESET_VAL);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_RX_THRESHOLD_OFFSET, GQSPI_RX_FIFO_THRESHOLD_RESET_VAL);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GF_THRESHOLD_OFFSET, GQSPI_GEN_FIFO_THRESHOLD_RESET_VAL);

  /* DMA initialisation */
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_CTRL_OFFSET, GQSPI_DMA_DST_CTRL_RESET_VAL);

  return;
}

/*********************************************************************//**
  <!-- qspiAbort -->

  Abort any QSPI flash module operations
*************************************************************************/
static void   /** \return None */
qspiAbort
(
void
)
{
  /* Clear and disable interrupts */
  Uint32_t interruptStatus = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_ISR_OFFSET);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_ISR_OFFSET, interruptStatus | GQSPI_ISR_WR_TO_CLR_MASK);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_I_STS_OFFSET, d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_DMA_DST_I_STS_OFFSET));
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_STS_OFFSET, d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_DMA_DST_STS_OFFSET) | GQSPI_DMA_DST_STS_WTC);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_IDR_OFFSET, GQSPI_IDR_ALL_MASK);
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_DMA_DST_I_DIS_OFFSET, GQSPI_DMA_DST_INTR_ALL_MASK);

  /* Clear FIFO */
  if ((d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_ISR_OFFSET) & GQSPI_ISR_RXEMPTY_MASK) != 0u)
  {
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_FIFO_CTRL_OFFSET, GQSPI_FIFO_CTRL_RST_TX_FIFO_MASK | GQSPI_FIFO_CTRL_RST_GEN_FIFO_MASK);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  /* Clear RX FIFO */
  if ((interruptStatus & GQSPI_ISR_RXEMPTY_MASK) != 0u)
  {
    /* Switch to IO mode */
    Uint32_t configReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_CFG_OFFSET);
    configReg &= ~GQSPI_CFG_MODE_EN_MASK;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_CFG_OFFSET, configReg);
    /* Reset RX FIFO */
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_FIFO_CTRL_OFFSET, GQSPI_FIFO_CTRL_RST_RX_FIFO_MASK);
    /* Switch to DMA mode */
    configReg |= GQSPI_CFG_MODE_EN_DMA_MASK;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_CFG_OFFSET, configReg);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- qspiReset -->

  Reset the QSPI flash module - Set to a known state
*************************************************************************/
static void   /** \return None */
qspiReset
(
void
)
{
  qspiAbort();  /* Abort any transfer that is in progress */
  setDefaultConfig();  /* Set default configuration */

  return;
}

/*********************************************************************//**
  <!-- readStatusRegister -->

  Read the status register
*************************************************************************/
static d_Status_t        /** \return QSPI Flash write/erase complete status */
readStatusRegister
(
Uint8_t * const pValue
)
{
  chipSelectAssert();

  /* Setup and write command to TX FIFO */
  const Uint8_t READ_STATUS_COMMAND = 0x00000005u;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, READ_STATUS_COMMAND);  /* Write command: one word only*/

  /* Generic FIFO write command entry */
  const static Uint32_t GQSPI_GENFIFO_TX_READ_STATUS_COMMAND_LEN = 0x00000001u;
  Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                        | GQSPI_GENFIFO_BUS_LOWER
                        | GQSPI_GENFIFO_CS_LOWER
                        | GQSPI_GENFIFO_DATA_XFER
                        | GQSPI_GENFIFO_TX
                        | GQSPI_GENFIFO_TX_READ_STATUS_COMMAND_LEN;
  d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

  /* Start Generic FIFO */
  startGenericFifoCommandExecution();

  /* Wait for command to be transferred with TX timeout */
  d_Status_t status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);

  if (status == d_STATUS_SUCCESS)
  {
    /* Read bytes */
    setToIOmode();

    /* Generic FIFO read command entry */
    const static Uint32_t GQSPI_GENFIFO_RX_READ_STATUS_COMMAND_LEN = 0x00000001u;
    genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                 | GQSPI_GENFIFO_BUS_LOWER
                 | GQSPI_GENFIFO_CS_LOWER
                 | GQSPI_GENFIFO_DATA_XFER
                 | GQSPI_GENFIFO_RX
                 | GQSPI_GENFIFO_RX_READ_STATUS_COMMAND_LEN;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* One Dummy entry for IO mode */
    genFifoEntry = 0x00000000u;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* Start Generic FIFO */
    startGenericFifoCommandExecution();

    /* Read status */
    const Uint32_t RX_COMMAND_TIME_OUT_MICROSECONDS = 100u;
    Uint32_t qspiStatusReg;
    Uint32_t elapsedTime;
    Uint32_t startTimeRx = d_TIMER_ReadValueInTicks();
    do
    {
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTimeRx, NULL);
      qspiStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_ISR_OFFSET);
    } while (((qspiStatusReg & GQSPI_ISR_RXEMPTY_MASK) != 0u) && (elapsedTime < RX_COMMAND_TIME_OUT_MICROSECONDS));

    if ((qspiStatusReg & GQSPI_ISR_RXEMPTY_MASK) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_TIMEOUT;
    }
    else
    {
      Uint32_t flashStatusReg = d_GEN_RegisterRead(QSPI_BASEADDR + GQSPI_RXD_OFFSET);
      *pValue = (Uint8_t)flashStatusReg;
    }

    (void)chipSelectDeassert(); /* Execute ChipSelectDeassert */

    /* Reset RX FIFO - just in case */
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_FIFO_CTRL_OFFSET, GQSPI_FIFO_CTRL_RST_RX_FIFO_MASK);

    setToDMAmode();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- writeStatusRegister -->

  Write the status register
*************************************************************************/
// gcov-jst 53 It is not practical to get this coverage. Requires a new device which has never been initialised.
static d_Status_t    /** \return Write status register status */
writeStatusRegister
(
const Uint8_t value  /**< [in] Value to write to status register */
)
{
  d_Status_t status = writeEnable();

  if (status == d_STATUS_SUCCESS)
  {
    chipSelectAssert();  /* Start with generic FIFO entry: ChipSelectAssert */

    /* Setup and write command to TX FIFO */
    Uint32_t WRITE_STATUS_COMMAND = 0x00000001u | ((Uint32_t)value << 8u);
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_TXD_OFFSET, WRITE_STATUS_COMMAND);  /* Write command: one word only*/

    /* Generic FIFO write command entry */
    const static Uint32_t GQSPI_GENFIFO_WRITE_STATUS_COMMAND_LEN = 0x00000002u;
    Uint32_t genFifoEntry = GQSPI_GENFIFO_MODE_SPI
                          | GQSPI_GENFIFO_BUS_LOWER
                          | GQSPI_GENFIFO_CS_LOWER
                          | GQSPI_GENFIFO_DATA_XFER
                          | GQSPI_GENFIFO_TX
                          | GQSPI_GENFIFO_WRITE_STATUS_COMMAND_LEN;
    d_GEN_RegisterWrite(QSPI_BASEADDR + GQSPI_GENFIFO_OFFSET, genFifoEntry);

    /* Start Generic FIFO */
    startGenericFifoCommandExecution();

    /* Wait for command to be transferred with TX timeout */
    status = waitTransferCompleteWithTimeout(TX_COMMAND_TIME_OUT_MICROSECONDS);
  }
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    status = chipSelectDeassert(); /* Execute ChipSelectDeassert */
  }
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS) /* Chip select de-assert successful */
  {
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    Uint32_t elapsedTime = 0u;
    do
    {
      status = readStatusComplete();
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status != d_STATUS_SUCCESS) && (elapsedTime < 400000u));
  }
  ELSE_DO_NOTHING

  return status;
}

