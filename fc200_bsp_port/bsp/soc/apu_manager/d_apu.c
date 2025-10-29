/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_apu.c

  Abstract           : Start the APU processor core 0.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-
                       SDD References: 136T-2200-131100-001-D22 SWDES-

\note
  CSC ID             : SWDES-
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/memory_manager/d_memory_cache.h"
#include "kernel/general/d_gen_register.h"

#include "soc/apu_manager/d_apu.h"

/***** Constants ********************************************************/

/* APU Base Address */
#define APU_BASEADDR      0xFD5C0000u

/* Register: APU_CONFIG_0 */
#define APU_CONFIG_0    ((APU_BASEADDR) + 0x00000020u)
#define APU_CONFIG_0_VINITHI_MASK  0x000000F0u

/* Register: APU_RVBARADDR0L */
#define APU_RVBARADDR0L    ((APU_BASEADDR) + 0x00000040u)

/* Register: APU_RVBARADDR0H */
#define APU_RVBARADDR0H    ((APU_BASEADDR) + 0x00000044u)

/* PMU_GLOBAL Base Address */
#define PMU_GLOBAL_BASEADDR      0xFFD80000u

/* Register: PMU_GLOBAL_REQ_PWRUP_INT_EN */
#define PMU_GLOBAL_REQ_PWRUP_INT_EN    ((PMU_GLOBAL_BASEADDR) + 0x00000118u)
#define PMU_GLOBAL_REQ_PWRUP_INT_EN_PL_MASK    0x00800000u

/* Register: PMU_GLOBAL_REQ_PWRUP_TRIG */
#define PMU_GLOBAL_REQ_PWRUP_TRIG    ((PMU_GLOBAL_BASEADDR) + 0x00000120u)
#define PMU_GLOBAL_REQ_PWRUP_TRIG_PL_MASK    0x00800000u

/* Register: PMU_GLOBAL_REQ_PWRUP_STATUS */
#define PMU_GLOBAL_REQ_PWRUP_STATUS    ((PMU_GLOBAL_BASEADDR) + 0x00000110u)
#define PMU_GLOBAL_REQ_PWRUP_STATUS_PL_SHIFT   23U
#define PMU_GLOBAL_REQ_PWRUP_STATUS_PL_MASK    0x00800000u

/* Register: PMU_GLOBAL_PWR_STATE */
#define PMU_GLOBAL_PWR_STATE    ((PMU_GLOBAL_BASEADDR) + 0x00000100u)
#define PMU_GLOBAL_PWR_STATE_PL_MASK           0x00800000u
#define PMU_GLOBAL_PWR_STATE_FP_MASK           0x00400000u
#define PMU_GLOBAL_PWR_STATE_USB1_MASK         0x00200000u
#define PMU_GLOBAL_PWR_STATE_USB0_MASK         0x00100000u
#define PMU_GLOBAL_PWR_STATE_OCM_BANK3_MASK    0x00080000u
#define PMU_GLOBAL_PWR_STATE_OCM_BANK2_MASK    0x00040000u
#define PMU_GLOBAL_PWR_STATE_OCM_BANK1_MASK    0x00020000u
#define PMU_GLOBAL_PWR_STATE_OCM_BANK0_MASK    0x00010000u
#define PMU_GLOBAL_PWR_STATE_TCM1B_MASK        (Uint32_t)0x00008000u
#define PMU_GLOBAL_PWR_STATE_TCM1A_MASK        (Uint32_t)0x00004000u
#define PMU_GLOBAL_PWR_STATE_TCM0B_MASK        (Uint32_t)0x00002000u
#define PMU_GLOBAL_PWR_STATE_TCM0A_MASK        (Uint32_t)0x00001000u
#define PMU_GLOBAL_PWR_STATE_R5_1_MASK         (Uint32_t)0x00000800u
#define PMU_GLOBAL_PWR_STATE_R5_0_MASK         (Uint32_t)0x00000400u
#define PMU_GLOBAL_PWR_STATE_L2_BANK0_MASK     0x00000080u
#define PMU_GLOBAL_PWR_STATE_PP1_MASK          0x00000020u
#define PMU_GLOBAL_PWR_STATE_PP0_MASK          0x00000010u
#define PMU_GLOBAL_PWR_STATE_ACPU3_MASK        0x00000008u
#define PMU_GLOBAL_PWR_STATE_ACPU2_MASK        0x00000004u
#define PMU_GLOBAL_PWR_STATE_ACPU1_MASK        0x00000002u
#define PMU_GLOBAL_PWR_STATE_ACPU0_MASK        0x00000001u

/* CRF_APB Base Address */
#define CRF_APB_BASEADDR      0xFD1A0000u

/* Register: CRF_APB_RST_FPD_APU */
#define CRF_APB_RST_FPD_APU    ((CRF_APB_BASEADDR) + 0x00000104u)
#define CRF_APB_RST_FPD_APU_ACPU0_RESET_MASK  (Uint32_t)0x00000001u
#define CRF_APB_RST_FPD_APU_APU_L2_RESET_MASK (Uint32_t)0x00000100u

/* Register: CRF_APB_ACPU_CTRL */
#define CRF_APB_ACPU_CTRL    ((CRF_APB_BASEADDR) + 0x00000060u)
#define CRF_APB_ACPU_CTRL_CLKACT_FULL_MASK    0x01000000u
#define CRF_APB_ACPU_CTRL_CLKACT_HALF_MASK    0x02000000u

#define CRF_APB_RST_FPD_APU_ACPU0_PWRON_RESET_MASK    (Uint32_t)0x00000400u

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static d_Status_t SetCpuPwrSettings (void);
static d_Status_t PowerUpIsland(Uint32_t PwrIslandMask);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_APU_Start -->

  Start the APU.
*************************************************************************/
d_Status_t d_APU_Start(const Uint32_t address)
{
  d_Status_t status;

  d_MEMORY_DCacheDisable();

  /* Update reset vector */
  d_GEN_RegisterWrite(APU_RVBARADDR0L, address);

  /* RPU can only access lower 4GB */
  d_GEN_RegisterWrite(APU_RVBARADDR0H, 0u);

  /* Take CPU out of reset */
  status = SetCpuPwrSettings();

  d_MEMORY_DCacheEnable();

  return status;
}

/*********************************************************************//**
  <!-- SetCpuPwrSettings -->

  Power up the APU, enable the clock and release the reset.
*************************************************************************/
static d_Status_t
SetCpuPwrSettings
(
void
)
{
  d_Status_t status;
  Uint32_t PwrStateMask;

  /* Reset the CPU */
  PwrStateMask = PMU_GLOBAL_PWR_STATE_ACPU0_MASK |
  PMU_GLOBAL_PWR_STATE_FP_MASK |
  PMU_GLOBAL_PWR_STATE_L2_BANK0_MASK;

  status = PowerUpIsland(PwrStateMask);

  if (status == d_STATUS_SUCCESS)
  {
    Uint32_t RegValue;
    /* Enable the clock */
    RegValue = d_GEN_RegisterRead(CRF_APB_ACPU_CTRL);
    RegValue |= (CRF_APB_ACPU_CTRL_CLKACT_FULL_MASK |
                 CRF_APB_ACPU_CTRL_CLKACT_HALF_MASK);
    d_GEN_RegisterWrite(CRF_APB_ACPU_CTRL, RegValue);

    /* Release reset */
    RegValue = d_GEN_RegisterRead(CRF_APB_RST_FPD_APU);
    RegValue &= ~(CRF_APB_RST_FPD_APU_ACPU0_RESET_MASK |
                  CRF_APB_RST_FPD_APU_APU_L2_RESET_MASK |
                  CRF_APB_RST_FPD_APU_ACPU0_PWRON_RESET_MASK);
    d_GEN_RegisterWrite(CRF_APB_RST_FPD_APU, RegValue);
  }

  return status;
}

/*********************************************************************//**
  <!-- PowerUpIsland -->

  Power up a power island.
*************************************************************************/
static d_Status_t
PowerUpIsland
(
Uint32_t PwrIslandMask
)
{
  Uint32_t regVal;
  d_Status_t status = d_STATUS_SUCCESS;

  /* Power up request enable */
  d_GEN_RegisterWrite(PMU_GLOBAL_REQ_PWRUP_INT_EN, PwrIslandMask);

  /* Trigger power up request */
  d_GEN_RegisterWrite(PMU_GLOBAL_REQ_PWRUP_TRIG, PwrIslandMask);

  /* Poll for Power up complete */
  do
  {
    regVal = d_GEN_RegisterRead(PMU_GLOBAL_REQ_PWRUP_STATUS) & PwrIslandMask;
  } while (regVal != 0x0u);

  return status;
}

