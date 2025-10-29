/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_event_logger

  Abstract           : Event Logger driver module

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-72
                                                                SWREQ-73
                       SDD References: 136T-2200-131000-001-D22 SWDES-221
                                                                SWDES-222
                                                                SWDES-223
                                                                SWDES-224
                                                                SWDES-226
                                                                SWDES-227
                                                                SWDES-228
                                                                SWDES-229
                                                                SWDES-230
                                                                SWDES-231
\note
  CSC ID             : SWDES-77
*************************************************************************/

#ifndef D_EVENT_LOGGER_H
#define D_EVENT_LOGGER_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals ********************************************************/

/* #define is used, rather than 'const' as these values are used to define array sizes */
#define d_EVENT_MAX_EVENT_ENTRIES       5000u    /** Number of events that may be logged to the MMC */
#define TEXT_FIELD_SIZE 128u

/* Use when reading the last logged event */
#define LAST_LOGGED_EVENT               0xFFFFFFFFu

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* Event code definition. Further codes may be used by the application */
typedef enum
{
  d_EVENT_CRITICAL = 0,
  d_EVENT_NON_CRITICAL
} d_EVENT_EventCode_t; /* Event identification */

/* Event log */
#pragma pack(1) /* Pack all metadata with byte alignment */
typedef struct
{
  Uint64_t timestamp;
  Uint32_t eventNumber;
  Uint32_t eventCode;
  Uint8_t textField[TEXT_FIELD_SIZE];
  Uint32_t textLength;
  Uint32_t eventValue;
  Uint32_t crc;
} d_EVENT_LogEvent_t;
#pragma pack()	/* Revert packing to default (compilation start) value */

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the event logger CSC */
d_Status_t d_EVENT_Initialise(void);

/* Clear the event log on the MMC */
d_Status_t d_EVENT_Clear(void);

/* Read the number of logged events */
d_Status_t d_EVENT_Count(Uint32_t * const pCount);

/* Log an event */
d_Status_t d_EVENT_Log_Event(const d_EVENT_EventCode_t eventCode, const Char_t * const textField, const Uint32_t textLength, const Uint32_t eventValue);

/* Read an event from the MMC */
d_Status_t d_EVENT_ReadEventMmc(const Uint32_t entryNumber, d_EVENT_LogEvent_t * const pEvent);

/* Background write of data to MMC. This function should be called by the scheduler periodically. */
void d_EVENT_ProcessMmc(void);

#endif /* D_EVENT_LOGGER_H */
