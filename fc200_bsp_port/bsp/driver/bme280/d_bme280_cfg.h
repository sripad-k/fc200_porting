/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_bme_cfg.h

  Abstract           : BME280 SPI configuration.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

#ifndef D_BME280_CFG_H
#define D_BME280_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "xparameters.h"

/***** Literals *********************************************************/

#define d_BME280_MAX_INTERFACES  4u

#define d_BME280_COUNT const Uint32_t d_BME280_Count = (sizeof(d_BME280_Config) / sizeof(d_BME280_Definition_t))


/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t channel;
  Uint32_t device;
} d_BME280_Definition_t;

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_BME280_Definition_t d_BME280_Config[];

extern const Uint32_t d_BME280_Count;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* D_BME280_CFG_H */
