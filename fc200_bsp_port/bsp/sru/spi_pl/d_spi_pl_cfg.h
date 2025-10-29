/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SPI Header File

  Abstract           : SOC PL SPI interface configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-758
                       SDD References: 136T-2200-131100-001-D22 SWDES-2427
                                                                SWDES-2428
                                                                SWDES-2429
                                                                SWDES-2430
                                                                SWDES-2431
                                                                SWDES-2432
\note
  CSC ID             : SWDES-2422
*************************************************************************/

#ifndef D_SPI_PL_CFG_H
#define D_SPI_PL_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/* Maximum number of interfaces */
#define d_SPI_PL_MAX_INTERFACES  10u

/* Maximum number of devices per channel */
#define d_SPI_PL_MAX_DEVICES       4u

/* Macro used to define the number of interfaces */
#define d_SPI_PL_COUNT const Uint32_t d_SPI_PL_Count = (sizeof(d_SPI_PL_Definition) / sizeof(d_SPI_PL_Definition_t))

/***** Type Definitions *************************************************/

/* Callback type for event */
typedef void (*d_SPI_PL_IrqHandlerEvent) (const Uint32_t channel);
typedef void (*d_SPI_PL_IrqHandlerDevice) (const Uint32_t spiChannel, const Uint32_t spiDevice);

typedef struct
{
  Pointer_t baseAddress;
  Uint32_t frequency;
  Uint32_t interruptNumber;
  Bool_t excludeCommandResponse;
  d_SPI_PL_IrqHandlerEvent eventHandler;
  d_SPI_PL_IrqHandlerDevice deviceHandler;
} d_SPI_PL_Definition_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11;  Size not specified to allow size to be determined by defining values in application.
extern const d_SPI_PL_Definition_t d_SPI_PL_Definition[];

/* Number of interfaces */
extern const Uint32_t d_SPI_PL_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_SPI_PL_CFG_H */
