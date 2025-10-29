/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_bme280.h

  Abstract           : BME280 device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

#ifndef D_BME280_H
#define D_BME280_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialises the BME280 CSC */
d_Status_t d_BME280_Initialise(const Uint32_t device, const Uint8_t config, const Uint8_t ctrl_hum, const Uint8_t ctrl_meas);

/* Read the BME280 data */
d_Status_t d_BME280_Read(const Uint32_t device, Float64_t * const pHumidity, Float64_t * const pPressure, Float64_t * const pTemperature);

#endif /* D_BME280_H */
