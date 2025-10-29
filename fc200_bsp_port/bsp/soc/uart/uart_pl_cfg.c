/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Buffer definition

  Abstract           : This is a template for the UART PL CSC configuration
                       of the interfaces.
                       The file should be copied to the application code
                       space updated for the required interfaces.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "soc/uart/d_uart_pl_cfg.h"
#include "sru/platform/ioc.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_UART_PL_Configuration_t d_UART_PL_Configuration[] =
{
  {
    XPAR_UART_FDX0A_CORE16550_BASEADDR,
    XPAR_UART_FDX0A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_UART_FDX0B_CORE16550_BASEADDR,
    XPAR_UART_FDX0B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_UART_HDX0_CORE16550_BASEADDR,
    XPAR_UART_HDX0_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_UART_HDX1_CORE16550_BASEADDR,
    XPAR_UART_HDX1_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_GNSS_GNSS_1_CORE16550_BASEADDR,
    XPAR_GNSS_GNSS_1_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_GNSS_GNSS_2_CORE16550_BASEADDR,
    XPAR_GNSS_GNSS_2_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_IMU_IMU_CORE16550_BASEADDR, 
    XPAR_IMU_IMU_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },
  {
    XPAR_UART_FTDI_CORE16550_BASEADDR,
    XPAR_UART_FTDI_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_UART_IRQ_INTR
  },

#if defined PLATFORM_FCRP || defined PLATFORM_FC200
  {
    (Uint32_t)(Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_0_A_CORE16550_BASEADDR,
    IOC_FDX_0_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_0_B_CORE16550_BASEADDR,
    IOC_FDX_0_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_1_A_CORE16550_BASEADDR,
    IOC_FDX_1_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_1_B_CORE16550_BASEADDR,
    IOC_FDX_1_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_2_A_CORE16550_BASEADDR,
    IOC_FDX_2_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_2_B_CORE16550_BASEADDR,
    IOC_FDX_2_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_3_A_CORE16550_BASEADDR,
    IOC_FDX_3_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_3_B_CORE16550_BASEADDR,
    IOC_FDX_3_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_4_A_CORE16550_BASEADDR,
    IOC_FDX_4_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_4_B_CORE16550_BASEADDR,
    IOC_FDX_4_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_HDX_0_CORE16550_BASEADDR,
    IOC_HDX_0_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + (Uint32_t)IOC_HDX_1_CORE16550_BASEADDR,
    IOC_HDX_1_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC0_IRQ_INTR
  },

  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_0_A_CORE16550_BASEADDR,
    IOC_FDX_0_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_0_B_CORE16550_BASEADDR,
    IOC_FDX_0_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_1_A_CORE16550_BASEADDR,
    IOC_FDX_1_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_1_B_CORE16550_BASEADDR,
    IOC_FDX_1_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_2_A_CORE16550_BASEADDR,
    IOC_FDX_2_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_2_B_CORE16550_BASEADDR,
    IOC_FDX_2_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_3_A_CORE16550_BASEADDR,
    IOC_FDX_3_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_3_B_CORE16550_BASEADDR,
    IOC_FDX_3_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_4_A_CORE16550_BASEADDR,
    IOC_FDX_4_A_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_FDX_4_B_CORE16550_BASEADDR,
    IOC_FDX_4_B_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_HDX_0_CORE16550_BASEADDR,
    IOC_HDX_0_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + (Uint32_t)IOC_HDX_1_CORE16550_BASEADDR,
    IOC_HDX_1_CORE16550_CLOCK_FREQ_HZ,
    XPAR_FABRIC_INTERRUPT_CONTROLLER_0_IOC1_IRQ_INTR
  },
#endif
};

/* Define the number of PL based UARTs */
__attribute__((weak)) d_UART_PL_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

