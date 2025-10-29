/*********************************************************************//**
\file
\brief
  Module Title       : Error handler configuration

  Abstract           : Error handling driver.

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

#ifndef D_ERROR_HANDLER_CFG_H
#define D_ERROR_HANDLER_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

#define UART_LOGGING_NOT_REQUIRED   0xFFFFFFFFu

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/* Define the UART channel to use for diagnostic messages */
extern const Uint32_t d_ERROR_UartChannel;

/***** Function Declarations ********************************************/

#endif /* D_ERROR_HANDLER_CFG_H */
