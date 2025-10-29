/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Shared Memory Interface Ethernet

  Abstract           : Ethernet interface

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-
                       SDD References: 136T-2200-131100-001-D22 SWDES-

\note
  CSC ID             : SWDES-80
*************************************************************************/

#ifndef SMI_ETH_H
#define SMI_ETH_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the variable message length buffers */
void d_SMI_EthInitialise(void);

/* Add ethernet port to be checked for */
d_Status_t d_SMI_EthPortAdd(const Uint32_t port);

/* Send packet as received from SMI */
d_Status_t d_SMI_EthTransmit(const Uint8_t * const buffer, const Uint32_t length);

#endif /* SMI_ETH_H */
