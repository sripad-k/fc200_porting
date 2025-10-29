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

#ifndef D_INT_IRQ_HANDLER_H
#define D_INT_IRQ_HANDLER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/* Number of interrupt sources */
#define MAXIMUM_IRQ    188u

/* Number of software generated interrupts */
#define MAXIMUM_SGI     16u

/***** Type Definitions *************************************************/

typedef enum
{
  d_INT_ACTIVE_HIGH = 1,
  d_INT_RISING_EDGE = 3
} d_INT_Trigger_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Handler for IRQ exceptions */
void d_INT_IrqHandler(void);

/* Initialise the interrupt handler */
void d_INT_IrqDeviceInitialise(void);

/* Enable the specified interrupt */
d_Status_t d_INT_IrqEnable(const Uint32_t irq);

/* Disable the specified interrupt */
d_Status_t d_INT_IrqDisable(const Uint32_t irq);

/* Enable the APU interrupt */
void d_INT_ApuEnable(void);

/* Disable the APU interrupt */
void d_INT_ApuDisable(void);

/* Acknowledge the APU interrupt */
void d_INT_ApuAck(void);

/* Enable interrupts */
void d_INT_Enable(void);

/* Disable interrupts. */
void d_INT_Disable(void);

/* Set interrupt and trigger type priority */
d_Status_t d_INT_IrqSetPriorityTriggerType(const Uint32_t irq, Uint32_t priority, const d_INT_Trigger_t edge);

/* Generate a software interrupt */
d_Status_t d_INT_SgiGenerate(const Uint32_t irq);

/* Generate an IPI interrupt to the APU */
void d_INT_Ipi(void);

#endif /* D_INT_IRQ_HANDLER_H */
