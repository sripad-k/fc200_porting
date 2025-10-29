/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_discrete_pl

  Abstract           : General Purpose Input Output functions for the
                       Programmable Logic.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-112
                       SDD References: 136T-2200-131100-001-D22 SWDES-165
                                                                SWDES-166
                                                                SWDES-167
                                                                SWDES-168
                                                                SWDES-169
                                                                SWDES-170
                                                                SWDES-171
                                                                SWDES-192
\note
  CSC ID             : SWDES-55
*************************************************************************/

/***** Includes *********************************************************/

#include "sru/discrete_pl/d_discrete_pl.h"            /* Discrete driver */
#include "sru/discrete_pl/d_discrete_pl_cfg.h"        /* Discrete driver config */
#include "soc/defines/d_common_status.h"              /* Error status */
#include "kernel/error_handler/d_error_handler.h"     /* Error handler */
#include "kernel/general/d_gen_register.h"            /* Register functions */
#include "sru/fcu/d_fcu_cfg.h"                        /* FCU CSC */

/***** Constants ********************************************************/

static const Uint32_t BIT_SET   = 0x00000001u;      /* 32 bit unsigned integer with LS bit set to 1 */

/* PL GPIO address offsets */
#define DISC_CONFIG     0x0000u  /* Discrete configuration */
#define DISC_DATA_OUT   0x0004u  /* Discrete output */
#define DISC_OUT_FAULT  0x0008u  /* Discrete output fault */
#define DISC_OUT_LOOP   0x000Cu  /* Discrete output loopback */
#define DISC_DATA_IN    0x0010u  /* Discrete input */
#define DISC_INT_RE     0x0014u  /* Discrete interrupt rising edge */
#define DISC_INT_FE     0x0018u  /* Discrete interrupt falling edge */
#define DISC_INT_STATUS 0x001Cu  /* Discrete interrupt status */
#define DISC_IN_FAULT   0x0020u  /* Discrete input fault */

#define PIN_COUNT  32u

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t inputPinCount;
  Uint32_t outputPinCount;
} discreteConfig_t;

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

static discreteConfig_t discreteConfig[d_DISC_PL_MAX_INTERFACES];

/***** Function Declarations ********************************************/

static void WritePin(const Uint32_t block,
                     const Uint32_t bit,
                     const Uint32_t dataToWrite);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_DISC_PL_Initialise -->

  Verify configuration and initialise data.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_Initialise
(
void
)
{
  Uint32_t index;
  d_Status_t status = d_STATUS_SUCCESS;
  
  if (d_DISC_PL_Count > d_DISC_PL_MAX_INTERFACES)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_UNKNOWN, 0, 1, d_DISC_PL_Count, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_CONFIGURATION;
  }

  index = 0;
  while ((index < d_DISC_PL_Count) && (status == d_STATUS_SUCCESS))
  {
    if (d_DISC_PL_BaseAddress[index] == 0x00000000u)
    {
      // gcov-jst 3 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 2, index, 0);
      // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
      return d_STATUS_INVALID_CONFIGURATION;
    }
    else
    {
      d_Status_t channelStatus = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[index]);
      if (channelStatus == d_STATUS_SUCCESS)
      {
        discreteConfig[index].inputPinCount = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[index] + DISC_CONFIG) & 0xFFu;

        if (discreteConfig[index].inputPinCount > PIN_COUNT)
        {
          // gcov-jst 3 It is not practical to generate this failure during bench testing.
          d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 3, index, discreteConfig[index].inputPinCount);
          // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
          return d_STATUS_INVALID_CONFIGURATION;
        }
        discreteConfig[index].outputPinCount = (d_GEN_RegisterRead(d_DISC_PL_BaseAddress[index] + DISC_CONFIG) >> 8u) & 0xFFu;
        if (discreteConfig[index].outputPinCount > PIN_COUNT)
        {
          // gcov-jst 3 It is not practical to generate this failure during bench testing.
          d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 4, index, discreteConfig[index].outputPinCount);
          // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
          return d_STATUS_INVALID_CONFIGURATION;
        }
      }
      else
      {
         // gcov-jst 1 It is not practical to generate this failure during bench testing.
      	 status = channelStatus;
      }
    }
    index++;
  }

  initialised = d_TRUE;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_PL_ReadPin -->

  Read the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_ReadPin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit,               /**< [in] Pin to read */
Uint32_t* const pPinStatus        /**< [out] Pointer: return read status of pin*/
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].inputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pPinStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    Uint32_t regValue = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_DATA_IN);

    *pPinStatus = (regValue >> bit) & BIT_SET;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_SetPin -->

  Set the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_SetPin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit                /**< [in] Pin to set */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].outputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    WritePin(block, bit, BIT_SET);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_ClearPin -->

  Clear the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_ClearPin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit                /**< [in] Pin to clear */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].outputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    WritePin(block, bit, 0u);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_TogglePin -->

  Toggle the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_TogglePin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit                /**< [in] Pin to toggle */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].outputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read current value */
    Uint32_t regAddress = d_DISC_PL_BaseAddress[block] + DISC_DATA_OUT;
    Uint32_t regValue = d_GEN_RegisterRead(regAddress);
    /* Invert pin value */
    regValue = regValue ^ (BIT_SET << bit);
    d_GEN_RegisterWrite(regAddress, regValue);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_ReadOutputPin -->

  Read the status of a discrete output pin.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_ReadOutputPin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit,               /**< [in] Pin to read */
Uint32_t* const pPinStatus        /**< [out] Pointer: return read status of pin*/
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].outputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pPinStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    Uint32_t regValue = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_DATA_OUT);

    *pPinStatus = (regValue >> bit) & BIT_SET;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_ReadLoopbackPin -->

  Read the status of a discrete loopback pin.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_ReadLoopbackPin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit,               /**< [in] Pin to read */
Uint32_t* const pPinStatus        /**< [out] Pointer: return read status of pin*/
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].outputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pPinStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    Uint32_t regValue = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_OUT_LOOP);

    *pPinStatus = (regValue >> bit) & BIT_SET;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_ReadFault -->

  Read the fault register.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_ReadFault
(
const Uint32_t block,             /**< [in] Firmware block number */
Uint32_t* const pFault            /**< [out] Pointer: return fault status */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFault == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    *pFault = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_OUT_FAULT);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_ReadFaultInput -->

  Read the fault register.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_ReadFaultInput
(
const Uint32_t block,             /**< [in] Firmware block number */
Uint32_t* const pFault            /**< [out] Pointer: return fault status */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFault == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    *pFault = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_IN_FAULT);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_InterruptEnable -->

  Enable the interrupt for the specified IP block and pin.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_InterruptEnable
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit,               /**< [in] Pin to enable */
const d_DISC_PL_Edge_t edge       /**< [in] Edge to trigger on */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].inputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (edge >= d_DISC_PL_EDGE_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, edge, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t regAddress;
    if (edge == d_DISC_PL_EDGE_RISING)
    {
      regAddress = d_DISC_PL_BaseAddress[block] + DISC_INT_RE;
    }
    else
    {
      regAddress = d_DISC_PL_BaseAddress[block] + DISC_INT_FE;
    }
  
    Uint32_t regValue = d_GEN_RegisterRead(regAddress);
  
    regValue = regValue | (BIT_SET << bit);
  
    d_GEN_RegisterWrite(regAddress, regValue);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_InterruptDisable -->

  Disable the interrupt for the specified IP block and pin.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_InterruptDisable
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit                /**< [in] Pin to disable */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].inputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t regValue = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_INT_RE);
    regValue = regValue & ~(BIT_SET << bit);
    d_GEN_RegisterWrite(d_DISC_PL_BaseAddress[block] + DISC_INT_RE, regValue);
  
    regValue = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_INT_FE);
    regValue = regValue & ~(BIT_SET << bit);
    d_GEN_RegisterWrite(d_DISC_PL_BaseAddress[block] + DISC_INT_FE, regValue);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_InterruptClear -->

  Clear the interrupt from the specified IP block and pin.
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_InterruptClear
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit                /**< [in] Pin to clear */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].inputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t regAddress = d_DISC_PL_BaseAddress[block] + DISC_INT_STATUS;

    Uint32_t regValue = d_GEN_RegisterRead(regAddress);

    if ((regValue & (BIT_SET << bit)) != 0u)
    {
      regValue = regValue | (BIT_SET << bit);

      d_GEN_RegisterWrite(regAddress, regValue);
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_PL_InterruptStatus -->

  Read the interrupt status
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_PL_InterruptStatus
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit,               /**< [in] Pin to read */
Bool_t * const pState             /**< [out] Pointer to storage for state */
)
{
  if (initialised != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (block >= d_DISC_PL_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, block, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (bit >= discreteConfig[block].inputPinCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, bit, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pState == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_Status_t status = d_FCU_IocAddressCheck(d_DISC_PL_BaseAddress[block]);
  if (status == d_STATUS_SUCCESS)
  {
    /* Read register value */
    Uint32_t regValue = d_GEN_RegisterRead(d_DISC_PL_BaseAddress[block] + DISC_INT_STATUS);

    *pState = (regValue >> bit) & BIT_SET;
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- WritePin -->

  Set the status of a discrete pin according to the data bit
*************************************************************************/
static void                       /** \return None */
WritePin
(
const Uint32_t block,             /**< [in] Firmware block number */
const Uint32_t bit,               /**< [in] Bit to set */
const Uint32_t dataToWrite        /**< [in] Data to write to pin - LS bit of 32 bits */
)
{
  Uint32_t regAddress = d_DISC_PL_BaseAddress[block] + DISC_DATA_OUT;
  Uint32_t regValue = d_GEN_RegisterRead(regAddress);
  
  if (dataToWrite == 0u)
  {
    regValue = regValue & ~(BIT_SET << bit);
  }
  else
  {
    regValue = regValue | (BIT_SET << bit);
  }
  
  d_GEN_RegisterWrite(regAddress, regValue);

  return;
}

