/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : IMU ADIS16505

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

#ifndef D_IMU_ADIS16505_H
#define D_IMU_ADIS16505_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

#define d_IMU_ADIS16505_DIMENSIONS        3u

/***** Type Definitions *************************************************/

typedef Float32_t d_GyroBias_t[d_IMU_ADIS16505_DIMENSIONS];

typedef enum
{
  IMU_ADIS16505_STATE_INIT = 0,
  IMU_ADIS16505_STATE_BIT,
  IMU_ADIS16505_STATE_BIT_RESULT,
  IMU_ADIS16505_STATE_STREAM,
  IMU_ADIS16505_STATE_MEMORY,
  IMU_ADIS16505_STATE_MEMORY_RESULT,
  IMU_ADIS16505_STATE_WAIT,
  IMU_ADIS16505_STATE_FAIL,
  IMU_ADIS16505_STATE_WAIT_HILS
} d_IMU_Adis16505_State_t;

/* IMU data available */
typedef struct
{
  Bool_t validDevice;                          /* True if device passed initialisation */
  Uint8_t model;                               /* From device */
  Float32_t revision;                          /* From device */
  Char_t date[11];                             /* From device */
  
  Bool_t valid;                                /* True if last message from IMU checksum passed */
  Bool_t stale;                                /* True if the data has not been updated for 20 ms */
  Uint16_t diagStatus;                         /* From device */
  Float32_t gyro[d_IMU_ADIS16505_DIMENSIONS];  /* From device */
  Float32_t accl[d_IMU_ADIS16505_DIMENSIONS];  /* From device */
  Float32_t temperature;                       /* From device */
  Uint16_t timeStamp;                          /* From device */
  Uint32_t sampleTime;                         /* Time last sample received */
  Bool_t initRetry;                            /* True if the Sensor BIT is retried at initialisation */
  Uint16_t bitResult;                          /* The result of the last BIT execution */
  Uint16_t memoryResult;                       /* The result of the last memory update execution */
  Uint16_t bitResultFlash;                     /* The result of the Flash BIT at initialisation */
  Uint16_t bitResultSensor;                    /* The result of the sensor BIT at initialisation */
} d_IMU_Adis16505_Data_t;

typedef enum
{
  IMU_ADIS16505_BIT_SENSOR = 0,
  IMU_ADIS16505_BIT_FLASH,
  IMU_ADIS16505_BIT_COUNT
} d_IMU_Adis16505_Bit_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Power up the IMU device */
d_Status_t d_IMU_Adis16505_PowerOn(void);

/* Power down the IMU device */
d_Status_t d_IMU_Adis16505_PowerOff(void);

/* Initialises the IMU CSC */
d_Status_t d_IMU_Adis16505_Initialise(d_IMU_Adis16505_Data_t * pImuData);

/* Get the IMU state */
d_IMU_Adis16505_State_t d_IMU_Adis16505_State(void);

/* Reset the IMU CSC */
d_Status_t d_IMU_Adis16505_Reset(void);

/* Read the IMU data */
d_Status_t d_IMU_Adis16505_Read(void);

/* Perform BIT functions */
d_Status_t d_IMU_Adis16505_Bit(d_IMU_Adis16505_Bit_t test);

/* Set the gyro bias values */
d_Status_t d_IMU_Adis16505_GyroBiasSet(const d_GyroBias_t bias);

/* Read the gyro bias values */
d_Status_t d_IMU_Adis16505_GyroBiasRead(d_GyroBias_t bias);

/* SPI interrupt handler */
void d_IMU_Adis16505_InterruptHandlerSpi(const Uint32_t channel);

/* Device interrupt handler */
void d_IMU_Adis16505_InterruptHandlerDevice(const Uint32_t channel, const Uint32_t device);

#endif /* D_IMU_ADIS16505_H */
