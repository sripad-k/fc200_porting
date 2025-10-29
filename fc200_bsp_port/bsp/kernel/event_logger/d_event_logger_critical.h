/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger_critical.h

  Abstract           : Event Logger for critical errors

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

#ifndef D_EVENT_LOGGER_CRITICAL_H
#define D_EVENT_LOGGER_CRITICAL_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals ********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the critical error interface */
d_Status_t d_EVENT_CriticalInitialise(const Uint32_t device);

/* Log a critical error */
d_Status_t d_EVENT_Critical(d_EVENT_LogEvent_t * const pEvent);

/* Get the number of logged critical errors */
d_Status_t d_EVENT_CriticalCount(Uint32_t * const pCount);

/* Clear the critical error log */
d_Status_t d_EVENT_CriticalClear(void);

/* Read a critical error */
d_Status_t d_EVENT_CriticalRead(const Uint32_t entryNumber, d_EVENT_LogEvent_t * const pEvent);

#endif /* D_EVENT_LOGGER_CRITICAL_H */
