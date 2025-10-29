/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : IRQ Split Configuration Definition

  Abstract           : This is a template for the INT CSC configuration
                       of the IRQ split interface.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-164
                                                                SWREQ-165
                                                                SWREQ-166
                                                                SWREQ-167
                       SDD References: 136T-2200-131000-001-D22 SWDES-172
                                                                SWDES-173
                                                                SWDES-174
                                                                SWDES-175
                                                                SWDES-176
                                                                SWDES-177
                                                                SWDES-178
                                                                SWDES-179
                                                                SWDES-180
                                                                SWDES-181
                                                                SWDES-182
                                                                SWDES-183
                                                                SWDES-184
                                                                SWDES-185
                                                                SWDES-186
                                                                SWDES-187
                                                                SWDES-188
\note
  CSC ID             : SWDES-52
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "soc/interrupt_manager/d_int_irq_split_cfg.h"
#include "soc/uart/d_uart_pl.h"
#include "sru/spi_pl/d_spi_pl.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_INT_IrqSplit_t d_INT_IrqSplit[] =
{
  {
    XPAR_INTERRUPT_CONTROLLER_0_BASEADDR,
    {
      {
        d_UART_PlInterruptHandler,
        8
      },
      {
        d_UART_PlInterruptHandler,
        9
      },
      {
        d_UART_PlInterruptHandler,
        10
      },
      {
        d_UART_PlInterruptHandler,
        11
      },
      {
        d_UART_PlInterruptHandler,
        12
      },
      {
        d_UART_PlInterruptHandler,
        13
      },
      {
        d_UART_PlInterruptHandler,
        14
      },
      {
        d_UART_PlInterruptHandler,
        15
      },
      {
        d_UART_PlInterruptHandler,
        16
      },
      {
        d_UART_PlInterruptHandler,
        17
      },
      {
        d_UART_PlInterruptHandler,
        18
      },
      {
        d_UART_PlInterruptHandler,
        19
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        d_SPI_PL_InterruptHandler,
        2
      }
    }
  },

  {
    XPAR_INTERRUPT_CONTROLLER_0_BASEADDR + 0x04u,
    {
      {
        d_UART_PlInterruptHandler,
        20
      },
      {
        d_UART_PlInterruptHandler,
        21
      },
      {
        d_UART_PlInterruptHandler,
        22
      },
      {
        d_UART_PlInterruptHandler,
        23
      },
      {
        d_UART_PlInterruptHandler,
        24
      },
      {
        d_UART_PlInterruptHandler,
        25
      },
      {
        d_UART_PlInterruptHandler,
        26
      },
      {
        d_UART_PlInterruptHandler,
        27
      },
      {
        d_UART_PlInterruptHandler,
        28
      },
      {
        d_UART_PlInterruptHandler,
        29
      },
      {
        d_UART_PlInterruptHandler,
        30
      },
      {
        d_UART_PlInterruptHandler,
        31
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        d_SPI_PL_InterruptHandler,
        3
      }
    }
  },

  {
    XPAR_INTERRUPT_CONTROLLER_0_BASEADDR + 0x08u,
    {
      {
        d_UART_PlInterruptHandler,
        0
      },
      {
        d_UART_PlInterruptHandler,
        1
      },
      {
        d_UART_PlInterruptHandler,
        2
      },
      {
        d_UART_PlInterruptHandler,
        3
      },
      {
        d_UART_PlInterruptHandler,
        4
      },
      {
        d_UART_PlInterruptHandler,
        5
      },
      {
        d_UART_PlInterruptHandler,
        6
      },
      {
        d_UART_PlInterruptHandler,
        7
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      },
      {
        NULL,
        0
      }
    }
  },

};

__attribute__((weak)) d_INT_IRQ_SPLIT_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

