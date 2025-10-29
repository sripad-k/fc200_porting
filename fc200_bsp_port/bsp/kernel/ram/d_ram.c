/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : RAM 

  Abstract           : Memory test functionality

  Software Structure : SRS References: 136T-2200-131100-001-D22 SWREQ-237
                       SDD References: 136T-2200-131100-001-D22 SWDES-656
                                                                SWDES-662
\note
  CSC ID             : SWDES-74
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_asm.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"

#include "d_ram_usr.h"
#include "d_ram.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_RAM_Initialise -->

  Initialise the RAM CRC.
*************************************************************************/
void                 /** \return None */
d_RAM_Initialise
(
void
)
{
  d_RAM_CodeCrcInitialise();
  d_RAM_StackInitialise();

  return;
}

/*********************************************************************//**
  <!-- d_RAM_Cyclic -->

  Cyclic processing.
*************************************************************************/
void                 /** \return None */
d_RAM_Cyclic
(
void
)
{
  d_RAM_CodeCrcCyclic();
  d_RAM_StackCyclic();

  return;
}
