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

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "soc/timer/d_timer.h"
#include "soc/uart/d_uart.h"
#include "soc/interrupt_manager/d_INT_Critical.h"
#include "sru/fcu/d_fcu.h"
#include "sru/spi_pl/d_spi_pl.h"
#include "sru/spi_pl/d_spi_pl_cfg.h"

#include "d_imu_adis16505.h"
#include "d_imu_adis16505_cfg.h"

/***** Constants ********************************************************/

static const Uint32_t HILS_UART = 8u;

/* Device number on SPI interface */
static const Uint32_t IMU_DEVICE = 0u;

/* Data invalid if older than 20000 us */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t STALE_TIMEOUT = 20000u;

static const Uint32_t SPI_TRANSACTION_TIMEOUT_USEC      = 2000u; /* Timeout period for SPI transaction to complete */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SPI_BUSY_WAIT                     = 2000u;

/* Maximum number of retries of sensor test at startup */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t MAX_TRIES = 2;

/* Timeout waiting for response from HILS IMU, in microseconds. This is large to handle the relatively slow response from the test PC simulator */
static const Uint32_t HILS_MAX_TIMEOUT = 250000;

/* IMU register addresses */
#define REG_DIAG_STAT     0x02u
#define REG_XG_BIAS_LOW   0x40u
#define REG_XG_BIAS_HIGH  0x41u
#define REG_YG_BIAS_LOW   0x42u
#define REG_YG_BIAS_HIGH  0x43u
#define REG_ZG_BIAS_LOW   0x44u
#define REG_ZG_BIAS_HIGH  0x45u
#define REG_FILT_CTRL     0x5Cu
#define REG_RANG_MDL      0x5Eu
#define REG_MSC_CTRL      0x60u
#define REG_UP_SCALE      0x62u
#define REG_DEC_RATE      0x64u
#define REG_GLOB_CMD      0x68u
#define REG_FIRM_REV      0x6Cu
#define REG_FIRM_DM       0x6Eu
#define REG_FIRM_Y        0x70u
#define REG_PROD_ID       0x72u

/* DIAG_STAT register bit definitions */
#define REG_DIAG_STAT_DATA_OVERRUN    0x0002u
#define REG_DIAG_STAT_SPI_ERROR       0x0008u
#define REG_DIAG_STAT_STANDBY_MODE    0x0010u
#define REG_DIAG_STAT_SENSOR_FAILURE  0x0020u
#define REG_DIAG_STAT_MEMORY_FAILURE  0x0040u
#define REG_DIAG_STAT_CLOCK_ERROR     0x0080u
#define REG_DIAG_STAT_GYRO1_FAILURE   0x0100u
#define REG_DIAG_STAT_GYRO2_FAILURE   0x0200u
#define REG_DIAG_STAT_ACCEL_FAILURE   0x0400u

/* MSC_CTRL register bit definitions */
#define REG_MSC_CTRL_DR_POLARITY      0x0001u
#define REG_MSC_CTRL_SYNC_POLARITY    0x0002u
#define REG_MSC_CTRL_SYNC_MODE_INT    0x0000u
#define REG_MSC_CTRL_SYNC_MODE_DIRECT 0x0004u
#define REG_MSC_CTRL_SYNC_MODE_SCALED 0x0008u
#define REG_MSC_CTRL_SYNC_MODE_OUTPUT 0x000Cu
#define REG_MSC_CTRL_SENS_BW          0x0010u
#define REG_MSC_CTRL_PERCUSSION       0x0040u
#define REG_MSC_CTRL_LINEAR_ACC_COMP  0x0080u
#define REG_MSC_CTRL_BURST_SEL        0x0100u
#define REG_MSC_CTRL_BURST32          0x0200u

#define GLOB_CMD_SOFTWARE_RESET         0x80u
#define GLOB_CMD_FLASH_MEMORY_TEST      0x10u
#define GLOB_CMD_FLASH_MEMORY_UPDATE    0x08u
#define GLOB_CMD_SENSOR_SELF_TEST       0x04u

/* Expected value of product identification register */
#define PRODUCT_ID                    0x4079u

/* Expected value of range model register */
#define RANGE_MODEL                   0x0007u

/* The number of bytes received in burst, excluding the command which also gives response data */
#define BURST_BYTES                   34u

#define RESOLUTION_GYRO     ((0.025 * 3.1415926536 / 180.0) / 65536.0)
#define RESOLUTION_ACCL     ((78.3 / 32000) / 65536.0)

/* Maximum reset time in milliseconds */
#define RESET_TIME          1000u

/* Minimum time to execute BIT function in microseconds */
#define MINIMUM_BIT_TIME    20000u

/* Minimum time to execute BIT function in microseconds */
#define MINIMUM_MEMORY_TIME    50000u

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static d_IMU_Adis16505_Data_t imuData;

static d_IMU_Adis16505_State_t state = d_SPI_PL_STATUS_INIT;
static d_IMU_Adis16505_Data_t * pAppData = NULL;

static Bool_t initialised = d_FALSE;
static Bool_t spiBurstInProgress = d_FALSE;

static Uint32_t bitStartTime;
static Uint32_t memoryStartTime;

static Bool_t hilsBusy = d_FALSE;
static Uint32_t hilsBurstRequestTime;
/***** Function Declarations ********************************************/

static void deviceInterruptHandler(Bool_t hils);
static void deviceInterruptBit(void);
static void deviceInterruptStream(Bool_t hils);
static Bool_t parseBurstMessage(const Uint8_t * const pResponse);
static d_Status_t deviceSetup(void);
static d_Status_t deviceBit(void);
static d_Status_t registerRead(const Uint32_t channel, Uint16_t * const pValue);
static d_Status_t registerReadImu(const Uint32_t channel, Uint16_t * const pValue);
static d_Status_t registerWrite(const Uint32_t channel, const Uint16_t value);
static d_Status_t registerWriteImu(const Uint32_t channel, const Uint16_t value);
static Uint16_t extractFromBuffer(const Uint8_t * const buffer);
static Uint32_t bcd2(const Uint8_t value);
static Uint32_t bcd4(const Uint16_t value);

static d_Status_t registerReadHils(const Uint32_t address, Uint16_t * const pValue);
static d_Status_t registerWriteHils(const Uint32_t channel, const Uint16_t value);
static void burstReadHils(void);
static void burstReadHilsResponse(void);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_IMU_Adis16505_PowerOn -->

  Power up the IMU device.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_PowerOn
(
void
)
{
  d_Status_t status;

  /* Ensure device not reset */
  status = d_UART_Configure(HILS_UART, 115200, d_UART_DATA_BITS_8, d_UART_PARITY_NONE, d_UART_STOP_BITS_1);

  if (status == d_STATUS_SUCCESS)
  {
    /* Switch on IMU power */
    status = d_DISC_SetAsOutputPin(d_IMU_Adis16505_DiscretePower);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    (void)d_DISC_SetPin(d_IMU_Adis16505_DiscretePower);  /* Set output high to turn on power */
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_PowerOff -->

  Power down the IMU device.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_PowerOff
(
void
)
{
  d_Status_t status;

  /* Ensure device not reset */
  status = d_UART_Configure(HILS_UART, 115200, d_UART_DATA_BITS_8, d_UART_PARITY_NONE, d_UART_STOP_BITS_1);

  if (status == d_STATUS_SUCCESS)
  {
    /* Switch on IMU power */
    status = d_DISC_SetAsOutputPin(d_IMU_Adis16505_DiscretePower);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    (void)d_DISC_ClearPin(d_IMU_Adis16505_DiscretePower);  /* Set output low to turn off power */
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_Initialise -->

  Initialises the ADIS16505 IMU CSC.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_Initialise
(
d_IMU_Adis16505_Data_t * pImuData /**< [in]  Pointer to storage for IMU data */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint16_t regValue = 0u;

  initialised = d_FALSE;
  state = IMU_ADIS16505_STATE_INIT;

  /* Initialise data structure */
  imuData.validDevice = d_FALSE;
  imuData.valid = d_FALSE;
  imuData.stale = d_FALSE;
  imuData.initRetry = d_FALSE;

  /* Check parameter */
  if (pImuData == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Check configured SPI interface number is valid */
  if (d_IMU_Adis16505_SpiChannel >= d_SPI_PL_Count)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, d_IMU_Adis16505_SpiChannel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  /* Save pointer to application structure */
  pAppData = pImuData;

  d_TIMER_Initialise();

  if (d_FCU_HilsActive() != d_TRUE)
  {
    status = d_SPI_PL_Initialise(d_IMU_Adis16505_SpiChannel);
  }

  if (status == d_STATUS_SUCCESS)
  {
    /* Read the product ID to confirm communications */
    status = registerRead(REG_PROD_ID, &regValue);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    if (regValue == PRODUCT_ID)
    {
      status = d_IMU_Adis16505_Reset();

      /* Delay to ensure reset has commenced */
      (void)d_TIMER_DelayMilliseconds(5);

      /* Allow time for device to start up. Typical 255ms */
      Uint32_t startTime = d_TIMER_ReadValueInTicks();
      Uint32_t elapsedTime;
      regValue = 0xFFFFu;
      do
      {
        (void)registerRead(REG_PROD_ID, &regValue);
        elapsedTime = d_TIMER_ElapsedMilliseconds(startTime, NULL);
      } while ((regValue != PRODUCT_ID) && (elapsedTime < RESET_TIME));

      if (elapsedTime >= RESET_TIME)
      {
        // gcov-jst 1 It is not practical to generate this failure during bench testing.
        status = d_STATUS_DEVICE_NOT_READY;
      }
      ELSE_DO_NOTHING
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_DEVICE_NOT_FOUND;
    }
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    status = deviceSetup();
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    status = deviceBit();
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  /* Copy the data read so far to the application structure */
  (void)d_IMU_Adis16505_Read();

  if (status == d_STATUS_SUCCESS)
  {
    initialised = d_TRUE;
    imuData.validDevice = d_TRUE;
    state = IMU_ADIS16505_STATE_STREAM;
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_State -->

  Get the IMU state.
*************************************************************************/
d_IMU_Adis16505_State_t           /** \return IMU state */
d_IMU_Adis16505_State
(
void
)
{
  return state;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_Reset -->

  Reset the ADIS16505 IMU CSC.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_Reset
(
void
)
{
  d_Status_t status;

  /* Stop burst reading */
  state = IMU_ADIS16505_STATE_INIT;

  /* Delay in case burst read is in progress */
  (void)d_TIMER_DelayMicroseconds(500u);

  /* Send software reset command */
  status = registerWrite(REG_GLOB_CMD, GLOB_CMD_SOFTWARE_RESET);

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_Read -->

  Read the ADIS16505 IMU data.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_Read
(
void
)
{
  /* Make sure initialisation function has been called */
  if (pAppData == NULL)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  Uint32_t timeNow = d_TIMER_ReadValueInTicks();

  Uint32_t age = timeNow - imuData.sampleTime;
  if (age > STALE_TIMEOUT)
  {
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    imuData.stale = d_TRUE;
  }

  Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
  *pAppData = imuData;
  d_INT_CriticalSectionLeave(interruptFlags);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_Bit -->

  Perform BIT functions.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_Bit
(
d_IMU_Adis16505_Bit_t test        /**< [in]  BIT to perform */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint16_t command;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (test >= IMU_ADIS16505_BIT_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, test, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((state != IMU_ADIS16505_STATE_STREAM) && (state != IMU_ADIS16505_STATE_WAIT_HILS))
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_DEVICE_BUSY;
  }

  if (test == IMU_ADIS16505_BIT_SENSOR)
  {
    /* Sensor self test */
    command = GLOB_CMD_SENSOR_SELF_TEST;
  }
  else
  {
    /* Flash memory test */
    command = GLOB_CMD_FLASH_MEMORY_TEST;
  }

  (void)d_TIMER_ElapsedMicroseconds(0, &bitStartTime);

  /* Select BIT state so does not attempt to perform a burst read */
  state = IMU_ADIS16505_STATE_BIT;

  /* Delay in case burst read is in progress */
  (void)d_TIMER_DelayMicroseconds(500u);

  status = registerWrite(REG_GLOB_CMD, command);

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_GyroBias -->

  Set the gyro bias values.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_GyroBiasSet
(
const d_GyroBias_t bias           /**< [in] Gyro bias values */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if ((state != IMU_ADIS16505_STATE_STREAM) && (state != IMU_ADIS16505_STATE_WAIT_HILS))
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_DEVICE_BUSY;
  }

  for (Uint32_t index = 0; index < d_IMU_ADIS16505_DIMENSIONS; index++)
  {
    if (((bias[index] / (Float32_t)RESOLUTION_GYRO) > 1310720000.0f) ||
        ((bias[index] / (Float32_t)RESOLUTION_GYRO) < -1310720000.0f))
    {
      d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, index, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_PARAMETER;
    }
  }

  /* Select BIT state so does not attempt to perform a burst read */
  state = IMU_ADIS16505_STATE_WAIT;

  /* Delay in case burst read is in progress */
  (void)d_TIMER_DelayMicroseconds(500u);

  for (Uint32_t index = 0; index < d_IMU_ADIS16505_DIMENSIONS; index++)
  {
    // clang-tidy-jst bugprone-narrowing-conversions 1 Values are checked with limits at start of the function.
    Int32_t intBias = bias[index] / (Float32_t)RESOLUTION_GYRO;

    Uint32_t reg = REG_XG_BIAS_LOW + (index * 4u);
    (void)registerWrite(reg, (Uint32_t)intBias & 0xFFFFu);
    (void)registerWrite(reg + 2u, ((Uint32_t)intBias >> 16u) & 0xFFFFu);
  }

  (void)d_TIMER_ElapsedMicroseconds(0, &memoryStartTime);
  state = IMU_ADIS16505_STATE_MEMORY;
  (void)registerWrite(REG_GLOB_CMD, GLOB_CMD_FLASH_MEMORY_UPDATE);

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_GyroBias -->

  Set the gyro bias values.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_IMU_Adis16505_GyroBiasRead
(
d_GyroBias_t bias                 /**< [out] Gyro bias values */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if ((state != IMU_ADIS16505_STATE_STREAM) && (state != IMU_ADIS16505_STATE_WAIT_HILS))
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_DEVICE_BUSY;
  }

  /* Select MEMORY state so does not attempt to perform a burst read */
  state = IMU_ADIS16505_STATE_WAIT;

  /* Delay in case burst read is in progress */
  (void)d_TIMER_DelayMicroseconds(500u);

  for (Uint32_t index = 0; index < d_IMU_ADIS16505_DIMENSIONS; index++)
  {
    Uint16_t low = 0;
    Uint16_t high = 0;
    Uint32_t value;
    Float32_t fvalue;

    Uint32_t reg = REG_XG_BIAS_LOW + (index * 4u);

    (void)registerRead(reg, &low);
    (void)registerRead(reg + 2u, &high);

    value = ((Uint32_t)high << 16u) | (Uint32_t)low;

    fvalue = (Float32_t)((Int32_t)value);

    bias[index] = fvalue * (Float32_t)RESOLUTION_GYRO;
  }
  state = IMU_ADIS16505_STATE_STREAM;

  return status;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_InterruptHandlerDevice -->

  Device interrupt handler.
*************************************************************************/
void                              /** \return None */
d_IMU_Adis16505_InterruptHandlerDevice
(
const Uint32_t channel,           /**< [in]  Channel number */
const Uint32_t device             /**< [in]  Device number */
)
{
  UNUSED_PARAMETER(channel);
  UNUSED_PARAMETER(device);

  Bool_t hils = d_FCU_HilsActive();
  if ((initialised == d_TRUE) && ((hils != d_TRUE) || (hilsBusy == d_FALSE)))
  {
    hilsBusy = d_TRUE; /* Nate that this will be set if not HILS operation but has no impact */
    deviceInterruptHandler(hils);
    hilsBusy = d_FALSE;
  }
  ELSE_DO_NOTHING

  return;
}

/*********************************************************************//**
  <!-- d_IMU_Adis16505_InterruptHandlerSpi -->

  SPI interrupt handler. Transfer complete.
*************************************************************************/
void                              /** \return None */
d_IMU_Adis16505_InterruptHandlerSpi
(
const Uint32_t channel            /**< [in]  Channel number */
)
{
  UNUSED_PARAMETER(channel);

  if (spiBurstInProgress == d_TRUE)
  {
    d_SPI_PL_State_t spiStatus;
    Uint32_t rxCount;
    Uint8_t response[BURST_BYTES + 2u];
    Bool_t dataValid = d_FALSE;

    (void)d_SPI_PL_Result(d_IMU_Adis16505_SpiChannel, &spiStatus, &response[0], BURST_BYTES + 2u, &rxCount);
    if (rxCount == (BURST_BYTES + 2u))
    {
      dataValid = parseBurstMessage(&response[2]);
      spiBurstInProgress = d_FALSE;
    }
    imuData.valid = dataValid;
  }
  ELSE_DO_NOTHING

  return;
}

/*********************************************************************//**
  <!-- deviceInterruptHandler -->

  Device interrupt handler.
*************************************************************************/
static void                       /** \return None */
deviceInterruptHandler
(
Bool_t hils                       /**< [in] HILS operation = d_TRUE */
)
{
  /* If not HILS then make sure not stuck in HILS wait state */
  if ((hils == d_FALSE) && (state > IMU_ADIS16505_STATE_FAIL))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    state = IMU_ADIS16505_STATE_STREAM;
  }
  deviceInterruptBit();
  deviceInterruptStream(hils);

  return;
}

/*********************************************************************//**
  <!-- deviceInterruptBit -->

  Device interrupt handler.
*************************************************************************/
static void                       /** \return None */
deviceInterruptBit
(
void
)
{
  /* Check if BIT complete */
  if (state == IMU_ADIS16505_STATE_BIT)
  {
    Uint32_t elapsedTime = d_TIMER_ElapsedMicroseconds(bitStartTime, NULL);
    if (elapsedTime > MINIMUM_BIT_TIME)
    {
      (void)registerRead(REG_DIAG_STAT, &imuData.bitResult);
      state = IMU_ADIS16505_STATE_BIT_RESULT;
    }
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    ELSE_DO_NOTHING
  }
  /* Check saving memory is complete */
  else if (state == IMU_ADIS16505_STATE_MEMORY)
  {
    Uint32_t elapsedTime = d_TIMER_ElapsedMicroseconds(memoryStartTime, NULL);
    if (elapsedTime > MINIMUM_MEMORY_TIME)
    {
      (void)registerRead(REG_DIAG_STAT, &imuData.memoryResult);
      state = IMU_ADIS16505_STATE_MEMORY_RESULT;
    }
    // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
    ELSE_DO_NOTHING
  }
  else if ((state == IMU_ADIS16505_STATE_BIT_RESULT) || (state == IMU_ADIS16505_STATE_MEMORY_RESULT))
  {
    state = IMU_ADIS16505_STATE_STREAM;
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- deviceInterruptStream -->

  Device interrupt handler.
*************************************************************************/
static void                       /** \return None */
deviceInterruptStream
(
Bool_t hils                       /**< [in] HILS operation = d_TRUE */
)
{
  if (state == IMU_ADIS16505_STATE_STREAM)
  {
    /* Request burst transfer */
    if (hils == d_TRUE)
    {
      burstReadHils();
    }
    else
    {
      Uint8_t command[2];
      command[0] = 0x68u;
      command[1] = 0x00u;

      d_Status_t status = d_SPI_PL_Transfer(d_IMU_Adis16505_SpiChannel, IMU_DEVICE, &command[0], 2u, NULL, BURST_BYTES);
      if (status == d_STATUS_SUCCESS)
      {
        spiBurstInProgress = d_TRUE;
      }
      else
      {
        // gcov-jst 1 It is not practical to generate this failure during bench testing.
        DO_NOTHING();
      }
    }
  }
  else if (state == IMU_ADIS16505_STATE_WAIT_HILS)
  {
    if (hils == d_TRUE)
    {
      burstReadHilsResponse();
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- parseBurstMessage -->

  Extract data from a burst message.
*************************************************************************/
static Bool_t
parseBurstMessage
(
const Uint8_t * const pResponse
)
{
  Bool_t returnValue = d_FALSE;

  Uint16_t checksum = extractFromBuffer(&pResponse[BURST_BYTES - 4u]);
  Uint16_t calcChecksum = 0u;
  for (Uint32_t index = 0u; index < (BURST_BYTES - 4u); index++)
  {
    calcChecksum = calcChecksum + pResponse[index];
  }
  if (checksum == calcChecksum)
  {
    Uint16_t itemp;
    imuData.diagStatus = extractFromBuffer(&pResponse[0]);
    for (Uint32_t index = 0; index < d_IMU_ADIS16505_DIMENSIONS; index++)
    {
      Uint16_t low   = extractFromBuffer(&pResponse[2u + (index * 4u)]);
      itemp = extractFromBuffer(&pResponse[4u + (index * 4u)]);
      Uint32_t measurement = ((Uint32_t)itemp << 16u) | low;
      imuData.gyro[index] = (Float32_t)((Int32_t)measurement) * (Float32_t)RESOLUTION_GYRO;
      low   = extractFromBuffer(&pResponse[14u + (index * 4u)]);
      itemp = extractFromBuffer(&pResponse[16u + (index * 4u)]);
      measurement = ((Uint32_t)itemp << 16u) | low;
      imuData.accl[index] = (Float32_t)((Int32_t)measurement) * (Float32_t)RESOLUTION_ACCL;
    }
    itemp = extractFromBuffer(&pResponse[26]);
    imuData.temperature = (Float32_t)((Int16_t)itemp) / 10.0f;

    imuData.timeStamp = extractFromBuffer(&pResponse[28]);
    imuData.sampleTime = d_TIMER_ReadValueInTicks();
    returnValue = d_TRUE;
    imuData.stale = d_FALSE;
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- deviceSetup -->

  Collect status information and setup the IMU.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
deviceSetup
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint16_t regValue = 0x0000u;
  Uint16_t rangMdlValue = 0x0000u;

  /* Read version information */
  (void)registerRead(REG_FIRM_REV, &regValue);
  imuData.revision = (Float32_t)bcd4(regValue) / 100.0f;
  (void)registerRead(REG_FIRM_DM, &regValue);
  imuData.date[4] = '/';
  imuData.date[5] = (Char_t)('0' + ((Uint16_t)(regValue >> 12u) & 0x0Fu));
  imuData.date[6] = (Char_t)('0' + ((Uint16_t)(regValue >> 8u) & 0x0Fu));
  imuData.date[7] = '/';
  imuData.date[8] = (Char_t)('0' + ((Uint16_t)(regValue >> 4u) & 0x0Fu));
  imuData.date[9] = (Char_t)('0' + (regValue & 0x0Fu));
  imuData.date[10] = 0;
  (void)registerRead(REG_FIRM_Y, &regValue);
  imuData.date[0] = (Char_t)('0' + ((Uint16_t)(regValue >> 12u) & 0x0Fu));
  imuData.date[1] = (Char_t)('0' + ((Uint16_t)(regValue >> 8u) & 0x0Fu));
  imuData.date[2] = (Char_t)('0' + ((Uint16_t)(regValue >> 4u) & 0x0Fu));
  imuData.date[3] = (Char_t)('0' + (regValue & 0x0Fu));

  /* Check the range of the device is correct */
  (void)registerRead(REG_RANG_MDL, &rangMdlValue);
  imuData.model = rangMdlValue;

  if (rangMdlValue != (Uint16_t)RANGE_MODEL)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_DEVICE_ERROR;
  }
  else
  {
    DO_NOTHING();
  }

  if (status == d_STATUS_SUCCESS)
  {
    (void)registerWrite(REG_MSC_CTRL, REG_MSC_CTRL_SYNC_MODE_SCALED |
                                      REG_MSC_CTRL_SENS_BW |
                                      REG_MSC_CTRL_DR_POLARITY |
                                      REG_MSC_CTRL_BURST32);
    /* Sample frequency = external sync (50Hz) * REG_UP_SCALE (40) = 2000Hz */
    (void)registerWrite(REG_UP_SCALE, 40u);

    /* No filtering */
    (void)registerWrite(REG_FILT_CTRL, 0u);

    /* Decimation for 100Hz sample output */
    (void)registerWrite(REG_DEC_RATE, 19u);

    /* Set to streaming so data will be received when available */
    state = IMU_ADIS16505_STATE_STREAM;

    (void)d_TIMER_DelayMilliseconds(300u);

    /* Read the diagnostic status and confirm no errors */
    /* Clock error indicates at startup but cleared operationally */
    (void)registerRead(REG_DIAG_STAT, &imuData.diagStatus);
    if ((imuData.diagStatus & (REG_DIAG_STAT_DATA_OVERRUN |
                               REG_DIAG_STAT_SPI_ERROR |
                               REG_DIAG_STAT_STANDBY_MODE)) != 0u)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      status = d_STATUS_DEVICE_ERROR;
    }
    else
    {
      DO_NOTHING();
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  imuData.diagStatus = 0xFFFFu;

  return status;
}

/*********************************************************************//**
  <!-- deviceBit -->

  Perform startup BIT.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
deviceBit
(
void
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint32_t startTime;
  Uint32_t elapsedTime;

  /* Set the initialised flag so the BIT functions can be executed */
  initialised = d_TRUE;

  /* Perform BIT */
  (void)d_TIMER_ElapsedMilliseconds(0, &startTime);
  (void)d_IMU_Adis16505_Bit(IMU_ADIS16505_BIT_FLASH);
  do
  {
    state = d_IMU_Adis16505_State();
    elapsedTime = d_TIMER_ElapsedMilliseconds(startTime, NULL);
  } while ((state == IMU_ADIS16505_STATE_BIT) && (elapsedTime < 1000u));
  imuData.bitResultFlash = imuData.bitResult;

  if ((elapsedTime >= 1000u) || ((imuData.bitResult & REG_DIAG_STAT_MEMORY_FAILURE) != 0u))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_DEVICE_ERROR;
  }
  else
  {
    /* Delay to ensure back in Stream state */
    (void)d_TIMER_DelayMilliseconds(20u);

    imuData.diagStatus = 0xFFFFu;
    Uint32_t tries = 0;
    do
    {
      status = d_STATUS_SUCCESS;
      (void)d_TIMER_ElapsedMilliseconds(0, &startTime);
      (void)d_IMU_Adis16505_Bit(IMU_ADIS16505_BIT_SENSOR);
      do
      {
        state = d_IMU_Adis16505_State();
        elapsedTime = d_TIMER_ElapsedMilliseconds(startTime, NULL);
      } while ((state == IMU_ADIS16505_STATE_BIT) && (elapsedTime < 1000u));
      imuData.bitResultSensor = imuData.bitResult;

      if ((elapsedTime >= 1000u) || (imuData.bitResult & (REG_DIAG_STAT_SENSOR_FAILURE |
                                                         REG_DIAG_STAT_GYRO1_FAILURE |
                                                         REG_DIAG_STAT_GYRO2_FAILURE |
                                                         REG_DIAG_STAT_ACCEL_FAILURE)) != 0u)
      {
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        status = d_STATUS_DEVICE_ERROR;
        imuData.initRetry = d_TRUE;
      }
      tries++;
    }
    while ((tries < MAX_TRIES) && (status != d_STATUS_SUCCESS));

  }

  /* Restore initialised flag. It will be set in the initialise function if all tests pass */
  initialised = d_FALSE;

  return status;
}

/*********************************************************************//**
  <!-- registerRead -->

  Read register value
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
registerRead
(
const Uint32_t address,           /**< [in]  Register address */
Uint16_t * const pValue           /**< [out] Pointer to storage for register value */
)
{
  d_Status_t status;

  if (d_FCU_HilsActive() == d_TRUE)
  {
    status = registerReadHils(address, pValue);
  }
  else
  {
    status = registerReadImu(address, pValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- registerReadImu -->

  Read register value
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
registerReadImu
(
const Uint32_t address,           /**< [in]  Register address */
Uint16_t * const pValue           /**< [out] Pointer to storage for register value */
)
{
  d_Status_t status;

  d_SPI_PL_State_t spiStatus;
  Uint32_t rxCount = 0;

  Uint8_t command[2];
  Uint8_t response[2];

  command[0] = address;
  command[1] = 0x00u;

  status = d_SPI_PL_Transfer(d_IMU_Adis16505_SpiChannel, IMU_DEVICE, &command[0], 2u, NULL, 0);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime;
    Uint32_t elapsedTime;

    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
    do
    {
      status = d_SPI_PL_Status(d_IMU_Adis16505_SpiChannel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < SPI_TRANSACTION_TIMEOUT_USEC));

    status = d_SPI_PL_Result(d_IMU_Adis16505_SpiChannel, &spiStatus, NULL, 0, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    /* Stall gap */
    (void)d_TIMER_DelayMicroseconds(20);

    status = d_SPI_PL_Transfer(d_IMU_Adis16505_SpiChannel, IMU_DEVICE, &command[0], 2u, NULL, 0);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime;
    Uint32_t elapsedTime;

    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
    do
    {
      status = d_SPI_PL_Status(d_IMU_Adis16505_SpiChannel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < SPI_TRANSACTION_TIMEOUT_USEC));

    status = d_SPI_PL_Result(d_IMU_Adis16505_SpiChannel, &spiStatus, &response[0], 2, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if ((status == d_STATUS_SUCCESS) && (rxCount == 2u))
  {
    *pValue = (Uint16_t)((Uint16_t)response[0] << 8u) | response[1];
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }

  return status;
}

/*********************************************************************//**
  <!-- registerWrite -->

  Write register value.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
registerWrite
(
const Uint32_t address,           /**< [in]  Register address */
const Uint16_t value              /**< [in]  Value to write */
)
{
  d_Status_t status;

  if (d_FCU_HilsActive() == d_TRUE)
  {
    status = registerWriteHils(address, value);
  }
  else
  {
    status = registerWriteImu(address, value);
  }

  return status;
}

/*********************************************************************//**
  <!-- registerWriteImu -->

  Write register value.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
registerWriteImu
(
const Uint32_t address,           /**< [in]  Register address */
const Uint16_t value              /**< [in]  Value to write */
)
{
  d_Status_t status;

  d_SPI_PL_State_t spiStatus;
  Uint32_t rxCount = 0;
  Uint32_t startTime;
  Uint32_t elapsedTime;

  Uint8_t command[2];

  command[0] = address | 0X80U;
  command[1] = value & 0xFFu;

  (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
  do
  {
    status = d_SPI_PL_Status(d_IMU_Adis16505_SpiChannel, &spiStatus);
    elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
  } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_READY) && (elapsedTime < SPI_BUSY_WAIT));

  if (status == d_STATUS_SUCCESS)
  {
    /* Additional stall gap */
    (void)d_TIMER_DelayMicroseconds(40);
    status = d_SPI_PL_Transfer(d_IMU_Adis16505_SpiChannel, IMU_DEVICE, &command[0], 2u, NULL, 0);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
    do
    {
      status = d_SPI_PL_Status(d_IMU_Adis16505_SpiChannel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < SPI_TRANSACTION_TIMEOUT_USEC));

    status = d_SPI_PL_Result(d_IMU_Adis16505_SpiChannel, &spiStatus, NULL, 0, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    /* Stall gap */
    (void)d_TIMER_DelayMicroseconds(20);

    command[0] = command[0] + 1U;
    command[1] = value >> 8U;

    status = d_SPI_PL_Transfer(d_IMU_Adis16505_SpiChannel, IMU_DEVICE, &command[0], 2u, NULL, 0);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (status == d_STATUS_SUCCESS)
  {
    startTime = d_TIMER_ReadValueInTicks();
    do
    {
      status = d_SPI_PL_Status(d_IMU_Adis16505_SpiChannel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < SPI_TRANSACTION_TIMEOUT_USEC));

    status = d_SPI_PL_Result(d_IMU_Adis16505_SpiChannel, &spiStatus, NULL, 0, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- extractFromBuffer -->

  Extract a big endian value from a buffer.
*************************************************************************/
static Uint16_t                   /** \return Big endian value */
extractFromBuffer
(
const Uint8_t * const buffer      /**< [in]  Pointer to buffer */
)
{
  return (Uint16_t)((Uint16_t)buffer[0u] << 8u) | buffer[1u];
}

/*********************************************************************//**
  <!-- bcd2 -->

  Convert a two nibble BCD to integer.
*************************************************************************/
static Uint32_t                   /** \return Integer value */
bcd2
(
const Uint8_t value               /**< [in]  BCD value */
)
{
  return (value & 0x0fu) + ((value >> 4u) * 10u);
}

/*********************************************************************//**
  <!-- bcd4 -->

  Convert a four nibble BCD to integer.
*************************************************************************/
static Uint32_t                   /** \return Integer value */
bcd4
(
const Uint16_t value              /**< [in]  BCD value */
)
{
  return (bcd2(value >> 8u) * 100u) + bcd2(value & 0xFFu);
}

/*********************************************************************//**
  <!-- imu_hils_wait_response -->

  Wait for a response from the HILS IMU simulator.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
imu_hils_wait_response
(
const Uint32_t length,            /**< [in]  Length of expected response message */
Uint8_t * const pResponse,        /**< [out] Buffer to store the response message */
const Uint32_t responseLength     /**< [in]  Length of the response buffer */
)
{
  d_Status_t returnValue = d_STATUS_TIMEOUT;
  Uint32_t startTime = d_TIMER_ReadValueInTicks();
  Uint32_t received = 0;
  Uint32_t elapsedTime = 0;
  do
  {
    elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    (void)d_UART_Receive(HILS_UART, pResponse, responseLength, &received);
    if (received == length)
    {
      returnValue = d_STATUS_SUCCESS;
    }
  } while ((returnValue == d_STATUS_TIMEOUT) && (elapsedTime < HILS_MAX_TIMEOUT));

  return returnValue;
}

/*********************************************************************//**
  <!-- registerReadHils -->

  Read a register from the HILS IMU simulator.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
registerReadHils
(
const Uint32_t address,           /**< [in]  Register address */
Uint16_t * const pValue           /**< [out] Pointer to storage for register value */
)
{
  hilsBusy = d_TRUE;
  Uint8_t command[5];
  Uint8_t response[5];
  command[0] = 0x73u;
  command[1] = 0xCCu;
  command[2] = 0x02u;
  command[3] = 0x00u;
  command[4] = (Uint8_t)address;
  (void)d_UART_FlushRx(HILS_UART);
  (void)d_UART_Transmit(HILS_UART, &command[0], 5);
  d_Status_t status = imu_hils_wait_response(5, response, 5);
  if (status == d_STATUS_SUCCESS)
  {
    *pValue = ((Uint16_t)response[3] << 8) | response[4];
  }
  hilsBusy = d_FALSE;

  return status;
}

/*********************************************************************//**
  <!-- registerWriteHils -->

  Write register value to the HILS IMU simulator.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
registerWriteHils
(
const Uint32_t address,           /**< [in]  Register address */
const Uint16_t value              /**< [in]  Value to write */
)
{
  hilsBusy = d_TRUE;
  Uint8_t command[5];
  command[0] = 0x73u;
  command[1] = 0xCCu;
  command[2] = 0x00u;
  command[3] = (Uint8_t)(value & 0xFFu);
  command[4] = (Uint8_t)address | (Uint8_t)0x80u;
  (void)d_UART_Transmit(HILS_UART, &command[0], 5);
  command[3] = (Uint8_t)(value >> 8u);
  command[4] = (Uint8_t)(address + 1u) | (Uint8_t)0x80u;
  (void)d_UART_Transmit(HILS_UART, &command[0], 5);
  hilsBusy = d_FALSE;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- burstReadHils -->

  Perform a burst read from the HILS IMU simulator.
*************************************************************************/
static void                       /** \return None */
burstReadHils
(
void
)
{
  Uint8_t command[5];
  command[0] = 0x73u;
  command[1] = 0xCCu;
  command[2] = 0x20u;
  command[3] = 0x00u;
  command[4] = 0x68u;
  (void)d_UART_FlushRx(HILS_UART);
  (void)d_UART_Transmit(HILS_UART, &command[0], 5);
  state = IMU_ADIS16505_STATE_WAIT_HILS;
  hilsBurstRequestTime = d_TIMER_ReadValueInTicks(); 

  return;
}

/*********************************************************************//**
  <!-- burstReadHilsResponse -->

  Perform a burst read from the HILS IMU simulator.
*************************************************************************/
static void                       /** \return None */
burstReadHilsResponse
(
void
)
{
  Uint8_t response[BURST_BYTES + 1u];
  Uint32_t received = 0;
  
  (void)d_UART_Receive(HILS_UART, response, BURST_BYTES + 1u, &received);
  if (received >= (BURST_BYTES + 1u))
  {
    (void)parseBurstMessage(&response[3]);
    state = IMU_ADIS16505_STATE_STREAM;
  }
  else
  {
    // gcov-jst 2 It is not practical to ensure coverage of this path during bench testing.
    Uint32_t elapsedTime = d_TIMER_ElapsedMicroseconds(hilsBurstRequestTime, NULL);
    if (elapsedTime > HILS_MAX_TIMEOUT)
    {
      // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
      state = IMU_ADIS16505_STATE_STREAM;
    }
    else
    {
      /* Continue waiting */
      // gcov-jst 1 It is not practical to ensure coverage of this path during bench testing.
      DO_NOTHING();
    }
  }
    
  return;
}

