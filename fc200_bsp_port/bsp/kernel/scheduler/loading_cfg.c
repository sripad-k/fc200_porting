/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Loading task count

  Abstract           : This is a template for the Scheduler Loading CSC
                       definition of the number of loading tasks. The file
                       should be copied to the application code space and
                       renamed to a c file.

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
#include "kernel/scheduler/d_sched_loading_cfg.h"

/***** Constants ********************************************************/

/* Number of tasks used for loading measurement */
__attribute__((weak)) const Uint32_t LOADING_TASK_COUNT = 2;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

