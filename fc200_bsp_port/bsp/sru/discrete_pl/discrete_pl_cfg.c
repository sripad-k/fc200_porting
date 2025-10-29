/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : DISCRETE PL definition

  Abstract           : This is a configuration file for the DISCRETE_PL CSC
                       The file should be copied to the application code
                       space and updated for the required settings.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-112
                       SDD References: 136T-2200-131100-001-D22 SWDES-165
                                                                SWDES-166
                                                                SWDES-167
                                                                SWDES-168
                                                                SWDES-169
                                                                SWDES-170
                                                                SWDES-171
                                                                SWDES-192
\note
  CSC ID             : SWDES-55
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/discrete_pl/d_discrete_pl_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Pointer_t d_DISC_PL_BaseAddress[] =
{
  XPAR_ZYNQ_DISC_BASEADDR,                                  /* GPIO signals */
#if defined PLATFORM_FCRP || defined PLATFORM_FC200
  XPAR_IOCA_AXIOE_BASEADDR + IOC_IOC_DISCRETE_BASEADDR,     /* IOC discretes */
  XPAR_IOCA_AXIOE_BASEADDR + IOC_TTL_DISCRETE_BASEADDR,     /* TTL discretes */
  XPAR_IOCA_AXIOE_BASEADDR + IOC_SUPPLY_DISCRETE_BASEADDR,  /* Supply discretes */
  XPAR_IOCB_AXIOE_BASEADDR + IOC_IOC_DISCRETE_BASEADDR,     /* IOC discretes */
  XPAR_IOCB_AXIOE_BASEADDR + IOC_TTL_DISCRETE_BASEADDR,     /* TTL discretes */
  XPAR_IOCB_AXIOE_BASEADDR + IOC_SUPPLY_DISCRETE_BASEADDR,  /* Supply discretes */
#endif
};

/* Define the number of PL based blocks */
__attribute__((weak)) d_DISC_PL_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

