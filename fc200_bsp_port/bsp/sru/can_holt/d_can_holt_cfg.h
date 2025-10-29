/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : CAN Holt Header File

  Abstract           : CAN interface using Holt HI-3110 device via SPI configuration.

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

#ifndef D_CAN_HOLT_CFG_H
#define D_CAN_HOLT_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_CAN_HOLT_MAX_INTERFACES  10u

#define d_CAN_HOLT_COUNT const Uint32_t d_CAN_HOLT_Count = (sizeof(d_CAN_HOLT_Configuration) / sizeof(d_CAN_HOLT_Configuration_t))

/***** Type Definitions *************************************************/

/* Callback type for successful sending of frame */
typedef void (*d_CAN_HOLT_SendHandler) (const Uint32_t channel);

/* Callback type for receipt of frame */
typedef void (*d_CAN_HOLT_ReceiveHandler) (const Uint32_t channel);

/* Callback type for event */
typedef void (*d_CAN_HOLT_EventHandler) (const Uint32_t channel, const Uint8_t event);

/* Callback type for all error interrupts */
typedef void (*d_CAN_HOLT_ErrorHandler) (const Uint32_t channel, const Uint8_t errorStatus);

typedef struct
{
  Uint32_t spiChannel;
  Uint32_t device;
  Uint32_t interruptNumber;
  Uint32_t clockDivisor;
  Uint32_t firstTimeSegment;
  Uint32_t secondTimeSegment;
  Uint32_t syncJumpWidth;
  d_CAN_HOLT_SendHandler sendHandler;
  d_CAN_HOLT_ReceiveHandler receiveHandler;
  d_CAN_HOLT_EventHandler eventHandler;
  d_CAN_HOLT_ErrorHandler errorHandler;
} d_CAN_HOLT_Configuration_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_CAN_HOLT_Configuration_t d_CAN_HOLT_Configuration[];

extern const Uint32_t d_CAN_HOLT_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_CAN_HOLT_CFG_H */
