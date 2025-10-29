/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Watchdog

  Abstract           : Watchdog interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-162
                       SDD References: 136T-2200-131000-001-D22 SWDES-251
                                                                SWDES-252
                                                                SWDES-253
\note
  CSC ID             : SWDES-65
*************************************************************************/

#ifndef D_WATCHDOG_H
#define D_WATCHDOG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the watchdog interface */
d_Status_t d_WDOG_Initialise(void);

/* Strobe the watchdog */
d_Status_t d_WDOG_Service(void);

/* Start strobing the watchdog */
d_Status_t d_WDOG_Start(void);

/* Stop strobing the watchdog */
d_Status_t d_WDOG_Stop(void);

#endif /* D_WATCHDOG_H */
