/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Interrupt Critical Section

  Abstract : Provision of critical section functions and other assembler macros.

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

#ifndef D_INT_CRITICAL_H
#define D_INT_CRITICAL_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_asm.h"

/*********************************************************************//**
  <!-- d_INT_IrqNestedEnable -->

  Enable nested interrupts by clearing the I bit in CPSR.
*************************************************************************/
#define d_INT_IrqNestedEnable() \
		__asm__ __volatile__ ("stmfd   sp!, {lr}"); \
		__asm__ __volatile__ ("mrs     lr, spsr");  \
		__asm__ __volatile__ ("stmfd   sp!, {lr}"); \
		__asm__ __volatile__ ("msr     cpsr_c, #0x5F"); \
		__asm__ __volatile__ ("stmfd   sp!, {lr}");
	
/*********************************************************************//**
  <!-- d_INT_IrqNestedDisable -->

  Disable nested interrupts.
*************************************************************************/
#define d_INT_IrqNestedDisable() \
		__asm__ __volatile__ ("ldmfd   sp!, {lr}");   \
		__asm__ __volatile__ ("msr     cpsr_c, #0xD2"); \
		__asm__ __volatile__ ("ldmfd   sp!, {lr}"); \
		__asm__ __volatile__ ("msr     spsr_cxsf, lr"); \
		__asm__ __volatile__ ("ldmfd   sp!, {lr}");

/*********************************************************************//**
  <!-- d_INT_CriticalSectionEnter -->

  Disable interrupts and return interrupt enable state.
*************************************************************************/
static inline __attribute__((always_inline)) 
Uint32_t                        /** \return Interrupt state on entry */
d_INT_CriticalSectionEnter
(
void
)
{
  Uint32_t statusRegister;

  statusRegister = d_mfcpsr();
  d_mtcpsr(statusRegister | 0x0C0u);
	
  return statusRegister;
}

/*********************************************************************//**
  <!-- d_INT_CriticalSectionLeave -->

  Enable interrupts based on state when critical section entered.
*************************************************************************/
static inline __attribute__((always_inline)) 
void                         /** \return None */
d_INT_CriticalSectionLeave
(
// cppcheck-suppress misra-c2012-2.7; Parameter is used in assembler function. which is not visible to cppcheck.
Uint32_t statusRegister      /**< [in] Interrupt state to restore */
)
{
  d_mtcpsr(statusRegister);
}

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/


#endif /* D_INT_CRITICAL_H */
