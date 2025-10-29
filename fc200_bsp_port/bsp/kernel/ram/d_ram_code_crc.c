/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM Code CRC

  Abstract           : Memory test functionality

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-237
                       SDD References: 136T-2200-131100-001-D22 SWDES-656
                                                                SWDES-657
                                                                SWDES-661

\note
  CSC ID             : SWDES-74
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_asm.h"
#include "soc/defines/d_common_status.h"
#include "kernel/crc32/d_crc32.h"
#include "kernel/error_handler/d_error_handler.h"

#include "d_ram_usr.h"
#include "d_ram_cfg.h"
#include "d_ram.h"

/***** Constants ********************************************************/

/* Linker defined variables */
// clang-tidy-jst bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp 2 Externally defined linker generated variables. Does not present a risk.
extern Uint32_t _start;
extern Uint32_t __rodata1_end;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static Uint32_t iterations_per_invocation;

static d_RAM_CodeCrcState_t crcState = d_RAM_CRC_BUSY;

static Uint8_t * pStart;
static Uint8_t * pEnd;
static Uint8_t * pCurrent;
static Uint32_t expectedCrc;
static Uint32_t currentCrc;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_RAM_CodeCrcInitialise -->

  Initialise the code CRC check process.
*************************************************************************/
void                                        /** \return None */
d_RAM_CodeCrcInitialise
(
void
)
{
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  pStart = (Uint8_t *)((Uint32_t)&_start & 0xFFFFF000u);
  pEnd = (Uint8_t *)&__rodata1_end;

  // cppcheck-suppress misra-c2012-18.4;  Calculation to determine distance between pointers. Violation of 'Advisory' rule does not present a risk.
  Uint32_t codeSize = pEnd - pStart;

  expectedCrc = d_CRC32_Calculate(pStart, codeSize);

  pCurrent = pStart;
  currentCrc = 0xFFFFFFFFu;

  iterations_per_invocation = ((codeSize / sizeof(Uint32_t)) * d_RAM_CyclicExecutionTime) / d_RAM_CodeCheckTime;

  initialised = d_TRUE;
  
  return;
}

/*********************************************************************//**
  <!-- d_RAM_CodeCrc -->

  Return the CRC.
*************************************************************************/
d_Status_t                                  /** \return result of the memory test */
d_RAM_CodeCrc
(
Uint32_t * const pCrc                       /**< [out] Pointer to storage for CRC */
)
{
  /* Ensure the CSC has been initialised */
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  /* Parameter check */
  if (pCrc == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  *pCrc = expectedCrc;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_RAM_CodeCrcCyclic -->

  Cyclic code CRC processing.
*************************************************************************/
void                                        /** \return None */
d_RAM_CodeCrcCyclic
(
void
)
{
  Uint32_t iteration = 0;

  /* Ensure the CSC has been initialised */
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  
  while ((iteration < iterations_per_invocation) &&
         (pCurrent < pEnd))
  {
    d_CRC32_Add(&currentCrc, pCurrent, sizeof (Uint32_t));

    // cppcheck-suppress misra-c2012-18.4;  Operation to move pointer by one word. Violation of 'Advisory' rule does not present a risk.
    pCurrent = pCurrent + sizeof(Uint32_t);
    iteration++;
  }

  if (pCurrent >= pEnd)
  {
    if (currentCrc != ~expectedCrc)
    {
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
      crcState = d_RAM_CRC_FAIL;
    }
    else if (crcState != d_RAM_CRC_FAIL)
    {
      crcState = d_RAM_CRC_PASS;
    }
    else
    {
      /* Failed state remains */
    }
    pCurrent = pStart;
    currentCrc = 0xFFFFFFFFu;
  }

  return;
}

/*********************************************************************//**
  <!-- d_RAM_CodeCrcStatus -->

  Get status of cyclic code CRC processing.
*************************************************************************/
d_Status_t                                  /** \return Function status */
d_RAM_CodeCrcStatus
(
d_RAM_CodeCrcState_t * const pCodeCrcState  /**< [out] Pointer to storage status */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  /* Ensure the CSC has been initialised */
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (pCodeCrcState == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  *pCodeCrcState = crcState;

  return status;
}
