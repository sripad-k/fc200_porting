/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Memory Cache Manager

  Abstract : Memory cache control

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-153
                       SDD References: 136T-2200-131000-001-D22 SWDES-139
                                                                SWDES-140
                                                                SWDES-141
                                                                SWDES-142
                                                                SWDES-143
                                                                SWDES-144
                                                                SWDES-145
                                                                SWDES-146
                                                                SWDES-147

\note
  CSC ID             : SWDES-51
*************************************************************************/

#ifndef D_MEMORY_CACHE_H
#define D_MEMORY_CACHE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/* memory synchronisation operations */
/* Instruction Synchronisation Barrier */
#define d_isb() __asm__ __volatile__ ("isb sy")
/* Data Synchronisation Barrier */
#define d_dsb() __asm__ __volatile__("dsb sy")
/* Data Memory Barrier */
#define d_dmb() __asm__ __volatile__("dmb sy")

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Enable the Data cache */
void d_MEMORY_DCacheEnable(void);

/* Disable the Data cache */
void d_MEMORY_DCacheDisable(void);

/* Invalidate the entire Data cache */
void d_MEMORY_DCacheInvalidate(void);

/* Invalidate a portion of Data cache */
void d_MEMORY_DCacheInvalidateRange(const Pointer_t address, Uint32_t length);

/* Flush the entire Data cache */
void d_MEMORY_DCacheFlush(void);

/* Flush a portion of Data cache */
void d_MEMORY_DCacheFlushRange(const Pointer_t address, Uint32_t length);

/* Enable the instruction cache */
void d_MEMORY_ICacheEnable(void);

/* Disable the instruction cache */
void d_MEMORY_ICacheDisable(void);

/* Invalidate the entire instruction cache */
void d_MEMORY_ICacheInvalidate(void);

#endif /* D_MEMORY_CACHE_H */
