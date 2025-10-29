/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM BIT

  Abstract           : Memory test functionality

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-151
                                                                SWREQ-152
                                                                SWREQ-236
                       SDD References: 136T-2200-131100-001-D22 SWDES-555
                                                                SWDES-658
                                                                SWDES-659
                                                                SWDES-660

\note
  CSC ID             : SWDES-74
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"

#include "d_ram_usr.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static d_Status_t TestPowerOfTwosAddressForPattern(volatile Uint32_t * const pBaseAddress,
                                                   const Uint32_t pattern,
                                                   const Uint32_t byteCount);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_RAM_DataBusTest -->

  Perform Data bus test using the given address. This test is destructive to the data at this address

*************************************************************************/
d_Status_t                                  /** \return result of the memory test */
d_RAM_DataBusTest
(
volatile Uint32_t * const pAddress          /**< [in] Address where the test should be performed */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  /* No check for pAddress == NULL as null might be a valid address to run the test */
  
  /* Test will perform a walking 1's test at the given address to make sure the data bus is intact */
  Uint32_t pattern = 1u;

  while ((pattern != 0u) && (returnValue == d_STATUS_SUCCESS))
  {
    *pAddress = pattern;
    // cppcheck-suppress knownConditionTrueFalse; The whole point of the test is to check if the memory write was successfull so we suppress this issue
    if (*pAddress != pattern)
    {
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, pattern, 0, 0, 0);
      returnValue = d_STATUS_BIT_FAILURE;
    }
    else
    {
      pattern <<= 1u;
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_RAM_AddressBusTest -->

  Perform Address bus test using the given base address, This test is destructive to the data at this address

*************************************************************************/
d_Status_t                                  /** \return result of the memory test */
d_RAM_AddressBusTest
(
volatile Uint32_t * const pBaseAddress,     /**< [in] Address where the test should be performed */
const Uint32_t byteCount                    /**< [in] size of the region to test, suggest using a size as a power of two */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  /* No check for pAddress == NULL as null might be a valid address to run the test */

  /* Test the address bus wiring in a memory region by performing a walking 1's test on the relevant bits of the address
     and checking for aliasing. This test will find single-bit address failures such as stuck high, stuck-low, and shorted pins.*/
  Uint32_t addressMask = (byteCount/sizeof(Uint32_t) - 1u);
  Uint32_t offset;
  Uint32_t testOffset;

  Uint32_t pattern     =  0xAAAAAAAAu;
  Uint32_t antipattern =  0x55555555u;

  /* Write the default pattern at each of the power-of-two offsets */
  for (offset = 1u; (offset & addressMask) != 0u; offset <<= 1u)
  {
    pBaseAddress[offset] = pattern;
  }

  /* Check for address bits stuck high */
  testOffset = 0;
  pBaseAddress[testOffset] = antipattern;

  returnValue = TestPowerOfTwosAddressForPattern(pBaseAddress, pattern, byteCount);
  if (returnValue == d_STATUS_SUCCESS)
  {
    pBaseAddress[testOffset] = pattern;

    /* Check for address bits stuck low or shorted */
    testOffset = 1u;
    while (((testOffset & addressMask) != 0u) && (returnValue == d_STATUS_SUCCESS))
    {
      pBaseAddress[testOffset] = antipattern;
      if (pBaseAddress[0] != pattern)
      {
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, pBaseAddress[testOffset], 0, 0, 0);
        returnValue = d_STATUS_BIT_FAILURE;
      }
      else
      {
        DO_NOTHING();
      }

      offset = 1u;
      while (((offset & addressMask) != 0u) && (returnValue == d_STATUS_SUCCESS))
      {
        if ((pBaseAddress[offset] != pattern) && (offset != testOffset))
        {
          // gcov-jst 2 It is not practical to generate this failure during bench testing.
          d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, pBaseAddress[testOffset], 0, 0, 0);
          returnValue = d_STATUS_BIT_FAILURE;
        }
        else
        {
          DO_NOTHING();
        }

        offset <<= 1u;
      }

      pBaseAddress[testOffset] = pattern;
      testOffset <<= 1u;
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_RAM_DeviceIntegrityTest -->

  Perform physical memory integrity test

*************************************************************************/
d_Status_t                                  /** \return result of the memory test */
d_RAM_DeviceIntegrityTest
(
volatile Uint32_t * const pBaseAddress,     /**< [in] Address where the test should be performed */
const Uint32_t byteCount                    /**< [in] size of the region to test, suggest using a size as a power of two */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  /* No check for pAddress == NULL as null might be a valid address to run the test */

  Uint32_t offset;
  Uint32_t nWords = byteCount / sizeof(Uint32_t);

  Uint32_t pattern;
  Uint32_t antipattern;

  /* Fill memory with a known pattern */
  pattern = 1u;
  for (offset = 0u; offset < nWords; offset++)
  {
    pBaseAddress[offset] = pattern;
    pattern++;
  }

  /* Check each location and invert it for the second pass */
  pattern = 1u;
  offset = 0u;
  while ((offset < nWords) && (returnValue == d_STATUS_SUCCESS))
  {
    if (pBaseAddress[offset] != pattern)
    {
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, pBaseAddress[offset], 0, 0, 0);
      returnValue = d_STATUS_BIT_FAILURE;
    }
    else
    {
      DO_NOTHING();
    }

    antipattern = ~pattern;
    pBaseAddress[offset] = antipattern;

    pattern++;
    offset++;;
  }

  /* Check each location for the inverted pattern and zero it */
  pattern = 1u;
  offset = 0u;
  while ((offset < nWords) && (returnValue == d_STATUS_SUCCESS))
  {
    antipattern = ~pattern;
    if (pBaseAddress[offset] != antipattern)
    {
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, pBaseAddress[offset], 0, 0, 0);
      returnValue = d_STATUS_BIT_FAILURE;
    }
    else
    {
      DO_NOTHING();
    }

    pattern++;
    offset++;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- TestPowerOfTwosAddressForPattern -->

  Test power-of-twos addresses against a given pattern

*************************************************************************/
static d_Status_t                           /** \return result of the memory test */
TestPowerOfTwosAddressForPattern
(volatile Uint32_t * const pBaseAddress,    /**< [in] Address where the test should be performed */
const Uint32_t pattern,                     /**< [in] Pattern to test against */
const Uint32_t byteCount)                   /**< [in] size of the region to test */
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  Uint32_t addressMask = (byteCount/sizeof(Uint32_t) - 1u);

  for (Uint32_t offset = 1u; (offset & addressMask) != 0u; offset <<= 1u)
  {
    if (pBaseAddress[offset] != pattern)
    {
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, pBaseAddress[offset], 0, 0, 0);
      returnValue = d_STATUS_BIT_FAILURE;
    }
    else
    {
      DO_NOTHING();
    }
  }

  return returnValue;
}

