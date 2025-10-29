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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/defines/d_common_asm.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/error_handler/d_error_exception.h"

#include "xreg_cortexr5.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_ERROR_ExceptionUndefHandler -->

  Log an undefined instruction exception.
*************************************************************************/
void                          /** \return None */
d_ERROR_ExceptionUndefHandler
(
const Uint32_t * const addr   /**< [in] Address of undefined instruction */
)
{
  d_ERROR_LogData_t errorLogData;

  errorLogData.Emodule[0] = 0;
  errorLogData.Eline = 0;
  errorLogData.Etype = d_STATUS_UNDEFINED_INSTRUCTION;
  errorLogData.Ecriticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  errorLogData.Edata[0] = *addr;
  errorLogData.Edata[1] = 0;
  errorLogData.Edata[2] = 0;
  errorLogData.Edata[3] = 0;

  d_ERROR_Log(&errorLogData);

  return;
}

/*********************************************************************//**
  <!-- d_ERROR_ExceptionAddrHandler -->

  Log an prefetch abort exception.
*************************************************************************/
void                          /** \return None */
d_ERROR_ExceptionAddrHandler
(
const Uint32_t * const addr   /**< [in] Address of abort */
)
{
  d_ERROR_LogData_t errorLogData;

  errorLogData.Emodule[0] = 0;
  errorLogData.Eline = 0;
  errorLogData.Etype = d_STATUS_EXEC_FROM_UNDEFINED_ADDRESS;
  errorLogData.Ecriticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  errorLogData.Edata[0] = *addr;
  errorLogData.Edata[1] = d_mfcp(XREG_CP15_INST_FAULT_STATUS);
  errorLogData.Edata[2] = 0;
  errorLogData.Edata[3] = 0;

  d_ERROR_Log(&errorLogData);

  return;
}

/*********************************************************************//**
  <!-- d_ERROR_ExceptionDataHandler -->

  Log a data abort exception. The call to d_ERROR_Log is not expected 
  to return. If it does this function returns and the next instruction
  is executed.
*************************************************************************/
// gcov-jst 22 This is a weak function overridden by a function in the test application code.
__attribute__((weak))
void                          /** \return None */
d_ERROR_ExceptionDataHandler
(
const Uint32_t * const addr   /**< [in] Address of abort */
)
{
  d_ERROR_LogData_t errorLogData;

  errorLogData.Emodule[0] = 0;
  errorLogData.Eline = 0;
  errorLogData.Etype = d_STATUS_DATA_FROM_UNDEFINED_ADDRESS;
  errorLogData.Ecriticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  errorLogData.Edata[0] = *addr;
  errorLogData.Edata[1] = d_mfcp(XREG_CP15_DATA_FAULT_ADDRESS);
  errorLogData.Edata[2] = d_mfcp(XREG_CP15_DATA_FAULT_STATUS);;
  errorLogData.Edata[3] = 0;

  d_ERROR_Log(&errorLogData);

  return;
}

/*********************************************************************//**
  <!-- d_ERROR_ExceptionFiqHandler -->

  Log an FIQ exception without handler.
*************************************************************************/
void                  /** \return None */
d_ERROR_ExceptionFiqHandler
(
const Uint32_t id     /**< [in] FIQ interreupt number */
)
{
  d_ERROR_LogData_t errorLogData;

  errorLogData.Emodule[0] = 0;
  errorLogData.Eline = 0;
  errorLogData.Etype = d_STATUS_FIQ_WITHOUT_HANDLER;
  errorLogData.Ecriticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  errorLogData.Edata[0] = id;
  errorLogData.Edata[1] = 0;
  errorLogData.Edata[2] = 0;
  errorLogData.Edata[3] = 0;

  d_ERROR_Log(&errorLogData);

  return;
}

/*********************************************************************//**
  <!-- d_ERROR_ExceptionSwintHandler -->

  Log a software interrupt exception without handler.
*************************************************************************/
void                          /** \return None */
d_ERROR_ExceptionSwintHandler
(
const Uint32_t id            /**< [in] Software interrupt number */
)
{
  d_ERROR_LogData_t errorLogData;

  errorLogData.Emodule[0] = 0;
  errorLogData.Eline = 0;
  errorLogData.Etype = d_STATUS_UNDEFINED_SERVICE_EXCEPTION;
  errorLogData.Ecriticality = d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN;
  errorLogData.Edata[0] = id;
  errorLogData.Edata[1] = 0;
  errorLogData.Edata[2] = 0;
  errorLogData.Edata[3] = 0;

  d_ERROR_Log(&errorLogData);

  return;
}

