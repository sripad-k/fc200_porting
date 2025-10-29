/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_discrete_cfg

  Abstract           : General Purpose Input Output functions for the
                       Processing System and the Programmable Logic.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-114
                       SDD References: 136T-2200-131100-001-D22 SWDES-86
                                                                SWDES-88
                                                                SWDES-89
                                                                SWDES-90
                                                                SWDES-91
                                                                SWDES-92
                                                                SWDES-93

\note
  CSC ID             : SWDES-42
*************************************************************************/

#ifndef D_DISCRETE_CFG_H
#define D_DISCRETE_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */

/***** Type Definitions *************************************************/

typedef void (*d_DISC_IrqHandler_t)(const Uint32_t parameter);

typedef struct
{
  d_DISC_IrqHandler_t function;
  Uint32_t parameter;
}d_DISC_IrqVectorTable_t;

typedef void (*d_DISC_FcuMasterChanged_t)(const Uint32_t master);

/***** Constants ********************************************************/

extern const d_DISC_IrqVectorTable_t d_DISC_IrqVectorTable[d_DISC_IO_COUNT];

extern const d_DISC_FcuMasterChanged_t d_DISC_FcuMasterChanged;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_DISCRETE_CFG_H */
