/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : IMU IMU ADIS16505 Configuration

  Abstract           : Driver for the Analog Devices ADIS16505 IMU

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

#ifndef D_IMU_ADIS16505_CFG_H
#define D_IMU_ADIS16505_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/discrete/d_discrete.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

extern const Uint32_t d_IMU_Adis16505_SpiChannel;

extern const d_DISC_IO_t d_IMU_Adis16505_DiscretePower;

/* Note that the UART transmit pin is used to reset the device only */
extern const Uint32_t d_IMU_Adis16505_UartChannel;

/***** Function Declarations ********************************************/

#endif /* D_IMU_ADIS16505_CFG_H */
