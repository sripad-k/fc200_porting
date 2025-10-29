/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : CAN definition

  Abstract           : This is a configuratin file for the CAN CSC
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-325
                       SDD References: 136T-2200-131100-001-D22 SWDES-117
                                                                SWDES-541
                                                                SWDES-606
                                                                SWDES-607
                                                                SWDES-608
                                                                SWDES-609
                                                                SWDES-610
                                                                SWDES-611
                                                                SWDES-612
                                                                SWDES-613
                                                                SWDES-614

\note
  CSC ID             : SWDES-47
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "soc/can/d_can_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_CAN_Config_t d_CAN_Config[1] = 
{
  {
    0,                          /* Base address */
    0,                          /* Interrupt */
    5,                          /* Divisor, (100MHz / 20) / 5(divisor) = 1Mbps */
    14,                         /* First time segment */
    3,                          /* Second time segment */
    3,                          /* Sync jump width */
    NULL,                       /* Interrupt handler for send complete */
    NULL,                       /* Interrupt handler for received frame */
    NULL,                       /* Interrupt handler for event */
    NULL                        /* Interrupt handler for error */
  }
};

/* Define the number of CAN interfaces */
__attribute__((weak)) d_CAN_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

