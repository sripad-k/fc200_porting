/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : BIT

  Abstract           : ALU Built In Test

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-249
                       SDD References: 136T-2200-131000-001-D22 SWDES-276
                                                                SWDES-277
                                                                SWDES-278
                                                                SWDES-279
                                                                SWDES-280
                                                                SWDES-281
                                                                SWDES-281
                                                                SWDES-283
                                                                SWDES-284
                                                                SWDES-285
                                                                SWDES-286
                                                                SWDES-287
                                                                SWDES-288
                                                                SWDES-289
                                                                SWDES-290
                                                                SWDES-291
\note
  CSC ID             : SWDES-50
*************************************************************************/

// NOLINTBEGIN(hicpp-no-assembler)

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/defines/d_common_asm.h"

#include "soc/bit/d_bit.h"

/***** Type Definitions *************************************************/

/* Functions to be executed for the tests */
enum
{
  FUNCTION_ADD = 0,
  FUNCTION_SUBTRACT,
  FUNCTION_OR,
  FUNCTION_AND,
  FUNCTION_XOR,
  FUNCTION_COUNT
};

/* Test patterns of all 8 configurations of the two operands and the carry input */
enum
{
  PATTERN_NC_0_0 = 0,    /* 1st operand bit 0, 2nd operand bit 0, No carry in */
  PATTERN_NC_0_1,        /* 1st operand bit 1, 2nd operand bit 0, No carry in */
  PATTERN_NC_1_0,        /* 1st operand bit 0, 2nd operand bit 1, No carry in */
  PATTERN_NC_1_1,        /* 1st operand bit 1, 2nd operand bit 1, No carry in */
  PATTERN_C_0_0,         /* 1st operand bit 0, 2nd operand bit 0, Carry in */
  PATTERN_C_0_1,         /* 1st operand bit 1, 2nd operand bit 0, Carry in */
  PATTERN_C_1_0,         /* 1st operand bit 0, 2nd operand bit 1, Carry in */
  PATTERN_C_1_1,         /* 1st operand bit 1, 2nd operand bit 1, Carry in */
  PATTERN_COUNT
};

/* Tests for different bits that impact setting up the test registers */
enum
{
  BIT_0 = 0,
  BIT_MOST,
  BIT_30,
  BIT_31,
  BIT_COUNT
};

/* Expected values of the output register and CPSR. The relevant bits of the CPSR are as follows:
   Bit 31: Negative   
   Bit 30: Zero
   Bit 29: Carry
   Bit 28: Overflow
   
   Note that only the carry bit is relevant to the tests, but all bits are checked */

/* Structure defining the expected bit value and CPSR for a test */   
typedef struct
{
  Uint32_t result;     /* The expected vale of the register used as output of the operation */
  Uint32_t cpsr;       /* The expected value of the Current Program Status Register */
} expected_t;

/***** Constants ********************************************************/

/* Expected values for all tests. Note that the expected bit value is the same for all bits under test, only the CPSR flags can change */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const expected_t expected[BIT_COUNT][FUNCTION_COUNT][PATTERN_COUNT] =
{
  /* BIT_0 */
  {
    /* Function Add */
    {
      /* Patterns */
      {0x00000000u, 0x80000000},    /* Result 0, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000}     /* Result 1, carry out */
    },
    /* Function Subtract */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000000u, 0x80000000},    /* Result 0, negative */
      {0x00000001u, 0x80000000}     /* Result 1, negative */
    },
    /* Function OR */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x00000000},    /* Result 1 */
      {0x00000001u, 0x00000000},    /* Result 1 */
      {0x00000001u, 0x00000000},    /* Result 1 */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function AND */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x00000000},    /* Result 1 */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function XOR */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
  },
  /* BIT_MOST */
  {
    /* Function Add */
    {
      /* Patterns */
      {0x00000000u, 0x80000000},    /* Result 0, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000}     /* Result 1, carry out */
    },
    /* Function Subtract */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x20000000},    /* Result 0, carry out */
      {0x00000000u, 0x80000000},    /* Result 0, negative */
      {0x00000001u, 0x80000000}     /* Result 1, negative */
    },
    /* Function OR */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x00000000},    /* Result 1 */
      {0x00000001u, 0x00000000},    /* Result 1 */
      {0x00000001u, 0x00000000},    /* Result 1 */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function AND */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x00000000},    /* Result 1 */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function XOR */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
  },
  /* BIT_30 */
  {
    /* Function Add */
    {
      /* Patterns */
      {0x00000000u, 0x80000000},    /* Result 0, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000}     /* Result 1, carry out */
    },
    /* Function Subtract */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x20000000},    /* Result 0, carry out */
      {0x00000000u, 0x80000000},    /* Result 0, negative */
      {0x00000001u, 0x80000000}     /* Result 1, negative */
    },
    /* Function OR */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x00000000},    /* Result 1 */
      {0x00000001u, 0x00000000},    /* Result 1 */
      {0x00000001u, 0x00000000},    /* Result 1 */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function AND */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x00000000},    /* Result 1 */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function XOR */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000001u, 0x20000000},    /* Result 1, carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
  },
  /* BIT_31 */
  {
    /* Function Add */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x70000000},    /* Result 0, zero, carry out and overflow */
      {0x00000001u, 0x90000000},    /* Result 1, negative and overflow */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0xA0000000}     /* Result 1, negative and carry out */
    },
    /* Function Subtract */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0xA0000000},    /* Result 1, negative and carry out */
      {0x00000001u, 0x90000000},    /* Result 1, negative and overflow */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000000u, 0x30000000},    /* Result 0, carry out and overflow */
      {0x00000000u, 0x00000000},    /* Result 0 */
      {0x00000001u, 0x80000000}     /* Result 1, negative */
    },
    /* Function OR */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function AND */
    {
      /* Patterns */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000000u, 0x40000000},    /* Result 0, zero */
      {0x00000001u, 0x80000000},    /* Result 1, negative */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    },
    /* Function XOR */
    {
      /* Patterns */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      {0x00000001u, 0xA0000000},    /* Result 1, negative and carry out */
      {0x00000001u, 0xA0000000},    /* Result 1, negative and carry out */
      {0x00000000u, 0x60000000},    /* Result 0, zero and carry out */
      /* Following entries not used */
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000},
      {0x00000000u, 0x00000000}
    }
  }
};

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static d_Status_t process(const Uint32_t bitNumber, const Uint32_t functionNumber, const Uint32_t patternNumber);
static Uint32_t prepareRegister1(const Uint32_t bitNumber, const Uint32_t functionNumber, const Uint32_t patternNumber);
static Uint32_t prepareRegister2(const Uint32_t bitNumber, const Uint32_t functionNumber, const Uint32_t patternNumber);
static Uint32_t getBitReference(const Uint32_t bitNumber);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_BIT_AluTest -->

  Perform a test on the SOC ALU.
*************************************************************************/
d_Status_t               /** \return Success or Failure */
d_BIT_AluTest
(
void
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  d_Status_t status;
  Uint32_t bitNumber;
  Uint32_t functionNumber;
  Uint32_t patternNumber;

  for (bitNumber = 0u; bitNumber < 32u; bitNumber++)
  {
    for (functionNumber = 0u; functionNumber < FUNCTION_COUNT; functionNumber++)
    {
      for (patternNumber = 0u; patternNumber < PATTERN_COUNT; patternNumber++)
      {
        status = process(bitNumber, functionNumber, patternNumber);
        if (status != d_STATUS_SUCCESS)
        {
          returnValue = d_STATUS_BIT_FAILURE;
        }
      }
    }
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- process -->

  Perform the test with one function on one bit.
*************************************************************************/
static d_Status_t               /** \return Success or Failure */
process
(
const Uint32_t bitNumber,       /**< [in] Bit number under test */
const Uint32_t functionNumber,  /**< [in] Function number to test */
const Uint32_t patternNumber    /**< [in] Bit pattern to test */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  register Uint32_t register1 __asm("r6");
  register Uint32_t register2 __asm("r7");

  /* Test add and subtract with all patterns, and logical only without carry input */
  if ((functionNumber < 2u) || (patternNumber < 4u))
  {
    Uint32_t bitReference;
    Uint32_t expectedRegister1;
    Uint32_t expectedCpsr;
    Uint32_t cpsrTemp;
    Uint32_t cpsr0;

    register1 = prepareRegister1(bitNumber, functionNumber, patternNumber);
    register2 = prepareRegister2(bitNumber, functionNumber, patternNumber);
    /* Following statement to remove compiler warning regarding unused variable register2 */
    (void)(register2);
    
    bitReference = getBitReference(bitNumber);
          
    expectedRegister1 = expected[bitReference][functionNumber][patternNumber].result;
    expectedRegister1 = expectedRegister1 << bitNumber;
    expectedCpsr = expected[bitReference][functionNumber][patternNumber].cpsr;

    switch (functionNumber)
    {
    case FUNCTION_ADD:
      cpsrTemp = d_mfcpsr();
      /* Carry input is forced on bit 0, else it is carry from previous bit operation */
      if ((patternNumber >= 4u) && (bitNumber == 0u))
      {
        /* Set carry bit */
         cpsrTemp = cpsrTemp | 0x20000000u;
      }
      else
      {
        /* Ensure carry bit is clear */
        cpsrTemp = cpsrTemp & 0xD0000000u;
      }

      d_mtcpsr(cpsrTemp);
      /* Perform the operation */
      __asm__ __volatile__("adcs    r6, r7");
      /* Save program status register */
      cpsr0 = d_mfcpsr();
      break;

    case FUNCTION_SUBTRACT:
      cpsrTemp = d_mfcpsr();
      /* Carry input is forced on bit 0, else it is carry from previous bit operation */
      if ((patternNumber >= 4u) && (bitNumber == 0u))
      {
        /* Clear carry bit */
        cpsrTemp = cpsrTemp & 0xD0000000u;
      }
      else
      {
        /* Ensure carry bit is set */
        cpsrTemp = cpsrTemp | 0x20000000u;
      }
       d_mtcpsr(cpsrTemp);
      /* Perform the operation */
      __asm__ __volatile__("sbcs    r6, r7");
      /* Save program status register */
      cpsr0 = d_mfcpsr();
      break;

    case FUNCTION_OR:
      /* Perform the operation */
      __asm__ __volatile__("orrs    r6, r7");
      /* Save program status register */
      cpsr0 = d_mfcpsr();
      break;

    case FUNCTION_AND:
      /* Perform the operation */
      __asm__ __volatile__("ands    r6, r7");
      /* Save program status register */
      cpsr0 = d_mfcpsr();
      break;

    case FUNCTION_XOR:
      /* Perform the operation */
      __asm__ __volatile__("eors    r6, r7");
      /* Save program status register */
      cpsr0 = d_mfcpsr();
      break;

    default:
      /* Never executed */
      cpsr0 = 0u;
      break;
    }

    /* Check the four Program Status Register bits against expected values */
    if ((cpsr0 & 0xF0000000u) != expectedCpsr)
    {
      returnValue = d_STATUS_BIT_FAILURE;
    }
    /* Check the value of the bit under test against expected value */
    if ((register1 & (0x01u << bitNumber)) != (expectedRegister1 & (0x01u << bitNumber)))
    {
      returnValue = d_STATUS_BIT_FAILURE;
    }
  }
 
  return returnValue;
}

/*********************************************************************//**
  <!-- prepareRegister1 -->

  Initialise first register in preparation for test.
*************************************************************************/
static Uint32_t                 /** \return Register 1 value for test */
prepareRegister1
(
const Uint32_t bitNumber,       /**< [in] Bit number under test */
const Uint32_t functionNumber,  /**< [in] Function number to test */
const Uint32_t patternNumber    /**< [in] Bit pattern to test */
)
{
  Uint32_t reg1;

  reg1 = (patternNumber & 0x01u) << bitNumber;

  /* Do not process carry bit patterns for logical instructions */
  if (functionNumber < 2u)
  {
    if (functionNumber == 0u)
    {
      if (bitNumber < 31u)
      {
        reg1 = reg1 | (0xFFFFFFFEu << bitNumber); /* Propagate carry out */
      }
    }
    /* Generate carry in for bit under test */
    if ((bitNumber > 0u) && (patternNumber > 3u))
    {
      if (functionNumber == 0u)
      {
        reg1 = reg1 | (0x01u << (bitNumber - 1u));
      }
    }
  }

  return reg1;
}

/*********************************************************************//**
  <!-- prepareRegister2 -->

  Initialise second register in preparation for test.
*************************************************************************/
static Uint32_t                 /** \return Register 2 value for test */
prepareRegister2
(
const Uint32_t bitNumber,       /**< [in] Bit number under test */
const Uint32_t functionNumber,  /**< [in] Function number to test */
const Uint32_t patternNumber    /**< [in] Bit pattern to test */
)
{
  Uint32_t reg2;

  reg2 = ((patternNumber & 0x02u) >> 1u) << bitNumber;

  /* Do not process carry bit patterns for logical instructions */
  if (functionNumber < 2u)
  {
    /* Generate carry in for bit under test */
    if ((bitNumber > 0u) && (patternNumber > 3u))
    {
      reg2 = reg2 | (0x01u << (bitNumber - 1u));
    }
  }
  
  return reg2;
}

/*********************************************************************//**
  <!-- getBitReference -->

  Get reference to expected result data based on bit number.
*************************************************************************/
static Uint32_t                 /** \return Bit reference to expected data */
getBitReference
(
const Uint32_t bitNumber        /**< [in] Bit number under test */
)
{
  Uint32_t bitReference;
  
  if (bitNumber == 0u)
  {
    bitReference = BIT_0;
  }
  else if (bitNumber == 30u)
  {
    bitReference = BIT_30;
  }
  else if (bitNumber == 31u)
  {
    bitReference = BIT_31;
  }
  else
  {
    bitReference = BIT_MOST;
  }

  return bitReference;
}
