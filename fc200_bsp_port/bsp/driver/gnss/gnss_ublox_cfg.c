/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : GNSS Ublox Configuration

  Abstract           : This is the GNSS CSC configuration.
                       The file should be copied to the application code
                       space and updated for the required parameters.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "sru/fcu/d_fcu.h"
#include "driver/gnss/d_gnss_ublox.h"
#include "driver/gnss/d_gnss_ublox_cfg.h"

#include "xparameters.h"

/***** Variables ********************************************************/

static d_GNSS_Ublox_t  GNSS_Ublox;

/***** Constants ********************************************************/

__attribute__((weak)) const Uint32_t d_GNSS_Ublox_UartChannel[d_GNSS_UART_CHANNEL_COUNT] = {6u, 0u};

__attribute__((weak)) d_GNSS_Ublox_t * const pD_GNSS_Ublox  = &GNSS_Ublox;

__attribute__((weak)) const d_GNSS_Ublox_Message_t d_GNSS_Ublox_Messages[] = 
{
  /*UBX-MON-HW */
  {0x0Au, 0x09u, 1000u},
  /*UBX-TIM_-TP */
  {0x0Du, 0x01u, 1000u},
  /*UBX-NAV-PVT */
  {0x01u, 0x07u, 300u},
  /*UBX-NAV-COV */
  {0x01u, 0x36u, 300u},
  /*UBX-NAV-DOP */
  {0x01u, 0x04u, 300u},
  /*UBX-NAV-STATUS */
  {0x01u, 0x03u, 300u},
  /*UBX-NAV-VELECEF */
  {0x01u, 0x11u, 300u},
  /*UBX-MON-SYS */
  {0x0Au, 0x39u, 1000u},
  /*UBX-SEC-SIG */
  {0x27u, 0x09u, 1000u}
};

/* Define the number of expected messages */
__attribute__((weak)) d_GNSS_Ublox_DefineMessageCount;

/* Power control discrete */
__attribute__((weak)) const d_DISC_IO_t d_GNSS_Ublox_DiscretePower = d_FCU_GNSS_PWR_ON;

/* Reset discrete */
__attribute__((weak)) const d_DISC_IO_t d_GNSS_Ublox_DiscreteReset = d_FCU_GNSS_RESET;

__attribute__((weak)) const Uint32_t d_GNSS_Ublox_Interrupt = 0u;

__attribute__((weak)) const d_GNSS_Ublox_1pps_t d_GNSS_Ublox_1pps = NULL;

__attribute__((weak)) const d_GNSS_Ublox_RawReceived_t d_GNSS_Ublox_RawReceived = NULL;

/***** Type Definitions *************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

