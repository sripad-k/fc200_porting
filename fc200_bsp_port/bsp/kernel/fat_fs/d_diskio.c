/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_diskio

  Abstract           : Kernel FAT FS disk interface

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

/***** Includes *********************************************************/

#include "kernel/fat_fs/d_diskio.h"
#include "d_ff.h"

#include "sleep.h"

#include "soc/sata/d_sata.h"

/***** Constants ********************************************************/

#define XSDPS_NUM_INSTANCES  2

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

static DSTATUS Stat[XSDPS_NUM_INSTANCES] = {STA_NOINIT, STA_NOINIT};  /* Disk status */

static d_SATA_DeviceInfo_t driveInfo;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*-----------------------------------------------------------------------*/
/* Get Disk Status              */
/*-----------------------------------------------------------------------*/

/*****************************************************************************/
/**
*
* Gets the status of the disk.
* In case of SD, it checks whether card is present or not.
*
* @param  pdrv - Drive number
*
* @return
*    0    Status ok
*    STA_NOINIT  Drive not initialized
*    STA_NODISK  No medium in the drive
*    STA_PROTECT  Write protected
*
* @note    In case Card detect signal is not connected,
*    this function will not be able to check if card is present.
*
******************************************************************************/
DSTATUS d_FAT_DiskStatus (
    Uint8_t pdrv  /* Drive number (0) */
)
{
  DSTATUS s = Stat[pdrv];

  return s;
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive             */
/*-----------------------------------------------------------------------*/
/*****************************************************************************/
/**
*
* Initializes the drive.
* In case of SD, it initializes the host controller and the card.
* This function also selects additional settings such as bus width,
* speed and block size.
*
* @param  pdrv - Drive number
*
* @return  s - which contains an OR of the following information
*    STA_NODISK  Disk is not present
*    STA_NOINIT  Drive not initialized
*    STA_PROTECT  Drive is write protected
*    0 or only STA_PROTECT both indicate successful initialization.
*
* @note
*
******************************************************************************/
DSTATUS
d_FAT_DiskInitialize
(
    Uint8_t pdrv  /* Physical drive number (0) */
)
{
  DSTATUS s;
  DSTATUS retval;

  s = d_FAT_DiskStatus(pdrv);
  if (((s & STA_NODISK) != 0U) || ((s & STA_NOINIT) == 0U))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    retval = s;
  }
  else
  {
    d_Status_t status = d_SATA_Initialise();
    if (status != d_STATUS_SUCCESS)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      retval = s | STA_NOINIT;
    }
    else
    {
      s &= (~STA_NOINIT);
      Stat[pdrv] = s;
      retval = s;

      driveInfo = d_SATA_GetDriveInfo();    
    }
  }
  
  return retval;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)               */
/*-----------------------------------------------------------------------*/
/*****************************************************************************/
/**
*
* Reads the drive
* In case of SD, it reads the SD card using ADMA2 in polled mode.
*
* @param  pdrv - Drive number
* @param  *buff - Pointer to the data buffer to store read data
* @param  sector - Start sector number
* @param  count - Sector count
*
* @return
*    RES_OK    Read successful
*    STA_NOINIT  Drive not initialized
*    RES_ERROR  Read not successful
*
* @note
*
******************************************************************************/
DRESULT d_FAT_DiskRead (
    Uint8_t pdrv,  /* Physical drive number (0) */
    Uint8_t *buff,  /* Pointer to the data buffer to store read data */
    Uint32_t sector,  /* Start sector number (LBA) */
    Uint32_t count  /* Sector count (1..128) */
)
{
  DSTATUS s;
  DRESULT retval = RES_OK;
  
  s = d_FAT_DiskStatus(pdrv);

  if ((s & STA_NOINIT) != 0U) 
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    retval = RES_NOTRDY;
  }
  else if (count == 0U) 
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    retval = RES_PARERR;
  }


  else
  {
    d_Status_t status = d_SATA_Read(sector, buff, count * driveInfo.sectorSize);
    if (status != d_STATUS_SUCCESS)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      retval = RES_ERROR;
    }
  }
  
  return retval;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions            */
/*-----------------------------------------------------------------------*/

DRESULT d_FAT_DiscIoctl (
  Uint8_t pdrv,        /* Physical drive number (0) */
  Uint8_t cmd,        /* Control code */
  void *buff        /* Buffer to send/receive control data */
)
{
  UNUSED_PARAMETER(pdrv);  /* Retained for consistency of ioctl function */
  
  DRESULT res = RES_ERROR;


  switch (cmd)
  {
    case (Uint8_t)CTRL_SYNC :  /* Make sure that no pending write process */
      (void)d_SATA_Flush();
      res = RES_OK;
      break;

    case (Uint8_t)GET_SECTOR_COUNT : /* Get number of sectors on the disk (DWORD) */
      // cppcheck-suppress misra-c2012-11.5; The pointer to void definition is from a Xilinx generated header file. It is intended to be cast to something else.
      *(Uint32_t *)buff = driveInfo.capacity;
      res = RES_OK;
      break;

    case (Uint8_t)GET_SECTOR_SIZE:
    case (Uint8_t)GET_BLOCK_SIZE :  /* Get erase block size in unit of sector (DWORD) */
      // cppcheck-suppress misra-c2012-11.5; The pointer to void definition is from a Xilinx generated header file. It is intended to be cast to something else.
      *(Uint32_t *)buff = driveInfo.sectorSize;
      res = RES_OK;
      break;
      
    default:
      res = RES_PARERR;
      break;
  }

  return res;
}

/******************************************************************************/
/**
*
* This function is User Provided Timer Function for FatFs module
*
* @return  Uint32_t
*
* @note    None
*
****************************************************************************/

Uint32_t d_FAT_Gettime (void)
{
  return  ((Uint32_t)30u << 25u)  /* Fixed to Jan. 1, 2010. Epoch is 1980 */
    | ((Uint32_t)1u << 21u) /* Month, 1 - 12 */
    | ((Uint32_t)1u << 16u) /* Day, 1 - 31 */
    | ((Uint32_t)0u << 11u) /* Hour */
    | ((Uint32_t)0u << 5u)  /* Minute */
    | ((Uint32_t)0u);       /* Second */
}

/*****************************************************************************/
/**
*
* Reads the drive
* In case of SD, it reads the SD card using ADMA2 in polled mode.
*
* @param  pdrv - Drive number
* @param  *buff - Pointer to the data to be written
* @param  sector - Sector address
* @param  count - Sector count
*
* @return
*    RES_OK    Read successful
*    STA_NOINIT  Drive not initialized
*    RES_ERROR  Read not successful
*
* @note
*
******************************************************************************/
DRESULT d_FAT_DiskWrite (
  Uint8_t pdrv,      /* Physical drive nmuber (0..) */
  const Uint8_t * const buff,  /* Data to be written */
  Uint32_t sector,    /* Sector address (LBA) */
  Uint32_t count      /* Number of sectors to write (1..128) */
)
{
  DSTATUS s;
  DRESULT retval = RES_OK;
  
  s = d_FAT_DiskStatus(pdrv);
  if ((s & STA_NOINIT) != 0U) 
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    retval = RES_NOTRDY;
  }
  
  else if (count == 0U) 
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    retval = RES_PARERR;
  }

  else
  {
    d_Status_t status = d_SATA_Write(sector, buff, count * driveInfo.sectorSize);
    if (status != d_STATUS_SUCCESS)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      retval = RES_ERROR;
    }
  }
  
  return retval;
}
