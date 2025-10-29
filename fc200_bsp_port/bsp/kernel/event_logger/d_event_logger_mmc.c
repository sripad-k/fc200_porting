/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger_mmc

  Abstract           : Event Logger driver module for the MMC Flash

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
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"
#include "d_event_logger.h"
#include "d_event_logger_mmc.h"
#include "d_event_logger_mmc_event.h"
#include "kernel/buffer/d_buffer.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/crc32/d_crc32.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- InitialiseMmc -->

  Initialise the event log on the MMC
*************************************************************************/
d_Status_t             /** \return Success or Failure */
InitialiseMmc
(
void
)
{
  d_Status_t returnValue = d_MMC_Initialise(NULL);
  
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = EventMmcInitialise();
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  return returnValue;
}

/*********************************************************************//**
  <!-- d_EVENT_ProcessMmc -->

  Background write of data to MMC
  This function should be called by the scheduler periodically.
*************************************************************************/
void         /** \return None */
d_EVENT_ProcessMmc
(
void
)
{
  /* Write any events to MMC */
  ProcessMmcEvent();

  return;
}

