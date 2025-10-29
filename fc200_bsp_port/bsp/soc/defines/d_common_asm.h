/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Assembler macros

  Abstract           : Macros allowing reading and writing of processor
                       registers using assembler.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-181
                       SDD References: 136T-2200-131000-001-D22 SWDES-83
\note
  CSCI_ID            : SWDES-12
*************************************************************************/

#ifndef D_ASM_H
#define D_ASM_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Macros (Inline Functions) Definitions ****************************/

// cppcheck-suppress misra-c2012-20.7;  Assembler instructions do not allow enclosing parameter in parentheses.
#define d_mfcp(rn)	({Uint32_t rval = 0U; \
			 __asm__ __volatile__(\
			   "mrc " rn "\n"\
			   : "=r" (rval)\
			 );\
			 rval;\
			 })

// cppcheck-suppress misra-c2012-20.7;  Assembler instructions do not allow enclosing parameter in parentheses.
#define d_mtcp(rn, v)	__asm__ __volatile__(\
			 "mcr " rn "\n"\
			 : : "r" (v)\
			)

/* Get Current Program Status Register */
#define d_mfcpsr()  ({Uint32_t rval = 0U; \
             __asm__ __volatile__(\
             "mrs  %0, cpsr\n"\
             : "=r" (rval)\
             );\
            rval;\
             })

/* Set Current Program Status Register */
#define d_mtcpsr(v)  __asm__ __volatile__(\
        "msr  cpsr,%0\n"\
        : : "r" (v) : "cc" \
      )

/* Get the stack pointer */
#define d_getsp()  ({Uint32_t rval = 0U; \
             __asm__ __volatile__(\
             "mov  %0, sp\n"\
             : "=r" (rval)\
             );\
            rval;\
             })

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_ASM_H */

