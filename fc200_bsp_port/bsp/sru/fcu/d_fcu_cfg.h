/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_fcu_cfg

  Abstract           : FCU specific interface configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-759
                                                                SWREQ-760
                                                                SWREQ-761
                       SDD References: 136T-2200-131100-001-D22 SWDES-890
                                                                SWDES-2446
                                                                SWDES-2447
                                                                SWDES-2448
                                                                SWDES-2449
                                                                SWDES-2450
                                                                SWDES-2451
                                                                SWDES-2452
                                                                SWDES-2453
\note
  CSC ID             : SWDES-887
*************************************************************************/

#ifndef D_FCU_CFG_H
#define D_FCU_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */

/***** Type Definitions *************************************************/

typedef void (*d_FCU_MasterChanged_t)(const Uint32_t master);

/***** Constants ********************************************************/

extern const d_FCU_MasterChanged_t d_FCU_MasterChanged;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

d_Status_t d_FCU_IocAddressCheck(const Uint32_t address);

#endif /* D_FCU_CFG_H */
