/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : IRQ Exception Handler

  Abstract : Handler for IRQ interrupt exceptions.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-164
                                                                SWREQ-165
                                                                SWREQ-166
                                                                SWREQ-167
                       SDD References: 136T-2200-131000-001-D22 SWDES-172
                                                                SWDES-173
                                                                SWDES-174
                                                                SWDES-175
                                                                SWDES-176
                                                                SWDES-177
                                                                SWDES-178
                                                                SWDES-179
                                                                SWDES-180
                                                                SWDES-181
                                                                SWDES-182
                                                                SWDES-183
                                                                SWDES-184
                                                                SWDES-185
                                                                SWDES-186
                                                                SWDES-187
                                                                SWDES-188
\note
  CSC ID             : SWDES-52
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/defines/d_common_asm.h"
#include "kernel/general/d_gen_register.h"

#include "xparameters.h"

#include "kernel/error_handler/d_error_handler.h"

#include "xscugic_hw.h"
#include "xscugic.h"

#include "d_int_irq_handler.h"
#include "d_int_irq_table.h"
#include "d_int_critical.h"

/***** Constants ********************************************************/

/* Interrupt Controller CPU register base address */
#define CPU_BASEADDR     XPAR_SCUGIC_0_CPU_BASEADDR

/* Interrupt Controller Distributor register base address */
#define DIST_BASEADDR    XPAR_SCUGIC_0_DIST_BASEADDR

/* All interrupt priority 0xA0 */
#define DEFAULT_PRIORITY    0xa0a0a0a0UL

/* Interrupt enable flags */
#define CPSR_IRQ_ENABLE      0x80u
#define CPSR_FIQ_ENABLE      0x40u

/* IPI APU Interrupt */
#define IPI_APU_IRQ             65u
#define IPI_APU_PROCESSOR_MASK  0x01u

/* IPI Registers */
#define IPI_RPU_BASE_ADDRESS   0xFF310000u
#define IPI_TRIG_OFFSET        0x00000000u
#define IPI_ISR_OFFSET         0x00000010u
#define IPI_IER_OFFSET         0x00000018u
#define IPI_IDR_OFFSET         0x0000001Cu

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static void GicDistInit(void);
static void GicCPUInit(void);

static void WriteDistReg(Uint32_t offset, Uint32_t value);
static Uint32_t ReadDistReg(Uint32_t offset);
static void WriteCpuReg(Uint32_t offset, Uint32_t value);
static Uint32_t ReadCpuReg(Uint32_t offset);

extern XScuGic_Config XScuGic_ConfigTable[1];

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_INT_IrqHandler -->

  Handler for IRQ exceptions.
*************************************************************************/
void                     /** \return None */
d_INT_IrqHandler
(
void
)
{
  Uint32_t IntIDFull;
  Uint32_t IrqNumber;
  Bool_t serviced = d_FALSE;

  IntIDFull = ReadCpuReg(XSCUGIC_INT_ACK_OFFSET);
  IrqNumber = IntIDFull & XSCUGIC_ACK_INTID_MASK;

  if (IrqNumber < MAXIMUM_IRQ)
  {
    if (IrqVectorTable[IrqNumber].function != NULL)
    {
      IrqVectorTable[IrqNumber].function(IrqVectorTable[IrqNumber].parameter);
      serviced = d_TRUE;
    }
    else
    {
      /* Check if old table exists */
      if (XScuGic_ConfigTable[0].HandlerTable != NULL)
      {
        XScuGic_VectorTableEntry *TablePtr = &(XScuGic_ConfigTable[0].HandlerTable[IrqNumber]);
        if (TablePtr->Handler != NULL)
        {
          TablePtr->Handler(TablePtr->CallBackRef);
          serviced = d_TRUE;
        }
        else
        {
          DO_NOTHING();
        }
      }
      else
      {
        DO_NOTHING();
      }
    }

    if (serviced == d_FALSE)
    {
      /* Log error if no handler */
      d_ERROR_Logger(d_STATUS_IRQ_WITHOUT_HANDLER, d_ERROR_CRITICALITY_NON_CRITICAL, IrqNumber, 0, 0, 0);
    }
    else
    {
      DO_NOTHING();
    }

#ifndef FREERTOS
    /* Write to the EOI register, we are all done here. */
    if (IrqNumber != SchedulerInterrupt)
    {
      WriteCpuReg(XSCUGIC_EOI_OFFSET, IntIDFull);
    }
    else
    {
      DO_NOTHING();
    }
#endif
  }
#ifndef FREERTOS
  else
  {
    // gcov-jst 1 It is not possible to generate this error during bench testing.
    WriteCpuReg(XSCUGIC_EOI_OFFSET, IntIDFull);
  }
#else
  /* Write to the EOI register, we are all done here. */
  WriteCpuReg(XSCUGIC_EOI_OFFSET, IntIDFull);
#endif
  return;
}

/*********************************************************************//**
  <!-- d_INT_IrqDeviceInitialise -->

  Initialise the interrupt handler.
*************************************************************************/
void                     /** \return None */
d_INT_IrqDeviceInitialise
(
void
)
{
  GicDistInit();

  GicCPUInit();

  return;
}

/*********************************************************************//**
  <!-- d_INT_IrqEnable -->

  Enable the specified interrupt.
  This assumes the interrupt is directed to the first core
*************************************************************************/
d_Status_t            /** \return Function status */
d_INT_IrqEnable
(
const Uint32_t irq    /**< [in] interrupt to enable */
)
{
  Uint32_t regValue;
  Uint32_t offset;
  Uint32_t cpuId;

  if (irq >= MAXIMUM_IRQ)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, irq, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Direct shared peripheral interrupt to first core */
  regValue = ReadDistReg(XSCUGIC_SPI_TARGET_OFFSET_CALC(irq));

  offset = (irq & 0x03u);
  cpuId = 0x1u;

  regValue |= cpuId << (offset * 8u);
  WriteDistReg(XSCUGIC_SPI_TARGET_OFFSET_CALC(irq), regValue);

  /* Enable the interrupt */
  // cppcheck-suppress misra-c2012-12.2; The modulo 32 ensures the shift is less than 32 bits
  WriteDistReg(XSCUGIC_ENABLE_SET_OFFSET + ((irq / 32u) * 4u), 0x00000001u << (irq % 32u));

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_INT_IrqDisable -->

  Disable the specified interrupt.
*************************************************************************/
d_Status_t            /** \return Function status */
d_INT_IrqDisable
(
const Uint32_t irq    /**< [in] interrupt to disable */
)
{
  if (irq >= MAXIMUM_IRQ)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, irq, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  // cppcheck-suppress misra-c2012-12.2; The modulo 32 ensures the shift is less than 32 bits
  WriteDistReg(XSCUGIC_DISABLE_OFFSET + ((irq / 32u) * 4u), 0x00000001u << (irq % 32u));

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_INT_ApuEnable -->

  Enable the APU interrupt.
*************************************************************************/
void d_INT_ApuEnable(void)
{
  /* Enable IPI interrupt from APU */
  d_GEN_RegisterWrite(IPI_RPU_BASE_ADDRESS + IPI_IER_OFFSET, IPI_APU_PROCESSOR_MASK);

  /* Enable RPU_0 designated IPI interrupt */
  (void)d_INT_IrqEnable(IPI_APU_IRQ);

  return;
}

/*********************************************************************//**
  <!-- d_INT_ApuDisable -->

  Disable the APU interrupt.
*************************************************************************/
void d_INT_ApuDisable(void)
{
  /* Disable IPI interrupt from APU */
  d_GEN_RegisterWrite(IPI_RPU_BASE_ADDRESS + IPI_IDR_OFFSET, IPI_APU_PROCESSOR_MASK);

  /* Enable RPU_0 designated IPI interrupt */
  (void)d_INT_IrqDisable(IPI_APU_IRQ);

  return;
}

/*********************************************************************//**
  <!-- d_INT_ApuAck -->

  Acknowledge the APU interrupt.
*************************************************************************/
void d_INT_ApuAck(void)
{
  /* Read the ISR status register */
  Uint32_t regValue = d_GEN_RegisterRead(IPI_RPU_BASE_ADDRESS + IPI_ISR_OFFSET);
  /* Wite value back to clear any interrupt flags */
  d_GEN_RegisterWrite(IPI_RPU_BASE_ADDRESS + IPI_ISR_OFFSET, regValue);

  return;
}

/*********************************************************************//**
  <!-- d_INT_Enable -->

  Enable interrupts.
*************************************************************************/
void                     /** \return None */
d_INT_Enable
(
void
)
{
  Uint64_t statusRegister;

  statusRegister = d_mfcpsr();
  d_mtcpsr(statusRegister & ~CPSR_IRQ_ENABLE);

  return;
}

/*********************************************************************//**
  <!-- d_INT_Disable -->

  Disable interrupts.
*************************************************************************/
void                     /** \return None */
d_INT_Disable
(
void
)
{
  Uint64_t statusRegister;

  statusRegister = d_mfcpsr();
  d_mtcpsr(statusRegister | CPSR_IRQ_ENABLE | CPSR_FIQ_ENABLE);

  return;
}


/*********************************************************************//**
  <!-- d_INT_IrqSetPriorityTriggerType -->

  Set interrupt and trigger type priority.
*************************************************************************/
d_Status_t                  /** \return Function status */
d_INT_IrqSetPriorityTriggerType
(
const Uint32_t irq,         /**< [in] Interrupt number */
const Uint32_t priority,    /**< [in] Priority */
const d_INT_Trigger_t edge  /**< [in] Trigger edge */
)
{
  Uint32_t RegValue;
  Uint8_t LocalPriority;

  if (irq >= MAXIMUM_IRQ)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, irq, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (priority >= 256u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, priority, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if ((edge != d_INT_ACTIVE_HIGH) && (edge != d_INT_RISING_EDGE))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, edge, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  LocalPriority = (Uint8_t)priority;

  /*
   * Determine the register to write to using the Int_Id.
   */
  RegValue = ReadDistReg(XSCUGIC_PRIORITY_OFFSET_CALC(irq));

  /*
   * The priority bits are Bits 7 to 3 in GIC Priority Register. This
   * means the number of priority levels supported are 32 and they are
   * in steps of 8. The priorities can be 0, 8, 16, 32, 48, ... etc.
   * The lower order 3 bits are masked before putting it in the register.
   */
  LocalPriority = LocalPriority & (Uint8_t)XSCUGIC_INTR_PRIO_MASK;
  /*
   * Shift and Mask the correct bits for the priority and trigger in the
   * register
   */
  RegValue &= ~(XSCUGIC_PRIORITY_MASK << ((irq % 4u) * 8u));
  RegValue |= (Uint32_t)LocalPriority << ((irq % 4u) * 8u);

  /*
   * Write the value back to the register.
   */
  WriteDistReg(XSCUGIC_PRIORITY_OFFSET_CALC(irq),  RegValue);

  /*
   * Determine the register to write to using the Int_Id.
   */
  RegValue = ReadDistReg(XSCUGIC_INT_CFG_OFFSET_CALC(irq));

  /*
   * Shift and Mask the correct bits for the priority and trigger in the
   * register
   */
  RegValue &= ~(XSCUGIC_INT_CFG_MASK << ((irq % 16u) * 2u));
  RegValue |= (Uint32_t)edge << ((irq % 16u) * 2u);

  /*
   * Write the value back to the register.
   */
  WriteDistReg(XSCUGIC_INT_CFG_OFFSET_CALC(irq), RegValue);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_INT_SgiGenerate -->

  Generate a software interrupt.
*************************************************************************/
d_Status_t                  /** \return Function status */
d_INT_SgiGenerate
(
const Uint32_t irq          /**< [in] SGI Interrupt number */
)
{
  if (irq >= MAXIMUM_SGI)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, irq, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  WriteDistReg(XSCUGIC_SFI_TRIG_OFFSET, 0x02010000u | irq);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_INT_Ipi -->

  Generate an IPI interrupt to the APU.
*************************************************************************/
void d_INT_Ipi(void)
{
  d_GEN_RegisterWrite(IPI_RPU_BASE_ADDRESS + IPI_TRIG_OFFSET, IPI_APU_PROCESSOR_MASK);

  return;
}

/*********************************************************************//**
  <!-- GicDistInit -->

  Initialise Interrupt Controller Distributor
*************************************************************************/
static void     /** \return None */
GicDistInit
(
void
)
{
  Uint32_t Int_Id;

  /* Disable the distributor */
  WriteDistReg(XSCUGIC_DIST_EN_OFFSET, 0UL);

  for (Int_Id = 32; Int_Id < MAXIMUM_IRQ; Int_Id += 16u)
  {
    /*
     * Each INT_ID uses two bits, or 16 INT_ID per register
     * Set them all to be level sensitive, active HIGH.
     */
    WriteDistReg(XSCUGIC_INT_CFG_OFFSET + (Int_Id * 4u) / 16u, 0UL);
  }

  for (Int_Id = 0; Int_Id < MAXIMUM_IRQ; Int_Id += 4u)
  {
    /*
     * 2. The priority using int the priority_level register
     * The priority_level registers use one byte per INT_ID.
     * Write a default value that can be changed elsewhere.
     */
    WriteDistReg(XSCUGIC_PRIORITY_OFFSET +((Int_Id * 4u)/4u), DEFAULT_PRIORITY);
  }

  for (Int_Id = 0; Int_Id < MAXIMUM_IRQ; Int_Id += 32u)
  {
    /*
     * Disable all SPI using the enable_set register.
     */
    WriteDistReg(XSCUGIC_DISABLE_OFFSET +((Int_Id * 4u)/32u), 0xFFFFFFFFUL);
  }

  /* Enable the distributor */
  WriteDistReg(XSCUGIC_DIST_EN_OFFSET, 0x01UL);

  return;
}

/*********************************************************************//**
  <!-- GicCPUInit -->

  Initialise Interrupt Controller CPU
*************************************************************************/
static void     /** \return None */
GicCPUInit
(
void
)
{
  /* Program the priority mask of the CPU using the Priority mask register */
  WriteCpuReg(XSCUGIC_CPU_PRIOR_OFFSET, 0xF0);

  /* Set EnableS=1, to enable the CPU interface to signal secure interrupts */
  WriteCpuReg(XSCUGIC_CONTROL_OFFSET, XSCUGIC_CNTR_EN_S_MASK);

  return;
}

/*********************************************************************//**
  <!-- WriteDistReg -->

  Write to Interrupt Controller Distributor register.
*************************************************************************/
static void          /** \return None */
WriteDistReg
(
Uint32_t offset,     /**< [in] Register offset */
Uint32_t value       /**< [in] Value to write */
)
{
  d_GEN_RegisterWrite(DIST_BASEADDR + offset, value);

  return;
}

/*********************************************************************//**
  <!-- ReadDistReg -->

  Read Interrupt Controller Distributor register.
*************************************************************************/
static Uint32_t     /** \return Register value */
ReadDistReg
(
Uint32_t offset     /**< [in] Register offset */
)
{
  return d_GEN_RegisterRead(DIST_BASEADDR + offset);
}

/*********************************************************************//**
  <!-- WriteCpuReg -->

  Write to Interrupt Controller CPU register.
*************************************************************************/
static void         /** \return None */
WriteCpuReg
(
Uint32_t offset,    /**< [in] Register offset */
Uint32_t value      /**< [in] Value to write */
)
{
  d_GEN_RegisterWrite(CPU_BASEADDR + offset, value);

  return;
}

/*********************************************************************//**
  <!-- ReadCpuReg -->

  Read Interrupt Controller CPU register.
*************************************************************************/
static Uint32_t    /** \return Register value */
ReadCpuReg
(
Uint32_t offset    /**< [in] Register offset */
)
{
  return d_GEN_RegisterRead(CPU_BASEADDR + offset);
}
