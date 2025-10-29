/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_imu_iim20670.h

  Abstract           : IIM20670 device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

#ifndef D_IIM20670_H
#define D_IIM20670_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

#define d_IMU_IIM20670_DIMENSIONS  3u

/***** Type Definitions *************************************************/

/* Returned data structure */
typedef struct
{
  Float32_t gyro[d_IMU_IIM20670_DIMENSIONS];             /* Radians per second */
  Float32_t temperature1;                                /* Degrees C */
  Float32_t accelerometer[d_IMU_IIM20670_DIMENSIONS];    /* Metres per second squared */
  Float32_t temperature2;                                /* Degrees C */
  Float32_t accelerometerLr[d_IMU_IIM20670_DIMENSIONS];  /* Metres per second squared, Low Resolution */
} d_IMU_IIM20670_DataBlock_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialises the IIM20670 CSC */
d_Status_t d_IMU_IIM20670_Initialise(const Uint32_t device);

/* Read the IIM20670 data */
d_Status_t d_IMU_IIM20670_Read(const Uint32_t device, d_IMU_IIM20670_DataBlock_t * pDataBlock);

#endif /* D_IIM20670_H */
