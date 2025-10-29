/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Event logger configuration

  Abstract           : This is the event logger CSC
                       definition of buffer indexes. The file
                       should be copied to the application code space and
                       renamed to a c file.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "kernel/event_logger/d_event_logger.h"
#include "kernel/event_logger/d_event_logger_critical.h"
#include "kernel/event_logger/d_event_logger_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Uint32_t d_EVENT_Fixed_Events_Buffer_Index = 0;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

__attribute__((weak)) const Uint32_t d_EVENT_LogCriticalDevice = 1;

__attribute__((weak)) const d_EVENT_LogCriticalInitialise_t d_EVENT_LogCriticalInitialise = d_EVENT_CriticalInitialise;

__attribute__((weak)) const d_EVENT_LogCritical_t d_EVENT_LogCritical = d_EVENT_Critical;

/***** Function Definitions *********************************************/

