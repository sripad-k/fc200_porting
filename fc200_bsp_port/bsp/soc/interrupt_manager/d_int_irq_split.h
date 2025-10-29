/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : IRQ Split Handler

  Abstract : Handles interrupt indication source with flag word.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-164
                                                                SWREQ-165
                                                                SWREQ-166
                                                                SWREQ-167
                       SDD References: 136T-2200-131000-001-D22 SWDES-172
                                                                SWDES-173
                                                                SWDES-174
                                                                SWDES-175
                                                                SWDES-176
                                                                SWDES-177
                                                                SWDES-178
                                                                SWDES-179
                                                                SWDES-180
                                                                SWDES-181
                                                                SWDES-182
                                                                SWDES-183
                                                                SWDES-184
                                                                SWDES-185
                                                                SWDES-186
                                                                SWDES-187
                                                                SWDES-188
\note
  CSC ID             : SWDES-52
*************************************************************************/

#ifndef D_INT_IRQ_SPLIT_H
#define D_INT_IRQ_SPLIT_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Handler for IRQ split exceptions */
void d_INT_SplitHandler(const Uint32_t parameter);

#endif /* D_INT_IRQ_SPLIT_H */
