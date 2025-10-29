/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SPI PL definition

  Abstract           : This is the SPI PL CSC configuration
                       of the interfaces.
                       The file should be copied to the application code
                       space and updated for the required buffers.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-758
                       SDD References: 136T-2200-131100-001-D22 SWDES-2427
                                                                SWDES-2428
                                                                SWDES-2429
                                                                SWDES-2430
                                                                SWDES-2431
                                                                SWDES-2432
\note
  CSC ID             : SWDES-2422
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/spi_pl/d_spi_pl_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_SPI_PL_Definition_t d_SPI_PL_Definition[] =
{
  {
    XPAR_FLASH_FLASH_SPI_0_BASEADDR,
    22000000,
    126,
    d_TRUE,
    NULL,
    NULL
  },
  {
    XPAR_IMU_IMU_SPI_BASEADDR,
    1000000,
    125,
    d_FALSE,
    NULL,
    NULL
  },
#if defined PLATFORM_FCRP || defined PLATFORM_FC200
  {
    XPAR_IOCA_AXIOE_BASEADDR + IOC_SPI_AXI_0_BASEADDR,
    6250000,
    136,
    d_TRUE,
    NULL,
    NULL
  },
  {
    XPAR_IOCB_AXIOE_BASEADDR + IOC_SPI_AXI_0_BASEADDR,
    6250000,
    137,
    d_TRUE,
    NULL,
    NULL
  },
#endif
};

/* Define the number of PL based UARTs */
__attribute__((weak)) d_SPI_PL_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

