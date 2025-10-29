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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/discrete/d_discrete.h"
#include "sru/watchdog/d_watchdog_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_DISC_IO_t watchdogPin = d_DISC_MIO_71;

__attribute__((weak)) const Uint32_t pulseLength = 1;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

