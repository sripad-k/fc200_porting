/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Elapsed time power cycle counter interface definition.

  Abstract           : This is DS1682 CSC configuration.
                       The file should be copied to the application code
                       space and updated for the required devices.

  Software Structure : SRS References: Document numbers and versions.
                       SDD References: Document numbers and versions.

*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/ds1682/d_ds1682_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Pointer_t d_DS1682_BaseAddress[] =
{
  XPAR_PSU_BIT_DS1683_AXI_BASEADDR,
  XPAR_IOCA_AXIOE_BASEADDR + IOC_ETC_REG_BASEADDR, /* IOC A */
  XPAR_IOCB_AXIOE_BASEADDR + IOC_ETC_REG_BASEADDR, /* IOC B */
};

/* Define the number of PL based blocks */
__attribute__((weak)) d_DS1682_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

