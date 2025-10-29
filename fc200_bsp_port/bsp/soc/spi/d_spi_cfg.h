/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_spi_cfg.h

  Abstract           : SPI configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-246
                       SDD References: 136T-2200-131100-001-D22 SWDES-111
                                                                SWDES-112
                                                                SWDES-113
                                                                SWDES-114
                                                                SWDES-115
\note
CSC_ID             : SWDES-45
*************************************************************************/

#ifndef D_SPI_CFG_H
#define D_SPI_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "xparameters.h"

/***** Literals *********************************************************/

#define d_SPI_MAX_INTERFACES  2u

#define d_SPI_COUNT const Uint32_t d_SPI_Count = (sizeof(d_SPI_Config) / sizeof(d_SPI_Config_t))


/***** Type Definitions *************************************************/

typedef enum
{
  d_SPI_CLK_INACTIVE_STATE_LOW = 0,
  d_SPI_CLK_INACTIVE_STATE_HIGH,

  d_SPI_CLK_INACTIVE_STATE_COUNT
} d_SPI_ClkInactiveState_t;

typedef enum
{
  d_SPI_CLK_PHASE_FIRST = 0,
  d_SPI_CLK_PHASE_SECOND,

  d_SPI_CLK_PHASE_COUNT
} d_SPI_ClkPhase_t;
  

/* Callback type for event */
typedef void (*d_SPI_EventHandler) (const Uint32_t channel, const d_SPI_State_t status);

typedef struct
{
  Uint32_t baseAddress;
  Uint32_t clkFrequency;
  Uint32_t interruptNumber;
  d_SPI_ClkInactiveState_t clkInactiveState;
  d_SPI_ClkPhase_t clkPhase;
  Uint32_t baud;
  d_SPI_EventHandler eventHandler;
} d_SPI_Config_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_SPI_Config_t d_SPI_Config[];

extern const Uint32_t d_SPI_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_SPI_CFG_H */
