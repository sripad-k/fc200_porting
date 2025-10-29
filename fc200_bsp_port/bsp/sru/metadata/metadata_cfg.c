/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Metadata interface configuration, Revision 03.

  Abstract           : This is the METADATA CSC configuration.
                       The file should be copied to the application code
                       space and updated for the metadata blocks.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-157
                                                                SWREQ-158
                                                                SWREQ-159
                       SDD References: 136T-2200-131000-001-D22 SWDES-546
                                                                SWDES-604
\note
  CSCID              : SWDES-60
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/metadata/d_metadata_cfg.h"

/***** Constants ********************************************************/

static const Uint32_t HEADER_OFFSET = 0x800u;

/* Storage for PS metadata read from Flash */
static __attribute__((aligned(64))) Uint32_t metaDataStorage[d_METADATA_MAXIMUM_SIZE];

__attribute__((weak)) const Pointer_t d_METADATA_BaseAddress[] =
{
  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  (Uint32_t)&metaDataStorage[0],                                          /* FCU PS, need to fetch from Flash */
  XPAR_FLASH_CONFIGURATOR_0_BASEADDR + HEADER_OFFSET,                     /* FCU PL */
#if defined PLATFORM_FC200
  XPAR_PSU_BIT_PSU_CONFIG_0_BASEADDR + HEADER_OFFSET,                     /* FCU PSU */
  XPAR_IOCA_AXIOE_BASEADDR + IOC_CONFIGURATOR_0_BASEADDR + HEADER_OFFSET, /* IOC 0 */
  XPAR_IOCB_AXIOE_BASEADDR + IOC_CONFIGURATOR_0_BASEADDR + HEADER_OFFSET, /* IOC 1 */
#endif
};

/* Define the number of Metadata sections */
__attribute__((weak)) d_METADATA_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

