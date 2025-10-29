/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : General memory functions

  Abstract           : Driver providing memory 'copy', 'set' and 'compare' capability

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-266
                       SDD References: 136T-2200-131000-001-D22 SWDES-720
                                                                SWDES-721
                                                                SWDES-722
\note
  CSCID              : SWDES-719
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/general/d_gen_memory.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_GEN_MemoryCopy -->

  Fast memory copy.
*************************************************************************/
void                              /** \return None */
d_GEN_MemoryCopy
(
Uint8_t * const pDestination,     /**< [out] Destination address */
const Uint8_t * const pSource,    /**< [in]  Source address */
const Uint32_t length             /**< [in]  Number of bytes to copy */
)
{
  Uint32_t index;
  
  /* If source and destination addresses are not word aligned, or small block to copy
     then use a simple byte copy */
// cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  if ((((Pointer_t)pDestination % 4u) != 0u) || (((Pointer_t)pSource % 4u) != 0u) || (length < 16u))
  {
    for (index = 0; index < length; index++)
    {
      pDestination[index] = pSource[index];
    }
  }
  else
  {
// cppcheck-suppress misra-c2012-11.3;  Validity of pointer conversion is assured by check on line 47. Violation of 'Advisory' rule does not present a risk.
    Uint32_t * destination32 = (Uint32_t *)pDestination;
// cppcheck-suppress misra-c2012-11.3;  Validity of pointer conversion is assured by check on line 47. Violation of 'Advisory' rule does not present a risk.
    const Uint32_t * source32 = (const Uint32_t *)pSource;
    /* Copy words */
    for (index = 0; index < (length / 4u); index++)
    {
      destination32[index] = source32[index];
    }
    Uint8_t * destination8 = (Uint8_t *)&destination32[index];
    const Uint8_t * source8 = (const Uint8_t *)&source32[index];
    /* Copy any remaining bytes */
    for (index = 0; index < (length % 4u); index++)
    {
      destination8[index] = source8[index];
    }
  }
  
  return;
}

/*********************************************************************//**
  <!-- d_GEN_MemorySet -->

  Set memory to a specific value.
*************************************************************************/
void                              /** \return None */
d_GEN_MemorySet
(
Uint8_t * const pDestination,     /**< [out] Destination address */
const Uint8_t value,              /**< [in]  Value to set */
const Uint32_t length             /**< [in]  Number of bytes to set */
)
{
  Uint32_t index;

  for (index = 0; index < length; index++)
  {
    pDestination[index] = value;
  }

  return;
}

/*********************************************************************//**
  <!-- d_GEN_MemoryCompare -->

  Compare two blocks of memory.
*************************************************************************/
Bool_t                            /** \return Equal True or False */
d_GEN_MemoryCompare
(
const Uint8_t * const pSource_1,  /**< [in] Address 1 */
const Uint8_t * const pSource_2,  /**< [in] Address 2 */
const Uint32_t length             /**< [in] Number of bytes to compare */
)
{
  Bool_t equal = d_TRUE;
  Uint32_t index = 0;
  while ((index < length) && (equal == d_TRUE))
  {
    if (pSource_1[index] != pSource_2[index])
    {
      equal = d_FALSE;
    }
    index++;
  }
  
  return equal;
}  

