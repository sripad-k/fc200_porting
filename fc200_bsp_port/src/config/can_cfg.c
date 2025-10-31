/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Buffer definition

  Abstract           : This is a template for the CAN CSC configuration
                       of the interfaces.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: Document numbers and versions.
                       SDD References: Document numbers and versions.

*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "soc/can/d_can_cfg.h"


/***** Constants ********************************************************/

const d_CAN_Config_t d_CAN_Config[XPAR_XCANPS_NUM_INSTANCES] = 
{
    {
      XPAR_PSU_CAN_0_BASEADDR,    /* Base address */
      XPAR_PSU_CAN_0_INTR,        /* Interrupt */
      5,                          /* Divisor, (100MHz / 20) / 5(divisor) = 1Mbps */
      14,                         /* First time segment */
      3,                          /* Second time segment */
      3,                          /* Sync jump width */
      NULL,                /* Interrupt handler for send complete */
	  NULL,                /* Interrupt handler for received frame */
	  NULL,               /* Interrupt handler for event */
	  NULL                /* Interrupt handler for error */
    },
    {
      XPAR_PSU_CAN_1_BASEADDR,    /* Base address */
      XPAR_PSU_CAN_1_INTR,        /* Interrupt */
      5,                          /* Divisor, (100MHz / 20) / 5(divisor) = 1Mbps */
      14,                         /* First time segment */
      3,                          /* Second time segment */
      3,                          /* Sync jump width */
	  NULL,                /* Interrupt handler for send complete */
	  NULL,                /* Interrupt handler for received frame */
	  NULL,               /* Interrupt handler for event */
	  NULL                /* Interrupt handler for error */
    }
};

/* Define the number of CAN interfaces */
d_CAN_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

