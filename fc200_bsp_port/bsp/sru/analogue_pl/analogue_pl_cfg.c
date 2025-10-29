/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Analogue PL Configuration

  Abstract           : This is the ANALOGUE PL CSC configuration.
                       The file should be copied to the application code
                       space and updated for the required sensors.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-127
                       SDD References: 136T-2200-131000-001-D22 SWDES-189
                                                                SWDES-190
                                                                SWDES-191
                                                                SWDES-543
                       SRS References: 136T-2200-131000-001-D20 SWREQ-222
                       SDD References: 136T-2200-131000-001-D22 SWREQ-???
\note
  CSC ID             : SWDES-56
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

#include "xparameters.h"
#include "sru/platform/ioc.h"
#include "sru/analogue_pl/d_analogue_pl_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_ANALOGUE_PL_Configuration_t d_ANALOGUE_PL_Configuration[] =
{
  /* PL ADC */
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x00u, /* DDR_VPP */
    12u,
    1.221f,
    0.0f,
    2347.0f,
    2778.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x04u, /* 1V2_MGT */
    12u,
    0.610f,
    0.0f,
    1136.0f,
    1264.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x08u, /* DDR_VTT */
    12u,
    0.610f,
    0.0f,
    542.0f,
    658.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x0Cu, /* DDR_VREF */
    12u,
    0.610f,
    0.0f,
    542.0f,
    658.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x10u, /* FCU_DISC_POWER */
    12u,
    7.011f,
    0.0f,
    17642.0f,   /* Limits specified by Hano, not in ATP */
    36664.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x14u, /* 2V5_LDO_IN */
    12u,
    1.221f,
    0.0f,
    1972.0f,
    3028.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x18u, /* 5V_eFUSE */
    12u,
    1.909f,
    0.0f,
    4722.0f,
    5278.0f
  },
  {
    (Uint32_t)XPAR_FCU_ADS7138_BASEADDR + 0x1Cu, /* eFUSE_P5V_CURRENT */
    12u,
    2.481f,
    0.0f,
    800.0f,
    5000.0f
  },
  
  /* PSU */
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x00u, /* 28V_DISC_BOOST */
    16u,
    14.038f,
    0.0f,
    24000.0f,
    26000.3f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x04u, /* 28V_A_V_SENSE */
    16u,
    14.038f,
    0.0f,
    27000.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x08u, /* 28V_B_V_SENSE */
    16u,
    14.038f,
    0.0f,
    27000.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x0Cu, /* P28V_IN */
    16u,
    14.038f,
    0.0f,
    26520.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x10u, /* 28V_FILTERED */
    16u,
    14.038f,
    0.0f,
    26520.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x14u, /* 28V_SURGE_LIMIT */
    16u,
    14.038f,
    0.0f,
    26500.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x18u, /* 28V_DIODE_OR */
    16u,
    14.038f,
    0.0f,
    26480.0f,
    29000.0f

  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x1Cu, /* VHOLDUP */
    16u,
    14.038f,
    0.0f,
    44400.0f,
    48100.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x20u, /* VREF_3V3_DIG */
    16u,
    1.526f,
    0.0f,
    3190.0f,
    3440.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x24u, /* 28V_I_SENSE */
    16u,
    2.146f,
    0.0f,
    250.0f,
    750.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x28u, /* 5V_eFUSE_IMON */
    16u,
    4.002f,
    0.0f,
    1500.0f,
    8000.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x2Cu, /* 5V */
    16u,
    1.526f,
    0.0f,
    4860.0f,
    5220.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x30u, /* 5V_INT */
    16u,
    1.526f,
    0.0f,
    4860.0f,
    5220.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x34u, /* 28V_DISC_FCU */
    16u,
    14.038f,
    0.0f,
    24000.0f,
    28440.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x38u, /* 28V_DISC_IOC */
    16u,
    14.038f,
    0.0f,
    25800.0f,
    28450.0f
  },
  {
    (Uint32_t)XPAR_PSU_BIT_PSU_ADS7138_BASEADDR + 0x3Cu, /* DISC_eFUSE_IMON */
    16u,
    0.219f,
    0.0f,
    0.0f,
    2000.0f
  },

#if defined PLATFORM_FCRP || defined PLATFORM_FC200
  /* IOC A */
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR, /* IOC 0 DISC_ADC_IOC_IN1 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    0.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x04u, /* IOC 0 DISC_ADC_IOC_IN2 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    0.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x08u, /* IOC 0 DISC_ADC_IOC_IN3 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    0.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x0Cu, /* IOC 0 DISC_ADC_IOC_IN4 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    0.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x10u, /* IOC 0 P28V_IOCA_BIT */
    16u,
    9.521f,
    0.0f,
    24000.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x14u, /* IOC 0 P28V_IOCB_BIT */
    16u,
    9.521f,
    0.0f,
    24000.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x18u, /* IOC 0 P15V_IOCA_BIT */
    16u,
    3.979f,
    0.0f,
    13500.0f,
    16500.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x1Cu, /* IOC 0 P15V_IOCB_BIT */
    16u,
    3.979f,
    0.0f,
    13500.0f,
    16500.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x20u, /* IOC 0 P5V_IOCA_BIT */
    16u,
    1.343f,
    0.0f,
    4850.0f,
    5150.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x24u, /* IOC 0 P5V_IOCB_BIT */
    16u,
    1.343f,
    0.0f,
    4850.0f,
    5150.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x28u, /* IOC 0 P3V3_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    3169.0f,
    3432.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x2Cu, /* IOC 0 P3V3_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    3169.0f,
    3432.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x30u, /* IOC 0 P2V5_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    2350.0f,
    2650.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x34u, /* IOC 0 P2V5_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    2350.0f,
    2650.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x38u, /* IOC 0 P1V8_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    1728.0f,
    1872.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x3Cu, /* IOC 0 P1V8_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    1728.0f,
    1872.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x40u, /* IOC 0 P1V05_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    1008.0f,
    1092.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x44u, /* IOC 0 P1V05_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    1008.0f,
    1092.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x48u, /* IOC 0 P1V_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    965.0f,
    1045.2f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x4Cu, /* IOC 0 P1V_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    965.0f,
    1045.2f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x50u, /* IOC 0 P5V_GPIO_BIT */
    16u,
    1.343f,
    0.0f,
    4850.0f,
    5150.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x54u, /* IOC 0 P3V3_GPIO_BIT */
    16u,
    1.221f,
    0.0f,
    3168.0f,
    3432.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x58u, /* IOC 0 eFUSE_P5V_IMON1 */
    16u,
    3.308f,
    0.0f,
    300.0f,
    3000.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x5Cu, /* IOC 0 eFUSE_P5V_IMON2 */
    16u,
    3.308f,
    0.0f,
    300.0f,
    3000.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x60u, /* IOC 0 PF_VDD */
    16u,
    0.125f,
    0.0f,
    970.0f,      /* Limits specified by Deon, not in ATP */
    1030.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x64u, /* IOC 0 PF_VDD18 */
    16u,
    0.125f,
    0.0f,
    1746.0f,     /* Limits specified by Deon, not in ATP */
    1854.0f
  },
  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x68u, /* IOC 0 PF_VDD25 */
    16u,
    0.125f,
    0.0f,
    2425.0f,     /* Limits specified by Deon, not in ATP */
    2575.0f
  },

  {
    (Uint32_t)XPAR_IOCA_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x6Cu, /* IOC 0 FPGA temperature */
    16u,
    0.0625f,
    -273.15f,
    -50.0f,
    105.0f
  },

  /* IOC B */
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR, /* IOC 1 DISC_ADC_IOC_IN1 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    100.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x04u, /* IOC 1 DISC_ADC_IOC_IN2 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    100.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x08u, /* IOC 1 DISC_ADC_IOC_IN3 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    100.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x0Cu, /* IOC 1 DISC_ADC_IOC_IN4 */
    16u,
    8.594f,
    0.0f,
    0.0f,   /* Limits not specified in ATP */
    100.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x10u, /* IOC 1 P28V_IOCA_BIT */
    16u,
    9.521f,
    0.0f,
    24000.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x14u, /* IOC 1 P28V_IOCB_BIT */
    16u,
    9.521f,
    0.0f,
    24000.0f,
    29000.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x18u, /* IOC 1 P15V_IOCA_BIT */
    16u,
    3.979f,
    0.0f,
    13500.0f,
    16500.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x1Cu, /* IOC 1 P15V_IOCB_BIT */
    16u,
    3.979f,
    0.0f,
    13500.0f,
    16500.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x20u, /* IOC 1 P5V_IOCA_BIT */
    16u,
    1.343f,
    0.0f,
    4850.0f,
    5150.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x24u, /* IOC 1 P5V_IOCB_BIT */
    16u,
    1.343f,
    0.0f,
    4850.0f,
    5150.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x28u, /* IOC 1 P3V3_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    3169.0f,
    3432.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x2Cu, /* IOC 1 P3V3_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    3169.0f,
    3432.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x30u, /* IOC 1 P2V5_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    2350.0f,
    2650.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x34u, /* IOC 1 P2V5_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    2350.0f,
    2650.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x38u, /* IOC 1 P1V8_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    1728.0f,
    1872.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x3Cu, /* IOC 1 P1V8_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    1728.0f,
    1872.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x40u, /* IOC 1 P1V05_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    1008.0f,
    1092.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x44u, /* IOC 1 P1V05_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    1008.0f,
    1092.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x48u, /* IOC 1 P1V_IOCA_BIT */
    16u,
    1.221f,
    0.0f,
    965.0f,
    1045.2f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x4Cu, /* IOC 1 P1V_IOCB_BIT */
    16u,
    1.221f,
    0.0f,
    965.0f,
    1045.2f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x50u, /* IOC 1 P5V_GPIO_BIT */
    16u,
    1.343f,
    0.0f,
    4850.0f,
    5150.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x54u, /* IOC 1 P3V3_GPIO_BIT */
    16u,
    1.221f,
    0.0f,
    3168.0f,
    3432.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x58u, /* IOC 1 eFUSE_P5V_IMON1 */
    16u,
    3.308f,
    0.0f,
    300.0f,
    3000.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x5Cu, /* IOC 1 eFUSE_P5V_IMON2 */
    16u,
    3.308f,
    0.0f,
    300.0f,
    3000.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x60u, /* IOC 1 PF_VDD */
    16u,
    0.125f,
    0.0f,
    970.0f,      /* Limits specified by Deon, not in ATP */
    1030.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x64u, /* IOC 1 PF_VDD18 */
    16u,
    0.125f,
    0.0f,
    1746.0f,     /* Limits specified by Deon, not in ATP */
    1854.0f
  },
  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x68u, /* IOC 1 PF_VDD25 */
    16u,
    0.125f,
    0.0f,
    2425.0f,     /* Limits specified by Deon, not in ATP */
    2575.0f
  },

  {
    (Uint32_t)XPAR_IOCB_AXIOE_BASEADDR + IOC_ADC_REG_BASEADDR + 0x6Cu, /* IOC 1 FPGA temperature */
    16u,
    0.0625f,
    -273.15f,
    -50.0f,
    105.0f
  }

#endif
};

/* Define the number of PL based blocks */
__attribute__((weak)) d_ANALOGUE_PL_COUNT;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

