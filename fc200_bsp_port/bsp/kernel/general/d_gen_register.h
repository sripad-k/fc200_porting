/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : General register access functions

  Abstract           : Driver providing 32 and 64 bit register access functions.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-265
                       SDD References: 136T-2200-131000-001-D22 SWDES-723
\note
  CSCID              : SWDES-719
*************************************************************************/

#ifndef D_GEN_REGISTER_H
#define D_GEN_REGISTER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/*********************************************************************//**
  <!-- d_GEN_RegisterWrite -->

  Write to a memory location, specified by an integer address.
*************************************************************************/
static inline __attribute__((always_inline)) 
void                    /** \return None */
d_GEN_RegisterWrite
(
const Uint32_t Addr,    /**< [in] Contains the address to perform the output operation. */
const Uint32_t Value    /**< [in] Contains the 32 bit Value to be written at the specified address. */
)
{
  // cppcheck-suppress misra-c2012-11.4; Conversion required to convert Xilinx defined register address, as integer, to pointer. Violation of 'Advisory' rule does not present a risk.
  *(volatile Uint32_t *)Addr = Value;

  return;
}

/*********************************************************************//**
  <!-- d_GEN_RegisterWriteMask -->

  Write specific bits to a memory location, specified by an integer address.
*************************************************************************/
static inline __attribute__((always_inline)) 
void                    /** \return None */
d_GEN_RegisterWriteMask
(
const Uint32_t Addr,    /**< [in] Contains the address to perform the output operation. */
const Uint32_t Mask,    /**< [in] Contains the 32 bit Mask of bits to write. */
const Uint32_t Value    /**< [in] Contains the 32 bit Value to be written at the specified address. */
)
{
  // cppcheck-suppress misra-c2012-11.4; Conversion required to convert Xilinx defined register address, as integer, to pointer. Violation of 'Advisory' rule does not present a risk.
  *(volatile Uint32_t *)Addr = (*(volatile Uint32_t *)Addr & ~Mask) | (Value & Mask);

  return;
}

/*********************************************************************//**
  <!-- d_GEN_RegisterRead -->

  Read a memory location specified by an integer address.
*************************************************************************/
static inline __attribute__((always_inline)) 
Uint32_t                /**< The 32 bit Value read from the specified input address. */
d_GEN_RegisterRead
(
Uint32_t Addr           /**< [in] Contains the address to perform the input operation. */
)
{
  // cppcheck-suppress misra-c2012-11.4; Conversion required to convert Xilinx defined register address, as integer, to pointer. Violation of 'Advisory' rule does not present a risk.
  return *(volatile Uint32_t * const)Addr;
}

/*********************************************************************//**
  <!-- d_GEN_RegisterWriteP -->

  Write to a memory location, specified by a pointer.
*************************************************************************/
static inline __attribute__((always_inline)) 
void                    /** \return None */
d_GEN_RegisterWriteP
(
volatile Pointer_t * const Addr, /**< [in] Contains the address to perform the output operation. */
const Uint32_t Value    /**< [in] Contains the 32 bit Value to be written at the specified address. */
)
{
  *Addr = Value;

  return;
}

/*********************************************************************//**
  <!-- d_GEN_RegisterWriteMaskP -->

  Write specific bits to a memory location, specified by a pointer.
*************************************************************************/
static inline __attribute__((always_inline)) 
void                               /** \return None */
d_GEN_RegisterWriteMaskP
(
volatile Pointer_t * const Addr,   /**< [in] Contains the address to perform the output operation. */
const Uint32_t Mask,               /**< [in] Contains the 32 bit mask of bits to write. */
const Uint32_t Value               /**< [in] Contains the 32 bit Value to be written at the specified address. */
)
{
  *Addr = (*Addr & ~Mask) | (Value & Mask);

  return;
}

/*********************************************************************//**
  <!-- d_GEN_RegisterReadP -->

  Read a memory location, specified by a pointer.
*************************************************************************/
static inline __attribute__((always_inline)) 
Uint32_t                       /**< The 32 bit Value read from the specified input address. */
d_GEN_RegisterReadP
(
const Uint32_t volatile * const Addr    /**< [in] Contains the address to perform the input operation. */
)
{
  return *Addr;
}

Uint64_t d_GEN_RegisterRead64(const Uint32_t address);
void d_GEN_RegisterWrite64(const Uint32_t address, const Uint64_t value);

#endif /* D_GEN_REGISTER_H */
