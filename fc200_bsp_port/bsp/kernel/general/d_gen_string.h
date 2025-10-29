/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : General string functions

  Abstract           : Driver providing basic string formatting and
                       manipulation functions which are safe

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-754
                       SDD References: 136T-2200-131000-001-D22 SWDES-724
                                                                SWDES-725
                                                                SWDES-726
                                                                SWDES-727
                                                                SWDES-728
                                                                SWDES-729
                                                                SWDES-730
\note
  CSCID              : SWDES-719
*************************************************************************/

#ifndef D_GEN_STRING_H
#define D_GEN_STRING_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

Int32_t d_GEN_SprintfInteger(Char_t * const outputString, const Uint32_t bufferSize, const Char_t * const format, const Int32_t value);

Int32_t d_GEN_SprintfFloat(Char_t * const outputString, const Uint32_t bufferSize, const Char_t * const format, const Float32_t value);

Int32_t d_GEN_ConvertIntegerToString(Int32_t value, Char_t * const pBuffer, const Uint32_t bufferSize, Uint32_t digits, const Uint32_t base);

Int32_t d_GEN_ConvertFloatToString(const Float32_t value, Char_t * const pBuffer, const Uint32_t bufferSize);

Int32_t d_GEN_StringLength(const Char_t * const pBuffer, const Uint32_t bufferSize);

Int32_t d_GEN_StringConcatenate(Char_t * const pBuffer, const Uint32_t bufferLength, const Char_t * const pSource, const Uint32_t length);

Int32_t d_GEN_StringCopy(Char_t * const pDestination, const Uint32_t destinationLength, const Char_t * const pSource, const Uint32_t sourceLength);

#endif /* D_GEN_STRING_H */
