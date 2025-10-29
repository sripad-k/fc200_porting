/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Temperature configuration

  Abstract           : This is the TEMPERATURE CSC configuration
                       The file should be copied to the application code
                       space and updated for the required sensors.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-125
                       SDD References: 136T-2200-131000-001-D22 SWDES-2426
\note
  CSC ID             : SWDES-2420
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/temperature/d_temperature_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_TEMPERATURE_Configuration_t d_TEMPERATURE_Configuration[] =
{
  {
    XPAR_FCU_TMP1075_BASEADDR, -50.0f, 105.0f                               /* Sensor on the FCU */
  },
  {
    XPAR_PSU_BIT_PSU_TMP1075_BASEADDR, -50.0f, 105.0f                       /* Sensor on the PSU */
  },
#if defined PLATFORM_FCRP || defined PLATFORM_FC200
  {
    XPAR_IOCA_AXIOE_BASEADDR + IOC_TEMPERATURE_REG_BASEADDR, -50.0f, 105.0f /* IOC A board temperature */
  },
  {
    XPAR_IOCB_AXIOE_BASEADDR + IOC_TEMPERATURE_REG_BASEADDR, -50.0f, 105.0f /* IOC B board temperature */
  }
#endif
};

/* Define the number of PL based blocks */
__attribute__((weak)) d_TEMPERATURE_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

