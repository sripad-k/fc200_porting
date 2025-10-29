/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM STACK

  Abstract           : Memory test functionality

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-755
                       SDD References: 136T-2200-131100-001-D22 SWDES-662
                                                                SWDES-663

\note
  CSC ID             : SWDES-74
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_asm.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"

#include "d_ram_usr.h"
#include "d_ram_cfg.h"
#include "d_ram.h"

/***** Constants ********************************************************/

/* Linker defined variables */
// clang-tidy-jst bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp 6 Externally defined linker generated variables. Does not present a risk.
extern Uint32_t _stack_end;
extern Uint32_t _STACK_SIZE;
extern Uint32_t _irq_stack_end;
extern Uint32_t _IRQ_STACK_SIZE;
extern Uint32_t _supervisor_stack_end;
extern Uint32_t _SUPERVISOR_STACK_SIZE;

/***** Type Definitions *************************************************/

typedef enum
{
  STACK_MAIN = 0,
  STACK_IRQ,
  STACK_SUPERVISOR,
  
  STACK_COUNT
} stackId_t;

typedef struct
{
  Uint32_t * pStart;
  Uint32_t startUint;
  Uint32_t wordCount;
  Uint32_t scanIndex;
  Uint32_t used;
} stackStatus_t;

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static stackStatus_t stackStatus[STACK_COUNT];

static Uint32_t iterations_per_invocation;

/***** Function Declarations ********************************************/

static void stackFill(Uint32_t * const pStart, const Uint32_t length, const Uint32_t startValue);
static void stackCheck(const stackId_t stackId);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_RAM_StackInitialise -->

  Initialise the stack usage calculation process.
*************************************************************************/
void                                            /** \return None */
d_RAM_StackInitialise
(
void
)
{
  /* Stack */
  stackStatus[STACK_MAIN].pStart = &_stack_end;
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  stackStatus[STACK_MAIN].startUint = (Uint32_t)stackStatus[STACK_MAIN].pStart;
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  stackStatus[STACK_MAIN].wordCount = (Uint32_t)&_STACK_SIZE / sizeof(Uint32_t);
  stackStatus[STACK_MAIN].scanIndex = 0;
  stackStatus[STACK_MAIN].used = 0u;

  /* Subtract 32 from stack below current SP to allow for call to stackFill function */
  stackFill(stackStatus[STACK_MAIN].pStart,
            (d_getsp() - stackStatus[STACK_MAIN].startUint - 64u) / sizeof(Uint32_t),
            stackStatus[STACK_MAIN].startUint);

  /* IRQ stack */
  stackStatus[STACK_IRQ].pStart = &_irq_stack_end;
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  stackStatus[STACK_IRQ].startUint = (Uint32_t)stackStatus[STACK_IRQ].pStart;
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  stackStatus[STACK_IRQ].wordCount = (Uint32_t)&_IRQ_STACK_SIZE / sizeof(Uint32_t);
  stackStatus[STACK_IRQ].scanIndex = 0;
  stackStatus[STACK_IRQ].used = 0u;

  stackFill(stackStatus[STACK_IRQ].pStart, stackStatus[STACK_IRQ].wordCount, stackStatus[STACK_IRQ].startUint);

  /* SUPERVISOR stack */
  stackStatus[STACK_SUPERVISOR].pStart = &_supervisor_stack_end;
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  stackStatus[STACK_SUPERVISOR].startUint = (Uint32_t)stackStatus[STACK_SUPERVISOR].pStart;
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  stackStatus[STACK_SUPERVISOR].wordCount = (Uint32_t)&_SUPERVISOR_STACK_SIZE / sizeof(Uint32_t);
  stackStatus[STACK_SUPERVISOR].scanIndex = 0;
  stackStatus[STACK_SUPERVISOR].used = 0u;

  stackFill(stackStatus[STACK_SUPERVISOR].pStart, stackStatus[STACK_SUPERVISOR].wordCount, stackStatus[STACK_SUPERVISOR].startUint);

  /* Calculate number of iterations required per invocation */
  Uint32_t stackSize = stackStatus[STACK_MAIN].wordCount;
  if (stackStatus[STACK_IRQ].wordCount > stackSize)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    stackSize = stackStatus[STACK_IRQ].wordCount;
  }
  else
  {
    DO_NOTHING();
  }

  if (stackStatus[STACK_SUPERVISOR].wordCount > stackSize)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    stackSize = stackStatus[STACK_SUPERVISOR].wordCount;
  }
  else
  {
    DO_NOTHING();
  }

  iterations_per_invocation = (stackSize * d_RAM_CyclicExecutionTime) / d_RAM_StackCheckTime;

  initialised = d_TRUE;
  
  return;
}

/*********************************************************************//**
  <!-- d_RAM_StackCyclic -->

  Cyclic stack processing.
*************************************************************************/
void                                            /** \return None */
d_RAM_StackCyclic
(
void
)
{
  /* Ensure the CSC has been initialised */
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_UNKNOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  stackCheck(STACK_MAIN);
  stackCheck(STACK_IRQ);
  stackCheck(STACK_SUPERVISOR);

  return;
}

/*********************************************************************//**
  <!-- d_RAM_StackUsage -->

  Return the stack usage.
*************************************************************************/
d_Status_t                                      /** \return Function status */
d_RAM_StackUsage
(
d_RAM_StackUsed_t * const pUsedStackMain,       /**< [out] Pointer to storage for main stack used */
d_RAM_StackUsed_t * const pUsedStackIrq,        /**< [out] Pointer to storage for IRQ stack used */
d_RAM_StackUsed_t * const pUsedStackSupervisor  /**< [out] Pointer to storage for supervisor stack used */
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

  /* Parameter check */
  if (pUsedStackMain == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pUsedStackIrq == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pUsedStackSupervisor == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  pUsedStackMain->stackSize = stackStatus[STACK_MAIN].wordCount;
  pUsedStackMain->stackUsed = stackStatus[STACK_MAIN].used;
  pUsedStackMain->percentage = 100.0f * (Float32_t)stackStatus[STACK_MAIN].used / (Float32_t)stackStatus[STACK_MAIN].wordCount;

  pUsedStackIrq->stackSize = stackStatus[STACK_IRQ].wordCount;
  pUsedStackIrq->stackUsed = stackStatus[STACK_IRQ].used;
  pUsedStackIrq->percentage = 100.0f * (Float32_t)stackStatus[STACK_IRQ].used / (Float32_t)stackStatus[STACK_IRQ].wordCount;

  pUsedStackSupervisor->stackSize = stackStatus[STACK_SUPERVISOR].wordCount;
  pUsedStackSupervisor->stackUsed = stackStatus[STACK_SUPERVISOR].used;
  pUsedStackSupervisor->percentage = 100.0f * (Float32_t)stackStatus[STACK_SUPERVISOR].used / (Float32_t)stackStatus[STACK_SUPERVISOR].wordCount;

  return status;
}

/*********************************************************************//**
  <!-- stackFill -->

  Fill the stack with the inverse of the address.
*************************************************************************/
static void                                 /** \return None */
stackFill
(
Uint32_t * const pStart,                    /**< [in] Pointer to storage for CRC */
const Uint32_t length,                      /**< [in] Pointer to storage for CRC */
const Uint32_t startValue                   /**< [in] Pointer to storage for CRC */
)
{
  Uint32_t index;
  Uint32_t value = startValue;

  for (index = 0; index < length; index++)
  {
    pStart[index] = ~value;
    value = value + sizeof(Uint32_t);
  }

  return;
}

/*********************************************************************//**
  <!-- stackCheck -->

  Perform cyclic stack processing on specified stack.
*************************************************************************/
static void                                 /** \return None */
stackCheck
(
const stackId_t stackId                     /**< [in] Stack identifier */
)
{
  Uint32_t iteration = 0;
  Bool_t done = d_FALSE;
  Uint32_t value = stackStatus[stackId].startUint + (stackStatus[stackId].scanIndex * sizeof(Uint32_t));

  while ((iteration < iterations_per_invocation) &&
         (stackStatus[stackId].scanIndex < stackStatus[stackId].wordCount) &&
         (done == d_FALSE))
  {
    if (stackStatus[stackId].pStart[stackStatus[stackId].scanIndex] != ~value)
    {
      stackStatus[stackId].used = stackStatus[stackId].wordCount - stackStatus[stackId].scanIndex;
      stackStatus[stackId].scanIndex = 0;
      
      /* Log error if more than 80% usage */
      if (((stackStatus[stackId].used * 100u) / stackStatus[stackId].wordCount) > 80u)
      {
        // gcov-jst 1 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_BIT_FAILURE, d_ERROR_CRITICALITY_UNKNOWN, stackId, stackStatus[stackId].used, stackStatus[stackId].wordCount, 0);
      }
      else
      {
        DO_NOTHING();
      }
      
      done = d_TRUE;
    }
    else
    {
      stackStatus[stackId].scanIndex++;
      value = value + sizeof(Uint32_t);
      iteration++;
    }
  }

  /* If there were no changed entries, restart the scan */
  if (stackStatus[stackId].scanIndex == stackStatus[stackId].wordCount)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    stackStatus[stackId].used = 0u;
    stackStatus[stackId].scanIndex = 0;
  }
  else
  {
    DO_NOTHING();
  }
  
  return;
}

