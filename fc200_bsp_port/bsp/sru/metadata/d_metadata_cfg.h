/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_metaadata_cfg.h

  Abstract           : Metadata interface configuration, Revision 03.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-157
                                                                SWREQ-158
                                                                SWREQ-159
                       SDD References: 136T-2200-131000-001-D22 SWDES-546
                                                                SWDES-604
\note
  CSCID              : SWDES-60
*************************************************************************/

#ifndef D_METADATA_CFG_H
#define D_METADATA_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/* Metadata maximum size, in words, as used by bootloader */
#define d_METADATA_MAXIMUM_SIZE  1024u

#define d_METADATA_COUNT const Uint32_t d_METADATA_Count = (sizeof(d_METADATA_BaseAddress) / sizeof(Pointer_t))

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const Pointer_t d_METADATA_BaseAddress[];

extern const Uint32_t d_METADATA_Count;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_METADATA_CFG_H */
