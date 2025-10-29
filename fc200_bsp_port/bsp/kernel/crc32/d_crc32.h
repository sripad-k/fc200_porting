/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : CRC32

  Abstract           : CSC providing CRC32 calculations

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-144
                       SDD References: 136T-2200-131000-001-D22 SWDES-210
                                                                SWDES-211
                                                                SWDES-212
                                                                SWDES-731
\note
  CSC ID             : SWDES-81
*************************************************************************/

#ifndef D_CRC32_H
#define D_CRC32_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the CRC32 CSC */
d_Status_t d_CRC32_Initialise(void);

/* Calculate CRC32 of block of memory */
Uint32_t d_CRC32_Calculate(const Uint8_t * const pMemory, const Uint32_t length);

/* Adds block of memory to existing CRC32 */
void d_CRC32_Add(Uint32_t * const pCrc, const Uint8_t * const pMemory, const Uint32_t length);

/* Verify functionality by calculating CRC of known block of memory */
d_Status_t d_CRC32_Bit(void);

#endif /* D_CRC32_H */
