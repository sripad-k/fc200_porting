/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger_mmc_event

  Abstract           : Event Logger driver module

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-72
                                                                SWREQ-73
                       SDD References: 136T-2200-131000-001-D22 SWDES-221
                                                                SWDES-222
                                                                SWDES-223
                                                                SWDES-224
                                                                SWDES-226
                                                                SWDES-227
                                                                SWDES-228
                                                                SWDES-229
                                                                SWDES-230
                                                                SWDES-231
\note
  CSC ID             : SWDES-77
*************************************************************************/

#ifndef D_EVENT_LOGGER_MMC_EVENT_H
#define D_EVENT_LOGGER_MMC_EVENT_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals ********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

extern Uint32_t missedEvents;

/***** Function Declarations ********************************************/

/* Initialise the event logger MMC interface */
d_Status_t EventMmcInitialise(void);

d_Status_t EventMmcCount(Uint32_t * const pCount);

d_Status_t EventMmcClear(void);

/* Write stored events to MMC */
void ProcessMmcEvent(void);

#endif /* D_EVENT_LOGGER_MMC_EVENT_H */
