/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : d_sata

  Abstract : SOC SATA controller interface

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-247
                       SDD References: 136T-2200-131000-001-D22 SWDES-542
                                                                SWDES-700
                                                                SWDES-701
                                                                SWDES-702
                                                                SWDES-703
                                                                SWDES-704
                                                                SWDES-705
                                                                SWDES-706
                                                                SWDES-707
                                                                SWDES-708
                                                                SWDES-709
                                                                SWDES-710
                                                                SWDES-2693
                                                                SWDES-2694
                                                                SWDES-2695
                                                                SWDES-2696
                                                                SWDES-2697
\note
  CSC ID             : SWDES-48
*************************************************************************/

#ifndef D_SATA_H
#define D_SATA_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef struct
{
  Char_t vendor[9u];
  Char_t product[17u];
  Char_t serial[10u];
  Char_t revision[5u];
  Uint32_t capacity; // capacity is measured in sectors
  Uint32_t sectorSize;
} d_SATA_DeviceInfo_t;

typedef struct
{
  Uint32_t powerOnHours;
  Uint32_t normalPowerOnOffCount;
  Uint32_t deviceUserCapacity;
  Int8_t   deviceTemperature;
  Uint8_t  remainingLifePercentage;
} d_SATA_SmartInfo_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

d_Status_t d_SATA_Initialise(void);

d_SATA_DeviceInfo_t d_SATA_GetDriveInfo(void);

d_Status_t d_SATA_Flush(void);

d_Status_t d_SATA_Write(const Uint32_t lba, const Uint8_t * const pBuffer, const Uint32_t writeLength);

d_Status_t d_SATA_Read(const Uint32_t lba, Uint8_t * pBuffer, const Uint32_t readLength);

d_Status_t d_SATA_GetSMARTInfo(d_SATA_SmartInfo_t * pSmartInfo);


// Function will test if the SATA drive is a Self Encrypting Drive
Bool_t d_SATA_IsSelfEncryptingDrive(void);

// Function will check if the current drive is locked and requires unlocking for access
Bool_t d_SATA_IsDriveLocked(void);

// Function checks whether the SATA drive is configured for encryption or not
Bool_t d_SATA_IsEncryptionEnabled(void);

// Function will do the initial setup of the Self encryption to enable Locking of the Self Encrypting drive
// Only needs to be called once for a device
d_Status_t d_SATA_EnableEncryption(const Char_t * const password, const Uint32_t passwordLength);

// Function will disable encryption on the Self Encrypting Drive
d_Status_t d_SATA_DisableEncryption(const Char_t * const password, const Uint32_t passwordLength);

// Function will unlock the Self Encrypting drive so data can be accessed via Read and Write functions
d_Status_t d_SATA_UnlockDrive(const Char_t * const password, const Uint32_t passwordLength);

// Function will Lock the Self Encrypting drive so data cannot be accessed
d_Status_t d_SATA_LockDrive(const Char_t * const password, const Uint32_t passwordLength);

/***** Macros ***********************************************************/
#ifdef ENABLE_SATA_DIAGNOSTIC_FUNCTION_ENTRY_LOGGING

#define d_LOG_SATA_FUNCTION_ENTRY() \
  d_LOG_FUNCTION_ENTRY()

#else

#define d_LOG_SATA_FUNCTION_ENTRY() \
  do {} while (0)

#endif

#ifdef ENABLE_SATA_DIAGNOSTIC_FUNCTION_EXIT_LOGGING

#define d_LOG_SATA_FUNCTION_EXIT(returnValue) \
  d_LOG_FUNCTION_EXIT(returnValue)

#else

#define d_LOG_SATA_FUNCTION_EXIT(returnValue) \
  do {} while (0)

#endif

#endif // D_SATA_H

