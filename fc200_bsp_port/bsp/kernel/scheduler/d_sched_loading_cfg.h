/*********************************************************************//**
\file
\brief
  Module Title       : Processor loading measurement configuration

  Abstract           : Provides functions to measure the processor
                       loading for tasks and interrupts.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-74
                                                                SWREQ-75
                       SDD References: 136T-2200-131000-001-D22 SWDES-557
                                                                SWDES-664
                                                                SWDES-665
                                                                SWDES-666
                                                                SWDES-667
                                                                SWDES-668
                                                                SWDES-669
                                                                SWDES-670
                                                                SWDES-671
                                                                SWDES-672
                                                                SWDES-673
                                                                SWDES-674
                                                                SWDES-835
\note
  CSC ID             : SWDES-79
*************************************************************************/

#ifndef D_SCHED_LOADING_CFG_H
#define D_SCHED_LOADING_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/* Define the number of tasks, from application */
extern const Uint32_t LOADING_TASK_COUNT;

/***** Function Declarations ********************************************/

#endif /* D_SCHED_LOADING_CFG_H */
