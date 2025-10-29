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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_memory.h"
#include "d_sha256.h"

/***** Constants ********************************************************/

#define TOTAL_LEN_LEN 8u

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static inline Uint32_t rotateRight(const Uint32_t value, const Uint32_t count);
static inline void consumeChunk(Uint32_t * const h, const Uint8_t * const p);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SHA256_Initialise -->

  Initialise the CRC32 CSC.
  This function perfoms no processing and is included for adherence to the
  coding standard.
*************************************************************************/
d_Status_t             /** \return result, always success */
d_SHA256_Initialise
(
void
)
{
  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_SHA256_Init -->

  Initialise SHA calculation.
*************************************************************************/
void                                /** \return None */
d_SHA256_Init
(
Sha256_t * const sha256,            /**< [in]  A pointer to a SHA-256 structure */
Uint8_t hash[SIZE_OF_SHA_256_HASH]  /**< [in]  Hash array, where the result will be delivered */
)
{
  sha256->hash = hash;
  sha256->chunkIndex = 0;
  sha256->spaceLeft = SIZE_OF_SHA_256_CHUNK;
  sha256->totalLen = 0;
  /*
   * Initialize hash values (first 32 bits of the fractional parts of the square roots of the first 8 primes
   * 2..19):
   */
  sha256->h[0] = 0x6a09e667u;
  sha256->h[1] = 0xbb67ae85u;
  sha256->h[2] = 0x3c6ef372u;
  sha256->h[3] = 0xa54ff53au;
  sha256->h[4] = 0x510e527fu;
  sha256->h[5] = 0x9b05688cu;
  sha256->h[6] = 0x1f83d9abu;
  sha256->h[7] = 0x5be0cd19u;
}

/*********************************************************************//**
  <!-- d_SHA256_Write -->

  Add block of memory to SHA.
*************************************************************************/
void                                /** \return None */
d_SHA256_Write
(
Sha256_t * const sha256,            /**< [in]  A pointer to a previously initialized SHA-256 structure */
const Uint8_t * const data,         /**< [in]  Pointer to the data to be added to the calculation */
const Uint32_t len                  /**< [in]  Length of the data to add */
)
{
  Uint32_t lenToProcess = len;
  sha256->totalLen += len;

  const Uint8_t *p = data;
  Uint32_t pindex = 0;
  
  while (lenToProcess > 0u)
  {
  /*
     * If the input chunks have sizes that are multiples of the calculation chunk size, no copies are
     * necessary. We operate directly on the input data instead.
     */
    if ((sha256->spaceLeft == SIZE_OF_SHA_256_CHUNK) && (lenToProcess >= SIZE_OF_SHA_256_CHUNK))
    {
      consumeChunk(sha256->h, &p[pindex]);
      lenToProcess -= SIZE_OF_SHA_256_CHUNK;
      pindex = pindex + SIZE_OF_SHA_256_CHUNK;
      continue;
    }
    else
    {
      DO_NOTHING();
    }
    
    /* General case, no particular optimisation. */
    pindex = 0;
    const Uint32_t consumed_len = (lenToProcess < sha256->spaceLeft) ? lenToProcess : sha256->spaceLeft;
    d_GEN_MemoryCopy(&sha256->chunk[sha256->chunkIndex], &p[pindex], consumed_len);
    sha256->spaceLeft -= consumed_len;
    lenToProcess -= consumed_len;
    pindex = pindex + consumed_len;
    if (sha256->spaceLeft == 0u)
    {
      consumeChunk(sha256->h, sha256->chunk);
      sha256->chunkIndex = 0;
      sha256->spaceLeft = SIZE_OF_SHA_256_CHUNK;
    }
    else
    {
      sha256->chunkIndex = sha256->chunkIndex + consumed_len;
    }
  }
  
  return;
}

/*********************************************************************//**
  <!-- d_SHA256_Close -->

  Close SHA calculation.
*************************************************************************/
Uint8_t *                           /** \return Pointer to the hash array, where the result is delivered */
d_SHA256_Close
(
Sha256_t *sha256                    /**< [in]  A pointer to a previously initialized SHA-256 structure */
)
{
  Uint8_t *pos = &sha256->chunk[sha256->chunkIndex];
  Uint32_t spaceLeft = sha256->spaceLeft;
  Uint32_t *const h = sha256->h;

  /*
   * The current chunk cannot be full. Otherwise, it would already have been consumed. I.e. there is space left for
   * at least one byte. The next step in the calculation is to add a single one-bit to the data.
   */
  *pos = 0x80u;
  pos++;
  --spaceLeft;

  /*
   * Now, the last step is to add the total data length at the end of the last chunk, and zero padding before
   * that. But we do not necessarily have enough space left. If not, we pad the current chunk with zeroes, and add
   * an extra chunk at the end.
   */
  if (spaceLeft < TOTAL_LEN_LEN) 
  {
    d_GEN_MemorySet(pos, 0x00u, spaceLeft);
    consumeChunk(h, sha256->chunk);
    pos = sha256->chunk;
    spaceLeft = SIZE_OF_SHA_256_CHUNK;
  }
  else
  {
    DO_NOTHING();
  }

  const Uint32_t left = spaceLeft - TOTAL_LEN_LEN;
  d_GEN_MemorySet(pos, 0x00u, left);
  Uint32_t len = sha256->totalLen;
  pos[7u + left] = (Uint8_t)(len << 3u);
  len >>= 5u;
  Int32_t i;
  for (i = 6; i >= 0; --i)
  {
    pos[(Uint32_t)i + left] = (Uint8_t)len;
    len >>= 8u;
  }
  consumeChunk(h, sha256->chunk);
  /* Produce the final hash value (big-endian): */
  Uint32_t j = 0u;
  Uint8_t *const hash = sha256->hash;
  for (i = 0; i < 8; i++) 
  {
    hash[j]      = (Uint8_t)(h[i] >> 24u);
    hash[j + 1u] = (Uint8_t)(h[i] >> 16u);
    hash[j + 2u] = (Uint8_t)(h[i] >> 8u);
    hash[j + 3u] = (Uint8_t)h[i];
    j = j + 4u;
  }
  
  return sha256->hash;
}

/*********************************************************************//**
  <!-- d_SHA256_Calculate -->

  Calculate SHA of block of memory.
*************************************************************************/
void                                 /** \return None */
d_SHA256_Calculate
(
Uint8_t hash[SIZE_OF_SHA_256_HASH],  /**< [out] Hash result */
const Uint8_t * const input,         /**< [in]  Data to hash */
const Uint32_t len                   /**< [in]  Data length */
)
{
  Sha256_t sha256;
  d_SHA256_Init(&sha256, hash);
  d_SHA256_Write(&sha256, input, len);
  (void)d_SHA256_Close(&sha256);
  
  return;
}

/*********************************************************************//**
  <!-- rotateRight -->

  Rotate a 32-bit value by a number of bits to the right.
*************************************************************************/
static inline Uint32_t        /** \return Rotated value */
rotateRight
(
const Uint32_t value,         /**< [in]  value */
const Uint32_t count          /**< [in]  bits to shift */
)
{
  /* Defined behaviour in standard C for all count where 0 < count < 32 */
  return (value >> count) | (value << (32u - count));
}

/*********************************************************************//**
  <!-- consumeChunk -->

   Update a hash value under calculation with a new chunk of data.
*************************************************************************/
static inline void            /** \return None */
consumeChunk
(
Uint32_t * const h,           /**< [in]  Pointer to the first hash item, of a total of eight */
const Uint8_t * const p       /**< [in]  Pointer to the chunk data, which has a standard length */
)
{
  Uint32_t i;
  Uint32_t j;
  Uint32_t ah[8];

  /* Initialize working variables to current hash value: */
  for (i = 0; i < 8u; i++)
  {
    ah[i] = h[i];
  }

  /*
   * The w-array is really w[64], but since we only need 16 of them at a time, we save stack by
   * calculating 16 at a time.
   *
   * This optimisation was not there initially and the rest of the comments about w[64] are kept in their
   * initial state.
   */

  /*
   * create a 64-entry message schedule array w[0..63] of 32-bit words (The initial values in w[0..63]
   * don't matter, so many implementations zero them here) copy chunk into first 16 words w[0..15] of the
   * message schedule array
   */
  Uint32_t w[16];

  /* Compression function main loop: */
  for (i = 0; i < 4u; i++)
  {
    for (j = 0; j < 16u; j++)
    {
      if (i == 0u)
      {
        w[j] = ((Uint32_t)p[0u + (j * 4u)] << 24u) | ((Uint32_t)p[1u + (j * 4u)] << 16u) | ((Uint32_t)p[2u + (j * 4u)] << 8u) | (Uint32_t)p[3u + (j * 4u)];
      }
      else
      {
        /* Extend the first 16 words into the remaining 48 words w[16..63] of the
         * message schedule array: */
        const Uint32_t s0 = rotateRight(w[(j + 1u) & 0xfu], 7u) ^ rotateRight(w[(j + 1u) & 0xfu], 18u) ^
                (w[(j + 1u) & 0xfu] >> 3u);
        const Uint32_t s1 = rotateRight(w[(j + 14u) & 0xfu], 17u) ^
                rotateRight(w[(j + 14u) & 0xfu], 19u) ^ (w[(j + 14u) & 0xfu] >> 10u);
        w[j] = w[j] + s0 + w[(j + 9u) & 0xfu] + s1;
      }
      const Uint32_t s1 = rotateRight(ah[4], 6u) ^ rotateRight(ah[4], 11u) ^ rotateRight(ah[4], 25u);
      const Uint32_t ch = (ah[4] & ah[5]) ^ (~ah[4] & ah[6]);

      /*
       * Initialize array of round constants:
       * (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
       */
      static const Uint32_t k[] =
      {
        0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u,
        0xab1c5ed5u, 0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu,
        0x9bdc06a7u, 0xc19bf174u, 0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu,
        0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau, 0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
        0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u, 0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu,
        0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u, 0xa2bfe8a1u, 0xa81a664bu,
        0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u, 0x19a4c116u,
        0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
        0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u,
        0xc67178f2u
      };

      const Uint32_t temp1 = ah[7] + s1 + ch + k[(i << 4u) | j] + w[j];
      const Uint32_t s0 = rotateRight(ah[0], 2) ^ rotateRight(ah[0], 13) ^ rotateRight(ah[0], 22);
      const Uint32_t maj = (ah[0] & ah[1]) ^ (ah[0] & ah[2]) ^ (ah[1] & ah[2]);
      const Uint32_t temp2 = s0 + maj;

      ah[7] = ah[6];
      ah[6] = ah[5];
      ah[5] = ah[4];
      ah[4] = ah[3] + temp1;
      ah[3] = ah[2];
      ah[2] = ah[1];
      ah[1] = ah[0];
      ah[0] = temp1 + temp2;
    }
  }

  /* Add the compressed chunk to the current hash value: */
  for (i = 0; i < 8u; i++)
  {
    h[i] += ah[i];
  }
  
  return;
}

