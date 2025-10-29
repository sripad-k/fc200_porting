/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_file

  Abstract           : File handler interface to PC

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

*************************************************************************/

#ifndef D_FILE_H
#define D_FILE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

Uint32_t d_FILE_Initialise(const Uint32_t serialChannel);

Int32_t d_FILE_fopen(const Char_t * const filename, const Char_t * const mode);
Int32_t d_FILE_fwrite(const Uint32_t fileId, const Uint8_t * const pData, const Uint32_t length);
Int32_t d_FILE_fclose(const Uint32_t fileId);

#endif /* D_FILE_H */
