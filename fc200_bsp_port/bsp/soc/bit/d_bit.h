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

#ifndef D_BIT_H
#define D_BIT_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Perform a test on the SOC ALU */
d_Status_t d_BIT_AluTest(void);

#endif /* D_BIT_H */
