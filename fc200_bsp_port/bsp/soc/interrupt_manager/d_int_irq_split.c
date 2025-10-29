/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : IRQ Split Handler

  Abstract : Handles interrupt indication source with flag word.

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

#include "kernel/error_handler/d_error_handler.h"

#include "d_int_irq_handler.h"
#include "d_int_irq_split.h"
#include "d_int_irq_split_cfg.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_INT_SplitHandler -->

  Handler for IRQ exceptions using a register to manage multiple interrupt sources.
*************************************************************************/
void                     /** \return None */
d_INT_SplitHandler
(
const Uint32_t parameter /**< [in] entry in split config table */
)
{
  if (parameter >= d_INT_IrqSplitCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, parameter, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }
  
  Uint32_t index;
  Uint32_t flagValue = d_GEN_RegisterRead(d_INT_IrqSplit[parameter].flagAddress);

  for (index = 0; index < d_INT_IRQ_SPLIT_MAX_FLAGS; index++)
  {
    if ((flagValue & ((Uint32_t)0x01u << index)) != 0u)
    {
      if (d_INT_IrqSplit[parameter].handlerEntry[index].pHandler != NULL)
      {
        d_INT_IrqSplit[parameter].handlerEntry[index].pHandler(d_INT_IrqSplit[parameter].handlerEntry[index].parameter);
      }
      else
      {
        // gcov-jst 1 It is not practical to execute this code during bench testing. Unable to generate an unused interrupt.
        DO_NOTHING();
      }
    }
    else
    {
      DO_NOTHING();
    }
  }
  
  return;
}

