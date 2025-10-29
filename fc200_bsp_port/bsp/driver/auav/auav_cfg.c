/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : AUAV SPI interface definition

  Abstract           : This is the AUAV CSC configuration.
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

//#include "xparameters.h"
#include "driver/auav/d_auav_cfg.h"

/***** Constants ********************************************************/

/* It is assumed all AUAV devices are interfaced using the SPI_PL CSC */
/* Note that the 'excludeCommandResponse' configuration item of the SPI interface must be set to d_FALSE */
__attribute__((weak)) const d_AUAV_Config_t d_AUAV_Config[] =
{
  { /* Sensor 0 */
    {
      {3, 0},
      {3, 1}
    },
    24.884f
  },
//  {
//    {
//      {3, 2},
//      {3, 3}
//    },
//    24.884f
//  },
};

/* Define the number of AUAV devices */
__attribute__((weak)) d_AUAV_COUNT;

__attribute__((weak)) const d_AUAV_IrqHandler callback = NULL;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

