/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : IIM20670 interface definition

  Abstract           : This is the IIM20670 CSC configuration.
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

#include "driver/imu_iim20670/d_imu_iim20670_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Uint32_t d_IMU_IIM20670_BaseAddress[] =
{
  0x0000u
};

/* Define the number of IIM20670 devices */
__attribute__((weak)) d_IMU_IIM20670_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

