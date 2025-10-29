/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM 

  Abstract           : Memory test functionality public header file

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-151
                                                                SWREQ-152
                                                                SWREQ-236
                                                                SWREQ-755
                       SDD References: 136T-2200-131100-001-D22 SWDES-555
                                                                SWDES-658
                                                                SWDES-659
                                                                SWDES-660

\note
  CSC ID             : SWDES-74
*************************************************************************/

#ifndef D_RAM_USR_H
#define D_RAM_USR_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum
{
  d_RAM_CRC_BUSY = 0,
  d_RAM_CRC_PASS,
  d_RAM_CRC_FAIL
} d_RAM_CodeCrcState_t;

typedef struct
{
  Uint32_t stackSize;
  Uint32_t stackUsed;
  Float32_t percentage;
} d_RAM_StackUsed_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* General */
/* Initialise the RAM CRC. */
void d_RAM_Initialise(void);

/* Cyclic processing */
void d_RAM_Cyclic(void);

/* Memory Test */
/* Perform Data bus test using the given address. */
d_Status_t d_RAM_DataBusTest(volatile Uint32_t * const pAddress);

/* Perform Address bus test using the given base address. */
d_Status_t d_RAM_AddressBusTest(volatile Uint32_t * const pBaseAddress, Uint32_t byteCount);

/* Perform physical memory integrity test. */
d_Status_t d_RAM_DeviceIntegrityTest(volatile Uint32_t * const pBaseAddress, const Uint32_t byteCount);

/* Code CRC check */
d_Status_t d_RAM_CodeCrcStatus(d_RAM_CodeCrcState_t * const pCodeCrcState);

/* Stack Check */
/* Return the stack usage */
d_Status_t d_RAM_StackUsage(d_RAM_StackUsed_t * const pUsedStackMain,
                            d_RAM_StackUsed_t * const pUsedStackIrq,
                            d_RAM_StackUsed_t * const pUsedStackSupervisor);

#endif /* D_RAM_USR_H */
