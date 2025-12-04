/*
 * File: FW_throttle.c
 *
 * Code generated for Simulink model 'FW_throttle'.
 *
 * Model version                  : 2.3
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:37 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FW_throttle.h"
#include "rtwtypes.h"
#include "FW_throttle_private.h"
#include <string.h>
#define FW_throttle_period             (0.01)

/* Block states (default storage) */
FW_throttle_TDW FW_throttle_DW;

/* System initialize for referenced model: 'FW_throttle' */
void FW_throttle_Init(void)
{
  /* InitializeConditions for RateLimiter: '<Root>/100%_per_sec' */
  FW_throttle_DW.PrevY = 0.0;
}

/* Output and update for referenced model: 'FW_throttle' */
void FW_throttle(const real_T *rtu_FW_Controller_att_pusher_cmd, real_T
                 *rty_engineCmd_out)
{
  real_T rtb_u00_per_sec;

  /* RateLimiter: '<Root>/100%_per_sec' */
  rtb_u00_per_sec = (*rtu_FW_Controller_att_pusher_cmd) - FW_throttle_DW.PrevY;
  if (rtb_u00_per_sec > FW_throttle_period) {
    rtb_u00_per_sec = FW_throttle_DW.PrevY + FW_throttle_period;
  } else if (rtb_u00_per_sec < -0.01) {
    rtb_u00_per_sec = FW_throttle_DW.PrevY - 0.01;
  } else {
    rtb_u00_per_sec = *rtu_FW_Controller_att_pusher_cmd;
  }

  FW_throttle_DW.PrevY = rtb_u00_per_sec;

  /* End of RateLimiter: '<Root>/100%_per_sec' */

  /* Saturate: '<Root>/Saturation' */
  if (rtb_u00_per_sec > 1.0) {
    *rty_engineCmd_out = 1.0;
  } else if (rtb_u00_per_sec < 0.0) {
    *rty_engineCmd_out = 0.0;
  } else {
    *rty_engineCmd_out = rtb_u00_per_sec;
  }

  /* End of Saturate: '<Root>/Saturation' */
}

/* Model initialize function */
void FW_throttle_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&FW_throttle_DW, 0,
                sizeof(FW_throttle_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
