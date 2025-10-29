/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Watchdog

  Abstract           : Watchdog interface.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-162
                       SDD References: 136T-2200-131000-001-D22 SWDES-250
\note
  CSC ID             : SWDES-65
*************************************************************************/

#ifndef D_WATCHDOG_CFG_H
#define D_WATCHDOG_CFG_H

/***** Includes *********************************************************/

#include "soc/discrete/d_discrete.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/* Watchdog strobe pin */
extern const d_DISC_IO_t watchdogPin;

/* Pulse length in microseconds */
extern const Uint32_t pulseLength;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_WATCHDOG_CFG_H */
