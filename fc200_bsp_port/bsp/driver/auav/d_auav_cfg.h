/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_auav_cfg.h

  Abstract           : AUAV SPI configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

#ifndef D_AUAV_CFG_H
#define D_AUAV_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "d_auav.h"
//#include "xparameters.h"

/***** Literals *********************************************************/

#define d_AUAV_MAX_INTERFACES  4u

#define d_AUAV_COUNT const Uint32_t d_AUAV_Count = (sizeof(d_AUAV_Config) / (sizeof(d_AUAV_SpiChannel_t) * (Uint32_t)d_AUAV_CHANNEL_COUNT))

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t spiChannel;
  Uint32_t spiDevice;
} d_AUAV_SpiChannel_t;

typedef struct
{
  d_AUAV_SpiChannel_t spi[d_AUAV_CHANNEL_COUNT];
  Float32_t calRange;
} d_AUAV_Config_t;

/* Callback type for event */
typedef void (*d_AUAV_IrqHandler) (const Uint32_t device);

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_AUAV_Config_t d_AUAV_Config[];

extern const Uint32_t d_AUAV_Count;

extern const d_AUAV_IrqHandler callback;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_AUAV_CFG_H */
