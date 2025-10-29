/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Interrupt handler vector table

  Abstract           : This is a template for the interrupt handler vector table
                       used by the Interrupt Manager CSC. The file should be copied
                       to the application code space and renamed to a c file.
                       Add any interupt handlers to the table as required by the
                       application, such as that shown below for the scheduler timer
                       interrupt. Refer to the d_int_irq_table.h file for the type
                       definition of the table entries.

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
#include "soc/interrupt_manager/d_int_irq_table.h"
#include "soc/interrupt_manager/d_int_irq_split.h"
#include "soc/uart/d_uart_ps.h"
#include "soc/dma/d_dma.h"
#include "soc/can/d_can.h"
#include "soc/spi/d_spi.h"
#include "soc/discrete/d_discrete.h"
#include "sru/spi_pl/d_spi_pl.h"
#include "driver/gnss/d_gnss_ublox.h"

/* Include here any header files containing interrupt handler function definitions */

/***** Constants ********************************************************/

__attribute__((weak)) const d_INT_IrqVectorTable_t IrqVectorTable[MAXIMUM_IRQ] =
{
     {NULL, 0},                          /* 00 - Software interrupt 0 */
     {NULL, 0},                          /* 01 - Software interrupt 1 */
     {NULL, 0},                          /* 02 - Software interrupt 2 */
     {NULL, 0},                          /* 03 - Software interrupt 3 */
     {NULL, 0},                          /* 04 - Software interrupt 4 */
     {NULL, 0},                          /* 05 - Software interrupt 5 */
     {NULL, 0},                          /* 06 - Software interrupt 6 */
     {NULL, 0},                          /* 07 - Software interrupt 7 */
     {NULL, 0},                          /* 08 - Software interrupt 8 */
     {NULL, 0},                          /* 09 - Software interrupt 9 */
     {NULL, 0},                          /* 10 - Software interrupt 10 */
     {NULL, 0},                          /* 11 - Software interrupt 11 */
     {NULL, 0},                          /* 12 - Software interrupt 12 */
     {NULL, 0},                          /* 13 - Software interrupt 13 */
     {NULL, 0},                          /* 14 - Software interrupt 14 */
     {NULL, 0},                          /* 15 - Software interrupt 15 */

     {NULL, 0},                          /* 16 - Unused */
     {NULL, 0},                          /* 17 - Unused */
     {NULL, 0},                          /* 18 - Unused */
     {NULL, 0},                          /* 19 - Unused */
     {NULL, 0},                          /* 20 - Unused */
     {NULL, 0},                          /* 21 - Unused */
     {NULL, 0},                          /* 22 - Unused */
     {NULL, 0},                          /* 23 - Unused */
     {NULL, 0},                          /* 24 - Unused */
     {NULL, 0},                          /* 25 - Unused */
     {NULL, 0},                          /* 26 - Unused */

/* Private Peripheral Interrupts */
     {NULL, 0},                          /* 27 - PPI Global timer */
     {NULL, 0},                          /* 28 - PPI FIQ from FPGA */
     {NULL, 0},                          /* 29 - PPI Private timer */
     {NULL, 0},                          /* 30 - PPI Private watchdog */
     {NULL, 0},                          /* 31 - PPI IRQ from FPGA */

/* Shared peripheral Interrupts */
     {NULL, 0},                          /* 32 - */
     {NULL, 0},                          /* 33 - */
     {NULL, 0},                          /* 34 - */
     {NULL, 0},                          /* 35 - */
     {NULL, 0},                          /* 36 - */
     {NULL, 0},                          /* 37 - */
     {NULL, 0},                          /* 38 - */
     {NULL, 0},                          /* 39 - */
     {NULL, 0},                          /* 40 - */
     {NULL, 0},                          /* 41 - */

     {NULL, 0},                          /* 42 - XPS_OCMINTR_INT_ID */
     {NULL, 0},                          /* 43 - */
     {NULL, 0},                          /* 44 - */
     {NULL, 0},                          /* 45 - */
     {NULL, 0},                          /* 46 - XPS_NAND_INT_ID */
     {NULL, 0},                          /* 47 - XPS_QSPI_INT_ID */
     {d_DISC_InterruptHandler, 0},       /* 48 - XPS_GPIO_INT_ID */
     {NULL, 0},                          /* 49 - XPS_I2C0_INT_ID */
     {NULL, 0},                          /* 50 - XPS_I2C1_INT_ID */
     {d_SPI_InterruptHandler, 0},        /* 51 - XPS_SPI0_INT_ID */

     {d_SPI_InterruptHandler, 1},        /* 52 - XPS_SPI1_INT_ID */
     {d_UART_PsInterruptHandler, 0},     /* 53 - XPS_UART0_INT_ID */
     {d_UART_PsInterruptHandler, 1},     /* 54 - XPS_UART1_INT_ID */
     {d_CAN_InterruptHandler, 0},        /* 55 - XPS_CAN0_INT_ID */
     {d_CAN_InterruptHandler, 1},        /* 56 - XPS_CAN1_INT_ID */
     {NULL, 0},                          /* 57 - */
     {NULL, 0},                          /* 58 - XPS_RTC_ALARM_INT_ID */
     {NULL, 0},                          /* 59 - XPS_RTC_SEC_INT_ID */
     {NULL, 0},                          /* 60 - */
     {NULL, 0},                          /* 61 - */

     {NULL, 0},                          /* 62 - */
     {NULL, 0},                          /* 63 - */
     {NULL, 0},                          /* 64 - */
     {NULL, 0},                          /* 65 - Inter-Processor Interrupt */
     {NULL, 0},                          /* 66 - */
     {NULL, 0},                          /* 67 - */
     {NULL, 0},                          /* 68 - XPS_TTC0_0_INT_ID */
     {NULL, 0},                          /* 69 - XPS_TTC0_1_INT_ID */
     {NULL, 0},                          /* 70 - XPS_TTC0_2_INT_ID */
     {NULL, 0},                          /* 71 - XPS_TTC1_0_INT_ID */

     {NULL, 0},                          /* 72 - XPS_TTC1_1_INT_ID */
     {NULL, 0},                          /* 73 - XPS_TTC1_2_INT_ID */
     {NULL, 0},                          /* 74 - XPS_TTC2_0_INT_ID */
     {NULL, 0},                          /* 75 - XPS_TTC2_1_INT_ID */
     {NULL, 0},                          /* 76 - XPS_TTC2_2_INT_ID */
     {NULL, 0},                          /* 77 - XPS_TTC3_0_INT_ID */
     {NULL, 0},                          /* 78 - XPS_TTC3_1_INT_IDR */
     {NULL, 0},                          /* 79 - XPS_TTC3_2_INT_IDR */
     {NULL, 0},                          /* 80 - XPS_SDIO0_INT_ID */
     {NULL, 0},                          /* 81 - XPS_SDIO1_INT_ID */

     {NULL, 0},                          /* 82 - */
     {NULL, 0},                          /* 83 - */
     {NULL, 0},                          /* 84 - */
     {NULL, 0},                          /* 85 - */
     {NULL, 0},                          /* 86 - */
     {NULL, 0},                          /* 87 - */
     {NULL, 0},                          /* 88 - XPS_AMS_INT_ID */
     {NULL, 0},                          /* 89 - XPS_GEM0_INT_ID */
     {NULL, 0},                          /* 90 - XPS_GEM0_WAKE_INT_ID */
     {NULL, 0},                          /* 91 - XPS_GEM1_INT_ID */

     {NULL, 0},                          /* 92 - XPS_GEM1_WAKE_INT_ID */
     {NULL, 0},                          /* 93 - XPS_GEM2_INT_ID */
     {NULL, 0},                          /* 94 - XPS_GEM2_WAKE_INT_ID */
     {NULL, 0},                          /* 95 - XPS_GEM3_INT_ID */
     {NULL, 0},                          /* 96 - XPS_GEM3_WAKE_INT_ID */
     {NULL, 0},                          /* 97 - XPS_USB3_0_ENDPT_INT_ID */
     {NULL, 0},                          /* 98 - */
     {NULL, 0},                          /* 99 - */
     {NULL, 0},                          /* 100 - */
     {NULL, 0},                          /* 101 - */

     {NULL, 0},                          /* 102 - XPS_USB3_1_ENDPT_INT_ID */
     {NULL, 0},                          /* 103 - */
     {NULL, 0},                          /* 104 - */
     {NULL, 0},                          /* 105 - */
     {NULL, 0},                          /* 106 - */
     {NULL, 0},                          /* 107 - XPS_USB3_0_WAKE_INT_ID */
     {NULL, 0},                          /* 108 - XPS_USB3_1_WAKE_INT_ID */
     {d_DMA_InterruptHandler, 0},        /* 109 - XPS_ADMA_CH0_INT_ID */
     {d_DMA_InterruptHandler, 1},        /* 110 - XPS_ADMA_CH1_INT_ID */
     {d_DMA_InterruptHandler, 2},        /* 111 - XPS_ADMA_CH2_INT_ID */

     {d_DMA_InterruptHandler, 3},        /* 112 - XPS_ADMA_CH3_INT_ID */
     {d_DMA_InterruptHandler, 4},        /* 113 - XPS_ADMA_CH4_INT_ID */
     {d_DMA_InterruptHandler, 5},        /* 114 - XPS_ADMA_CH5_INT_ID */
     {d_DMA_InterruptHandler, 6},        /* 115 - XPS_ADMA_CH6_INT_ID */
     {d_DMA_InterruptHandler, 7},        /* 116 - XPS_ADMA_CH7_INT_ID */
     {NULL, 0},                          /* 117 - */
     {NULL, 0},                          /* 118 - XPS_CSU_DMA_INT_ID */
     {NULL, 0},                          /* 119 - */
     {NULL, 0},                          /* 120 - XPS_XMPU_LPD_INT_ID */
     {d_INT_SplitHandler, 2},            /* 121 - XPS_FPGA0_INT_ID */
     {NULL, 0},                          /* 122 - XPS_FPGA1_INT_ID */
     {NULL, 0},                          /* 123 - XPS_FPGA2_INT_ID */
     {d_INT_SplitHandler, 3},            /* 124 - XPS_FPGA3_INT_ID */
     {d_SPI_PL_InterruptHandler, 1},     /* 125 - XPS_FPGA4_INT_ID */
     {d_SPI_PL_InterruptHandler, 0},     /* 126 - XPS_FPGA5_INT_ID */
     {d_GNSS_Ublox_1ppsInterrupt, 0},    /* 127 - XPS_FPGA6_INT_ID */
     {NULL, 0},                          /* 128 - XPS_FPGA7_INT_ID */
     {NULL, 0},                          /* 129 - */
     {NULL, 0},                          /* 130 - */
     {NULL, 0},                          /* 131 - */

     {NULL, 0},                          /* 132 - */
     {NULL, 0},                          /* 133 - */
     {NULL, 0},                          /* 134 - */
     {NULL, 0},                          /* 135 - */
     {d_INT_SplitHandler, 0},            /* 136 - XPS_FPGA8_INT_ID */
     {d_INT_SplitHandler, 1},            /* 137 - XPS_FPGA9_INT_ID */
     {NULL, 0},                          /* 138 - XPS_FPGA10_INT_ID */
     {NULL, 0},                          /* 139 - XPS_FPGA11_INT_ID */
     {NULL, 0},                          /* 140 - XPS_FPGA12_INT_ID */
     {NULL, 0},                          /* 141 - XPS_FPGA13_INT_ID */

     {NULL, 0},                          /* 142 - XPS_FPGA14_INT_ID */
     {NULL, 0},                          /* 143 - XPS_FPGA15_INT_ID */
     {NULL, 0},                          /* 144 - */
     {NULL, 0},                          /* 145 - */
     {NULL, 0},                          /* 146 - */
     {NULL, 0},                          /* 147 - */
     {NULL, 0},                          /* 148 - */
     {NULL, 0},                          /* 149 - */
     {NULL, 0},                          /* 150 - */
     {NULL, 0},                          /* 151 - */

     {NULL, 0},                          /* 152 - */
     {NULL, 0},                          /* 153 - */
     {NULL, 0},                          /* 154 - */
     {NULL, 0},                          /* 155 - XPS_APM0_INT_ID */
     {d_DMA_InterruptHandler, 8},        /* 156 - XPS_ZDMA_CH0_INT_ID */
     {d_DMA_InterruptHandler, 9},        /* 157 - XPS_ZDMA_CH1_INT_ID */
     {d_DMA_InterruptHandler, 10},       /* 158 - XPS_ZDMA_CH2_INT_ID */
     {d_DMA_InterruptHandler, 11},       /* 159 - XPS_ZDMA_CH3_INT_ID */
     {d_DMA_InterruptHandler, 12},       /* 160 - XPS_ZDMA_CH4_INT_ID */
     {d_DMA_InterruptHandler, 13},       /* 161 - XPS_ZDMA_CH5_INT_ID */

     {d_DMA_InterruptHandler, 14},       /* 162 - XPS_ZDMA_CH6_INT_ID */
     {d_DMA_InterruptHandler, 15},       /* 163 - XPS_ZDMA_CH7_INT_ID */
     {NULL, 0},                          /* 164 - */
     {NULL, 0},                          /* 165 - */
     {NULL, 0},                          /* 166 - */
     {NULL, 0},                          /* 167 - */
     {NULL, 0},                          /* 168 - XPS_XMPU_FPD_INT_ID */
     {NULL, 0},                          /* 169 - */
     {NULL, 0},                          /* 170 - */
     {NULL, 0},                          /* 171 - */

     {NULL, 0},                          /* 172 - */
     {NULL, 0},                          /* 173 - */
     {NULL, 0},                          /* 174 - */
     {NULL, 0},                          /* 175 - */
     {NULL, 0},                          /* 176 - */
     {NULL, 0},                          /* 177 - */
     {NULL, 0},                          /* 178 - */
     {NULL, 0},                          /* 179 - */
     {NULL, 0},                          /* 180 - */
     {NULL, 0},                          /* 181 - */

     {NULL, 0},                          /* 182 - */
     {NULL, 0},                          /* 183 - */
     {NULL, 0},                          /* 184 - */
     {NULL, 0},                          /* 185 - */
     {NULL, 0},                          /* 186 - XPS_FPD_CCI_INT_ID */
     {NULL, 0}                           /* 187 - XPS_FPD_SMMU_INT_ID */
};

__attribute__((weak)) const Uint32_t SchedulerInterrupt = XPS_FPGA2_INT_ID;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/
