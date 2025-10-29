/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Interrupt Vector Table

  Abstract : Header file for the application provided vector table.

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

#ifndef D_INT_IRQ_TABLE_H
#define D_INT_IRQ_TABLE_H

/***** Includes *********************************************************/

#include "d_int_irq_handler.h"

typedef void (*d_INT_IrqHandler_t)(const Uint32_t parameter);

typedef struct
{
  d_INT_IrqHandler_t function;
  Uint32_t parameter;
}d_INT_IrqVectorTable_t;


extern const d_INT_IrqVectorTable_t IrqVectorTable[MAXIMUM_IRQ];

extern const Uint32_t SchedulerInterrupt;

#endif /* D_INT_IRQ_TABLE_H */
