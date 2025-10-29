/*********************************************************************//**
\file
\brief
  Module Title       : Ethernet Interface Module

  Abstract           : Physical layer setup for the LWIP network stack library.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-109
                                                                SWREQ-110
                       SDD References: 136T-2200-131000-001-D22 SWDES-551
                                                                SWDES-675
                                                                SWDES-676
                                                                SWDES-677
                                                                SWDES-678
                                                                SWDES-679
                                                                SWDES-680
                                                                SWDES-681

\note
  CSC ID             : SWDES-67
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "netif/xemacpsif.h"

#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"

/***** Constants ********************************************************/

#define PHY_DETECT_REG              1
#define PHY_IDENTIFIER_1_REG          2
#define PHY_IDENTIFIER_2_REG          3
#define PHY_DETECT_MASK           0x1808
#define PHY_MARVELL_IDENTIFIER        0x0141
#define PHY_TI_IDENTIFIER          0x2000
#define PHY_REALTEK_IDENTIFIER        0x001c
#define PHY_XILINX_PCS_PMA_ID1      0x0174
#define PHY_XILINX_PCS_PMA_ID2      0x0C00

#define XEMACPS_GMII2RGMII_SPEED1000_FD    0x140
#define XEMACPS_GMII2RGMII_SPEED100_FD    0x2100
#define XEMACPS_GMII2RGMII_SPEED10_FD    0x100
#define XEMACPS_GMII2RGMII_REG_NUM      0x10

#define PHY_REGCR    0x0D
#define PHY_ADDAR    0x0E
#define PHY_RGMIIDCTL  0x86
#define PHY_RGMIICTL  0x32
#define PHY_STS      0x11
#define PHY_TI_CR    0x10
#define PHY_TI_CFG4    0x31

#define PHY_REGCR_ADDR  0x001F
#define PHY_REGCR_DATA  0x401F
#define PHY_TI_CRVAL  0x5048
#define PHY_TI_CFG4RESVDBIT7  0x80

/* Frequency setting */
#define SLCR_LOCK_ADDR      (XPS_SYS_CTRL_BASEADDR + 0x4)
#define SLCR_UNLOCK_ADDR    (XPS_SYS_CTRL_BASEADDR + 0x8)
#define SLCR_GEM0_CLK_CTRL_ADDR  (XPS_SYS_CTRL_BASEADDR + 0x140)
#define SLCR_GEM1_CLK_CTRL_ADDR  (XPS_SYS_CTRL_BASEADDR + 0x144)
#define SLCR_GEM_SRCSEL_EMIO  0x40
#define SLCR_LOCK_KEY_VALUE   0x767B
#define SLCR_UNLOCK_KEY_VALUE  0xDF0D
#define SLCR_ADDR_GEM_RST_CTRL  (XPS_SYS_CTRL_BASEADDR + 0x214)
#define EMACPS_SLCR_DIV_MASK  0xFC0FC0FF

#define IEEE_CTRL_RESET                         0x9140
#define IEEE_CTRL_ISOLATE_DISABLE               0xFBFF

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static void SetUpSLCRDivisors(Uint32_t mac_baseaddr);
static void getAbpDiv1000(Uint32_t mac_baseaddr, Uint32_t * pCrlApbDiv0, Uint32_t * pCrlApbDiv1);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- phy_setup_emacps -->

  Setup the physical layer interface.
*************************************************************************/
Uint32_t                     /** \return None */
phy_setup_emacps
(
XEmacPs *xemacpsp,           /**< [in] Pointer to emac structure */
Uint32_t phy_addr            /**< [in] PHY layer address. Not used */
)
{
  UNUSED_PARAMETER(phy_addr);

  Uint32_t speed = 1000u;

  SetUpSLCRDivisors(xemacpsp->Config.BaseAddress);

  return speed;
}

/*********************************************************************//**
  <!-- SetUpSLCRDivisors -->

  Setup the physical layer interface divisors.
*************************************************************************/
static void                  /** \return None */
SetUpSLCRDivisors
(
Uint32_t mac_baseaddr        /**< [in] MAC interface base address */
)
{
  volatile Uint32_t CrlApbBaseAddr;
  Uint32_t CrlApbDiv0 = 0u;
  Uint32_t CrlApbDiv1 = 0u;
  Uint32_t net_cfg;

  /* Setup divisors in CRL_APB for Zynq Ultrascale+ MPSoC */
  if (mac_baseaddr == ZYNQMP_EMACPS_0_BASEADDR) 
  {
    // cppcheck-suppress misra-c2012-7.2; The 'u' suffix cannot be appended as the item is defined in a Xilinx generated file.
    CrlApbBaseAddr = CRL_APB_GEM0_REF_CTRL;
  }
  else if (mac_baseaddr == ZYNQMP_EMACPS_1_BASEADDR) 
  {
    // cppcheck-suppress misra-c2012-7.2; The 'u' suffix cannot be appended as the item is defined in a Xilinx generated file.
    CrlApbBaseAddr = CRL_APB_GEM1_REF_CTRL;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    net_cfg = *(Uint32_t *)(ZYNQMP_EMACPS_1_BASEADDR + XEMACPS_NWCFG_OFFSET);
    net_cfg = net_cfg | XEMACPS_NWCFG_PCSSEL_MASK | XEMACPS_NWCFG_1000_MASK;
	  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    *(Uint32_t *)(ZYNQMP_EMACPS_1_BASEADDR + XEMACPS_NWCFG_OFFSET) = net_cfg;
  }
  else if (mac_baseaddr == ZYNQMP_EMACPS_2_BASEADDR) 
  {
    // cppcheck-suppress misra-c2012-7.2; The 'u' suffix cannot be appended as the item is defined in a Xilinx generated file.
    CrlApbBaseAddr = CRL_APB_GEM2_REF_CTRL;
	  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    net_cfg = *(Uint32_t *)(ZYNQMP_EMACPS_2_BASEADDR + XEMACPS_NWCFG_OFFSET);
    net_cfg = net_cfg | XEMACPS_NWCFG_PCSSEL_MASK | XEMACPS_NWCFG_1000_MASK;
	  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    *(Uint32_t *)(ZYNQMP_EMACPS_2_BASEADDR + XEMACPS_NWCFG_OFFSET) = net_cfg;
  }
  else if (mac_baseaddr == ZYNQMP_EMACPS_3_BASEADDR) 
  {
    // cppcheck-suppress misra-c2012-7.2; The 'u' suffix cannot be appended as the item is defined in a Xilinx generated file.
    CrlApbBaseAddr = CRL_APB_GEM3_REF_CTRL;
  }
  else
  {
    /* Do nothing */
  }

  getAbpDiv1000(mac_baseaddr, &CrlApbDiv0, &CrlApbDiv1);

  if ((CrlApbDiv0 != 0u) && (CrlApbDiv1 != 0u))
  {
    Uint32_t CrlApbGemCtrl = Xil_In32((UINTPTR)CrlApbBaseAddr);
    CrlApbGemCtrl &= ~CRL_APB_GEM_DIV0_MASK;
    CrlApbGemCtrl |= CrlApbDiv0 << CRL_APB_GEM_DIV0_SHIFT;
    CrlApbGemCtrl &= ~CRL_APB_GEM_DIV1_MASK;
    CrlApbGemCtrl |= CrlApbDiv1 << CRL_APB_GEM_DIV1_SHIFT;
    Xil_Out32((UINTPTR)CrlApbBaseAddr, CrlApbGemCtrl);
  }
  else 
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, CrlApbDiv0, CrlApbDiv1, 0, 0);
  }

  return;
}

/*********************************************************************//**
  <!-- getAbpDiv1000 -->

  Get the APB divisors for 1000 MHz.
*************************************************************************/
static void                  /** \return None */
getAbpDiv1000
(
Uint32_t mac_baseaddr,       /**< [in] MAC interface base address */
Uint32_t * pCrlApbDiv0,      /**< [out] Pointer to storage for divisor 0 */
Uint32_t * pCrlApbDiv1       /**< [out] Pointer to storage for divisor 1 */
)
{
  if (mac_baseaddr == ZYNQMP_EMACPS_0_BASEADDR)
  // clang-tidy-jst bugprone-branch-clone 1 Separate statements for clarity although they perform the same operation.
  {
    *pCrlApbDiv0 = XPAR_PSU_ETHERNET_0_ENET_SLCR_1000MBPS_DIV0;
    *pCrlApbDiv1 = XPAR_PSU_ETHERNET_0_ENET_SLCR_1000MBPS_DIV1;
  }
  else if (mac_baseaddr == ZYNQMP_EMACPS_1_BASEADDR)
  {
    *pCrlApbDiv0 = XPAR_PSU_ETHERNET_1_ENET_SLCR_1000MBPS_DIV0;
    *pCrlApbDiv1 = XPAR_PSU_ETHERNET_1_ENET_SLCR_1000MBPS_DIV1;
  }
  else if (mac_baseaddr == ZYNQMP_EMACPS_2_BASEADDR)
  {
    *pCrlApbDiv0 = XPAR_PSU_ETHERNET_2_ENET_SLCR_1000MBPS_DIV0;
    *pCrlApbDiv1 = XPAR_PSU_ETHERNET_2_ENET_SLCR_1000MBPS_DIV1;
  }
  else if (mac_baseaddr == ZYNQMP_EMACPS_3_BASEADDR)
  {
    *pCrlApbDiv0 = XPAR_PSU_ETHERNET_3_ENET_SLCR_1000MBPS_DIV0;
    *pCrlApbDiv1 = XPAR_PSU_ETHERNET_3_ENET_SLCR_1000MBPS_DIV1;
  }
  else
  {
    /* This will never be executed */
  }
  
  return;
}

