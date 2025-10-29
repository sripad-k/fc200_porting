/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger_mmc

  Abstract           : Event Logger driver module

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

#ifndef D_EVENT_LOGGER_MMC_H
#define D_EVENT_LOGGER_MMC_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "sru/mmc/d_mmc_interface.h"

/***** Literals ********************************************************/

#define MMC_SECTOR_SIZE  512u

/* Events */
#define  LOG_EVENT_ALLOC_SECTOR_1        1u
#define  LOG_EVENT_ALLOC_SECTOR_2        2u
#define  LOG_EVENT_START_SECTOR          5u
#define  LOG_EVENT_SECTOR_COUNT          (((d_EVENT_MAX_EVENT_ENTRIES * sizeof(d_EVENT_LogEvent_t)) + (MMC_SECTOR_SIZE - 1u)) / MMC_SECTOR_SIZE)

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the MMC interface */
d_Status_t InitialiseMmc(void);

/*Write stored data to MMC */
void ProcessMmc(void);

#endif /* D_EVENT_LOGGER_MMC_H */
