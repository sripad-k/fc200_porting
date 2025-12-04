/*
 * File: FW_throttle.h
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

#ifndef FW_throttle_h_
#define FW_throttle_h_
#ifndef FW_throttle_COMMON_INCLUDES_
#define FW_throttle_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FW_throttle_COMMON_INCLUDES_ */

#include "FW_throttle_types.h"
#include <string.h>

extern void FW_throttle_Init(void);
extern void FW_throttle(const real_T *rtu_FW_Controller_att_pusher_cmd, real_T
  *rty_engineCmd_out);

/* Model reference registration function */
extern void FW_throttle_initialize(void);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'FW_throttle'
 */

/*-
 * Requirements for '<Root>': FW_throttle

 */
#endif                                 /* FW_throttle_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
