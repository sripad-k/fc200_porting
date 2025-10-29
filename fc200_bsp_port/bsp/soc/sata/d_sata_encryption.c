/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Timer

  Abstract           : Module providing basic timing functions

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-762
                       SDD References: 136T-2200-131000-001-D22 SWDES-2698
\note
  CSC ID             : SWDES-48
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"

#include "d_sata_encryption.h"

/***** Macros ***********************************************************/

/** change the "endianess" of a 32bit field */
#define SWAP32(x) ((Uint32_t) (((x) & 0x000000ffu) << 24u) | (((x) & 0x0000ff00u) << 8u) \
	                        | (((x) & 0x00ff0000u) >> 8u) | (((x) & 0xff000000u) >> 24u))

#define AppendToBuffer(buffer, i, x)  \
    do { \
        (buffer)[(i)] = (x); \
        (i)++; \
    } while (0)

                    

/***** Constants ********************************************************/

/** User IDs used in the TCG storage SSCs */
static const Uint8_t OPALUID[][8u] = {
	// users
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff}, /**< session management  */
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }, /**< special "thisSP" syntax */
	{ 0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x01 }, /**< Administrative SP */
	{ 0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x02 }, /**< Locking SP */
	{ 0x00, 0x00, 0x02, 0x05, 0x00, 0x01, 0x00, 0x01 }, /**< ENTERPRISE Locking SP  */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01 }, /**<anybody */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x06 }, /**< SID */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x02 }, /**< ADMINS */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01 }, /**< ADMIN1 */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x03, 0x00, 0x01 }, /**< USER1 */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x03, 0x00, 0x02 }, /**< USER2 */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x01, 0xff, 0x01 }, /**< PSID user */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x80, 0x01 }, /**< BandMaster 0 */
	{ 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x84, 0x01 }, /**< EraseMaster */
	// tables
	{ 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x01 }, /**< Locking_GlobalRange */
	{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE0, 0x01 }, /**< ACE_Locking_Range_Set_RdLocked UID */
	{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE8, 0x01 }, /**< ACE_Locking_Range_Set_WrLocked UID */
	{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE0, 0x00 }, /**< ACE_Locking_Range_GLOBAL_Set_RdLocked UID */
	{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE8, 0x00 }, /**< ACE_Locking_Range_GLOBAL_Set_WrLocked UID */
	{ 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xF8, 0x01 }, /**< ACE_MBRControl_Set_DoneToDOR */
	{ 0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x00, 0x01 }, /**< MBR Control */
  { 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00 }, /**< Shadow MBR */
  { 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00}, /**< AUTHORITY_TABLE */
  { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00}, /**< C_PIN_TABLE */
  { 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01 }, /**< OPAL Locking Info */
  { 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00 }, /**< Enterprise Locking Info */
  //C_PIN_TABLE object ID's
  { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x84, 0x02}, /**< C_PIN_MSID */
  { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01}, /**< C_PIN_SID */
  { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01}, /**< C_PIN_ADMIN1 */
  //half UID's (only first 4 bytes used)
  { 0x00, 0x00, 0x0C, 0x05, 0xff, 0xff, 0xff, 0xff }, /** Half-UID – Authority_object_ref */
  { 0x00, 0x00, 0x04, 0x0E, 0xff, 0xff, 0xff, 0xff }, /** Half-UID – Boolean ACE */
  // special value for omitted optional parameter
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, /**< HEXFF for omitted */
};

/** TCG Storage SSC Methods.
 */
static const Uint8_t OPALMETHOD[][8u] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01}, /**< Properties */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02}, /**<STARTSESSION */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x02}, /**< Revert */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x03}, /**< Activate */
	{ 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06 }, /**< Enterprise Get */
	{ 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07 }, /**< Enterprise Set */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08}, /**< NEXT */
	{ 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c }, /**< Enterprise Authenticate */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0d}, /**< GetACL */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x10}, /**< GenKey */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x11}, /**< revertSP */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x16}, /**<Get */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x17}, /**< Set */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1c}, /**< Authenticate */
    { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x01}, /**< Random */
	{ 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x08, 0x03 }, /**< Erase */
};

/***** Type Definitions *************************************************/

typedef enum {
    //Boolean
	OPAL_TRUE = 0x01,
	OPAL_FALSE = 0x00,
	OPAL_BOOLEAN_EXPR = 0x03,
	// cellblocks
	TABLE = 0x00,
	STARTROW = 0x01,
	ENDROW = 0x02,
	STARTCOLUMN = 0x03,
	ENDCOLUMN = 0x04,
	VALUES = 0x01,
	// authority table
	PIN = 0x03,
	// locking tokens
	RANGESTART = 0x03,
	RANGELENGTH = 0x04,
	READLOCKENABLED = 0x05,
	WRITELOCKENABLED = 0x06,
	READLOCKED = 0x07,
	WRITELOCKED = 0x08,
	ACTIVEKEY = 0x0A,
	//locking info table
	MAXRANGES = 0x04,
  // mbr control
  MBRENABLE = 0x01,
  MBRDONE = 0x02,
	// properties
	HOSTPROPERTIES =0x00,
  // response tokenis() returned values
  DTA_TOKENID_BYTESTRING = 0xe0,
  DTA_TOKENID_SINT = 0xe1,
  DTA_TOKENID_UINT = 0xe2,
  DTA_TOKENID_TOKEN = 0xe3, // actual token is returned
  // atoms
  STARTLIST = 0xf0,
  ENDLIST = 0xf1,
  STARTNAME = 0xf2,
  ENDNAME = 0xf3,
  CALL = 0xf8,
  ENDOFDATA = 0xf9,
  ENDOFSESSION = 0xfa,
  STARTTRANSACTON = 0xfb,
  ENDTRANSACTON = 0xfC,
  EMPTYATOM = 0xff,
  WHERE = 0x00,
} OPAL_TOKEN;

typedef enum {
    UINT_3 = 0x83u,
    BYTESTRING4 = 0xa4u,
    BYTESTRING8 = 0xa8u,
} OPAL_SHORT_ATOM;



typedef enum {
  PROPERTIES,
  STARTSESSION,
  REVERT,
  ACTIVATE,
	EGET,
	ESET,
  NEXT,
  EAUTHENTICATE,
  GETACL,
  GENKEY,
  REVERTSP,
  GET,
  SET,
  AUTHENTICATE,
  RANDOM,
	ERASE,
} OPAL_METHOD;

typedef struct {
    Uint32_t reserved0;
    Uint8_t extendedComID[4];
    Uint32_t outstandingData;
    Uint32_t minTransfer;
    Uint32_t length;
} OPALComPacket;

/** Packet structure. */
typedef struct {
    Uint32_t TSN;
    Uint32_t HSN;
    Uint32_t seqNumber;
    Uint16_t reserved0;
    Uint16_t ackType;
    Uint32_t acknowledgement;
    Uint32_t length;
} OPALPacket;

/** Data sub packet header */
typedef struct {
    Uint8_t reserved0[6];
    Uint16_t kind;
    Uint32_t length;
} OPALDataSubPacket;

typedef struct {
    OPALComPacket cp;
    OPALPacket pkt;
    OPALDataSubPacket subpkt;
} OPALHeader;



/***** Variables ********************************************************/

static Uint32_t hostSessionNumber = 0u;
static Uint32_t TPerSessionNumber = 0u;

/***** Function Declarations ********************************************/  

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- addTokenU64 -->

  Functions adds a number token to the payload using the OPAL format

*************************************************************************/
static Uint32_t             /** \return the amount of bytes added to the buffer */
addTokenU64
(Uint8_t * pBuffer,         /**< [out] pointer to the buffer where the token should be added */
const Uint32_t bufferSpace, /**< [in] Total available space in the buffer */
const Uint64_t number)      /**< [in] Number to add */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSpace < 8u) // make sure we have enough buffer space, we will at max add 8 characters
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  Uint32_t index = 0u;
  if (number < 64u) 
  {
    AppendToBuffer(pBuffer, index, (Uint8_t) number & 0x000000000000003fu);
  }
  else 
  {
    Uint32_t iterations;
    if (number < 0x100u) 
    {
      AppendToBuffer(pBuffer, index, 0x81u);
      iterations = 1u;
    }
    else if (number < 0x10000u) 
    {
      AppendToBuffer(pBuffer, index, 0x82u);
      iterations = 2u;
    }
    else if (number < 0x100000000u) 
    {
      AppendToBuffer(pBuffer, index, 0x84u);
      iterations = 4u;
    }
    else 
    {
      AppendToBuffer(pBuffer, index, 0x88u);
      iterations = 8u;
    }

    for (Uint32_t i = iterations; i > 0u; i--)
    {
      Uint64_t x = (number >> (((Uint64_t)i - 1u) * 8u)) & (Uint64_t)0xffu;
      AppendToBuffer(pBuffer, index, (Uint8_t)x);
    }

  }

  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_GetTokenFromResponse -->

  Functions gets the requested token from the response string

*************************************************************************/
Uint32_t                              /** \return the token value */
d_SATAENC_GetTokenFromResponse
(const Uint8_t * const pBuffer,       /**< [in] Pointer to the buffer of the received string */
const Uint32_t bufferSize,            /**< [in] the size of the buffer */
const Uint32_t tokenNumber)           /**< [in] the token number or identifier */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < sizeof(OPALHeader))
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  // First parse the response
  Uint64_t tokenArray[32u];
  Uint32_t index = 0u;
  OPALHeader h;
  const Uint8_t * reply = (const Uint8_t * const) pBuffer;
  Uint32_t cpos = 0;
  Uint32_t tokenLength;
  
  d_GEN_MemoryCopy((Uint8_t*)&h, pBuffer, sizeof (OPALHeader));
  for (Uint32_t i = 0; i < 32u; i++)
  {
    tokenArray[i] = 0;
  }
  
  // cppcheck-suppress misra-c2012-18.4;  Operation on Uint8_t pointer. Violation of 'Advisory' rule does not present a risk.
  reply += sizeof(OPALHeader);
  while (cpos < SWAP32(h.subpkt.length)) 
  {      
    if (!(reply[cpos] & 0x80u)) //tiny atom
    {
      tokenLength = 1u;
      AppendToBuffer(tokenArray, index, (Uint64_t)(reply[cpos] & 0x3fu));
    }
    else if (!(reply[cpos] & 0x40u)) // short atom
    {
      tokenLength = (reply[cpos] & 0x0fu) + 1u;
      Uint64_t whatever = 0u;
      Uint32_t b = 0u;
      for (Uint32_t i = tokenLength - 1u; i > 0u; i--) 
      {
        Uint64_t update = (Uint64_t)reply[cpos + i] << (8u * (Uint64_t)b);
        whatever |= update;
        b++;
      }
      AppendToBuffer(tokenArray, index, whatever);
    }
    else if (!(reply[cpos] & 0x20u)) // medium atom
    {
      tokenLength = (((reply[cpos] & 0x07u) << 8u) | reply[cpos + 1u]) + 2u;
      // Not supported
      AppendToBuffer(tokenArray, index, 0u);
    }
    else if (!(reply[cpos] & 0x10u)) // long atom
    {
      // not supported
      tokenLength = ((reply[cpos + 1u] << 16u) | (reply[cpos + 2u] << 8u) | reply[cpos + 3u]) + 4u;
      AppendToBuffer(tokenArray, index, 0u);
    }
    else // TOKEN
    {
      // not supported
      tokenLength = 1u;
      AppendToBuffer(tokenArray, index, 0u);
    }

    cpos += tokenLength;
  }

  return (Uint8_t)(tokenArray[tokenNumber]);
}

/*********************************************************************//**
  <!-- d_SATAENC_IsOperationComplete -->

  Functions checks the flags in the response header to verify if the last operation is complete

*************************************************************************/
Bool_t                              /** \return True if the operation is complete */
d_SATAENC_IsOperationComplete
(const Uint8_t * const pBuffer,     /**< [in] Pointer to the receive buffer  */
const Uint32_t bufferSize)          /**< [in] The size of the buffer */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_FALSE;
  }

  if (bufferSize < sizeof(OPALHeader))
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_FALSE;
  }

  Bool_t returnValue = d_TRUE;
  OPALHeader hdr;
  d_GEN_MemoryCopy((Uint8_t*)&hdr, pBuffer, sizeof(hdr));
  if ((0u != hdr.cp.outstandingData) && (0u == hdr.cp.minTransfer))
  {
    returnValue = d_FALSE;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATAENC_UpdateSessionNumbersFromResponse -->

  Functions updates the session numbers from the response received

*************************************************************************/
void                                          /** \return None */
d_SATAENC_UpdateSessionNumbersFromResponse
(const Uint8_t * const pBuffer,               /**< [in] Pointer to the receiving buffer */
const Uint32_t bufferSize)                    /**< [in] Size of the receiving buffer */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  hostSessionNumber = SWAP32(d_SATAENC_GetTokenFromResponse(pBuffer, bufferSize, 4u));
  TPerSessionNumber = SWAP32(d_SATAENC_GetTokenFromResponse(pBuffer, bufferSize, 5u));
}

/*********************************************************************//**
  <!-- d_SATAENC_Complete -->

  Completes the message by filling the End of Data and populating the header

*************************************************************************/
static Uint32_t               /** \return The number of bytes added to the payload buffer */
d_SATAENC_Complete
(Uint8_t * pBuffer,           /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize,    /**< [in] Size of the payload buffer */
Bool_t EODflag)               /**< [in] flag to indicate that we should append the End Of Data tokens */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < 9u) // we will be added max 9 bytes, make sure there is enough space
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  Uint32_t index = 0u;

  if (EODflag == d_TRUE)
  {
    AppendToBuffer(pBuffer, index, ENDOFDATA);

    AppendToBuffer(pBuffer, index, STARTLIST); 
    AppendToBuffer(pBuffer, index, 0u);
    AppendToBuffer(pBuffer, index, 0u);
    AppendToBuffer(pBuffer, index, 0u);
    AppendToBuffer(pBuffer, index, ENDLIST);
  }

  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_PopulateHeader -->

  Completes the message by filling the End of Data and populating the header

*************************************************************************/
static Uint32_t               /** \return The number of bytes added to the payload buffer */
d_SATAENC_PopulateHeader
(Uint8_t * pBuffer,           /**< [out] Pointer to the START of the payload buffer */
const Uint32_t bufferSize,    /**< [in] Size of the payload buffer */
const Uint32_t packetSize)    /**< [in] The amount of bytes already packed into the payload so we can update the header */
{
  if (bufferSize < sizeof(OPALHeader)) // make sure there is enough space for the header
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  OPALHeader hdr;
  d_GEN_MemorySet((Uint8_t*)&hdr, 0, sizeof(hdr));
  hdr.subpkt.length = SWAP32(packetSize - (sizeof (OPALHeader)));
  Uint32_t index = packetSize;
  while ((index % 4u) != 0u) 
  {
    AppendToBuffer(pBuffer, index, 0x00);
  }

  hdr.pkt.length = SWAP32((index - sizeof (OPALComPacket)) - sizeof (OPALPacket));
  hdr.cp.length = SWAP32(index - sizeof (OPALComPacket));
  hdr.pkt.HSN = hostSessionNumber; 
  hdr.pkt.TSN = TPerSessionNumber; 
  const Uint16_t COM_ID = 0x1005u;
  hdr.cp.extendedComID[0] = (Uint8_t)((COM_ID & 0xff00u) >> 8u);
  hdr.cp.extendedComID[1] = (Uint8_t)(COM_ID & 0x00ffu);
  hdr.cp.extendedComID[2] = 0x00u;
  hdr.cp.extendedComID[3] = 0x00u;
  hdr.subpkt.reserved0[0] = 0x00u;
  hdr.subpkt.kind = 0u;

  d_GEN_MemoryCopy(&pBuffer[0], (Uint8_t*)&hdr, sizeof(hdr));

  return (index - packetSize);
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildSetTablePayload -->

  Builds the payload for the OPAL Set Table command

*************************************************************************/
Uint32_t                              /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildSetTablePayload
(Uint8_t * pBuffer,                   /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize,            /**< [in] Size of the payload buffer */
const Char_t * const password,        /**< [in] Password to be used */
const Uint32_t pwdLength)             /**< [in] String length of the password */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 40u + pwdLength)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (password == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  d_GEN_MemorySet(pBuffer, 0, bufferSize);

  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  AppendToBuffer(pBuffer, index, CALL);

  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[OPAL_C_PIN_SID][0u], sizeof(OPALUID[OPAL_C_PIN_SID]));
  index += sizeof(OPALUID[OPAL_C_PIN_SID]); 

  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALMETHOD[SET][0u], sizeof(OPALMETHOD[SET]));
  index += sizeof(OPALMETHOD[SET]);


  AppendToBuffer(pBuffer, index, STARTLIST); 
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, VALUES);
  AppendToBuffer(pBuffer, index, STARTLIST); 
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, PIN);   // name

  // Send raw password
  AppendToBuffer(pBuffer, index, 0xd0u);
  AppendToBuffer(pBuffer, index, pwdLength);
  d_GEN_MemoryCopy(&pBuffer[index], (const Uint8_t* const)password, pwdLength);
  index += pwdLength;

  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_TRUE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildSetLockRangePayload -->

  Builds the payload for the OPAL Set locking range 0
  This will either lock or unlock the entire device 

*************************************************************************/
Uint32_t                            /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildSetLockRangePayload
(Uint8_t * pBuffer,                 /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize,          /**< [in] Size of the payload buffer */
Bool_t lock)                        /**< [in] Flag to indicate locking state */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 34u)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  OPAL_TOKEN lockingState = OPAL_FALSE;
  if (lock == d_TRUE)
  {
    lockingState = OPAL_TRUE;    
  }

  d_GEN_MemorySet(pBuffer, 0, bufferSize);

  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  AppendToBuffer(pBuffer, index, CALL);

  // invoker ID
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[OPAL_LOCKINGRANGE_GLOBAL][0u], sizeof(OPALUID[0]));
  index += sizeof(OPALUID[0]); 

  // Opal method
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALMETHOD[SET][0u], sizeof(OPALMETHOD[0]));
  index += sizeof(OPALMETHOD[0]);

  AppendToBuffer(pBuffer, index, STARTLIST);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, VALUES);
  AppendToBuffer(pBuffer, index, STARTLIST);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, READLOCKED);
  AppendToBuffer(pBuffer, index, lockingState);
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, WRITELOCKED);
  AppendToBuffer(pBuffer, index, lockingState);
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_TRUE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildRevertLockingSPPayload -->

  Builds the payload for the OPAL Revert Locking SP message
  Reset the locking SP without wiping the data 

*************************************************************************/
Uint32_t                                /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildRevertLockingSPPayload
(Uint8_t * pBuffer,                 /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize)          /**< [in] Size of the payload buffer */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 28u)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  d_GEN_MemorySet(pBuffer, 0, bufferSize);
  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  AppendToBuffer(pBuffer, index, CALL);

  // invoker ID
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[OPAL_THISSP_UID][0u], sizeof(OPALUID[0]));
  index += sizeof(OPALUID[0]); 

  // Opal method
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALMETHOD[REVERTSP][0u], sizeof(OPALMETHOD[0]));
  index += sizeof(OPALMETHOD[0]);

  AppendToBuffer(pBuffer, index, STARTLIST);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, 0x83u);      // Keep global locking??
  AppendToBuffer(pBuffer, index, 0x06u);
  AppendToBuffer(pBuffer, index, 0x00u);
  AppendToBuffer(pBuffer, index, 0x00u); 
  AppendToBuffer(pBuffer, index, OPAL_TRUE);
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_TRUE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildLockingRangePayload -->

  Builds the payload for the OPAL Enable/ Disable locking range 0
  This will enable locking for the entire device 

*************************************************************************/
Uint32_t                            /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildLockingRangePayload    
(Uint8_t * pBuffer,                 /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize,          /**< [in] Size of the payload buffer */
const Bool_t lockingEnabled)        /**< [in] Flag to indicate locking state */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 34u)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  d_GEN_MemorySet(pBuffer, 0, bufferSize);

  OPAL_TOKEN lockingState = OPAL_FALSE;
  if (lockingEnabled == d_TRUE)
  {
    lockingState = OPAL_TRUE;    
  }

  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  AppendToBuffer(pBuffer, index, CALL);

  // invoker ID
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[OPAL_LOCKINGRANGE_GLOBAL][0u], sizeof(OPALUID[0]));
  index += sizeof(OPALUID[0]); 

  // Opal method
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALMETHOD[SET][0u], sizeof(OPALMETHOD[0]));
  index += sizeof(OPALMETHOD[0]);

  AppendToBuffer(pBuffer, index, STARTLIST);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, VALUES);
  AppendToBuffer(pBuffer, index, STARTLIST);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, READLOCKENABLED);
  AppendToBuffer(pBuffer, index, lockingState);     // read locking
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, STARTNAME);
  AppendToBuffer(pBuffer, index, WRITELOCKENABLED);
  AppendToBuffer(pBuffer, index, lockingState);     // write locking
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);
  AppendToBuffer(pBuffer, index, ENDNAME);
  AppendToBuffer(pBuffer, index, ENDLIST);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_TRUE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildActivateLockingSPPayload -->

  Builds the payload for the OPAL Activate Locking SP message

*************************************************************************/
Uint32_t                                  /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildActivateLockingSPPayload
(Uint8_t * pBuffer,                       /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize)                /**< [in] Size of the payload buffer */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 21u)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  d_GEN_MemorySet(pBuffer, 0, bufferSize);

  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  AppendToBuffer(pBuffer, index, CALL);

  // invoker ID
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[OPAL_LOCKINGSP_UID][0u], sizeof(OPALUID[OPAL_LOCKINGSP_UID]));
  index += sizeof(OPALUID[OPAL_LOCKINGSP_UID]); 

  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALMETHOD[ACTIVATE][0u], sizeof(OPALMETHOD[ACTIVATE]));
  index += sizeof(OPALMETHOD[ACTIVATE]);

  AppendToBuffer(pBuffer, index, STARTLIST);
  AppendToBuffer(pBuffer, index, ENDLIST);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_TRUE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildSessionTerminatePayload -->

  Builds the payload for the OPAL session termination message

*************************************************************************/
Uint32_t                                  /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildSessionTerminatePayload
(Uint8_t * pBuffer,                       /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize)                /**< [in] Size of the payload buffer */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 1u)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  d_GEN_MemorySet(pBuffer, 0, bufferSize);

  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  AppendToBuffer(pBuffer, index, ENDOFSESSION);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_FALSE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

/*********************************************************************//**
  <!-- d_SATAENC_BuildSessionStartPayload -->

  Builds the payload for the OPAL session start message

*************************************************************************/
Uint32_t                                    /** \return The number of bytes added to the payload buffer */
d_SATAENC_BuildSessionStartPayload
(Uint8_t * pBuffer,                       /**< [out] Pointer to the payload buffer */
const Uint32_t bufferSize,                /**< [in] Size of the payload buffer */
const Char_t * const password,            /**< [in] password to be used for the session */
const Uint32_t pwdLength,                 /**< [in] length of a password */
const d_SATAENC_uidPair_t uids)           /**< [in] Uid pair to be used for establishing the session */
{
  if (pBuffer == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (bufferSize < (sizeof(OPALHeader) + 56u + pwdLength)) // make sure there is enough space in the buffer for everything we want to add
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  if (password == NULL)
  {
    // gcov-jst 3 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return 0u;
  }

  // Reset session numbers
  hostSessionNumber = 0u;
  TPerSessionNumber = 0u;

  d_GEN_MemorySet(pBuffer, 0, bufferSize);

  Uint32_t index = sizeof(OPALHeader);  // leave space for OPAL Header (56 bytes)
  
  AppendToBuffer(pBuffer, index, CALL); 
  
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[OPAL_SMUID_UID][0u], sizeof(OPALUID[0u]));
  index += sizeof(OPALUID[0u]); 

  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALMETHOD[STARTSESSION][0u], sizeof(OPALMETHOD[0u]));
  index += sizeof(OPALMETHOD[0u]); 

  AppendToBuffer(pBuffer, index, STARTLIST); // [  (Open Bracket) 
  index += addTokenU64(&pBuffer[index], bufferSize - index, 105u);  // session number  

  // Security Provider
  AppendToBuffer(pBuffer, index, BYTESTRING8);
  d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[uids.secProvider][0u], sizeof(OPALUID[0u])); 
  index += sizeof(OPALUID[0u]);

  AppendToBuffer(pBuffer, index, 1u); // Write 

  if (password != NULL)
  {
    // Send password
    AppendToBuffer(pBuffer, index,  STARTNAME);
    AppendToBuffer(pBuffer, index,  0u);
    AppendToBuffer(pBuffer, index, 0xd0u);
    AppendToBuffer(pBuffer, index, pwdLength);
    d_GEN_MemoryCopy(&pBuffer[index], (const Uint8_t* const)password, pwdLength);
    index += pwdLength;
    AppendToBuffer(pBuffer, index, ENDNAME); 

    // Sign Authority
    AppendToBuffer(pBuffer, index, STARTNAME); 
    AppendToBuffer(pBuffer, index, 3u); 
    AppendToBuffer(pBuffer, index, BYTESTRING8); 
    d_GEN_MemoryCopy(&pBuffer[index], &OPALUID[uids.signAuth][0u], sizeof(OPALUID[0u]));
    index += sizeof(OPALUID[0u]);
    AppendToBuffer(pBuffer, index, ENDNAME);
  }

  AppendToBuffer(pBuffer, index, ENDLIST);

  index += d_SATAENC_Complete(&pBuffer[index], bufferSize - index, d_TRUE);
  index += d_SATAENC_PopulateHeader(pBuffer, bufferSize, index);
  return index;
}

