/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : d_sata_ops

  Abstract : SOC SATA driver operations file

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-247
                       SDD References: 136T-2200-131000-001-D22 SWDES-2693
                                                                SWDES-2694
                                                                SWDES-2695
                                                                SWDES-2696
                                                                SWDES-2697
\note
  CSC ID             : SWDES-48
*************************************************************************/

#ifndef D_SATA_OPS_H
#define D_SATA_OPS_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

#include "d_sata.h"
#include "d_sata_encryption.h"

#include "xparameters.h"                          /* PS defines for Zynq */
#include "xparameters_ps.h"
#include "xresetps_hw.h"
#include "xclockps_hw.h"

/***** Constants ********************************************************/

#define AHCI_PORT_DMA_SIZE   (0x17400u)

#define CAP_REGISTER        ((Uint32_t)0x00u)                 // HBA Capabilities
#define GHC_REGISTER        ((Uint32_t)0x04u)                 // Global Host control register offset
#define PI_REGISTER         ((Uint32_t)0x0Cu)                   // Ports implemented register
#define EM_CTL_REGISTER     ((Uint32_t)0x20u)                   // Enclosure Management Control register
#define CAP2_REGISTER       ((Uint32_t)0x24u)                   // HBA Capabilities Extended
#define IS_REGISTER         ((Uint32_t)0x08u)                   // Host interrupt status
#define VS_REGISTER         ((Uint32_t)0x10u)                   // AHCI Version

// SATA AHCI Port registers
#define PORT_REG_OFFSET     ((Uint32_t)0x80u)                   // offset between port 0 and 1 registers
#define PxCLB_REGISTER      ((Uint32_t)0x100u)                  // port command list base address
#define PxCLBU_REGISTER     ((Uint32_t)0x104u)                  // port command list base address upper 32bits
#define PxFB_REGISTER       ((Uint32_t)0x108u)                  // port FIS Base address
#define PxFBU_REGISTER      ((Uint32_t)0x10Cu)                  // port FIS Base address upper 32bits 
#define PxIS_REGISTER       ((Uint32_t)0x110u)                  // port Interrupt status
#define PxCMD_REGISTER      ((Uint32_t)0x118u)                  // Port Command and status register
#define PxSSTS_REGISTER     ((Uint32_t)0x128u)                  // port SATA Status register
#define PxSCTL_REGISTER     ((Uint32_t)0x12Cu)                  // port SATA Control register
#define PxSERR_REGISTER     ((Uint32_t)0x130u)                  // port SATA Error  and diagnostics
#define PxTFD_REGISTER      ((Uint32_t)0x120u)                  // port SATA Task File Data
#define PxCI_REGISTER       ((Uint32_t)0x138u)                  // port SATA command issue register


// Vendor Specific registers
#define PCFG_REGISTER     ((Uint32_t)0xA4u)                     // Port Configuration register 
#define PAXIC_REGISTER    ((Uint32_t)0xC0u)                     // AXI Control register
#define PPCFG_REGISTER    ((Uint32_t)0xA8u)                     // Port Phy configuration: control layer
#define PP2C_REGISTER     ((Uint32_t)0xACu)                     // Port PHY configuration 2 (Phy2Cfg): OOB timing for COMMINIT.
#define PP3C_REGISTER     ((Uint32_t)0xB0u)                     // Port PHY Configuration 3 (Phy3CFg): OOB timing for the COMMINIT.
#define PP4C_REGISTER     ((Uint32_t)0xB4u)                     // Port PHY Configuration 4: burst timing in COM.
#define PP5C_REGISTER     ((Uint32_t)0xB8u)                     // Port PHY Configuration 5: retry, interval time.
#define PTC_REGISTER      ((Uint32_t)0xC8u)                     // Port transfer configuration (TransCfg): transport layer.

// SERDES Registers
#define L0_PLL_FBDIV_FRAC_3_MSB       ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x2360u)
#define L0_PLL_SS_STEP_SIZE_3_MSB     ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x237Cu)
#define L1_PLL_FBDIV_FRAC_3_MSB       ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x6360u)
#define L1_PLL_SS_STEP_SIZE_3_MSB     ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x637Cu)
#define L2_PLL_FBDIV_FRAC_3_MSB       ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0xA360u)
#define L2_PLL_SS_STEP_SIZE_3_MSB     ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0xA37Cu)
#define L3_PLL_FBDIV_FRAC_3_MSB       ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0xE360u)
#define L3_PLL_SS_STEP_SIZE_3_MSB     ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0xE37Cu)
#define ICM_CFG0                      ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x10010u)
#define PLL_REF_SEL0                  ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x10000u)
#define L0_TM_DIG_6                   ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x106Cu)
#define L0_TX_DIG_61                  ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x00F4u)  // Not in the datasheet, got this one from the Linux Driver
#define L0_PLL_STATUS_READ_1          ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x23E4u)
#define L0_TM_PLL_DIG_37              ((Uint32_t)XPAR_PSU_SERDES_S_AXI_BASEADDR + 0x2094u)

// SIOU registers
#define SATA_MISC_CTRL_REGISTER       ((Uint32_t)XPAR_PSU_SIOU_S_AXI_BASEADDR + 0x100u)

// Host Control register (GHC) bits 
#define HOST_CONTROL_AHCI_ENABLE      ((Uint32_t)1 << 31u)
#define HOST_CONTROL_HBA_RESET        ((Uint32_t)1u)
#define HOST_CONTROL_INETRRUPT_ENABLE ((Uint32_t)2u)

// capability register (CAP) bits
#define CAP_SPM                       ((Uint32_t)1u << 17u)  
#define CAP_SMPS                      ((Uint32_t)1u << 28u)
#define CAP_SSS                       ((Uint32_t)1 << 27u)

// EM CTRL register bits
#define EM_CTL_RESET                  ((Uint32_t)1u << 9u)

// SATA Port 1 and 2 register bits
#define PORT_SCTL_SPD_GEN1          ((Uint32_t)1u << 4u)
#define PORT_SCTL_IPM               ((Uint32_t)3u << 8u)
#define PORT_CMD_ICC_ACTIVE         ((Uint32_t)1u << 28u)
#define PORT_CMD_FRE                ((Uint32_t)1u << 4u)      // FIS Rx enable
#define PORT_CMD_ST                 ((Uint32_t)1u)
#define PORT_CMD_LIST_ON            ((Uint32_t)1u << 15u)
#define PORT_CMD_FIS_ON             ((Uint32_t)1u << 14u)
#define PORT_CMD_SPIN_UP            ((Uint32_t)1u << 1u)
#define PORT_CMD_POD                ((Uint32_t)1u << 2u)
#define PORT_SSTS_DET_MASK          ((Uint32_t)0x03u)
#define PORT_STS_DET_PHYRDY         ((Uint32_t)0x03u) // Status detect Phy ready
#define PORT_STS_DET_COMINIT        ((Uint32_t)0x01u)
#define PORT_TFD_ATA_BUSY           ((Uint32_t)1u << 7u)
#define PORT_TFD_ATA_DRQ            ((Uint32_t)1u << 3u) // Data request i/o

// Port Config register (PCFG) bits
#define PORT_CONTROL_TPSS_VAL       ((Uint32_t)0x32u << 16u)
#define PORT_CONTROL_TPRS_VAL       ((Uint32_t)0x02u << 12u)
#define PORT_CONTROL_PAD_VAL        ((Uint32_t)0x02u)          // selects port 0

// AXI Control register (PAXIC) bits
#define PAXIC_BUS_WIDTH_64            ((Uint32_t)0x01u)
#define PAXIC_TRANSFER_LIMIT          ((Uint32_t)0x4u << 20u)
#define PAXIC_MEM_ADDR_READ_ID_DATA   ((Uint32_t)1 << 16u)
#define PAXIC_MEM_ADDR_WRITE_ID_DATA  ((Uint32_t)1 << 8u)

// Port Phy Config Bits
#define PPCFG_TTA                     ((Uint32_t)0x1FFFEu)
#define PPCFG_PSSO_EN	              ((Uint32_t)1u << 28u)
#define PPCFG_PSS_EN                  ((Uint32_t)1u << 29u)
#define PPCFG_ESDF_EN 	              ((Uint32_t)1u << 31u)

// Port transfer configuration (PTC) register bits
#define PTC_RX_WM_VAL 	              ((Uint32_t)0x40u)
#define PTC_RSVD                      ((Uint32_t)1u << 27u)

// SERDES Register bits
#define TM_FORCE_EN_FRAC              ((Uint32_t)1u << 6u)
#define TM_FORCE_EN_SS                ((Uint32_t)1u << 7u)
#define L0_ICM_CFG_MASK               ((Uint32_t)0x07u)
#define L0_ICM_CFG_VALUE              ((Uint32_t)0x02u)
#define PLL_REF_SEL_MASK              ((Uint32_t)0x1Fu)
#define PLL_REF_SEL_150MHZ            ((Uint32_t)0x11u)
#define SCRAMBLE_DESCRAMBLE_ENCODER_DECODER_VALUE ((Uint32_t)0x0Fu)
#define PLL_LOCK_STATUS               ((Uint32_t)1u << 4u) 
#define L0_TM_COARSE_CODE_LIMIT       ((Uint32_t)0x10u)

// SIOU Register bits
#define SATA_PM_CLK_VAL               ((Uint32_t)0x03u)

#define ATA_SECT_SIZE                 ((Uint32_t)512u)

/***** Type Definitions *************************************************/

typedef struct 
{
	Uint32_t	opts;
	Uint32_t	status;
	Uint32_t	tbl_addr;
	Uint32_t	tbl_addr_hi;
	Uint32_t	reserved[4];
} AhchiCmdHdr_t;          // AHCI command header

typedef struct 
{
	Uint32_t	addr;
	Uint32_t	addr_hi;
	Uint32_t	reserved;
	Uint32_t	flags_size;
} AhciSg_t;  // AHCI scatter-gather table

typedef struct
{
  AhchiCmdHdr_t *cmdSlot;
  AhciSg_t *cmdTblSg;
  Uint32_t cmdTbl;
  Uint32_t rxFis;
} AhciCmdInfo_t;


/***** Variables ********************************************************/

extern AhciCmdInfo_t cmdInfo;
extern Uint8_t dmaMem[AHCI_PORT_DMA_SIZE]; // DMA memory must be 1K-byte aligned
extern d_SATA_DeviceInfo_t d_SATA_DeviceInfo;

/***** Function Declarations ********************************************/

Uint32_t d_SATA_RegisterRead(Uint32_t offset);
void d_SATA_RegisterWrite(Uint32_t offset, Uint32_t value);

d_Status_t d_SATAOP_ReadSmartData(const Uint32_t port, d_SATA_SmartInfo_t *pSmartInfo);

d_Status_t d_SATAOP_Inquiry(const Uint32_t port);

d_Status_t d_SATAOP_ReadWrite(const Uint32_t port, const Uint32_t lba, const Uint8_t * const pBuffer, const Uint32_t readLength, const Bool_t writeFlag);

d_Status_t d_SATAOP_Flush(const Uint32_t port);

d_Status_t d_SATAOP_Discovery0(const Uint32_t port, d_SATAENC_SelfEncryptionStatus_t * pLockStatus);

d_Status_t d_SATAOP_EncInitialSetup(const Uint32_t port, const Char_t * const password, const Uint32_t pwdLength);

d_Status_t d_SATAOP_SetLockingUnlocking(const Uint32_t port, const Char_t * const password, const Uint32_t pwdLength, const Bool_t lock);

d_Status_t d_SATAOP_EnableDisableLockingRange(const Uint32_t port, const Char_t * const password, const Uint32_t pwdLength, const Bool_t lockingEnabled);




#endif // D_SATA_OPS_H
