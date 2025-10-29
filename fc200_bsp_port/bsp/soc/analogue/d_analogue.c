/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Analogue PL

  Abstract           : Analogue interface to PL measured values.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-210
                                                                SWREQ-211
                       SDD References: 136T-2200-131000-001-D22 SWDES-94
                                                                SWDES-95
                                                                SWDES-96
                                                                SWDES-97
\note
  CSC ID             : SWDES-56
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/timer/d_timer.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/error_handler/d_error_handler.h"
#include "d_analogue.h"

#include "xsysmonpsu.h"

/***** Constants ********************************************************/

/* Maximum time allowed for PS SYSMON to startup after reset, in microseconds */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t MAX_RESET_TIME = 100u;

/* PS System Monitor registers */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t PSSYSMON_BASE_ADDRESS = 0xFFA50800u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SEQ_CHANNEL0_OFFSET   = 0x00000120u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SEQ_CHANNEL2_OFFSET   = 0x00000118u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SEQ_AVERAGE0_OFFSET   = 0x00000128u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t SEQ_AVERAGE2_OFFSET   = 0x0000011Cu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t CONFIG_REG0_OFFSET    = 0x00000100u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t CONFIG_REG1_OFFSET    = 0x00000104u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t CONFIG_REG3_OFFSET    = 0x0000010Cu;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t PS_CTRL_STATUS_REG_ADDRESS = 0xFFA50040u;

/***** Macro Definitions ************************************************/

/* Conversion to engineering units macros */
#define XSysMonPsu_RawToTemperature_OnChip(AdcData) \
	((((float)(AdcData)/65536.0f)/0.00196342531f ) - 280.2309f)

#define XSysMonPsu_RawToVoltage(AdcData)            \
	((((float)(AdcData))* (3.0f))/65536.0f)

#define XSysMonPsu_VpVnRawToVoltage(AdcData)        \
	((((float)(AdcData))* (1.0f))/65536.0f)

#define XSysMonPsu_VccopsioRawToVoltage(AdcData)    \
	((((float)(AdcData))* (6.0f))/65536.0f)

/***** Type Definitions *************************************************/

/* Enumeration for identifying scaling required */
typedef enum
{
  SCALING_TEMPERATURE = 0,
  SCALING_VOLTAGE,
  SCALING_VOLTAGE_VP_VN,
  SCALING_VOLTAGE_PSIO
} scaling_t;

typedef struct
{
  Uint32_t  channel;
  Uint32_t  block;
  scaling_t scaling;
  Float32_t minimum;
  Float32_t maximum;
} channel_t;

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static const channel_t channels[d_ANALOG_CHANNEL_COUNT] =
{

  {XSM_CH_TEMP,       XSYSMON_PS, SCALING_TEMPERATURE, -50.0f, 105.0f},//0
  {XSM_CH_SUPPLY1,    XSYSMON_PS, SCALING_VOLTAGE, 0.797f, 0.903f},//1
  {XSM_CH_SUPPLY2,    XSYSMON_PS, SCALING_VOLTAGE, 0.797f, 0.903f},//2
  {XSM_CH_SUPPLY3,    XSYSMON_PS, SCALING_VOLTAGE, 1.680f, 1.920f},//3
  {XSM_CH_SUPPLY4,    XSYSMON_PS, SCALING_VOLTAGE, 1.107f, 1.293f},//4
  {XSM_CH_SUPPLY5,    XSYSMON_PS, SCALING_VOLTAGE_PSIO, 3.047f, 3.553f},//5
  {XSM_CH_SUPPLY6,    XSYSMON_PS, SCALING_VOLTAGE_PSIO, 1.677f, 1.923f},//6
  {0x80,              XSYSMON_PS, SCALING_VOLTAGE_PSIO, 3.047f, 3.553f},//7
  {0x81,              XSYSMON_PS, SCALING_VOLTAGE_PSIO, 3.047f, 3.553f},//8
  {0x82,              XSYSMON_PS, SCALING_VOLTAGE, 0.814f, 0.886f},//9
  {0x83,              XSYSMON_PS, SCALING_VOLTAGE, 1.716f, 1.884f},//10

  {0x84,              XSYSMON_PS, SCALING_VOLTAGE, 1.689f, 1.911f},//11
  {0x85,              XSYSMON_PS, SCALING_TEMPERATURE, -50.0f, 105.0f},//12

  {XSM_CH_TEMP,       XSYSMON_PL, SCALING_TEMPERATURE, -50.0f, 105.0f},//13
  {XSM_CH_SUPPLY1,    XSYSMON_PL, SCALING_VOLTAGE, 0.814f, 0.887f},//14
  {XSM_CH_SUPPLY2,    XSYSMON_PL, SCALING_VOLTAGE, 1.716f, 1.884f},//15
  {XSM_CH_VREFP,      XSYSMON_PL, SCALING_VOLTAGE_VP_VN, 0.0f, 5.0f},//16
  {XSM_CH_VREFN,      XSYSMON_PL, SCALING_VOLTAGE_VP_VN, 0.0f, 5.0f},//17
  {XSM_CH_SUPPLY3,    XSYSMON_PL, SCALING_VOLTAGE, 0.814f, 0.887f},//18
  {XSM_CH_SUPPLY4,    XSYSMON_PL, SCALING_VOLTAGE, 0.797f, 0.903f},//19
  {XSM_CH_SUPPLY5,    XSYSMON_PL, SCALING_VOLTAGE, 0.797f, 0.903f},//20
  {XSM_CH_SUPPLY6,    XSYSMON_PL, SCALING_VOLTAGE, 1.680f, 1.920f},//21
};

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_ANALOG_Initialise -->

  Initialise the analogue interface.
*************************************************************************/
d_Status_t         /** \return Function status */
d_ANALOG_Initialise
(
void
)
{
  d_Status_t status;
  
  status = d_ERROR_Initialise();

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t value;
    
    /* Configure the analogue bus multiplexer */
    d_GEN_RegisterWrite(XPAR_PSU_AMS_BASEADDR + XPS_BA_OFFSET + 0x00000114u, 0x3210u);

    /* Step 1 */
    d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + CONFIG_REG1_OFFSET, 0);
    d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + CONFIG_REG3_OFFSET, 0);

    /* Step 2 */
    /* Force reset */
    d_GEN_RegisterWrite(PS_CTRL_STATUS_REG_ADDRESS, 0x02u);
    /* Release reset */
    d_GEN_RegisterWrite(PS_CTRL_STATUS_REG_ADDRESS, 0x00u);
    /* Allow time for reset to complete */
    (void)d_TIMER_DelayMicroseconds(MAX_RESET_TIME);
    /* Check startup complete */
    value = d_GEN_RegisterRead(PS_CTRL_STATUS_REG_ADDRESS);
    if ((value & 0x10000u) == 0u)
    {
      // gcov-jst 1 It is not possible to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_TIMEOUT, d_ERROR_CRITICALITY_NON_CRITICAL, 0, 0, 0, 0);
    }
    
    /* Step 3 */
    (void)d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + SEQ_CHANNEL0_OFFSET, 0x47E0u);
    (void)d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + SEQ_CHANNEL2_OFFSET, 0x003Cu);
      
    /* Step 7*/
    (void)d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + SEQ_AVERAGE0_OFFSET, 0x47E0u);
    (void)d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + SEQ_AVERAGE2_OFFSET, 0x003Cu);
    d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + CONFIG_REG0_OFFSET, 0x2000u);

    /* Step 8 */
    d_GEN_RegisterWrite(PSSYSMON_BASE_ADDRESS + CONFIG_REG1_OFFSET, 0x2000u);
    
    initialised = d_TRUE;
  }
  else
  {
    // gcov-jst 1 It is not possible to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_ANALOG_Read -->

  Read the an analogue value.
************************************************************************/
d_Status_t                  /** \return Success or Failure */
d_ANALOG_Read
(
d_ANALOG_Channel_t signal,  /**< [in] Signal to measure */
Float32_t * const pValue    /**< [out] Pointer to storage for measure value */
)
{
  Float32_t tempf = 0.0f;

  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }
  
  if (signal >= d_ANALOG_CHANNEL_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, signal, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint16_t adcData;
  Pointer_t effAddress;

  effAddress = (Uint32_t)XPAR_PSU_AMS_BASEADDR;
  if (channels[signal].block == XSYSMON_PS)
  {
    effAddress = effAddress + XPS_BA_OFFSET;
  }
  else
  {
    effAddress = effAddress + XPL_BA_OFFSET;
  }

  adcData = (Uint16_t)d_GEN_RegisterRead(effAddress + (channels[signal].channel << 2U));

  switch (channels[signal].scaling)
  {
    case SCALING_TEMPERATURE:
      tempf = XSysMonPsu_RawToTemperature_OnChip(adcData);
      break;

    case SCALING_VOLTAGE:
      tempf = XSysMonPsu_RawToVoltage(adcData);
      break;

    case SCALING_VOLTAGE_VP_VN:
      tempf = XSysMonPsu_VpVnRawToVoltage(adcData);
      break;

    case SCALING_VOLTAGE_PSIO:
      tempf = XSysMonPsu_VccopsioRawToVoltage(adcData);
      break;

    // gcov-jst 3 Default case for robustness purposes only. It will never be executed.
    default:
      tempf = 0.0f;
      break;
  }

  *pValue = tempf;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_ANALOG_Bit -->

  Read all channels and verify values are within tolerance.
************************************************************************/
d_Status_t
d_ANALOG_Bit
(
  d_ANALOG_Results_t results
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  Float32_t value;
  
  if (results == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  for (Uint32_t index = 0; index < (Uint32_t)d_ANALOG_CHANNEL_COUNT; index++)
  {
    if ((channels[index].minimum != 0.0f) || (channels[index].maximum != 0.0f))
    {
      (void)d_ANALOG_Read(index, &value);
      results[index].value = value;
      if (value < channels[index].minimum)
      {
        // gcov-jst 2 It is not possible to generate this failure during bench testing.
        results[index].bitStatus = d_ANALOG_BIT_BELOW_MINIMUM;
        status = d_STATUS_FAILURE;
      }
      else if (value > channels[index].maximum)
      {
        // gcov-jst 2 It is not possible to generate this failure during bench testing.
        results[index].bitStatus = d_ANALOG_BIT_ABOVE_MAXIMUM;
        status = d_STATUS_FAILURE;
      }
      else
      {
        results[index].bitStatus = d_ANALOG_BIT_PASS;
      }
    }
    else
    {
      results[index].bitStatus = d_ANALOG_BIT_NOT_CHECKED;
    }
  }
  
  return status;
}


