/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: att_ctrl_switcher.h
 *
 * Code generated for Simulink model 'att_ctrl_switcher'.
 *
 * Model version                  : 2.203
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:10 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef att_ctrl_switcher_h_
#define att_ctrl_switcher_h_
#ifndef att_ctrl_switcher_COMMON_INCLUDES_
#define att_ctrl_switcher_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* att_ctrl_switcher_COMMON_INCLUDES_ */

#include "att_ctrl_switcher_types.h"
#include <string.h>

extern void att_ctrl_switcher_Init(void);
extern void att_ctrl_switcher(const real_T rtu_sensor_eul_ang[3], const real_T
  rtu_sensor_omg[3], const real_T *rtu_sensor_aspd_cas, const real_T
  *rtu_sensor_h_radar_agl, const real_T *rtu_sensor_gspd, const real_T
  *rtu_sensor_chi, const real_T *rtu_attAltCmd_rollCmd, const real_T
  *rtu_attAltCmd_pitchCmd, const vom_t *rtu_vom_status, const pilot_ext_t
  *rtu_Pilot, const boolean_T *rtu_bInAirFlag, const boolean_T
  *rtu_sFlags_bGPSLoss, const real_T *rtu_mode_data_hover_data_hover_yaw_ref,
  const boolean_T *rtu_mode_data_hover_data_hover_yaw_override, const real_T
  *rtu_mode_data_autotakeoff_data_takeoff_yaw_ref, const real_T
  *rtu_mode_data_autoland_data_land_yaw_ref, const real_T
  *rtu_mode_data_rth_data_rth_yaw_ref, const boolean_T
  *rtu_mode_data_rth_data_bBreaking, const real_T
  *rtu_mode_data_ft_data_FT_Heading, const real_T
  *rtu_mode_data_bt_data_BT_Heading, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const uint8_T *rtu_loiter_mode, const real_T
  *rtu_rollCmd_fw_guid, const uint8_T *rtu_TECS_mode, const real_T
  *rtu_dTECSThetaCmd_rad, const uint8_T *rtu_CoG_tracking, const lifter_state_t *
  rtu_lifter_state, real_T *rty_busControllerIF_att_rollCmd, real_T
  *rty_busControllerIF_att_pitchCmd, real_T *rty_busControllerIF_att_yawCmd,
  real_T *rty_busControllerIF_att_yawRateCmd, boolean_T
  *rty_busControllerIF_att_yawHold, real_T *rty_busControllerIF_att_yawFF,
  boolean_T *rty_busControllerIF_att_init_reset);

/* Model reference registration function */
extern void att_ctrl_switcher_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S29>/Data Type Duplicate' : Unused code path elimination
 * Block '<S29>/Data Type Propagation' : Unused code path elimination
 * Block '<S30>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S32>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S36>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S39>/Data Type Duplicate' : Unused code path elimination
 * Block '<S39>/Data Type Propagation' : Unused code path elimination
 * Block '<S47>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S25>/Constant3' : Unused code path elimination
 * Block '<S25>/Constant4' : Unused code path elimination
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
 * '<Root>' : 'att_ctrl_switcher'
 * '<S1>'   : 'att_ctrl_switcher/ALL FIXED MODES'
 * '<S2>'   : 'att_ctrl_switcher/AUTOLAND'
 * '<S3>'   : 'att_ctrl_switcher/AUTOTAKEOFF'
 * '<S4>'   : 'att_ctrl_switcher/BT'
 * '<S5>'   : 'att_ctrl_switcher/FT'
 * '<S6>'   : 'att_ctrl_switcher/HOVER'
 * '<S7>'   : 'att_ctrl_switcher/MANUAL'
 * '<S8>'   : 'att_ctrl_switcher/READY'
 * '<S9>'   : 'att_ctrl_switcher/RTH'
 * '<S10>'  : 'att_ctrl_switcher/STARTUP'
 * '<S11>'  : 'att_ctrl_switcher/UMAN'
 * '<S12>'  : 'att_ctrl_switcher/AUTOLAND/RC_Tem_Loss'
 * '<S13>'  : 'att_ctrl_switcher/AUTOLAND/h_less_than_5m'
 * '<S14>'  : 'att_ctrl_switcher/AUTOLAND/h_less_than_5m/Compare To Constant'
 * '<S15>'  : 'att_ctrl_switcher/AUTOTAKEOFF/RC_Tem_Loss'
 * '<S16>'  : 'att_ctrl_switcher/AUTOTAKEOFF/h_below_threshold'
 * '<S17>'  : 'att_ctrl_switcher/AUTOTAKEOFF/h_below_threshold/Compare To Constant'
 * '<S18>'  : 'att_ctrl_switcher/BT/Compare To Constant2'
 * '<S19>'  : 'att_ctrl_switcher/FT/Compare To Constant'
 * '<S20>'  : 'att_ctrl_switcher/FT/zero_division_protection'
 * '<S21>'  : 'att_ctrl_switcher/HOVER/Compare To Constant2'
 * '<S22>'  : 'att_ctrl_switcher/HOVER/Detect Change'
 * '<S23>'  : 'att_ctrl_switcher/HOVER/Detect Increase'
 * '<S24>'  : 'att_ctrl_switcher/HOVER/GPS_Loss'
 * '<S25>'  : 'att_ctrl_switcher/HOVER/Smooth_Transition'
 * '<S26>'  : 'att_ctrl_switcher/HOVER/Subsystem'
 * '<S27>'  : 'att_ctrl_switcher/HOVER/Smooth_Transition/Rate_Limiter_Dynamic1'
 * '<S28>'  : 'att_ctrl_switcher/HOVER/Smooth_Transition/Rate_Limiter_Dynamic1/Compare To Constant'
 * '<S29>'  : 'att_ctrl_switcher/HOVER/Smooth_Transition/Rate_Limiter_Dynamic1/Saturation Dynamic'
 * '<S30>'  : 'att_ctrl_switcher/HOVER/Smooth_Transition/Rate_Limiter_Dynamic1/Unit Delay External IC'
 * '<S31>'  : 'att_ctrl_switcher/HOVER/Subsystem/Detect Decrease'
 * '<S32>'  : 'att_ctrl_switcher/HOVER/Subsystem/Interval Test'
 * '<S33>'  : 'att_ctrl_switcher/HOVER/Subsystem/wrapToPi'
 * '<S34>'  : 'att_ctrl_switcher/MANUAL/Subsystem'
 * '<S35>'  : 'att_ctrl_switcher/MANUAL/Subsystem/Compare To Constant2'
 * '<S36>'  : 'att_ctrl_switcher/MANUAL/Subsystem/Interval Test'
 * '<S37>'  : 'att_ctrl_switcher/MANUAL/Subsystem/wrapToPi'
 * '<S38>'  : 'att_ctrl_switcher/RTH/GPS_Loss'
 * '<S39>'  : 'att_ctrl_switcher/RTH/Saturation Dynamic'
 * '<S40>'  : 'att_ctrl_switcher/UMAN/Compare To Constant1'
 * '<S41>'  : 'att_ctrl_switcher/UMAN/yaw_cmd'
 * '<S42>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/Compare To Constant2'
 * '<S43>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/Detect Change'
 * '<S44>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/Detect Increase'
 * '<S45>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/Subsystem'
 * '<S46>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/zero_division_protection'
 * '<S47>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/Subsystem/Interval Test'
 * '<S48>'  : 'att_ctrl_switcher/UMAN/yaw_cmd/Subsystem/wrapToPi'
 */

/*-
 * Requirements for '<Root>': att_ctrl_switcher

 */
#endif                                 /* att_ctrl_switcher_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
