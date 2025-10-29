/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : d_sata_encryption

  Abstract : SOC SATA driver self encrypting drive support

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-762
                       SDD References: 136T-2200-131000-001-D22 SWDES-2698
\note
  CSC ID             : SWDES-48
*************************************************************************/

#ifndef D_SATA_ENCRYPTION_H
#define D_SATA_ENCRYPTION_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum 
{
  // users
  OPAL_SMUID_UID,
  OPAL_THISSP_UID,
  OPAL_ADMINSP_UID,
  OPAL_LOCKINGSP_UID,
	ENTERPRISE_LOCKINGSP_UID,
  OPAL_ANYBODY_UID,
  OPAL_SID_UID,
  OPAL_ADMINS_UID,
  OPAL_ADMIN1_UID,
  OPAL_USER1_UID,
  OPAL_USER2_UID,
  OPAL_PSID_UID,
	ENTERPRISE_BANDMASTER0_UID,
	ENTERPRISE_ERASEMASTER_UID,
  // tables
  OPAL_LOCKINGRANGE_GLOBAL,
  OPAL_LOCKINGRANGE_ACE_RDLOCKED,
  OPAL_LOCKINGRANGE_ACE_WRLOCKED,
	OPAL_LOCKINGRANGE_GLOBAL_ACE_RDLOCKED,
	OPAL_LOCKINGRANGE_GLOBAL_ACE_WRLOCKED,
	OPAL_MBRControl_Set_DoneToDOR,
  OPAL_MBRCONTROL,
  OPAL_MBR,
  OPAL_AUTHORITY_TABLE,
  OPAL_C_PIN_TABLE,
	OPAL_LOCKING_INFO_TABLE,
	ENTERPRISE_LOCKING_INFO_TABLE,
  //C_PIN_TABLE object ID's
  OPAL_C_PIN_MSID,
  OPAL_C_PIN_SID,
  OPAL_C_PIN_ADMIN1,
  //half UID's (only first 4 bytes used)
  OPAL_HALF_UID_AUTHORITY_OBJ_REF,
  OPAL_HALF_UID_BOOLEAN_ACE,
  // omitted optional parameter
  OPAL_UID_HEXFF,
} OPAL_UID;

typedef struct
{
  OPAL_UID secProvider;
  OPAL_UID signAuth;
} d_SATAENC_uidPair_t;

typedef struct 
{
  Bool_t lockingSupported;
  Bool_t lockingEnabled;
  Bool_t locked;
} d_SATAENC_SelfEncryptionStatus_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

Bool_t d_SATAENC_IsOperationComplete(const Uint8_t * pBuffer, const Uint32_t bufferSize);

Uint32_t d_SATAENC_BuildSessionStartPayload(Uint8_t * pBuffer, const Uint32_t bufferSize, const Char_t * const password, const Uint32_t pwdLength, const d_SATAENC_uidPair_t uids);

Uint32_t d_SATAENC_BuildSessionTerminatePayload(Uint8_t * pBuffer, const Uint32_t bufferSize);

Uint32_t d_SATAENC_BuildSetTablePayload(Uint8_t * pBuffer, const Uint32_t bufferSize, const Char_t * const password, const Uint32_t pwdLength);

Uint32_t d_SATAENC_BuildActivateLockingSPPayload(Uint8_t * pBuffer, const Uint32_t bufferSize);

Uint32_t d_SATAENC_BuildLockingRangePayload(Uint8_t * pBuffer, const Uint32_t bufferSize, Bool_t lock);

Uint32_t d_SATAENC_BuildSetLockRangePayload(Uint8_t * pBuffer, const Uint32_t bufferSize, Bool_t lock);

Uint32_t d_SATAENC_BuildRevertLockingSPPayload(Uint8_t * pBuffer, const Uint32_t bufferSize);

Uint32_t d_SATAENC_GetTokenFromResponse(const Uint8_t * pBuffer, const Uint32_t bufferSize, const Uint32_t tokenNumber);

void d_SATAENC_UpdateSessionNumbersFromResponse(const Uint8_t * pBuffer, const Uint32_t bufferSize);


#endif //D_SATA_ENCRYPTION_H
