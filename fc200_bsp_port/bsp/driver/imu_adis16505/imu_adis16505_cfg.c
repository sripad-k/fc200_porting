/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : IMU ADIS16505 Configuration

  Abstract           : Driver configuration for the Analog Devices ADIS16505 IMU

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-756
                       SDD References: 136T-2200-131000-001-D22 SWDES-2433
                                                                SWDES-2434
                                                                SWDES-2435
                                                                SWDES-2436
                                                                SWDES-2437
                                                                SWDES-2438
                                                                SWDES-2439
                                                                SWDES-2440
                                                                SWDES-2441
\note
  CSC ID             : SWDES-2423
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "sru/fcu/d_fcu.h"
#include "driver/imu_adis16505/d_imu_adis16505_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const Uint32_t d_IMU_Adis16505_SpiChannel = 1;

__attribute__((weak)) const d_DISC_IO_t d_IMU_Adis16505_DiscretePower = d_FCU_IMU_PWR_ON;

__attribute__((weak)) const Uint32_t d_IMU_Adis16505_UartChannel = 8;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

