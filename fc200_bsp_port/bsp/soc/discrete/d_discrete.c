/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_discrete

  Abstract           : General Purpose Input Output functions for the
                       Processing System and the Programmable Logic.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-114
                       SDD References: 136T-2200-131100-001-D22 SWDES-86
                                                                SWDES-88
                                                                SWDES-89
                                                                SWDES-90
                                                                SWDES-91
                                                                SWDES-92
                                                                SWDES-93

\note
  CSC ID             : SWDES-42
*************************************************************************/

/***** Includes *********************************************************/

#include "xparameters_ps.h"                        /* PS defines for Zynq */
#include "soc/defines/d_common_status.h"           /* Error status */
#include "soc/interrupt_manager/d_int_critical.h"  /* Critical section handler */
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "kernel/general/d_gen_register.h"         /* Register functions */

#include "xgpiops.h"

#include "soc/discrete/d_discrete.h"            /* Discrete CSC */
#include "soc/discrete/d_discrete_cfg.h"        /* Discrete configuration */

/***** Constants ********************************************************/

const static Uint32_t BIT_SET   = 0x00000001u;      /* 32 bit unsigned integer with LS bit set to 1 */
const static Uint32_t BIT_CLEAR = 0x00000000u;      /* 32 bit unsigned integer with LS bit set to 0 */

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static void GetBankPinNumbers(const d_DISC_IO_t pin,
                              Uint32_t* const pBankNumber,
                              Uint32_t* const pPinNumberInBank);

static void SetDirectionPin(const d_DISC_IO_t pin,
                            const Uint32_t direction);

static void SetOutputEnablePin(const d_DISC_IO_t pin,
                               const Uint32_t enable);

static void WritePin(const d_DISC_IO_t pin,
                     const Uint32_t dataToWrite);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_DISC_SetAsInputPin -->

  Set the direction of a discrete pin to an input
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_SetAsInputPin
(
const d_DISC_IO_t pin             /**< [in] Pin to set */
)
{
  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Disable output pin and set as input */
  SetOutputEnablePin(pin, BIT_CLEAR);
  SetDirectionPin(pin, BIT_CLEAR);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_SetAsOutputPin -->

  Set the direction of a discrete pin to an output
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_SetAsOutputPin
(
const d_DISC_IO_t pin             /**< [in] Pin to set */
)
{
  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Set as output and enable output pin */
  SetDirectionPin(pin, BIT_SET);
  SetOutputEnablePin(pin, BIT_SET);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_ReadPin -->

  Read the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of read operation */
d_DISC_ReadPin
(
const d_DISC_IO_t pin,            /**< [in] Pin to read */
Uint32_t* const pPinStatus        /**< [out] Pointer: return read status of pin*/
)
{
  Uint32_t bankNumber;             /* Bank where the pin is located */
  Uint32_t pinNumberInBank;        /* Pin number within the specific bank */

  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pPinStatus == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  GetBankPinNumbers(pin, &bankNumber, &pinNumberInBank);

  /* Read register value and filter the status of the pin */
  Uint32_t regValue = d_GEN_RegisterRead(XPAR_PSU_GPIO_0_BASEADDR +                /* Base address */
                                         XGPIOPS_DATA_RO_OFFSET +                  /* DATA_RO offset */
                                         (bankNumber * XGPIOPS_DATA_BANK_OFFSET)); /* Bank offset */

  *pPinStatus = (regValue >> pinNumberInBank) & BIT_SET;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_SetPin -->

  Set the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of peration */
d_DISC_SetPin
(
const d_DISC_IO_t pin             /**< [in] Pin to set */
)
{
  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  WritePin(pin, BIT_SET);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_ClearPin -->

  Clear the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_ClearPin
(
const d_DISC_IO_t pin             /**< [in] Pin to clear */
)
{
  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  WritePin(pin, BIT_CLEAR);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_TogglePin -->

  Toggle the status of a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_TogglePin
(
const d_DISC_IO_t pin             /**< [in] Pin to toggle */
)
{
  d_Status_t status;
  Uint32_t pinStatus;       /* Read and write status of pin */

  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Read current value */
  status = d_DISC_ReadPin(pin, &pinStatus);

    /* Invert pin value and write back */
  if (status == d_STATUS_SUCCESS)
  {
    pinStatus = (pinStatus ^ BIT_SET) & BIT_SET;
    WritePin(pin, pinStatus);
  }
  else
  {
    // gcov-jst 1 Parameter pin has already been validated so this can never occur. Included for robustness only.
    DO_NOTHING();
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DISC_InterruptEnable -->

  Enable the interrupt on a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_InterruptEnable
(
const d_DISC_IO_t pin,            /**< [in] Pin to enable interupt on */
d_DISC_Trigger_t trigger          /**< [in] Interrupt trigger type */
)
{
  Uint32_t bankNumber;            /* Bank where the pin is located */
  Uint32_t pinNumberInBank;       /* Pin number within the specific bank */
  Uint32_t bit;
  
  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (trigger >= d_DISC_TRIGGER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, trigger, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  GetBankPinNumbers(pin, &bankNumber, &pinNumberInBank);

  /* Calculate register address to read and write */
  Uint32_t regAddress = ((Uint32_t)XPAR_PSU_GPIO_0_BASEADDR +                          /* Base address */
                         ((Uint32_t)bankNumber * (Uint32_t)XGPIOPS_REG_MASK_OFFSET));  /* Bank offset */

  /* Set trigger type */
  bit = (Uint32_t)trigger / 2u;
  d_GEN_RegisterWriteMask(regAddress + (Uint32_t)XGPIOPS_INTTYPE_OFFSET, (BIT_SET << pinNumberInBank), (bit << pinNumberInBank));

  /* Set trigger polarity */
  bit = (Uint32_t)trigger & 0x01u;
  d_GEN_RegisterWriteMask(regAddress + (Uint32_t)XGPIOPS_INTPOL_OFFSET, (BIT_SET << pinNumberInBank), (bit << pinNumberInBank));

  /* Enable interrupt */
  d_GEN_RegisterWrite(regAddress + (Uint32_t)XGPIOPS_INTEN_OFFSET, (BIT_SET << pinNumberInBank));
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_InterruptDisable -->

  Disable the interrupt on a discrete pin
*************************************************************************/
d_Status_t                        /** \return Status of operation */
d_DISC_InterruptDisable
(
const d_DISC_IO_t pin             /**< [in] Pin to disable interupt on */
)
{
  Uint32_t bankNumber;            /* Bank where the pin is located */
  Uint32_t pinNumberInBank;       /* Pin number within the specific bank */

  if (pin >= d_DISC_IO_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, pin, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  GetBankPinNumbers(pin, &bankNumber, &pinNumberInBank);

  /* Calculate register address to read and write */
  Uint32_t regAddress = ((Uint32_t)XPAR_PSU_GPIO_0_BASEADDR +                          /* Base address */
                         (Uint32_t)XGPIOPS_INTDIS_OFFSET +                             /* INTDIS register offset */
                         ((Uint32_t)bankNumber * (Uint32_t)XGPIOPS_REG_MASK_OFFSET));  /* Bank offset */

  d_GEN_RegisterWrite(regAddress, (BIT_SET << pinNumberInBank));
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DISC_GetBankPinNumbers -->

  Convert Pin to Bank and Pin number within the bank
*************************************************************************/
void                              /** \return None */
d_DISC_InterruptHandler
(
const Uint32_t parameter          /**< [in] Interrupt parameter, unused by this function */
)
{
  UNUSED_PARAMETER(parameter);

  Uint32_t bitCount;
  Uint32_t pinId = 0u;
   
  for (Uint32_t bank = 0u; bank < 6u; bank++)
  {
    /* Calculate register address to read and write */
    Uint32_t regAddress = ((Uint32_t)XPAR_PSU_GPIO_0_BASEADDR +                          /* Base address */
                           ((Uint32_t)bank * (Uint32_t)XGPIOPS_REG_MASK_OFFSET));  /* Bank offset */

    Uint32_t intMask = d_GEN_RegisterRead(regAddress + (Uint32_t)XGPIOPS_INTMASK_OFFSET);
    Uint32_t intStatus = d_GEN_RegisterRead(regAddress + (Uint32_t)XGPIOPS_INTSTS_OFFSET);
  
    if (bank < 3u)
    {
      /* MIO banks have 26 discrete bits */
      bitCount = 26u;
    }
    else
    {
      /* EMIO banks have 32 discrete bits */
      bitCount = 32u;
    }

    for (Uint32_t bit = 0u; bit < bitCount; bit++)
    {
      Uint32_t mask = 0x01u << bit;
    
      /* Check if unmasked and has triggered */
      if (((intMask & mask) == 0u) && ((intStatus & mask) != 0u))
      {
        /* Execute the handler function if not NULL */
        if (d_DISC_IrqVectorTable[pinId].function != NULL)
        {
          d_DISC_IrqVectorTable[pinId].function(d_DISC_IrqVectorTable[pinId].parameter);
        }
        else
        {
          /* Log error if no handler */
          // gcov-jst 1 It is not practical to execute this code during bench testing. There are no discrete inputs we can control.
          d_ERROR_Logger(d_STATUS_IRQ_WITHOUT_HANDLER, d_ERROR_CRITICALITY_UNKNOWN, pinId, 1, 0, 0);
        }
        
        /* Clear the interrupt flag */
        d_GEN_RegisterWrite(regAddress + (Uint32_t)XGPIOPS_INTSTS_OFFSET, mask);
      }
      
      pinId++;
    }
  }
  
  return;
}

/*********************************************************************//**
  <!-- d_DISC_GetBankPinNumbers -->

  Convert Pin to Bank and Pin number within the bank
*************************************************************************/
static void                       /** \return None */
GetBankPinNumbers
(
const d_DISC_IO_t pin,            /**< [in]  Pin to convert */
Uint32_t* const pBankNumber,      /**< [out] Pointer: Bank where the pin is located */
Uint32_t* const pPinNumberInBank  /**< [out] Pointer: Pin number within the specific bank */
)
{
  const d_DISC_IO_t BANK0_MAX_PIN_NUM = d_DISC_MIO_25;   /* Bank 0, Pins 0   -  25 */
  const d_DISC_IO_t BANK1_MAX_PIN_NUM = d_DISC_MIO_51;   /* Bank 1, Pins 26  -  51 */
  const d_DISC_IO_t BANK2_MAX_PIN_NUM = d_DISC_MIO_77;   /* Bank 2, Pins 52  -  77 */
  const d_DISC_IO_t BANK3_MAX_PIN_NUM = d_DISC_EMIO_31;  /* Bank 3, Pins 78  - 109 */
  const d_DISC_IO_t BANK4_MAX_PIN_NUM = d_DISC_EMIO_62;  /* Bank 4, Pins 110 - 141 */

  if (pin <= BANK0_MAX_PIN_NUM)                   /* Bank 0, Pins 0 - 25 */
  {
    *pBankNumber = 0u;
    *pPinNumberInBank = pin;
  }
  else if (pin <= BANK1_MAX_PIN_NUM)              /* Bank 1, Pins 26 - 51 */
  {
    *pBankNumber = 1u;
    *pPinNumberInBank = (Uint32_t)pin - (Uint32_t)BANK0_MAX_PIN_NUM - 1u;
  }
  else if (pin <= BANK2_MAX_PIN_NUM)              /* Bank 2, Pins 52 - 77 */
  {
    *pBankNumber = 2u;
    *pPinNumberInBank = (Uint32_t)pin - (Uint32_t)BANK1_MAX_PIN_NUM - 1u;
  }
  else if (pin <= BANK3_MAX_PIN_NUM)              /* Bank 3, Pins 78 - 109 */
  {
    *pBankNumber = 3u;
    *pPinNumberInBank = (Uint32_t)pin - (Uint32_t)BANK2_MAX_PIN_NUM - 1u;
  }
  else if (pin <= BANK4_MAX_PIN_NUM)              /* Bank 4, Pins 110 - 141 */
  {
    *pBankNumber = 4u;
    *pPinNumberInBank = (Uint32_t)pin - (Uint32_t)BANK3_MAX_PIN_NUM - 1u;
  }
  else                                            /* Bank 5, Pins 142 - 173 */
  {
    *pBankNumber = 5u;
    *pPinNumberInBank = (Uint32_t)pin - (Uint32_t)BANK4_MAX_PIN_NUM - 1u;
  }

  return;
}

/*********************************************************************//**
  <!-- d_DISC_SetDirectionPin -->

  Set the direction of a discrete pin to either input or output
*************************************************************************/
static void                       /** \return None */
SetDirectionPin
(
const d_DISC_IO_t pin,            /**< [in] Pin to set */
const Uint32_t direction          /**< [in] Set the pin to this direction */
)
{
  Uint32_t bankNumber;            /* Bank where the pin is located */
  Uint32_t pinNumberInBank;       /* Pin number within the specific bank */

  GetBankPinNumbers(pin, &bankNumber, &pinNumberInBank);

  /* Calculate register address to read and write */
  Uint32_t regAddress = ((Uint32_t)XPAR_PSU_GPIO_0_BASEADDR +                          /* Base address */
                         (Uint32_t)XGPIOPS_DIRM_OFFSET +                               /* DIRM register offset */
                         ((Uint32_t)bankNumber * (Uint32_t)XGPIOPS_REG_MASK_OFFSET)); /* Bank offset */

  Uint32_t flags = d_INT_CriticalSectionEnter();
  /* Read current value of direction mode register */
  Uint32_t dirModeReg = d_GEN_RegisterRead(regAddress);

  /* Update direction mode register */
  if (direction != BIT_CLEAR) /*  Output Direction */
  {
    dirModeReg = dirModeReg | (BIT_SET << pinNumberInBank);
  }
  else /* Input Direction */
  {
    dirModeReg =  dirModeReg & ~(BIT_SET << pinNumberInBank);
  }

  /* Write back new value to direction mode register */
  d_GEN_RegisterWrite(regAddress, dirModeReg);
  d_INT_CriticalSectionLeave(flags);
  
  return;
}

/*********************************************************************//**
  <!-- d_DISC_SetOutputEnablePin -->

  Enabled or disable an output discrete pin
*************************************************************************/
static void                       /** \return None */
SetOutputEnablePin
(
const d_DISC_IO_t pin,            /**< [in] Pin to set */
const Uint32_t enable             /**< [in] Set the pin output enable */
)
{
  Uint32_t bankNumber;            /* Bank where the pin is located */
  Uint32_t pinNumberInBank;       /* Pin number within the specific bank */

  GetBankPinNumbers(pin, &bankNumber, &pinNumberInBank);

  /* Calculate register address to read and write */
  Uint32_t regAddress = ((Uint32_t)XPAR_PSU_GPIO_0_BASEADDR +               /* Base address */
                         (Uint32_t)XGPIOPS_OUTEN_OFFSET +                   /* OUTEN register offset */
                         (bankNumber * (Uint32_t)XGPIOPS_REG_MASK_OFFSET)); /* Bank offset */


  Uint32_t flags = d_INT_CriticalSectionEnter();
  /* Read current value of output enable register */
  Uint32_t outputEnableReg = d_GEN_RegisterRead(regAddress);

  if (enable != BIT_CLEAR) /* Enable output */
  {
    outputEnableReg = outputEnableReg | (BIT_SET << pinNumberInBank);
  }
  else /* Disable output */
  {
    outputEnableReg =  outputEnableReg & ~(BIT_SET << pinNumberInBank);
  }

  /* Write back new value to output enable register */
  d_GEN_RegisterWrite(regAddress, outputEnableReg);
  d_INT_CriticalSectionLeave(flags);

  return;
}

/*********************************************************************//**
  <!-- d_DISC_WritePin -->

  Set the status of a discrete pin according to the data bit
*************************************************************************/
static void                       /** \return None */
WritePin
(
const d_DISC_IO_t pin,            /**< [in] Pin to write */
const Uint32_t dataToWrite        /**< [in] Data to write to pin - LS bit of 32 bits */
)
{
  Uint32_t bankNumber;                        /* Bank where the pin is located */
  Uint32_t pinNumberInBank;                   /* Pin number within the specific bank */
  Uint32_t regAddress;                        /* Register address */

  GetBankPinNumbers(pin, &bankNumber, &pinNumberInBank);

  regAddress = ((Uint32_t)XPAR_PSU_GPIO_0_BASEADDR +            /* Base address */
                (Uint32_t)XGPIOPS_DATA_OFFSET +                 /* Data register offset */
                (bankNumber * (Uint32_t)XGPIOPS_DATA_BANK_OFFSET));  /* Data offset */

  Uint32_t flags = d_INT_CriticalSectionEnter();
  Uint32_t outputReg = d_GEN_RegisterRead(regAddress);

  if (dataToWrite != BIT_CLEAR) /* Enable output */
  {
    outputReg = outputReg | (BIT_SET << pinNumberInBank);
  }
  else /* Disable output */
  {
    outputReg =  outputReg & ~(BIT_SET << pinNumberInBank);
  }

  d_GEN_RegisterWrite(regAddress, outputReg);
  d_INT_CriticalSectionLeave(flags);

  return;
}

