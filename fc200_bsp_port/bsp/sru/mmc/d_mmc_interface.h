/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : MMC Memory Interface Header File

  Abstract           : Wrapper for the MMC Xilinx driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-270
                                                                SWREQ-271
                                                                SWREQ-272
                                                                SWREQ-273
                       SDD References: 136T-2200-131100-001-D22 SWDES-254
                                                                SWDES-255
                                                                SWDES-256
                                                                SWDES-257
                                                                SWDES-258
\note
  CSC ID             : SWDES-61
*************************************************************************/

#ifndef D_MMC_INTERFACE_H
#define D_MMC_INTERFACE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef struct
{
  Uint16_t DeviceId;         /**< Unique ID  of device */
  Uint32_t BaseAddress;      /**< Base address of the device */
  Uint32_t InputClockHz;     /**< Input clock frequency */
  Uint32_t CardDetect;       /**< Card Detect */
  Uint32_t WriteProtect;     /**< Write Protect */
  Uint32_t BusWidth;         /**< Bus Width */
  Uint32_t BankNumber;       /**< MIO Bank selection for SD */
  Uint32_t HasEMIO;          /**< If SD is connected to EMIO */
  Uint8_t IsCacheCoherent;   /**< If SD is Cache Coherent or not */
} d_MMC_Config_t;

typedef struct
{
  d_MMC_Config_t Config;     /**< Configuration structure */
  Uint32_t IsReady;          /**< Device is initialized and ready */
  Uint32_t Host_Caps;        /**< Capabilities of host controller */
  Uint32_t Host_CapsExt;     /**< Extended Capabilities */
  Uint32_t HCS;              /**< High capacity support in card */
  Uint8_t  CardType;         /**< Type of card - SD/MMC/eMMC */
  Uint8_t  Card_Version;     /**< Card version */
  Uint8_t  HC_Version;       /**< Host controller version */
  Uint8_t  BusWidth;         /**< Current operating bus width */
  Uint32_t BusSpeed;         /**< Current operating bus speed */
  Uint8_t  Switch1v8;        /**< 1.8V Switch support */
  Uint32_t CardID[4];        /**< Card ID Register */
  Uint32_t RelCardAddr;      /**< Relative Card Address */
  Uint32_t CardSpecData[4];  /**< Card Specific Data Register */
  Uint32_t SectorCount;      /**< Sector Count */
  Uint32_t SdCardConfig;     /**< Sd Card Configuration Register */
  Uint32_t Mode;             /**< Bus Speed Mode */
  Uint32_t OTapDelay;        /**< Output Tap Delay */
  Uint32_t ITapDelay;        /**< Input Tap Delay */
  Uint64_t Dma64BitAddr;     /**< 64 Bit DMA Address */
  Uint16_t TransferMode;     /**< Transfer Mode */
  Uint32_t SlcrBaseAddr;     /**< SLCR base address*/
  Uint8_t  IsBusy;           /**< Busy Flag*/
  Uint32_t BlkSize;          /**< Block Size*/
} d_MMC_Instance_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the MMC interface. */
d_Status_t d_MMC_Initialise(d_MMC_Instance_t ** ppMmcInstance);

/* Get the number of sectors */
d_Status_t d_MMC_SectorCount(Uint32_t * const pSectors);

/* Perform BIT on a sector */
d_Status_t d_MMC_Bit(const Uint32_t sector);

/* Read one or more sectors */
d_Status_t d_MMC_SectorRead(const Uint32_t sector, const Uint32_t count, Uint8_t * const pBuffer);

/* Write one or more sectors */
d_Status_t d_MMC_SectorWrite(const Uint32_t sector, const Uint32_t count, const Uint8_t * const pBuffer);

/* Erase one or more sectors */
d_Status_t d_MMC_Erase(const Uint32_t sectorStart, const Uint32_t sectorEnd);

#endif /* D_MMC_INTERFACE_H */
