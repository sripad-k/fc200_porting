/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_flash_mac.h

  Abstract           : Flash MAC device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-177
                                                                SWREQ-178
                                                                SWREQ-179
                                                                SWREQ-180
                                                                SWREQ-214
                       SDD References: 136T-2200-131100-001-D22 SWDES-295
                                                                SWDES-615
                                                                SWDES-616
                                                                SWDES-617
                                                                SWDES-618
                                                                SWDES-619
                                                                SWDES-620
                                                                SWDES-621
                                                                SWDES-622
                                                                SWDES-623
                                                                SWDES-624
                       
\note
  CSC ID             : SWDES-59
*************************************************************************/

#ifndef D_FLASH_MAC_H
#define D_FLASH_MAC_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

#define d_FLASH_MAC_SIZE         ((Uint32_t)2u * 1024u * 1024u)
#define d_FLASH_MAC_SECTOR_SIZE  ((Uint32_t)4u * 1024u)
#define d_FLASH_MAC_PAGE_SIZE    (256u)

#define d_FLASH_MAC_PROTECTION_SIZE (6u)

/***** Type Definitions *************************************************/

/* MAC address type */
typedef Uint8_t d_FLASH_MAC_Protection_t[d_FLASH_MAC_PROTECTION_SIZE];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialises the FLASH MAC CSC */
d_Status_t d_FLASH_MAC_Initialise(void);

/* Unlock the FLASH MAC device */
d_Status_t d_FLASH_MAC_Unlock(const Uint32_t device);

/* Read up to 256 bytes from the FLASH MAC device */
d_Status_t d_FLASH_MAC_Read(const Uint32_t device,
                            const Uint32_t addressToRead,
                            const Uint32_t bytesToRead,
                            Uint8_t * const pDataBuffer,
                            const Uint32_t bufferSizeInBytes);

/* Write data to the FLASH MAC device */
d_Status_t d_FLASH_MAC_Write(const Uint32_t device,
                             const Uint32_t addressToWrite,
                             const Uint32_t bytesToWrite,
                             const Uint8_t * const pDataBuffer,
                             const Uint32_t bufferSizeInBytes);

/* Erases a 4k sector of the FLASH MAC device */
d_Status_t d_FLASH_MAC_EraseSector(const Uint32_t device,
                                   const Uint32_t sectorIndex);

/* Erases the entire FLASH MAC device */
d_Status_t d_FLASH_MAC_EraseDevice(const Uint32_t device);

/* Read the MAC address */
d_Status_t d_FLASH_MAC_MacAddress(const Uint32_t device, d_MacAddress_t macAddress);

/* Read the device status register */
d_Status_t d_FLASH_MAC_StatusRead(const Uint32_t device, Uint8_t * const pStatus);

/* Read the device config register */
d_Status_t d_FLASH_MAC_ConfigRead(const Uint32_t device, Uint8_t * const pConfig);

/* Write the device config register */
d_Status_t d_FLASH_MAC_ConfigWrite(const Uint32_t device, const Uint8_t config);

/* Read the block protection register */
d_Status_t d_FLASH_MAC_BlockProtectionRead(const Uint32_t device, d_FLASH_MAC_Protection_t protect);

/* Write the block protection register */
d_Status_t d_FLASH_MAC_BlockProtectionWrite(const Uint32_t device, const d_FLASH_MAC_Protection_t protect);

/* Read the device status register to determine if it is busy */
d_Status_t d_FLASH_MAC_CheckFlashNotBusy(const Uint32_t device);

/* Read a byte from the Serial Flash Discoverable Parameters */
d_Status_t d_FLASH_MAC_SfdpByte(const Uint32_t device,
                                const Uint32_t address,
                                Uint8_t * const pValue);

/* Read a word from the Serial Flash Discoverable Parameters */
d_Status_t d_FLASH_MAC_SfdpWord(const Uint32_t device,
                                const Uint32_t address,
                                Uint32_t * const pValue);

#endif /* D_FLASH_MAC_H */
