/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_imu_iim20670_cfg.h

  Abstract           : IIM20670 SPI configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

#ifndef D_IIM20670_CFG_H
#define D_IIM20670_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_IMU_IIM20670_MAX_INTERFACES  4u

#define d_IMU_IIM20670_COUNT const Uint32_t d_IMU_IIM20670_Count = (sizeof(d_IMU_IIM20670_BaseAddress) / sizeof(Uint32_t))


/***** Type Definitions *************************************************/

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const Uint32_t d_IMU_IIM20670_BaseAddress[];

extern const Uint32_t d_IMU_IIM20670_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_IIM20670_CFG_H */
