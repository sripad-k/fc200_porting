/*********************************************************************//**
\file
\brief
  Module Title       : Processor loading measurement

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

#ifndef D_SCHED_LOADING_H
#define D_SCHED_LOADING_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

#define MAX_LOADING_TASKS    (Uint32_t)20

/***** Type Definitions *************************************************/

/* Structure of loading data as calculated from the task execution times */
typedef struct
{
  Float32_t averageTime;
  Float32_t maximumTime;
  Float32_t frequency;
  Float32_t loading;
} d_SCHED_TaskMetrics_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise loading measurement */
d_Status_t d_SCHED_LoadingInitialise(void);

/* Indicate the start of a task */
void d_SCHED_LoadingTaskStart(const Uint32_t taskId);

/* Indicate the end of a task */
void d_SCHED_LoadingTaskEnd(const Uint32_t taskId);

/* Reset loading times to restart averaging */
void d_SCHED_LoadingReset(void);

/* Process the event buffer to determine the time spent in each task */
void d_SCHED_LoadingProcessBuffer(void);

/* Calculate the task loading metrics */
Float32_t d_SCHED_LoadingMetrics(void);

/* Get the loading data for sending to a host PC */
Uint32_t d_SCHED_LoadingGetMetrics(Uint8_t * const destination_ptr,
                                   const Uint32_t maxCount);

/* Get the loading data for a specific task */
d_Status_t d_SCHED_LoadingGetMetric(const Uint32_t task,
                                    d_SCHED_TaskMetrics_t * const pMetrics);

#endif /* D_SCHED_LOADING_H */
