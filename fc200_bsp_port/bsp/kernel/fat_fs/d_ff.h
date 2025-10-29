/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : d_ff.h

  Abstract : Kernel FAT FS API

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

#ifndef D_FF_H
#define D_FF_H

/***** Includes *********************************************************/

#include "xil_types.h"
#include "soc/defines/d_common_types.h"
#include "d_ffconf.h"	/* FatFs configuration options */

/***** Constants ********************************************************/

/* Flags and offset address                                     */

/* File access mode and open method flags (3rd argument of d_FAT_Open) */
#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30

/* Fast seek controls (2nd argument of d_FAT_LSeek) */
#define d_FAT_CREATE_LINKMAP	((d_FAT_FSIZE_t)0 - 1)

/* Format options (2nd argument of d_FAT_Mkfs) */
#define FM_FAT		0x01
#define FM_FAT32	0x02
#define FM_EXFAT	0x04
#define FM_ANY		0x07
#define FM_SFD		0x08

/* Filesystem type (FATFS.fs_type) */
#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3
#define FS_EXFAT	4

/* File attribute bits for directory entry (FILINFO.fattrib) */
#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */


/***** Type Definitions *************************************************/

/* Definitions of volume management */

#if FF_MULTI_PARTITION		/* Multiple partition configuration */
typedef struct {
	Uint8_t pd;	/* Physical drive number */
	Uint8_t pt;	/* Partition: 0:Auto detect, 1-4:Forced partition) */
} PARTITION;
extern PARTITION VolToPart[10];	/* Volume - Partition resolution table */
#endif

#if FF_STR_VOLUME_ID
#ifndef FF_VOLUME_STRS
extern const char* VolumeStr[FF_VOLUMES];	/* User defied volume ID */
#endif
#endif



/* Type of path name strings on FatFs API */

//#ifndef _INC_TCHAR
//#define _INC_TCHAR

typedef char TCHAR;
//#define _T(x) x
//#define _TEXT(x) x

//#endif



/* Type of file size variables */

#if FF_FS_EXFAT
typedef Uint64_t d_FAT_FSIZE_t;
#else
typedef Uint32_t d_FAT_FSIZE_t;
#endif



/* Filesystem object structure (FATFS) */

typedef struct {
	Uint8_t	fs_type;		/* Filesystem type (0:N/A) */
	Uint8_t	pdrv;			/* Physical drive number */
	Uint8_t	n_fats;			/* Number of FATs (1 or 2) */
	Uint8_t	wflag;			/* win[] flag (b0:dirty) */
	Uint8_t	fsi_flag;		/* FSINFO flags (b7:disabled, b0:dirty) */
	Uint16_t	id;				/* Volume mount ID */
	Uint16_t	n_rootdir;		/* Number of root directory entries (FAT12/16) */
	Uint16_t	csize;			/* Cluster size [sectors] */
#if FF_MAX_SS != FF_MIN_SS
	Uint16_t	ssize;			/* Sector size (512, 1024, 2048 or 4096) */
#endif
#ifdef FF_USE_LFN
	Uint16_t*	lfnbuf;			/* LFN working buffer */
#endif
#if FF_FS_EXFAT
	Uint8_t*	dirbuf;			/* Directory entry block scratchpad buffer for exFAT */
#endif
#if FF_FS_REENTRANT
	FF_SYNC_t	sobj;		/* Identifier of sync object */
#endif
#if !FF_FS_READONLY
	Uint32_t	last_clst;		/* Last allocated cluster */
	Uint32_t	free_clst;		/* Number of free clusters */
#endif
#if FF_FS_RPATH
	Uint32_t	cdir;			/* Current directory start cluster (0:root) */
#if FF_FS_EXFAT
	Uint32_t	cdc_scl;		/* Containing directory start cluster (invalid when cdir is 0) */
	Uint32_t	cdc_size;		/* b31-b8:Size of containing directory, b7-b0: Chain status */
	Uint32_t	cdc_ofs;		/* Offset in the containing directory (invalid when cdir is 0) */
#endif
#endif
	Uint32_t	n_fatent;		/* Number of FAT entries (number of clusters + 2) */
	Uint32_t	fsize;			/* Size of a FAT [sectors] */
	Uint32_t	volbase;		/* Volume base sector */
	Uint32_t	fatbase;		/* FAT base sector */
	Uint32_t	dirbase;		/* Root directory base sector/cluster */
	Uint32_t	database;		/* Data base sector */
	Uint32_t	winsect;		/* Current sector appearing in the win[] */
#ifdef __ICCARM__
#pragma data_alignment = 32
	Uint8_t	win[FF_MAX_SS];
#else
#ifdef __aarch64__
	Uint8_t	win[FF_MAX_SS] __attribute__ ((aligned(64)));	/* Disk access window for Directory, FAT (and file data at tiny cfg) */
#else
	Uint8_t	win[FF_MAX_SS] __attribute__ ((aligned(32)));	/* Disk access window for Directory, FAT (and file data at tiny cfg) */
#endif
#endif
} d_FAT_Fs_t;



/* Object ID and allocation information (FFOBJID) */

typedef struct {
	d_FAT_Fs_t*	fs;				/* Pointer to the hosting volume of this object */
	Uint16_t	id;				/* Hosting volume mount ID */
	Uint8_t	attr;			/* Object attribute */
	Uint8_t	stat;			/* Object chain status (b1-0: =0:not contiguous, =2:contiguous, =3:flagmented in this session, b2:sub-directory stretched) */
	Uint32_t	sclust;			/* Object data start cluster (0:no cluster or root directory) */
	d_FAT_FSIZE_t	objsize;		/* Object size (valid when sclust != 0) */
#if FF_FS_EXFAT
	Uint32_t	n_cont;			/* Size of first fragment - 1 (valid when stat == 3) */
	Uint32_t	n_frag;			/* Size of last fragment needs to be written to FAT (valid when not zero) */
	Uint32_t	c_scl;			/* Containing directory start cluster (valid when sclust != 0) */
	Uint32_t	c_size;			/* b31-b8:Size of containing directory, b7-b0: Chain status (valid when c_scl != 0) */
	Uint32_t	c_ofs;			/* Offset in the containing directory (valid when file object and sclust != 0) */
#endif
#ifdef d_FAT_FS_LOCK
	Uint32_t	lockid;			/* File lock ID origin from 1 (index of file semaphore table Files[]) */
#endif
} d_FAT_ObjId_t;



/* File object structure (FIL) */

typedef struct {
	d_FAT_ObjId_t	obj;			/* Object identifier (must be the 1st member to detect invalid object pointer) */
	Uint8_t	flag;			/* File status flags */
	Uint8_t	err;			/* Abort flag (error code) */
	d_FAT_FSIZE_t	fptr;			/* File read/write pointer (Zeroed on file open) */
	Uint32_t	clust;			/* Current cluster of fpter (invalid when fptr is 0) */
	Uint32_t	sect;			/* Sector number appearing in buf[] (0:invalid) */
#if !FF_FS_READONLY
	Uint32_t	dir_sect;		/* Sector number containing the directory entry (not used at exFAT) */
	Uint8_t*	dir_ptr;		/* Pointer to the directory entry in the win[] (not used at exFAT) */
#endif
#if d_FAT_USE_FASTSEEK
	Uint32_t*	cltbl;			/* Pointer to the cluster link map table (nulled on open, set by application) */
#endif
#if !FF_FS_TINY
#ifdef __ICCARM__
#pragma data_alignment = 32
	Uint8_t	buf[FF_MAX_SS];	/* File private data read/write window */
#else
#ifdef __aarch64__
	Uint8_t	buf[FF_MAX_SS] __attribute__ ((aligned(64)));	/* File private data read/write window */
#else
	Uint8_t	buf[FF_MAX_SS] __attribute__ ((aligned(32)));	/* File private data read/write window */
#endif
#endif
#endif
} d_FAT_File_t;



/* Directory object structure (DIR) */

typedef struct {
	d_FAT_ObjId_t		obj;			/* Object identifier */
	Uint32_t	dptr;			/* Current read/write offset */
	Uint32_t	clust;			/* Current cluster */
	Uint32_t	sect;			/* Current sector (0:Read operation has terminated) */
	Uint8_t*	dir;			/* Pointer to the directory item in the win[] */
	Uint8_t		fn[12];			/* SFN (in/out) {body[8],ext[3],status[1]} */
#ifdef FF_USE_LFN
	Uint32_t	blk_ofs;		/* Offset of current entry block being processed (0xFFFFFFFF:Invalid) */
#endif
#ifdef FF_USE_FIND
	const TCHAR* pat;		/* Pointer to the name matching pattern */
#endif
} d_FAT_Dir_t;



/* File information structure (FILINFO) */

typedef struct {
	d_FAT_FSIZE_t	fsize;			/* File size */
	Uint32_t	fdate;			/* Modified date */
	Uint32_t	ftime;			/* Modified time */
	Uint8_t	fattrib;		/* File attribute */
#ifdef FF_USE_LFN
	TCHAR	altname[FF_SFN_BUF + 1];/* Altenative file name */
	TCHAR	fname[FF_LFN_BUF + 1];	/* Primary file name */
#else
	TCHAR	fname[12 + 1];	/* File name */
#endif
} d_FAT_FileInfo_t;



/* File function return code (FRESULT) */

typedef enum {
	d_FAT_OK = 0,				/* (0) Succeeded */
	d_FAT_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	d_FAT_INT_ERR,				/* (2) Assertion failed */
	d_FAT_NOT_READY,			/* (3) The physical drive cannot work */
	d_FAT_NO_FILE,				/* (4) Could not find the file */
	d_FAT_NO_PATH,				/* (5) Could not find the path */
	d_FAT_INVALID_NAME,		/* (6) The path name format is invalid */
	d_FAT_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	d_FAT_EXIST,				/* (8) Access denied due to prohibited access */
	d_FAT_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	d_FAT_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	d_FAT_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	d_FAT_NOT_ENABLED,			/* (12) The volume has no work area */
	d_FAT_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	d_FAT_MKFS_ABORTED,		/* (14) The d_FAT_Mkfs() aborted due to any problem */
	d_FAT_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	d_FAT_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	d_FAT_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	d_FAT_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > d_FAT_FS_LOCK */
	d_FAT_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} d_FAT_Result_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

d_FAT_Result_t d_FAT_Open (d_FAT_File_t* fp, const TCHAR* path, Uint8_t mode);				/* Open or create a file */
d_FAT_Result_t d_FAT_Close (d_FAT_File_t* fp);											/* Close an open file object */
d_FAT_Result_t d_FAT_Read (d_FAT_File_t* fp, void* buff, Uint32_t btr, Uint32_t* br);			/* Read data from the file */
d_FAT_Result_t d_FAT_Write (d_FAT_File_t* fp, const void* buff, Uint32_t btw, Uint32_t* bw);	/* Write data to the file */
d_FAT_Result_t d_FAT_LSeek (d_FAT_File_t* fp, d_FAT_FSIZE_t ofs);								/* Move file pointer of the file object */
d_FAT_Result_t d_FAT_Truncate (d_FAT_File_t* fp);										/* Truncate the file */
d_FAT_Result_t d_FAT_Sync (d_FAT_File_t* fp);											/* Flush cached data of the writing file */
d_FAT_Result_t d_FAT_Opendir (d_FAT_Dir_t* dp, const TCHAR* path);						/* Open a directory */
d_FAT_Result_t d_FAT_CloseDir (d_FAT_Dir_t* dp);										/* Close an open directory */
d_FAT_Result_t d_FAT_ReadDir (d_FAT_Dir_t* dp, d_FAT_FileInfo_t* fno);							/* Read a directory item */
d_FAT_Result_t d_FAT_FindFirst (d_FAT_Dir_t* dp, d_FAT_FileInfo_t* fno, const TCHAR* path, const TCHAR* pattern);	/* Find first file */
d_FAT_Result_t d_FAT_FindNext (d_FAT_Dir_t* dp, d_FAT_FileInfo_t* fno);							/* Find next file */
d_FAT_Result_t d_FAT_MkDir (const TCHAR* path);								/* Create a sub directory */
d_FAT_Result_t d_FAT_UnLink (const TCHAR* path);								/* Delete an existing file or directory */
d_FAT_Result_t d_FAT_Rename (const TCHAR* path_old, const TCHAR* path_new);	/* Rename/Move a file or directory */
d_FAT_Result_t d_FAT_Stat (const TCHAR* path, d_FAT_FileInfo_t* fno);					/* Get file status */
d_FAT_Result_t d_FAT_Chmod (const TCHAR* path, Uint8_t attr, Uint8_t mask);			/* Change attribute of a file/dir */
d_FAT_Result_t d_FAT_Utime (const TCHAR* path, const d_FAT_FileInfo_t* fno);			/* Change timestamp of a file/dir */
d_FAT_Result_t d_FAT_ChDir (const TCHAR* path);								/* Change current directory */
d_FAT_Result_t d_FAT_ChDrive (const TCHAR* path);								/* Change current drive */
d_FAT_Result_t d_FAT_GetCwd (TCHAR* buff, Uint32_t len);							/* Get current directory */
d_FAT_Result_t d_FAT_GetFree (const TCHAR* path, Uint32_t* nclst, d_FAT_Fs_t** fatfs);	/* Get number of free clusters on the drive */
d_FAT_Result_t d_FAT_GetLabel (const TCHAR* path, TCHAR* label, Uint32_t* vsn);	/* Get volume label */
d_FAT_Result_t d_FAT_SetLabel (const TCHAR* label);							/* Set volume label */
d_FAT_Result_t d_FAT_Forward (d_FAT_File_t* fp, Uint32_t(*func)(const Uint8_t*,Uint32_t), Uint32_t btf, Uint32_t* bf);	/* Forward data to the stream */
d_FAT_Result_t d_FAT_Expand (d_FAT_File_t* fp, d_FAT_FSIZE_t szf, Uint8_t opt);					/* Allocate a contiguous block to the file */
d_FAT_Result_t d_FAT_Mount (d_FAT_Fs_t* fs, const TCHAR* path, Uint8_t opt);			/* Mount/Unmount a logical drive */
d_FAT_Result_t d_FAT_Mkfs (const TCHAR* path, Uint8_t opt, Uint32_t au, void* work, Uint32_t len);	/* Create a FAT volume */
d_FAT_Result_t d_FAT_FDisk (Uint8_t pdrv, const Uint32_t* szt, void* work);			/* Divide a physical drive into some partitions */
d_FAT_Result_t d_FAT_Setcp (Uint16_t cp);											/* Set current code page */
int d_FAT_Putc (TCHAR c, d_FAT_File_t* fp);										/* Put a character to the file */
int d_FAT_Puts (const TCHAR* str, d_FAT_File_t* cp);								/* Put a string to the file */
int d_FAT_Printf (d_FAT_File_t* fp, const TCHAR* str, ...);						/* Put a formatted string to the file */
TCHAR* d_FAT_Gets (TCHAR* buff, int len, d_FAT_File_t* fp);						/* Get a string from the file */

#define d_FAT_Eof(fp) ((int)((fp)->fptr == (fp)->obj.objsize))
#define d_FAT_Error(fp) ((fp)->err)
#define d_FAT_Tell(fp) ((fp)->fptr)
#define d_FAT_Size(fp) ((fp)->obj.objsize)
#define d_FAT_Rewind(fp) d_FAT_LSeek((fp), 0)
#define d_FAT_Rewinddir(dp) d_FAT_ReadDir((dp), 0)
#define d_FAT_RmDir(path) d_FAT_UnLink(path)
#define d_FAT_Unmount(path) d_FAT_Mount(NULL, path, 0)

/*--------------------------------------------------------------*/
/* Additional user defined functions                            */

/* RTC function */
#if !FF_FS_READONLY && !FF_FS_NORTC
typedef unsigned int       Uint32_t;
Uint32_t d_FAT_Gettime (void);
#endif

/* LFN support functions */
#if FF_USE_LFN >= 1						/* Code conversion (defined in unicode.c) */
Uint16_t d_FAT_Oem2uni (Uint16_t oem, Uint16_t cp);	/* OEM code to Unicode conversion */
Uint16_t d_FAT_Uni2oem (Uint32_t uni, Uint16_t cp);	/* Unicode to OEM code conversion */
Uint32_t d_FAT_WToUpper (Uint32_t uni);			/* Unicode upper-case conversion */
#endif
#if FF_USE_LFN == 3						/* Dynamic memory allocation */
void* ff_memalloc (Uint32_t msize);			/* Allocate memory block */
void ff_memfree (void* mblock);			/* Free memory block */
#endif

/* Sync functions */
#if FF_FS_REENTRANT
int ff_cre_syncobj (Uint8_t vol, FF_SYNC_t* sobj);	/* Create a sync object */
int ff_req_grant (FF_SYNC_t sobj);		/* Lock sync object */
void ff_rel_grant (FF_SYNC_t sobj);		/* Unlock sync object */
int ff_del_syncobj (FF_SYNC_t sobj);	/* Delete a sync object */
#endif

#endif /* d_FF_H */
