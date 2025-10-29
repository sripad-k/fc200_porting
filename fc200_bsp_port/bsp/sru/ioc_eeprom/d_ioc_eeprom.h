/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_ioc_eeprom.h

  Abstract           : IOC EEPROM interface..

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-753
                       SDD References: 136T-2200-131100-001-D22 SWDES-2424
                                                                SWDES-2425

\note
  CSC ID             : SWDES-2419
*************************************************************************/

#ifndef D_IOC_EEPROM_H
#define D_IOC_EEPROM_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/* Number of pages available in 256kb EEPROM */
#define d_IOC_EEPROM_PAGE_COUNT     512u

/* Page size in bytes */
#define d_IOC_EEPROM_PAGE_SIZE      64u

/* Status bit definitions */
#define d_IOC_EEPROM_READ_BUSY        0x01u
#define d_IOC_EEPROM_READ_ERROR       0x02u
#define d_IOC_EEPROM_WRITE_BUSY       0x04u
#define d_IOC_EEPROM_WRITE_ERROR      0x08u
#define d_IOC_EEPROM_CONFIG_VALID     0x10u
#define d_IOC_EEPROM_INTERFACE_VALID  0x20u

/***** Type Definitions *************************************************/

typedef Uint8_t d_IOC_EEPROM_PageBuffer_t[d_IOC_EEPROM_PAGE_SIZE];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Verify configuration */
d_Status_t d_IOC_EEPROM_Initialise(void);

/* Read the interface status */
d_Status_t d_IOC_EEPROM_Status(const Uint32_t device, Uint32_t * const pStatus);

/* Read the EEPROM */
d_Status_t d_IOC_EEPROM_Read(const Uint32_t device);

/* Write a page to the EEPROM */
d_Status_t d_IOC_EEPROM_Write(const Uint32_t device, const Uint32_t page, const d_IOC_EEPROM_PageBuffer_t pBuffer);

#endif /* D_IOC_EEPROM_H */
