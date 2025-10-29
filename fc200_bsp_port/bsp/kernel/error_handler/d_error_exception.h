/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Error Exception Handling

  Abstract : Error handling driver exception processing

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

#ifndef D_ERROR_EXCEPTION_H
#define D_ERROR_EXCEPTION_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/* Function prototypes for exception errors */
void d_ERROR_ExceptionUndefHandler(const Uint32_t * const addr);
void d_ERROR_ExceptionAddrHandler(const Uint32_t * const addr);
void d_ERROR_ExceptionDataHandler(const Uint32_t * const addr);
void d_ERROR_ExceptionFiqHandler(const Uint32_t id);
void d_ERROR_ExceptionSwintHandler(const Uint32_t id);

#endif /* D_ERROR_EXCEPTION_H */
