/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM 

  Abstract           : Memory test functionality

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-237
                       SDD References: 136T-2200-131100-001-D22 SWDES-656
                                                                SWDES-662
\note
  CSC ID             : SWDES-74
*************************************************************************/

#ifndef D_RAM_H
#define D_RAM_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the code CRC check process */
void d_RAM_CodeCrcInitialise(void);

/* Return the CRC */
d_Status_t d_RAM_CodeCrc(Uint32_t * const pCrc);

/* Cyclic code CRC processing */
void d_RAM_CodeCrcCyclic(void);

/* Initialise the stack usage calculation process */
void d_RAM_StackInitialise(void);

/* Cyclic stack processing */
void d_RAM_StackCyclic(void);

#endif /* D_RAM_H */
