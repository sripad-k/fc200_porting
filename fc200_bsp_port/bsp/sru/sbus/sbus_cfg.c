/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SBUS CFG

  Abstract           : This is the SBUS CSC configuration
                       The file should be copied to the application code
                       space and updated for the required interfaces.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-331
                                                                SWREQ-332
                       SDD References: 136T-2200-131000-001-D22 SWDES-893
                                                                SWDES-894

\note
  CSC ID             : SWDES-890
*************************************************************************/

/***** Includes *********************************************************/

#include "xparameters.h"

#include "soc/defines/d_common_types.h"
#include "sru/platform/ioc.h"

#include "sru/sbus/d_sbus_cfg.h"

/***** Constants ********************************************************/

/* Interface bas address definitions */
__attribute__((weak)) const Uint32_t d_SBUS_BaseAddress[d_SBUS_IOC_COUNT] =
{
  XPAR_IOCA_AXIOE_BASEADDR + IOC_SBUS_AXI_BASEADDR,
  XPAR_IOCB_AXIOE_BASEADDR + IOC_SBUS_AXI_BASEADDR
};

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

