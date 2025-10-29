/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : BME280 SPI interface definition

  Abstract           : This is the BME280 CSC configuration.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "driver/bme280/d_bme280_cfg.h"

/***** Constants ********************************************************/

/* It is assumed all BME280 devices are interfaced using the SPI_PL CSC */
__attribute__((weak)) const d_BME280_Definition_t d_BME280_Config[] =
{
  {4, 0},
};

/* Define the number of BME280 devices */
__attribute__((weak)) d_BME280_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

