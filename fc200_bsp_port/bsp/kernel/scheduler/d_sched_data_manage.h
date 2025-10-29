/*********************************************************************//**
\file
\brief
  Module Title      : Scheduler data management

  Abstract          : Ensure shared variables are consistent, and
                      complete, throughout each task's execution time

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

#ifndef D_SCHED_DATA_MANAGE_H
#define D_SCHED_DATA_MANAGE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Copies data from higher rate tasks that is used by this task */
void d_SCHED_DataMngTaskStart(const Uint32_t period);

/* Copies data from specifed task to other users with higher rates */
void d_SCHED_DataMngTaskEnd(const Uint32_t period);

#endif /* D_SCHED_DATA_MANAGE_H */
