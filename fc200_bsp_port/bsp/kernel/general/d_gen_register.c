/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : General register access functions

  Abstract           : Driver providing 32 and 64 bit register access functions.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-265
                       SDD References: 136T-2200-131000-001-D22 SWDES-723
\note
  CSCID              : SWDES-719
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/general/d_gen_register.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_GEN_RegisterRead64 -->

  Reads the value of a 64 bit register.
*************************************************************************/
Uint64_t                /** \return the value of the register */
d_GEN_RegisterRead64
(
const Uint32_t address  /**< [in] Register address */
)
{
  Uint32_t upperRegisterValue;
  Uint32_t lowerRegisterValue;

  /* Loop to ensure LS and MS words are from the same 64bit word */
  do
  {
    upperRegisterValue = d_GEN_RegisterRead(address + 4u);
    lowerRegisterValue = d_GEN_RegisterRead(address);

  } while (upperRegisterValue != d_GEN_RegisterRead(address + 4u));

  return ((((Uint64_t)upperRegisterValue) << 32u ) | lowerRegisterValue);
}

/*********************************************************************//**
  <!-- d_GEN_RegisterWrite64 -->

  Write the value to a 64 bit register.
*************************************************************************/
void                     /** \return None */
d_GEN_RegisterWrite64
(
const Uint32_t address,  /**< [in] Register address */
const Uint64_t value     /**< [in] Value to write */
)
{
  d_GEN_RegisterWrite(address, (Uint32_t)(value & 0xFFFFFFFFu));
  d_GEN_RegisterWrite(address + 4u, (Uint32_t)(value >> 32u));

  return;
}

