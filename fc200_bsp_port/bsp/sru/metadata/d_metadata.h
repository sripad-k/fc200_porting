/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_metadata

  Abstract           : Metadata CSC module, Revision 03

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-157
                                                                SWREQ-158
                                                                SWREQ-159
                       SDD References: 136T-2200-131000-001-D22 SWDES-546
                                                                SWDES-604
\note
  CSCID              : SWDES-60
*************************************************************************/

#ifndef METADATA_H
#define METADATA_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"         /* Common type definitions */
#include "soc/defines/d_common_status.h"        /* Common status and error definitions */

/***** Constants ********************************************************/

extern const Uint32_t d_META_DATA_OFFSET_IN_QSPI_FLASH;

extern const Uint8_t d_META_FORMAT_REVISION;

typedef enum
{
  d_META_FW_TYPE_BIN = 0u,
  d_META_FW_TYPE_MASK = 1u,
  d_META_FW_TYPE_VERIFY = 2u,
} d_META_FWtype_t;

typedef enum
{
  d_META_SW_TYPE_BOOTLOADER = 0u,
  d_META_SW_TYPE_PROGRAMMER = 1u,
  d_META_SW_TYPE_APPLICATION = 2u
} d_META_SWtype_t;

/***** Type Definitions *************************************************/

#pragma pack(1) /* Pack all metadata with byte alignment */

typedef struct
{
  Uint32_t configStartIdentifier;           /* Unique number indicating the start of the metadata header */
  Uint8_t formatRevision;                   /* Revision of the metadata format. Corresponds to the revision of document
                                               905A-0000-360001-001-G19 in ARAS */
  Uint8_t headerPadding1[3u];               /* Padding to align data boundary to 4 bytes */
  Uint8_t numberHWblocks;                   /* Number of hardware blocks */
  Uint8_t numberFWblocks;                   /* Number of firmware blocks */
  Uint8_t numberSWblocks;                   /* Number of software blocks */
  Uint8_t numberMISCblocks;                 /* Number of miscellaneous blocks */
  Uint32_t offsetHWblock;                   /* Hardware blocks offset */
  Uint32_t offsetFWblock;                   /* Firmware blocks offset */
  Uint32_t offsetSWblock;                   /* Software blocks offset */
  Uint32_t offsetMISCblock;                 /* Miscellaneous blocks offset */
  Uint32_t checksum;                        /* Checksum of the metadata header */
} d_META_Header_t;                          /* Metadata header definition */

typedef struct
{
  Uint32_t typeSpecifier;     /* Can be used to indicate whether this is an SRU or LRU */
  Char_t manufactureDate[8];  /* CCYYMMDD */
  Char_t partNumber[20];      /* 20 character ARAS part number */
  Uint8_t revision;           /* Should be aligned with ARAS revision */
  Uint8_t hwPadding1[3u];     /* Padding to align data boundary to 4 bytes */
  Char_t serialNumber[4];     /* Alphanumeric */
  Uint32_t blockChecksum;     /* Checksum of the metadata block */
} d_META_HWblock_t;           /* HW block definition */

typedef struct
{
  Uint32_t typeSpecifier;     /* Can be used to indicate whether this is a SRU or LRU */
  Char_t programDate[8];      /* CCYYMMDD */
  Char_t programTime[6];      /* HHMMSS */
  Char_t partNumber[20];      /* 20 character ARAS part number */
  Uint8_t revision;           /* Should be aligned with ARAS revision */
  Uint8_t fwPadding[1u];      /* Padding to align data boundary to 4 bytes */
  Uint32_t startAddress;      /* Start address of binary in non-volatile memory */
  Uint32_t size;              /* Size of binary in non-volatile memory */
  Uint32_t checksum;          /* Checksum of binary in non-volatile memory */
  Uint32_t blockChecksum;     /* Checksum of the metadata block */
} d_META_FWblock_t;           /* FW block definition */

typedef struct
{
  Uint32_t typeSpecifier;     /* Used to identify which state this block is used by */
  Char_t programDate[8];      /* CCYYMMDD */
  Char_t programTime[6];      /* HHMMSS */
  Char_t partNumber[20];      /* 20 character ARAS part number */
  Uint8_t revision;           /* Should be aligned with ARAS revision */
  Uint8_t swPadding[1u];      /* Padding to align data boundary to 8 bytes */
  Uint32_t targetAddressLo;   /* Lower 32 bits of start address of binary in volatile memory */
  Uint32_t targetAddressHi;   /* Higher 32 bits of start address of binary in volatile memory */
  Uint32_t startAddress;      /* Start address of binary in non-volatile memory */
  Uint32_t size;              /* Size of binary in non-volatile memory */
  Uint32_t checksum;          /* Checksum of binary in non-volatile memory */
  Uint32_t blockChecksum;     /* Checksum of the metadata block */
} d_META_SWblock_t;           /* SW block definition */

typedef struct
{
  Uint32_t typeSpecifier;     /* Used to identify which state this block is used by */
  Uint32_t startAddress;      /* Start address of data in non-volatile memory */
  Uint32_t length;            /* Length of data in non-volatile memory */
  Uint32_t checksum;          /* Checksum of data in non-volatile memory */
  Uint32_t blockChecksum;     /* Checksum of the metadata block */
} d_META_MISCblock_t;         /* Miscellaneous block definition */

#pragma pack()	/* Revert packing to default (compilation start) value */

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise CSC */
d_Status_t d_META_Initialise(void);

/* CRC32 validation of Metadata */
d_Status_t d_META_ValidTest(const Uint32_t index);

/* Get memory usage */
d_Status_t d_META_MemoryUsage(const Uint32_t index, Uint32_t * const pFlashUsage, Uint32_t * const pRamUsage);

#endif /* METADATA_H */
