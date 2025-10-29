/*********************************************************************//**
\file
\brief
  Module Title       : Event Logger handler configuration

  Abstract           : Event Logger handling CSC.

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

#ifndef D_EVENT_LOGGER_CFG_H
#define D_EVENT_LOGGER_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef d_Status_t(*d_EVENT_LogCriticalInitialise_t)(const Uint32_t device);

typedef d_Status_t(*d_EVENT_LogCritical_t)(d_EVENT_LogEvent_t * const pEvent);

/***** Variables ********************************************************/

/* Index to buffer entry for storing events */
extern const Uint32_t d_EVENT_Fixed_Events_Buffer_Index;

/***** Function Declarations ********************************************/

extern const Uint32_t d_EVENT_LogCriticalDevice;

/* Function to initialise CSC for storing critical error logs */
extern const d_EVENT_LogCriticalInitialise_t d_EVENT_LogCriticalInitialise;

/* Function to log a critical error */
extern const d_EVENT_LogCritical_t d_EVENT_LogCritical;

#endif /* D_EVENT_LOGGER_CFG_H */
