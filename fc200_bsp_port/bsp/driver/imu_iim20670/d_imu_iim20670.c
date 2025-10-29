/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_imu_iim20670.c

  Abstract           : IIM20670 device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_register.h"

#include "xparameters.h"

#include "d_imu_iim20670.h"
#include "d_imu_iim20670_cfg.h"

/***** Constants ********************************************************/

/* IMU register addresses */
#define REGISTER_GYRO_X       0x00u
#define REGISTER_GYRO_Y       0x04u
#define REGISTER_GYRO_Z       0x08u
#define REGISTER_TEMP1        0x0Cu
#define REGISTER_ACCEL_X      0x10u
#define REGISTER_ACCEL_Y      0x14u
#define REGISTER_ACCEL_Z      0x18u
#define REGISTER_TEMP2        0x1Cu
#define REGISTER_ACCEL_X_LR   0x20u
#define REGISTER_ACCEL_Y_LR   0x24u
#define REGISTER_ACCEL_Z_LR   0x28u

#define GYRO_SCALING       (328.0f / (80.0f * 32768.0f))
#define ACCEL_SCALING      (4.096f / (80.0f * 32768.0f))
#define ACCEL_LR_SCALING   (8.192f / (80.0f * 32768.0f))
#define TEMP_SCALING       (20.0f)
#define TEMP_OFFSET        (25.0f)

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float32_t G = 9.8f;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float32_t PI = 3.1415926536f;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised[d_IMU_IIM20670_MAX_INTERFACES] = {d_FALSE, d_FALSE, d_FALSE, d_FALSE};

/***** Function Declarations ********************************************/

/* Read a register */
static Float32_t RegisterRead(const Uint32_t device, const Uint32_t address);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_IIM20670_Initialise -->

  Initialises the IIM20670 CSC.
*************************************************************************/
d_Status_t                        /** \return function status */
d_IMU_IIM20670_Initialise
(
const Uint32_t device
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  /* Check valid device number */
  if (device >= d_IMU_IIM20670_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[device] != d_TRUE)
  {
    /* Check number of devices is valid */
    if (d_IMU_IIM20670_Count > d_IMU_IIM20670_MAX_INTERFACES)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, d_IMU_IIM20670_Count, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }

  }

  initialised[device] = d_TRUE;

  return status;
}

/*********************************************************************//**
  <!-- d_IIM20670_Read -->

  Read the IIM20670 data.
*************************************************************************/
d_Status_t
d_IMU_IIM20670_Read
(
Uint32_t device,
d_IMU_IIM20670_DataBlock_t * pDataBlock
)
{
  Float32_t rawValue;
  
  if (device >= d_IMU_IIM20670_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[device] != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (pDataBlock == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  for (Uint32_t index = 0; index < d_IMU_IIM20670_DIMENSIONS; index++)
  {
    rawValue = RegisterRead(device, REGISTER_GYRO_X + (index * 4u));
    /* Get value in radians per second */
    pDataBlock->gyro[index] = rawValue * GYRO_SCALING * PI / 180.0f;
  }
  
  for (Uint32_t index = 0; index < d_IMU_IIM20670_DIMENSIONS; index++)
  {
    rawValue = RegisterRead(device, REGISTER_ACCEL_X + (index * 4u));
    /* Get value in metres per second squared */
    pDataBlock->accelerometer[index] = rawValue * ACCEL_SCALING * G;
  }

  for (Uint32_t index = 0; index < d_IMU_IIM20670_DIMENSIONS; index++)
  {
    rawValue = RegisterRead(device, REGISTER_ACCEL_X_LR + (index * 4u));
    /* Get value in metres per second squared */
    pDataBlock->accelerometerLr[index] = rawValue * ACCEL_LR_SCALING * G;
  }

  rawValue = RegisterRead(device, REGISTER_TEMP1);
  pDataBlock->temperature1 = (rawValue / TEMP_SCALING) + TEMP_OFFSET;
  
  rawValue = RegisterRead(device, REGISTER_TEMP2);
  pDataBlock->temperature2 = (rawValue / TEMP_SCALING) + TEMP_OFFSET;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- RegisterRead -->

  Local read a register.
*************************************************************************/
static Float32_t
RegisterRead
(
const Uint32_t device,
const Uint32_t address
)
{
#if defined PLATFORM_FC100
  Int32_t rawValue = (Int32_t)d_GEN_RegisterRead(XPAR_IIM20670_BASEADDR + d_IMU_IIM20670_BaseAddress[device] + address);
#else
  UNUSED_PARAMETER(device);
  UNUSED_PARAMETER(address);
  Int32_t rawValue = 0;
#endif
  
  return (Float32_t)rawValue;
}

