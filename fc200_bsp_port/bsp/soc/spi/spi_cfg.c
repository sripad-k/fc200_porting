/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : spi_cfg.c

  Abstract           : This is a template for the SPI CSC configuration
                       of the interfaces.
                       The file should be copied to the application code
                       space and updated for the required settings.

  Software Structure : SRS References: Document numbers and versions.
                       SDD References: Document numbers and versions.

*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "soc/spi/d_spi.h"
#include "soc/spi/d_spi_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_SPI_Config_t d_SPI_Config[] =
{
  {
    XPAR_PSU_SPI_0_BASEADDR,         /* Base address */
    XPAR_PSU_SPI_0_SPI_CLK_FREQ_HZ,  /* Clock frequency */
    XPS_SPI0_INT_ID,                 /* Interrupt */
    d_SPI_CLK_INACTIVE_STATE_LOW,    /* Clock inactive state */
    d_SPI_CLK_PHASE_FIRST,           /* Clock phase */
    20000000u,                       /* Baud rate */
    NULL                             /* Callback for event */
  },
  {
    XPAR_PSU_SPI_1_BASEADDR,         /* Base address */
    XPAR_PSU_SPI_1_SPI_CLK_FREQ_HZ,  /* Clock frequency */
    XPS_SPI1_INT_ID,                 /* Interrupt */
    d_SPI_CLK_INACTIVE_STATE_LOW,    /* Clock inactive state */
    d_SPI_CLK_PHASE_FIRST,           /* Clock phase */
    20000000u,                       /* Baud rate */
    NULL                             /* Callback for event */
  }
};

/* Define the number of SPI interfaces */
__attribute__((weak)) d_SPI_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

