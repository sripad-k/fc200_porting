/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_analogue_cfg.h

  Abstract           : IOC_EEPROM interface configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-753
                       SDD References: 136T-2200-131100-001-D22 SWDES-2424
                                                                SWDES-2425

\note
  CSC ID             : SWDES-2419
*************************************************************************/

#ifndef D_IOC_EEPROM_CFG_H
#define D_IOC_EEPROM_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_IOC_EEPROM_COUNT const Uint32_t d_IOC_EEPROM_Count = (sizeof(d_IOC_EEPROM_BaseAddress) / sizeof(Pointer_t))

/***** Type Definitions *************************************************/

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const Pointer_t d_IOC_EEPROM_BaseAddress[];

extern const Uint32_t d_IOC_EEPROM_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_IOC_EEPROM_CFG_H */
