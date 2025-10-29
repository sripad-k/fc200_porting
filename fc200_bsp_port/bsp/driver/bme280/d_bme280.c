/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_bme280.c

  Abstract           : BME280 device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???

\note
  CSC ID             : SWDES-??
*************************************************************************/

/***** Includes *********************************************************/

#include "stdio.h"

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "soc/timer/d_timer.h"
#include "sru/spi_pl/d_spi_pl.h"
#include "sru/spi_pl/d_spi_pl_cfg.h"

#include "d_bme280.h"
#include "d_bme280_cfg.h"

/***** Constants ********************************************************/

/* Device register addresses */
#define REGISTER_CALIB0      0x88u
#define REGISTER_ID          0xD0u
#define REGISTER_RESET       0xE0u
#define REGISTER_CALIB1      0xE1u
#define REGISTER_CTRL_HUM    0xF2u
#define REGISTER_STATUS      0xF3u
#define REGISTER_CTRL_MEAS   0xF4u
#define REGISTER_CONFIG      0xF5u
#define REGISTER_PRESS_MSB   0xF7u
#define REGISTER_PRESS_LSB   0xF8u
#define REGISTER_PRESS_XLSB  0xF9u
#define REGISTER_TEMP_MSB    0xFAu
#define REGISTER_TEMP_LSB    0xFBu
#define REGISTER_TEMP_XLSB   0xFCu
#define REGISTER_HUM_MAS     0xFDu
#define REGISTER_HUM_LSB     0xFEu

#define DEVICE_ID            0x60u

#define RESET_COMMAND        0xB6u

/* SPI clock speed in MHz. Timeouts are based on bytes * clock speed * 2 */
static const Uint32_t CLOCK_SPEED = 1;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float64_t TEMPERATURE_MIN = -40.0;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float64_t TEMPERATURE_MAX = 85.0;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float64_t PRESSURE_MIN = 30000.0;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float64_t PRESSURE_MAX = 110000.0;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float64_t HUMIDITY_MIN = 0.0;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Float64_t HUMIDITY_MAX = 100.0;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Bool_t initialised[d_BME280_MAX_INTERFACES] = {d_FALSE, d_FALSE, d_FALSE, d_FALSE};

/* Temperature calibration data */
static Float64_t compensationT1;
static Float64_t compensationT2;
static Float64_t compensationT3;

/* Pressure calibration data */
static Float64_t compensationP1;
static Float64_t compensationP2;
static Float64_t compensationP3;
static Float64_t compensationP4;
static Float64_t compensationP5;
static Float64_t compensationP6;
static Float64_t compensationP7;
static Float64_t compensationP8;
static Float64_t compensationP9;

/* Humidity calibration data */
static Float64_t compensationH1;
static Float64_t compensationH2;
static Float64_t compensationH3;
static Float64_t compensationH4;
static Float64_t compensationH5;
static Float64_t compensationH6;

static Int32_t t_fine;

/***** Function Declarations ********************************************/

static d_Status_t ValidateConfig(const Uint32_t device);

static d_Status_t DataRead(const Uint32_t device,
                           const Uint8_t address,
                           const Uint32_t count,
                           Uint8_t * const pValue);

static d_Status_t CalibrationDataRead(const Uint32_t device);

static d_Status_t RegisterRead(const Uint32_t device, const Uint8_t address, Uint8_t * const pValue);

static d_Status_t RegisterWrite(const Uint32_t device, const Uint8_t address, Uint8_t value);

static Float64_t CompensateT(Uint32_t adc_T);
static Float64_t CompensateH(Uint32_t adc_H);
static Float64_t CompensateP(Uint32_t adc_P);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_BME280_Initialise -->

  Initialises the BME280 device.
*************************************************************************/
d_Status_t                        /** \return function status */
d_BME280_Initialise
(
const Uint32_t device,
const Uint8_t config,
const Uint8_t ctrl_hum,
const Uint8_t ctrl_meas
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  status = ValidateConfig(device);
  
  if (status != d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return status;
  }

  d_TIMER_Initialise();

  status = d_SPI_PL_Initialise(d_BME280_Config[device].channel);

  if (status == d_STATUS_SUCCESS)
  {
    /* Reset the device */
    status = RegisterWrite(device, REGISTER_RESET, RESET_COMMAND);
  }

  /* Delay to allow reset to complete */
  (void)d_TIMER_DelayMilliseconds(2);

  /* Read and verify the ID */
  if (status == d_STATUS_SUCCESS)
  {
    Uint8_t idValue;
    status = RegisterRead(device, REGISTER_ID, &idValue);
    if ((status == d_STATUS_SUCCESS) && (idValue != DEVICE_ID))
    {
      status = d_STATUS_HARDWARE_ERROR;
    }
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = CalibrationDataRead(device);
  }

  if (status == d_STATUS_SUCCESS)
  {
    /* Update settings */
    status = RegisterWrite(device, REGISTER_CONFIG, config);
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = RegisterWrite(device, REGISTER_CTRL_HUM, ctrl_hum);
  }
  if (status == d_STATUS_SUCCESS)
  {
    status = RegisterWrite(device, REGISTER_CTRL_MEAS, ctrl_meas);
  }

  if (status == d_STATUS_SUCCESS)
  {
    initialised[device] = d_TRUE;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_BME280_Read -->

  Read the BME280 data.
*************************************************************************/
d_Status_t
d_BME280_Read
(
const Uint32_t device,
Float64_t * const pHumidity,    /* % */
Float64_t * const pPressure,    /* Pa */
Float64_t * const pTemperature  /* Deg C */
)
{
  d_Status_t status;
  
  if (device >= d_BME280_Count)
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

  if (pHumidity == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pPressure == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pTemperature == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint8_t rawData[REGISTER_HUM_LSB - REGISTER_PRESS_MSB + 1u];
  d_GEN_MemorySet(&rawData[0], 0u, sizeof(rawData));

  status = DataRead(device, REGISTER_PRESS_MSB, REGISTER_HUM_LSB - REGISTER_PRESS_MSB + 1u, &rawData[0]);

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t rawValue = (((Uint32_t)rawData[3]) << 12u) | (((Uint32_t)rawData[4]) << 4u) | (((Uint32_t)rawData[5]) >> 4u);
    *pTemperature = CompensateT(rawValue);

    rawValue = ((((Uint32_t)rawData[0]) << 12u) | (((Uint32_t)rawData[1]) << 4u) | (((Uint32_t)rawData[2]) >> 4u));
    *pPressure = CompensateP(rawValue);

    rawValue = (((Uint32_t)rawData[6]) << 8u) | (Uint32_t)rawData[7];
    *pHumidity = CompensateH(rawValue);
  }
  
  return status;
}

/*********************************************************************//**
  <!-- ValidateConfig -->

  Check the configuration data.
*************************************************************************/
static d_Status_t                 /** \return function status */
ValidateConfig
(
const Uint32_t device             /**< [in]  BME device number */
)
{
  /* Check number of devices is valid */
  if ((d_BME280_Count == 0u) || (d_BME280_Count > d_BME280_MAX_INTERFACES))
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, d_BME280_Count, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (device >= d_BME280_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (d_BME280_Config[device].channel >= d_SPI_PL_Count)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, device, d_BME280_Config[device].channel, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  if (d_BME280_Config[device].device >= d_SPI_PL_MAX_DEVICES)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, device, d_BME280_Config[device].device, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- CalibrationDataRead -->

  Read the calibration data.
*************************************************************************/
static d_Status_t
CalibrationDataRead
(
const Uint32_t device             /**< [in]  BME device number */
)
{
  Uint8_t rawData[26];

  /* Read first block of calibration data */
  d_Status_t status = DataRead(device, REGISTER_CALIB0, 26u, &rawData[0]);
  
  if (status == d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationT1 = (Float64_t)(*(Uint16_t *)&rawData[0]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationT2 = (Float64_t)(*(Int16_t *)&rawData[2]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationT3 = (Float64_t)(*(Int16_t *)&rawData[4]);

    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP1 = (Float64_t)(*(Uint16_t *)&rawData[6]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP2 = (Float64_t)(*(Int16_t *)&rawData[8]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP3 = (Float64_t)(*(Int16_t *)&rawData[10]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP4 = (Float64_t)(*(Int16_t *)&rawData[12]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP5 = (Float64_t)(*(Int16_t *)&rawData[14]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP6 = (Float64_t)(*(Int16_t *)&rawData[16]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP7 = (Float64_t)(*(Int16_t *)&rawData[18]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP8 = (Float64_t)(*(Int16_t *)&rawData[20]);
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationP9 = (Float64_t)(*(Int16_t *)&rawData[22]);

    compensationH1 = (Float64_t)rawData[25];

    /* Read second block of calibration data */
    status = DataRead(device, REGISTER_CALIB1, 6u, &rawData[0]);
  }

  if (status == d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
    compensationH2 = (Float64_t)(*(Int16_t *)&rawData[0]);
    compensationH3 = (Float64_t)rawData[2];
    Uint16_t temp = ((Uint16_t)rawData[3] << 4) | (rawData[4] & 0x0Fu);
    compensationH4 = (Float64_t)((Int16_t)temp);
    temp = ((Uint16_t)rawData[5] << 4) | ((rawData[3] & 0xF0u) >> 4);
    compensationH5 = (Float64_t)((Int16_t)temp);
    compensationH6 = (Float64_t)((Int8_t)rawData[5]);
  }
  
  return status;
}
  
/*********************************************************************//**
  <!-- DataRead -->

  Read multiple registers
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
DataRead
(
const Uint32_t device,            /**< [in]  BME device number */
const Uint8_t address,            /**< [in]  Register address */
const Uint32_t count,             /**< [in]  Register count */
Uint8_t * const pValue            /**< [out] Pointer to storage for register values */
)
{
  d_Status_t status;
  d_SPI_PL_State_t spiStatus;
  Uint8_t command;
  
  command = address;

  status = d_SPI_PL_Transfer(d_BME280_Config[device].channel, d_BME280_Config[device].device, &command, 1u, NULL, count);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime;
    Uint32_t elapsedTime;

    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
    do
    {
      status = d_SPI_PL_Status(d_BME280_Config[device].channel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < (8u * (count + 1u) * 2u / CLOCK_SPEED)));

    if (spiStatus != d_SPI_PL_STATUS_COMPLETE)
    {
      status = d_STATUS_TIMEOUT;
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t rxCount;
    status = d_SPI_PL_Result(d_BME280_Config[device].channel, &spiStatus, pValue, count, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- RegisterRead -->

  Read register value
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
RegisterRead
(
const Uint32_t device,            /**< [in]  BME device number */
const Uint8_t address,            /**< [in]  Register address */
Uint8_t * const pValue            /**< [out] Pointer to storage for register value */
)
{
  d_Status_t status;

  d_SPI_PL_State_t spiStatus;
  Uint32_t rxCount = 0;

  Uint8_t command;
  Uint8_t response;

  command = address;

  status = d_SPI_PL_Transfer(d_BME280_Config[device].channel, d_BME280_Config[device].device, &command, 1u, NULL, 1u);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t startTime;
    Uint32_t elapsedTime;

    (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
    do
    {
      status = d_SPI_PL_Status(d_BME280_Config[device].channel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < (32u / CLOCK_SPEED)));

    if (spiStatus != d_SPI_PL_STATUS_COMPLETE)
    {
      status = d_STATUS_TIMEOUT;
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  if (status == d_STATUS_SUCCESS)
  {
    status = d_SPI_PL_Result(d_BME280_Config[device].channel, &spiStatus, &response, 1, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if ((status == d_STATUS_SUCCESS) && (rxCount == 1u))
  {
    *pValue = response;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }

  return status;
}

/*********************************************************************//**
  <!-- RegisterWrite -->

  Write register value.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
RegisterWrite
(
const Uint32_t device,            /**< [in]  BME device number */
const Uint8_t address,            /**< [in]  Register address */
const Uint8_t value               /**< [in]  Value to write */
)
{
  d_Status_t status;

  d_SPI_PL_State_t spiStatus;
  Uint32_t rxCount = 0;

  Uint8_t command[2];

  command[0] = address & 0x7Fu;
  command[1] = value;


  status = d_SPI_PL_Transfer(d_BME280_Config[device].channel, d_BME280_Config[device].device, &command[0], 2u, NULL, 0);

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t elapsedTime;
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    do
    {
      status = d_SPI_PL_Status(d_BME280_Config[device].channel, &spiStatus);
      elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
    } while ((status == d_STATUS_SUCCESS) && (spiStatus != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < (32u / CLOCK_SPEED)));

    if (spiStatus != d_SPI_PL_STATUS_COMPLETE)
    {
      status = d_STATUS_TIMEOUT;
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING
  
  if (status == d_STATUS_SUCCESS)
  {
    status = d_SPI_PL_Result(d_BME280_Config[device].channel, &spiStatus, NULL, 0, &rxCount);
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return status;
}

/*********************************************************************//**
  <!-- CompensateT -->

  Returns temperature in DegC, Float64_t precision. Output value of 51.23
  equals 51.23 DegC. t_fine carries fine temperature as global value.
*************************************************************************/
static Float64_t
CompensateT
(
Uint32_t adc_T
)
{
  Float64_t var1;
  Float64_t var2;
  Float64_t var3;
  Float64_t temperature;
  
  var1 = ((Float64_t)adc_T / 16384.0) - (compensationT1 / 1024.0);
  var1 = var1 * compensationT2;
  var2 = (((Float64_t)adc_T) / 131072.0) - (compensationT1 / 8192.0);
  var2 = var2 * var2 * compensationT3;
  var3 = var1 + var2;
  t_fine = (Int32_t)var3;
  temperature = var3 / 5120.0;
  if (temperature < TEMPERATURE_MIN)
  {
    temperature = TEMPERATURE_MIN;
  }
  else if (temperature > TEMPERATURE_MAX)
  {
    temperature = TEMPERATURE_MAX;
  }
  else
  {
    /* In range */
  }

  return temperature;
}

/*********************************************************************//**
  <!-- CompensateP -->

  Returns pressure in mbar.
*************************************************************************/
static Float64_t
CompensateP
(
Uint32_t adc_P
)
{
  Float64_t var1;
  Float64_t var2;
  Float64_t pressure;

  var1 = ((Float64_t)t_fine / 2.0) - 64000.0;
  var2 = var1 * var1 * compensationP6 / 32768.0;
  var2 = var2 + (var1 * compensationP5 * 2.0);
  var2 = (var2 / 4.0) + (compensationP4 * 65536.0);
  var1 = (((compensationP3 * var1 * var1) / 524288.0) + (compensationP2 * var1)) / 524288.0;
  var1 = (1.0 + (var1 / 32768.0)) * compensationP1;
  if (var1 <= 0.0)
  {
    pressure = PRESSURE_MIN; /* avoid exception caused by division by zero */
  }
  else
  {
    pressure = 1048576.0 - (Float64_t)adc_P;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = compensationP9 * pressure * pressure / 2147483648.0;
    var2 = pressure * compensationP8 / 32768.0;
    pressure = pressure + ((var1 + var2 + compensationP7) / 16.0);

    if (pressure > PRESSURE_MAX)
    {
      pressure = PRESSURE_MAX;
    }
    else if (pressure < PRESSURE_MIN)
    {
      pressure = PRESSURE_MIN;
    }
    else
    {
      /* In range */;
    }
  }
  
  return pressure;
}

/*********************************************************************//**
  <!-- CompensateH -->

  Returns humidity in %rH as as Float64_t. Output value of 46.332 represents
  46.332 %rH.
*************************************************************************/
static Float64_t
CompensateH
(
Uint32_t adc_H
)
{
  Float64_t var1;
  Float64_t var2;
  Float64_t var3;
  Float64_t var4;
  Float64_t var5;
  Float64_t var6;
  Float64_t humidity;

  var1 = (Float64_t)t_fine - 76800.0;
  var2 = (compensationH4 * 64.0) + ((compensationH5 / 16384.0) * var1);
  var3 = (Float64_t)adc_H - var2;
  var4 = compensationH2 / 65536.0;
  var5 = 1.0 + ((compensationH3 / 67108864.0) * var1);
  var6 = 1.0 + ((compensationH6 / 67108864.0) * var1 * var5);
  var6 = var3 * var4 * var5 * var6;
  humidity = var6 * (1.0 - ((compensationH1 * var6) / 524288.0));

 if (humidity > HUMIDITY_MAX)
  {
    humidity = HUMIDITY_MAX;
  }
  else if (humidity < HUMIDITY_MIN)
  {
    humidity = HUMIDITY_MIN;
  }
  else
  {
    /* In range */;
  }

  return humidity;
}

