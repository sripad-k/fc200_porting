/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_TECS_switcher.h
 *
 * Code generated for Simulink model 'FW_TECS_switcher'.
 *
 * Model version                  : 2.176
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:50:56 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_TECS_switcher_h_
#define FW_TECS_switcher_h_
#ifndef FW_TECS_switcher_COMMON_INCLUDES_
#define FW_TECS_switcher_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FW_TECS_switcher_COMMON_INCLUDES_ */

#include "FW_TECS_switcher_types.h"
#include <string.h>

extern void FW_TECS_switcher_Init(void);
extern void FW_TECS_switcher(const vom_t *rtu_vom_status, const real_T
  *rtu_Pilot_roll_ch, const real_T *rtu_Pilot_throttle_ch, const real_T
  rtu_Sensor_eul_ang[3], const real_T rtu_Sensor_pos_lla[3], const real_T
  rtu_Sensor_vel_ned[3], const real_T *rtu_Sensor_aspd_cas, const real_T
  *rtu_Sensor_gspd, const real_T *rtu_Sensor_chi, const uint8_T *rtu_TECS_mode,
  const real_T *rtu_mode_data_ft_data_FT_x, const real_T
  *rtu_mode_data_ft_data_FT_y, const real_T *rtu_mode_data_ft_data_FT_Altitude,
  const real_T *rtu_mode_data_ft_data_FT_Heading, const real_T
  *rtu_mode_data_ft_data_FT_AirspeedRef, const real_T
  *rtu_mode_data_loiter_data_loiter_radius, const real_T
  *rtu_mode_data_loiter_data_loiter_direction, const real_T
  *rtu_mode_data_loiter_data_loiter_Center_Lat, const real_T
  *rtu_mode_data_loiter_data_loiter_Center_Lon, const real_T
  *rtu_mode_data_loiter_data_loiter_altitude, const real_T
  *rtu_mode_data_loiter_data_loiter_AirSpeedRef, const real_T
  *rtu_mode_data_bt_data_BT_Hover_Lat, const real_T
  *rtu_mode_data_bt_data_BT_Hover_Lon, const real_T
  *rtu_mode_data_bt_data_BT_Altitude, const real_T
  *rtu_mode_data_bt_data_BT_Heading, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const eFWRTH_phase
  *rtu_mode_data_fwrth_data_phase, const real_T *rtu_mode_data_fwrth_data_CAS_sp,
  const real_T *rtu_mode_data_fwrth_data_alt_sp, const boolean_T
  *rtu_wp_data_wp_list_valid, const uint16_T *rtu_wp_data_wp_list_count, const
  real_T *rtu_wp_data_cur_wp_lat, const real_T *rtu_wp_data_cur_wp_lon, const
  real_T *rtu_wp_data_cur_wp_alt, const uint16_T *rtu_wp_data_cmd_wp_idx, const
  boolean_T *rtu_wp_data_last_wp_land, const vom_t *rtu_std_command_vom_cmd,
  const real_T *rtu_std_command_airspeed_cas_cmd, const real_T
  *rtu_std_command_fwrth_apr_deg, const boolean_T *rtu_bGPSLossFlag, const
  boolean_T *rtu_bTelemtryLinkLoss1sec, busFW_TECS_switcher
  *rty_busFW_TECS_switcher, uint16_T *rty_wp_req_idx, boolean_T
  *rty_FWRTH_SM_in_approach_circle_done, boolean_T
  *rty_FWRTH_SM_in_reduce_speed_alt_done, boolean_T
  *rty_FWRTH_SM_in_circle_align_done, boolean_T
  *rty_FWRTH_SM_in_cross_tangent_point_done, real_T *rty_FWRTH_SM_in_land_lat,
  real_T *rty_FWRTH_SM_in_land_lon, real_T *rty_FWRTH_SM_in_approach_ang, real_T
  *rty_WP_SMdata_cur_leg_heading, real_T *rty_WP_SMdata_cur_leg_length, real_T
  *rty_WP_SMdata_cur_leg_remaining_dist, boolean_T *rty_WP_SMdata_wp_list_valid,
  boolean_T *rty_WP_SMdata_last_wp_land, real_T *rty_WP_SMdata_land_wp_lat,
  real_T *rty_WP_SMdata_land_wp_lon, real_T *rty_WP_SMdata_curpos_to_wp_heading,
  boolean_T *rty_WP_SMdata_WPN_cmd_received);

/* Model reference registration function */
extern void FW_TECS_switcher_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S24>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S26>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S101>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S10>/Reshape' : Reshape block reduction
 * Block '<S10>/Reshape1' : Reshape block reduction
 * Block '<S28>/Reshape' : Reshape block reduction
 * Block '<S28>/Reshape1' : Reshape block reduction
 * Block '<S39>/Reshape' : Reshape block reduction
 * Block '<S39>/Reshape1' : Reshape block reduction
 * Block '<S78>/Reshape' : Reshape block reduction
 * Block '<S78>/Reshape1' : Reshape block reduction
 * Block '<S89>/Reshape' : Reshape block reduction
 * Block '<S89>/Reshape1' : Reshape block reduction
 * Block '<S8>/Reshape' : Reshape block reduction
 * Block '<S105>/Reshape1' : Reshape block reduction
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
 * '<Root>' : 'FW_TECS_switcher'
 * '<S1>'   : 'FW_TECS_switcher/ALL_FW_MODES'
 * '<S2>'   : 'FW_TECS_switcher/BTransition'
 * '<S3>'   : 'FW_TECS_switcher/FDirector'
 * '<S4>'   : 'FW_TECS_switcher/FTransition'
 * '<S5>'   : 'FW_TECS_switcher/FW_RTH'
 * '<S6>'   : 'FW_TECS_switcher/LOITER'
 * '<S7>'   : 'FW_TECS_switcher/UMAN'
 * '<S8>'   : 'FW_TECS_switcher/WPNav'
 * '<S9>'   : 'FW_TECS_switcher/BTransition/Compare To Constant2'
 * '<S10>'  : 'FW_TECS_switcher/BTransition/L1_guidance'
 * '<S11>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1'
 * '<S12>'  : 'FW_TECS_switcher/BTransition/L1_guidance/guidance_line'
 * '<S13>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/ECEF2NED'
 * '<S14>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF'
 * '<S15>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF1'
 * '<S16>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S17>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S18>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S19>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S20>'  : 'FW_TECS_switcher/BTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S21>'  : 'FW_TECS_switcher/FDirector/Compare To Constant1'
 * '<S22>'  : 'FW_TECS_switcher/FDirector/FW_CoG_tracking'
 * '<S23>'  : 'FW_TECS_switcher/FDirector/Subsystem'
 * '<S24>'  : 'FW_TECS_switcher/FDirector/FW_CoG_tracking/Interval Test'
 * '<S25>'  : 'FW_TECS_switcher/FDirector/FW_CoG_tracking/wrapToPi'
 * '<S26>'  : 'FW_TECS_switcher/FDirector/Subsystem/Interval Test'
 * '<S27>'  : 'FW_TECS_switcher/FTransition/Compare To Constant2'
 * '<S28>'  : 'FW_TECS_switcher/FTransition/L1_guidance'
 * '<S29>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1'
 * '<S30>'  : 'FW_TECS_switcher/FTransition/L1_guidance/guidance_line'
 * '<S31>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/ECEF2NED'
 * '<S32>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF'
 * '<S33>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF1'
 * '<S34>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S35>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S36>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S37>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S38>'  : 'FW_TECS_switcher/FTransition/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S39>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance'
 * '<S40>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle'
 * '<S41>'  : 'FW_TECS_switcher/FW_RTH/check_circle_align'
 * '<S42>'  : 'FW_TECS_switcher/FW_RTH/check_speed_alt'
 * '<S43>'  : 'FW_TECS_switcher/FW_RTH/check_tangent'
 * '<S44>'  : 'FW_TECS_switcher/FW_RTH/obtain_loiter_center'
 * '<S45>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/MATLAB Function'
 * '<S46>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1'
 * '<S47>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/ECEF2NED'
 * '<S48>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/LLA2ECEF'
 * '<S49>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/LLA2ECEF1'
 * '<S50>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S51>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S52>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S53>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S54>'  : 'FW_TECS_switcher/FW_RTH/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S55>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/Compare To Constant'
 * '<S56>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/Compare To Constant1'
 * '<S57>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs'
 * '<S58>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/ECEF2NED'
 * '<S59>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/LLA2ECEF'
 * '<S60>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/LLA2ECEF1'
 * '<S61>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/ECEF2NED/MATLAB Function1'
 * '<S62>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/LLA2ECEF/Subsystem2'
 * '<S63>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S64>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/LLA2ECEF1/Subsystem2'
 * '<S65>'  : 'FW_TECS_switcher/FW_RTH/check_approach_circle/dist_from_LLAs/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S66>'  : 'FW_TECS_switcher/FW_RTH/check_circle_align/Compare To Constant'
 * '<S67>'  : 'FW_TECS_switcher/FW_RTH/check_circle_align/Compare To Constant1'
 * '<S68>'  : 'FW_TECS_switcher/FW_RTH/check_circle_align/heading_from_LLAs'
 * '<S69>'  : 'FW_TECS_switcher/FW_RTH/check_circle_align/wrapToPi'
 * '<S70>'  : 'FW_TECS_switcher/FW_RTH/check_circle_align/wrapToPi1'
 * '<S71>'  : 'FW_TECS_switcher/FW_RTH/check_speed_alt/Compare To Constant'
 * '<S72>'  : 'FW_TECS_switcher/FW_RTH/check_tangent/Compare To Constant'
 * '<S73>'  : 'FW_TECS_switcher/FW_RTH/check_tangent/Compare To Constant1'
 * '<S74>'  : 'FW_TECS_switcher/FW_RTH/check_tangent/wrapToPi2'
 * '<S75>'  : 'FW_TECS_switcher/FW_RTH/obtain_loiter_center/WP_from_heading_and_dist'
 * '<S76>'  : 'FW_TECS_switcher/FW_RTH/obtain_loiter_center/wrapToPi'
 * '<S77>'  : 'FW_TECS_switcher/FW_RTH/obtain_loiter_center/wrapToPi1'
 * '<S78>'  : 'FW_TECS_switcher/LOITER/L1_guidance'
 * '<S79>'  : 'FW_TECS_switcher/LOITER/L1_guidance/MATLAB Function1'
 * '<S80>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1'
 * '<S81>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/ECEF2NED'
 * '<S82>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/LLA2ECEF'
 * '<S83>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/LLA2ECEF1'
 * '<S84>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S85>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S86>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S87>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S88>'  : 'FW_TECS_switcher/LOITER/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S89>'  : 'FW_TECS_switcher/UMAN/L1_guidance'
 * '<S90>'  : 'FW_TECS_switcher/UMAN/Subsystem'
 * '<S91>'  : 'FW_TECS_switcher/UMAN/L1_guidance/MATLAB Function'
 * '<S92>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1'
 * '<S93>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/ECEF2NED'
 * '<S94>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/LLA2ECEF'
 * '<S95>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/LLA2ECEF1'
 * '<S96>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S97>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S98>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S99>'  : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S100>' : 'FW_TECS_switcher/UMAN/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S101>' : 'FW_TECS_switcher/UMAN/Subsystem/Interval Test'
 * '<S102>' : 'FW_TECS_switcher/WPNav/Compare To Constant1'
 * '<S103>' : 'FW_TECS_switcher/WPNav/Compare To Constant3'
 * '<S104>' : 'FW_TECS_switcher/WPNav/Detect Increase'
 * '<S105>' : 'FW_TECS_switcher/WPNav/Subsystem'
 * '<S106>' : 'FW_TECS_switcher/WPNav/WP_manager'
 * '<S107>' : 'FW_TECS_switcher/WPNav/guidance_line1'
 * '<S108>' : 'FW_TECS_switcher/WPNav/heading_from_LLAs'
 * '<S109>' : 'FW_TECS_switcher/WPNav/pos_NED2'
 * '<S110>' : 'FW_TECS_switcher/WPNav/Subsystem/guidance_circle'
 * '<S111>' : 'FW_TECS_switcher/WPNav/Subsystem/wrapToPi'
 * '<S112>' : 'FW_TECS_switcher/WPNav/WP_manager/Detect Change'
 * '<S113>' : 'FW_TECS_switcher/WPNav/WP_manager/Detect Increase'
 * '<S114>' : 'FW_TECS_switcher/WPNav/WP_manager/Detect Increase1'
 * '<S115>' : 'FW_TECS_switcher/WPNav/WP_manager/Subsystem'
 * '<S116>' : 'FW_TECS_switcher/WPNav/WP_manager/heading_from_LLAs'
 * '<S117>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED'
 * '<S118>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1'
 * '<S119>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/ECEF2NED'
 * '<S120>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/LLA2ECEF'
 * '<S121>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/LLA2ECEF1'
 * '<S122>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/ECEF2NED/MATLAB Function1'
 * '<S123>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/LLA2ECEF/Subsystem2'
 * '<S124>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S125>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/LLA2ECEF1/Subsystem2'
 * '<S126>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S127>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/ECEF2NED'
 * '<S128>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/LLA2ECEF'
 * '<S129>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/LLA2ECEF1'
 * '<S130>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/ECEF2NED/MATLAB Function1'
 * '<S131>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/LLA2ECEF/Subsystem2'
 * '<S132>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S133>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/LLA2ECEF1/Subsystem2'
 * '<S134>' : 'FW_TECS_switcher/WPNav/WP_manager/pos_NED1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S135>' : 'FW_TECS_switcher/WPNav/pos_NED2/ECEF2NED'
 * '<S136>' : 'FW_TECS_switcher/WPNav/pos_NED2/LLA2ECEF'
 * '<S137>' : 'FW_TECS_switcher/WPNav/pos_NED2/LLA2ECEF1'
 * '<S138>' : 'FW_TECS_switcher/WPNav/pos_NED2/ECEF2NED/MATLAB Function1'
 * '<S139>' : 'FW_TECS_switcher/WPNav/pos_NED2/LLA2ECEF/Subsystem2'
 * '<S140>' : 'FW_TECS_switcher/WPNav/pos_NED2/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S141>' : 'FW_TECS_switcher/WPNav/pos_NED2/LLA2ECEF1/Subsystem2'
 * '<S142>' : 'FW_TECS_switcher/WPNav/pos_NED2/LLA2ECEF1/Subsystem2/zero_division_protection'
 */

/*-
 * Requirements for '<Root>': FW_TECS_switcher

 */
#endif                                 /* FW_TECS_switcher_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
