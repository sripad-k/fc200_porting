/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART Header File

  Abstract           : SOC UART interface configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-233
                       SDD References: 136T-2200-131000-001-D22 SWDES-98
                                                                SWDES-99
                                                                SWDES-100
                                                                SWDES-101
                                                                SWDES-102
                                                                SWDES-103
                                                                SWDES-104
                                                                SWDES-105
                                                                SWDES-106
                                                                SWDES-107
                                                                SWDES-108
                                                                SWDES-109
                                                                SWDES-110
\note
  CSC ID             : SWDES-44
*************************************************************************/

#ifndef D_UART_PS_CFG_H
#define D_UART_PS_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_UART_PS_MAX_INTERFACES  40u

#define d_UART_PS_COUNT const Uint32_t d_UART_PsCount = (sizeof(d_UART_PS_Configuration) / sizeof(d_UART_PS_Configuration_t))

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t baseAddress;
  Uint32_t clockFrequency;
  Uint32_t interruptNumber;
} d_UART_PS_Configuration_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_UART_PS_Configuration_t d_UART_PS_Configuration[];

extern const Uint32_t d_UART_PsCount;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_UART_PS_CFG_H */
