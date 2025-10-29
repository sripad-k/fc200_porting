/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_fcu.c

  Abstract           : FCU Specific interface.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-759
                                                                SWREQ-760
                                                                SWREQ-761
                       SDD References: 136T-2200-131100-001-D22 SWDES-890
                                                                SWDES-2446
                                                                SWDES-2447
                                                                SWDES-2448
                                                                SWDES-2449
                                                                SWDES-2450
                                                                SWDES-2451
                                                                SWDES-2452
                                                                SWDES-2453
\note
  CSC ID             : SWDES-887
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "sru/fcu/d_fcu.h"
#include "sru/fcu/d_fcu_cfg.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/error_handler/d_error_handler.h"
#include "sru/platform/ioc.h"

#include "xparameters.h"

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t REGISTER_STATUS_OFFSET = 0x00u;
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t REGISTER_SCORE_OFFSET = 0x04u;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Uint32_t currentMaster;
static Uint32_t slotNumber;

static Bool_t stateGse;
static Bool_t stateProg;
static Bool_t stateMaint;
static Bool_t stateHils;

/***** Function Declarations ********************************************/

static void latchMaintenanceDiscretes(void);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_FCU_Initialise -->

  Initialise discretes.
*************************************************************************/
d_Status_t                    /** \return None */
d_FCU_Initialise
(
void
)
{
  for (Uint32_t index = (Uint32_t)d_FCU_LED1; index <= (Uint32_t)d_FCU_LED8; index++)
  {
    (void)d_DISC_SetAsOutputPin(index);
  }

  for (Uint32_t index = (Uint32_t)d_FCU_GSE; index <= (Uint32_t)d_FCU_HILS; index++)
  {
    (void)d_DISC_SetAsInputPin(index);
  }

  latchMaintenanceDiscretes();

  for (Uint32_t index = (Uint32_t)d_FCU_SAFE_LOOPBACK1; index <= (Uint32_t)d_FCU_SAFE_FAULT2; index++)
  {
    (void)d_DISC_SetAsInputPin(index);
  }

  for (Uint32_t index = (Uint32_t)d_FCU_MASTER_FCU1; index <= (Uint32_t)d_FCU_MASTER_FCU3; index++)
  {
    (void)d_DISC_SetAsInputPin(index);
    (void)d_DISC_InterruptEnable(index, d_DISC_TRIGGER_EDGE_FALLING);
  }

  for (Uint32_t index = (Uint32_t)d_FCU_NOGO_FCU1; index <= (Uint32_t)d_FCU_NOGO_FCU3; index++)
  {
    (void)d_DISC_SetAsOutputPin(index);
    (void)d_DISC_ClearPin(index);
  }

  (void)d_DISC_SetAsInputPin(d_FCU_SLOT1);
  (void)d_DISC_SetAsInputPin(d_FCU_SLOT2);

  slotNumber = d_FCU_SlotNumber();
  currentMaster = d_FCU_GetMaster();
  if (slotNumber == currentMaster)
  {
    d_FCU_SetMaster();
  }
  else
  {
    d_FCU_SetSlave();
  }

  /* Enable the discrete interrupt */
  (void)d_INT_IrqEnable(XPS_GPIO_INT_ID);
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_FCU_SetMaster -->

  Tell the PL that this FCU is Master. This is for synchronisation only
  and should be set based on master / slave selection received from IOC
  using function d_FCU_GetMaster.
*************************************************************************/
void                          /** \return None */
d_FCU_SetMaster
(
void
)
{
  d_GEN_RegisterWrite((Uint32_t)XPAR_SYNCHRONISER_BASEADDR, d_GEN_RegisterRead((Uint32_t)XPAR_SYNCHRONISER_BASEADDR) | 0x00000001u);

  return;
}

/*********************************************************************//**
  <!-- d_FCU_SetSlave -->

  Tell the PL that this FCU is Slave. This is for synchronisation only
  and should be set based on master / slave selection received from IOC
  using function d_FCU_GetMaster.
*************************************************************************/
void                          /** \return None */
d_FCU_SetSlave
(
void
)
{
  d_GEN_RegisterWrite((Uint32_t)XPAR_SYNCHRONISER_BASEADDR, d_GEN_RegisterRead((Uint32_t)XPAR_SYNCHRONISER_BASEADDR) & 0xFFFFFFFEu);

  return;
}

/*********************************************************************//**
  <!-- d_FCU_SlotNumber -->

  Get FCU slot number.
*************************************************************************/
Uint32_t                      /** \return Slot number */
d_FCU_SlotNumber
(
void
)
{
  Uint32_t pin1Value;
  Uint32_t pin2Value;
  Uint32_t slot;

  (void)d_DISC_ReadPin(d_FCU_SLOT1, &pin1Value);
  (void)d_DISC_ReadPin(d_FCU_SLOT2, &pin2Value);
  Uint32_t slotValue = pin1Value | (pin2Value << 1u);
  /* If both high then board probably not plugged into FC. Set slot to zero */
  if (slotValue == 3u)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    slot = 0;
  }
  else
  {
    slot = 2u - slotValue;
  }

  return slot;
}

/*********************************************************************//**
  <!-- d_FCU_GetMaster -->

  Get which FCU is master.
*************************************************************************/
Int32_t                       /** \return Slot number */
d_FCU_GetMaster
(
void
)
{
  Uint32_t pinValue;
  Int32_t master = 3; /* Invalid */

  (void)d_DISC_ReadPin(d_FCU_MASTER_FCU1, &pinValue);
  if (pinValue == 0u)
  {
    /* FCU1 selected */
    master = 0;
  }
  else
  {
    DO_NOTHING();
  }

  (void)d_DISC_ReadPin(d_FCU_MASTER_FCU2, &pinValue);
  if (pinValue == 0u)
  {
    /* Ensure no previous selection */
    if (master == 3)
    {
      /* FCU2 selected */
      master = 1;
    }
    else
    {
      /* Multiple masters selected */
      // gcov-jst 1 It is not practical to execute this code during bench testing.
      master = -1; /* invalid */
    }
  }
  else
  {
    DO_NOTHING();
  }

  (void)d_DISC_ReadPin(d_FCU_MASTER_FCU3, &pinValue);
  if (pinValue == 0u)
  {
    /* Ensure no previous selection */
    // gcov-jst 4 It is not possible to execute this code during bench testing with FC-200.
    if (master == 3)
    {
      /* FCU3 selected */
      master = 2;
    }
    else
    {
      /* Multiple masters selected */
      // gcov-jst 1 It is not practical to execute this code during bench testing.
      master = -1; /* invalid */
    }
  }
  else
  {
    DO_NOTHING();
  }

  /* If no master selected then set output as invalid */
  if (master == 3)
  {
    // gcov-jst 1 It is not practical to execute this code during bench testing.
    master = -1;
  }
  else
  {
    DO_NOTHING();
  }

  return master;
}

/*********************************************************************//**
  <!-- d_FCU_ArbiterStatusRead -->

  Read arbiter status register.
*************************************************************************/
d_Status_t                    /** \return operation status */
d_FCU_ArbiterStatusRead
(
const Uint32_t ioc,           /**< {in]  IOC number */
Uint32_t * const pData        /**< {out] Arbiter status register */
)
{
  Uint32_t regValue;

  if (ioc >= (Uint32_t)d_FCU_IOC_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, ioc, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pData == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (d_FCU_IocOnline(ioc) != d_TRUE)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    // cppcheck-suppress misra-c2012-10.4; Adding integer to error code is the most efficient method of implementing this
    return d_STATUS_OFFLINE_IOCA + ioc;
  }
  
  if (ioc == 0u)
  {
    regValue = d_GEN_RegisterRead((Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ARBITER_REG_BASEADDR + REGISTER_STATUS_OFFSET);
  }
  else
  {
    regValue = d_GEN_RegisterRead((Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ARBITER_REG_BASEADDR + REGISTER_STATUS_OFFSET);
  }
  
  *pData = regValue;
  
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_FCU_Score -->

  Set the arbitration score for the FCUs.
*************************************************************************/
d_Status_t                    /** \return Funtion status */
d_FCU_Score
(
const d_FCU_Fcu_t fcu,        /**< [in] FCU id */
const Uint8_t score           /**< [in] Score for the FCU */
)
{
  if (fcu >= d_FCU_FCU_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, (Uint32_t)fcu, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t regValue = d_GEN_RegisterRead((Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ARBITER_REG_BASEADDR + REGISTER_SCORE_OFFSET);

  Uint32_t shift = (Uint32_t)fcu * 8u;

  regValue = regValue & ~((Uint32_t)0xFFu << shift);

  regValue = regValue | (Uint32_t)score << shift;

  d_GEN_RegisterWrite((Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ARBITER_REG_BASEADDR + REGISTER_SCORE_OFFSET, regValue);
  d_GEN_RegisterWrite((Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ARBITER_REG_BASEADDR + REGISTER_SCORE_OFFSET, regValue);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_FCU_NoGo -->

  Set the NOGO output for this FCU.
*************************************************************************/
d_Status_t                    /** \return Funtion status */
d_FCU_NoGo
(
const d_FCU_Fcu_t fcu         /**< [in] FCU id */
)
{
  if (fcu >= d_FCU_FCU_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, (Uint32_t)fcu, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  // gcov-jst 3 It is not practical to execute this code during bench testing.
  (void)d_DISC_SetPin(d_FCU_NOGO_FCU1 + (d_DISC_IO_t)fcu);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_FCU_GseActive -->

  Get state of Ground Suport Equipment input.
*************************************************************************/
Bool_t                        /** \return Maintenance discrete state */
d_FCU_GseActive
(
void
)
{
  return stateGse;
}

/*********************************************************************//**
  <!-- d_FCU_ProgActive -->

  Get state of Programming mode input.
*************************************************************************/
Bool_t                        /** \return Maintenance discrete state */
d_FCU_ProgActive
(
void
)
{
  return stateProg;
}

/*********************************************************************//**
  <!-- d_FCU_MaintActive -->

  Get state of Maintenance mode input.
*************************************************************************/
Bool_t                        /** \return Maintenance discrete state */
d_FCU_MaintActive
(
void
)
{
  return stateMaint;
}

/*********************************************************************//**
  <!-- d_FCU_HilsActive -->

  Get state of HILS input.
*************************************************************************/
Bool_t                        /** \return HILS discrete state */
d_FCU_HilsActive
(
void
)
{
  return stateHils;
}

/*********************************************************************//**
  <!-- d_FCU_IocOnline -->

  Get state of IOC online discrete input.
*************************************************************************/
Bool_t                        /** \return Maintenance discrete state */
d_FCU_IocOnline
(
const d_FCU_Ioc_t ioc         /**< [in] IOC identifier */
)
{
  Uint32_t state;
  d_DISC_IO_t pin;

  if (ioc == d_FCU_IOC_A)
  {
    pin = d_FCU_IOCA_LINK_UP;
  }
  else if (ioc == d_FCU_IOC_B)
  {
    pin = d_FCU_IOCB_LINK_UP;
  }
  else
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, (Uint32_t)ioc, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_FALSE;
  }

  (void)d_DISC_ReadPin(pin, &state);

  return (Bool_t) state;
}

/*********************************************************************//**
  <!-- d_FCU_Master_Handler -->

  Interrupt handler on change in master.
*************************************************************************/
void d_FCU_Master_Handler(const Uint32_t selectedMaster)
{
  /* Check if master has changed */
  if (selectedMaster != currentMaster)
  {
    if (slotNumber == selectedMaster)
    {
      d_FCU_SetMaster();
    }
    else
    {
      d_FCU_SetSlave();
    }

    currentMaster = selectedMaster;
    
    /* Notify application */
    if (d_FCU_MasterChanged != NULL)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      d_FCU_MasterChanged(currentMaster);
    }
    else
    {
      DO_NOTHING();
    }
  }

  return;
}

/*********************************************************************//**
  <!-- latchMaintenanceDiscretes -->

  Latch the state of the maintenance discrete signal inputs.
*************************************************************************/
static void latchMaintenanceDiscretes(void)
{
  Uint32_t pinStatus;

  (void)d_DISC_ReadPin(d_FCU_GSE, &pinStatus);
  if (pinStatus != 0u)
  {
    stateGse = d_TRUE;
  }
  else
  {
    stateGse = d_FALSE;
  }

  (void)d_DISC_ReadPin(d_FCU_PROGRAMMING, &pinStatus);
  if (pinStatus != 0u)
  {
    stateProg = d_TRUE;
  }
  else
  {
    stateProg = d_FALSE;
  }

  (void)d_DISC_ReadPin(d_FCU_MAINTENANCE, &pinStatus);
  if (pinStatus != 0u)
  {
    // gcov-jst 3 It is not practical to execute this code during bench testing.
    stateMaint = d_TRUE;
  }
  else
  {
    stateMaint = d_FALSE;
  }

  (void)d_DISC_ReadPin(d_FCU_HILS, &pinStatus);
  if (pinStatus != 0u)
  {
    stateHils = d_TRUE;
  }
  else
  {
    stateHils = d_FALSE;
  }

  return;
}
