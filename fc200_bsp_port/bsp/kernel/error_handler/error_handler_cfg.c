/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Error handler configuration

  Abstract           : This is the error handler CSC
                       definition of the diagnostic serial channel. The
                       file should be copied to the application code
                       space and renamed to a c file.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-183
                                                                SWREQ-184
                                                                SWREQ-185
                       SDD References: 136T-2200-131000-001-D22 SWDES-238
                                                                SWDES-239
                                                                SWDES-240
                                                                SWDES-241
                                                                SWDES-242
                                                                SWDES-243
                                                                SWDES-244
                                                                SWDES-245
                                                                SWDES-246
                                                                SWDES-247
                                                                SWDES-248
                                                                SWDES-249
\note
  CSC ID             : SWDES-74
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "kernel/error_handler/d_error_handler_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Uint32_t d_ERROR_UartChannel = 0;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

