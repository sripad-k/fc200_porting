/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_auav.h

  Abstract           : AUAV device driver.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-???
                       SDD References: 136T-2200-131100-001-D22 SWDES-???
                       
\note
  CSC ID             : SWDES-??
*************************************************************************/

#ifndef D_AUAV_H
#define D_AUAV_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"      /** Contains variable type definitions like Uint8 */
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

#define d_AUAV_MAX_DEVICES  4u

/***** Type Definitions *************************************************/

typedef enum
{
  d_AUAV_CHANNEL_ABSOLUTE,
  d_AUAV_CHANNEL_DIFFERENTIAL,
  
  d_AUAV_CHANNEL_COUNT
} d_AUAV_Channel_t;

typedef struct
{
  Float32_t pressure;     /* Pa */
  Float32_t temperature;  /* Deg C */
  Bool_t valid;
} d_AUAV_Reading_t;

typedef struct
{
  d_AUAV_Reading_t absolute;
  d_AUAV_Reading_t differential;
} d_AUAV_DataBlock_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialises the AUAV CSC */
d_Status_t d_AUAV_Initialise(const Uint32_t device, d_AUAV_DataBlock_t * const pDataBlock);

/* Read the AUAV status */
d_Status_t d_AUAV_Read_Status(const Uint32_t device, const d_AUAV_Channel_t channel, Uint8_t * const pStatus);

/* Start conversion of the AUAV sensor data */
d_Status_t d_AUAV_Start(const Uint32_t device);

/* d_AUAV_InterruptHandler */
void d_AUAV_InterruptHandler(const Uint32_t spiChannel, const Uint32_t spiDevice);

#endif /* D_AUAV_H */
