/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_discrete_pl

  Abstract           : General Purpose Input Output functions for the
                       Programmable Logic.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-112
                       SDD References: 136T-2200-131100-001-D22 SWDES-165
                                                                SWDES-166
                                                                SWDES-167
                                                                SWDES-168
                                                                SWDES-169
                                                                SWDES-170
                                                                SWDES-171
                                                                SWDES-192
\note
  CSC ID             : SWDES-55
*************************************************************************/

#ifndef D_DISCRETE_PL_H
#define D_DISCRETE_PL_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum
{
  d_DISC_PL_EDGE_RISING = 0,
  d_DISC_PL_EDGE_FALLING,

  d_DISC_PL_EDGE_COUNT
} d_DISC_PL_Edge_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Verify configuration and initialise data */
d_Status_t d_DISC_PL_Initialise(void);

/* Read the status of a discrete pin */
d_Status_t d_DISC_PL_ReadPin(const Uint32_t block, const Uint32_t bit, Uint32_t * const pPinStatus);

/* Set the status of a discrete pin */
d_Status_t d_DISC_PL_SetPin(const Uint32_t block, const Uint32_t bit);

/* Clear the status of a discrete pin */
d_Status_t d_DISC_PL_ClearPin(const Uint32_t block, const Uint32_t bit);

/* Toggle the status of a discrete pin */
d_Status_t d_DISC_PL_TogglePin(const Uint32_t block, const Uint32_t bit);

/* Read the status of a discrete output pin */
d_Status_t d_DISC_PL_ReadOutputPin(const Uint32_t block, const Uint32_t bit, Uint32_t * const pPinStatus);

/* Read the status of a discrete loopback pin */
d_Status_t d_DISC_PL_ReadLoopbackPin(const Uint32_t block, const Uint32_t bit, Uint32_t * const pPinStatus);

/* Read the output fault register */
d_Status_t d_DISC_PL_ReadFault(const Uint32_t block, Uint32_t * const pFault);

/* Read the input fault register */
d_Status_t d_DISC_PL_ReadFaultInput(const Uint32_t block, Uint32_t * const pFault);

/* Enable the interrupt for the specified IP block and pin */
d_Status_t d_DISC_PL_InterruptEnable(const Uint32_t block, const Uint32_t bit, const d_DISC_PL_Edge_t edge);

/* Disable the interrupt for the specified IP block and pin */
d_Status_t d_DISC_PL_InterruptDisable(const Uint32_t block, const Uint32_t bit);

/* Clear the interrupt from the specified IP block and pin */
d_Status_t d_DISC_PL_InterruptClear(const Uint32_t block, const Uint32_t bit);

/* Read the interrupt status */
d_Status_t d_DISC_PL_InterruptStatus(const Uint32_t block, const Uint32_t bit, Bool_t * const pState);

#endif /* D_DISCRETE_PL_H */
