/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : FLASH_MAC SPI interface definition

  Abstract           : This is the FLASH_MAC CSC configuration.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/flash_mac/d_flash_mac_cfg.h"

/***** Constants ********************************************************/

/* It is assumed all FLASH_MAC devices are interfaced using the SPI_PL CSC */
__attribute__((weak)) const d_FLASH_MAC_Definition_t d_FLASH_MAC_Config[] =
{
  {0, 0},   /* MAC Flash 0 */
  {0, 1}    /* MAC Flash 1 */
};

/* Define the number of FLASH_MAC devices */
__attribute__((weak)) d_FLASH_MAC_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

