/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Buffer definition

  Abstract           : This is the Buffer CSC configuration.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-145
                       SDD References: 136T-2200-131100-001-D22 SWDES-200
                                                                SWDES-201
                                                                SWDES-202
                                                                SWDES-203
                                                                SWDES-204
                                                                SWDES-205
                                                                SWDES-206
                                                                SWDES-207
                                                                SWDES-208
                                                                SWDES-209

\note
  CSC ID             : SWDES-80
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "kernel/buffer/d_buffer_cfg.h"
#include "kernel/event_logger/d_event_logger.h"
#include "sru/ethernet/d_eth_interface.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/* Note that there are two entries below for each buffer, firstly a macro to define the required storage
   and secondly a macro to generate the configuration table. Also note that an entry for each buffer
   should be inserted in the buffer definition enum in the associated header file. */

/* Fixed item length buffers ********************************************/   

/* Define buffers for fixed length items */
/* Parameters for the definition macro are:
   name       any arbitrary name unique to the fixed item length buffers
   entrySize  the size in bytes of each entry
   entries    the maximum number of entries that may be stored */

// cppcheck-suppress misra-c2012-8.2; CppCheck erroneously reports this violation assuming the following line is a function definition.
d_BUFFER_FIXED(events, sizeof(d_EVENT_LogEvent_t), 20);
d_BUFFER_FIXED(bytes, sizeof(Uint8_t), 1000);
d_BUFFER_FIXED(words, sizeof(Uint32_t), 100);

/* Create table of buffer configuration data */
/* Note that enums should be provided in the associated header file to access the buffers.
   The order of the enumeration items should match the order of the following definitions */
/* The first entry is used to buffer logging events and should notn be moved */
/* The macro parameter is the name as used in the above definition */

__attribute__((weak)) const d_BUFFER_FixedDef_t d_BUFFER_FixedDef[] =
{
  d_BUFFER_FIXED_ENTRY(events),
  d_BUFFER_FIXED_ENTRY(bytes),
  d_BUFFER_FIXED_ENTRY(words),
};

/* Define the number of fixed item length bufers */
__attribute__((weak)) d_BUFFER_FIXED_COUNT;

/* Variable item length buffers ********************************************/   

/* Define buffers for variable length items */
/* Parameters for the definition macro are:
   name       any arbitrary name unique to the variable item length buffers
   totalSize  the total buffer size in bytes
   entries    the maximum number of entries that may be stored */

// cppcheck-suppress misra-c2012-8.2; CppCheck erroneously reports this violation assuming the following line is a function definition.
d_BUFFER_VAR(msg1, 500, 5);
d_BUFFER_VAR(msg2, 100, 10);

/* Create table of buffer configuration data */
/* Note that enums should be provided in the associated header file to access the buffers.
   The order of the enumeration items should match the order of the following definitions */
/* The macro parameter is the name as used in the above definition */

__attribute__((weak)) const d_BUFFER_VarDef_t d_BUFFER_VarDef[] =
{
  d_BUFFER_VAR_ENTRY(msg1),
  d_BUFFER_VAR_ENTRY(msg2)
};

/* Define the number of variable item length buffers */
__attribute__((weak)) d_BUFFER_VAR_COUNT;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

