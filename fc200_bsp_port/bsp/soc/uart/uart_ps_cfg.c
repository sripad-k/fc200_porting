/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : PS UART CFG

  Abstract           : This is a template for the UART SL CSC configuration
                       of the interfaces.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "soc/uart/d_uart_ps_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_UART_PS_Configuration_t d_UART_PS_Configuration[] =
{
  {
    XPAR_PSU_UART_0_BASEADDR,
    XPAR_PSU_UART_0_UART_CLK_FREQ_HZ,
    XPS_UART0_INT_ID
  }
};

/* Define the number of PL based UARTs */
__attribute__((weak)) d_UART_PS_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

