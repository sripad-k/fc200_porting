/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Function Status Return Values

  Abstract : Header file providing function return status values.

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

#ifndef D_ENDIAN_H
#define D_ENDIAN_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/*****************************************************************************/
/**
*
* @brief    Perform a 32-bit endian conversion.
*
* @param	Data: 32 bit value to be converted
*
* @return	32 bit data with converted endianness
*
******************************************************************************/
static inline __attribute__((always_inline)) Uint32_t Xil_EndianSwap32(Uint32_t Data)
{
	Uint16_t LoWord;
	Uint16_t HiWord;

	/* get each of the half words from the 32 bit word */

	LoWord = (Uint16_t) (Data & 0x0000FFFFU);
	HiWord = (Uint16_t) ((Data & 0xFFFF0000U) >> 16U);

	/* byte swap each of the 16 bit half words */

	LoWord = (((LoWord & 0xFF00U) >> 8U) | ((LoWord & 0x00FFU) << 8U));
	HiWord = (((HiWord & 0xFF00U) >> 8U) | ((HiWord & 0x00FFU) << 8U));

	/* swap the half words before returning the value */

	return ((((Uint32_t)LoWord) << (Uint32_t)16U) | (Uint32_t)HiWord);
}


#endif /* D_ENDIAN_H */

