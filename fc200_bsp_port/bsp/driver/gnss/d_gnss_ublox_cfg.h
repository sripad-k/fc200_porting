/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : GNSS Ublox Configuration

  Abstract           : Driver for the u-blox F9 GNSS

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-333
                       SDD References: 136T-2200-131100-001-D22 SWDES-296
                                                                SWDES-297
                                                                SWDES-298
                                                                SWDES-299
                                                                SWDES-300
                                                                SWDES-301
\note
  CSC ID             : SWDES-72
*************************************************************************/

#ifndef D_GNSS_UBLOX_CFG_H
#define D_GNSS_UBLOX_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/discrete/d_discrete.h"

/***** Constants ********************************************************/

#define d_GNSS_Ublox_DefineMessageCount const Uint32_t d_GNSS_Ublox_MessageCount = (sizeof(d_GNSS_Ublox_Messages) / sizeof(d_GNSS_Ublox_Message_t))

/* Allow communication on both UART channels */
#define d_GNSS_UART_CHANNEL_COUNT  2u

/***** Type Definitions *************************************************/

typedef struct
{
  Uint8_t class;
  Uint8_t id;
  Uint32_t period;
} d_GNSS_Ublox_Message_t;

/* 1ppps call back function type definition */
typedef void (*d_GNSS_Ublox_1pps_t)(void);

/* Raw GNSS message call back function type definition */
typedef void (*d_GNSS_Ublox_RawReceived_t)(const Uint8_t * const pData, const Uint32_t messageLength);

/***** Variables ********************************************************/

/* UART channels. Enter zero for second channel if not available. */
extern const Uint32_t d_GNSS_Ublox_UartChannel[d_GNSS_UART_CHANNEL_COUNT];

extern d_GNSS_Ublox_t * const pD_GNSS_Ublox;

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_GNSS_Ublox_Message_t d_GNSS_Ublox_Messages[];

extern const Uint32_t d_GNSS_Ublox_MessageCount;

/* Discrete settings */
extern const d_DISC_IO_t d_GNSS_Ublox_DiscretePower;
extern const d_DISC_IO_t d_GNSS_Ublox_DiscreteReset;

/* GNSS 1pps interrupt number */
extern const Uint32_t d_GNSS_Ublox_Interrupt;

/* Function to be executed on receipt of 1pps interrupt from GNSS */
extern const d_GNSS_Ublox_1pps_t d_GNSS_Ublox_1pps;

/* Function to be executed on receipt of raw GNSS messages */
extern const d_GNSS_Ublox_RawReceived_t d_GNSS_Ublox_RawReceived;

/***** Function Declarations ********************************************/

#endif /* D_GNSS_UBLOX_CFG_H */
