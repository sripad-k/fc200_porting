/*********************************************************************//**
\file
\brief
  Module Title       : Scheduler configuration

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

#ifndef D_SCHED_SCHEDULER_CFG_H
#define D_SCHED_SCHEDULER_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/* Define the tick period in millisecond, supplied by the applications */
extern const Uint32_t TICK_PERIOD;

/* Define the number of tasks, supplied by the application */
extern const Uint32_t SCHEDULER_TASK_COUNT;

/***** Type Definitions *************************************************/

/* Task definition structure */
typedef struct
{
  Uint32_t multiplier;    /* TICK_PERIOD multiplier for the task */
  void (*task)(void);     /* Task function */
} ScheduleTask_t;

/***** Variables ********************************************************/

/* This constant needs to be declared after the type definition above */
// cppcheck-suppress misra-c2012-8.11; Array size is defined by the configuration data of this driver. Violation of the 'Advisory' rule does not present a risk
extern const ScheduleTask_t SchedulerTasks[];

/***** Function Declarations ********************************************/

#endif /* D_SCHED_SCHEDULER_CFG_H */
