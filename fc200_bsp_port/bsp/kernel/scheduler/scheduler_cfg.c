/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Scheduler task definition

  Abstract           : This is a template for the Scheduler CSC
                       definition of the number of tasks and their periods.
                       The file should be copied to the application code
                       space, updated for the required tasks, and renamed
                       to a c file.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "kernel/scheduler/d_sched_scheduler_cfg.h"
#include "kernel/scheduler/d_sched_data_manage.h"

/* Include here any header files containing task function definitions */

/***** Constants ********************************************************/

/* Task tick period in milliseconds */
__attribute__((weak)) const Uint32_t TICK_PERIOD = 20;

/* Task definitions */
__attribute__((weak)) const ScheduleTask_t SchedulerTasks[] =
{
  {
    1, NULL          /* Tick period multiplier (20ms), task entry point */
  },

};

/* Number of tasks */
__attribute__((weak)) const Uint32_t SCHEDULER_TASK_COUNT = (sizeof(SchedulerTasks) / sizeof(ScheduleTask_t));

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/* Weak functions that are used if the data manager CSC is not used */

/*********************************************************************//**
  <!-- d_SCHED_DataMngTaskStart -->
  
  Copies data from higher rate tasks that is used by this task.
*************************************************************************/
__attribute__((weak))
void                      /** \return None */
// gcov-jst 8 This function is only linked in if not provided by the application.
d_SCHED_DataMngTaskStart
(
Uint32_t period           /**< [in] period that has started */
)
{
  UNUSED_PARAMETER(period);
  
  return;
}

/*********************************************************************//**
  <!-- d_SCHED_DataMngTaskEnd -->

  Copies data from specifed task to other users with higher rates.
*************************************************************************/
__attribute__((weak))
void                      /** \return None */
// gcov-jst 8 This function is only linked in if not provided by the application.
d_SCHED_DataMngTaskEnd
(
Uint32_t period           /**< [in] period that has ended */
)
{
  UNUSED_PARAMETER(period);
  
  return;
}
