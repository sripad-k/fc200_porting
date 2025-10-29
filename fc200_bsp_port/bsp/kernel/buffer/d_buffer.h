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

#ifndef BUFFER_H
#define BUFFER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the fixed message length buffers */
void d_BUFFER_FixedInitialise(void);

/* Flush a fixed message length buffer */
d_Status_t d_BUFFER_FixedFlush(const Uint32_t id);

/* Write a number of items to a fixed item length buffer */
d_Status_t d_BUFFER_FixedWrite(const Uint32_t id, const Uint8_t * const pData, const Uint32_t count, Uint32_t * const pActual);

/* Read a number of items from a fixed item length buffer */
d_Status_t d_BUFFER_FixedRead(const Uint32_t id, Uint8_t * const pData, const Uint32_t count, Uint32_t * const pActual);

/* Read the number of items in a fixed item length buffer */
d_Status_t d_BUFFER_FixedCount(const Uint32_t id, Uint32_t * const pCount);

/* Initialise the variable message length buffers */
void d_BUFFER_VarInitialise(void);

/* Flush a variable message length buffer */
d_Status_t d_BUFFER_VarFlush(const Uint32_t id);

/* Write a variable length message to a buffer */
d_Status_t d_BUFFER_VarWrite(const Uint32_t id, const Uint8_t * const pData, const Uint32_t length);

/* Read a variable length message from a buffer */
d_Status_t d_BUFFER_VarRead(const Uint32_t id, Uint8_t * const pData, Uint32_t * const pLength);

/* Read the number of messages in a variable length buffer */
d_Status_t d_BUFFER_VarCount(const Uint32_t id, Uint32_t * const pCount);

#endif /* BUFFER_H */
