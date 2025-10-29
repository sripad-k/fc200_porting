/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : d_diskio

  Abstract : Kernel FAT FS disk interface

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-230
                       SDD References: 136T-2200-131000-001-D22 SWDES-556
                                                                SWDES-682
                                                                SWDES-683
                                                                SWDES-684
                                                                SWDES-685
                                                                SWDES-686
                                                                SWDES-687
                                                                SWDES-688
                                                                SWDES-689
                                                                SWDES-690
                                                                SWDES-691
                                                                SWDES-692
                                                                SWDES-693
                                                                SWDES-694
                                                                SWDES-695
                                                                SWDES-696
                                                                SWDES-697
                                                                SWDES-698
                                                                SWDES-699
\note
  CSC ID             : SWDES-76
*************************************************************************/

#ifndef D_DISKIO_H
#define D_DISKIO_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Constants ********************************************************/

#define USE_WRITE	1	/* 1: Enable disk_write function */
#define USE_IOCTL	1	/* 1: Enable disk_ioctl function */

/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01U	/* Drive not initialized */
#define STA_NODISK		0x02U	/* No medium in the drive */
#define STA_PROTECT		0x04U	/* Write protected */


/* Command code for disk_ioctrl function */

/* Generic command (used by FatFs) */
#define CTRL_SYNC			0U	/* Flush disk cache (for write functions) */
#define GET_SECTOR_COUNT	1U	/* Get media size (for only f_mkfs()) */
#define GET_SECTOR_SIZE		2U	/* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE		3U	/* Get erase block size (for only f_mkfs()) */
#define CTRL_TRIM			4U	/* Inform device that the data on the block of sectors is no longer used (needed at FF_USE_TRIM == 1) */

/* Generic command (not used by FatFs) */
#define CTRL_POWER			5U	/* Get/Set power status */
#define CTRL_LOCK			6U	/* Lock/Unlock media removal */
#define CTRL_EJECT			7U	/* Eject media */
#define CTRL_FORMAT			8U	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10U	/* Get card type */
#define MMC_GET_CSD			11U	/* Get CSD */
#define MMC_GET_CID			12U	/* Get CID */
#define MMC_GET_OCR			13U	/* Get OCR */
#define MMC_GET_SDSTAT		14U	/* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20U	/* Get F/W revision */
#define ATA_GET_MODEL		21U	/* Get model name */
#define ATA_GET_SN			22U	/* Get serial number */

/***** Type Definitions *************************************************/

/* Status of Disk Functions */
typedef Uint8_t	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Prototypes for disk control functions */

DSTATUS d_FAT_DiskInitialize (Uint8_t pdrv);
DSTATUS d_FAT_DiskStatus (Uint8_t pdrv);
DRESULT d_FAT_DiskRead (Uint8_t pdrv, Uint8_t* buff, Uint32_t sector, Uint32_t count);
DRESULT d_FAT_DiskWrite (Uint8_t pdrv, const Uint8_t* const buff, Uint32_t sector, Uint32_t count);
DRESULT d_FAT_DiscIoctl (Uint8_t pdrv, Uint8_t cmd, void* buff);

#endif /* D_DISKIO_H */
