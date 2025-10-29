/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_can_holt

  Abstract           : CAN interface using Holt HI-3110 device via SPI.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-213
                       SDD References: 136T-2200-131100-001-D22 SWDES-549
                                                                SWDES-625
                                                                SWDES-626
                                                                SWDES-627
                                                                SWDES-628
                                                                SWDES-629
                                                                SWDES-630
                                                                SWDES-631
                                                                SWDES-632
                                                                SWDES-633
                                                                SWDES-634
                                                                SWDES-635
                                                                SWDES-636
                                                                SWDES-637
                                                                SWDES-638

\note
  CSC ID             : SWDES-64
*************************************************************************/

#ifndef D_CAN_HOLT_H
#define D_CAN_HOLT_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/* Number of filters */
#define FILTER_COUNT  8u

/* Read register commands */
#define d_CAN_HOLT_COMMAND_READ_MESSAGE    0x46u
#define d_CAN_HOLT_COMMAND_READ_FILTER_0   0xA2u
#define d_CAN_HOLT_COMMAND_READ_MASK_0     0xB4u
#define d_CAN_HOLT_COMMAND_READ_CTRL0      0xD2u
#define d_CAN_HOLT_COMMAND_READ_CTRL1      0xD4u
#define d_CAN_HOLT_COMMAND_READ_BTR0       0xD6u
#define d_CAN_HOLT_COMMAND_READ_BTR1       0xD8u
#define d_CAN_HOLT_COMMAND_READ_MESSTAT    0xDAu
#define d_CAN_HOLT_COMMAND_READ_ERR        0xDCu
#define d_CAN_HOLT_COMMAND_READ_INTF       0xDEu
#define d_CAN_HOLT_COMMAND_READ_STATF      0xE2u
#define d_CAN_HOLT_COMMAND_READ_INTE       0xE4u
#define d_CAN_HOLT_COMMAND_READ_GPINE      0xE8u
#define d_CAN_HOLT_COMMAND_READ_REC        0xEAu
#define d_CAN_HOLT_COMMAND_READ_TEC        0xECu

/* Write register commands */
#define d_CAN_HOLT_COMMAND_WRITE_MESSAGE   0x12u
#define d_CAN_HOLT_COMMAND_WRITE_CTRL0     0x14u
#define d_CAN_HOLT_COMMAND_WRITE_CTRL1     0x16u
#define d_CAN_HOLT_COMMAND_WRITE_BTR0      0x18u
#define d_CAN_HOLT_COMMAND_WRITE_BTR1      0x1Au
#define d_CAN_HOLT_COMMAND_WRITE_INTE      0x1Cu
#define d_CAN_HOLT_COMMAND_WRITE_GPINE     0x22u
#define d_CAN_HOLT_COMMAND_MASTER_RESET    0x56u
#define d_CAN_HOLT_COMMAND_WRITE_FILTER_0  0x62u
#define d_CAN_HOLT_COMMAND_WRITE_MASK_0    0x74u

/* Interrupt Status/Enable Register Bit Definitions and Masks */
#define d_CAN_HOLT_RXTMP_MASK    0x80u  /* Message received in temporary receive buffer */
#define d_CAN_HOLT_RXFIFO_MASK   0x40u  /* Message received in FIFO */
#define d_CAN_HOLT_TXCPLT_MASK   0x20u  /* Successful transmission complete */
#define d_CAN_HOLT_BUSERR_MASK   0x10u  /* Bus Error */
#define d_CAN_HOLT_MCHG_MASK     0x08u  /* Mode Change bit */
#define d_CAN_HOLT_WAKEUP_MASK   0x04u  /* Wake-Up detected */
#define d_CAN_HOLT_F1MESS_MASK   0x02u  /* Filter 1 passed a valid message */
#define d_CAN_HOLT_F0MESS_MASK   0x01u  /* Filter 0 passed a valid message */

/* Error Register Bit Definitions and Masks */
#define d_CAN_HOLT_BUSOFF_MASK    0x80u  /* Bus-off status */
#define d_CAN_HOLT_TXERRP_MASK    0x40u  /* Transmit Error Passive status */
#define d_CAN_HOLT_RXERRP_MASK    0x20u  /* Receive Error Passive status */
#define d_CAN_HOLT_BITERR_MASK    0x10u  /* Bit Error */
#define d_CAN_HOLT_FRMERR_MASK    0x08u  /* Form Error */
#define d_CAN_HOLT_CRCERR_MASK    0x04u  /* CRRC Error */
#define d_CAN_HOLT_ACKERR_MASK    0x02u  /* Acknowledgement Error */
#define d_CAN_HOLT_STUFERR_MASK   0x01u  /* Stuff Error */

/***** Type Definitions *************************************************/

typedef enum
{
  d_CAN_HOLT_MODE_NORMAL = 0,
  d_CAN_HOLT_MODE_LOOPBACK,
  d_CAN_HOLT_MODE_MONITOR,
  d_CAN_HOLT_MODE_SLEEP,
  d_CAN_HOLT_MODE_INITIALISE,
  
  d_CAN_HOLT_MODE_COUNT
} d_CAN_HOLT_Mode_t;

typedef struct
{
  Bool_t extended;
  Uint32_t id;
  Uint32_t exId;
  Bool_t substituteRemoteTxRequest;
  Bool_t remoteTxRequest;
  Uint32_t dataLength;
  Uint8_t data[8];
} d_CAN_HOLT_Message_t;

typedef struct
{
  Bool_t extended;
  Uint32_t id;
  Uint32_t exId;
  Uint8_t data[2];
} d_CAN_HOLT_Filter_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Verify configuration and initialise data */
d_Status_t d_CAN_HOLT_Initialise(const Uint32_t channel);

/* Reset the interface */
d_Status_t d_CAN_HOLT_MasterReset(const Uint32_t channel);

/* Set operating mode */
d_Status_t d_CAN_HOLT_ModeSet(const Uint32_t channel, const d_CAN_HOLT_Mode_t mode);

/* Get operating mode */
d_Status_t d_CAN_HOLT_ModeGet(const Uint32_t channel, d_CAN_HOLT_Mode_t * const pMode);

/* Set filter registers */
d_Status_t d_CAN_HOLT_FilterSet(const Uint32_t channel, const Uint32_t entry, const d_CAN_HOLT_Filter_t * const pFilter, const d_CAN_HOLT_Filter_t * const pMask);

/* Get filter registers */
d_Status_t d_CAN_HOLT_FilterGet(const Uint32_t channel, const Uint32_t entry, d_CAN_HOLT_Filter_t * const pFilter, d_CAN_HOLT_Filter_t * const pMask);

/* Enable filtering */
d_Status_t d_CAN_HOLT_FilterEnable(const Uint32_t channel);

/* Disable filtering */
d_Status_t d_CAN_HOLT_FilterDisable(const Uint32_t channel);

/* Perform a CAN loopback test */
d_Status_t d_CAN_HOLT_SelfTest(const Uint32_t channel);

/* Send a message frame */
d_Status_t d_CAN_HOLT_SendMessage(const Uint32_t channel, const d_CAN_HOLT_Message_t * const pMessage);

/* Receive a message frame */
d_Status_t d_CAN_HOLT_ReceiveMessage(const Uint32_t channel, d_CAN_HOLT_Message_t * const pMessage);

/* Enable CAN Interrupt for channel */
d_Status_t d_CAN_HOLT_InterruptEnable(const Uint32_t channel, const Uint8_t mask);

/* Disable CAN Interrupt for channel */
d_Status_t d_CAN_HOLT_InterruptDisable(const Uint32_t channel, const Uint8_t mask);

/* Interrupt handler */
void d_CAN_HOLT_InterruptHandler(const Uint32_t channel);

/* Read a CAN register */
d_Status_t d_CAN_HOLT_RegisterRead(const Uint32_t channel, const Uint8_t regOffset, Uint8_t * const pValue);

/* Write a CAN register */
d_Status_t d_CAN_HOLT_RegisterWrite(const Uint32_t channel, const Uint8_t regOffset, const Uint8_t value);

#endif /* D_CAN_HOLT_H */
