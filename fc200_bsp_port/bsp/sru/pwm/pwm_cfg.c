/******[Configuration Header]*****************************************//**
\file
\brief
   Module Title       : pwm_cfg.c

  Abstract           : Pulse Width Modulation interface configuration.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-327
                       SDD References: 136T-2200-131000-001-D22 SWDES-554
                                                                SWDES-605
\note
  CSC ID             : SWDES-71
*************************************************************************/

/***** Includes *********************************************************/

#include "xparameters.h"

#include "soc/defines/d_common_types.h"
#include "sru/platform/ioc.h"

#include "sru/pwm/d_pwm_cfg.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_PWM_AddressDefinition_t d_PWM_AddressDefinition[] =
{
  {
    XPAR_IOCA_AXIOE_BASEADDR + IOC_PULSE_AXI_0_BASEADDR,
    XPAR_IOCA_AXIOE_BASEADDR + IOC_SERVO_AXI_BASEADDR
  },
  {
    XPAR_IOCB_AXIOE_BASEADDR + IOC_PULSE_AXI_0_BASEADDR,
    XPAR_IOCB_AXIOE_BASEADDR + IOC_SERVO_AXI_BASEADDR
  }
};

__attribute__((weak)) const d_PWM_Definition_t d_PWM_Definition[] = 
{
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f},
  {-1.0f, 1.0f, 0.0f}
};

/* Define the number of IOC interfaces */
__attribute__((weak)) d_PWM_COUNT_IOC;

/* Define the number of PWM channels */
__attribute__((weak)) d_PWM_COUNT_CHANNEL;

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

