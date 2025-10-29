/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_flash_mac_cfg.h

  Abstract           : FLASH_MAC SPI configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-177
                                                                SWREQ-178
                                                                SWREQ-179
                                                                SWREQ-180
                                                                SWREQ-214
                       SDD References: 136T-2200-131100-001-D22 SWDES-295
                                                                SWDES-615
                                                                SWDES-616
                                                                SWDES-617
                                                                SWDES-618
                                                                SWDES-619
                                                                SWDES-620
                                                                SWDES-621
                                                                SWDES-622
                                                                SWDES-623
                                                                SWDES-624
                       
\note
  CSC ID             : SWDES-59
*************************************************************************/

#ifndef D_FLASH_MAC_CFG_H
#define D_FLASH_MAC_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "xparameters.h"

/***** Literals *********************************************************/

#define d_FLASH_MAC_MAX_INTERFACES  4u

#define d_FLASH_MAC_COUNT const Uint32_t d_FLASH_MAC_Count = (sizeof(d_FLASH_MAC_Config) / sizeof(d_FLASH_MAC_Definition_t))


/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t channel;
  Uint32_t device;
} d_FLASH_MAC_Definition_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_FLASH_MAC_Definition_t d_FLASH_MAC_Config[];

extern const Uint32_t d_FLASH_MAC_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_FLASH_MAC_CFG_H */
