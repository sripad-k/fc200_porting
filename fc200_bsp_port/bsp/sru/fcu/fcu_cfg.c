/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : fcu_cfg.c

  Abstract           : Weak master change interrupt handler.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-759
                                                                SWREQ-760
                                                                SWREQ-761
                       SDD References: 136T-2200-131100-001-D22 SWDES-890
                                                                SWDES-2446
                                                                SWDES-2447
                                                                SWDES-2448
                                                                SWDES-2449
                                                                SWDES-2450
                                                                SWDES-2451
                                                                SWDES-2452
                                                                SWDES-2453
\note
  CSC ID             : SWDES-887
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_status.h"
#include "xparameters.h"
#include "sru/platform/ioc.h"

#include "sru/fcu/d_fcu.h"
#include "sru/fcu/d_fcu_cfg.h"

/***** Constants ********************************************************/

/* Application function executed on change of master FCU */
__attribute__((weak)) const d_FCU_MasterChanged_t d_FCU_MasterChanged = NULL;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_FCU_IocAddressCheck -->

  Check if the address specified is on one of the IOCs and that is offline.
*************************************************************************/
__attribute__((weak))
d_Status_t                    /** \return Success if not IOC address */ 
d_FCU_IocAddressCheck
(
const Uint32_t address        /**< [in] address to be checked */
)
{
  d_Status_t returnValue;

  if (((address >= XPAR_IOCA_AXIOE_BASEADDR) && (address <= XPAR_IOCA_AXIOE_HIGHADDR)) && (d_FCU_IocOnline(d_FCU_IOC_A) != d_TRUE))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_OFFLINE_IOCA;
  }
  else if (((address >= XPAR_IOCB_AXIOE_BASEADDR) && (address <= XPAR_IOCB_AXIOE_HIGHADDR)) && (d_FCU_IocOnline(d_FCU_IOC_B) != d_TRUE))
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_OFFLINE_IOCB;
  }
  else
  {
    returnValue = d_STATUS_SUCCESS;
  }
  
  return returnValue;
}
