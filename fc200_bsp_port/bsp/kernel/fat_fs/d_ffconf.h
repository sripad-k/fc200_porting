/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : d_ffconf.h

  Abstract : Kernel FAT FS Configuration file

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

#ifndef D_FFCONF_H
#define D_FFCONF_H

/***** Includes *********************************************************/

/***** Constants ********************************************************/

#define 	d_FILE_SYSTEM_USE_STRFUNC 2 /* Enables f_puts() and f_gets() */
#define  	FILE_SYSTEM_NUM_LOGIC_VOL 2
#define  	FILE_SYSTEM_USE_MKFS
#define   FILE_SYSTEM_SET_FS_RPATH 0

#define d_FAT_FS_INTERFACE_SATA 1

#ifdef FILE_SYSTEM_READ_ONLY
#define FF_FS_READONLY	1	/* 1:Read only */
#else
#define FF_FS_READONLY	0	/* 0:Read/Write */
#endif
/* This option switches read-only configuration. (0:Read/Write or 1:Read-only)
/  Read-only configuration removes writing API functions, d_FAT_Write(), d_FAT_Sync(),
/  d_FAT_UnLink(), d_FAT_MkDir(), d_FAT_Chmod(), d_FAT_Rename(), d_FAT_Truncate(), d_FAT_GetFree()
/  and optional writing functions as well. */


#define FF_FS_MINIMIZE	0
/* This option defines minimization level to remove some basic API functions.
/
/   0: Basic functions are fully enabled.
/   1: d_FAT_Stat(), d_FAT_GetFree(), d_FAT_UnLink(), d_FAT_MkDir(), d_FAT_Truncate() and d_FAT_Rename()
/      are removed.
/   2: d_FAT_Opendir(), d_FAT_ReadDir() and d_FAT_CloseDir() are removed in addition to 1.
/   3: d_FAT_LSeek() function is removed in addition to 2. */


#if d_FILE_SYSTEM_USE_STRFUNC == 0
#define	d_FAT_USE_STRFUNC	0	/* 0:Disable */
#elif d_FILE_SYSTEM_USE_STRFUNC == 1
#define	d_FAT_USE_STRFUNC	1	/* 1:Enable */
#elif d_FILE_SYSTEM_USE_STRFUNC == 2
#define	d_FAT_USE_STRFUNC	2	/* 2:Enable */
#endif
/* This option switches string functions, d_FAT_Gets(), d_FAT_Putc(), d_FAT_Puts() and d_FAT_Printf().
/
/  0: Disable string functions.
/  1: Enable without LF-CRLF conversion.
/  2: Enable with LF-CRLF conversion. */


#define FF_USE_FIND		1
/* This option switches filtered directory read functions, d_FAT_FindFirst() and
/  d_FAT_FindNext(). (0:Disable, 1:Enable 2:Enable with matching altname[] too) */

#ifdef FILE_SYSTEM_USE_MKFS
#define	FF_USE_MKFS		1	/* 1:Enable */
#else
#define	FF_USE_MKFS		0	/* 0:Disable */
#endif
/* This option switches d_FAT_Mkfs() function. (0:Disable or 1:Enable) */


#define d_FAT_USE_FASTSEEK	1
/* This option switches fast seek function. (0:Disable or 1:Enable) */


#define FF_USE_EXPAND	0
/* This option switches d_FAT_Expand function. (0:Disable or 1:Enable) */


#ifdef FILE_SYSTEM_USE_CHMOD
#define FF_USE_CHMOD	1	/* 1:Enable */
#else
#define FF_USE_CHMOD	0	/* 0:Disable */
#endif
/* This option switches attribute manipulation functions, d_FAT_Chmod() and d_FAT_Utime().
/  (0:Disable or 1:Enable) Also FF_FS_READONLY needs to be 0 to enable this option. */


#define FF_USE_LABEL	0
/* This option switches volume label functions, d_FAT_GetLabel() and d_FAT_SetLabel().
/  (0:Disable or 1:Enable) */


#define FF_USE_FORWARD	0
/* This option switches d_FAT_Forward() function. (0:Disable or 1:Enable) */


/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations
/---------------------------------------------------------------------------*/

#define d_FAT_CODE_PAGE	850u
/* This option specifies the OEM code page to be used on the target system.
/   850 - Latin 1 */

#ifdef FILE_SYSTEM_USE_LFN
#define	FF_USE_LFN	FILE_SYSTEM_USE_LFN	/* 0 to 3 */
#else
#define	FF_USE_LFN	3		/* 0 to 3 */
#endif
#define FF_MAX_LFN		255
/* The FF_USE_LFN switches the support for LFN (long file name).
/
/   0: Disable LFN. FF_MAX_LFN has no effect.
/   1: Enable LFN with static working buffer on the BSS. Always NOT thread-safe.
/   2: Enable LFN with dynamic working buffer on the STACK.
/   3: Enable LFN with dynamic working buffer on the HEAP.
/
/  To enable the LFN, ffunicode.c needs to be added to the project. The LFN function
/  requiers certain internal working buffer occupies (FF_MAX_LFN + 1) * 2 bytes and
/  additional (FF_MAX_LFN + 44) / 15 * 32 bytes when exFAT is enabled.
/  The FF_MAX_LFN defines size of the working buffer in UTF-16 code unit and it can
/  be in range of 12 to 255. It is recommended to be set 255 to fully support LFN
/  specification.
/  When use stack for the working buffer, take care on stack overflow. When use heap
/  memory for the working buffer, memory management functions, ff_memalloc() and
/  ff_memfree() in ffsystem.c, need to be added to the project. */


#define FF_LFN_UNICODE	0
/* This option switches the character encoding on the API when LFN is enabled.
/
/   0: ANSI/OEM in current CP (TCHAR = char)
/   1: Unicode in UTF-16 (TCHAR = WCHAR)
/   2: Unicode in UTF-8 (TCHAR = char)
/   3: Unicode in UTF-32 (TCHAR = DWORD)
/
/  Also behavior of string I/O functions will be affected by this option.
/  When LFN is not enabled, this option has no effect. */


#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
/* This set of options defines size of file name members in the FILINFO structure
/  which is used to read out directory items. These values should be suffcient for
/  the file names to read. The maximum possible length of the read file name depends
/  on character encoding. When LFN is not enabled, these options have no effect. */


#define FF_STRF_ENCODE	3
/* When FF_LFN_UNICODE >= 1 with LFN enabled, string I/O functions, d_FAT_Gets(),
/  d_FAT_Putc(), d_FAT_Puts and d_FAT_Printf() convert the character encoding in it.
/  This option selects assumption of character encoding ON THE FILE to be
/  read/written via those functions.
/
/   0: ANSI/OEM in current CP
/   1: Unicode in UTF-16LE
/   2: Unicode in UTF-16BE
/   3: Unicode in UTF-8
*/


#if FILE_SYSTEM_SET_FS_RPATH == 0
#define FF_FS_RPATH		0U
#elif FILE_SYSTEM_SET_FS_RPATH == 1
#define FF_FS_RPATH		1U
#elif FILE_SYSTEM_SET_FS_RPATH == 2
#define FF_FS_RPATH		2U
#endif
/* This option configures support for relative path.
/
/   0: Disable relative path and remove related functions.
/   1: Enable relative path. d_FAT_ChDir() and d_FAT_ChDrive() are available.
/   2: d_FAT_GetCwd() function is available in addition to 1.
*/


/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations
/---------------------------------------------------------------------------*/

#if FILE_SYSTEM_NUM_LOGIC_VOL == 1
#define FF_VOLUMES 1U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 2
#define FF_VOLUMES 2U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 3
#define FF_VOLUMES 3U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 4
#define FF_VOLUMES 4U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 5
#define FF_VOLUMES 5U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 6
#define FF_VOLUMES 6U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 7
#define FF_VOLUMES 7U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 8
#define FF_VOLUMES 8U
#elif FILE_SYSTEM_NUM_LOGIC_VOL == 9
#define FF_VOLUMES 9U
#else
#define FF_VOLUMES 10U
#endif
/* Number of volumes (logical drives) to be used. (1-10) */


#define FF_STR_VOLUME_ID	0
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
/* FF_STR_VOLUME_ID switches support for volume ID in arbitrary strings.
/  When FF_STR_VOLUME_ID is set to 1 or 2, arbitrary strings can be used as drive
/  number in the path name. FF_VOLUME_STRS defines the volume ID strings for each
/  logical drives. Number of items must not be less than FF_VOLUMES. Valid
/  characters for the volume ID strings are A-Z, a-z and 0-9, however, they are
/  compared in case-insensitive. If FF_STR_VOLUME_ID >= 1 and FF_VOLUME_STRS is
/  not defined, a user defined volume string table needs to be defined as:
/
/  const char* VolumeStr[FF_VOLUMES] = {"ram","flash","sd","usb",...
*/


#define	FF_MULTI_PARTITION	1	/* 1:Enable multiple partition */
/* This option switches support for multiple volumes on the physical drive.
/  By default (0), each logical drive number is bound to the same physical drive
/  number and only an FAT volume found on the physical drive will be mounted.
/  When this function is enabled (1), each logical drive number can be bound to
/  arbitrary physical drive and partition listed in the VolToPart[]. Also d_FAT_FDisk()
/  function will be available. */


#define FF_MIN_SS		512
#define FF_MAX_SS		512
/* This set of options configures the range of sector size to be supported. (512,
/  1024, 2048 or 4096) Always set both 512 for most systems, generic memory card and
/  harddisk. But a larger value may be required for on-board flash memory and some
/  type of optical media. When FF_MAX_SS is larger than FF_MIN_SS, FatFs is configured
/  for variable sector size mode and disk_ioctl() function needs to implement
/  GET_SECTOR_SIZE command. */


#ifdef FILE_SYSTEM_USE_TRIM
#define FF_USE_TRIM	1
#else
#define FF_USE_TRIM	0
#endif
/* This option switches support for ATA-TRIM. (0:Disable or 1:Enable)
/  To enable Trim function, also CTRL_TRIM command should be implemented to the
/  disk_ioctl() function. */


#define FF_FS_NOFSINFO	0
/* If you need to know correct free space on the FAT32 volume, set bit 0 of this
/  option, and d_FAT_GetFree() function at first time after volume mount will force
/  a full FAT scan. Bit 1 controls the use of last allocated cluster number.
/
/  bit0=0: Use free cluster count in the FSINFO if available.
/  bit0=1: Do not trust free cluster count in the FSINFO.
/  bit1=0: Use last allocated cluster number in the FSINFO if available.
/  bit1=1: Do not trust last allocated cluster number in the FSINFO.
*/

/*---------------------------------------------------------------------------/
/ System Configurations
/---------------------------------------------------------------------------*/

#define FF_FS_TINY		0
/* This option switches tiny buffer configuration. (0:Normal or 1:Tiny)
/  At the tiny configuration, size of file object (FIL) is shrunk FF_MAX_SS bytes.
/  Instead of private sector buffer eliminated from the file object, common sector
/  buffer in the filesystem object (FATFS) is used for the file data transfer. */


#ifdef FILE_SYSTEM_FS_EXFAT
#define FF_FS_EXFAT		1
#else
#define FF_FS_EXFAT		0
#endif
/* This option switches support for exFAT filesystem. (0:Disable or 1:Enable)
/  To enable exFAT, also LFN needs to be enabled.
/  Note that enabling exFAT discards ANSI C (C89) compatibility. */


#define FF_FS_NORTC		0
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2018
/* The option FF_FS_NORTC switches timestamp functiton. If the system does not have
/  any RTC function or valid timestamp is not needed, set FF_FS_NORTC = 1 to disable
/  the timestamp function. Every object modified by FatFs will have a fixed timestamp
/  defined by FF_NORTC_MON, FF_NORTC_MDAY and FF_NORTC_YEAR in local time.
/  To enable timestamp function (FF_FS_NORTC = 0), d_FAT_Gettime() function need to be
/  added to the project to read current time form real-time clock. FF_NORTC_MON,
/  FF_NORTC_MDAY and FF_NORTC_YEAR have no effect.
/  These options have no effect at read-only configuration (FF_FS_READONLY = 1). */


#define d_FAT_FS_LOCK		16
/* The option d_FAT_FS_LOCK switches file lock function to control duplicated file open
/  and illegal operation to open objects. This option must be 0 when FF_FS_READONLY
/  is 1.
/
/  0:  Disable file lock function. To avoid volume corruption, application program
/      should avoid illegal open, remove and rename to the open objects.
/  >0: Enable file lock function. The value defines how many files/sub-directories
/      can be opened simultaneously under file lock control. Note that the file
/      lock control is independent of re-entrancy. */


#define FF_FS_REENTRANT	0
#define FF_FS_TIMEOUT	1000
#define FF_SYNC_t		HANDLE
/* The option FF_FS_REENTRANT switches the re-entrancy (thread safe) of the FatFs
/  module itself. Note that regardless of this option, file access to different
/  volume is always re-entrant and volume control functions, d_FAT_Mount(), d_FAT_Mkfs()
/  and d_FAT_FDisk() function, are always not re-entrant. Only file/directory access
/  to the same volume is under control of this function.
/
/   0: Disable re-entrancy. FF_FS_TIMEOUT and FF_SYNC_t have no effect.
/   1: Enable re-entrancy. Also user provided synchronization handlers,
/      ff_req_grant(), ff_rel_grant(), ff_del_syncobj() and ff_cre_syncobj()
/      function, must be added to the project. Samples are available in
/      option/syscall.c.
/
/  The FF_FS_TIMEOUT defines timeout period in unit of time tick.
/  The FF_SYNC_t defines O/S dependent sync object type. e.g. HANDLE, ID, OS_EVENT*,
/  SemaphoreHandle_t and etc. A header file for O/S definitions needs to be
/  included somewhere in the scope of ff.h. */

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_FFCONF_H */