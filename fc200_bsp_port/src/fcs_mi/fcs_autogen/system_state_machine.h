/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: system_state_machine.h
 *
 * Code generated for Simulink model 'system_state_machine'.
 *
 * Model version                  : 3.584
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:45 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef system_state_machine_h_
#define system_state_machine_h_
#ifndef system_state_machine_COMMON_INCLUDES_
#define system_state_machine_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                               /* system_state_machine_COMMON_INCLUDES_ */

#include "system_state_machine_types.h"
#include <string.h>

extern void system_state_machine_Init(boolean_T *rty_bInAirFlag, vom_t
  *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_bGPSLoss10sec, boolean_T *rty_sFlags_bGPSLoss, boolean_T
  *rty_sFlags_rampup_phase, boolean_T *rty_sFlags_bTelemtryLinkLoss1sec,
  boolean_T *rty_sFlags_ep_loss1sec, safety_state_t *rty_safety_state, uint8_T
  *rty_loiter_mode, uint8_T *rty_TECS_mode, lifter_state_t *rty_lifter_state,
  pic_t *rty_pic_status, uint8_T *rty_CoG_tracking);
extern void system_state_machine(const vom_t *rtu_std_command_vom_cmd, const
  uint16_T *rtu_std_command_vom_cmd_cnt, const pic_t *rtu_std_command_pic_cmd,
  const uint16_T *rtu_std_command_pic_cmd_cnt, const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const real_T rtu_sensor_vel_ned[3], const
  real_T *rtu_sensor_aspd_cas, const real_T *rtu_sensor_h_radar_agl, const
  real_T *rtu_sensor_chi, const int8_T *rtu_pilot_input_switch_1, const real_T
  *rtu_pilot_input_arm_ch, const real_T *rtu_controllerAltCtrl_forceDes, const
  boolean_T *rtu_failure_flags_gcs_link_loss, const boolean_T
  *rtu_failure_flags_gps_loss, const boolean_T *rtu_failure_flags_ep_data_loss,
  const boolean_T *rtu_failure_flags_ip_data_loss, const uint8_T
  *rtu_extd_cmd_loiter_cmd, const uint8_T *rtu_extd_cmd_tecs_cmd, const
  boolean_T *rtu_FWRTH_SM_in_approach_circle_done, const boolean_T
  *rtu_FWRTH_SM_in_reduce_speed_alt_done, const boolean_T
  *rtu_FWRTH_SM_in_circle_align_done, const boolean_T
  *rtu_FWRTH_SM_in_cross_tangent_point_done, const real_T
  *rtu_FWRTH_SM_in_land_lat, const real_T *rtu_FWRTH_SM_in_land_lon, const
  real_T *rtu_FWRTH_SM_in_approach_ang, const real_T
  *rtu_WP_SMdata_in_cur_leg_heading, const real_T
  *rtu_WP_SMdata_in_cur_leg_length, const real_T
  *rtu_WP_SMdata_in_cur_leg_remaining_dist, const boolean_T
  *rtu_WP_SMdata_in_wp_list_valid, const boolean_T
  *rtu_WP_SMdata_in_last_wp_land, const real_T *rtu_WP_SMdata_in_land_wp_lat,
  const real_T *rtu_WP_SMdata_in_land_wp_lon, const real_T
  *rtu_WP_SMdata_in_curpos_to_wp_heading, const boolean_T
  *rtu_WP_SMdata_in_WPN_cmd_received, const boolean_T *rtu_wp_data_wp_list_valid,
  boolean_T *rty_bInAirFlag, vom_t *rty_vom_status, busMode_data *rty_mode_data,
  boolean_T *rty_sFlags_bGPSLoss10sec, boolean_T *rty_sFlags_bGPSLoss, boolean_T
  *rty_sFlags_rampup_phase, boolean_T *rty_sFlags_bTelemtryLinkLoss1sec,
  boolean_T *rty_sFlags_ep_loss1sec, vom_t *rty_SM_state_vom_status,
  safety_state_t *rty_SM_state_safety_state, uint8_T *rty_SM_state_inAir_flag,
  uint8_T *rty_SM_state_pic_status, uint8_T *rty_SM_state_loiter_mode, uint8_T
  *rty_SM_state_lifter_mode, uint8_T *rty_SM_state_tecs_mode, uint8_T
  *rty_SM_state_CoG_tracking, safety_state_t *rty_safety_state, uint8_T
  *rty_loiter_mode, uint8_T *rty_TECS_mode, lifter_state_t *rty_lifter_state,
  pic_t *rty_pic_status, uint8_T *rty_CoG_tracking);

/* Model reference registration function */
extern void system_state_machine_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S6>/Delay One Step' : Unused code path elimination
 * Block '<S33>/Delay Input1' : Unused code path elimination
 * Block '<S33>/FixPt Relational Operator' : Unused code path elimination
 * Block '<S6>/OR' : Unused code path elimination
 * Block '<S6>/Reshape1' : Unused code path elimination
 * Block '<S6>/Reshape2' : Unused code path elimination
 * Block '<S6>/Switch' : Unused code path elimination
 * Block '<S1>/Reshape' : Reshape block reduction
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
 * '<Root>' : 'system_state_machine'
 * '<S1>'   : 'system_state_machine/system_state_machine'
 * '<S2>'   : 'system_state_machine/system_state_machine/CapturedAlt'
 * '<S3>'   : 'system_state_machine/system_state_machine/Compare To Constant'
 * '<S4>'   : 'system_state_machine/system_state_machine/Detect Rise Positive'
 * '<S5>'   : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check'
 * '<S6>'   : 'system_state_machine/system_state_machine/dHomeLLA'
 * '<S7>'   : 'system_state_machine/system_state_machine/disarm'
 * '<S8>'   : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow'
 * '<S9>'   : 'system_state_machine/system_state_machine/groudnspeed '
 * '<S10>'  : 'system_state_machine/system_state_machine/heading_from_LLAs'
 * '<S11>'  : 'system_state_machine/system_state_machine/unlech'
 * '<S12>'  : 'system_state_machine/system_state_machine/unlech1'
 * '<S13>'  : 'system_state_machine/system_state_machine/unlech_vom_cmd'
 * '<S14>'  : 'system_state_machine/system_state_machine/wrapToPi'
 * '<S15>'  : 'system_state_machine/system_state_machine/CapturedAlt/FlagLogic'
 * '<S16>'  : 'system_state_machine/system_state_machine/CapturedAlt/FlagLogic/Compare To Constant6'
 * '<S17>'  : 'system_state_machine/system_state_machine/Detect Rise Positive/Positive'
 * '<S18>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/Compare To Constant'
 * '<S19>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/Compare To Constant1'
 * '<S20>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/Compare To Constant2'
 * '<S21>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/Compare To Constant3'
 * '<S22>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs'
 * '<S23>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_to_home'
 * '<S24>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/ECEF2NED'
 * '<S25>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/LLA2ECEF'
 * '<S26>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/LLA2ECEF1'
 * '<S27>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/ECEF2NED/MATLAB Function1'
 * '<S28>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/LLA2ECEF/Subsystem2'
 * '<S29>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S30>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/LLA2ECEF1/Subsystem2'
 * '<S31>'  : 'system_state_machine/system_state_machine/bRTH_ExitCond_Check/dist_from_LLAs/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S32>'  : 'system_state_machine/system_state_machine/dHomeLLA/Compare To Constant'
 * '<S33>'  : 'system_state_machine/system_state_machine/dHomeLLA/Detect Change'
 * '<S34>'  : 'system_state_machine/system_state_machine/dHomeLLA/Detect Increase'
 * '<S35>'  : 'system_state_machine/system_state_machine/disarm/Compare To Constant1'
 * '<S36>'  : 'system_state_machine/system_state_machine/disarm/Compare To Constant2'
 * '<S37>'  : 'system_state_machine/system_state_machine/disarm/Detect Rise Positive1'
 * '<S38>'  : 'system_state_machine/system_state_machine/disarm/Detect Rise Positive1/Positive'
 * '<S39>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA'
 * '<S40>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.destinationPoint'
 * '<S41>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA'
 * '<S42>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint'
 * '<S43>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.HOVER.wraptopi'
 * '<S44>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.RTH.wraptopi'
 * '<S45>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs'
 * '<S46>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/ECEF2NED'
 * '<S47>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/LLA2ECEF'
 * '<S48>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/LLA2ECEF1'
 * '<S49>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/ECEF2NED/MATLAB Function1'
 * '<S50>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/LLA2ECEF/Subsystem2'
 * '<S51>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S52>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/LLA2ECEF1/Subsystem2'
 * '<S53>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA/dist_from_LLAs/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S54>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.BackTransition.destinationPoint/WP_from_heading_and_dist'
 * '<S55>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs'
 * '<S56>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/ECEF2NED'
 * '<S57>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/LLA2ECEF'
 * '<S58>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/LLA2ECEF1'
 * '<S59>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/ECEF2NED/MATLAB Function1'
 * '<S60>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/LLA2ECEF/Subsystem2'
 * '<S61>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S62>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/LLA2ECEF1/Subsystem2'
 * '<S63>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA/dist_from_LLAs/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S64>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint/WP_from_heading_and_dist'
 * '<S65>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.HOVER.wraptopi/wrapToPi'
 * '<S66>'  : 'system_state_machine/system_state_machine/fcs_state_machine_stateflow/FlightManagement.FMM.RTH.wraptopi/wrapToPi'
 * '<S67>'  : 'system_state_machine/system_state_machine/unlech/Detect Change'
 * '<S68>'  : 'system_state_machine/system_state_machine/unlech1/Detect Increase'
 * '<S69>'  : 'system_state_machine/system_state_machine/unlech_vom_cmd/Detect Increase'
 */

/*-
 * Requirements for '<Root>': system_state_machine

 */
#endif                                 /* system_state_machine_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
