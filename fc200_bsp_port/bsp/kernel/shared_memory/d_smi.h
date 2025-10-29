/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Buffer

  Abstract           : Buffer driver module

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-145
                       SDD References: 136T-2200-131100-001-D22 SWDES-200
                                                                SWDES-201
                                                                SWDES-202
                                                                SWDES-203
                                                                SWDES-204
                                                                SWDES-205
                                                                SWDES-206
                                                                SWDES-207
                                                                SWDES-208
                                                                SWDES-209

\note
  CSC ID             : SWDES-80
*************************************************************************/

#ifndef SMI_H
#define SMI_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the variable message length buffers */
void d_SMI_Initialise(void);

/* Flush a variable message length/ buffer */
void d_SMI_TransmitFlush(void);

/* Flush a variable message length/ buffer */
void d_SMI_ReceiveFlush(void);

/* Write a variable length message to a buffer */
d_Status_t d_SMI_Transmit(const Uint8_t * const pData, const Uint32_t length);

/* Read a variable length message from a buffer */
d_Status_t d_SMI_Receive(Uint8_t * const pData, Uint32_t * const pLength);

/* Read the number of messages in a variable length buffer */
d_Status_t d_SMI_ReceiveCount(Uint32_t * const pCount);

#endif /* SMI_H */
