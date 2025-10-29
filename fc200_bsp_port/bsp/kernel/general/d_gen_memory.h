/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : General memory functions

  Abstract           : Driver providing memory 'copy', 'set' and 'compare' capability

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-266
                       SDD References: 136T-2200-131000-001-D22 SWDES-720
                                                                SWDES-721
                                                                SWDES-722
\note
  CSCID              : SWDES-719
*************************************************************************/

#ifndef D_GEN_MEMORY_H
#define D_GEN_MEMORY_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

void d_GEN_MemoryCopy(Uint8_t * const pDestination,     /**< Destination address */
                      const Uint8_t * const pSource,    /**< Source address */
                      const Uint32_t length);           /**< Number of bytes to copy */

void d_GEN_MemorySet(Uint8_t * const pDestination,      /**< Destination address */
                     const Uint8_t value,               /**< Value to set */
                     const Uint32_t length);            /**< Number of bytes to set */

Bool_t d_GEN_MemoryCompare(const Uint8_t * const pSource_1,  /**< Address 1 */
                           const Uint8_t * const pSource_2,  /**< Address 2 */
                           const Uint32_t length);           /**< Number of bytes to compare */

#endif /* D_GEN_MEMORY_H */
