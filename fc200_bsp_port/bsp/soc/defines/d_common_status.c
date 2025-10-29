/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Function Status Return Values

  Abstract           : Function return values as strings

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-181
                       SDD References: 136T-2200-131000-001-D22 SWDES-85
\note
  CSCI_ID            : SWDES-12
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"

/***** Type Definitions *************************************************/

/* Structure of entries in status message table */
typedef struct
{
  const Char_t * const message;  /* Pointer to the message */
  const Uint32_t len;            /* Length of the message */
} StatusEntry_t;

/***** Macros ***********************************************************/

/* Macro to define one entry in status message table */
#define MESSAGE_ENTRY(entry) {(entry), sizeof(entry) - 1u}

/***** Constants ********************************************************/

// cppcheck-suppress misra-c2012-8.9; A large constant array should not be placed on the stack. Violation of 'Advisory' rule does not present a risk
static const StatusEntry_t STATUS_ENTRY[] =
{
  MESSAGE_ENTRY("Success"),                        /* d_STATUS_SUCCESS */
  MESSAGE_ENTRY("Failure"),                        /* d_STATUS_FAILURE */
  MESSAGE_ENTRY("Invalid parameter"),              /* d_STATUS_INVALID_PARAMETER */
  MESSAGE_ENTRY("Invalid mode"),                   /* d_STATUS_INVALID_MODE */
  MESSAGE_ENTRY("Hardware error"),                 /* d_STATUS_HARDWARE_ERROR */
  MESSAGE_ENTRY("Not initialised"),                /* d_STATUS_NOT_INITIALISED */
  MESSAGE_ENTRY("Not ready"),                      /* d_STATUS_NOT_READY */
  MESSAGE_ENTRY("Timeout"),                        /* d_STATUS_TIMEOUT */
  MESSAGE_ENTRY("Overrun"),                        /* d_STATUS_OVERRUN */
  MESSAGE_ENTRY("Device not found"),               /* d_STATUS_DEVIDE_NOT_FOUND */
  MESSAGE_ENTRY("Device error"),                   /* d_STATUS_DEVICE_ERROR */
  MESSAGE_ENTRY("Device not ready"),               /* d_STATUS_DEVICE_NOT_READY */
  MESSAGE_ENTRY("Device busy"),                    /* d_STATUS_DEVICE_BUSY */
  MESSAGE_ENTRY("Stack error"),                    /* d_STATUS_STACK_ERROR */
  MESSAGE_ENTRY("Invalid"),                        /* d_STATUS_INVALID */
  MESSAGE_ENTRY("BIT failure"),                    /* d_STATUS_BIT_FAILURE */
  MESSAGE_ENTRY("Bad data"),                       /* d_STATUS_BAD_DATA */
  MESSAGE_ENTRY("Limit exceeded"),                 /* d_STATUS_LIMIT_EXCEEDED */
  MESSAGE_ENTRY("Incompatible"),                   /* d_STATUS_INCOMPATIBLE */
  MESSAGE_ENTRY("Undefined instruction"),          /* d_STATUS_UNDEFINED_INSTRUCTION */
  MESSAGE_ENTRY("Undefined service exception"),    /* d_STATUS_UNDEFINED_SERVICE_EXCEPTION */
  MESSAGE_ENTRY("Execute from invalid address"),   /* d_STATUS_EXEC_FROM_UNDEFINED_ADDRESS */
  MESSAGE_ENTRY("Data with invalid address"),      /* d_STATUS_DATA_FROM_UNDEFINED_ADDRESS */
  MESSAGE_ENTRY("IRQ without handler"),            /* d_STATUS_IRQ_WITHOUT_HANDLER */
  MESSAGE_ENTRY("FIQ without handler"),            /* d_STATUS_FIQ_WITHOUT_HANDLER */
  MESSAGE_ENTRY("Insufficient memory"),            /* d_STATUS_INSUFFICIENT_MEMORY */
  MESSAGE_ENTRY("Buffer full"),                    /* d_STATUS_BUFFER_FULL */
  MESSAGE_ENTRY("Buffer empty"),                   /* d_STATUS_BUFFER_EMPTY */
  MESSAGE_ENTRY("Sync exception"),                 /* d_STATUS_SYNC_EXCEPTION */
  MESSAGE_ENTRY("Error exception"),                /* d_STATUS_ERROR_EXCEPTION */
  MESSAGE_ENTRY("No data"),                        /* d_STATUS_NO_DATA */
  MESSAGE_ENTRY("Invalid configuration"),          /* d_STATUS_INVALID_CONFIGURATION */
  MESSAGE_ENTRY("Write protected"),                /* d_STATUS_WRITE_PROTECTED */
  MESSAGE_ENTRY("IOCA Offline"),                   /* d_STATUS_OFFLINE_IOCA */
  MESSAGE_ENTRY("IOCB Offline"),                   /* d_STATUS_OFFLINE_IOCB */
  MESSAGE_ENTRY("IOCC Offline"),                   /* d_STATUS_OFFLINE_IOCC */
};

#define STATUS_COUNT (sizeof(STATUS_ENTRY) / sizeof(StatusEntry_t))

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_StatusMessage -->
  
  Retrieve the string description of a status message, and length
*************************************************************************/
const Char_t *                    /** \return Pointer to message string associated with message number */
d_StatusMessage
(
const Int32_t value,              /**< [in] Status message number */
Uint32_t * const pMessageLength   /**< [out] Message length */
)
{
  const Char_t * retval = NULL;

  if ((-value >= 0) && (-value < STATUS_COUNT))
  {
    retval = STATUS_ENTRY[-value].message;
    if (pMessageLength != NULL)
    {
      *pMessageLength = STATUS_ENTRY[-value].len;
    }
    else
    {
      DO_NOTHING();
    }
  }
  else
  {
    DO_NOTHING();
  }

  return retval;
}
