/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_can.h

  Abstract           : CAN driver module

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

#ifndef CAN_H
#define CAN_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"
#include "soc/can/d_can_hw.h"
#include "soc/can/d_can_cfg.h"

/***** Constants ********************************************************/

/***** Macros ***********************************************************/

/****************************************************************************/
/**
*
* This macro calculates CAN message identifier value given identifier field
* values.
*
* @param  StandardId contains Standard Message ID value.
* @param  SubRemoteTransReq contains Substitute Remote Transmission
*    Request value.
* @param  IdExtension contains Identifier Extension value.
* @param  ExtendedId contains Extended Message ID value.
* @param  RemoteTransReq contains Remote Transmission Request value.
*
* @return  Message Identifier value.
*
* @note    C-Style signature:
*    u32 d_CAN_CreateIdValue(u32 StandardId,
*          u32 SubRemoteTransReq,
*          u32 IdExtension, u32 ExtendedId,
*          u32 RemoteTransReq)
*
*    Read the CAN specification for meaning of each parameter.
*
*****************************************************************************/
#define d_CAN_CreateIdValue(StandardId, SubRemoteTransReq, IdExtension, \
    ExtendedId, RemoteTransReq)         \
 ((((StandardId) << d_CAN_IDR_ID1_SHIFT) & d_CAN_IDR_ID1_MASK) |  \
 (((SubRemoteTransReq) << d_CAN_IDR_SRR_SHIFT) & d_CAN_IDR_SRR_MASK)|\
 (((IdExtension) << d_CAN_IDR_IDE_SHIFT) & d_CAN_IDR_IDE_MASK) |  \
 (((ExtendedId) << d_CAN_IDR_ID2_SHIFT) & d_CAN_IDR_ID2_MASK) |  \
 ((RemoteTransReq) & d_CAN_IDR_RTR_MASK))

/****************************************************************************/
/**
*
* This macro calculates value for Data Length Code register given Data
* Length Code value.
*
* @param  DataLengCode indicates Data Length Code value.
*
* @return  Value that can be assigned to Data Length Code register.
*
* @note    C-Style signature:
*    u32 d_CAN_CreateDlcValue(u32 DataLengCode)
*
*    Read the CAN specification for meaning of Data Length Code.
*
*****************************************************************************/
#define d_CAN_CreateDlcValue(DataLengCode) \
  (((DataLengCode) << d_CAN_DLCR_DLC_SHIFT) & d_CAN_DLCR_DLC_MASK)

/***** Type Definitions *************************************************/

typedef enum
{
  d_CAN_MODE_UNKNOWN = 0,
  d_CAN_MODE_CONFIG,
  d_CAN_MODE_NORMAL,
  d_CAN_MODE_LOOPBACK,
  d_CAN_MODE_SLEEP,
  d_CAN_MODE_SNOOP,
  
  d_CAN_MODE_COUNT
} d_CAN_Mode_t;

typedef struct
{
  Bool_t extended;
  Uint32_t id;
  Uint32_t exId;
  Bool_t substituteRemoteTxRequest;
  Bool_t remoteTxRequest;
  Uint32_t dataLength;
  Uint8_t data[8];
} d_CAN_Message_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise a CAN channel */
d_Status_t d_CAN_Initialise(const Uint32_t channel);

/* Set operating mode */
d_Status_t d_CAN_ModeSet(const Uint32_t channel, const d_CAN_Mode_t mode);

/* Get operating mode */
d_Status_t d_CAN_ModeGet(const Uint32_t channel, d_CAN_Mode_t * const pMode);

/* Perform a software reset */
d_Status_t d_CAN_SoftwareReset(const Uint32_t channel);

/* Perform a CAN loopback test */
d_Status_t d_CAN_SelfTest(const Uint32_t channel);

/* Send a message frame */
d_Status_t d_CAN_SendMessage(const Uint32_t channel, const d_CAN_Message_t * const pMessage);

/* Receive a message frame */
d_Status_t d_CAN_ReceiveMessage(const Uint32_t channel, d_CAN_Message_t * const pMessage);

/* Enable CAN Interrupt for channel */
d_Status_t d_CAN_InterruptEnable(const Uint32_t channel, const Uint32_t mask);

/* Disable CAN Interrupt for channel */
d_Status_t d_CAN_InterruptDisable(const Uint32_t channel, const Uint32_t mask);

/* Interrupt handlers */
void d_CAN_InterruptHandler(const Uint32_t channel);

/* Read a CAN register */
d_Status_t d_CAN_RegisterRead(const Uint32_t channel, const Uint32_t regOffset, Uint32_t * const pValue);

/* Write a CAN register */
d_Status_t d_CAN_RegisterWrite(const Uint32_t channel, const Uint32_t regOffset, const Uint32_t value);

#endif /* CAN_H */
