/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Timer Counter

  Abstract           : Control of Zynq UltraScale triple timer counters

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-328
                       SDD References: 136T-2200-131000-001-D22 SWDES-148
                                                                SWDES-149
                                                                SWDES-150
                                                                SWDES-151
                                                                SWDES-152
                                                                SWDES-153
                                                                SWDES-154
                                                                SWDES-155
                                                                SWDES-156
                                                                SWDES-157
\note
  CSC ID             : SWDES-53
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "soc/timer/d_timer_counter.h"

/***** Constants ********************************************************/

/* Clock control register */
#define TIMER_TTC_CLK_CNTRL_OFFSET     0x00000000u
#define TIMER_TTC_CLK_CNTRL_PS_MASK    0x0000001Fu
#define TIMER_TTC_CLK_CNTRL_PS_ENABLE  0x00000001u

/* Counter control register */
#define TIMER_TTC_CNT_CNTRL_OFFSET   0x0000000Cu
#define TIMER_TTC_CNT_CNTRL_DIS_MASK 0x00000001u
#define TIMER_TTC_CNT_CNTRL_INT_MASK 0x00000002u
#define TIMER_TTC_CNT_CNTRL_RST_MASK 0x00000010u

/* Counter value register */
#define TIMER_TTC_COUNT_VALUE_OFFSET 0x00000018u

/* Interval counter register */
#define TIMER_TTC_COUNT_INTERVAL_OFFSET 0x00000024u

/* Interrupt status register */
#define TIMER_TTC_INT_STATUS_OFFSET     0x00000054u

/* Interrupt enable register */
#define TIMER_TTC_INT_ENABLE_OFFSET  0x00000060u

/* Maximum prescales register = 2^16 = 65536 */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t maxPrescaleValue = 15;

/* Timer base addresses */
static const Uint32_t DeviceBaseAddress[d_TIMER_COUNT] =
{
  0xFF110000u,
  0xFF110004u,
  0xFF110008u,
  0xFF120000u,
  0xFF120004u,
  0xFF120008u,
  0xFF130000u,
  0xFF130004u,
  0xFF130008u,
  0xFF140000u,
  0xFF140004u,
  0xFF140008u
};

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_TIMER_Configure -->

  Configure timer.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Configure
(
const d_Timer_t timer,           /**< Timer */
const Bool_t prescalerEnable,    /**< Prescaler enable */
const Uint32_t prescalerValue    /**< Prescaler enable */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if ((prescalerEnable != d_TRUE) && (prescalerEnable != d_FALSE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, prescalerEnable, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if (prescalerValue > maxPrescaleValue)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, prescalerValue, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    /* Disable timer to configure */
    (void)d_TIMER_Stop(timer);

    /* Disable interrupts */
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_INT_ENABLE_OFFSET;
    d_GEN_RegisterWrite(registerAddress, 0u);

    /* Setup prescaler */
    registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_CLK_CNTRL_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress) & ~TIMER_TTC_CLK_CNTRL_PS_MASK;
    if (prescalerEnable == d_TRUE)
    {
      registerValue = registerValue | TIMER_TTC_CLK_CNTRL_PS_ENABLE | (prescalerValue << 1u);
    }
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_Options -->

  Start timer.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Options
(
const d_Timer_t timer,           /**< Timer */
const Bool_t intervalMode        /**< Interval mode selection */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if ((intervalMode != d_TRUE) && (intervalMode != d_FALSE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, intervalMode, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    /* Disable timer to configure */
    (void)d_TIMER_Stop(timer);

    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_CNT_CNTRL_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress);
    if (intervalMode == d_TRUE)
    {
      registerValue = registerValue | TIMER_TTC_CNT_CNTRL_INT_MASK;
    }
    else
    {
      registerValue = registerValue & ~TIMER_TTC_CNT_CNTRL_INT_MASK;
    }
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_Interval -->

  Set timer interval.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Interval
(
const d_Timer_t timer,           /**< Timer */
const Uint32_t interval          /**< Interval count value */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    /* Disable timer to configure */
    (void)d_TIMER_Stop(timer);

    /* Set the interval value */
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_COUNT_INTERVAL_OFFSET;
    d_GEN_RegisterWrite(registerAddress, interval);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_Start -->

  Start timer.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Start
(
const d_Timer_t timer            /**< Timer */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_CNT_CNTRL_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress) & ~TIMER_TTC_CNT_CNTRL_DIS_MASK;
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_Stop -->

  Stop timer.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Stop
(
const d_Timer_t timer            /**< Timer */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_CNT_CNTRL_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress) | TIMER_TTC_CNT_CNTRL_DIS_MASK;
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_Reset -->

  Reset timer.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Reset
(
const d_Timer_t timer            /**< Timer */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_CNT_CNTRL_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress) | TIMER_TTC_CNT_CNTRL_RST_MASK;
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_Read -->

  Read timer value.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_Read
(
const d_Timer_t timer,           /**< Timer */
Uint32_t * const pValue          /**< Pointer to storage for value */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_COUNT_VALUE_OFFSET;
    *pValue = d_GEN_RegisterRead(registerAddress);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_InterruptEnable -->

  Enable timer interrupt.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_InterruptEnable
(
const d_Timer_t timer,           /**< Timer */
d_Timer_Interrupt_t interrupt    /**< Type of interrupt */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if (interrupt >= d_TIMER_INTERRUPT_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, interrupt, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_INT_ENABLE_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress) | (0x01u << (Uint32_t)interrupt);
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_InterruptDisable -->

  Enable timer interrupt.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_InterruptDisable
(
const d_Timer_t timer,           /**< Timer */
d_Timer_Interrupt_t interrupt    /**< Type of interrupt */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if (interrupt >= d_TIMER_INTERRUPT_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, interrupt, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_INT_ENABLE_OFFSET;
    Uint32_t registerValue = d_GEN_RegisterRead(registerAddress) & ~(0x01u << (Uint32_t)interrupt);
    d_GEN_RegisterWrite(registerAddress, registerValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_TIMER_InterruptStatus -->

  Read interrupt status
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_TIMER_InterruptStatus
(
const d_Timer_t timer,           /**< Timer */
Uint32_t * const pValue          /**< Pointer to storage for value */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (timer >= d_TIMER_COUNT)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, timer, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else if (pValue == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    status = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    Uint32_t registerAddress = DeviceBaseAddress[timer] + TIMER_TTC_INT_STATUS_OFFSET;
    *pValue = d_GEN_RegisterRead(registerAddress);
  }

  return status;
}

