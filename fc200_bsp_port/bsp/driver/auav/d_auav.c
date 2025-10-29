/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_auav.c

  Abstract           : AUAV device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???

\note
  CSC ID             : SWDES-??
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "soc/timer/d_timer.h"
#include "sru/spi_pl/d_spi_pl.h"
#include "sru/spi_pl/d_spi_pl_cfg.h"

#include "d_auav.h"
#include "d_auav_cfg.h"

/***** Constants ********************************************************/

/* Commands */
#define COMMAND_START_SINGLE      0xAAu
#define COMMAND_START_AVERAGE_2   0xACu
#define COMMAND_START_AVERAGE_4   0xADu
#define COMMAND_START_AVERAGE_8   0xAEu
#define COMMAND_START_AVERAGE_16  0xAFu
#define COMMAND_READ_SENSOR       0xF0u
#define COMMAND_READ_STATUS       0xF0u
#define COMMAND_READ_DATA         0xF0u

/* SPI clock speed in MHz. Timeouts are based on bytes times clock speed times 2 */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t CLOCK_SPEED = 1u;

/***** Type Definitions *************************************************/

typedef struct
{
  Float32_t compensationA;
  Float32_t compensationB;
  Float32_t compensationC;
  Float32_t compensationD;
  Float32_t compensationEs;
  Float32_t compensationTc50h;
  Float32_t compensationTc50l;
} compensation_t;

/***** Variables ********************************************************/

static Bool_t initialised[d_AUAV_MAX_DEVICES] = {d_FALSE, d_FALSE, d_FALSE, d_FALSE};

static d_AUAV_DataBlock_t * pOfpDataBlock[d_AUAV_MAX_DEVICES];

static compensation_t compensation[d_AUAV_MAX_DEVICES][d_AUAV_CHANNEL_COUNT];

static Bool_t conversionComplete[d_AUAV_MAX_DEVICES][d_AUAV_CHANNEL_COUNT];

/***** Function Declarations ********************************************/

static d_Status_t InitialiseInterface(const Uint32_t device);

static d_Status_t ReadPressure(const Uint32_t device,
                               const d_AUAV_Channel_t channel);

static d_Status_t Measure(const Uint32_t device,
                          const d_AUAV_Channel_t channel,
                          const Uint8_t cmd);

static d_Status_t ReadCompensation(const Uint32_t device,
                                   const d_AUAV_Channel_t channel);

static d_Status_t RequestData(const Uint32_t device,
                              const d_AUAV_Channel_t channel,
                              const Uint8_t address);

static d_Status_t FetchData(const Uint32_t device,
                            const d_AUAV_Channel_t channel,
                            Uint16_t * const pValue);

static void CompensationFloat(const Uint32_t device,
                              const d_AUAV_Channel_t channel,
                              const Uint16_t * const rawValues);

static void CompensatePressure(const Uint32_t device,
                               const d_AUAV_Channel_t channel,
                               Uint8_t * const rawData,
                               Float32_t * const pPressure,
                               Float32_t * const pTemperature);

static d_Status_t WaitTransferComplete(const Uint32_t device,
                                       const Uint32_t byteCount);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_AUAV_Initialise -->

  Initialises the AUAV CSC.
*************************************************************************/
d_Status_t                        /** \return function status */
d_AUAV_Initialise
(
const Uint32_t device,            /**< [in]  device number */
d_AUAV_DataBlock_t * const pDataBlock
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  
  /* Check valid device number */
  if (device >= d_AUAV_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (initialised[device] != d_TRUE)
  {
    /* Check number of devices is valid */
    if ((d_AUAV_Count == 0u) || (d_AUAV_Count > d_AUAV_MAX_INTERFACES))
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, d_AUAV_Count, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }

    /* Check data pointer */
    if (pDataBlock == NULL)
    {
      d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_PARAMETER;
    }

    /* Check configured SPI interface numbers are valid */
    for (d_AUAV_Channel_t channel = d_AUAV_CHANNEL_ABSOLUTE; channel < d_AUAV_CHANNEL_COUNT; channel++)
    {
      if (d_AUAV_Config[device].spi[channel].spiChannel >= d_SPI_PL_Count)
      {
        // gcov-jst 3 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, device, channel, d_AUAV_Config[device].spi[channel].spiChannel);
        // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
        return d_STATUS_INVALID_CONFIGURATION;
      }

      if (d_AUAV_Config[device].spi[channel].spiDevice >= d_SPI_PL_MAX_DEVICES)
      {
        // gcov-jst 3 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, device, channel, d_AUAV_Config[device].spi[channel].spiDevice);
        // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
        return d_STATUS_INVALID_CONFIGURATION;
      }
    }
    
    status = d_SPI_PL_Initialise(d_AUAV_Config[device].spi[d_AUAV_CHANNEL_ABSOLUTE].spiChannel);

    if (status == d_STATUS_SUCCESS)
    {
      /* Perform recommended startup initialisation */
      status = InitialiseInterface(device);
    }
    
    if (status == d_STATUS_SUCCESS)
    {
      status = ReadCompensation(device, d_AUAV_CHANNEL_ABSOLUTE);
    }

    if (status == d_STATUS_SUCCESS)
    {
      status = ReadCompensation(device, d_AUAV_CHANNEL_DIFFERENTIAL);
    }

    pOfpDataBlock[device] = pDataBlock;

    pOfpDataBlock[device]->absolute.valid = d_FALSE;
    pOfpDataBlock[device]->differential.valid = d_FALSE;

    d_TIMER_Initialise();

    initialised[device] = d_TRUE;
  }

  return status;
}

/*********************************************************************//**
  <!-- d_AUAV_Read_Status -->

  Read the AUAV status.
*************************************************************************/
d_Status_t
d_AUAV_Read_Status
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel,
Uint8_t * const pStatus
)
{
  if (device >= d_AUAV_Count)
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

  if (channel >= d_AUAV_CHANNEL_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint8_t command = COMMAND_READ_STATUS;
  d_Status_t status = d_SPI_PL_Transfer(d_AUAV_Config[device].spi[channel].spiChannel, d_AUAV_Config[device].spi[channel].spiDevice, &command, 1u, NULL, 0u);
  
  if (status == d_STATUS_SUCCESS)
  {
    status = WaitTransferComplete(device, 1u);
  }
  
  if (status == d_STATUS_SUCCESS)
  {
    Uint8_t spiStatus;
    Uint32_t rxCount;
    status = d_SPI_PL_Result(d_AUAV_Config[device].spi[channel].spiChannel, &spiStatus, pStatus, 1u, &rxCount);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_AUAV_Start -->

  Start conversion of the AUAV sensor data.
*************************************************************************/
d_Status_t                        /** \return Success or Failure */
d_AUAV_Start
(
Uint32_t device                   /**< [in]  device number */
)
{
  if (device >= d_AUAV_Count)
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

  conversionComplete[device][d_AUAV_CHANNEL_ABSOLUTE] = d_FALSE;
  conversionComplete[device][d_AUAV_CHANNEL_DIFFERENTIAL] = d_FALSE;

  (void)Measure(device, d_AUAV_CHANNEL_ABSOLUTE, COMMAND_START_SINGLE);
  (void)Measure(device, d_AUAV_CHANNEL_DIFFERENTIAL, COMMAND_START_SINGLE);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_AUAV_InterruptHandler -->

  Device interrupt handler.
*************************************************************************/
void d_AUAV_InterruptHandler(const Uint32_t spiChannel, const Uint32_t spiDevice)
{
  /* Find AUAV device from SPI PL channel */
  Uint32_t device = 0;
  while ((device < d_AUAV_Count) && ((d_AUAV_Config[device].spi[0].spiChannel != spiChannel) || ((d_AUAV_Config[device].spi[0].spiDevice / 2u) != (spiDevice / 2u))))
  {
    device++;
  }
  
  if ((device < d_AUAV_Count) && (initialised[device] == d_TRUE))

  {
    /* The device number is used by the AUAV as the channel number */
    Uint32_t channelInt = spiDevice % 2u;
    d_AUAV_Channel_t channel = (d_AUAV_Channel_t)channelInt;

    /* Read the data */
    d_Status_t status = ReadPressure(device, channel);

    if (status == d_STATUS_SUCCESS)
    {
      /* Set conversion complete */
      conversionComplete[device][channel] = d_TRUE;

      /* If other channel conversion complete then callback */
      if (conversionComplete[device][((Uint32_t)channel + 1u) & 0x01u] == d_TRUE)
      {
        if (callback != NULL)
        {
          callback(device);
        }
      }
    }
  }

  return;
}

/*********************************************************************//**
  <!-- InitialiseInterface -->

  Perform initialisation as recommended.
*************************************************************************/
static d_Status_t
InitialiseInterface
(
const Uint32_t device             /**< [in]  device number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint16_t rawData;

  status = RequestData(device, d_AUAV_CHANNEL_DIFFERENTIAL, 43u);
  if (status == d_STATUS_SUCCESS)
  {
    (void)d_TIMER_DelayMicroseconds(20000);
    status = FetchData(device, d_AUAV_CHANNEL_DIFFERENTIAL, &rawData);
    /* Ignore the data */
    if (status == d_STATUS_DEVICE_BUSY)
    {
    	status = d_STATUS_SUCCESS;
    }
  }

  if (status == d_STATUS_SUCCESS)
  {
    (void)d_TIMER_DelayMicroseconds(100000);
    status = RequestData(device, d_AUAV_CHANNEL_ABSOLUTE, 47u);
  }

  if (status == d_STATUS_SUCCESS)
  {
    (void)d_TIMER_DelayMicroseconds(20000);
    status = FetchData(device, d_AUAV_CHANNEL_ABSOLUTE, &rawData);
    /* Ignore the data */
    if (status == d_STATUS_DEVICE_BUSY)
    {
    	status = d_STATUS_SUCCESS;
    }
  }

  return status;
}

/*********************************************************************//**
  <!-- ReadPressure -->

  Read the absolute and differential pressures.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
ReadPressure
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel
)
{
  d_Status_t status;
  Uint8_t command[7];
  Uint8_t response[7];
  Uint32_t rxCount = 0;
  d_SPI_PL_State_t spiState;

  command[0] = COMMAND_READ_SENSOR;
  for (Uint32_t index = 1u; index < 7u; index++)
  {
    command[index] = 0u;
  }

  status = d_SPI_PL_Transfer(d_AUAV_Config[device].spi[channel].spiChannel, d_AUAV_Config[device].spi[channel].spiDevice, &command[0], 7u, NULL, 0u);

  if (status == d_STATUS_SUCCESS)
  {
    status = WaitTransferComplete(device, 7u);
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = d_SPI_PL_Result(d_AUAV_Config[device].spi[channel].spiChannel, &spiState, &response[0], 7u, &rxCount);
  }

  if ((status == d_STATUS_SUCCESS) && (rxCount == 7u))
  {
    Float32_t pressure;
    Float32_t temperature;

    CompensatePressure(device, channel, &response[0], &pressure, &temperature);

    /* Store data in OFP structure */
    if (channel == d_AUAV_CHANNEL_ABSOLUTE)
    {
      pOfpDataBlock[device]->absolute.pressure = pressure;
      pOfpDataBlock[device]->absolute.temperature = temperature;
      pOfpDataBlock[device]->absolute.valid = d_TRUE;
    }
    else
    {
      pOfpDataBlock[device]->differential.pressure = pressure;
      pOfpDataBlock[device]->differential.temperature = temperature;
      pOfpDataBlock[device]->differential.valid = d_TRUE;
    }
  }
  else
  {
    if (channel == d_AUAV_CHANNEL_ABSOLUTE)
    {
      pOfpDataBlock[device]->absolute.valid = d_FALSE;
    }
    else
    {
      pOfpDataBlock[device]->differential.valid = d_FALSE;
    }
  }

  return status;
}

/*********************************************************************//**
  <!-- Measure -->

  Start a measurement.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
Measure
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel,
const Uint8_t cmd
)
{
  d_Status_t status;
  Uint8_t command[3];
  Uint32_t rxCount = 0;
  d_SPI_PL_State_t spiState;

  command[0] = cmd;
  command[1] = 0x00u;
  command[2] = 0x00u;

  status = d_SPI_PL_Transfer(d_AUAV_Config[device].spi[channel].spiChannel, d_AUAV_Config[device].spi[channel].spiDevice, &command[0], 3u, NULL, 0u);

  if (status == d_STATUS_SUCCESS)
  {
    status = WaitTransferComplete(device, 3u);
  }

  if (status == d_STATUS_SUCCESS)
  {
    Uint8_t response[3];
    status = d_SPI_PL_Result(d_AUAV_Config[device].spi[channel].spiChannel, &spiState, &response[0], 3u, &rxCount);
    if ((status == d_STATUS_SUCCESS) && (rxCount != 3u))
    {
      status = d_STATUS_FAILURE;
    }      
  }


  return status;
}

/*********************************************************************//**
  <!-- ReadCompensation -->

  Read the compensation data
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
ReadCompensation
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel    /**< [in]  channel to read */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Uint8_t startAddress;
  Uint16_t rawValues[10];
  
  (void)d_TIMER_DelayMicroseconds(100000);

  d_GEN_MemorySet((Uint8_t *)&rawValues[0], 0u, sizeof(rawValues));

  if (channel == d_AUAV_CHANNEL_ABSOLUTE)
  {
    startAddress = 47u;
  }
  else
  {
    startAddress = 43u;
  }

  Uint32_t index = 0;
  /* Five times the typical value */
  Uint32_t eepromReadDelay = 500u;
  while ((index < 10u) && (status == d_STATUS_SUCCESS))
  {
    Uint16_t response = 0u;

    /* Read compensation data with progressively increasing delay until get success */
//    do
//    {
      status = RequestData(device, channel, startAddress + index);

      if (status == d_STATUS_SUCCESS)
      {
        /* Memory read access time delay, maximum value from spec */
        (void)d_TIMER_DelayMicroseconds(eepromReadDelay);
        status = FetchData(device, channel, &response);
        if (status != d_STATUS_SUCCESS)
        {
          eepromReadDelay = eepromReadDelay + 500u;
        }
      }
//    } while ((status != d_STATUS_SUCCESS) && (eepromReadDelay <= 2000u));

    if (status == d_STATUS_SUCCESS)
    {
      rawValues[index] = response;
    }

    index++;
  }

  if (status == d_STATUS_SUCCESS)
  {
    CompensationFloat(device, channel, &rawValues[0]);
  }

  return status;
}

/*********************************************************************//**
  <!-- RequestData -->

  Request compensation data
*************************************************************************/
static d_Status_t
RequestData
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel,   /**< [in]  channel to read */
const Uint8_t address
)
{
  d_Status_t status;
  Uint8_t command[3];
  Uint32_t rxCount = 0;
  d_SPI_PL_State_t spiState;

  command[0] = address;
  command[1] = 0u;
  command[2] = 0u;

  status = d_SPI_PL_Transfer(d_AUAV_Config[device].spi[channel].spiChannel, d_AUAV_Config[device].spi[channel].spiDevice, &command[0], 3u, NULL, 0u);

  if (status == d_STATUS_SUCCESS)
  {
    status = WaitTransferComplete(device, 3u);
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = d_SPI_PL_Result(d_AUAV_Config[device].spi[channel].spiChannel, &spiState, NULL, 0u, &rxCount);
  }

  return status;
}

/*********************************************************************//**
  <!-- FetchData -->

  Fetch compensation data
*************************************************************************/
static d_Status_t
FetchData
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel,   /**< [in]  channel to read */
Uint16_t * const pValue
)
{
  d_Status_t status;
  Uint8_t command[3];
  Uint8_t response[3];
  Uint32_t rxCount = 0;
  d_SPI_PL_State_t spiState;

  command[0] = COMMAND_READ_DATA;
  command[1] = 0u;
  command[2] = 0u;

  status = d_SPI_PL_Transfer(d_AUAV_Config[device].spi[channel].spiChannel, d_AUAV_Config[device].spi[channel].spiDevice, &command[0], 3u, NULL, 0u);

  if (status == d_STATUS_SUCCESS)
  {
    status = WaitTransferComplete(device, 3u);
  }

  if (status == d_STATUS_SUCCESS)
  {
    status = d_SPI_PL_Result(d_AUAV_Config[device].spi[channel].spiChannel, &spiState, &response[0], 3u, &rxCount);
  }

  if (status == d_STATUS_SUCCESS)
  {
    if (rxCount != 3u)
    {
      status = d_STATUS_FAILURE;
    }
//    else if (response[0] != (0x40u | ((Uint8_t)channel << 4)))
//    {
//      status = d_STATUS_DEVICE_BUSY;
//    }
    else
    {
      *pValue = (((Uint16_t)response[1]) << 8) | response[2];
    }
  }

  return status;
}

/*********************************************************************//**
  <!-- CompensationFloat -->

  Covert compensation raw values to floating point.
*************************************************************************/
static void CompensationFloat
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel,   /**< [in]  channel to read */
const Uint16_t * const rawValues
)
{
  Uint32_t tempu = (((Uint32_t)rawValues[0]) << 16) | rawValues[1];
  Int32_t tempi = (Int32_t)tempu;
  compensation[device][channel].compensationA = (Float32_t)tempi / (Float32_t)0x7FFFFFFFu;

  tempu = (((Uint32_t)rawValues[2]) << 16) | rawValues[3];
  tempi = (Int32_t)tempu;
  compensation[device][channel].compensationB = (Float32_t)tempi / (Float32_t)0x7FFFFFFFu;

  tempu = (((Uint32_t)rawValues[4]) << 16) | rawValues[5];
  tempi = (Int32_t)tempu;
  compensation[device][channel].compensationC = (Float32_t)tempi / (Float32_t)0x7FFFFFFFu;

  tempu = (((Uint32_t)rawValues[6]) << 16) | rawValues[7];
  tempi = (Int32_t)tempu;
  compensation[device][channel].compensationD = (Float32_t)tempi / (Float32_t)0x7FFFFFFFu;

  Uint8_t tempu8 = (Uint8_t)(rawValues[8] >> 8);
  Int8_t tempi8 = (Int8_t)tempu8;
  compensation[device][channel].compensationTc50h = (Float32_t)tempi8 / (Float32_t)0x7Fu;

  tempu8 = (Uint8_t)(rawValues[8] & 0xFFu);
  tempi8 = (Int8_t)tempu8;
  compensation[device][channel].compensationTc50l = (Float32_t)tempi8 / (Float32_t)0x7Fu;

  tempi8 = (Int8_t)rawValues[9];
  compensation[device][channel].compensationEs = (Float32_t)tempi8 / (Float32_t)0x7Fu;

  return;
}

/*********************************************************************//**
  <!-- CompensatePressure -->

  Perform pressure compensation.
*************************************************************************/
static void
CompensatePressure
(
const Uint32_t device,            /**< [in]  device number */
const d_AUAV_Channel_t channel,    /**< [in]  channel to read */
Uint8_t * const rawData,
Float32_t * const pPressure,
Float32_t * const pTemperature
)
{
  /* Constants */
  static const Int32_t   TREF_COUNTS = 7576807;
  static const Float32_t TC50SCALE = 100.0f * 100.0f * 167772.2f;

  /* Local variables */
  Float32_t Pnorm;
  Float32_t AP3;
  Float32_t BP2;
  Float32_t CP;
  Float32_t Corr;
  Float32_t Pcorr;
  Float32_t Pdiff;
  Float32_t TC50;
  Float32_t Pnfso;
  Float32_t Tcorr;
  Float32_t PCorrt;
  Int32_t  iPraw;
  Int32_t  Tdiff;
  Int32_t  iTemp;
  Uint32_t Pcomp;

  /* Convert unsigned 24-bit pressure value to signed +/- 23-bit */
  Uint32_t uPraw = (rawData[1] << 16u) + (rawData[2] << 8u) + rawData[3];
  iPraw = (Int32_t)uPraw;
  iPraw = iPraw - 0x800000;
  
  /* Convert signed 23-bit value to float, normalised to +/- 1.0 */
  Pnorm = (Float32_t)iPraw;
  Pnorm = Pnorm / (Float32_t)0x7FFFFFu;

  AP3 = compensation[device][channel].compensationA * Pnorm * Pnorm * Pnorm; /* A*Pout^3 */
  BP2 = compensation[device][channel].compensationB * Pnorm * Pnorm;         /* B*Pout^2 */
  CP  = compensation[device][channel].compensationC * Pnorm;                 /* C*Pout */
  Corr = AP3 + BP2 + CP + compensation[device][channel].compensationD;       /* Linearity correction term */
  Pcorr = Pnorm + Corr;                                                      /* Corrected P, range +/- 1.0 */

//  iPcorr = (Int32_t)(Pcorr * (Float32_t)0x7FFFFF);
//  iPcorr = iPcorr + 0x800000;

  /* Compute difference from reference temperature, in sensor counts */
  iTemp = (rawData[4] << 16u) + (rawData[5] << 8u) + rawData[6];
  Tdiff = iTemp - TREF_COUNTS;

  /* Convert to engineering uits and return */
  *pTemperature = (((Float32_t)iTemp * 155.0f) / (Float32_t)0xFFFFFFu) - 45.0f;

  Pnfso = (Pcorr + 1.0f) / 2.0f;

  /* TC50: Select High/Low, based on current temp above/below 25C */
  if (Tdiff > 0)
  {
    TC50 = compensation[device][channel].compensationTc50h;
  }
  else
  {
    TC50 = compensation[device][channel].compensationTc50l;
  }

  /* Find absolute difference between midrange and reading (abs(Pnsfo - 0.5)) */
  if (Pnfso > 0.5f)
  {
    Pdiff = Pnfso - 0.5f;
  }
  else
  {
    Pdiff = 0.5f - Pnfso;
  }

  Tcorr = (1.0f - (compensation[device][channel].compensationEs * 2.5f * Pdiff)) * (Float32_t)Tdiff * TC50 / TC50SCALE;
  PCorrt = Pnfso - Tcorr; /* Corrected P: float [0 to +1.0] */

  Pcomp = (Uint32_t)(PCorrt * (Float32_t)0xFFFFFFu);

  if (channel == d_AUAV_CHANNEL_ABSOLUTE)
  {
    /* Absolute pressure in Pascal */
    *pPressure = 25000.0f + ((1.25f * ((Float32_t)Pcomp - 1677721.6f) / 16777216.0f) * 100000.0f);
  }
  else
  {
    /* Differential pressure in Pascal */
    *pPressure = 125.0f * (((Float32_t)Pcomp - 8388608.0f) / 16777216.0f) * d_AUAV_Config[device].calRange;
  }

  return;
}

/*********************************************************************//**
  <!-- WaitTransferComplete -->

  Wait for completion of the SPI transfer.
*************************************************************************/
static d_Status_t                 /** \return Success or Failure */
WaitTransferComplete
(
const Uint32_t device,            /**< [in]  device number */
const Uint32_t byteCount
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  
  Uint32_t startTime;
  Uint32_t elapsedTime;
  d_SPI_PL_State_t spiState;

  (void)d_TIMER_ElapsedMicroseconds(0, &startTime);
  do
  {
    status = d_SPI_PL_Status(d_AUAV_Config[device].spi[d_AUAV_CHANNEL_ABSOLUTE].spiChannel, &spiState);
    elapsedTime = d_TIMER_ElapsedMicroseconds(startTime, NULL);
  } while ((spiState != d_SPI_PL_STATUS_COMPLETE) && (elapsedTime < 8u * (byteCount + 1u) * 2u / CLOCK_SPEED));

  if (spiState != d_SPI_PL_STATUS_COMPLETE)
  {
    status = d_STATUS_TIMEOUT;
  }
  
  return status;
}
