/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : DMA

  Abstract           : This is DMA CSC configuration.
                       The file should be copied to the application code
                       space and updated for the required channel
                       interrupt handlers.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-146
                       SDD References: 136T-2200-131000-001-D22 SWDES-120
                                                                SWDES-121
                                                                SWDES-122
                                                                SWDES-123
                                                                SWDES-124
                                                                SWDES-125
                                                                SWDES-126
                                                                SWDES-127
                                                                SWDES-128
\note
  CSC ID             : SWDES-49
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "soc/dma/d_dma_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_DMA_Config_t d_DMA_Config[XPAR_XZDMA_NUM_INSTANCES] =
{
  { /* Channel ADMA 0 */
    NULL,
    NULL
  },
  { /* Channel ADMA 1 */
    NULL,
    NULL
  },
  { /* Channel ADMA 2 */
    NULL,
    NULL
  },
  { /* Channel ADMA 3 */
    NULL,
    NULL
  },
  { /* Channel ADMA 4 */
    NULL,
    NULL
  },
  { /* Channel ADMA 5 */
    NULL,
    NULL
  },
  { /* Channel ADMA 6 */
    NULL,
    NULL
  },
  { /* Channel ADMA 7 */
    NULL,
    NULL
  },
  { /* Channel GDMA 0 */
    NULL,
    NULL
  },
  { /* Channel GDMA 1 */
    NULL,
    NULL
  },
  { /* Channel GDMA 2 */
    NULL,
    NULL
  },
  { /* Channel GDMA 3 */
    NULL,
    NULL
  },
  { /* Channel GDMA 4 */
    NULL,
    NULL
  },
  { /* Channel GDMA 5 */
    NULL,
    NULL
  },
  { /* Channel GDMA 6 */
    NULL,
    NULL
  },
  { /* Channel GDMA 7 */
    NULL,
    NULL
  },
};

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/


