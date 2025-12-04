/*
 * File: vel_ctrl_switcher.h
 *
 * Code generated for Simulink model 'vel_ctrl_switcher'.
 *
 * Model version                  : 2.87
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:25 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef vel_ctrl_switcher_h_
#define vel_ctrl_switcher_h_
#ifndef vel_ctrl_switcher_COMMON_INCLUDES_
#define vel_ctrl_switcher_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* vel_ctrl_switcher_COMMON_INCLUDES_ */

#include "vel_ctrl_switcher_types.h"
#include <string.h>

extern void vel_ctrl_switcher_Init(void);
extern void vel_ctrl_switcher(const real_T rtu_Sensor_pos_lla[3], const real_T
  rtu_Sensor_vel_ned[3], const real_T *rtu_Sensor_h_radar_agl, const pilot_ext_t
  *rtu_Pilot, const real_T *rtu_ctrlIF_vel_velCmdH_x, const real_T
  *rtu_ctrlIF_vel_velCmdH_y, const real_T *rtu_ctrlIF_vel_hRateCmd, const vom_t *
  rtu_vom_status, const boolean_T *rtu_sFlags_bGPSLoss10sec, const boolean_T
  *rtu_sFlags_bGPSLoss, const real_T *rtu_mode_data_hover_data_hover_alt_agl,
  const real_T *rtu_mode_data_autotakeoff_data_takeoff_alt_agl, const real_T
  *rtu_mode_data_rth_data_rth_velX, const real_T
  *rtu_mode_data_rth_data_rth_velY, const real_T
  *rtu_mode_data_rth_data_rth_alt_agl, const boolean_T
  *rtu_mode_data_rth_data_bRTH_PosModeFlag, const real_T
  *rtu_mode_data_ft_data_FT_Altitude, const real_T
  *rtu_mode_data_bt_data_BT_Altitude, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const boolean_T *rtu_bInAirFlag, const uint8_T *
  rtu_TECS_mode, const lifter_state_t *rtu_lifter_state, real_T
  *rty_busControllerIF_vel_velCmdH_x, real_T *rty_busControllerIF_vel_velCmdH_y,
  real_T *rty_busControllerIF_vel_hRateCmd, real_T *rty_busControllerIF_vel_hCmd,
  boolean_T *rty_busControllerIF_vel_hHold, boolean_T
  *rty_busControllerIF_vel_init_reset_vel, boolean_T
  *rty_busControllerIF_vel_init_reset_hdot);

/* Model reference registration function */
extern void vel_ctrl_switcher_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S16>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S23>/FixPt Data Type Duplicate' : Unused code path elimination
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
 * '<Root>' : 'vel_ctrl_switcher'
 * '<S1>'   : 'vel_ctrl_switcher/ALL FIXED MODES'
 * '<S2>'   : 'vel_ctrl_switcher/AUTOLAND'
 * '<S3>'   : 'vel_ctrl_switcher/AUTOTAKEOFF'
 * '<S4>'   : 'vel_ctrl_switcher/BTransition'
 * '<S5>'   : 'vel_ctrl_switcher/FT'
 * '<S6>'   : 'vel_ctrl_switcher/HOVER'
 * '<S7>'   : 'vel_ctrl_switcher/MANUAL'
 * '<S8>'   : 'vel_ctrl_switcher/READY'
 * '<S9>'   : 'vel_ctrl_switcher/RTH'
 * '<S10>'  : 'vel_ctrl_switcher/STARTUP'
 * '<S11>'  : 'vel_ctrl_switcher/UMAN'
 * '<S12>'  : 'vel_ctrl_switcher/AUTOLAND/Compare To Constant'
 * '<S13>'  : 'vel_ctrl_switcher/BTransition/Compare To Constant2'
 * '<S14>'  : 'vel_ctrl_switcher/MANUAL/Subsystem'
 * '<S15>'  : 'vel_ctrl_switcher/MANUAL/Subsystem/Compare To Constant2'
 * '<S16>'  : 'vel_ctrl_switcher/MANUAL/Subsystem/Interval Test'
 * '<S17>'  : 'vel_ctrl_switcher/UMAN/Compare To Constant'
 * '<S18>'  : 'vel_ctrl_switcher/UMAN/Compare To Constant2'
 * '<S19>'  : 'vel_ctrl_switcher/UMAN/Subsystem'
 * '<S20>'  : 'vel_ctrl_switcher/UMAN/Subsystem/Compare To Constant2'
 * '<S21>'  : 'vel_ctrl_switcher/UMAN/Subsystem/Detect Change'
 * '<S22>'  : 'vel_ctrl_switcher/UMAN/Subsystem/Detect Change1'
 * '<S23>'  : 'vel_ctrl_switcher/UMAN/Subsystem/Interval Test'
 * '<S24>'  : 'vel_ctrl_switcher/UMAN/Subsystem/TECS_OFF'
 */

/*-
 * Requirements for '<Root>': vel_ctrl_switcher

 */
#endif                                 /* vel_ctrl_switcher_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
