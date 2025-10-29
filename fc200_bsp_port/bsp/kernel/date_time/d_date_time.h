/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_date_time

  Abstract           : Date and Time driver module

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-275
                                                                SWREQ-276
                                                                SWREQ-277
                       SDD References: 136T-2200-131100-001-D22 SWDES-217
                                                                SWDES-218
                                                                SWDES-219
                                                                SWDES-220

\note
  CSC ID             : SWDES-78
*************************************************************************/

#ifndef D_DATE_TIME_H
#define D_DATE_TIME_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise the Date and Time interface. */
d_Status_t d_DATE_TIME_Initialise(void);

/* Get the timestamp */
Uint64_t d_DATE_TIME_Timestamp(void);

/* Set the timestamp */
void d_DATE_TIME_TimestampSet(const Uint64_t newTimestamp);

/* Add milliseconds to timestamp */
void d_DATE_TIME_TimestampUpdate(void);

#endif /* D_DATE_TIME_H */
