/*********************************************************************//**
\file
\brief
  Module Title       : Scheduler

  Abstract           : Rate monotonic scheduler.

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

#ifndef D_SCHED_SCHEDULER_H
#define D_SCHED_SCHEDULER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Check the configuration data and initialise the local data */
d_Status_t d_SCHED_SchedulerInitialise(void);

/* Perform the scheduling algorithm */
void d_SCHED_SchedulerTick(void);

/* Suspend a task */
d_Status_t d_SCHED_TaskSuspend(const Uint32_t task);


#endif /* D_SCHED_SCHEDULER_H */
