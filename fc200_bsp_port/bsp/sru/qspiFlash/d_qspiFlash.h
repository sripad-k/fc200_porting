/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : QSPI Flash driver header file

  Abstract           : QSPI Flash driver

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-141
                                                                SWREQ-142
                                                                SWREQ-143
                       SDD References: 136T-2200-131000-001-D22 SWDES-254
                                                                SWDES-255
                                                                SWDES-256
                                                                SWDES-257
                                                                SWDES-258
\note
  CSC ID             : SWDES-66
*************************************************************************/

#ifndef D_QSPIFLASH_H
#define D_QSPIFLASH_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Literals *********************************************************/

typedef enum
{
  d_QSPI_STATUS_UNKNOWN = 0u,
  d_QSPI_STATUS_IDLE = 1u,
  d_QSPI_STATUS_ERASE_BUSY = 2u,
  d_QSPI_STATUS_ERASE_COMPLETE = 3u,
  d_QSPI_STATUS_ERASE_FAIL = 4u,
  d_QSPI_STATUS_PROGRAM_BUSY = 5u,
  d_QSPI_STATUS_PROGRAM_COMPLETE = 6u,
  d_QSPI_STATUS_PROGRAM_FAIL = 7u,
  d_QSPI_STATUS_READ_BUSY = 8u,
  d_QSPI_STATUS_READ_COMPLETE = 9u,
  d_QSPI_STATUS_READ_FAIL = 10u,
  d_QSPI_STATUS_ERROR = 11u
} d_QSPI_ModuleStatus_t;

/***** Constants ********************************************************/

#define d_QSPI_SUB_SECTOR_SIZE_WORDS 1024u /* Number of words in sub-sector (singleChip = 4096 bytes = 1024 words) */

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the QSPI flash driver */
d_Status_t d_QSPI_Initialise(void);

/* Read QSPI flash memory with DMA into buffer */
d_Status_t d_QSPI_Read(const Uint32_t qspiAddress, const Uint32_t numWordsToRead, Uint32_t* const pReadBuffer, const Uint32_t readBufferSizeInWords);

/* Program QSPI flash memory from buffer */
d_Status_t d_QSPI_Write(const Uint32_t qspiAddress, const Uint32_t numWordsToWrite, Uint32_t* const pWriteBuffer, const Uint32_t writeBufferSizeInWords);

/* Erase 4KB sub-sector at QSPI flash memory address */
d_Status_t d_QSPI_EraseSubSector4K(const Uint32_t qspiAddress);

/* Get the current QSPI driver module status */
d_QSPI_ModuleStatus_t d_QSPI_GetQspiModuleStatus(void);

/* Read a byte from the Serial Flash Discoverable Parameters */
d_Status_t d_QSPI_SfdpByte(const Uint32_t address, Uint8_t * const pValue);

/* Read a word from the Serial Flash Discoverable Parameters */
d_Status_t d_QSPI_SfdpWord(const Uint32_t address, Uint32_t * const pValue);

#endif /* D_QSPIFLASH_H */
