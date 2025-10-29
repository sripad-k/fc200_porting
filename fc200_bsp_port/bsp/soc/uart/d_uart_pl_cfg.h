/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : UART Header File

  Abstract           : SOC PL UART interface configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-215
                       SDD References: 136T-2200-131000-001-D22 SWDES-544
                                                                SWDES-592
                                                                SWDES-593
                                                                SWDES-594
                                                                SWDES-595
                                                                SWDES-596
                                                                SWDES-597
                                                                SWDES-598
                                                                SWDES-599
                                                                SWDES-600
                                                                SWDES-601
                                                                SWDES-602
                                                                SWDES-603
\note
  CSC ID             : SWDES-57
*************************************************************************/

#ifndef D_UART_PL_CFG_H
#define D_UART_PL_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

#define d_UART_PL_MAX_INTERFACES  40u

#define d_UART_PL_MAX_FLAG_BITS   16u

#define d_UART_PL_COUNT const Uint32_t d_UART_PlCount = (sizeof(d_UART_PL_Configuration) / sizeof(d_UART_PL_Configuration_t))

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t baseAddress;
  Uint32_t clockFrequency;
  Uint32_t interruptNumber;
} d_UART_PL_Configuration_t;

typedef struct
{
  Uint32_t flagAddress;
  Uint32_t channel[d_UART_PL_MAX_FLAG_BITS];
} d_UART_PL_InterruptFlag_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_UART_PL_Configuration_t d_UART_PL_Configuration[];

extern const Uint32_t d_UART_PlCount;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_UART_PL_CFG_H */
