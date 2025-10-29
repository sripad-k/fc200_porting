/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : CAN CFG

  Abstract           : SOC CAN configuration.

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

#ifndef D_CAN_CFG_H
#define D_CAN_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "xparameters.h"

/***** Literals *********************************************************/

#define d_CAN_MAX_INTERFACES  2u

#define d_CAN_COUNT const Uint32_t d_CAN_Count = (sizeof(d_CAN_Config) / sizeof(d_CAN_Config_t))


/***** Type Definitions *************************************************/

/* Callback type for successful sending of frame */
typedef void (*d_CAN_SendHandler) (const Uint32_t channel);

/* Callback type for receipt of frame */
typedef void (*d_CAN_ReceiveHandler) (const Uint32_t channel);

/* Callback type for event */
typedef void (*d_CAN_EventHandler) (const Uint32_t channel, const Uint32_t event);

/* Callback type for all error interrupts */
typedef void (*d_CAN_ErrorHandler) (const Uint32_t channel, const Uint32_t errorStatus);

typedef struct
{
  Uint32_t baseAddress;
  Uint32_t interruptNumber;
  Uint32_t clockDivisor;
  Uint32_t firstTimeSegment;
  Uint32_t secondTimeSegment;
  Uint32_t syncJumpWidth;
  d_CAN_SendHandler sendHandler;
  d_CAN_ReceiveHandler receiveHandler;
  d_CAN_EventHandler eventHandler;
  d_CAN_ErrorHandler errorHandler;
} d_CAN_Config_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_CAN_Config_t d_CAN_Config[];

extern const Uint32_t d_CAN_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_CAN_CFG_H */
