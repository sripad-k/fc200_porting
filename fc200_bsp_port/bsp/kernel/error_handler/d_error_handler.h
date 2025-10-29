/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Error Handling

  Abstract : Error handling driver

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-183
                                                                SWREQ-184
                                                                SWREQ-185
                       SDD References: 136T-2200-131000-001-D22 SWDES-238
                                                                SWDES-239
                                                                SWDES-240
                                                                SWDES-241
                                                                SWDES-242
                                                                SWDES-243
                                                                SWDES-244
                                                                SWDES-245
                                                                SWDES-246
                                                                SWDES-247
                                                                SWDES-248
                                                                SWDES-249
\note
  CSC ID             : SWDES-74
*************************************************************************/

#ifndef D_ERROR_HANDLER_H
#define D_ERROR_HANDLER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals ********************************************************/

/* #define is used, rather than 'const' as these values are used to define array sizes */
#define d_ERROR_DATA_ITEM_COUNT  4u    /** Number of additional data items in error structure */
#define d_ERROR_PATH_SIZE        40u   /** Size of string to hold module path */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum
{
  d_ERROR_CRITICALITY_UNKNOWN,
  d_ERROR_CRITICALITY_NON_CRITICAL,
  d_ERROR_CRITICALITY_CRITICAL_RESET,
  d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN
} d_ERROR_Criticality_t; /* Define criticality of error */

typedef struct
{
  Char_t  Emodule[d_ERROR_PATH_SIZE];        /** Module path where error occurred */
  Uint32_t Eline;                            /** Error line number */
  d_Status_t Etype;                          /** Error type */
  d_ERROR_Criticality_t Ecriticality;        /** Error criticality */
  Uint32_t Edata[d_ERROR_DATA_ITEM_COUNT];   /** Error additional data */
} d_ERROR_LogData_t; /* Error log data */

/***** Macros (Inline Functions) Definitions ****************************/

/* Macro used to log an error allowing the module name and line number of the error to be determined */
#define d_ERROR_Logger(eType, eCriticality, eData0, eData1, eData2, eData3) \
        d_ERROR_LogRaw(__FILE__,            \
                       __LINE__,            \
                       (eType),             \
                       (eCriticality),      \
                       (eData0),            \
                       (eData1),            \
                       (eData2),            \
                       (eData3))

#ifdef ENABLE_DIAGNOSTIC_FUNCTION_ENTRY_LOGGING

#define d_LOG_FUNCTION_ENTRY() \
  do { d_ERROR_LogRaw(__FILE__, __LINE__, d_STATUS_SUCCESS, d_ERROR_CRITICALITY_NON_CRITICAL, 0u, 0u, 0u, 0u); } while (0)

#else

#define d_LOG_FUNCTION_ENTRY() \
  do {} while (0)

#endif

#ifdef ENABLE_DIAGNOSTIC_FUNCTION_EXIT_LOGGING

#define d_LOG_FUNCTION_EXIT(returnValue) \
  do { d_ERROR_LogRaw(__FILE__, __LINE__, d_STATUS_SUCCESS, d_ERROR_CRITICALITY_NON_CRITICAL, returnValue, 1u, 1u, 1u); } while (0)

#else

#define d_LOG_FUNCTION_EXIT(returnValue) \
  do {} while (0)

#endif

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the error handling CSC */
d_Status_t d_ERROR_Initialise(void);

/* Log an error using the log data structure */
void d_ERROR_Log(d_ERROR_LogData_t * const errorData);

/* Log an error using the component parts of the log data */
void d_ERROR_LogRaw(const Char_t * const eModule,               /** Error module path string */
                    const Uint32_t eLine,                       /** Error line number */
                    const d_Status_t eType,                     /** Error type */
                    const d_ERROR_Criticality_t eCriticality,   /** Error criticality */
                    const Uint32_t eData0,                      /** Error additional data 0 */
                    const Uint32_t eData1,                      /** Error additional data 1 */
                    const Uint32_t eData2,                      /** Error additional data 2 */
                    const Uint32_t eData3);                     /** Error additional data 3 */

d_ERROR_Criticality_t d_ERROR_Criticality(d_ERROR_LogData_t * errorData);

#endif /* D_ERROR_HANDLER_H */
