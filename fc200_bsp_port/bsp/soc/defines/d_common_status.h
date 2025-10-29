/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Function Status Return Values

  Abstract           : Function return values as strings

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-181
                       SDD References: 136T-2200-131000-001-D22 SWDES-84
\note
  CSCI_ID            : SWDES-12
*************************************************************************/

#ifndef D_STATUS_H
#define D_STATUS_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"

/***** Literals *********************************************************/

typedef enum
{
  d_STATUS_SUCCESS                       =  0,
  d_STATUS_FAILURE                       = -1,
  d_STATUS_INVALID_PARAMETER             = -2,
  d_STATUS_INVALID_MODE                  = -3,
  d_STATUS_HARDWARE_ERROR                = -4,
  d_STATUS_NOT_INITIALISED               = -5,
  d_STATUS_NOT_READY                     = -6,
  d_STATUS_TIMEOUT                       = -7,
  d_STATUS_OVERRUN                       = -8,
  d_STATUS_DEVICE_NOT_FOUND              = -9,
  d_STATUS_DEVICE_ERROR                  = -10,
  d_STATUS_DEVICE_NOT_READY              = -11,
  d_STATUS_DEVICE_BUSY                   = -12,
  d_STATUS_STACK_ERROR                   = -13,
  d_STATUS_INVALID                       = -14,
  d_STATUS_BIT_FAILURE                   = -15,
  d_STATUS_BAD_DATA                      = -16,
  d_STATUS_LIMIT_EXCEEDED                = -17,
  d_STATUS_INCOMPATIBLE                  = -18,
  d_STATUS_UNDEFINED_INSTRUCTION         = -19,
  d_STATUS_UNDEFINED_SERVICE_EXCEPTION   = -20,
  d_STATUS_EXEC_FROM_UNDEFINED_ADDRESS   = -21,
  d_STATUS_DATA_FROM_UNDEFINED_ADDRESS   = -22,
  d_STATUS_IRQ_WITHOUT_HANDLER           = -23,
  d_STATUS_FIQ_WITHOUT_HANDLER           = -24,
  d_STATUS_INSUFFICIENT_MEMORY           = -25,
  d_STATUS_BUFFER_FULL                   = -26,
  d_STATUS_BUFFER_EMPTY                  = -27,
  d_STATUS_SYNC_EXCEPTION                = -28,
  d_STATUS_ERROR_EXCEPTION               = -29,
  d_STATUS_NO_DATA                       = -30,
  d_STATUS_INVALID_CONFIGURATION         = -31,
  d_STATUS_WRITE_PROTECTED               = -32,
  d_STATUS_OFFLINE_IOCA                  = -33,
  d_STATUS_OFFLINE_IOCB                  = -34,
  d_STATUS_OFFLINE_IOCC                  = -35,
  /* Values -101 to -120 are reserved for the LWIP library. See d_eth_interface.h */
} d_Status_t;


/***** Constants ********************************************************/


/***** Type Definitions *************************************************/


/***** Variables ********************************************************/


/***** Function Declarations ********************************************/

/* Return a pointer to the text representation of the specified status code */
const Char_t * d_StatusMessage(const Int32_t value, Uint32_t * const pMessageLength);


#endif /* D_STATUS_H */

