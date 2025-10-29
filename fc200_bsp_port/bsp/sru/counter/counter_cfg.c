/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : counter_cfg.c

  Abstract           : Counter interface configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-
                       SDD References: 136T-2200-131000-001-D22 SWDES-
\note
  CSC ID             : SWDES-
*************************************************************************/

/***** Includes *********************************************************/

#include "xparameters.h"

#include "soc/defines/d_common_types.h"
#include "sru/platform/ioc.h"

#include "sru/counter/d_counter_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Uint32_t d_COUNTER_BaseAddress[] =
{
  XPAR_IOCA_AXIOE_BASEADDR + IOC_PULSE_AXI_0_BASEADDR,
  XPAR_IOCB_AXIOE_BASEADDR + IOC_PULSE_AXI_0_BASEADDR,
};

/* Define the number of IOC interfaces */
__attribute__((weak)) d_COUNTER_COUNT_IOC;

__attribute__((weak)) const Uint32_t d_COUNTER_CountChannel = 8;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

