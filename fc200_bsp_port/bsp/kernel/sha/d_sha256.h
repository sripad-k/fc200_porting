/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : SHA256

  Abstract           : Calculation of 256 bit SHA.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-320
                       SDD References: 136T-2200-131000-001-D22 SWDES-13
                                                                SWDES-14
                                                                SWDES-15
                                                                SWDES-16
                                                                SWDES-17
\note
  CSC ID             : SWDES-82
*************************************************************************/

#ifndef D_SHA256_H
#define D_SHA256_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/* Size of the SHA-256 sum. This times eight is 256 bits. */
#define SIZE_OF_SHA_256_HASH 32u

/* Size of the chunks used for the calculations. */
#define SIZE_OF_SHA_256_CHUNK 64u

/***** Type Definitions *************************************************/

/* The opaque SHA-256 type, that should be instantiated when using the streaming API. */
typedef struct
{
  Uint8_t *hash;
  Uint8_t chunk[SIZE_OF_SHA_256_CHUNK];
  Uint32_t chunkIndex;
  Uint32_t spaceLeft;
  Uint32_t totalLen;
  Uint32_t h[8];
} Sha256_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the SHA256 CSC */
d_Status_t d_SHA256_Initialise(void);

/* The simple SHA-256 calculation function. */
void d_SHA256_Calculate(Uint8_t hash[SIZE_OF_SHA_256_HASH], const Uint8_t * const input, const Uint32_t len);

/* Initialize a SHA-256 streaming calculation. */
void d_SHA256_Init(Sha256_t * const sha256, Uint8_t hash[SIZE_OF_SHA_256_HASH]);

/* Stream more input data for an on-going SHA-256 calculation. */
void d_SHA256_Write(Sha256_t * const sha256, const Uint8_t * const data, const Uint32_t len);

/* Conclude a SHA-256 streaming calculation, making the hash value available. */
Uint8_t *d_SHA256_Close(Sha256_t *sha256);

#endif /* D_SHA256_H */
