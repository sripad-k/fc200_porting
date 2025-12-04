/*
 * File: FW_attCtrl_switcher.h
 *
 * Code generated for Simulink model 'FW_attCtrl_switcher'.
 *
 * Model version                  : 2.60
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:23 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_attCtrl_switcher_h_
#define FW_attCtrl_switcher_h_
#ifndef FW_attCtrl_switcher_COMMON_INCLUDES_
#define FW_attCtrl_switcher_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                /* FW_attCtrl_switcher_COMMON_INCLUDES_ */

#include "FW_attCtrl_switcher_types.h"
#include <string.h>

extern void FW_attCtrl_switcher_Init(void);
extern void FW_attCtrl_switcher(const vom_t *rtu_vom_status, const real_T
  *rtu_Pilot_engine_ch, const real_T *rtu_Sensor_aspd_cas, const uint8_T
  *rtu_TECS_mode, const real_T *rtu_ControllerTECS_engineCmd, const real_T
  *rtu_ControllerTECS_theta_cmd, const real_T *rtu_MR_att_cmd_rollCmd, const
  real_T *rtu_MR_att_cmd_pitchCmd, const real_T
  *rtu_mode_data_ft_data_FT_PusherThrottle, const real_T
  *rtu_mode_data_bt_data_BT_PusherThrottle, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const real_T *rtu_rollCmd_FW_guidance,
  busFW_Controller_att *rty_busFW_Controller_att);

/* Model reference registration function */
extern void FW_attCtrl_switcher_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S14>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S16>/Data Type Duplicate' : Unused code path elimination
 * Block '<S16>/Data Type Propagation' : Unused code path elimination
 * Block '<S14>/Zero-Order Hold' : Eliminated since input and output rates are identical
 */

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
 * '<Root>' : 'FW_attCtrl_switcher'
 * '<S1>'   : 'FW_attCtrl_switcher/ALL_MR_MODES'
 * '<S2>'   : 'FW_attCtrl_switcher/BTransition'
 * '<S3>'   : 'FW_attCtrl_switcher/FDirector'
 * '<S4>'   : 'FW_attCtrl_switcher/FTransition'
 * '<S5>'   : 'FW_attCtrl_switcher/FW_RTH'
 * '<S6>'   : 'FW_attCtrl_switcher/LOITER'
 * '<S7>'   : 'FW_attCtrl_switcher/UMAN'
 * '<S8>'   : 'FW_attCtrl_switcher/WP'
 * '<S9>'   : 'FW_attCtrl_switcher/BTransition/Compare To Constant'
 * '<S10>'  : 'FW_attCtrl_switcher/BTransition/Compare To Constant1'
 * '<S11>'  : 'FW_attCtrl_switcher/BTransition/Compare To Constant2'
 * '<S12>'  : 'FW_attCtrl_switcher/FDirector/Compare To Constant'
 * '<S13>'  : 'FW_attCtrl_switcher/FTransition/Compare To Constant'
 * '<S14>'  : 'FW_attCtrl_switcher/FTransition/dyn_rate_limit'
 * '<S15>'  : 'FW_attCtrl_switcher/FTransition/pwm>1500'
 * '<S16>'  : 'FW_attCtrl_switcher/FTransition/dyn_rate_limit/Saturation Dynamic'
 * '<S17>'  : 'FW_attCtrl_switcher/FW_RTH/Compare To Constant'
 * '<S18>'  : 'FW_attCtrl_switcher/LOITER/Compare To Constant'
 * '<S19>'  : 'FW_attCtrl_switcher/UMAN/Compare To Constant'
 */

/*-
 * Requirements for '<Root>': FW_attCtrl_switcher

 */
#endif                                 /* FW_attCtrl_switcher_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
