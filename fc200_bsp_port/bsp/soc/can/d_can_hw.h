/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_can_hw.h

  Abstract           : CAN driver hardware definitions.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-325
                       SDD References: 136T-2200-131100-001-D22 SWDES-117
                                                                SWDES-541
                                                                SWDES-606
                                                                SWDES-607
                                                                SWDES-608
                                                                SWDES-609
                                                                SWDES-610
                                                                SWDES-611
                                                                SWDES-612
                                                                SWDES-613
                                                                SWDES-614

\note
  CSC ID             : SWDES-47
*************************************************************************/

#ifndef D_CAN_HW_H
#define D_CAN_HW_H

/***** Includes *********************************************************/

/***** Literals *********************************************************/

/* Register offsets for the CAN. Each register is 32 bits */
#define d_CAN_REGISTER_SRR_OFFSET          0x00000000u /* Software Reset Register */
#define d_CAN_REGISTER_MSR_OFFSET          0x00000004u /* Mode Select Register */
#define d_CAN_REGISTER_BRPR_OFFSET         0x00000008u /* Baud Rate Prescaler */
#define d_CAN_REGISTER_BTR_OFFSET          0x0000000Cu /* Bit Timing Register */
#define d_CAN_REGISTER_ECR_OFFSET          0x00000010u /* Error Counter Register */
#define d_CAN_REGISTER_ESR_OFFSET          0x00000014u /* Error Status Register */
#define d_CAN_REGISTER_SR_OFFSET           0x00000018u /* Status Register */
#define d_CAN_REGISTER_ISR_OFFSET          0x0000001Cu /* Interrupt Status Register */

#define d_CAN_REGISTER_IER_OFFSET          0x00000020u /* Interrupt Enable Register */
#define d_CAN_REGISTER_ICR_OFFSET          0x00000024u /* Interrupt Clear Register */
#define d_CAN_REGISTER_TCR_OFFSET          0x00000028u /* Timestamp Control Register */
#define d_CAN_REGISTER_WIR_OFFSET          0x0000002Cu /* Watermark Interrupt Reg */

#define d_CAN_REGISTER_TXFIFO_ID_OFFSET    0x00000030u /* TX FIFO ID */
#define d_CAN_REGISTER_TXFIFO_DLC_OFFSET   0x00000034u /* TX FIFO DLC */
#define d_CAN_REGISTER_TXFIFO_DW1_OFFSET   0x00000038u /* TX FIFO Data Word 1 */
#define d_CAN_REGISTER_TXFIFO_DW2_OFFSET   0x0000003Cu /* TX FIFO Data Word 2 */

#define d_CAN_REGISTER_TXHPB_ID_OFFSET     0x00000040u /* TX High Priority Buffer ID */
#define d_CAN_REGISTER_TXHPB_DLC_OFFSET    0x00000044u /* TX High Priority Buffer DLC */
#define d_CAN_REGISTER_TXHPB_DW1_OFFSET    0x00000048u /* TX High Priority Buf Data 1 */
#define d_CAN_REGISTER_TXHPB_DW2_OFFSET    0x0000004Cu /* TX High Priority Buf Data Word 2 */

#define d_CAN_REGISTER_RXFIFO_ID_OFFSET    0x00000050u /* RX FIFO ID */
#define d_CAN_REGISTER_RXFIFO_DLC_OFFSET   0x00000054u /* RX FIFO DLC */
#define d_CAN_REGISTER_RXFIFO_DW1_OFFSET   0x00000058u /* RX FIFO Data Word 1 */
#define d_CAN_REGISTER_RXFIFO_DW2_OFFSET   0x0000005Cu /* RX FIFO Data Word 2 */

#define d_CAN_REGISTER_AFR_OFFSET          0x00000060u /* Acceptance Filter Register */
#define d_CAN_REGISTER_AFMR1_OFFSET        0x00000064u /* Acceptance Filter Mask 1 */
#define d_CAN_REGISTER_AFIR1_OFFSET        0x00000068u /* Acceptance Filter ID  1 */
#define d_CAN_REGISTER_AFMR2_OFFSET        0x0000006Cu /* Acceptance Filter Mask  2 */
#define d_CAN_REGISTER_AFIR2_OFFSET        0x00000070u /* Acceptance Filter ID 2 */
#define d_CAN_REGISTER_AFMR3_OFFSET        0x00000074u /* Acceptance Filter Mask 3 */
#define d_CAN_REGISTER_AFIR3_OFFSET        0x00000078u /* Acceptance Filter ID 3 */
#define d_CAN_REGISTER_AFMR4_OFFSET        0x0000007Cu /* Acceptance Filter Mask  4 */
#define d_CAN_REGISTER_AFIR4_OFFSET        0x00000080u /* Acceptance Filter ID 4 */

/* Software Reset Register (SRR) Bit Definitions and Masks */
#define d_CAN_SRR_CEN_MASK    0x00000002u  /* Can Enable */
#define d_CAN_SRR_SRST_MASK   0x00000001u  /* Reset */

/* Mode Select Register (MSR) Bit Definitions and Masks */
#define d_CAN_MSR_SNOOP_MASK  0x00000004u /* Snoop Mode Select */
#define d_CAN_MSR_LBACK_MASK  0x00000002u /* Loop Back Mode Select */
#define d_CAN_MSR_SLEEP_MASK  0x00000001u /* Sleep Mode Select */

/* Baud Rate Prescaler register (BRPR) Bit Definitions and Masks */
#define d_CAN_BRPR_BRP_MASK  0x000000FFu /* Baud Rate Prescaler */

/* Bit Timing Register (BTR) Bit Definitions and Masks */
#define d_CAN_BTR_SJW_MASK  0x00000180u /* Synchronisation Jump Width */
#define d_CAN_BTR_SJW_SHIFT  7u
#define d_CAN_BTR_TS2_MASK  0x00000070u /* Time Segment 2 */
#define d_CAN_BTR_TS2_SHIFT  4u
#define d_CAN_BTR_TS1_MASK  0x0000000Fu /* Time Segment 1 */

/* Error Counter Register (ECR) Bit Definitions and Masks */
#define d_CAN_ECR_REC_MASK  0x0000FF00u /* Receive Error Counter */
#define d_CAN_ECR_REC_SHIFT     8u
#define d_CAN_ECR_TEC_MASK  0x000000FFu /* Transmit Error Counter */

/* Error Status Register (ESR) Bit Definitions and Masks */
#define d_CAN_ESR_ACKER_MASK  0x00000010u /* ACK Error */
#define d_CAN_ESR_BERR_MASK   0x00000008u /* Bit Error */
#define d_CAN_ESR_STER_MASK   0x00000004u /* Stuff Error */
#define d_CAN_ESR_FMER_MASK   0x00000002u /* Form Error */
#define d_CAN_ESR_CRCER_MASK  0x00000001u /* CRC Error */

/* Status Register (SR) Bit Definitions and Masks */
#define d_CAN_SR_SNOOP_MASK   0x00001000u /* Snoop Mask */
#define d_CAN_SR_ACFBSY_MASK  0x00000800u /* Acceptance Filter busy */
#define d_CAN_SR_TXFLL_MASK   0x00000400u /* TX FIFO is full */
#define d_CAN_SR_TXBFLL_MASK  0x00000200u /* TX High Priority Buffer full */
#define d_CAN_SR_ESTAT_MASK   0x00000180u /* Error Status */
#define d_CAN_SR_ESTAT_SHIFT           7u
#define d_CAN_SR_ERRWRN_MASK  0x00000040u /* Error Warning */
#define d_CAN_SR_BBSY_MASK    0x00000020u /* Bus Busy */
#define d_CAN_SR_BIDLE_MASK   0x00000010u /* Bus Idle */
#define d_CAN_SR_NORMAL_MASK  0x00000008u /* Normal Mode */
#define d_CAN_SR_SLEEP_MASK   0x00000004u /* Sleep Mode */
#define d_CAN_SR_LBACK_MASK   0x00000002u /* Loop Back Mode */
#define d_CAN_SR_CONFIG_MASK  0x00000001u /* Configuration Mode */

/* Interrupt Status/Enable/Clear Register Bit Definitions and Masks */
#define d_CAN_IXR_TXFEMP_MASK   0x00004000u /* Tx Fifo Empty Interrupt */
#define d_CAN_IXR_TXFWMEMP_MASK 0x00002000u /* Tx Fifo Watermark Empty */
#define d_CAN_IXR_RXFWMFLL_MASK 0x00001000u /* Rx FIFO Watermark Full */
#define d_CAN_IXR_WKUP_MASK     0x00000800u /* Wake up Interrupt */
#define d_CAN_IXR_SLP_MASK      0x00000400u /* Sleep Interrupt */
#define d_CAN_IXR_BSOFF_MASK    0x00000200u /* Bus Off Interrupt */
#define d_CAN_IXR_ERROR_MASK    0x00000100u /* Error Interrupt */
#define d_CAN_IXR_RXNEMP_MASK   0x00000080u /* RX FIFO Not Empty Interrupt */
#define d_CAN_IXR_RXOFLW_MASK   0x00000040u /* RX FIFO Overflow Interrupt */
#define d_CAN_IXR_RXUFLW_MASK   0x00000020u /* RX FIFO Underflow Interrupt */
#define d_CAN_IXR_RXOK_MASK     0x00000010u /* New Message Received Intr */
#define d_CAN_IXR_TXBFLL_MASK   0x00000008u /* TX High Priority Buf Full */
#define d_CAN_IXR_TXFLL_MASK    0x00000004u /* TX FIFO Full Interrupt */
#define d_CAN_IXR_TXOK_MASK     0x00000002u /* TX Successful Interrupt */
#define d_CAN_IXR_ARBLST_MASK   0x00000001u /* Arbitration Lost Interrupt */
#define d_CAN_IXR_ALL    ((Uint32_t)d_CAN_IXR_RXFWMFLL_MASK | \
                          (Uint32_t)d_CAN_IXR_WKUP_MASK     | \
                          (Uint32_t)d_CAN_IXR_SLP_MASK      | \
                          (Uint32_t)d_CAN_IXR_BSOFF_MASK    | \
                          (Uint32_t)d_CAN_IXR_ERROR_MASK    | \
                          (Uint32_t)d_CAN_IXR_RXNEMP_MASK   | \
                          (Uint32_t)d_CAN_IXR_RXOFLW_MASK   | \
                          (Uint32_t)d_CAN_IXR_RXUFLW_MASK   | \
                          (Uint32_t)d_CAN_IXR_RXOK_MASK     | \
                          (Uint32_t)d_CAN_IXR_TXBFLL_MASK   | \
                          (Uint32_t)d_CAN_IXR_TXFLL_MASK    | \
                          (Uint32_t)d_CAN_IXR_TXOK_MASK     | \
                          (Uint32_t)d_CAN_IXR_ARBLST_MASK)

/* CAN Timestamp Control Register (TCR) Bit Definitions and Masks */
#define d_CAN_TCR_CTS_MASK  0x00000001u /* Clear Timestamp counter mask */

/* CAN Watermark Register (WIR) Bit Definitions and Masks */
#define d_CAN_WIR_FW_MASK     0x0000003Fu /* Rx Full Threshold mask */
#define d_CAN_WIR_EW_MASK   0x00003F00u /* Tx Empty Threshold mask */
#define d_CAN_WIR_EW_SHIFT   0x00000008u /* Tx Empty Threshold shift */

/* CAN Frame Identifier (TX High Priority Buffer/TX/RX/Acceptance Filter
        Mask/Acceptance Filter ID) */
#define d_CAN_IDR_ID1_MASK  0xFFE00000u /* Standard Messg Identifier */
#define d_CAN_IDR_ID1_SHIFT  21u
#define d_CAN_IDR_SRR_MASK  0x00100000u /* Substitute Remote TX Req */
#define d_CAN_IDR_SRR_SHIFT  20u
#define d_CAN_IDR_IDE_MASK  0x00080000u /* Identifier Extension */
#define d_CAN_IDR_IDE_SHIFT  19u
#define d_CAN_IDR_ID2_MASK  0x0007FFFEu /* Extended Message Ident */
#define d_CAN_IDR_ID2_SHIFT  1u
#define d_CAN_IDR_RTR_MASK  0x00000001u /* Remote TX Request */

/* CAN Frame Data Length Code (TX High Priority Buffer/TX/RX) */
#define d_CAN_DLCR_DLC_MASK   0xF0000000u  /* Data Length Code */
#define d_CAN_DLCR_DLC_SHIFT   28u
#define d_CAN_DLCR_TIMESTAMP_MASK 0x0000FFFFu  /* Timestamp Mask (Rx only) */


/* CAN Frame Data Word 1 (TX High Priority Buffer/TX/RX) */
#define d_CAN_DW1R_DB0_MASK  0xFF000000u /* Data Byte 0 */
#define d_CAN_DW1R_DB0_SHIFT  24u
#define d_CAN_DW1R_DB1_MASK  0x00FF0000u /* Data Byte 1 */
#define d_CAN_DW1R_DB1_SHIFT  16u
#define d_CAN_DW1R_DB2_MASK  0x0000FF00u /* Data Byte 2 */
#define d_CAN_DW1R_DB2_SHIFT  8u
#define d_CAN_DW1R_DB3_MASK  0x000000FFu /* Data Byte 3 */

/* CAN Frame Data Word 2 (TX High Priority Buffer/TX/RX) */
#define d_CAN_DW2R_DB4_MASK  0xFF000000u /* Data Byte 4 */
#define d_CAN_DW2R_DB4_SHIFT  24u
#define d_CAN_DW2R_DB5_MASK  0x00FF0000u /* Data Byte 5 */
#define d_CAN_DW2R_DB5_SHIFT  16u
#define d_CAN_DW2R_DB6_MASK  0x0000FF00u /* Data Byte 6 */
#define d_CAN_DW2R_DB6_SHIFT  8u
#define d_CAN_DW2R_DB7_MASK  0x000000FFu /* Data Byte 7 */

/* Acceptance Filter Register (AFR) Bit Definitions and Masks */
#define d_CAN_AFR_UAF4_MASK  0x00000008u /* Use Acceptance Filter No.4 */
#define d_CAN_AFR_UAF3_MASK  0x00000004u /* Use Acceptance Filter No.3 */
#define d_CAN_AFR_UAF2_MASK  0x00000002u /* Use Acceptance Filter No.2 */
#define d_CAN_AFR_UAF1_MASK  0x00000001u /* Use Acceptance Filter No.1 */
#define d_CAN_AFR_UAF_ALL_MASK  ((Uint32_t)d_CAN_AFR_UAF4_MASK | \
          (Uint32_t)d_CAN_AFR_UAF3_MASK | \
          (Uint32_t)d_CAN_AFR_UAF2_MASK | \
          (Uint32_t)d_CAN_AFR_UAF1_MASK)

/* CAN frame length constants */
#define d_CAN_MAX_FRAME_SIZE_IN_WORDS 16u
/* Maximum CAN frame length in bytes */
#define d_CAN_MAX_FRAME_SIZE (d_CAN_MAX_FRAME_SIZE_IN_WORDS * sizeof(Uint32_t))

/***** Constants ********************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_CAN_HW_H */

