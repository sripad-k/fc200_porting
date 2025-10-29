/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : CAN Holt configuration

  Abstract           : This is the default CAN_HOLT CSC configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-213
                       SDD References: 136T-2200-131100-001-D22 SWDES-549
                                                                SWDES-625
                                                                SWDES-626
                                                                SWDES-627
                                                                SWDES-628
                                                                SWDES-629
                                                                SWDES-630
                                                                SWDES-631
                                                                SWDES-632
                                                                SWDES-633
                                                                SWDES-634
                                                                SWDES-635
                                                                SWDES-636
                                                                SWDES-637
                                                                SWDES-638

\note
  CSC ID             : SWDES-64
*************************************************************************/

/***** Includes *********************************************************/

#include "sru/can_holt/d_can_holt_cfg.h"
#include "xparameters.h"

/***** Constants ********************************************************/

/* Using 32MHz oscillator, Bit clock = 32Mhz / divisor * 2 = 16MHz */
/* time segments plus sync = 16, thus CAN bit rate = 1MHz */
__attribute__((weak)) const d_CAN_HOLT_Configuration_t d_CAN_HOLT_Configuration[] =
{
  {
    0,                              /* spiChannel */
    0,                              /* Device */
    0,                              /* Interrupt */
    1,                              /* Divisor, 1MHz / 1 = 1Mbps */
    11,                             /* First time segment */
    4,                              /* Second time segment */
    1,                              /* Sync jump width */
    NULL,                           /* Interrupt handler for send complete */
    NULL,                           /* Interrupt handler for received frame */
    NULL,                           /* Interrupt handler for event */
    NULL                            /* Interrupt handler for error */
  },
  {
    1,                              /* spiChannel */
    0,                              /* Device */
    0,                              /* Interrupt */
    1,                              /* Divisor, 1MHz / 1 = 1Mbps */
    11,                             /* First time segment */
    4,                              /* Second time segment */
    1,                              /* Sync jump width */
    NULL,                           /* Interrupt handler for send complete */
    NULL,                           /* Interrupt handler for received frame */
    NULL,                           /* Interrupt handler for event */
    NULL                            /* Interrupt handler for error */
  },
  {
    0,                              /* spiChannel */
    1,                              /* Device */
    0,                              /* Interrupt */
    1,                              /* Divisor, 1MHz / 1 = 1Mbps */
    11,                             /* First time segment */
    4,                              /* Second time segment */
    1,                              /* Sync jump width */
    NULL,                           /* Interrupt handler for send complete */
    NULL,                           /* Interrupt handler for received frame */
    NULL,                           /* Interrupt handler for event */
    NULL                            /* Interrupt handler for error */
  },
  {
    1,                              /* spiChannel */
    1,                              /* Device */
    0,                              /* Interrupt */
    1,                              /* Divisor, 1MHz / 1 = 1Mbps */
    11,                             /* First time segment */
    4,                              /* Second time segment */
    1,                              /* Sync jump width */
    NULL,                           /* Interrupt handler for send complete */
    NULL,                           /* Interrupt handler for received frame */
    NULL,                           /* Interrupt handler for event */
    NULL                            /* Interrupt handler for error */
  },
  {
    0,                              /* spiChannel */
    2,                              /* Device */
    0,                              /* Interrupt */
    1,                              /* Divisor, 1MHz / 1 = 1Mbps */
    11,                             /* First time segment */
    4,                              /* Second time segment */
    1,                              /* Sync jump width */
    NULL,                           /* Interrupt handler for send complete */
    NULL,                           /* Interrupt handler for received frame */
    NULL,                           /* Interrupt handler for event */
    NULL                            /* Interrupt handler for error */
  },
  {
    1,                              /* spiChannel */
    2,                              /* Device */
    0,                              /* Interrupt */
    1,                              /* Divisor, 1MHz / 1 = 1Mbps */
    11,                             /* First time segment */
    4,                              /* Second time segment */
    1,                              /* Sync jump width */
    NULL,                           /* Interrupt handler for send complete */
    NULL,                           /* Interrupt handler for received frame */
    NULL,                           /* Interrupt handler for event */
    NULL                            /* Interrupt handler for error */
  }
};

/* Define the number of PL based blocks */
__attribute__((weak)) d_CAN_HOLT_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

