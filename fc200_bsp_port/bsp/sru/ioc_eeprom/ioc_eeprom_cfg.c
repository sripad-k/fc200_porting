/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : IOC_EEPROM interface configuration.

  Abstract           : This is the IOC_EEPROM CSC configuration.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-753
                       SDD References: 136T-2200-131100-001-D22 SWDES-2424
                                                                SWDES-2425

\note
  CSC ID             : SWDES-2419
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/ioc_eeprom/d_ioc_eeprom_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Pointer_t d_IOC_EEPROM_BaseAddress[] =
{
  XPAR_IOCA_AXIOE_BASEADDR + IOC_CONFIGURATOR_0_BASEADDR, /* IOC 0 */
  XPAR_IOCB_AXIOE_BASEADDR + IOC_CONFIGURATOR_0_BASEADDR, /* IOC 1 */
};

/* Define the number of PL based blocks */
__attribute__((weak)) d_IOC_EEPROM_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

