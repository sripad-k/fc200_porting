#ifndef H_TYPES_RADALT
#define H_TYPES_RADALT


#include "type.h"

/**
 * @brief Radalt Data Abstraction Structure
 *
 */
typedef struct radalt_s
{
    uint8_t  status;   //!< Status Flag 0 -> all good                []
    uint8_t  counter;  //!< Counter for radalt data
    uint64_t time;     //!< Time stamp in milli seconds              [ms]
    bool     flag;     //!< Set to true when new data available      [ ]
    uint16_t stat;     //!< Radalt Bitmasked Status                  [ ]
    float    agl;      //!< Measured height above ground level       [m]
    float    vD;       //!< Measured vertical velocity               [m/s]
    float    snr;      //!< Signal to Noise Ratio                    [dB]
    float    offset;   //!< Antenna Distance to Ground               [m]
} radalt_s;

/**
 * @brief   Ainstein US-D1 message structure
 */
typedef struct us_d1_msg_s
{
    uint8_t header;        //!< Message header
    uint8_t version_id;    //!< Message version ID
    uint8_t altitude_lsb;  //!< Altitude LSB
    uint8_t altitude_msb;  //!< Altitude MSB
    uint8_t snr;           //!< Signal to Noise Ratio
    uint8_t checksum;      //!< Checksum
} us_d1_msg_s;

/**
 * @brief   Ainstein US-D1 message structure
 */
typedef struct radalt_msg_s
{
    uint8_t     flag;       //!< New Message flag
    us_d1_msg_s us_d1_msg;  //!< Ainstein US-D1 Message
} radalt_msg_s;

/**
 * @brief   Radalt decode status flag options
 */
enum
{
    RADALT_DCODE_PENDING        = 10,  // So far, all good; waiting for the things to come
    RADALT_DCODE_FAILED_START   = 11,  // Initalization fail
    RADALT_DCODE_FAILED_VERSION = 12,  // Fail during header scan
    RADALT_DCODE_FAILED_CRC     = 13,  // fail during payload scan
    RADALT_DCODE_FAILED_ERROR   = 14,  // general fail
    RADALT_DCODE_CRC_OK         = 15,  // success
};

#endif