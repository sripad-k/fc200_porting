/*
 * File: system_state_machine.c
 *
 * Code generated for Simulink model 'system_state_machine'.
 *
 * Model version                  : 3.635
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:05 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "system_state_machine.h"
#include "rtwtypes.h"
#include "system_state_machine_private.h"
#include "system_state_machine_types.h"
#include "rt_modd.h"
#include <math.h>
#include "zero_crossing_types.h"
#include <string.h>

/* Named constants for Chart: '<S1>/fcs_state_machine_stateflow' */
#define system_state_machine_IN_ARMED  ((uint8_T)1U)
#define system_state_machine_IN_AUTOLAND ((uint8_T)1U)
#define system_state_machine_IN_AUTOTAKEOFF ((uint8_T)2U)
#define system_state_machine_IN_BackTransition ((uint8_T)1U)
#define system_state_machine_IN_CoG_tracking_off ((uint8_T)1U)
#define system_state_machine_IN_CoG_tracking_on ((uint8_T)2U)
#define system_state_machine_IN_DISARMED ((uint8_T)2U)
#define system_state_machine_IN_FTransition ((uint8_T)2U)
#define system_state_machine_IN_FTransition_for_FDirector ((uint8_T)1U)
#define system_state_machine_IN_FW_Modes ((uint8_T)3U)
#define system_state_machine_IN_FW_RTH ((uint8_T)3U)
#define system_state_machine_IN_FW_RTH_align_with_circle ((uint8_T)1U)
#define system_state_machine_IN_FW_RTH_approach_circle ((uint8_T)2U)
#define system_state_machine_IN_FW_RTH_cross_tangent_point ((uint8_T)3U)
#define system_state_machine_IN_FW_RTH_reduce_speed_alt ((uint8_T)4U)
#define system_state_machine_IN_FlightDirector_Mode ((uint8_T)4U)
#define system_state_machine_IN_GPS_loss_false ((uint8_T)1U)
#define system_state_machine_IN_GPS_loss_timer1_start ((uint8_T)2U)
#define system_state_machine_IN_GPS_loss_timer2_start ((uint8_T)3U)
#define system_state_machine_IN_GPS_loss_timer3_stop ((uint8_T)4U)
#define system_state_machine_IN_GPS_loss_timer_triggered ((uint8_T)5U)
#define system_state_machine_IN_HOVER  ((uint8_T)4U)
#define system_state_machine_IN_HoverStartTimer ((uint8_T)1U)
#define system_state_machine_IN_HoverStopTimer ((uint8_T)2U)
#define system_state_machine_IN_InAir  ((uint8_T)1U)
#define system_state_machine_IN_Lifter_Status ((uint8_T)1U)
#define system_state_machine_IN_Loiter_Mode ((uint8_T)5U)
#define system_state_machine_IN_Loiter_off ((uint8_T)1U)
#define system_state_machine_IN_Loiter_on ((uint8_T)2U)
#define system_state_machine_IN_MANUAL ((uint8_T)5U)
#define system_state_machine_IN_NOMINAL ((uint8_T)1U)
#define system_state_machine_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define system_state_machine_IN_OnGround ((uint8_T)2U)
#define system_state_machine_IN_PIC_EXT ((uint8_T)1U)
#define system_state_machine_IN_PIC_INT ((uint8_T)2U)
#define system_state_machine_IN_RAMPUP ((uint8_T)2U)
#define system_state_machine_IN_RC_loss_false ((uint8_T)1U)
#define system_state_machine_IN_RC_loss_timer1_start ((uint8_T)2U)
#define system_state_machine_IN_RC_loss_timer2_start ((uint8_T)3U)
#define system_state_machine_IN_RC_loss_timer2_trigger ((uint8_T)4U)
#define system_state_machine_IN_RC_loss_timer3_start ((uint8_T)5U)
#define system_state_machine_IN_READY  ((uint8_T)6U)
#define system_state_machine_IN_RTH    ((uint8_T)7U)
#define system_state_machine_IN_RTH_Alt_Check ((uint8_T)1U)
#define system_state_machine_IN_RTH_Heading_Check ((uint8_T)2U)
#define system_state_machine_IN_RTH_Pos_Check ((uint8_T)3U)
#define system_state_machine_IN_RTH_Speed_Check ((uint8_T)4U)
#define system_state_machine_IN_STARTUP ((uint8_T)8U)
#define system_state_machine_IN_SpeedCheck ((uint8_T)2U)
#define system_state_machine_IN_TECS_off ((uint8_T)1U)
#define system_state_machine_IN_TECS_on ((uint8_T)2U)
#define system_state_machine_IN_Tele_loss_false ((uint8_T)1U)
#define system_state_machine_IN_Tele_loss_timer1_start ((uint8_T)2U)
#define system_state_machine_IN_Tele_loss_timer2_start ((uint8_T)3U)
#define system_state_machine_IN_Tele_loss_timer2_trigger ((uint8_T)4U)
#define system_state_machine_IN_Tele_loss_timer3_trigger ((uint8_T)5U)
#define system_state_machine_IN_TimerInit ((uint8_T)3U)
#define system_state_machine_IN_UMAN   ((uint8_T)9U)
#define system_state_machine_IN_WP_Mode ((uint8_T)6U)
#define system_state_machine_IN_ZEROG  ((uint8_T)10U)
#define system_state_machine_IN_align_yaw_with_WP ((uint8_T)1U)
#define system_state_machine_IN_altitude_check_1 ((uint8_T)3U)
#define system_state_machine_IN_altitude_check_2 ((uint8_T)4U)
#define system_state_machine_IN_exit_to_F_Trans ((uint8_T)2U)
#define system_state_machine_IN_nominal_hover ((uint8_T)3U)
#define system_state_machine_IN_state  ((uint8_T)3U)

/* Block signals (default storage) */
system_state_machine_TB system_state_machine_B;

/* Block states (default storage) */
system_state_machine_TDW system_state_machine_DW;

/* Previous zero-crossings (trigger) states */
system_state_machine_TZCE system_state_machine_PrevZCX;

/* Forward declaration for local functions */
static void system_state_machine_fj4fquesko(const real_T rtu_sensor_pos_lla[3],
  const real_T *rtu_sensor_chi, vom_t *rty_vom_status, busMode_data
  *rty_mode_data);
static void system_state_machine_odhkbmem0z(real_T *TrigonometricFunction,
  real_T *Add1, real_T *Lat_cfuwhmfptw, real_T *Lon_ikb0u2i02a, real_T
  *dist_buzfv3hgub, real_T *heading_ieyh5igw0r, const real_T rtu_sensor_pos_lla
  [3], const real_T *rtu_sensor_chi, vom_t *rty_vom_status, busMode_data
  *rty_mode_data);
static void system_state_machine_hdnpt4qldm(const real_T rtu_sensor_pos_lla[3],
  const real_T *rtu_sensor_aspd_cas, const real_T *rtu_sensor_chi, vom_t
  *rty_vom_status, busMode_data *rty_mode_data);
static void system_state_machine_hqecrwkbbn(real_T *TrigonometricFunction,
  real_T *Add1, real_T *Lat_cfuwhmfptw, real_T *Lon_ikb0u2i02a, real_T
  *dist_buzfv3hgub, real_T *heading_ieyh5igw0r, const real_T rtu_sensor_pos_lla
  [3], const real_T *rtu_sensor_chi, vom_t *rty_vom_status, busMode_data
  *rty_mode_data);
static void system_state_machine_odubvnrnpm(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], vom_t *rty_vom_status, busMode_data
  *rty_mode_data);
static void system_state_machine_gjywfyiloe(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const real_T *rtu_sensor_aspd_cas, const
  real_T *rtu_sensor_chi, const boolean_T *rtu_wp_data_wp_list_valid, vom_t
  *rty_vom_status, busMode_data *rty_mode_data);
static void system_state_machine_jgwda1ajrs(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], vom_t *rty_vom_status, busMode_data
  *rty_mode_data);
static void system_state_machine_aeuawg3vfo(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const int8_T *rtu_pilot_input_switch_1,
  const boolean_T *rtu_wp_data_wp_list_valid, vom_t *rty_vom_status,
  busMode_data *rty_mode_data, boolean_T *rty_sFlags_bGPSLoss);
static void system_state_machine_csj5zj0zmt(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const real_T *rtu_sensor_h_radar_agl,
  const int8_T *rtu_pilot_input_switch_1, vom_t *rty_vom_status, busMode_data
  *rty_mode_data, pic_t *rty_pic_status);
static void system_state_machine_fks3ydn24b(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const int8_T *rtu_pilot_input_switch_1,
  vom_t *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_bGPSLoss);
static void system_state_machine_afs03nhl30(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const int8_T *rtu_pilot_input_switch_1,
  vom_t *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_rampup_phase);
static void system_state_machine_phh325i5jc(const real_T *Gain, const real_T
  rtu_sensor_eul_ang[3], const real_T rtu_sensor_pos_lla[3], const real_T
  *rtu_sensor_aspd_cas, const real_T *rtu_sensor_h_radar_agl, const real_T
  *rtu_sensor_chi, const int8_T *rtu_pilot_input_switch_1, const boolean_T
  *rtu_wp_data_wp_list_valid, vom_t *rty_vom_status, busMode_data *rty_mode_data,
  boolean_T *rty_sFlags_bGPSLoss, boolean_T *rty_sFlags_rampup_phase, boolean_T *
  rty_sFlags_ep_loss1sec, uint8_T *rty_loiter_mode, pic_t *rty_pic_status,
  real_T *rty_h_rel_takeoff);
static void system_state_machine_hkbbt45tg5(const vom_t *vom_status, const
  real_T *rtu_sensor_h_radar_agl, const real_T *rtu_controllerAltCtrl_forceDes,
  boolean_T *rty_bInAirFlag, real_T *rty_h_rel_takeoff);
static void system_state_machine_lnm1jlgmlc(boolean_T
  *rty_sFlags_bTelemtryLinkLoss1sec);
static void system_state_machine_dfezhqrijv(boolean_T *rty_sFlags_bGPSLoss10sec,
  boolean_T *rty_sFlags_bGPSLoss);
static void system_state_machine_g0rztozkme(const vom_t *vom_status, boolean_T
  *rty_sFlags_bGPSLoss10sec, boolean_T *rty_sFlags_bGPSLoss, boolean_T
  *rty_sFlags_bTelemtryLinkLoss1sec, boolean_T *rty_sFlags_ep_loss1sec);
static void system_state_machine_fhrgqlbunr(const boolean_T
  *FixPtRelationalOperator, const boolean_T *FixPtRelationalOperator_cwrqbb5ufj,
  const real_T *Gain, const vom_t *vom_status, const real_T *Switch, const
  real_T *Switch_kc4btehf20, const real_T rtu_sensor_eul_ang[3], const real_T
  rtu_sensor_pos_lla[3], const real_T *rtu_sensor_aspd_cas, const real_T
  *rtu_sensor_h_radar_agl, const real_T *rtu_sensor_chi, const int8_T
  *rtu_pilot_input_switch_1, const real_T *rtu_controllerAltCtrl_forceDes, const
  boolean_T *rtu_wp_data_wp_list_valid, boolean_T *rty_bInAirFlag, vom_t
  *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_bGPSLoss10sec, boolean_T *rty_sFlags_bGPSLoss, boolean_T
  *rty_sFlags_rampup_phase, boolean_T *rty_sFlags_bTelemtryLinkLoss1sec,
  boolean_T *rty_sFlags_ep_loss1sec, safety_state_t *rty_safety_state, uint8_T
  *rty_loiter_mode, uint8_T *rty_TECS_mode, lifter_state_t *rty_lifter_state,
  pic_t *rty_pic_status, uint8_T *rty_CoG_tracking, real_T *rty_h_rel_takeoff);

/*
 * Output and update for function-call system:
 *    '<S8>/FlightManagement.FMM.HOVER.wraptopi'
 *    '<S8>/FlightManagement.FMM.RTH.wraptopi'
 */
void system_state_machine_lkpmp3sm0z(real_T rtu_u, real_T *rty_y, const
  system_state_machine_mxmu1gefao_TConstB *localC)
{
  /* Abs: '<S44>/Abs' incorporates:
   *  Constant: '<S66>/Constant2'
   *  Math: '<S66>/Mod2'
   *  Math: '<S66>/Mod3'
   *  Sum: '<S66>/Add3'
   *  Sum: '<S66>/Add4'
   *  Sum: '<S66>/Sum'
   */
  *rty_y = fabs(rt_modd(rt_modd((rtu_u - -3.1415926535897931) + localC->Sum1,
    localC->Sum1), localC->Sum1) - 3.1415926535897931);
}

/*
 * Output and update for function-call system:
 *    '<S8>/FlightManagement.FMM.FW_Modes.BackTransition.destinationPoint'
 *    '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint'
 */
void system_state_machine_mkamotaax2(real_T rtu_Lat, real_T rtu_Lon, real_T
  rtu_dist, real_T rtu_heading, real_T *rty_latOut, real_T *rty_lonOut)
{
  real_T rtb_TrigonometricFunction1_j3qnhe2a3e;
  real_T rtb_TrigonometricFunction2_gtnsluhxdu;
  real_T rtb_TrigonometricFunction3;
  real_T rtb_TrigonometricFunction4;
  real_T u0;

  /* Trigonometry: '<S55>/Trigonometric Function1' */
  rtb_TrigonometricFunction1_j3qnhe2a3e = sin(rtu_Lat);

  /* Product: '<S55>/Divide' incorporates:
   *  Constant: '<S55>/RADIUS_OF_EARTH'
   */
  rtb_TrigonometricFunction4 = rtu_dist / 6.371E+6;

  /* Trigonometry: '<S55>/Trigonometric Function2' */
  rtb_TrigonometricFunction2_gtnsluhxdu = cos(rtb_TrigonometricFunction4);

  /* Trigonometry: '<S55>/Trigonometric Function3' */
  rtb_TrigonometricFunction3 = cos(rtu_Lat);

  /* Trigonometry: '<S55>/Trigonometric Function4' */
  rtb_TrigonometricFunction4 = sin(rtb_TrigonometricFunction4);

  /* Sum: '<S55>/Add' incorporates:
   *  Product: '<S55>/Product'
   *  Product: '<S55>/Product1'
   *  Trigonometry: '<S55>/Trigonometric Function5'
   */
  u0 = (rtb_TrigonometricFunction1_j3qnhe2a3e *
        rtb_TrigonometricFunction2_gtnsluhxdu) + ((rtb_TrigonometricFunction3 *
    rtb_TrigonometricFunction4) * cos(rtu_heading));

  /* Saturate: '<S55>/Saturation' */
  if (u0 > 1.0) {
    u0 = 1.0;
  } else if (u0 < -1.0) {
    u0 = -1.0;
  } else {
    /* no actions */
  }

  /* Trigonometry: '<S55>/Trigonometric Function' incorporates:
   *  Saturate: '<S55>/Saturation'
   */
  *rty_latOut = asin(u0);

  /* Sum: '<S55>/Add1' incorporates:
   *  Product: '<S55>/Product2'
   *  Product: '<S55>/Product3'
   *  Sum: '<S55>/Add2'
   *  Trigonometry: '<S55>/Trigonometric Function6'
   *  Trigonometry: '<S55>/Trigonometric Function7'
   *  Trigonometry: '<S55>/Trigonometric Function8'
   */
  *rty_lonOut = rtu_Lon + atan2((sin(rtu_heading) * rtb_TrigonometricFunction4) *
    rtb_TrigonometricFunction3, rtb_TrigonometricFunction2_gtnsluhxdu -
    (rtb_TrigonometricFunction1_j3qnhe2a3e * sin(*rty_latOut)));
}

/*
 * Output and update for function-call system:
 *    '<S8>/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA'
 *    '<S8>/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA'
 */
void system_state_machine_k5j3b5svff(const real_T rtu_LLA1[3], const real_T
  rtu_LLA2[3], real_T *rty_distance)
{
  real_T sin_lat_tmp_0[9];
  real_T rtb_Product_msrnbt4gnk[3];
  real_T cos_lat_tmp;
  real_T cos_long_tmp;
  real_T rtb_Add_g2vn0y2s5j;
  real_T rtb_Sqrt_afskj0y53j;
  real_T rtb_Sqrt_b1kzgfuqba;
  real_T rtb_Sum_hnwdzvpcji_idx_0_tmp;
  real_T sin_lat_tmp;
  real_T sin_long_tmp;
  int32_T i;

  /* MATLAB Function: '<S47>/MATLAB Function1' incorporates:
   *  Trigonometry: '<S49>/Trigonometric Function'
   *  Trigonometry: '<S49>/Trigonometric Function1'
   *  Trigonometry: '<S49>/Trigonometric Function2'
   *  Trigonometry: '<S49>/Trigonometric Function3'
   */
  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S50>:1' */
  /* '<S50>:1:6' cos_lat = cos(LLA(1)); */
  cos_lat_tmp = cos(rtu_LLA1[0]);

  /* '<S50>:1:7' sin_lat = sin(LLA(1)); */
  sin_lat_tmp = sin(rtu_LLA1[0]);

  /* '<S50>:1:8' cos_long = cos(LLA(2)); */
  cos_long_tmp = cos(rtu_LLA1[1]);

  /* '<S50>:1:9' sin_long = sin(LLA(2)); */
  sin_long_tmp = sin(rtu_LLA1[1]);

  /* Product: '<S53>/Product' incorporates:
   *  Constant: '<S53>/e'
   */
  /* '<S50>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S50>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S50>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  rtb_Sqrt_afskj0y53j = sin_lat_tmp * 0.0818191908425;

  /* Sqrt: '<S53>/Sqrt' incorporates:
   *  Constant: '<S53>/const'
   *  Math: '<S53>/Square'
   *  Sum: '<S53>/Add'
   */
  rtb_Sqrt_afskj0y53j = sqrt(1.0 - (rtb_Sqrt_afskj0y53j * rtb_Sqrt_afskj0y53j));

  /* Switch: '<S54>/Switch3' incorporates:
   *  Abs: '<S54>/Abs'
   *  Product: '<S54>/Product'
   */
  if (rtb_Sqrt_afskj0y53j <= 0.001) {
    rtb_Sqrt_afskj0y53j = -0.001;
  }

  /* End of Switch: '<S54>/Switch3' */

  /* Product: '<S53>/Divide' incorporates:
   *  Constant: '<S53>/R_0'
   */
  rtb_Sqrt_afskj0y53j = 6.378137E+6 / rtb_Sqrt_afskj0y53j;

  /* Product: '<S49>/Product' incorporates:
   *  Product: '<S49>/Product1'
   *  Sum: '<S49>/Sum'
   */
  rtb_Sum_hnwdzvpcji_idx_0_tmp = (rtb_Sqrt_afskj0y53j + rtu_LLA1[2]) *
    cos_lat_tmp;

  /* Trigonometry: '<S48>/Trigonometric Function' */
  rtb_Add_g2vn0y2s5j = sin(rtu_LLA2[0]);

  /* Product: '<S51>/Product' incorporates:
   *  Constant: '<S51>/e'
   */
  rtb_Sqrt_b1kzgfuqba = rtb_Add_g2vn0y2s5j * 0.0818191908425;

  /* Sqrt: '<S51>/Sqrt' incorporates:
   *  Constant: '<S51>/const'
   *  Math: '<S51>/Square'
   *  Sum: '<S51>/Add'
   */
  rtb_Sqrt_b1kzgfuqba = sqrt(1.0 - (rtb_Sqrt_b1kzgfuqba * rtb_Sqrt_b1kzgfuqba));

  /* Switch: '<S52>/Switch3' incorporates:
   *  Abs: '<S52>/Abs'
   *  Product: '<S52>/Product'
   */
  if (rtb_Sqrt_b1kzgfuqba <= 0.001) {
    rtb_Sqrt_b1kzgfuqba = -0.001;
  }

  /* End of Switch: '<S52>/Switch3' */

  /* Product: '<S51>/Divide' incorporates:
   *  Constant: '<S51>/R_0'
   */
  rtb_Sqrt_b1kzgfuqba = 6.378137E+6 / rtb_Sqrt_b1kzgfuqba;

  /* MATLAB Function: '<S47>/MATLAB Function1' */
  sin_lat_tmp_0[0] = (-sin_lat_tmp) * cos_long_tmp;
  sin_lat_tmp_0[3] = (-sin_lat_tmp) * sin_long_tmp;
  sin_lat_tmp_0[6] = cos_lat_tmp;
  sin_lat_tmp_0[1] = -sin_long_tmp;
  sin_lat_tmp_0[4] = cos_long_tmp;
  sin_lat_tmp_0[7] = 0.0;
  sin_lat_tmp_0[2] = (-cos_lat_tmp) * cos_long_tmp;
  sin_lat_tmp_0[5] = (-cos_lat_tmp) * sin_long_tmp;
  sin_lat_tmp_0[8] = -sin_lat_tmp;

  /* Product: '<S48>/Product' incorporates:
   *  Product: '<S48>/Product1'
   *  Sum: '<S48>/Sum'
   *  Trigonometry: '<S48>/Trigonometric Function1'
   */
  cos_lat_tmp = (rtb_Sqrt_b1kzgfuqba + rtu_LLA2[2]) * cos(rtu_LLA2[0]);

  /* Sum: '<S46>/Sum' incorporates:
   *  Constant: '<S48>/1-e^2'
   *  Constant: '<S49>/1-e^2'
   *  Product: '<S48>/Product'
   *  Product: '<S48>/Product1'
   *  Product: '<S48>/Product2'
   *  Product: '<S48>/Product3'
   *  Product: '<S49>/Product'
   *  Product: '<S49>/Product1'
   *  Product: '<S49>/Product2'
   *  Product: '<S49>/Product3'
   *  Sum: '<S48>/Sum1'
   *  Sum: '<S49>/Sum1'
   *  Trigonometry: '<S48>/Trigonometric Function2'
   *  Trigonometry: '<S48>/Trigonometric Function3'
   */
  cos_long_tmp = (cos_lat_tmp * cos(rtu_LLA2[1])) -
    (rtb_Sum_hnwdzvpcji_idx_0_tmp * cos_long_tmp);
  sin_long_tmp = (cos_lat_tmp * sin(rtu_LLA2[1])) -
    (rtb_Sum_hnwdzvpcji_idx_0_tmp * sin_long_tmp);
  sin_lat_tmp = (rtb_Add_g2vn0y2s5j * ((0.993305620009879 * rtb_Sqrt_b1kzgfuqba)
    + rtu_LLA2[2])) - (sin_lat_tmp * ((0.993305620009879 * rtb_Sqrt_afskj0y53j)
    + rtu_LLA1[2]));

  /* Product: '<S47>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S47>/Product' */
    rtb_Product_msrnbt4gnk[i] = ((sin_lat_tmp_0[i] * cos_long_tmp) +
      (sin_lat_tmp_0[i + 3] * sin_long_tmp)) + (sin_lat_tmp_0[i + 6] *
      sin_lat_tmp);
  }

  /* End of Product: '<S47>/Product' */

  /* Sqrt: '<S40>/Sqrt' incorporates:
   *  Math: '<S40>/Square'
   *  Math: '<S40>/Square1'
   *  Sum: '<S40>/Add'
   */
  *rty_distance = sqrt((rtb_Product_msrnbt4gnk[0] * rtb_Product_msrnbt4gnk[0]) +
                       (rtb_Product_msrnbt4gnk[1] * rtb_Product_msrnbt4gnk[1]));
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_fj4fquesko(const real_T rtu_sensor_pos_lla[3],
  const real_T *rtu_sensor_chi, vom_t *rty_vom_status, busMode_data
  *rty_mode_data)
{
  real_T Add1_db1a23tfo4;
  real_T TrigonometricFunction_kveimm1l53;

  /* Entry 'BackTransition': '<S8>:2709' */
  /* '<S8>:2709:3' vom_status=vom_t.VOM_B_TRANS; */
  *rty_vom_status = VOM_B_TRANS;

  /* '<S8>:2709:4' currentLLA=sLLA; */
  system_state_machine_DW.currentLLA[0] = rtu_sensor_pos_lla[0];
  system_state_machine_DW.currentLLA[1] = rtu_sensor_pos_lla[1];
  system_state_machine_DW.currentLLA[2] = rtu_sensor_pos_lla[2];

  /* '<S8>:2709:5' if bBTrans_hover_sp_set */
  if (system_state_machine_DW.bBTrans_hover_sp_set) {
    /* '<S8>:2709:6' bBTrans_hover_sp_set = false; */
    system_state_machine_DW.bBTrans_hover_sp_set = false;
  } else {
    /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.BackTransition.destinationPoint' */
    /* '<S8>:2709:7' else */
    /* '<S8>:2709:8' [sMode_data.bt_data.BT_Hover_Lat, sMode_data.bt_data.BT_Hover_Lon] = destinationPoint(sLLA(1),sLLA(2),500, dChi_rad); */
    /* Simulink Function 'destinationPoint': '<S8>:2741' */
    system_state_machine_mkamotaax2(rtu_sensor_pos_lla[0], rtu_sensor_pos_lla[1],
      500.0, *rtu_sensor_chi, &TrigonometricFunction_kveimm1l53,
      &Add1_db1a23tfo4);

    /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.BackTransition.destinationPoint' */
    rty_mode_data->bt_data.BT_Hover_Lat = TrigonometricFunction_kveimm1l53;
    rty_mode_data->bt_data.BT_Hover_Lon = Add1_db1a23tfo4;

    /* '<S8>:2709:9' sMode_data.bt_data.BT_Heading=dChi_rad; */
    rty_mode_data->bt_data.BT_Heading = *rtu_sensor_chi;
  }

  /* '<S8>:2709:11' sMode_data.bt_data.BT_Altitude=sLLA(3); */
  rty_mode_data->bt_data.BT_Altitude = rtu_sensor_pos_lla[2];

  /* m */
  /* '<S8>:2709:12' sMode_data.bt_data.BT_PusherThrottle=0; */
  rty_mode_data->bt_data.BT_PusherThrottle = 0.0;

  /* '<S8>:2709:13' sMode_data.bt_data.bBT_AttitudeModeFlag=true; */
  rty_mode_data->bt_data.bBT_AttitudeModeFlag = true;

  /* '<S8>:2709:14' sMode_data.bt_data.BT_PitchCmd=0; */
  rty_mode_data->bt_data.BT_PitchCmd = 0.0;

  /* '<S8>:2709:15' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
  rty_mode_data->eFWLifter_Mode = OFF;
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_odhkbmem0z(real_T *TrigonometricFunction,
  real_T *Add1, real_T *Lat_cfuwhmfptw, real_T *Lon_ikb0u2i02a, real_T
  *dist_buzfv3hgub, real_T *heading_ieyh5igw0r, const real_T rtu_sensor_pos_lla
  [3], const real_T *rtu_sensor_chi, vom_t *rty_vom_status, busMode_data
  *rty_mode_data)
{
  /* During 'WP_Mode': '<S8>:2796' */
  /* '<S8>:2820:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_LOITER; */
  if (system_state_machine_B.Switch_d533txexjk == VOM_LOITER) {
    /* Transition: '<S8>:2820' */
    system_state_machine_DW.is_FW_Modes = system_state_machine_IN_Loiter_Mode;

    /* Entry 'Loiter_Mode': '<S8>:2748' */
    /* '<S8>:2748:3' vom_status = vom_t.VOM_LOITER; */
    *rty_vom_status = VOM_LOITER;

    /* '<S8>:2748:4' sMode_data.eFWLifter_Mode= lifter_state_t.OFF; */
    rty_mode_data->eFWLifter_Mode = OFF;

    /* '<S8>:2748:5' sMode_data.loiter_data.loiter_radius = 200; */
    rty_mode_data->loiter_data.loiter_radius = 200.0;

    /* '<S8>:2748:6' sMode_data.loiter_data.loiter_direction = -1; */
    rty_mode_data->loiter_data.loiter_direction = -1.0;

    /* CCW */
    /* '<S8>:2748:7' [sMode_data.loiter_data.loiter_Center_Lat, sMode_data.loiter_data.loiter_Center_Lon] = destinationPoint(sLLA(1), sLLA(2),250, dChi_rad); */
    *Lat_cfuwhmfptw = rtu_sensor_pos_lla[0];
    *Lon_ikb0u2i02a = rtu_sensor_pos_lla[1];
    *dist_buzfv3hgub = 250.0;
    *heading_ieyh5igw0r = *rtu_sensor_chi;

    /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint' */
    /* Simulink Function 'destinationPoint': '<S8>:2755' */
    system_state_machine_mkamotaax2(*Lat_cfuwhmfptw, *Lon_ikb0u2i02a,
      *dist_buzfv3hgub, *heading_ieyh5igw0r, TrigonometricFunction, Add1);

    /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint' */
    rty_mode_data->loiter_data.loiter_Center_Lat = *TrigonometricFunction;
    rty_mode_data->loiter_data.loiter_Center_Lon = *Add1;

    /* '<S8>:2748:8' sMode_data.loiter_data.loiter_altitude = sLLA(3); */
    rty_mode_data->loiter_data.loiter_altitude = rtu_sensor_pos_lla[2];

    /* m */
    /* '<S8>:2748:9' sMode_data.loiter_data.loiter_AirSpeedRef = 25; */
    rty_mode_data->loiter_data.loiter_AirSpeedRef = 25.0;

    /* mps */

    /* '<S8>:2799:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER || goto_back_trans; */
  } else if ((system_state_machine_B.Switch_d533txexjk == VOM_HOVER) ||
             (system_state_machine_DW.goto_back_trans)) {
    /* Transition: '<S8>:2799' */
    system_state_machine_DW.temporalCounter_i1 = 0U;
    system_state_machine_DW.is_FW_Modes = system_state_machine_IN_BackTransition;
    system_state_machine_fj4fquesko(rtu_sensor_pos_lla, rtu_sensor_chi,
      rty_vom_status, rty_mode_data);

    /* Entry Internal 'BackTransition': '<S8>:2709' */
    /* Transition: '<S8>:2719' */
    system_state_machine_DW.temporalCounter_i2 = 0U;
    system_state_machine_DW.is_BackTransition =
      system_state_machine_IN_SpeedCheck;

    /* Transition: '<S8>:2824' */
    /* '<S8>:2798:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_FW_RTH... */
    /* '<S8>:2798:2' || bTelemtryLinkLoss5sec==true; */
  } else if ((system_state_machine_B.Switch_d533txexjk == VOM_FW_RTH) ||
             (system_state_machine_DW.bTelemtryLinkLoss5sec)) {
    /* Transition: '<S8>:2798' */
    system_state_machine_DW.is_FW_Modes = system_state_machine_IN_FW_RTH;

    /* Entry 'FW_RTH': '<S8>:2758' */
    /* '<S8>:2758:3' vom_status=vom_t.VOM_FW_RTH; */
    *rty_vom_status = VOM_FW_RTH;

    /* '<S8>:2758:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
    rty_mode_data->eFWLifter_Mode = OFF;

    /* '<S8>:2758:5' sMode_data.fwrth_data.CAS_sp = 25.0; */
    rty_mode_data->fwrth_data.CAS_sp = 25.0;

    /* '<S8>:2758:6' sMode_data.fwrth_data.alt_sp = sLLA(3); */
    rty_mode_data->fwrth_data.alt_sp = rtu_sensor_pos_lla[2];

    /* Entry Internal 'FW_RTH': '<S8>:2758' */
    /* Transition: '<S8>:2760' */
    system_state_machine_DW.is_FW_RTH =
      system_state_machine_IN_FW_RTH_approach_circle;

    /* Entry 'FW_RTH_approach_circle': '<S8>:2765' */
    /* '<S8>:2765:3' sMode_data.fwrth_data.phase = eFWRTH_phase.approach_circle; */
    rty_mode_data->fwrth_data.phase = approach_circle;

    /* '<S8>:2818:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_FLTDIR; */
  } else if (system_state_machine_B.Switch_d533txexjk == VOM_FLTDIR) {
    /* Transition: '<S8>:2818' */
    system_state_machine_DW.is_FW_Modes =
      system_state_machine_IN_FlightDirector_Mode;

    /* Entry 'FlightDirector_Mode': '<S8>:2746' */
    /* '<S8>:2746:3' vom_status=vom_t.VOM_FLTDIR; */
    *rty_vom_status = VOM_FLTDIR;

    /* '<S8>:2746:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
    rty_mode_data->eFWLifter_Mode = OFF;

    /* '<S8>:2746:5' sMode_data.flightDirector_data.FD_CoG=dChi_rad; */
    rty_mode_data->flightDirector_data.FD_CoG = *rtu_sensor_chi;

    /* '<S8>:2746:6' sMode_data.flightDirector_data.FD_AirspeedRef=25; */
    rty_mode_data->flightDirector_data.FD_AirspeedRef = 25.0;

    /* '<S8>:2746:7' sMode_data.flightDirector_data.FD_Altitude=sLLA(3); */
    rty_mode_data->flightDirector_data.FD_Altitude = rtu_sensor_pos_lla[2];

    /* '<S8>:2796:7' if(WP_SMdata_in.last_wp_land && WP_SMdata_in.cur_leg_remaining_dist <=400 && WP_SMdata_in.cur_leg_length >=400 ) */
  } else if
      (((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.last_wp_land)
        &&
        (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.cur_leg_remaining_dist
         <= 400.0)) &&
       (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.cur_leg_length
        >= 400.0)) {
    /* '<S8>:2796:8' goto_back_trans = true; */
    system_state_machine_DW.goto_back_trans = true;

    /* '<S8>:2796:9' bBTrans_hover_sp_set = true; */
    system_state_machine_DW.bBTrans_hover_sp_set = true;

    /* '<S8>:2796:10' sMode_data.bt_data.BT_Hover_Lat = WP_SMdata_in.land_wp_lat; */
    rty_mode_data->bt_data.BT_Hover_Lat =
      system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.land_wp_lat;

    /* '<S8>:2796:11' sMode_data.bt_data.BT_Hover_Lon = WP_SMdata_in.land_wp_lon; */
    rty_mode_data->bt_data.BT_Hover_Lon =
      system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.land_wp_lon;

    /* '<S8>:2796:12' sMode_data.bt_data.BT_Heading = WP_SMdata_in.cur_leg_heading; */
    rty_mode_data->bt_data.BT_Heading =
      system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.cur_leg_heading;
  } else {
    /* no actions */
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_hdnpt4qldm(const real_T rtu_sensor_pos_lla[3],
  const real_T *rtu_sensor_aspd_cas, const real_T *rtu_sensor_chi, vom_t
  *rty_vom_status, busMode_data *rty_mode_data)
{
  real_T LLA1_daswp04pzd[3];
  real_T LLA2_cm2j2nf3uy[3];
  real_T Sqrt;

  /* During 'FTransition': '<S8>:2777' */
  /* '<S8>:2803:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER; */
  if (system_state_machine_B.Switch_d533txexjk == VOM_HOVER) {
    /* Transition: '<S8>:2803' */
    /* Exit Internal 'FTransition': '<S8>:2777' */
    system_state_machine_DW.is_FTransition =
      system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'FTransition': '<S8>:2777' */
    /* '<S8>:2777:13' sMode_data.ft_data.FT_PusherThrottle=0.0; */
    rty_mode_data->ft_data.FT_PusherThrottle = 0.0;

    /* '<S8>:2777:14' hover2FLTDIR=false; */
    system_state_machine_DW.hover2FLTDIR = false;

    /* '<S8>:2777:15' hover2WAYPNT=false; */
    system_state_machine_DW.hover2WAYPNT = false;
    system_state_machine_DW.temporalCounter_i1 = 0U;
    system_state_machine_DW.is_FW_Modes = system_state_machine_IN_BackTransition;
    system_state_machine_fj4fquesko(rtu_sensor_pos_lla, rtu_sensor_chi,
      rty_vom_status, rty_mode_data);

    /* Entry Internal 'BackTransition': '<S8>:2709' */
    /* Transition: '<S8>:2719' */
    system_state_machine_DW.temporalCounter_i2 = 0U;
    system_state_machine_DW.is_BackTransition =
      system_state_machine_IN_SpeedCheck;
  } else {
    /* '<S8>:2777:11' distance=CurrentDistance_From_HoverLLA([sMode_data.ft_data.FT_x;sMode_data.ft_data.FT_y;sMode_data.ft_data.FT_Altitude],sLLA); */
    LLA1_daswp04pzd[0] = rty_mode_data->ft_data.FT_x;
    LLA1_daswp04pzd[1] = rty_mode_data->ft_data.FT_y;
    LLA1_daswp04pzd[2] = rty_mode_data->ft_data.FT_Altitude;
    LLA2_cm2j2nf3uy[0] = rtu_sensor_pos_lla[0];
    LLA2_cm2j2nf3uy[1] = rtu_sensor_pos_lla[1];
    LLA2_cm2j2nf3uy[2] = rtu_sensor_pos_lla[2];

    /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA' */
    /* Simulink Function 'CurrentDistance_From_HoverLLA': '<S8>:2795' */
    system_state_machine_k5j3b5svff(LLA1_daswp04pzd, LLA2_cm2j2nf3uy, &Sqrt);

    /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.FTransition.CurrentDistance_From_HoverLLA' */
    if (system_state_machine_DW.is_FTransition == ((uint8_T)
         system_state_machine_IN_FTransition_for_FDirector)) {
      /* During 'FTransition_for_FDirector': '<S8>:2790' */
      /* '<S8>:2783:1' sf_internal_predicateOutput = dAirspeedCAS>20; */
      if ((*rtu_sensor_aspd_cas) > 20.0) {
        /* Transition: '<S8>:2783' */
        system_state_machine_DW.temporalCounter_i1 = 0U;
        system_state_machine_DW.is_FTransition =
          system_state_machine_IN_SpeedCheck;

        /* Transition: '<S8>:2784' */
        /* '<S8>:2786:1' sf_internal_predicateOutput = distance>700 ||  after(60,sec); */
      } else if ((Sqrt > 700.0) || (system_state_machine_DW.temporalCounter_i1 >=
                  ((uint16_T)6000))) {
        /* Transition: '<S8>:2786' */
        system_state_machine_DW.is_FTransition =
          system_state_machine_IN_NO_ACTIVE_CHILD;
        system_state_machine_DW.exit_port_index_FTransition = 2U;
      } else {
        /* no actions */
      }

      /* During 'SpeedCheck': '<S8>:2791' */
      /* '<S8>:2787:1' sf_internal_predicateOutput = (after (1, sec)) && hover2FLTDIR==true; */
    } else if ((system_state_machine_DW.temporalCounter_i1 >= ((uint16_T)
                 ((uint8_T)100))) && (system_state_machine_DW.hover2FLTDIR)) {
      /* Transition: '<S8>:2787' */
      system_state_machine_DW.is_FTransition =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_FTransition = 3U;

      /* '<S8>:2788:1' sf_internal_predicateOutput = dAirspeedCAS<20; */
    } else if ((*rtu_sensor_aspd_cas) < 20.0) {
      /* Transition: '<S8>:2788' */
      system_state_machine_DW.temporalCounter_i1 = 0U;
      system_state_machine_DW.is_FTransition =
        system_state_machine_IN_FTransition_for_FDirector;

      /* Entry 'FTransition_for_FDirector': '<S8>:2790' */
      /* '<S8>:2790:3' sMode_data.ft_data.FT_PusherThrottle=0.8; */
      rty_mode_data->ft_data.FT_PusherThrottle = 0.8;

      /* '<S8>:2789:1' sf_internal_predicateOutput = (after (1, sec)) && hover2WAYPNT==true; */
    } else if ((system_state_machine_DW.temporalCounter_i1 >= ((uint16_T)
                 ((uint8_T)100))) && (system_state_machine_DW.hover2WAYPNT)) {
      /* Transition: '<S8>:2789' */
      system_state_machine_DW.is_FTransition =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_FTransition = 4U;

      /* Transition: '<S8>:2785' */
      /* '<S8>:2786:1' sf_internal_predicateOutput = distance>700 ||  after(60,sec); */
    } else if ((Sqrt > 700.0) || (system_state_machine_DW.temporalCounter_i1 >=
                ((uint16_T)6000))) {
      /* Transition: '<S8>:2786' */
      system_state_machine_DW.is_FTransition =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_FTransition = 2U;
    } else {
      /* no actions */
    }

    switch (system_state_machine_DW.exit_port_index_FTransition) {
     case 2:
      system_state_machine_DW.exit_port_index_FTransition = 0U;

      /* Transition: '<S8>:2808' */
      /* Exit 'FTransition': '<S8>:2777' */
      /* '<S8>:2777:13' sMode_data.ft_data.FT_PusherThrottle=0.0; */
      rty_mode_data->ft_data.FT_PusherThrottle = 0.0;

      /* '<S8>:2777:14' hover2FLTDIR=false; */
      system_state_machine_DW.hover2FLTDIR = false;

      /* '<S8>:2777:15' hover2WAYPNT=false; */
      system_state_machine_DW.hover2WAYPNT = false;
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_FW_Modes = 2U;
      break;

     case 3:
      system_state_machine_DW.exit_port_index_FTransition = 0U;

      /* Transition: '<S8>:2813' */
      /* Transition: '<S8>:2817' */
      /* Exit 'FTransition': '<S8>:2777' */
      /* '<S8>:2777:13' sMode_data.ft_data.FT_PusherThrottle=0.0; */
      rty_mode_data->ft_data.FT_PusherThrottle = 0.0;

      /* '<S8>:2777:14' hover2FLTDIR=false; */
      system_state_machine_DW.hover2FLTDIR = false;

      /* '<S8>:2777:15' hover2WAYPNT=false; */
      system_state_machine_DW.hover2WAYPNT = false;
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_FlightDirector_Mode;

      /* Entry 'FlightDirector_Mode': '<S8>:2746' */
      /* '<S8>:2746:3' vom_status=vom_t.VOM_FLTDIR; */
      *rty_vom_status = VOM_FLTDIR;

      /* '<S8>:2746:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
      rty_mode_data->eFWLifter_Mode = OFF;

      /* '<S8>:2746:5' sMode_data.flightDirector_data.FD_CoG=dChi_rad; */
      rty_mode_data->flightDirector_data.FD_CoG = *rtu_sensor_chi;

      /* '<S8>:2746:6' sMode_data.flightDirector_data.FD_AirspeedRef=25; */
      rty_mode_data->flightDirector_data.FD_AirspeedRef = 25.0;

      /* '<S8>:2746:7' sMode_data.flightDirector_data.FD_Altitude=sLLA(3); */
      rty_mode_data->flightDirector_data.FD_Altitude = rtu_sensor_pos_lla[2];
      break;

     case 4:
      system_state_machine_DW.exit_port_index_FTransition = 0U;

      /* Transition: '<S8>:2819' */
      /* Transition: '<S8>:2807' */
      /* Exit 'FTransition': '<S8>:2777' */
      /* '<S8>:2777:13' sMode_data.ft_data.FT_PusherThrottle=0.0; */
      rty_mode_data->ft_data.FT_PusherThrottle = 0.0;

      /* '<S8>:2777:14' hover2FLTDIR=false; */
      system_state_machine_DW.hover2FLTDIR = false;

      /* '<S8>:2777:15' hover2WAYPNT=false; */
      system_state_machine_DW.hover2WAYPNT = false;
      system_state_machine_DW.is_FW_Modes = system_state_machine_IN_WP_Mode;

      /* Entry 'WP_Mode': '<S8>:2796' */
      /* '<S8>:2796:3' vom_status=vom_t.VOM_WAYPNT; */
      *rty_vom_status = VOM_WAYPNT;

      /* '<S8>:2796:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
      rty_mode_data->eFWLifter_Mode = OFF;

      /* '<S8>:2796:5' goto_back_trans = false; */
      system_state_machine_DW.goto_back_trans = false;
      break;

     default:
      /* no actions */
      break;
    }
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_hqecrwkbbn(real_T *TrigonometricFunction,
  real_T *Add1, real_T *Lat_cfuwhmfptw, real_T *Lon_ikb0u2i02a, real_T
  *dist_buzfv3hgub, real_T *heading_ieyh5igw0r, const real_T rtu_sensor_pos_lla
  [3], const real_T *rtu_sensor_chi, vom_t *rty_vom_status, busMode_data
  *rty_mode_data)
{
  /* During 'FW_RTH': '<S8>:2758' */
  /* '<S8>:2816:1' sf_internal_predicateOutput = (vom_cmd==vom_t.VOM_LOITER &&bTelemtryLinkLoss5sec==false && ... */
  /* '<S8>:2816:2' bL2RTH_LinkLossFlag==0); */
  if (((system_state_machine_B.Switch_d533txexjk == VOM_LOITER) &&
       (!system_state_machine_DW.bTelemtryLinkLoss5sec)) &&
      (system_state_machine_DW.bL2RTH_LinkLossFlag == 0.0)) {
    /* Transition: '<S8>:2816' */
    /* Exit Internal 'FW_RTH': '<S8>:2758' */
    system_state_machine_DW.is_FW_RTH = system_state_machine_IN_NO_ACTIVE_CHILD;
    system_state_machine_DW.is_FW_Modes = system_state_machine_IN_Loiter_Mode;

    /* Entry 'Loiter_Mode': '<S8>:2748' */
    /* '<S8>:2748:3' vom_status = vom_t.VOM_LOITER; */
    *rty_vom_status = VOM_LOITER;

    /* '<S8>:2748:4' sMode_data.eFWLifter_Mode= lifter_state_t.OFF; */
    rty_mode_data->eFWLifter_Mode = OFF;

    /* '<S8>:2748:5' sMode_data.loiter_data.loiter_radius = 200; */
    rty_mode_data->loiter_data.loiter_radius = 200.0;

    /* '<S8>:2748:6' sMode_data.loiter_data.loiter_direction = -1; */
    rty_mode_data->loiter_data.loiter_direction = -1.0;

    /* CCW */
    /* '<S8>:2748:7' [sMode_data.loiter_data.loiter_Center_Lat, sMode_data.loiter_data.loiter_Center_Lon] = destinationPoint(sLLA(1), sLLA(2),250, dChi_rad); */
    *Lat_cfuwhmfptw = rtu_sensor_pos_lla[0];
    *Lon_ikb0u2i02a = rtu_sensor_pos_lla[1];
    *dist_buzfv3hgub = 250.0;
    *heading_ieyh5igw0r = *rtu_sensor_chi;

    /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint' */
    /* Simulink Function 'destinationPoint': '<S8>:2755' */
    system_state_machine_mkamotaax2(*Lat_cfuwhmfptw, *Lon_ikb0u2i02a,
      *dist_buzfv3hgub, *heading_ieyh5igw0r, TrigonometricFunction, Add1);

    /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint' */
    rty_mode_data->loiter_data.loiter_Center_Lat = *TrigonometricFunction;
    rty_mode_data->loiter_data.loiter_Center_Lon = *Add1;

    /* '<S8>:2748:8' sMode_data.loiter_data.loiter_altitude = sLLA(3); */
    rty_mode_data->loiter_data.loiter_altitude = rtu_sensor_pos_lla[2];

    /* m */
    /* '<S8>:2748:9' sMode_data.loiter_data.loiter_AirSpeedRef = 25; */
    rty_mode_data->loiter_data.loiter_AirSpeedRef = 25.0;

    /* mps */
  } else {
    switch (system_state_machine_DW.is_FW_RTH) {
     case system_state_machine_IN_FW_RTH_align_with_circle:
      /* During 'FW_RTH_align_with_circle': '<S8>:2767' */
      /* '<S8>:2763:1' sf_internal_predicateOutput = FWRTH_SM_in.circle_align_done; */
      if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.circle_align_done)
      {
        /* Transition: '<S8>:2763' */
        system_state_machine_DW.is_FW_RTH =
          system_state_machine_IN_FW_RTH_cross_tangent_point;

        /* Entry 'FW_RTH_cross_tangent_point': '<S8>:2768' */
        /* '<S8>:2768:3' sMode_data.fwrth_data.phase = eFWRTH_phase.cross_tangent; */
        rty_mode_data->fwrth_data.phase = cross_tangent;

        /* '<S8>:2768:4' bBTrans_hover_sp_set = true; */
        system_state_machine_DW.bBTrans_hover_sp_set = true;

        /* '<S8>:2768:5' sMode_data.bt_data.BT_Hover_Lat = FWRTH_SM_in.land_lat; */
        rty_mode_data->bt_data.BT_Hover_Lat =
          system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.land_lat;

        /* '<S8>:2768:6' sMode_data.bt_data.BT_Hover_Lon = FWRTH_SM_in.land_lon; */
        rty_mode_data->bt_data.BT_Hover_Lon =
          system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.land_lon;

        /* '<S8>:2768:7' sMode_data.bt_data.BT_Heading = FWRTH_SM_in.approach_ang; */
        rty_mode_data->bt_data.BT_Heading =
          system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.approach_ang;
      }
      break;

     case system_state_machine_IN_FW_RTH_approach_circle:
      /* During 'FW_RTH_approach_circle': '<S8>:2765' */
      /* '<S8>:2761:1' sf_internal_predicateOutput = FWRTH_SM_in.approach_circle_done; */
      if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.approach_circle_done)
      {
        /* Transition: '<S8>:2761' */
        system_state_machine_DW.is_FW_RTH =
          system_state_machine_IN_FW_RTH_reduce_speed_alt;

        /* Entry 'FW_RTH_reduce_speed_alt': '<S8>:2766' */
        /* '<S8>:2766:3' sMode_data.fwrth_data.phase = eFWRTH_phase.set_speed_alt; */
        rty_mode_data->fwrth_data.phase = set_speed_alt;

        /* '<S8>:2766:4' sMode_data.fwrth_data.CAS_sp = 22.0; */
        rty_mode_data->fwrth_data.CAS_sp = 22.0;

        /* '<S8>:2766:5' sMode_data.fwrth_data.alt_sp = dHomeLLA(3)+40.0; */
        rty_mode_data->fwrth_data.alt_sp = system_state_machine_B.sLLA[2] + 40.0;

        /*  agl */
      }
      break;

     case system_state_machine_IN_FW_RTH_cross_tangent_point:
      /* During 'FW_RTH_cross_tangent_point': '<S8>:2768' */
      /* '<S8>:2764:1' sf_internal_predicateOutput = FWRTH_SM_in.cross_tangent_point_done; */
      if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.cross_tangent_point_done)
      {
        /* Transition: '<S8>:2764' */
        system_state_machine_DW.is_FW_RTH =
          system_state_machine_IN_NO_ACTIVE_CHILD;
        system_state_machine_DW.exit_port_index_FW_RTH = 2U;
      }
      break;

     default:
      /* During 'FW_RTH_reduce_speed_alt': '<S8>:2766' */
      /* '<S8>:2762:1' sf_internal_predicateOutput = FWRTH_SM_in.reduce_speed_alt_done; */
      if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.reduce_speed_alt_done)
      {
        /* Transition: '<S8>:2762' */
        system_state_machine_DW.is_FW_RTH =
          system_state_machine_IN_FW_RTH_align_with_circle;

        /* Entry 'FW_RTH_align_with_circle': '<S8>:2767' */
        /* '<S8>:2767:3' sMode_data.fwrth_data.phase = eFWRTH_phase.align_circle; */
        rty_mode_data->fwrth_data.phase = align_circle;
      }
      break;
    }

    if (system_state_machine_DW.exit_port_index_FW_RTH == ((uint32_T)2U)) {
      system_state_machine_DW.exit_port_index_FW_RTH = 0U;

      /* Transition: '<S8>:2812' */
      /* Transition: '<S8>:2800' */
      system_state_machine_DW.temporalCounter_i1 = 0U;
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_BackTransition;
      system_state_machine_fj4fquesko(rtu_sensor_pos_lla, rtu_sensor_chi,
        rty_vom_status, rty_mode_data);

      /* Entry Internal 'BackTransition': '<S8>:2709' */
      /* Transition: '<S8>:2719' */
      system_state_machine_DW.temporalCounter_i2 = 0U;
      system_state_machine_DW.is_BackTransition =
        system_state_machine_IN_SpeedCheck;
    }
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_odubvnrnpm(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], vom_t *rty_vom_status, busMode_data
  *rty_mode_data)
{
  /* Entry Internal 'HOVER': '<S8>:280' */
  /* Transition: '<S8>:2189' */
  system_state_machine_DW.is_HOVER = system_state_machine_IN_nominal_hover;

  /* Entry 'nominal_hover': '<S8>:2188' */
  /* '<S8>:2188:4' vom_status=vom_t.VOM_HOVER; */
  *rty_vom_status = VOM_HOVER;

  /* '<S8>:2188:5' sMode_data.hover_data.hover_x=sLLA(1); */
  rty_mode_data->hover_data.hover_x = rtu_sensor_pos_lla[0];

  /* '<S8>:2188:6' sMode_data.hover_data.hover_y=sLLA(2); */
  rty_mode_data->hover_data.hover_y = rtu_sensor_pos_lla[1];

  /* '<S8>:2188:7' sMode_data.hover_data.hover_yaw_ref=dYaw_rad; */
  rty_mode_data->hover_data.hover_yaw_ref = rtu_sensor_eul_ang[2];

  /* '<S8>:2188:8' sMode_data.hover_data.hover_yaw_override = false; */
  rty_mode_data->hover_data.hover_yaw_override = false;

  /* '<S8>:2188:9' bHover2WPN_yaw_aligned = false; */
  system_state_machine_DW.bHover2WPN_yaw_aligned = false;

  /* '<S8>:2188:10' if bTakeoff2Hover */
  if (system_state_machine_DW.bTakeoff2Hover) {
    /* '<S8>:2188:11' bTakeoff2Hover = false; */
    system_state_machine_DW.bTakeoff2Hover = false;

    /* '<S8>:2188:12' sMode_data.hover_data.hover_alt_agl=sMode_data.autotakeoff_data.takeoff_alt_agl; */
    rty_mode_data->hover_data.hover_alt_agl =
      rty_mode_data->autotakeoff_data.takeoff_alt_agl;
  } else {
    /* '<S8>:2188:13' else */
    /* '<S8>:2188:14' sMode_data.hover_data.hover_alt_agl=sLLA(3); */
    rty_mode_data->hover_data.hover_alt_agl = rtu_sensor_pos_lla[2];
  }

  /* '<S8>:2188:16' if bBTransition2Hover */
  if (system_state_machine_DW.bBTransition2Hover) {
    /* '<S8>:2188:17' sMode_data.hover_data.hover_x = sMode_data.bt_data.BT_Hover_Lat; */
    rty_mode_data->hover_data.hover_x = rty_mode_data->bt_data.BT_Hover_Lat;

    /* '<S8>:2188:18' sMode_data.hover_data.hover_y = sMode_data.bt_data.BT_Hover_Lon; */
    rty_mode_data->hover_data.hover_y = rty_mode_data->bt_data.BT_Hover_Lon;

    /* '<S8>:2188:19' sMode_data.hover_data.hover_alt_agl  = sMode_data.bt_data.BT_Altitude; */
    rty_mode_data->hover_data.hover_alt_agl = rty_mode_data->bt_data.BT_Altitude;

    /* '<S8>:2188:20' sMode_data.hover_data.hover_yaw_ref=dYaw_rad; */
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_gjywfyiloe(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const real_T *rtu_sensor_aspd_cas, const
  real_T *rtu_sensor_chi, const boolean_T *rtu_wp_data_wp_list_valid, vom_t
  *rty_vom_status, busMode_data *rty_mode_data)
{
  real_T LLA2[3];
  real_T Add1;
  real_T Lat_cfuwhmfptw;
  real_T Lon_ikb0u2i02a;
  real_T TrigonometricFunction;
  real_T dist_buzfv3hgub;
  real_T heading_ieyh5igw0r;
  boolean_T guard1;
  boolean_T timecheck;

  /* During 'FW_Modes': '<S8>:1443' */
  guard1 = false;
  switch (system_state_machine_DW.is_FW_Modes) {
   case system_state_machine_IN_BackTransition:
    /* During 'BackTransition': '<S8>:2709' */
    /* '<S8>:2709:17' sMode_data.bt_data.BT_RollCmd=0; */
    rty_mode_data->bt_data.BT_RollCmd = 0.0;

    /* '<S8>:2709:18' distance=CurrentDistance_From_LLA(currentLLA,sLLA); */
    LLA2[0] = rtu_sensor_pos_lla[0];
    LLA2[1] = rtu_sensor_pos_lla[1];
    LLA2[2] = rtu_sensor_pos_lla[2];

    /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA' */
    /* Simulink Function 'CurrentDistance_From_LLA': '<S8>:2745' */
    system_state_machine_k5j3b5svff(system_state_machine_DW.currentLLA, LLA2,
      &TrigonometricFunction);

    /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.BackTransition.CurrentDistance_From_LLA' */
    /* '<S8>:2709:19' timecheck= after(60,sec); */
    timecheck = (system_state_machine_DW.temporalCounter_i1 >= ((uint16_T)6000));
    if (system_state_machine_DW.is_BackTransition == ((uint8_T)
         system_state_machine_IN_Lifter_Status)) {
      /* During 'Lifter_Status': '<S8>:2734' */
      /* '<S8>:2728:1' sf_internal_predicateOutput = dAirspeedCAS<10; */
      if (((*rtu_sensor_aspd_cas) < 10.0) &&
          (system_state_machine_DW.temporalCounter_i2 >= ((uint8_T)100))) {
        /* Transition: '<S8>:2728' */
        /* '<S8>:2731:1' sf_internal_predicateOutput = (after (1, sec)); */
        /* Transition: '<S8>:2731' */
        system_state_machine_DW.is_BackTransition =
          system_state_machine_IN_NO_ACTIVE_CHILD;
        system_state_machine_DW.exit_port_index_BackTransition = 3U;

        /* Transition: '<S8>:2726' */
        /* Transition: '<S8>:2727' */
        /* Transition: '<S8>:2729' */
        /* '<S8>:2732:1' sf_internal_predicateOutput = distance>700 ||  timecheck==true; */
      } else if ((TrigonometricFunction > 700.0) || timecheck) {
        /* Transition: '<S8>:2732' */
        system_state_machine_DW.is_BackTransition =
          system_state_machine_IN_NO_ACTIVE_CHILD;
        system_state_machine_DW.exit_port_index_BackTransition = 2U;
      } else {
        /* Transition: '<S8>:2730' */
      }

      /* During 'SpeedCheck': '<S8>:2733' */
      /* '<S8>:2722:1' sf_internal_predicateOutput = dAirspeedCAS<21; */
    } else if (((*rtu_sensor_aspd_cas) < 21.0) &&
               (system_state_machine_DW.temporalCounter_i2 >= ((uint8_T)100))) {
      /* Transition: '<S8>:2722' */
      /* '<S8>:2724:1' sf_internal_predicateOutput = (after (1, sec)); */
      /* Transition: '<S8>:2724' */
      system_state_machine_DW.temporalCounter_i2 = 0U;
      system_state_machine_DW.is_BackTransition =
        system_state_machine_IN_Lifter_Status;

      /* Entry 'Lifter_Status': '<S8>:2734' */
      /* '<S8>:2734:3' sMode_data.eFWLifter_Mode=lifter_state_t.ON; */
      rty_mode_data->eFWLifter_Mode = ON;

      /* Transition: '<S8>:2720' */
      /* Transition: '<S8>:2721' */
      /* Transition: '<S8>:2723' */
      /* Transition: '<S8>:2725' */
      /* Transition: '<S8>:2727' */
      /* '<S8>:2732:1' sf_internal_predicateOutput = distance>700 ||  timecheck==true; */
    } else if ((TrigonometricFunction > 700.0) || timecheck) {
      /* Transition: '<S8>:2732' */
      system_state_machine_DW.is_BackTransition =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_BackTransition = 2U;
    } else {
      /* Transition: '<S8>:2730' */
    }

    switch (system_state_machine_DW.exit_port_index_BackTransition) {
     case 2:
      system_state_machine_DW.exit_port_index_BackTransition = 0U;

      /* Transition: '<S8>:2804' */
      /* Exit 'BackTransition': '<S8>:2709' */
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_FW_Modes = 2U;
      break;

     case 3:
      system_state_machine_DW.exit_port_index_BackTransition = 0U;

      /* Transition: '<S8>:2811' */
      /* Exit 'BackTransition': '<S8>:2709' */
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_NO_ACTIVE_CHILD;
      system_state_machine_DW.exit_port_index_FW_Modes = 3U;
      break;

     default:
      /* no actions */
      break;
    }
    break;

   case system_state_machine_IN_FTransition:
    system_state_machine_hdnpt4qldm(rtu_sensor_pos_lla, rtu_sensor_aspd_cas,
      rtu_sensor_chi, rty_vom_status, rty_mode_data);
    break;

   case system_state_machine_IN_FW_RTH:
    system_state_machine_hqecrwkbbn(&TrigonometricFunction, &Add1,
      &Lat_cfuwhmfptw, &Lon_ikb0u2i02a, &dist_buzfv3hgub, &heading_ieyh5igw0r,
      rtu_sensor_pos_lla, rtu_sensor_chi, rty_vom_status, rty_mode_data);
    break;

   case system_state_machine_IN_FlightDirector_Mode:
    /* During 'FlightDirector_Mode': '<S8>:2746' */
    /* '<S8>:2802:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_LOITER; */
    switch (system_state_machine_B.Switch_d533txexjk) {
     case VOM_LOITER:
      /* Transition: '<S8>:2802' */
      /* '<S8>:2802:1' bFDirector2LoiterFlag=1; */
      system_state_machine_DW.is_FW_Modes = system_state_machine_IN_Loiter_Mode;

      /* Entry 'Loiter_Mode': '<S8>:2748' */
      /* '<S8>:2748:3' vom_status = vom_t.VOM_LOITER; */
      *rty_vom_status = VOM_LOITER;

      /* '<S8>:2748:4' sMode_data.eFWLifter_Mode= lifter_state_t.OFF; */
      rty_mode_data->eFWLifter_Mode = OFF;

      /* '<S8>:2748:5' sMode_data.loiter_data.loiter_radius = 200; */
      rty_mode_data->loiter_data.loiter_radius = 200.0;

      /* '<S8>:2748:6' sMode_data.loiter_data.loiter_direction = -1; */
      rty_mode_data->loiter_data.loiter_direction = -1.0;

      /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint' */
      /* CCW */
      /* '<S8>:2748:7' [sMode_data.loiter_data.loiter_Center_Lat, sMode_data.loiter_data.loiter_Center_Lon] = destinationPoint(sLLA(1), sLLA(2),250, dChi_rad); */
      /* Simulink Function 'destinationPoint': '<S8>:2755' */
      system_state_machine_mkamotaax2(rtu_sensor_pos_lla[0], rtu_sensor_pos_lla
        [1], 250.0, *rtu_sensor_chi, &TrigonometricFunction, &Add1);

      /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.FW_Modes.Loiter_Mode.destinationPoint' */
      rty_mode_data->loiter_data.loiter_Center_Lat = TrigonometricFunction;
      rty_mode_data->loiter_data.loiter_Center_Lon = Add1;

      /* '<S8>:2748:8' sMode_data.loiter_data.loiter_altitude = sLLA(3); */
      rty_mode_data->loiter_data.loiter_altitude = rtu_sensor_pos_lla[2];

      /* m */
      /* '<S8>:2748:9' sMode_data.loiter_data.loiter_AirSpeedRef = 25; */
      rty_mode_data->loiter_data.loiter_AirSpeedRef = 25.0;

      /* mps */
      break;

     case VOM_HOVER:
      /* '<S8>:2801:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER; */
      /* Transition: '<S8>:2801' */
      system_state_machine_DW.temporalCounter_i1 = 0U;
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_BackTransition;
      system_state_machine_fj4fquesko(rtu_sensor_pos_lla, rtu_sensor_chi,
        rty_vom_status, rty_mode_data);

      /* Entry Internal 'BackTransition': '<S8>:2709' */
      /* Transition: '<S8>:2719' */
      system_state_machine_DW.temporalCounter_i2 = 0U;
      system_state_machine_DW.is_BackTransition =
        system_state_machine_IN_SpeedCheck;
      break;

     default:
      /* Transition: '<S8>:2814' */
      /* '<S8>:2798:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_FW_RTH... */
      /* '<S8>:2798:2' || bTelemtryLinkLoss5sec==true; */
      if ((system_state_machine_B.Switch_d533txexjk == VOM_FW_RTH) ||
          (system_state_machine_DW.bTelemtryLinkLoss5sec)) {
        /* Transition: '<S8>:2798' */
        system_state_machine_DW.is_FW_Modes = system_state_machine_IN_FW_RTH;

        /* Entry 'FW_RTH': '<S8>:2758' */
        /* '<S8>:2758:3' vom_status=vom_t.VOM_FW_RTH; */
        *rty_vom_status = VOM_FW_RTH;

        /* '<S8>:2758:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
        rty_mode_data->eFWLifter_Mode = OFF;

        /* '<S8>:2758:5' sMode_data.fwrth_data.CAS_sp = 25.0; */
        rty_mode_data->fwrth_data.CAS_sp = 25.0;

        /* '<S8>:2758:6' sMode_data.fwrth_data.alt_sp = sLLA(3); */
        rty_mode_data->fwrth_data.alt_sp = rtu_sensor_pos_lla[2];

        /* Entry Internal 'FW_RTH': '<S8>:2758' */
        /* Transition: '<S8>:2760' */
        system_state_machine_DW.is_FW_RTH =
          system_state_machine_IN_FW_RTH_approach_circle;

        /* Entry 'FW_RTH_approach_circle': '<S8>:2765' */
        /* '<S8>:2765:3' sMode_data.fwrth_data.phase = eFWRTH_phase.approach_circle; */
        rty_mode_data->fwrth_data.phase = approach_circle;

        /* '<S8>:2815:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_WAYPNT && wp_list_valid; */
      } else if ((system_state_machine_B.Switch_d533txexjk == VOM_WAYPNT) &&
                 (*rtu_wp_data_wp_list_valid)) {
        /* Transition: '<S8>:2815' */
        system_state_machine_DW.is_FW_Modes = system_state_machine_IN_WP_Mode;

        /* Entry 'WP_Mode': '<S8>:2796' */
        /* '<S8>:2796:3' vom_status=vom_t.VOM_WAYPNT; */
        *rty_vom_status = VOM_WAYPNT;

        /* '<S8>:2796:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
        rty_mode_data->eFWLifter_Mode = OFF;

        /* '<S8>:2796:5' goto_back_trans = false; */
        system_state_machine_DW.goto_back_trans = false;
      } else {
        /* no actions */
      }
      break;
    }
    break;

   case system_state_machine_IN_Loiter_Mode:
    /* During 'Loiter_Mode': '<S8>:2748' */
    /* '<S8>:2825:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_FLTDIR; */
    if (system_state_machine_B.Switch_d533txexjk == VOM_FLTDIR) {
      /* Transition: '<S8>:2825' */
      /* Exit 'Loiter_Mode': '<S8>:2748' */
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_FlightDirector_Mode;

      /* Entry 'FlightDirector_Mode': '<S8>:2746' */
      /* '<S8>:2746:3' vom_status=vom_t.VOM_FLTDIR; */
      *rty_vom_status = VOM_FLTDIR;

      /* '<S8>:2746:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
      rty_mode_data->eFWLifter_Mode = OFF;

      /* '<S8>:2746:5' sMode_data.flightDirector_data.FD_CoG=dChi_rad; */
      rty_mode_data->flightDirector_data.FD_CoG = *rtu_sensor_chi;

      /* '<S8>:2746:6' sMode_data.flightDirector_data.FD_AirspeedRef=25; */
      rty_mode_data->flightDirector_data.FD_AirspeedRef = 25.0;

      /* '<S8>:2746:7' sMode_data.flightDirector_data.FD_Altitude=sLLA(3); */
      rty_mode_data->flightDirector_data.FD_Altitude = rtu_sensor_pos_lla[2];

      /* '<S8>:2821:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_WAYPNT && wp_list_valid==true; */
    } else if ((system_state_machine_B.Switch_d533txexjk == VOM_WAYPNT) &&
               (*rtu_wp_data_wp_list_valid)) {
      /* Transition: '<S8>:2821' */
      /* Exit 'Loiter_Mode': '<S8>:2748' */
      system_state_machine_DW.is_FW_Modes = system_state_machine_IN_WP_Mode;

      /* Entry 'WP_Mode': '<S8>:2796' */
      /* '<S8>:2796:3' vom_status=vom_t.VOM_WAYPNT; */
      *rty_vom_status = VOM_WAYPNT;

      /* '<S8>:2796:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
      rty_mode_data->eFWLifter_Mode = OFF;

      /* '<S8>:2796:5' goto_back_trans = false; */
      system_state_machine_DW.goto_back_trans = false;

      /* '<S8>:2806:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_FW_RTH; */
    } else if (system_state_machine_B.Switch_d533txexjk == VOM_FW_RTH) {
      /* Transition: '<S8>:2806' */
      guard1 = true;

      /* Transition: '<S8>:2822' */
      /* '<S8>:2805:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER; */
    } else if (system_state_machine_B.Switch_d533txexjk == VOM_HOVER) {
      /* Transition: '<S8>:2805' */
      /* Transition: '<S8>:2823' */
      /* Exit 'Loiter_Mode': '<S8>:2748' */
      system_state_machine_DW.temporalCounter_i1 = 0U;
      system_state_machine_DW.is_FW_Modes =
        system_state_machine_IN_BackTransition;
      system_state_machine_fj4fquesko(rtu_sensor_pos_lla, rtu_sensor_chi,
        rty_vom_status, rty_mode_data);

      /* Entry Internal 'BackTransition': '<S8>:2709' */
      /* Transition: '<S8>:2719' */
      system_state_machine_DW.temporalCounter_i2 = 0U;
      system_state_machine_DW.is_BackTransition =
        system_state_machine_IN_SpeedCheck;

      /* '<S8>:2810:1' sf_internal_predicateOutput = bTelemtryLinkLoss5sec==true; */
    } else if (system_state_machine_DW.bTelemtryLinkLoss5sec) {
      /* Transition: '<S8>:2810' */
      /* '<S8>:2810:1' bL2RTH_LinkLossFlag=1; */
      system_state_machine_DW.bL2RTH_LinkLossFlag = 1.0;
      guard1 = true;
    } else {
      /* no actions */
    }
    break;

   default:
    system_state_machine_odhkbmem0z(&TrigonometricFunction, &Add1,
      &Lat_cfuwhmfptw, &Lon_ikb0u2i02a, &dist_buzfv3hgub, &heading_ieyh5igw0r,
      rtu_sensor_pos_lla, rtu_sensor_chi, rty_vom_status, rty_mode_data);
    break;
  }

  if (guard1) {
    /* Exit 'Loiter_Mode': '<S8>:2748' */
    system_state_machine_DW.is_FW_Modes = system_state_machine_IN_FW_RTH;

    /* Entry 'FW_RTH': '<S8>:2758' */
    /* '<S8>:2758:3' vom_status=vom_t.VOM_FW_RTH; */
    *rty_vom_status = VOM_FW_RTH;

    /* '<S8>:2758:4' sMode_data.eFWLifter_Mode=lifter_state_t.OFF; */
    rty_mode_data->eFWLifter_Mode = OFF;

    /* '<S8>:2758:5' sMode_data.fwrth_data.CAS_sp = 25.0; */
    rty_mode_data->fwrth_data.CAS_sp = 25.0;

    /* '<S8>:2758:6' sMode_data.fwrth_data.alt_sp = sLLA(3); */
    rty_mode_data->fwrth_data.alt_sp = rtu_sensor_pos_lla[2];

    /* Entry Internal 'FW_RTH': '<S8>:2758' */
    /* Transition: '<S8>:2760' */
    system_state_machine_DW.is_FW_RTH =
      system_state_machine_IN_FW_RTH_approach_circle;

    /* Entry 'FW_RTH_approach_circle': '<S8>:2765' */
    /* '<S8>:2765:3' sMode_data.fwrth_data.phase = eFWRTH_phase.approach_circle; */
    rty_mode_data->fwrth_data.phase = approach_circle;
  }

  switch (system_state_machine_DW.exit_port_index_FW_Modes) {
   case 2:
    system_state_machine_DW.exit_port_index_FW_Modes = 0U;

    /* Transition: '<S8>:2154' */
    /* Transition: '<S8>:2155' */
    system_state_machine_DW.is_FMM = system_state_machine_IN_RTH;

    /* Entry 'RTH': '<S8>:1035' */
    /* '<S8>:1035:3' vom_status=vom_t.VOM_MR_RTH; */
    *rty_vom_status = VOM_MR_RTH;

    /* '<S8>:1035:4' bRTH2LandingFlag=0; */
    system_state_machine_DW.bRTH2LandingFlag = 0.0;

    /* '<S8>:1035:5' sMode_data.rth_data.bRTH_PosModeFlag=false; */
    rty_mode_data->rth_data.bRTH_PosModeFlag = false;

    /* '<S8>:1035:6' sMode_data.rth_data.bBreaking=false; */
    rty_mode_data->rth_data.bBreaking = false;

    /* Entry Internal 'RTH': '<S8>:1035' */
    /* Transition: '<S8>:1038' */
    system_state_machine_DW.is_RTH = system_state_machine_IN_RTH_Speed_Check;

    /* Entry 'RTH_Speed_Check': '<S8>:1037' */
    /* '<S8>:1037:3' sMode_data.rth_data.bBreaking=true; */
    rty_mode_data->rth_data.bBreaking = true;

    /* '<S8>:1037:4' sMode_data.rth_data.rth_velX=0; */
    rty_mode_data->rth_data.rth_velX = 0.0;

    /* '<S8>:1037:5' sMode_data.rth_data.rth_velY=0; */
    rty_mode_data->rth_data.rth_velY = 0.0;

    /* '<S8>:1037:6' sMode_data.rth_data.rth_alt_agl=sLLA(3); */
    rty_mode_data->rth_data.rth_alt_agl = rtu_sensor_pos_lla[2];

    /* '<S8>:1037:7' sMode_data.rth_data.rth_yaw_ref=dYaw_rad; */
    rty_mode_data->rth_data.rth_yaw_ref = rtu_sensor_eul_ang[2];

    /* '<S8>:1037:8' sMode_data.rth_data.rth_x= sLLA(1); */
    rty_mode_data->rth_data.rth_x = rtu_sensor_pos_lla[0];

    /* '<S8>:1037:9' sMode_data.rth_data.rth_y= sLLA(2); */
    rty_mode_data->rth_data.rth_y = rtu_sensor_pos_lla[1];
    break;

   case 3:
    system_state_machine_DW.exit_port_index_FW_Modes = 0U;

    /* Transition: '<S8>:2002' */
    /* '<S8>:2002:1' bBTransition2Hover=true; */
    system_state_machine_DW.bBTransition2Hover = true;
    system_state_machine_DW.is_FMM = system_state_machine_IN_HOVER;

    /* Entry 'HOVER': '<S8>:280' */
    /* during */
    system_state_machine_odubvnrnpm(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rty_vom_status, rty_mode_data);
    break;

   default:
    /* no actions */
    break;
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_jgwda1ajrs(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], vom_t *rty_vom_status, busMode_data
  *rty_mode_data)
{
  /* Entry 'AUTOLAND': '<S8>:281' */
  /* '<S8>:281:2' vom_status=vom_t.VOM_LAND; */
  *rty_vom_status = VOM_LAND;

  /* '<S8>:281:3' if (bRTH2LandingFlag==1) */
  if (system_state_machine_DW.bRTH2LandingFlag == 1.0) {
    /* '<S8>:281:4' bRTH2LandingFlag = 0; */
    system_state_machine_DW.bRTH2LandingFlag = 0.0;

    /* '<S8>:281:5' sMode_data.autoland_data.land_x=dHomeLLA(1); */
    rty_mode_data->autoland_data.land_x = system_state_machine_B.sLLA[0];

    /* '<S8>:281:6' sMode_data.autoland_data.land_y=dHomeLLA(2); */
    rty_mode_data->autoland_data.land_y = system_state_machine_B.sLLA[1];

    /* '<S8>:281:7' sMode_data.autoland_data.land_yaw_ref=dYaw_rad */
    rty_mode_data->autoland_data.land_yaw_ref = rtu_sensor_eul_ang[2];
  } else if (system_state_machine_DW.bRTH2LandingFlag == 0.0) {
    /* '<S8>:281:8' elseif (bRTH2LandingFlag==0) */
    /* '<S8>:281:9' sMode_data.autoland_data.land_x=sLLA(1); */
    rty_mode_data->autoland_data.land_x = rtu_sensor_pos_lla[0];

    /* '<S8>:281:10' sMode_data.autoland_data.land_y=sLLA(2); */
    rty_mode_data->autoland_data.land_y = rtu_sensor_pos_lla[1];

    /* '<S8>:281:11' sMode_data.autoland_data.land_yaw_ref=dYaw_rad */
    rty_mode_data->autoland_data.land_yaw_ref = rtu_sensor_eul_ang[2];
  } else if (system_state_machine_DW.bRecoverGPS == 1.0) {
    /* '<S8>:281:12' elseif(bRecoverGPS==true) */
    /* '<S8>:281:13' sMode_data.autoland_data.land_x=sLLA(1); */
    rty_mode_data->autoland_data.land_x = rtu_sensor_pos_lla[0];

    /* '<S8>:281:14' sMode_data.autoland_data.land_y=sLLA(2); */
    rty_mode_data->autoland_data.land_y = rtu_sensor_pos_lla[1];

    /* '<S8>:281:15' sMode_data.autoland_data.land_yaw_ref=dYaw_rad */
    rty_mode_data->autoland_data.land_yaw_ref = rtu_sensor_eul_ang[2];
  } else {
    /* no actions */
  }

  /* if (LandCondition==1) */
  /*  land_x=hover_x; */
  /* land_y=hover_y; */
  /* land_yaw_ref=dYaw_rad; */
  /* elseif (LandCondition==2) */
  /*  land_x=sLLA(1); */
  /*   land_y=sLLA(2); */
  /*   land_yaw_ref=dYaw_rad; */
  /* else */
  /*    land_x=sLLA(1); */
  /*  land_y=sLLA(2); */
  /*  land_yaw_ref=dYaw_rad; */
  /* end */
  /* during: */
  /* if (sContingencyFlags.bTelemtry_LinkLoss==true) */
  /*   bLand_GPSLossFlag=false; */
  /* else */
  /*  bLand_GPSLossFlag=true; */
  /* end */
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_aeuawg3vfo(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const int8_T *rtu_pilot_input_switch_1,
  const boolean_T *rtu_wp_data_wp_list_valid, vom_t *rty_vom_status,
  busMode_data *rty_mode_data, boolean_T *rty_sFlags_bGPSLoss)
{
  real_T Abs_mmz3qadiva;
  real_T u;
  boolean_T b_out;
  boolean_T guard1;

  /* During 'HOVER': '<S8>:280' */
  /* Transition: '<S8>:141' */
  /* Transition: '<S8>:156' */
  /* '<S8>:139:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_MANUAL || (iRCoverride==1 && sContingencyFlags.ep_data_loss==0 ); */
  if ((system_state_machine_B.Switch_d533txexjk == VOM_MANUAL) ||
      (((*rtu_pilot_input_switch_1) == ((int8_T)1)) &&
       (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)))
  {
    /* Transition: '<S8>:139' */
    /* Exit Internal 'HOVER': '<S8>:280' */
    system_state_machine_DW.is_HOVER = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'HOVER': '<S8>:280' */
    /* '<S8>:280:5' sMode_data.hover_data.hover_yaw_override = false; */
    rty_mode_data->hover_data.hover_yaw_override = false;

    /* '<S8>:280:6' bBTransition2Hover=false; */
    system_state_machine_DW.bBTransition2Hover = false;

    /* '<S8>:280:7' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:280:8' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_MANUAL;

    /* Entry 'MANUAL': '<S8>:276' */
    /* '<S8>:276:3' vom_status=vom_t.VOM_MANUAL; */
    *rty_vom_status = VOM_MANUAL;
  } else {
    /* '<S8>:153:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_LAND || bGPSLoss==true; */
    b_out = ((system_state_machine_B.Switch_d533txexjk == VOM_LAND) ||
             (*rty_sFlags_bGPSLoss));
    if (b_out) {
      /* Transition: '<S8>:153' */
      /* Exit Internal 'HOVER': '<S8>:280' */
      system_state_machine_DW.is_HOVER = system_state_machine_IN_NO_ACTIVE_CHILD;

      /* Exit 'HOVER': '<S8>:280' */
      /* '<S8>:280:5' sMode_data.hover_data.hover_yaw_override = false; */
      rty_mode_data->hover_data.hover_yaw_override = false;

      /* '<S8>:280:6' bBTransition2Hover=false; */
      system_state_machine_DW.bBTransition2Hover = false;

      /* '<S8>:280:7' if bRecoverGPS==1 */
      if (system_state_machine_DW.bRecoverGPS == 1.0) {
        /* '<S8>:280:8' bRecoverGPS=0; */
        system_state_machine_DW.bRecoverGPS = 0.0;
      }

      system_state_machine_DW.is_FMM = system_state_machine_IN_AUTOLAND;
      system_state_machine_jgwda1ajrs(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
        rty_vom_status, rty_mode_data);

      /* '<S8>:1036:1' sf_internal_predicateOutput = (bEPIPLossFlag==true) || (bTelemtryLinkLoss5sec==true) || vom_cmd==vom_t.VOM_MR_RTH; */
    } else if (((system_state_machine_DW.bEPIPLossFlag) ||
                (system_state_machine_DW.bTelemtryLinkLoss5sec)) ||
               (system_state_machine_B.Switch_d533txexjk == VOM_MR_RTH)) {
      /* Transition: '<S8>:1036' */
      /* Exit Internal 'HOVER': '<S8>:280' */
      system_state_machine_DW.is_HOVER = system_state_machine_IN_NO_ACTIVE_CHILD;

      /* Exit 'HOVER': '<S8>:280' */
      /* '<S8>:280:5' sMode_data.hover_data.hover_yaw_override = false; */
      rty_mode_data->hover_data.hover_yaw_override = false;

      /* '<S8>:280:6' bBTransition2Hover=false; */
      system_state_machine_DW.bBTransition2Hover = false;

      /* '<S8>:280:7' if bRecoverGPS==1 */
      if (system_state_machine_DW.bRecoverGPS == 1.0) {
        /* '<S8>:280:8' bRecoverGPS=0; */
        system_state_machine_DW.bRecoverGPS = 0.0;
      }

      system_state_machine_DW.is_FMM = system_state_machine_IN_RTH;

      /* Entry 'RTH': '<S8>:1035' */
      /* '<S8>:1035:3' vom_status=vom_t.VOM_MR_RTH; */
      *rty_vom_status = VOM_MR_RTH;

      /* '<S8>:1035:4' bRTH2LandingFlag=0; */
      system_state_machine_DW.bRTH2LandingFlag = 0.0;

      /* '<S8>:1035:5' sMode_data.rth_data.bRTH_PosModeFlag=false; */
      rty_mode_data->rth_data.bRTH_PosModeFlag = false;

      /* '<S8>:1035:6' sMode_data.rth_data.bBreaking=false; */
      rty_mode_data->rth_data.bBreaking = false;

      /* Entry Internal 'RTH': '<S8>:1035' */
      /* Transition: '<S8>:1038' */
      system_state_machine_DW.is_RTH = system_state_machine_IN_RTH_Speed_Check;

      /* Entry 'RTH_Speed_Check': '<S8>:1037' */
      /* '<S8>:1037:3' sMode_data.rth_data.bBreaking=true; */
      rty_mode_data->rth_data.bBreaking = true;

      /* '<S8>:1037:4' sMode_data.rth_data.rth_velX=0; */
      rty_mode_data->rth_data.rth_velX = 0.0;

      /* '<S8>:1037:5' sMode_data.rth_data.rth_velY=0; */
      rty_mode_data->rth_data.rth_velY = 0.0;

      /* '<S8>:1037:6' sMode_data.rth_data.rth_alt_agl=sLLA(3); */
      rty_mode_data->rth_data.rth_alt_agl = rtu_sensor_pos_lla[2];

      /* '<S8>:1037:7' sMode_data.rth_data.rth_yaw_ref=dYaw_rad; */
      rty_mode_data->rth_data.rth_yaw_ref = rtu_sensor_eul_ang[2];

      /* '<S8>:1037:8' sMode_data.rth_data.rth_x= sLLA(1); */
      rty_mode_data->rth_data.rth_x = rtu_sensor_pos_lla[0];

      /* '<S8>:1037:9' sMode_data.rth_data.rth_y= sLLA(2); */
      rty_mode_data->rth_data.rth_y = rtu_sensor_pos_lla[1];
    } else {
      /* '<S8>:1479:1' sf_internal_predicateOutput =  bHover2WPN_yaw_aligned && dGroundSpeed_mps <=2; */
      guard1 = false;
      if ((system_state_machine_DW.bHover2WPN_yaw_aligned) &&
          (system_state_machine_B.Max <= 2.0)) {
        /* Transition: '<S8>:1479' */
        /* '<S8>:1479:1' hover2WAYPNT=true; */
        system_state_machine_DW.hover2WAYPNT = true;
        guard1 = true;

        /* '<S8>:2340:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_FLTDIR && dGroundSpeed_mps <=2; */
      } else if ((system_state_machine_B.Switch_d533txexjk == VOM_FLTDIR) &&
                 (system_state_machine_B.Max <= 2.0)) {
        /* Transition: '<S8>:2340' */
        /* '<S8>:2340:1' hover2FLTDIR=true; */
        system_state_machine_DW.hover2FLTDIR = true;
        guard1 = true;
      } else {
        /* during */
        switch (system_state_machine_DW.is_HOVER) {
         case system_state_machine_IN_align_yaw_with_WP:
          /* During 'align_yaw_with_WP': '<S8>:2190' */
          /* '<S8>:2193:1' sf_internal_predicateOutput = wraptopi(WP_SMdata_in.curpos_to_wp_heading - dYaw_rad) < 5*pi/180; */
          u =
            system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.curpos_to_wp_heading
            - rtu_sensor_eul_ang[2];

          /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.HOVER.wraptopi' */
          /* Simulink Function 'wraptopi': '<S8>:2196' */
          system_state_machine_lkpmp3sm0z(u, &Abs_mmz3qadiva,
            &system_state_machine_ConstB.FlightManagementFMMHOVERwraptopi);

          /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.HOVER.wraptopi' */
          if (Abs_mmz3qadiva < 0.087266462599716474) {
            /* Transition: '<S8>:2193' */
            system_state_machine_DW.is_HOVER =
              system_state_machine_IN_exit_to_F_Trans;

            /* Entry 'exit_to_F_Trans': '<S8>:2192' */
            /* '<S8>:2192:2' bHover2WPN_yaw_aligned = true; */
            system_state_machine_DW.bHover2WPN_yaw_aligned = true;

            /* '<S8>:2192:3' sMode_data.hover_data.hover_yaw_override = false; */
            rty_mode_data->hover_data.hover_yaw_override = false;
          }
          break;

         case system_state_machine_IN_exit_to_F_Trans:
          /* During 'exit_to_F_Trans': '<S8>:2192' */
          /* '<S8>:2192:2' bHover2WPN_yaw_aligned = true; */
          system_state_machine_DW.bHover2WPN_yaw_aligned = true;

          /* '<S8>:2192:3' sMode_data.hover_data.hover_yaw_override = false; */
          rty_mode_data->hover_data.hover_yaw_override = false;
          break;

         default:
          /* During 'nominal_hover': '<S8>:2188' */
          /* '<S8>:2191:1' sf_internal_predicateOutput = WP_SMdata_in.WPN_cmd_received && wp_list_valid; */
          if ((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.WPN_cmd_received)
              && (*rtu_wp_data_wp_list_valid)) {
            /* Transition: '<S8>:2191' */
            system_state_machine_DW.is_HOVER =
              system_state_machine_IN_align_yaw_with_WP;

            /* Entry 'align_yaw_with_WP': '<S8>:2190' */
            /* '<S8>:2190:3' sMode_data.hover_data.hover_yaw_ref=WP_SMdata_in.curpos_to_wp_heading; */
            rty_mode_data->hover_data.hover_yaw_ref =
              system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.curpos_to_wp_heading;

            /* '<S8>:2190:4' sMode_data.hover_data.hover_yaw_override = true; */
            rty_mode_data->hover_data.hover_yaw_override = true;
          }
          break;
        }
      }

      if (guard1) {
        /* Exit Internal 'HOVER': '<S8>:280' */
        system_state_machine_DW.is_HOVER =
          system_state_machine_IN_NO_ACTIVE_CHILD;

        /* Exit 'HOVER': '<S8>:280' */
        /* '<S8>:280:5' sMode_data.hover_data.hover_yaw_override = false; */
        rty_mode_data->hover_data.hover_yaw_override = false;

        /* '<S8>:280:6' bBTransition2Hover=false; */
        system_state_machine_DW.bBTransition2Hover = false;

        /* '<S8>:280:7' if bRecoverGPS==1 */
        if (system_state_machine_DW.bRecoverGPS == 1.0) {
          /* '<S8>:280:8' bRecoverGPS=0; */
          system_state_machine_DW.bRecoverGPS = 0.0;
        }

        system_state_machine_DW.is_FMM = system_state_machine_IN_FW_Modes;

        /* Entry 'FW_Modes': '<S8>:1443' */
        /* Entry Internal 'FW_Modes': '<S8>:1443' */
        /* Transition: '<S8>:2809' */
        system_state_machine_DW.is_FW_Modes =
          system_state_machine_IN_FTransition;

        /* Entry 'FTransition': '<S8>:2777' */
        /* '<S8>:2777:4' vom_status=vom_t.VOM_F_TRANS; */
        *rty_vom_status = VOM_F_TRANS;

        /* '<S8>:2777:5' sMode_data.ft_data.eLifter_Mode=lifter_state_t.ON; */
        rty_mode_data->ft_data.eLifter_Mode = ON;

        /* '<S8>:2777:6' sMode_data.ft_data.FT_x=sLLA(1); */
        rty_mode_data->ft_data.FT_x = rtu_sensor_pos_lla[0];

        /* '<S8>:2777:7' sMode_data.ft_data.FT_y=sLLA(2); */
        rty_mode_data->ft_data.FT_y = rtu_sensor_pos_lla[1];

        /* '<S8>:2777:8' sMode_data.ft_data.FT_Altitude=sLLA(3); */
        rty_mode_data->ft_data.FT_Altitude = rtu_sensor_pos_lla[2];

        /* '<S8>:2777:9' sMode_data.ft_data.FT_Heading=dYaw_rad; */
        rty_mode_data->ft_data.FT_Heading = rtu_sensor_eul_ang[2];

        /* Entry Internal 'FTransition': '<S8>:2777' */
        /* Transition: '<S8>:2782' */
        system_state_machine_DW.temporalCounter_i1 = 0U;
        system_state_machine_DW.is_FTransition =
          system_state_machine_IN_FTransition_for_FDirector;

        /* Entry 'FTransition_for_FDirector': '<S8>:2790' */
        /* '<S8>:2790:3' sMode_data.ft_data.FT_PusherThrottle=0.8; */
        rty_mode_data->ft_data.FT_PusherThrottle = 0.8;
      }
    }
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_csj5zj0zmt(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const real_T *rtu_sensor_h_radar_agl,
  const int8_T *rtu_pilot_input_switch_1, vom_t *rty_vom_status, busMode_data
  *rty_mode_data, pic_t *rty_pic_status)
{
  /* During 'MANUAL': '<S8>:276' */
  /* Transition: '<S8>:140' */
  /* '<S8>:137:1' sf_internal_predicateOutput = ( vom_cmd==vom_t.VOM_READY) || ... */
  /* '<S8>:137:2' ((bInAirFlag==0 && (sContingencyFlags.gps_loss == 1 || sContingencyFlags.ep_data_loss==1 ||  sContingencyFlags.ip_data_loss==1 || sContingencyFlags.gcs_link_loss==1))); */
  if ((system_state_machine_B.Switch_d533txexjk == VOM_READY) ||
      ((!system_state_machine_B.bInAirFlag) &&
       ((((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
          ||
          (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss))
         ||
         (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
        ||
        (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss))))
  {
    /* Transition: '<S8>:137' */
    /* Exit 'MANUAL': '<S8>:276' */
    /* '<S8>:276:5' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:276:6' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

    /* Entry 'READY': '<S8>:277' */
    /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
    *rty_vom_status = VOM_READY;

    /* '<S8>:277:4' dHoverTimer_sec=0; */
    /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
    system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;

    /* Transition: '<S8>:145' */
    /* '<S8>:142:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER && iRCoverride==0; */
  } else if ((system_state_machine_B.Switch_d533txexjk == VOM_HOVER) &&
             ((*rtu_pilot_input_switch_1) == ((int8_T)0))) {
    /* Transition: '<S8>:142' */
    /* Transition: '<S8>:152' */
    /* Exit 'MANUAL': '<S8>:276' */
    /* '<S8>:276:5' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:276:6' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_HOVER;

    /* Entry 'HOVER': '<S8>:280' */
    /* during */
    system_state_machine_odubvnrnpm(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rty_vom_status, rty_mode_data);

    /* Transition: '<S8>:146' */
    /* '<S8>:148:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_LAND && iRCoverride==0; */
  } else if (((system_state_machine_B.Switch_d533txexjk == VOM_LAND) &&
              ((*rtu_pilot_input_switch_1) == ((int8_T)0))) ||
             ((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
              && ((*rtu_sensor_h_radar_agl) < 1.0))) {
    /* Transition: '<S8>:148' */
    /* Transition: '<S8>:296' */
    /* Exit 'MANUAL': '<S8>:276' */
    /* '<S8>:276:5' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:276:6' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_AUTOLAND;
    system_state_machine_jgwda1ajrs(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rty_vom_status, rty_mode_data);

    /* Transition: '<S8>:295' */
    /* '<S8>:296:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==true && h_radar_AGL<dTakeoff_DecisionPoint; */
    /* Transition: '<S8>:1081' */
    /* '<S8>:1082:1' sf_internal_predicateOutput =  (ep_loss250ms==true&&pic_status == pic_t.EXTERNAL) ||... */
    /* '<S8>:1082:2'   (ip_loss250ms==true&&pic_status == pic_t.INTERNAL) || (vom_cmd==vom_t.VOM_MR_RTH&& iRCoverride==0); */
  } else if ((((system_state_machine_DW.ep_loss250ms) && ((*rty_pic_status) ==
                EXTERNAL)) || ((system_state_machine_DW.ip_loss250ms) &&
               ((*rty_pic_status) == INTERNAL))) ||
             ((system_state_machine_B.Switch_d533txexjk == VOM_MR_RTH) &&
              ((*rtu_pilot_input_switch_1) == ((int8_T)0)))) {
    /* Transition: '<S8>:1082' */
    /* Exit 'MANUAL': '<S8>:276' */
    /* '<S8>:276:5' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:276:6' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_RTH;

    /* Entry 'RTH': '<S8>:1035' */
    /* '<S8>:1035:3' vom_status=vom_t.VOM_MR_RTH; */
    *rty_vom_status = VOM_MR_RTH;

    /* '<S8>:1035:4' bRTH2LandingFlag=0; */
    system_state_machine_DW.bRTH2LandingFlag = 0.0;

    /* '<S8>:1035:5' sMode_data.rth_data.bRTH_PosModeFlag=false; */
    rty_mode_data->rth_data.bRTH_PosModeFlag = false;

    /* '<S8>:1035:6' sMode_data.rth_data.bBreaking=false; */
    /* Entry Internal 'RTH': '<S8>:1035' */
    /* Transition: '<S8>:1038' */
    system_state_machine_DW.is_RTH = system_state_machine_IN_RTH_Speed_Check;

    /* Entry 'RTH_Speed_Check': '<S8>:1037' */
    /* '<S8>:1037:3' sMode_data.rth_data.bBreaking=true; */
    rty_mode_data->rth_data.bBreaking = true;

    /* '<S8>:1037:4' sMode_data.rth_data.rth_velX=0; */
    rty_mode_data->rth_data.rth_velX = 0.0;

    /* '<S8>:1037:5' sMode_data.rth_data.rth_velY=0; */
    rty_mode_data->rth_data.rth_velY = 0.0;

    /* '<S8>:1037:6' sMode_data.rth_data.rth_alt_agl=sLLA(3); */
    rty_mode_data->rth_data.rth_alt_agl = rtu_sensor_pos_lla[2];

    /* '<S8>:1037:7' sMode_data.rth_data.rth_yaw_ref=dYaw_rad; */
    rty_mode_data->rth_data.rth_yaw_ref = rtu_sensor_eul_ang[2];

    /* '<S8>:1037:8' sMode_data.rth_data.rth_x= sLLA(1); */
    rty_mode_data->rth_data.rth_x = rtu_sensor_pos_lla[0];

    /* '<S8>:1037:9' sMode_data.rth_data.rth_y= sLLA(2); */
    rty_mode_data->rth_data.rth_y = rtu_sensor_pos_lla[1];
  } else {
    /* no actions */
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_fks3ydn24b(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const int8_T *rtu_pilot_input_switch_1,
  vom_t *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_bGPSLoss)
{
  real_T Abs;
  real_T u_buebzdk2nz;
  boolean_T guard1;
  boolean_T out;

  /* During 'RTH': '<S8>:1035' */
  /* Transition: '<S8>:1084' */
  /* '<S8>:1065:1' sf_internal_predicateOutput = bRTH_ExitCond==1&&bGPSLoss==0; */
  out = ((system_state_machine_B.Switch1) && (!(*rty_sFlags_bGPSLoss)));
  guard1 = false;
  if (out) {
    /* Transition: '<S8>:1065' */
    /* '<S8>:1065:1' bRTH2LandingFlag=1; */
    system_state_machine_DW.bRTH2LandingFlag = 1.0;
    guard1 = true;

    /* '<S8>:1091:1' sf_internal_predicateOutput = (vom_cmd==vom_t.VOM_MANUAL && ep_loss250ms==false) || (iRCoverride==1 && sContingencyFlags.ep_data_loss==0 ); */
  } else if (((system_state_machine_B.Switch_d533txexjk == VOM_MANUAL) &&
              (!system_state_machine_DW.ep_loss250ms)) ||
             (((*rtu_pilot_input_switch_1) == ((int8_T)1)) &&
              (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)))
  {
    /* Transition: '<S8>:1091' */
    /* Transition: '<S8>:1092' */
    /* Exit Internal 'RTH': '<S8>:1035' */
    system_state_machine_DW.is_RTH = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'RTH': '<S8>:1035' */
    /* '<S8>:1035:8' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:1035:9' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_MANUAL;

    /* Entry 'MANUAL': '<S8>:276' */
    /* '<S8>:276:3' vom_status=vom_t.VOM_MANUAL; */
    *rty_vom_status = VOM_MANUAL;

    /* '<S8>:2857:1' sf_internal_predicateOutput = bGPSLoss==true; */
  } else if (*rty_sFlags_bGPSLoss) {
    /* Transition: '<S8>:2857' */
    guard1 = true;
  } else {
    switch (system_state_machine_DW.is_RTH) {
     case system_state_machine_IN_RTH_Alt_Check:
      /* During 'RTH_Alt_Check': '<S8>:1045' */
      /* '<S8>:1048:1' sf_internal_predicateOutput = (sLLA(3)>sMode_data.rth_data.rth_alt_agl-1); */
      if (rtu_sensor_pos_lla[2] > (rty_mode_data->rth_data.rth_alt_agl - 1.0)) {
        /* Transition: '<S8>:1048' */
        system_state_machine_DW.is_RTH =
          system_state_machine_IN_RTH_Heading_Check;

        /* Entry 'RTH_Heading_Check': '<S8>:1039' */
        /* '<S8>:1039:3' sMode_data.rth_data.rth_yaw_ref=dRTH_TargetHeading_rad; */
        rty_mode_data->rth_data.rth_yaw_ref =
          system_state_machine_B.TrigonometricFunction6;
      }
      break;

     case system_state_machine_IN_RTH_Heading_Check:
      /* During 'RTH_Heading_Check': '<S8>:1039' */
      /* '<S8>:1046:1' sf_internal_predicateOutput = wraptopi(dRTH_TargetHeading_rad-dYaw_rad)<5*pi/180; */
      u_buebzdk2nz = system_state_machine_B.TrigonometricFunction6 -
        rtu_sensor_eul_ang[2];

      /* Outputs for Function Call SubSystem: '<S8>/FlightManagement.FMM.RTH.wraptopi' */
      /* Simulink Function 'wraptopi': '<S8>:1137' */
      system_state_machine_lkpmp3sm0z(u_buebzdk2nz, &Abs,
        &system_state_machine_ConstB.FlightManagementFMMRTHwraptopi);

      /* End of Outputs for SubSystem: '<S8>/FlightManagement.FMM.RTH.wraptopi' */
      if (Abs < 0.087266462599716474) {
        /* Transition: '<S8>:1046' */
        system_state_machine_DW.is_RTH = system_state_machine_IN_RTH_Pos_Check;

        /* Entry 'RTH_Pos_Check': '<S8>:1047' */
        /* '<S8>:1047:3' sMode_data.rth_data.rth_x=dHomeLLA(1); */
        rty_mode_data->rth_data.rth_x = system_state_machine_B.sLLA[0];

        /* '<S8>:1047:4' sMode_data.rth_data.rth_y=dHomeLLA(2); */
        rty_mode_data->rth_data.rth_y = system_state_machine_B.sLLA[1];

        /* '<S8>:1047:5' sMode_data.rth_data.bRTH_PosModeFlag=true; */
        rty_mode_data->rth_data.bRTH_PosModeFlag = true;
      }
      break;

     case system_state_machine_IN_RTH_Pos_Check:
      /* During 'RTH_Pos_Check': '<S8>:1047' */
      break;

     default:
      /* During 'RTH_Speed_Check': '<S8>:1037' */
      /* '<S8>:1040:1' sf_internal_predicateOutput = dGroundSpeed_mps<3; */
      if (system_state_machine_B.Max < 3.0) {
        /* Transition: '<S8>:1040' */
        system_state_machine_DW.is_RTH = system_state_machine_IN_RTH_Alt_Check;

        /* Entry 'RTH_Alt_Check': '<S8>:1045' */
        /* '<S8>:1045:3' sMode_data.rth_data.bBreaking=false; */
        rty_mode_data->rth_data.bBreaking = false;

        /* '<S8>:1045:4' sMode_data.rth_data.rth_alt_agl=dHomeLLA(3)+40; */
        rty_mode_data->rth_data.rth_alt_agl = system_state_machine_B.sLLA[2] +
          40.0;

        /* 30 */
      }
      break;
    }
  }

  if (guard1) {
    /* Exit Internal 'RTH': '<S8>:1035' */
    system_state_machine_DW.is_RTH = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'RTH': '<S8>:1035' */
    /* '<S8>:1035:8' if bRecoverGPS==1 */
    if (system_state_machine_DW.bRecoverGPS == 1.0) {
      /* '<S8>:1035:9' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }

    system_state_machine_DW.is_FMM = system_state_machine_IN_AUTOLAND;
    system_state_machine_jgwda1ajrs(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rty_vom_status, rty_mode_data);
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_afs03nhl30(const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const int8_T *rtu_pilot_input_switch_1,
  vom_t *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_rampup_phase)
{
  boolean_T guard1;

  /* During 'STARTUP': '<S8>:279' */
  /* '<S8>:333:1' sf_internal_predicateOutput = (sContingencyFlags.gps_loss == 1 || sContingencyFlags.ep_data_loss==1 ||  sContingencyFlags.ip_data_loss==1|| sContingencyFlags.gcs_link_loss==1) || ... */
  /* '<S8>:333:2' vom_cmd==vom_t.VOM_READY; */
  if (((((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
         ||
         (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss))
        ||
        (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
       ||
       (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss))
      || (system_state_machine_B.Switch_d533txexjk == VOM_READY)) {
    /* Transition: '<S8>:333' */
    /* Exit Internal 'STARTUP': '<S8>:279' */
    system_state_machine_DW.is_STARTUP = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'STARTUP': '<S8>:279' */
    /* '<S8>:279:5' rampup_phase=false; */
    *rty_sFlags_rampup_phase = false;

    /* '<S8>:279:6' startup2takeoff = false; */
    system_state_machine_DW.startup2takeoff = false;

    /* '<S8>:279:7' startup2manual = false; */
    system_state_machine_DW.startup2manual = false;

    /* '<S8>:279:8' startup2uman = false; */
    system_state_machine_DW.startup2uman = false;
    system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

    /* Entry 'READY': '<S8>:277' */
    /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
    *rty_vom_status = VOM_READY;

    /* '<S8>:277:4' dHoverTimer_sec=0; */
    /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
    system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;

    /* '<S8>:275:1' sf_internal_predicateOutput = (sContingencyFlags.gps_loss==0 && sContingencyFlags.ep_data_loss== 0 && sContingencyFlags.ip_data_loss== 0 && ... */
    /* '<S8>:275:2'  sContingencyFlags.gcs_link_loss==0)...  */
    /* '<S8>:275:3' && iRCoverride==0 && startup2takeoff &&rampup_timer>2; */
  } else if
      (((((((!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
            &&
            (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss))
           &&
           (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
          &&
          (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss))
         && ((*rtu_pilot_input_switch_1) == ((int8_T)0))) &&
        (system_state_machine_DW.startup2takeoff)) &&
       (system_state_machine_DW.rampup_timer > 2.0)) {
    /* Transition: '<S8>:275' */
    /* Exit Internal 'STARTUP': '<S8>:279' */
    system_state_machine_DW.is_STARTUP = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'STARTUP': '<S8>:279' */
    /* '<S8>:279:5' rampup_phase=false; */
    *rty_sFlags_rampup_phase = false;

    /* '<S8>:279:6' startup2takeoff = false; */
    system_state_machine_DW.startup2takeoff = false;

    /* '<S8>:279:7' startup2manual = false; */
    system_state_machine_DW.startup2manual = false;

    /* '<S8>:279:8' startup2uman = false; */
    system_state_machine_DW.startup2uman = false;
    system_state_machine_DW.is_FMM = system_state_machine_IN_AUTOTAKEOFF;

    /* Entry 'AUTOTAKEOFF': '<S8>:272' */
    /* '<S8>:272:3' vom_status=vom_t.VOM_TAKEOFF; */
    *rty_vom_status = VOM_TAKEOFF;

    /* '<S8>:272:4' sMode_data.autotakeoff_data.takeoff_yaw_ref=dYaw_rad; */
    rty_mode_data->autotakeoff_data.takeoff_yaw_ref = rtu_sensor_eul_ang[2];

    /* '<S8>:272:5' sMode_data.autotakeoff_data.takeoff_x= sLLA(1); */
    rty_mode_data->autotakeoff_data.takeoff_x = rtu_sensor_pos_lla[0];

    /* '<S8>:272:6' sMode_data.autotakeoff_data.takeoff_y= sLLA(2); */
    rty_mode_data->autotakeoff_data.takeoff_y = rtu_sensor_pos_lla[1];

    /* Constant: '<S1>/takeoff_alt' */
    /* '<S8>:272:7' sMode_data.autotakeoff_data.takeoff_alt_agl=dHomeLLA(3)+takeoff_alt; */
    rty_mode_data->autotakeoff_data.takeoff_alt_agl =
      system_state_machine_B.sLLA[2] + 20.0;

    /* '<S8>:272:8' bTakeoff2Hover = false; */
    system_state_machine_DW.bTakeoff2Hover = false;

    /* '<S8>:154:1' sf_internal_predicateOutput =  startup2manual &&rampup_timer>2; */
  } else if ((system_state_machine_DW.startup2manual) &&
             (system_state_machine_DW.rampup_timer > 2.0)) {
    /* Transition: '<S8>:154' */
    /* Exit Internal 'STARTUP': '<S8>:279' */
    system_state_machine_DW.is_STARTUP = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'STARTUP': '<S8>:279' */
    /* '<S8>:279:5' rampup_phase=false; */
    *rty_sFlags_rampup_phase = false;

    /* '<S8>:279:6' startup2takeoff = false; */
    system_state_machine_DW.startup2takeoff = false;

    /* '<S8>:279:7' startup2manual = false; */
    system_state_machine_DW.startup2manual = false;

    /* '<S8>:279:8' startup2uman = false; */
    system_state_machine_DW.startup2uman = false;
    system_state_machine_DW.is_FMM = system_state_machine_IN_MANUAL;

    /* Entry 'MANUAL': '<S8>:276' */
    /* '<S8>:276:3' vom_status=vom_t.VOM_MANUAL; */
    *rty_vom_status = VOM_MANUAL;

    /* Transition: '<S8>:1340' */
    /* '<S8>:1339:1' sf_internal_predicateOutput =  startup2uman &&rampup_timer>2; */
  } else if ((system_state_machine_DW.startup2uman) &&
             (system_state_machine_DW.rampup_timer > 2.0)) {
    /* Transition: '<S8>:1339' */
    /* Exit Internal 'STARTUP': '<S8>:279' */
    system_state_machine_DW.is_STARTUP = system_state_machine_IN_NO_ACTIVE_CHILD;

    /* Exit 'STARTUP': '<S8>:279' */
    /* '<S8>:279:5' rampup_phase=false; */
    *rty_sFlags_rampup_phase = false;

    /* '<S8>:279:6' startup2takeoff = false; */
    system_state_machine_DW.startup2takeoff = false;

    /* '<S8>:279:7' startup2manual = false; */
    system_state_machine_DW.startup2manual = false;

    /* '<S8>:279:8' startup2uman = false; */
    system_state_machine_DW.startup2uman = false;
    system_state_machine_DW.is_FMM = system_state_machine_IN_UMAN;

    /* Entry 'UMAN': '<S8>:1367' */
    /* '<S8>:1367:3' vom_status=vom_t.VOM_UMAN; */
    *rty_vom_status = VOM_UMAN;
  } else if (system_state_machine_DW.is_STARTUP == ((uint8_T)
              system_state_machine_IN_NOMINAL)) {
    /* During 'NOMINAL': '<S8>:2826' */
    /* '<S8>:2828:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_TAKEOFF; */
    guard1 = false;
    if (system_state_machine_B.Switch_d533txexjk == VOM_TAKEOFF) {
      /* Transition: '<S8>:2828' */
      /* '<S8>:2828:1' startup2takeoff=true; */
      system_state_machine_DW.startup2takeoff = true;
      guard1 = true;

      /* '<S8>:2829:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_MANUAL; */
    } else if (system_state_machine_B.Switch_d533txexjk == VOM_MANUAL) {
      /* Transition: '<S8>:2829' */
      /* '<S8>:2829:1' startup2manual=true; */
      system_state_machine_DW.startup2manual = true;
      guard1 = true;

      /* '<S8>:2827:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_UMAN; */
    } else if (system_state_machine_B.Switch_d533txexjk == VOM_UMAN) {
      /* Transition: '<S8>:2827' */
      /* '<S8>:2827:1' startup2uman=true; */
      system_state_machine_DW.startup2uman = true;
      guard1 = true;
    } else {
      /* '<S8>:2826:2' rampup_phase=false; */
      *rty_sFlags_rampup_phase = false;
    }

    if (guard1) {
      system_state_machine_DW.is_STARTUP = system_state_machine_IN_RAMPUP;

      /* Entry 'RAMPUP': '<S8>:2830' */
      /* '<S8>:2830:2' rampup_phase=true; */
      *rty_sFlags_rampup_phase = true;

      /* Constant: '<S1>/dt' */
      /* '<S8>:2830:3' rampup_timer=rampup_timer+dt; */
      system_state_machine_DW.rampup_timer += 0.01;
    }
  } else {
    /* During 'RAMPUP': '<S8>:2830' */
    /* '<S8>:2830:2' rampup_phase=true; */
    *rty_sFlags_rampup_phase = true;

    /* Constant: '<S1>/dt' */
    /* '<S8>:2830:3' rampup_timer=rampup_timer+dt; */
    system_state_machine_DW.rampup_timer += 0.01;
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_phh325i5jc(const real_T *Gain, const real_T
  rtu_sensor_eul_ang[3], const real_T rtu_sensor_pos_lla[3], const real_T
  *rtu_sensor_aspd_cas, const real_T *rtu_sensor_h_radar_agl, const real_T
  *rtu_sensor_chi, const int8_T *rtu_pilot_input_switch_1, const boolean_T
  *rtu_wp_data_wp_list_valid, vom_t *rty_vom_status, busMode_data *rty_mode_data,
  boolean_T *rty_sFlags_bGPSLoss, boolean_T *rty_sFlags_rampup_phase, boolean_T *
  rty_sFlags_ep_loss1sec, uint8_T *rty_loiter_mode, pic_t *rty_pic_status,
  real_T *rty_h_rel_takeoff)
{
  boolean_T f_out;
  boolean_T guard1;
  boolean_T guard2;

  /* During 'FMM': '<S8>:118' */
  guard1 = false;
  guard2 = false;
  switch (system_state_machine_DW.is_FMM) {
   case system_state_machine_IN_AUTOLAND:
    /* During 'AUTOLAND': '<S8>:281' */
    /* Transition: '<S8>:147' */
    /* '<S8>:151:1' sf_internal_predicateOutput = ((vom_cmd==vom_t.VOM_MANUAL) && ep_loss250ms==false) || (iRCoverride==1 && sContingencyFlags.ep_data_loss==0 ); */
    if (((system_state_machine_B.Switch_d533txexjk == VOM_MANUAL) &&
         (!system_state_machine_DW.ep_loss250ms)) ||
        (((*rtu_pilot_input_switch_1) == ((int8_T)1)) &&
         (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)))
    {
      /* Transition: '<S8>:151' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_MANUAL;

      /* Entry 'MANUAL': '<S8>:276' */
      /* '<S8>:276:3' vom_status=vom_t.VOM_MANUAL; */
      *rty_vom_status = VOM_MANUAL;

      /* '<S8>:138:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER&&(h_radar_AGL >=1)&&bHover5sec==false; */
    } else if (((system_state_machine_B.Switch_d533txexjk == VOM_HOVER) &&
                ((*rtu_sensor_h_radar_agl) >= 1.0)) &&
               (!system_state_machine_DW.bHover5sec)) {
      /* Transition: '<S8>:138' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_HOVER;

      /* Entry 'HOVER': '<S8>:280' */
      /* during */
      system_state_machine_odubvnrnpm(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
        rty_vom_status, rty_mode_data);

      /* Transition: '<S8>:160' */
      /* '<S8>:144:1' sf_internal_predicateOutput = (vom_cmd==vom_t.VOM_READY); */
    } else if (system_state_machine_B.Switch_d533txexjk == VOM_READY) {
      /* Transition: '<S8>:144' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

      /* Entry 'READY': '<S8>:277' */
      /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
      *rty_vom_status = VOM_READY;

      /* '<S8>:277:4' dHoverTimer_sec=0; */
      /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
      system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;
    } else {
      /* no actions */
    }
    break;

   case system_state_machine_IN_AUTOTAKEOFF:
    /* During 'AUTOTAKEOFF': '<S8>:272' */
    /* Transition: '<S8>:158' */
    /* Transition: '<S8>:149' */
    /* '<S8>:157:1' sf_internal_predicateOutput = (bInAirFlag==0 && vom_cmd==vom_t.VOM_READY) || ... */
    /* '<S8>:157:2' (bInAirFlag==0 && (sContingencyFlags.gps_loss == 1 || sContingencyFlags.ep_data_loss==1 || sContingencyFlags.ip_data_loss==1 || sContingencyFlags.gcs_link_loss==1)); */
    f_out = !system_state_machine_B.bInAirFlag;
    if ((f_out && (system_state_machine_B.Switch_d533txexjk == VOM_READY)) ||
        (f_out &&
         ((((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
            ||
            (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss))
           ||
           (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
          ||
          (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss))))
    {
      /* Transition: '<S8>:157' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

      /* Entry 'READY': '<S8>:277' */
      /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
      *rty_vom_status = VOM_READY;

      /* '<S8>:277:4' dHoverTimer_sec=0; */
      /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
      system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;

      /* '<S8>:159:1' sf_internal_predicateOutput = (sLLA(3)>= (dHomeLLA(3)+takeoff_alt)-1); */
    } else if (rtu_sensor_pos_lla[2] >= ((system_state_machine_B.sLLA[2] + 20.0)
                - 1.0)) {
      /* Transition: '<S8>:159' */
      /* '<S8>:159:1' bTakeoff2Hover=true; */
      system_state_machine_DW.bTakeoff2Hover = true;
      guard1 = true;

      /* Transition: '<S8>:274' */
      /* '<S8>:139:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_MANUAL || (iRCoverride==1 && sContingencyFlags.ep_data_loss==0 ); */
    } else if ((system_state_machine_B.Switch_d533txexjk == VOM_MANUAL) ||
               (((*rtu_pilot_input_switch_1) == ((int8_T)1)) &&
                (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)))
    {
      /* Transition: '<S8>:139' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_MANUAL;

      /* Entry 'MANUAL': '<S8>:276' */
      /* '<S8>:276:3' vom_status=vom_t.VOM_MANUAL; */
      *rty_vom_status = VOM_MANUAL;
    } else {
      /* '<S8>:135:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_HOVER && (dCapturedAlt_m>=1); */
      f_out = ((system_state_machine_B.Switch_d533txexjk == VOM_HOVER) &&
               ((*rty_h_rel_takeoff) >= 1.0));
      if (f_out) {
        /* Transition: '<S8>:135' */
        guard1 = true;
      } else {
        /* '<S8>:2838:1' sf_internal_predicateOutput = bGPSLoss==true && h_radar_AGL>4.0; */
        f_out = ((*rty_sFlags_bGPSLoss) && ((*rtu_sensor_h_radar_agl) > 4.0));
        if (f_out) {
          /* Transition: '<S8>:2838' */
          /* Transition: '<S8>:2839' */
          system_state_machine_DW.is_FMM = system_state_machine_IN_AUTOLAND;
          system_state_machine_jgwda1ajrs(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
            rty_vom_status, rty_mode_data);
        }
      }
    }
    break;

   case system_state_machine_IN_FW_Modes:
    system_state_machine_gjywfyiloe(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rtu_sensor_aspd_cas, rtu_sensor_chi, rtu_wp_data_wp_list_valid,
      rty_vom_status, rty_mode_data);
    break;

   case system_state_machine_IN_HOVER:
    system_state_machine_aeuawg3vfo(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rtu_pilot_input_switch_1, rtu_wp_data_wp_list_valid, rty_vom_status,
      rty_mode_data, rty_sFlags_bGPSLoss);
    break;

   case system_state_machine_IN_MANUAL:
    system_state_machine_csj5zj0zmt(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rtu_sensor_h_radar_agl, rtu_pilot_input_switch_1, rty_vom_status,
      rty_mode_data, rty_pic_status);
    break;

   case system_state_machine_IN_READY:
    /* During 'READY': '<S8>:277' */
    /* '<S8>:136:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_STARTUP&& ... */
    /* '<S8>:136:2' (sContingencyFlags.gps_loss == false &&  sContingencyFlags.ep_data_loss==false && sContingencyFlags.ip_data_loss==false && sContingencyFlags.gcs_link_loss==false); */
    f_out =
      !system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss;
    if (((((system_state_machine_B.Switch_d533txexjk == VOM_STARTUP) &&
           (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss))
          && f_out) &&
         (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
        &&
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss))
    {
      /* Transition: '<S8>:136' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_STARTUP;

      /* Entry 'STARTUP': '<S8>:279' */
      /* '<S8>:279:3' vom_status=vom_t.VOM_STARTUP; */
      *rty_vom_status = VOM_STARTUP;

      /* '<S8>:279:4' rampup_timer=0; */
      system_state_machine_DW.rampup_timer = 0.0;

      /* Entry Internal 'STARTUP': '<S8>:279' */
      /* Transition: '<S8>:2831' */
      system_state_machine_DW.is_STARTUP = system_state_machine_IN_NOMINAL;

      /* Entry 'NOMINAL': '<S8>:2826' */
      /* '<S8>:2826:2' rampup_phase=false; */
      *rty_sFlags_rampup_phase = false;

      /* '<S8>:2864:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_ZEROG && throttle_ch_normalized< 0.1 &&... */
      /* '<S8>:2864:2' sContingencyFlags.ep_data_loss==0; */
    } else if (((system_state_machine_B.Switch_d533txexjk == VOM_ZEROG) &&
                ((*Gain) < 0.1)) && f_out) {
      /* Transition: '<S8>:2864' */
      /*  PC-1241 */
      system_state_machine_DW.is_FMM = system_state_machine_IN_ZEROG;

      /* Entry 'ZEROG': '<S8>:2861' */
      /* '<S8>:2861:3' vom_status=vom_t.VOM_ZEROG; */
      *rty_vom_status = VOM_ZEROG;
    } else {
      /* no actions */
    }
    break;

   case system_state_machine_IN_RTH:
    system_state_machine_fks3ydn24b(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rtu_pilot_input_switch_1, rty_vom_status, rty_mode_data,
      rty_sFlags_bGPSLoss);
    break;

   case system_state_machine_IN_STARTUP:
    system_state_machine_afs03nhl30(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rtu_pilot_input_switch_1, rty_vom_status, rty_mode_data,
      rty_sFlags_rampup_phase);
    break;

   case system_state_machine_IN_UMAN:
    /* During 'UMAN': '<S8>:1367' */
    /* '<S8>:1371:1' sf_internal_predicateOutput = vom_cmd==vom_t.VOM_MR_RTH; */
    if (system_state_machine_B.Switch_d533txexjk == VOM_MR_RTH) {
      /* Transition: '<S8>:1371' */
      /*  PC-1220 */
      guard2 = true;

      /* '<S8>:1344:1' sf_internal_predicateOutput = (vom_cmd==vom_t.VOM_READY); */
    } else if (system_state_machine_B.Switch_d533txexjk == VOM_READY) {
      /* Transition: '<S8>:1344' */
      /*  PC-1223 */
      /* Transition: '<S8>:1345' */
      system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

      /* Entry 'READY': '<S8>:277' */
      /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
      *rty_vom_status = VOM_READY;

      /* '<S8>:277:4' dHoverTimer_sec=0; */
      /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
      system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;
    } else {
      /* '<S8>:1373:1' sf_internal_predicateOutput = loiter_mode==1&&ep_loss5sec==true&&ip_loss5sec==true; */
      f_out = ((((*rty_loiter_mode) == ((uint8_T)1)) &&
                (system_state_machine_DW.ep_loss5sec)) &&
               (system_state_machine_DW.ip_loss5sec));
      if (f_out) {
        /* Transition: '<S8>:1373' */
        /*  PC-1235 */
        guard2 = true;
      } else {
        /* '<S8>:1377:1' sf_internal_predicateOutput = loiter_mode==0&&ip_loss1sec==true&&ep_loss1sec==true; */
        f_out = ((((*rty_loiter_mode) == ((uint8_T)0)) &&
                  (system_state_machine_DW.ip_loss1sec)) &&
                 (*rty_sFlags_ep_loss1sec));
        if (f_out) {
          /* Transition: '<S8>:1377' */
          /*  PC-1236 */
          guard2 = true;
        }
      }
    }
    break;

   default:
    /* During 'ZEROG': '<S8>:2861' */
    /* '<S8>:2865:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==1 || ... */
    /* '<S8>:2865:2' vom_cmd==vom_t.VOM_READY; */
    if ((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
        || (system_state_machine_B.Switch_d533txexjk == VOM_READY)) {
      /* Transition: '<S8>:2865' */
      /*  PC-1240 */
      system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

      /* Entry 'READY': '<S8>:277' */
      /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
      *rty_vom_status = VOM_READY;

      /* '<S8>:277:4' dHoverTimer_sec=0; */
      /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
      system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;
    }
    break;
  }

  if (guard2) {
    /* Transition: '<S8>:1381' */
    system_state_machine_DW.is_FMM = system_state_machine_IN_RTH;

    /* Entry 'RTH': '<S8>:1035' */
    /* '<S8>:1035:3' vom_status=vom_t.VOM_MR_RTH; */
    *rty_vom_status = VOM_MR_RTH;

    /* '<S8>:1035:4' bRTH2LandingFlag=0; */
    system_state_machine_DW.bRTH2LandingFlag = 0.0;

    /* '<S8>:1035:5' sMode_data.rth_data.bRTH_PosModeFlag=false; */
    rty_mode_data->rth_data.bRTH_PosModeFlag = false;

    /* '<S8>:1035:6' sMode_data.rth_data.bBreaking=false; */
    rty_mode_data->rth_data.bBreaking = false;

    /* Entry Internal 'RTH': '<S8>:1035' */
    /* Transition: '<S8>:1038' */
    system_state_machine_DW.is_RTH = system_state_machine_IN_RTH_Speed_Check;

    /* Entry 'RTH_Speed_Check': '<S8>:1037' */
    /* '<S8>:1037:3' sMode_data.rth_data.bBreaking=true; */
    rty_mode_data->rth_data.bBreaking = true;

    /* '<S8>:1037:4' sMode_data.rth_data.rth_velX=0; */
    rty_mode_data->rth_data.rth_velX = 0.0;

    /* '<S8>:1037:5' sMode_data.rth_data.rth_velY=0; */
    rty_mode_data->rth_data.rth_velY = 0.0;

    /* '<S8>:1037:6' sMode_data.rth_data.rth_alt_agl=sLLA(3); */
    rty_mode_data->rth_data.rth_alt_agl = rtu_sensor_pos_lla[2];

    /* '<S8>:1037:7' sMode_data.rth_data.rth_yaw_ref=dYaw_rad; */
    rty_mode_data->rth_data.rth_yaw_ref = rtu_sensor_eul_ang[2];

    /* '<S8>:1037:8' sMode_data.rth_data.rth_x= sLLA(1); */
    rty_mode_data->rth_data.rth_x = rtu_sensor_pos_lla[0];

    /* '<S8>:1037:9' sMode_data.rth_data.rth_y= sLLA(2); */
    rty_mode_data->rth_data.rth_y = rtu_sensor_pos_lla[1];
  }

  if (guard1) {
    system_state_machine_DW.is_FMM = system_state_machine_IN_HOVER;

    /* Entry 'HOVER': '<S8>:280' */
    /* during */
    system_state_machine_odubvnrnpm(rtu_sensor_eul_ang, rtu_sensor_pos_lla,
      rty_vom_status, rty_mode_data);
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_hkbbt45tg5(const vom_t *vom_status, const
  real_T *rtu_sensor_h_radar_agl, const real_T *rtu_controllerAltCtrl_forceDes,
  boolean_T *rty_bInAirFlag, real_T *rty_h_rel_takeoff)
{
  /* During 'InAirOnGroundLogic': '<S8>:202' */
  switch (system_state_machine_DW.is_InAirOnGroundLogic) {
   case system_state_machine_IN_InAir:
    *rty_bInAirFlag = true;

    /* Constant: '<S1>/mass' */
    /* During 'InAir': '<S8>:251' */
    /* '<S8>:252:1' sf_internal_predicateOutput = (h_radar_AGL<0.6 ) && (f_mode_unitdelay==vom_t.VOM_LAND || f_mode_unitdelay==vom_t.VOM_MANUAL || f_mode_unitdelay == vom_t.VOM_UMAN) && forceDes<= (0.2*mass*9.81); */
    if ((((*rtu_sensor_h_radar_agl) < 0.6) && ((((*vom_status) == VOM_LAND) ||
           ((*vom_status) == VOM_MANUAL)) || ((*vom_status) == VOM_UMAN))) && ((*
          rtu_controllerAltCtrl_forceDes) <= 132.3369)) {
      /* Transition: '<S8>:252' */
      system_state_machine_DW.temporalCounter_i3 = 0U;
      system_state_machine_DW.is_InAirOnGroundLogic =
        system_state_machine_IN_altitude_check_2;
    }
    break;

   case system_state_machine_IN_OnGround:
    *rty_bInAirFlag = false;

    /* Constant: '<S1>/mass' */
    /* During 'OnGround': '<S8>:245' */
    /* '<S8>:1103:1' sf_internal_predicateOutput = dCapturedAlt_m>=0.6 && forceDes>= (0.8*mass*9.81) && (f_mode_unitdelay==vom_t.VOM_TAKEOFF || f_mode_unitdelay==vom_t.VOM_MANUAL || f_mode_unitdelay == vom_t.VOM_UMAN); */
    if ((((*rty_h_rel_takeoff) >= 0.6) && ((*rtu_controllerAltCtrl_forceDes) >=
          529.3476)) && ((((*vom_status) == VOM_TAKEOFF) || ((*vom_status) ==
           VOM_MANUAL)) || ((*vom_status) == VOM_UMAN))) {
      /* Transition: '<S8>:1103' */
      system_state_machine_DW.temporalCounter_i3 = 0U;
      system_state_machine_DW.is_InAirOnGroundLogic =
        system_state_machine_IN_altitude_check_1;
    }
    break;

   case system_state_machine_IN_altitude_check_1:
    /* During 'altitude_check_1': '<S8>:1234' */
    /* '<S8>:248:1' sf_internal_predicateOutput = (after (0.5, sec)); */
    if (system_state_machine_DW.temporalCounter_i3 >= ((uint8_T)50)) {
      /* Transition: '<S8>:248' */
      system_state_machine_DW.is_InAirOnGroundLogic =
        system_state_machine_IN_InAir;

      /* Entry 'InAir': '<S8>:251' */
      /* '<S8>:251:3' InAir_mode=true; */
      *rty_bInAirFlag = true;

      /* Constant: '<S1>/mass' */
      /* '<S8>:1235:1' sf_internal_predicateOutput = dCapturedAlt_m<0.6 || forceDes< (0.8*mass*9.81)|| ~(f_mode_unitdelay == vom_t.VOM_TAKEOFF || f_mode_unitdelay == vom_t.VOM_MANUAL || f_mode_unitdelay == vom_t.VOM_UMAN); */
    } else if ((((*rty_h_rel_takeoff) < 0.6) ||
                ((*rtu_controllerAltCtrl_forceDes) < 529.3476)) ||
               ((((*vom_status) != VOM_TAKEOFF) && ((*vom_status) != VOM_MANUAL))
                && ((*vom_status) != VOM_UMAN))) {
      /* Transition: '<S8>:1235' */
      system_state_machine_DW.is_InAirOnGroundLogic =
        system_state_machine_IN_OnGround;

      /* Entry 'OnGround': '<S8>:245' */
      /* '<S8>:245:2' InAir_mode=false; */
      *rty_bInAirFlag = false;
    } else {
      /* no actions */
    }
    break;

   default:
    /* During 'altitude_check_2': '<S8>:1236' */
    /* '<S8>:1106:1' sf_internal_predicateOutput = (after (1, sec)); */
    if (system_state_machine_DW.temporalCounter_i3 >= ((uint8_T)100)) {
      /* Transition: '<S8>:1106' */
      system_state_machine_DW.is_InAirOnGroundLogic =
        system_state_machine_IN_OnGround;

      /* Entry 'OnGround': '<S8>:245' */
      /* '<S8>:245:2' InAir_mode=false; */
      *rty_bInAirFlag = false;

      /* Constant: '<S1>/mass' */
      /* '<S8>:1237:1' sf_internal_predicateOutput = h_radar_AGL>0.6 || forceDes > (0.2*mass*9.81) || ~(f_mode_unitdelay==vom_t.VOM_LAND || f_mode_unitdelay==vom_t.VOM_MANUAL || f_mode_unitdelay == vom_t.VOM_UMAN); */
    } else if ((((*rtu_sensor_h_radar_agl) > 0.6) ||
                ((*rtu_controllerAltCtrl_forceDes) > 132.3369)) ||
               ((((*vom_status) != VOM_LAND) && ((*vom_status) != VOM_MANUAL)) &&
                ((*vom_status) != VOM_UMAN))) {
      /* Transition: '<S8>:1237' */
      system_state_machine_DW.is_InAirOnGroundLogic =
        system_state_machine_IN_InAir;

      /* Entry 'InAir': '<S8>:251' */
      /* '<S8>:251:3' InAir_mode=true; */
      *rty_bInAirFlag = true;
    } else {
      /* no actions */
    }
    break;
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_lnm1jlgmlc(boolean_T
  *rty_sFlags_bTelemtryLinkLoss1sec)
{
  /* During 'Telemtry_LinkLoss': '<S8>:408' */
  switch (system_state_machine_DW.is_Telemtry_LinkLoss) {
   case system_state_machine_IN_Tele_loss_false:
    *rty_sFlags_bTelemtryLinkLoss1sec = false;

    /* During 'Tele_loss_false': '<S8>:758' */
    /* '<S8>:759:1' sf_internal_predicateOutput = sContingencyFlags. gcs_link_loss==true; */
    if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss)
    {
      /* Transition: '<S8>:759' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_timer1_start;

      /* Entry 'Tele_loss_timer1_start': '<S8>:736' */
      /* '<S8>:736:3' bTelemtryLinkLoss250ms=false; */
      /* '<S8>:736:4' bTelemtryLinkLoss5sec=false; */
      system_state_machine_DW.bTelemtryLinkLoss5sec = false;

      /* '<S8>:736:5' timer_Tele=0 */
      system_state_machine_DW.timer_Tele = 0.0;
    }
    break;

   case system_state_machine_IN_Tele_loss_timer1_start:
    /* During 'Tele_loss_timer1_start': '<S8>:736' */
    /* '<S8>:739:1' sf_internal_predicateOutput = timer_Tele>=0.25; */
    if (system_state_machine_DW.timer_Tele >= 0.25) {
      /* Transition: '<S8>:739' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_timer2_start;

      /* Entry 'Tele_loss_timer2_start': '<S8>:741' */
      /* '<S8>:741:3' bTelemtryLinkLoss250ms=true; */

      /* Transition: '<S8>:2064' */
      /* Transition: '<S8>:2067' */
      /* '<S8>:2068:1' sf_internal_predicateOutput = sContingencyFlags.gcs_link_loss==false; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss)
    {
      /* Transition: '<S8>:2068' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_false;

      /* Entry 'Tele_loss_false': '<S8>:758' */
      /* '<S8>:758:3' bTelemtryLinkLoss250ms=false; */
      /* '<S8>:758:4' bTelemtryLinkLoss5sec=false; */
      system_state_machine_DW.bTelemtryLinkLoss5sec = false;

      /* '<S8>:758:5' bTelemtryLinkLoss1sec=false; */
      *rty_sFlags_bTelemtryLinkLoss1sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:736:7' timer_Tele=timer_Tele+ dt; */
      system_state_machine_DW.timer_Tele += 0.01;
    }
    break;

   case system_state_machine_IN_Tele_loss_timer2_start:
    /* During 'Tele_loss_timer2_start': '<S8>:741' */
    /* '<S8>:1375:1' sf_internal_predicateOutput = timer_Tele>=1; */
    if (system_state_machine_DW.timer_Tele >= 1.0) {
      /* Transition: '<S8>:1375' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_timer3_trigger;

      /* Entry 'Tele_loss_timer3_trigger': '<S8>:1374' */
      /* '<S8>:1374:3' bTelemtryLinkLoss1sec=true; */
      *rty_sFlags_bTelemtryLinkLoss1sec = true;

      /* Transition: '<S8>:2061' */
      /* Transition: '<S8>:2065' */
      /* Transition: '<S8>:2067' */
      /* '<S8>:2068:1' sf_internal_predicateOutput = sContingencyFlags.gcs_link_loss==false; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss)
    {
      /* Transition: '<S8>:2068' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_false;

      /* Entry 'Tele_loss_false': '<S8>:758' */
      /* '<S8>:758:3' bTelemtryLinkLoss250ms=false; */
      /* '<S8>:758:4' bTelemtryLinkLoss5sec=false; */
      system_state_machine_DW.bTelemtryLinkLoss5sec = false;

      /* '<S8>:758:5' bTelemtryLinkLoss1sec=false; */
      *rty_sFlags_bTelemtryLinkLoss1sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:741:5' timer_Tele=timer_Tele+ dt; */
      system_state_machine_DW.timer_Tele += 0.01;
    }
    break;

   case system_state_machine_IN_Tele_loss_timer2_trigger:
    /* During 'Tele_loss_timer2_trigger': '<S8>:737' */
    /* Transition: '<S8>:2056' */
    /* Transition: '<S8>:2059' */
    /* Transition: '<S8>:2062' */
    /* Transition: '<S8>:2065' */
    /* Transition: '<S8>:2067' */
    /* '<S8>:2068:1' sf_internal_predicateOutput = sContingencyFlags.gcs_link_loss==false; */
    if (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss)
    {
      /* Transition: '<S8>:2068' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_false;

      /* Entry 'Tele_loss_false': '<S8>:758' */
      /* '<S8>:758:3' bTelemtryLinkLoss250ms=false; */
      /* '<S8>:758:4' bTelemtryLinkLoss5sec=false; */
      system_state_machine_DW.bTelemtryLinkLoss5sec = false;

      /* '<S8>:758:5' bTelemtryLinkLoss1sec=false; */
      *rty_sFlags_bTelemtryLinkLoss1sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:737:5' timer_Tele=timer_Tele+ dt; */
      system_state_machine_DW.timer_Tele += 0.01;
    }
    break;

   default:
    *rty_sFlags_bTelemtryLinkLoss1sec = true;

    /* During 'Tele_loss_timer3_trigger': '<S8>:1374' */
    /* '<S8>:740:1' sf_internal_predicateOutput = timer_Tele>=5; */
    if (system_state_machine_DW.timer_Tele >= 5.0) {
      /* Transition: '<S8>:740' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_timer2_trigger;

      /* Entry 'Tele_loss_timer2_trigger': '<S8>:737' */
      /* '<S8>:737:3' bTelemtryLinkLoss5sec=true; */
      system_state_machine_DW.bTelemtryLinkLoss5sec = true;

      /* Transition: '<S8>:2052' */
      /* Transition: '<S8>:2062' */
      /* Transition: '<S8>:2065' */
      /* Transition: '<S8>:2067' */
      /* '<S8>:2068:1' sf_internal_predicateOutput = sContingencyFlags.gcs_link_loss==false; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss)
    {
      /* Transition: '<S8>:2068' */
      system_state_machine_DW.is_Telemtry_LinkLoss =
        system_state_machine_IN_Tele_loss_false;

      /* Entry 'Tele_loss_false': '<S8>:758' */
      /* '<S8>:758:3' bTelemtryLinkLoss250ms=false; */
      /* '<S8>:758:4' bTelemtryLinkLoss5sec=false; */
      system_state_machine_DW.bTelemtryLinkLoss5sec = false;

      /* '<S8>:758:5' bTelemtryLinkLoss1sec=false; */
      *rty_sFlags_bTelemtryLinkLoss1sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:1374:5' timer_Tele=timer_Tele+ dt; */
      system_state_machine_DW.timer_Tele += 0.01;
    }
    break;
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_dfezhqrijv(boolean_T *rty_sFlags_bGPSLoss10sec,
  boolean_T *rty_sFlags_bGPSLoss)
{
  /* During 'GPS_Loss': '<S8>:820' */
  switch (system_state_machine_DW.is_GPS_Loss) {
   case system_state_machine_IN_GPS_loss_false:
    *rty_sFlags_bGPSLoss10sec = false;

    /* During 'GPS_loss_false': '<S8>:827' */
    /* '<S8>:834:1' sf_internal_predicateOutput = sContingencyFlags.gps_loss==true; */
    if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
    {
      /* Transition: '<S8>:834' */
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_timer1_start;

      /* Entry 'GPS_loss_timer1_start': '<S8>:833' */
      /* '<S8>:833:3' bGPSLoss10sec=false; */
      *rty_sFlags_bGPSLoss10sec = false;

      /* '<S8>:833:4' bGPSLoss=true; */
      *rty_sFlags_bGPSLoss = true;
    } else {
      /* '<S8>:827:9' bRecoverGPS=0; */
      system_state_machine_DW.bRecoverGPS = 0.0;
    }
    break;

   case system_state_machine_IN_GPS_loss_timer1_start:
    *rty_sFlags_bGPSLoss10sec = false;

    /* During 'GPS_loss_timer1_start': '<S8>:833' */
    /* '<S8>:1401:1' sf_internal_predicateOutput = timer_GPSLoss>=2; */
    if (system_state_machine_DW.timer_GPSLoss >= 2.0) {
      /* Transition: '<S8>:1401' */
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_timer2_start;

      /* Entry 'GPS_loss_timer2_start': '<S8>:1400' */
      /* '<S8>:1400:3' bGPSLoss2sec=true; */
      system_state_machine_DW.bGPSLoss2sec = true;

      /* Transition: '<S8>:2088' */
      /* Transition: '<S8>:2087' */
      /* '<S8>:2085:1' sf_internal_predicateOutput = sContingencyFlags.gps_loss==false; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
    {
      /* Transition: '<S8>:2085' */
      /* '<S8>:2085:1' bRecoverGPS=1; */
      system_state_machine_DW.bRecoverGPS = 1.0;
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_false;

      /* Entry 'GPS_loss_false': '<S8>:827' */
      /* '<S8>:827:3' bGPSLoss10sec=false; */
      *rty_sFlags_bGPSLoss10sec = false;

      /* '<S8>:827:4' bGPSLoss5sec=false */
      /* '<S8>:827:5' bGPSLoss2sec=false; */
      system_state_machine_DW.bGPSLoss2sec = false;

      /* '<S8>:827:6' bGPSLoss=false; */
      *rty_sFlags_bGPSLoss = false;

      /* '<S8>:827:7' timer_GPSLoss=0; */
      system_state_machine_DW.timer_GPSLoss = 0.0;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:833:6' timer_GPSLoss=timer_GPSLoss+ dt; */
      system_state_machine_DW.timer_GPSLoss += 0.01;
    }
    break;

   case system_state_machine_IN_GPS_loss_timer2_start:
    /* During 'GPS_loss_timer2_start': '<S8>:1400' */
    /* Transition: '<S8>:2091' */
    /* Transition: '<S8>:2090' */
    /* Transition: '<S8>:2087' */
    /* '<S8>:2085:1' sf_internal_predicateOutput = sContingencyFlags.gps_loss==false; */
    if (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
    {
      /* Transition: '<S8>:2085' */
      /* '<S8>:2085:1' bRecoverGPS=1; */
      system_state_machine_DW.bRecoverGPS = 1.0;
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_false;

      /* Entry 'GPS_loss_false': '<S8>:827' */
      /* '<S8>:827:3' bGPSLoss10sec=false; */
      *rty_sFlags_bGPSLoss10sec = false;

      /* '<S8>:827:4' bGPSLoss5sec=false */
      /* '<S8>:827:5' bGPSLoss2sec=false; */
      system_state_machine_DW.bGPSLoss2sec = false;

      /* '<S8>:827:6' bGPSLoss=false; */
      *rty_sFlags_bGPSLoss = false;

      /* '<S8>:827:7' timer_GPSLoss=0; */
      system_state_machine_DW.timer_GPSLoss = 0.0;

      /* '<S8>:2842:1' sf_internal_predicateOutput = timer_GPSLoss>=5; */
    } else if (system_state_machine_DW.timer_GPSLoss >= 5.0) {
      /* Transition: '<S8>:2842' */
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_timer3_stop;

      /* Entry 'GPS_loss_timer3_stop': '<S8>:2841' */
      /* '<S8>:2841:3' bGPSLoss5sec=true; */
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:1400:5' timer_GPSLoss=timer_GPSLoss+ dt; */
      system_state_machine_DW.timer_GPSLoss += 0.01;
    }
    break;

   case system_state_machine_IN_GPS_loss_timer3_stop:
    /* During 'GPS_loss_timer3_stop': '<S8>:2841' */
    /* '<S8>:842:1' sf_internal_predicateOutput = timer_GPSLoss>=10; */
    if (system_state_machine_DW.timer_GPSLoss >= 10.0) {
      /* Transition: '<S8>:842' */
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_timer_triggered;

      /* Entry 'GPS_loss_timer_triggered': '<S8>:841' */
      /* '<S8>:841:3' bGPSLoss10sec=true; */
      *rty_sFlags_bGPSLoss10sec = true;

      /* '<S8>:841:4' bGPSLoss=true; */
      *rty_sFlags_bGPSLoss = true;

      /* Transition: '<S8>:2843' */
      /* Transition: '<S8>:2844' */
      /* Transition: '<S8>:2090' */
      /* Transition: '<S8>:2087' */
      /* '<S8>:2085:1' sf_internal_predicateOutput = sContingencyFlags.gps_loss==false; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
    {
      /* Transition: '<S8>:2085' */
      /* '<S8>:2085:1' bRecoverGPS=1; */
      system_state_machine_DW.bRecoverGPS = 1.0;
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_false;

      /* Entry 'GPS_loss_false': '<S8>:827' */
      /* '<S8>:827:3' bGPSLoss10sec=false; */
      *rty_sFlags_bGPSLoss10sec = false;

      /* '<S8>:827:4' bGPSLoss5sec=false */
      /* '<S8>:827:5' bGPSLoss2sec=false; */
      system_state_machine_DW.bGPSLoss2sec = false;

      /* '<S8>:827:6' bGPSLoss=false; */
      *rty_sFlags_bGPSLoss = false;

      /* '<S8>:827:7' timer_GPSLoss=0; */
      system_state_machine_DW.timer_GPSLoss = 0.0;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:2841:5' timer_GPSLoss=timer_GPSLoss+ dt; */
      system_state_machine_DW.timer_GPSLoss += 0.01;
    }
    break;

   default:
    *rty_sFlags_bGPSLoss10sec = true;

    /* During 'GPS_loss_timer_triggered': '<S8>:841' */
    /* Transition: '<S8>:2093' */
    /* Transition: '<S8>:2094' */
    /* Transition: '<S8>:2844' */
    /* Transition: '<S8>:2090' */
    /* Transition: '<S8>:2087' */
    /* '<S8>:2085:1' sf_internal_predicateOutput = sContingencyFlags.gps_loss==false; */
    if (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
    {
      /* Transition: '<S8>:2085' */
      /* '<S8>:2085:1' bRecoverGPS=1; */
      system_state_machine_DW.bRecoverGPS = 1.0;
      system_state_machine_DW.is_GPS_Loss =
        system_state_machine_IN_GPS_loss_false;

      /* Entry 'GPS_loss_false': '<S8>:827' */
      /* '<S8>:827:3' bGPSLoss10sec=false; */
      *rty_sFlags_bGPSLoss10sec = false;

      /* '<S8>:827:4' bGPSLoss5sec=false */
      /* '<S8>:827:5' bGPSLoss2sec=false; */
      system_state_machine_DW.bGPSLoss2sec = false;

      /* '<S8>:827:6' bGPSLoss=false; */
      *rty_sFlags_bGPSLoss = false;

      /* '<S8>:827:7' timer_GPSLoss=0; */
      system_state_machine_DW.timer_GPSLoss = 0.0;
    }
    break;
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_g0rztozkme(const vom_t *vom_status, boolean_T
  *rty_sFlags_bGPSLoss10sec, boolean_T *rty_sFlags_bGPSLoss, boolean_T
  *rty_sFlags_bTelemtryLinkLoss1sec, boolean_T *rty_sFlags_ep_loss1sec)
{
  /* During 'ContingencyStatus': '<S8>:384' */
  /* During 'ContingencyStatusManagment': '<S8>:407' */
  system_state_machine_lnm1jlgmlc(rty_sFlags_bTelemtryLinkLoss1sec);

  /* During 'ep_loss': '<S8>:734' */
  switch (system_state_machine_DW.is_ep_loss) {
   case system_state_machine_IN_RC_loss_false:
    *rty_sFlags_ep_loss1sec = false;

    /* During 'RC_loss_false': '<S8>:770' */
    /* '<S8>:762:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==1; */
    if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
    {
      /* Transition: '<S8>:762' */
      system_state_machine_DW.is_ep_loss =
        system_state_machine_IN_RC_loss_timer1_start;

      /* Entry 'RC_loss_timer1_start': '<S8>:765' */
      /* '<S8>:765:3' timer_RC=0; */
      system_state_machine_DW.timer_RC = 0.0;
    }
    break;

   case system_state_machine_IN_RC_loss_timer1_start:
    /* During 'RC_loss_timer1_start': '<S8>:765' */
    /* '<S8>:768:1' sf_internal_predicateOutput = timer_RC>=0.25; */
    if (system_state_machine_DW.timer_RC >= 0.25) {
      /* Transition: '<S8>:768' */
      system_state_machine_DW.is_ep_loss =
        system_state_machine_IN_RC_loss_timer2_start;

      /* Entry 'RC_loss_timer2_start': '<S8>:766' */
      /* '<S8>:766:3' ep_loss250ms=true; */
      system_state_machine_DW.ep_loss250ms = true;

      /* Transition: '<S8>:2080' */
      /* Transition: '<S8>:2083' */
      /* '<S8>:2082:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==0; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
    {
      /* Transition: '<S8>:2082' */
      system_state_machine_DW.is_ep_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:770' */
      /* '<S8>:770:3' ep_loss250ms=false; */
      system_state_machine_DW.ep_loss250ms = false;

      /* '<S8>:770:4' ep_loss1sec=false; */
      *rty_sFlags_ep_loss1sec = false;

      /* '<S8>:770:5' ep_loss5sec=false; */
      system_state_machine_DW.ep_loss5sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:765:5' timer_RC=timer_RC+ dt; */
      system_state_machine_DW.timer_RC += 0.01;
    }
    break;

   case system_state_machine_IN_RC_loss_timer2_start:
    /* During 'RC_loss_timer2_start': '<S8>:766' */
    /* '<S8>:1379:1' sf_internal_predicateOutput = timer_RC>=1; */
    if (system_state_machine_DW.timer_RC >= 1.0) {
      /* Transition: '<S8>:1379' */
      system_state_machine_DW.is_ep_loss =
        system_state_machine_IN_RC_loss_timer3_start;

      /* Entry 'RC_loss_timer3_start': '<S8>:1378' */
      /* '<S8>:1378:3' ep_loss1sec=true; */
      *rty_sFlags_ep_loss1sec = true;

      /* Transition: '<S8>:2071' */
      /* Transition: '<S8>:2079' */
      /* Transition: '<S8>:2083' */
      /* '<S8>:2082:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==0; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
    {
      /* Transition: '<S8>:2082' */
      system_state_machine_DW.is_ep_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:770' */
      /* '<S8>:770:3' ep_loss250ms=false; */
      system_state_machine_DW.ep_loss250ms = false;

      /* '<S8>:770:4' ep_loss1sec=false; */
      *rty_sFlags_ep_loss1sec = false;

      /* '<S8>:770:5' ep_loss5sec=false; */
      system_state_machine_DW.ep_loss5sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:766:5' timer_RC=timer_RC+ dt; */
      system_state_machine_DW.timer_RC += 0.01;
    }
    break;

   case system_state_machine_IN_RC_loss_timer2_trigger:
    /* During 'RC_loss_timer2_trigger': '<S8>:767' */
    /* Transition: '<S8>:2074' */
    /* Transition: '<S8>:2075' */
    /* Transition: '<S8>:2077' */
    /* Transition: '<S8>:2079' */
    /* Transition: '<S8>:2083' */
    /* '<S8>:2082:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==0; */
    if (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
    {
      /* Transition: '<S8>:2082' */
      system_state_machine_DW.is_ep_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:770' */
      /* '<S8>:770:3' ep_loss250ms=false; */
      system_state_machine_DW.ep_loss250ms = false;

      /* '<S8>:770:4' ep_loss1sec=false; */
      *rty_sFlags_ep_loss1sec = false;

      /* '<S8>:770:5' ep_loss5sec=false; */
      system_state_machine_DW.ep_loss5sec = false;
    }
    break;

   default:
    *rty_sFlags_ep_loss1sec = true;

    /* During 'RC_loss_timer3_start': '<S8>:1378' */
    /* '<S8>:769:1' sf_internal_predicateOutput = timer_RC>=5.0; */
    if (system_state_machine_DW.timer_RC >= 5.0) {
      /* Transition: '<S8>:769' */
      system_state_machine_DW.is_ep_loss =
        system_state_machine_IN_RC_loss_timer2_trigger;

      /* Entry 'RC_loss_timer2_trigger': '<S8>:767' */
      /* '<S8>:767:3' ep_loss5sec=true; */
      system_state_machine_DW.ep_loss5sec = true;

      /* Transition: '<S8>:2076' */
      /* Transition: '<S8>:2077' */
      /* Transition: '<S8>:2079' */
      /* Transition: '<S8>:2083' */
      /* '<S8>:2082:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==0; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
    {
      /* Transition: '<S8>:2082' */
      system_state_machine_DW.is_ep_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:770' */
      /* '<S8>:770:3' ep_loss250ms=false; */
      system_state_machine_DW.ep_loss250ms = false;

      /* '<S8>:770:4' ep_loss1sec=false; */
      *rty_sFlags_ep_loss1sec = false;

      /* '<S8>:770:5' ep_loss5sec=false; */
      system_state_machine_DW.ep_loss5sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:1378:5' timer_RC=timer_RC+ dt; */
      system_state_machine_DW.timer_RC += 0.01;
    }
    break;
  }

  /* During 'HoverTimer': '<S8>:595' */
  switch (system_state_machine_DW.is_HoverTimer) {
   case system_state_machine_IN_HoverStartTimer:
    /* During 'HoverStartTimer': '<S8>:659' */
    /* '<S8>:662:1' sf_internal_predicateOutput = TimerHover>=5; */
    if (system_state_machine_DW.TimerHover >= 5.0) {
      /* Transition: '<S8>:662' */
      system_state_machine_DW.is_HoverTimer =
        system_state_machine_IN_HoverStopTimer;

      /* Entry 'HoverStopTimer': '<S8>:661' */
      /* '<S8>:661:2' bHover5sec=true; */
      system_state_machine_DW.bHover5sec = true;

      /* '<S8>:798:1' sf_internal_predicateOutput = bTelemtryLinkLoss5sec==false && ( ep_loss5sec==false || ip_loss5sec==false ); */
    } else if ((!system_state_machine_DW.bTelemtryLinkLoss5sec) &&
               ((!system_state_machine_DW.ep_loss5sec) ||
                (!system_state_machine_DW.ip_loss5sec))) {
      /* Transition: '<S8>:798' */
      system_state_machine_DW.is_HoverTimer = system_state_machine_IN_TimerInit;

      /* Entry 'TimerInit': '<S8>:657' */
      /* '<S8>:657:3' bHover5sec=false; */
      system_state_machine_DW.bHover5sec = false;

      /* '<S8>:657:4' TimerHover=0; */
      system_state_machine_DW.TimerHover = 0.0;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:659:2' TimerHover=TimerHover+ dt; */
      system_state_machine_DW.TimerHover += 0.01;
    }
    break;

   case system_state_machine_IN_HoverStopTimer:
    /* During 'HoverStopTimer': '<S8>:661' */
    /* '<S8>:663:1' sf_internal_predicateOutput = bTelemtryLinkLoss5sec==false && ( ep_loss5sec==false || ip_loss5sec==false ); */
    if ((!system_state_machine_DW.bTelemtryLinkLoss5sec) &&
        ((!system_state_machine_DW.ep_loss5sec) ||
         (!system_state_machine_DW.ip_loss5sec))) {
      /* Transition: '<S8>:663' */
      system_state_machine_DW.is_HoverTimer = system_state_machine_IN_TimerInit;

      /* Entry 'TimerInit': '<S8>:657' */
      /* '<S8>:657:3' bHover5sec=false; */
      system_state_machine_DW.bHover5sec = false;

      /* '<S8>:657:4' TimerHover=0; */
      system_state_machine_DW.TimerHover = 0.0;
    } else {
      /* '<S8>:661:2' bHover5sec=true; */
      system_state_machine_DW.bHover5sec = true;
    }
    break;

   default:
    /* During 'TimerInit': '<S8>:657' */
    /* '<S8>:660:1' sf_internal_predicateOutput = f_mode_unitdelay==vom_t.VOM_HOVER && (bTelemtryLinkLoss5sec || (ep_loss5sec&&ip_loss5sec)); */
    if (((*vom_status) == VOM_HOVER) &&
        ((system_state_machine_DW.bTelemtryLinkLoss5sec) ||
         ((system_state_machine_DW.ep_loss5sec) &&
          (system_state_machine_DW.ip_loss5sec)))) {
      /* Transition: '<S8>:660' */
      system_state_machine_DW.is_HoverTimer =
        system_state_machine_IN_HoverStartTimer;

      /* Constant: '<S1>/dt' */
      /* Entry 'HoverStartTimer': '<S8>:659' */
      /* '<S8>:659:2' TimerHover=TimerHover+ dt; */
      system_state_machine_DW.TimerHover += 0.01;
    }
    break;
  }

  system_state_machine_dfezhqrijv(rty_sFlags_bGPSLoss10sec, rty_sFlags_bGPSLoss);

  /* During 'ip_loss': '<S8>:2305' */
  switch (system_state_machine_DW.is_ip_loss) {
   case system_state_machine_IN_RC_loss_false:
    /* During 'RC_loss_false': '<S8>:2326' */
    /* '<S8>:2314:1' sf_internal_predicateOutput = sContingencyFlags.ip_data_loss==1; */
    if (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss)
    {
      /* Transition: '<S8>:2314' */
      system_state_machine_DW.is_ip_loss =
        system_state_machine_IN_RC_loss_timer1_start;

      /* Entry 'RC_loss_timer1_start': '<S8>:2327' */
      /* '<S8>:2327:3' timer_RC=0; */
      system_state_machine_DW.timer_RC = 0.0;
    }
    break;

   case system_state_machine_IN_RC_loss_timer1_start:
    /* During 'RC_loss_timer1_start': '<S8>:2327' */
    /* '<S8>:2318:1' sf_internal_predicateOutput = timer_RC>=0.25; */
    if (system_state_machine_DW.timer_RC >= 0.25) {
      /* Transition: '<S8>:2318' */
      system_state_machine_DW.is_ip_loss =
        system_state_machine_IN_RC_loss_timer2_start;

      /* Entry 'RC_loss_timer2_start': '<S8>:2328' */
      /* '<S8>:2328:3' ip_loss250ms=true; */
      system_state_machine_DW.ip_loss250ms = true;

      /* Transition: '<S8>:2316' */
      /* Transition: '<S8>:2313' */
      /* '<S8>:2312:1' sf_internal_predicateOutput = sContingencyFlags.ip_data_loss==0; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss)
    {
      /* Transition: '<S8>:2312' */
      system_state_machine_DW.is_ip_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:2326' */
      /* '<S8>:2326:3' ip_loss250ms=false; */
      system_state_machine_DW.ip_loss250ms = false;

      /* '<S8>:2326:4' ip_loss1sec=false; */
      system_state_machine_DW.ip_loss1sec = false;

      /* '<S8>:2326:5' ip_loss5sec=false; */
      system_state_machine_DW.ip_loss5sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:2327:5' timer_RC=timer_RC+ dt; */
      system_state_machine_DW.timer_RC += 0.01;
    }
    break;

   case system_state_machine_IN_RC_loss_timer2_start:
    /* During 'RC_loss_timer2_start': '<S8>:2328' */
    /* '<S8>:2321:1' sf_internal_predicateOutput = timer_RC>=1; */
    if (system_state_machine_DW.timer_RC >= 1.0) {
      /* Transition: '<S8>:2321' */
      system_state_machine_DW.is_ip_loss =
        system_state_machine_IN_RC_loss_timer3_start;

      /* Entry 'RC_loss_timer3_start': '<S8>:2329' */
      /* '<S8>:2329:3' ip_loss1sec=true; */
      system_state_machine_DW.ip_loss1sec = true;

      /* Transition: '<S8>:2319' */
      /* Transition: '<S8>:2317' */
      /* Transition: '<S8>:2313' */
      /* '<S8>:2312:1' sf_internal_predicateOutput = sContingencyFlags.ip_data_loss==0; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss)
    {
      /* Transition: '<S8>:2312' */
      system_state_machine_DW.is_ip_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:2326' */
      /* '<S8>:2326:3' ip_loss250ms=false; */
      system_state_machine_DW.ip_loss250ms = false;

      /* '<S8>:2326:4' ip_loss1sec=false; */
      system_state_machine_DW.ip_loss1sec = false;

      /* '<S8>:2326:5' ip_loss5sec=false; */
      system_state_machine_DW.ip_loss5sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:2328:5' timer_RC=timer_RC+ dt; */
      system_state_machine_DW.timer_RC += 0.01;
    }
    break;

   case system_state_machine_IN_RC_loss_timer2_trigger:
    /* During 'RC_loss_timer2_trigger': '<S8>:2330' */
    /* Transition: '<S8>:2325' */
    /* Transition: '<S8>:2323' */
    /* Transition: '<S8>:2320' */
    /* Transition: '<S8>:2317' */
    /* Transition: '<S8>:2313' */
    /* '<S8>:2312:1' sf_internal_predicateOutput = sContingencyFlags.ip_data_loss==0; */
    if (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss)
    {
      /* Transition: '<S8>:2312' */
      system_state_machine_DW.is_ip_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:2326' */
      /* '<S8>:2326:3' ip_loss250ms=false; */
      system_state_machine_DW.ip_loss250ms = false;

      /* '<S8>:2326:4' ip_loss1sec=false; */
      system_state_machine_DW.ip_loss1sec = false;

      /* '<S8>:2326:5' ip_loss5sec=false; */
      system_state_machine_DW.ip_loss5sec = false;
    }
    break;

   default:
    /* During 'RC_loss_timer3_start': '<S8>:2329' */
    /* '<S8>:2324:1' sf_internal_predicateOutput = timer_RC>=5.0; */
    if (system_state_machine_DW.timer_RC >= 5.0) {
      /* Transition: '<S8>:2324' */
      system_state_machine_DW.is_ip_loss =
        system_state_machine_IN_RC_loss_timer2_trigger;

      /* Entry 'RC_loss_timer2_trigger': '<S8>:2330' */
      /* '<S8>:2330:3' ip_loss5sec=true; */
      system_state_machine_DW.ip_loss5sec = true;

      /* Transition: '<S8>:2322' */
      /* Transition: '<S8>:2320' */
      /* Transition: '<S8>:2317' */
      /* Transition: '<S8>:2313' */
      /* '<S8>:2312:1' sf_internal_predicateOutput = sContingencyFlags.ip_data_loss==0; */
    } else if
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss)
    {
      /* Transition: '<S8>:2312' */
      system_state_machine_DW.is_ip_loss = system_state_machine_IN_RC_loss_false;

      /* Entry 'RC_loss_false': '<S8>:2326' */
      /* '<S8>:2326:3' ip_loss250ms=false; */
      system_state_machine_DW.ip_loss250ms = false;

      /* '<S8>:2326:4' ip_loss1sec=false; */
      system_state_machine_DW.ip_loss1sec = false;

      /* '<S8>:2326:5' ip_loss5sec=false; */
      system_state_machine_DW.ip_loss5sec = false;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:2329:5' timer_RC=timer_RC+ dt; */
      system_state_machine_DW.timer_RC += 0.01;
    }
    break;
  }

  /* During 'EP_IP_Loss_5sec': '<S8>:2846' */
  switch (system_state_machine_DW.is_EP_IP_Loss_5sec) {
   case system_state_machine_IN_HoverStartTimer:
    /* During 'HoverStartTimer': '<S8>:2852' */
    /* '<S8>:2848:1' sf_internal_predicateOutput = TimerEPIPLoss>5; */
    if (system_state_machine_DW.TimerEPIPLoss > 5.0) {
      /* Transition: '<S8>:2848' */
      system_state_machine_DW.is_EP_IP_Loss_5sec =
        system_state_machine_IN_HoverStopTimer;

      /* Entry 'HoverStopTimer': '<S8>:2853' */
      /* '<S8>:2853:2' bEPIPLossFlag=true; */
      system_state_machine_DW.bEPIPLossFlag = true;

      /* '<S8>:2850:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==false  ||  sContingencyFlags.ip_data_loss==false; */
    } else if
        ((!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
         ||
         (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
    {
      /* Transition: '<S8>:2850' */
      system_state_machine_DW.is_EP_IP_Loss_5sec =
        system_state_machine_IN_TimerInit;

      /* Entry 'TimerInit': '<S8>:2854' */
      /* '<S8>:2854:3' bEPIPLossFlag=false; */
      system_state_machine_DW.bEPIPLossFlag = false;

      /* '<S8>:2854:4' TimerEPIPLoss=0; */
      system_state_machine_DW.TimerEPIPLoss = 0.0;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:2852:2' TimerEPIPLoss=TimerEPIPLoss+ dt; */
      system_state_machine_DW.TimerEPIPLoss += 0.01;
    }
    break;

   case system_state_machine_IN_HoverStopTimer:
    /* During 'HoverStopTimer': '<S8>:2853' */
    /* '<S8>:2851:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==false  ||  sContingencyFlags.ip_data_loss==false; */
    if ((!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
        ||
        (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
    {
      /* Transition: '<S8>:2851' */
      system_state_machine_DW.is_EP_IP_Loss_5sec =
        system_state_machine_IN_TimerInit;

      /* Entry 'TimerInit': '<S8>:2854' */
      /* '<S8>:2854:3' bEPIPLossFlag=false; */
      system_state_machine_DW.bEPIPLossFlag = false;

      /* '<S8>:2854:4' TimerEPIPLoss=0; */
      system_state_machine_DW.TimerEPIPLoss = 0.0;
    } else {
      /* '<S8>:2853:2' bEPIPLossFlag=true; */
      system_state_machine_DW.bEPIPLossFlag = true;
    }
    break;

   default:
    /* During 'TimerInit': '<S8>:2854' */
    /* '<S8>:2849:1' sf_internal_predicateOutput = sContingencyFlags.ep_data_loss==true && sContingencyFlags.ip_data_loss==true; */
    if ((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss)
        &&
        (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss))
    {
      /* Transition: '<S8>:2849' */
      system_state_machine_DW.is_EP_IP_Loss_5sec =
        system_state_machine_IN_HoverStartTimer;

      /* Constant: '<S1>/dt' */
      /* Entry 'HoverStartTimer': '<S8>:2852' */
      /* '<S8>:2852:2' TimerEPIPLoss=TimerEPIPLoss+ dt; */
      system_state_machine_DW.TimerEPIPLoss += 0.01;
    }
    break;
  }
}

/* Function for Chart: '<S1>/fcs_state_machine_stateflow' */
static void system_state_machine_fhrgqlbunr(const boolean_T
  *FixPtRelationalOperator, const boolean_T *FixPtRelationalOperator_cwrqbb5ufj,
  const real_T *Gain, const vom_t *vom_status, const real_T *Switch, const
  real_T *Switch_kc4btehf20, const real_T rtu_sensor_eul_ang[3], const real_T
  rtu_sensor_pos_lla[3], const real_T *rtu_sensor_aspd_cas, const real_T
  *rtu_sensor_h_radar_agl, const real_T *rtu_sensor_chi, const int8_T
  *rtu_pilot_input_switch_1, const real_T *rtu_controllerAltCtrl_forceDes, const
  boolean_T *rtu_wp_data_wp_list_valid, boolean_T *rty_bInAirFlag, vom_t
  *rty_vom_status, busMode_data *rty_mode_data, boolean_T
  *rty_sFlags_bGPSLoss10sec, boolean_T *rty_sFlags_bGPSLoss, boolean_T
  *rty_sFlags_rampup_phase, boolean_T *rty_sFlags_bTelemtryLinkLoss1sec,
  boolean_T *rty_sFlags_ep_loss1sec, safety_state_t *rty_safety_state, uint8_T
  *rty_loiter_mode, uint8_T *rty_TECS_mode, lifter_state_t *rty_lifter_state,
  pic_t *rty_pic_status, uint8_T *rty_CoG_tracking, real_T *rty_h_rel_takeoff)
{
  boolean_T c_out;
  boolean_T guard1;

  /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
  /* Chart: '<S1>/fcs_state_machine_stateflow' */
  /* During 'FlightManagement': '<S8>:161' */
  system_state_machine_phh325i5jc(Gain, rtu_sensor_eul_ang, rtu_sensor_pos_lla,
    rtu_sensor_aspd_cas, rtu_sensor_h_radar_agl, rtu_sensor_chi,
    rtu_pilot_input_switch_1, rtu_wp_data_wp_list_valid, rty_vom_status,
    rty_mode_data, rty_sFlags_bGPSLoss, rty_sFlags_rampup_phase,
    rty_sFlags_ep_loss1sec, rty_loiter_mode, rty_pic_status, rty_h_rel_takeoff);
  system_state_machine_hkbbt45tg5(vom_status, rtu_sensor_h_radar_agl,
    rtu_controllerAltCtrl_forceDes, rty_bInAirFlag, rty_h_rel_takeoff);
  system_state_machine_g0rztozkme(vom_status, rty_sFlags_bGPSLoss10sec,
    rty_sFlags_bGPSLoss, rty_sFlags_bTelemtryLinkLoss1sec,
    rty_sFlags_ep_loss1sec);

  /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
  /* During 'Safety_SM': '<S8>:952' */
  switch (system_state_machine_DW.is_Safety_SM) {
   case system_state_machine_IN_ARMED:
    /* During 'ARMED': '<S8>:993' */
    /* '<S8>:996:1' sf_internal_predicateOutput = ARM_knob_fallingedge>0; */
    if ((*FixPtRelationalOperator_cwrqbb5ufj) ||
        (((((system_state_machine_DW.bTelemtryLinkLoss5sec) &&
            (system_state_machine_DW.ep_loss5sec)) &&
           (system_state_machine_DW.ip_loss5sec)) &&
          (system_state_machine_DW.bGPSLoss2sec)) || ((*rty_vom_status) ==
          VOM_READY))) {
      /* Transition: '<S8>:996' */
      /* Transition: '<S8>:1399' */
      /* Transition: '<S8>:2836' */
      system_state_machine_DW.is_Safety_SM = system_state_machine_IN_DISARMED;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'DISARMED': '<S8>:954' */
      /* '<S8>:954:2' safety_state=safety_state_t.AC_DISARMED; */
      *rty_safety_state = AC_DISARMED;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
      /* '<S8>:954:3' safety_state_time=0; */
      system_state_machine_DW.safety_state_time = 0.0;
    } else {
      /* '<S8>:1399:1' sf_internal_predicateOutput = bTelemtryLinkLoss5sec==true && ep_loss5sec==true && ip_loss5sec&& bGPSLoss2sec==true; */
      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* '<S8>:2836:1' sf_internal_predicateOutput = vom_status==vom_t.VOM_READY; */
      /* '<S8>:993:2' safety_state=safety_state_t.AC_ARMED; */
      *rty_safety_state = AC_ARMED;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    }
    break;

   case system_state_machine_IN_DISARMED:
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    /* During 'DISARMED': '<S8>:954' */
    /* '<S8>:1001:1' sf_internal_predicateOutput = f_mode_unitdelay==vom_t.VOM_READY && ... */
    /* '<S8>:1001:2'  ( vom_status==vom_t.VOM_STARTUP ||  vom_status==vom_t.VOM_ZEROG); */
    c_out = (((*vom_status) == VOM_READY) && (((*rty_vom_status) == VOM_STARTUP)
              || ((*rty_vom_status) == VOM_ZEROG)));

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    if (c_out) {
      /* Transition: '<S8>:1001' */
      /*  PC-1010 */
      system_state_machine_DW.is_Safety_SM = system_state_machine_IN_state;

      /* Entry 'state': '<S8>:1000' */
    } else {
      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* '<S8>:954:2' safety_state=safety_state_t.AC_DISARMED; */
      *rty_safety_state = AC_DISARMED;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
      /* '<S8>:954:3' safety_state_time=0; */
      system_state_machine_DW.safety_state_time = 0.0;
    }
    break;

   default:
    /* During 'state': '<S8>:1000' */
    /* '<S8>:1002:1' sf_internal_predicateOutput = safety_state_time<2  && ARM_knob_risingedge>0; */
    if ((system_state_machine_DW.safety_state_time < 2.0) &&
        (*FixPtRelationalOperator)) {
      /* Transition: '<S8>:1002' */
      system_state_machine_DW.is_Safety_SM = system_state_machine_IN_ARMED;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'ARMED': '<S8>:993' */
      /* '<S8>:993:2' safety_state=safety_state_t.AC_ARMED; */
      *rty_safety_state = AC_ARMED;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */

      /* '<S8>:1003:1' sf_internal_predicateOutput = safety_state_time>2; */
    } else if (system_state_machine_DW.safety_state_time > 2.0) {
      /* Transition: '<S8>:1003' */
      system_state_machine_DW.is_Safety_SM = system_state_machine_IN_DISARMED;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'DISARMED': '<S8>:954' */
      /* '<S8>:954:2' safety_state=safety_state_t.AC_DISARMED; */
      *rty_safety_state = AC_DISARMED;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
      /* '<S8>:954:3' safety_state_time=0; */
      system_state_machine_DW.safety_state_time = 0.0;
    } else {
      /* Constant: '<S1>/dt' */
      /* '<S8>:1000:3' safety_state_time=safety_state_time+dt; */
      system_state_machine_DW.safety_state_time += 0.01;
    }
    break;
  }

  /* During 'Loiter_SM': '<S8>:1349' */
  if (system_state_machine_DW.is_Loiter_SM == ((uint8_T)
       system_state_machine_IN_Loiter_off)) {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_loiter_mode = 0U;

    /* During 'Loiter_off': '<S8>:1352' */
    /* '<S8>:1355:1' sf_internal_predicateOutput = loiter_cmd==1 && dGroundSpeed_mps>6 && (abs(Chi_Yaw_err) < pi/6) && sContingencyFlags.gps_loss==false && iRCoverride==0; */
    if ((((*Switch) == 1.0) && (system_state_machine_B.Max > 6.0)) && ((fabs
          (system_state_machine_B.Add4) < 0.52359877559829882) &&
         ((!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
          && ((*rtu_pilot_input_switch_1) == ((int8_T)0))))) {
      /* Transition: '<S8>:1355' */
      system_state_machine_DW.is_Loiter_SM = system_state_machine_IN_Loiter_on;

      /* Entry 'Loiter_on': '<S8>:1354' */
      /* '<S8>:1354:3' loiter_mode = 1; */
      *rty_loiter_mode = 1U;
    }

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
  } else {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_loiter_mode = 1U;

    /* During 'Loiter_on': '<S8>:1354' */
    /* '<S8>:1356:1' sf_internal_predicateOutput = loiter_cmd==0 || (abs(Chi_Yaw_err) > 80*pi/180) ||dGroundSpeed_mps<4 || sContingencyFlags.gps_loss==true || iRCoverride==1; */
    if (((*Switch) == 0.0) || ((fabs(system_state_machine_B.Add4) >
          1.3962634015954636) || (((system_state_machine_B.Max < 4.0) ||
           (system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss))
          || ((*rtu_pilot_input_switch_1) == ((int8_T)1))))) {
      /* Transition: '<S8>:1356' */
      system_state_machine_DW.is_Loiter_SM = system_state_machine_IN_Loiter_off;

      /* Entry 'Loiter_off': '<S8>:1352' */
      /* '<S8>:1352:3' loiter_mode = 0; */
      *rty_loiter_mode = 0U;
    }

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
  }

  /* During 'TECS_SM': '<S8>:1359' */
  if (system_state_machine_DW.is_TECS_SM == ((uint8_T)
       system_state_machine_IN_TECS_off)) {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_TECS_mode = 0U;
    *rty_lifter_state = ON;

    /* During 'TECS_off': '<S8>:1360' */
    /* '<S8>:1363:1' sf_internal_predicateOutput = TECS_cmd == 1 && dAirspeedCAS > 18 && vom_status==vom_t.VOM_UMAN && sContingencyFlags.gps_loss==false && iRCoverride==0; */
    c_out = ((((((*Switch_kc4btehf20) == 1.0) && ((*rtu_sensor_aspd_cas) > 18.0))
               && ((*rty_vom_status) == VOM_UMAN)) &&
              (!system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss))
             && ((*rtu_pilot_input_switch_1) == ((int8_T)0)));

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    if (c_out) {
      /* Transition: '<S8>:1363' */
      /* PC - 1224 */
      system_state_machine_DW.is_TECS_SM = system_state_machine_IN_TECS_on;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'TECS_on': '<S8>:1362' */
      /* '<S8>:1362:3' TECS_mode = 1; */
      *rty_TECS_mode = 1U;

      /* '<S8>:1362:4' lifter_state=lifter_state_t.OFF; */
      *rty_lifter_state = OFF;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    }
  } else {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_TECS_mode = 1U;
    *rty_lifter_state = OFF;

    /* During 'TECS_on': '<S8>:1362' */
    /* '<S8>:1364:1' sf_internal_predicateOutput = TECS_cmd==0 || dAirspeedCAS<16; */
    if ((((*Switch_kc4btehf20) == 0.0) || ((*rtu_sensor_aspd_cas) < 16.0)) ||
        (((system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss)
          || ((*rtu_pilot_input_switch_1) == ((int8_T)1))) || ((*rty_vom_status)
          != VOM_UMAN))) {
      /* Transition: '<S8>:1364' */
      /* PC - 1225 */
      /* Transition: '<S8>:1440' */
      /* PC - 1225 */
      /* Transition: '<S8>:1441' */
      /* PC - 1225 */
      system_state_machine_DW.is_TECS_SM = system_state_machine_IN_TECS_off;

      /* Entry 'TECS_off': '<S8>:1360' */
      /* '<S8>:1360:3' TECS_mode = 0; */
      *rty_TECS_mode = 0U;

      /* '<S8>:1360:4' lifter_state=lifter_state_t.ON; */
      *rty_lifter_state = ON;
    } else {
      /* '<S8>:1440:1' sf_internal_predicateOutput = sContingencyFlags.gps_loss==true || iRCoverride==1; */
      /* '<S8>:1441:1' sf_internal_predicateOutput = vom_status~=vom_t.VOM_UMAN; */
    }

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
  }

  /* During 'PIC_SM': '<S8>:1411' */
  if (system_state_machine_DW.is_PIC_SM == ((uint8_T)
       system_state_machine_IN_PIC_EXT)) {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_pic_status = EXTERNAL;

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    /* During 'PIC_EXT': '<S8>:1412' */
    /* '<S8>:1415:1' sf_internal_predicateOutput = (pic_cmd == pic_t.INTERNAL && sContingencyFlags.ip_data_loss==false); */
    c_out =
      !system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss;
    guard1 = false;
    if ((system_state_machine_B.Switch_po1f1vqhhu == INTERNAL) && c_out) {
      /* Transition: '<S8>:1415' */
      guard1 = true;
    } else {
      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* '<S8>:2337:1' sf_internal_predicateOutput =  (ep_loss1sec ==true && sContingencyFlags.ip_data_loss==false)&&(f_mode_unitdelay ~=vom_t.VOM_MANUAL); */
      c_out = (((*rty_sFlags_ep_loss1sec) && c_out) && ((*vom_status) !=
                VOM_MANUAL));

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
      if (c_out) {
        /* Transition: '<S8>:2337' */
        guard1 = true;
      }
    }

    if (guard1) {
      system_state_machine_DW.is_PIC_SM = system_state_machine_IN_PIC_INT;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'PIC_INT': '<S8>:1413' */
      /* '<S8>:1413:3' pic_status = pic_t.INTERNAL; */
      *rty_pic_status = INTERNAL;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    }
  } else {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_pic_status = INTERNAL;

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    /* During 'PIC_INT': '<S8>:1413' */
    /* '<S8>:1416:1' sf_internal_predicateOutput =  (iRCoverride==1 && sContingencyFlags.ep_data_loss==false && (f_mode_unitdelay ==vom_t.VOM_HOVER || f_mode_unitdelay ==vom_t.VOM_MANUAL || f_mode_unitdelay ==vom_t.VOM_TAKEOFF || f_mode_unitdelay ==vom_t.VOM_LAND )); */
    c_out =
      !system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss;
    if ((((((*rtu_pilot_input_switch_1) == ((int8_T)1)) && c_out) &&
          (((((*vom_status) == VOM_HOVER) || ((*vom_status) == VOM_MANUAL)) ||
            ((*vom_status) == VOM_TAKEOFF)) || ((*vom_status) == VOM_LAND))) ||
         ((system_state_machine_B.Switch_po1f1vqhhu == EXTERNAL) && c_out)) ||
        ((system_state_machine_DW.ip_loss1sec) && c_out)) {
      /* Transition: '<S8>:1416' */
      /* Transition: '<S8>:2338' */
      /* Transition: '<S8>:2339' */
      system_state_machine_DW.is_PIC_SM = system_state_machine_IN_PIC_EXT;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'PIC_EXT': '<S8>:1412' */
      /* '<S8>:1412:3' pic_status = pic_t.EXTERNAL; */
      *rty_pic_status = EXTERNAL;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    } else {
      /* '<S8>:2338:1' sf_internal_predicateOutput = (pic_cmd == pic_t.EXTERNAL && sContingencyFlags.ep_data_loss==false); */
      /* '<S8>:2339:1' sf_internal_predicateOutput = (ip_loss1sec ==true && sContingencyFlags.ep_data_loss==false); */
    }
  }

  /* During 'CoG_SM': '<S8>:1420' */
  if (system_state_machine_DW.is_CoG_SM == ((uint8_T)
       system_state_machine_IN_CoG_tracking_off)) {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_CoG_tracking = 0U;

    /* During 'CoG_tracking_off': '<S8>:1423' */
    /* '<S8>:1424:1' sf_internal_predicateOutput = (vom_status==vom_t.VOM_UMAN ) && dGroundSpeed_mps>6 && abs(Chi_Yaw_err) < 20*pi/180 && bGPSLoss==false; */
    c_out = ((((*rty_vom_status) == VOM_UMAN) && (system_state_machine_B.Max >
               6.0)) && ((fabs(system_state_machine_B.Add4) < 0.3490658503988659)
                         && (!(*rty_sFlags_bGPSLoss))));

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    if (c_out) {
      /* Transition: '<S8>:1424' */
      system_state_machine_DW.is_CoG_SM =
        system_state_machine_IN_CoG_tracking_on;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'CoG_tracking_on': '<S8>:1422' */
      /* '<S8>:1422:3' CoG_tracking = 1; */
      *rty_CoG_tracking = 1U;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    }
  } else {
    /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
    /* Chart: '<S1>/fcs_state_machine_stateflow' */
    *rty_CoG_tracking = 1U;

    /* During 'CoG_tracking_on': '<S8>:1422' */
    /* '<S8>:1426:1' sf_internal_predicateOutput = (vom_status~=vom_t.VOM_UMAN ) || (abs(Chi_Yaw_err) > 80*pi/180) || dGroundSpeed_mps<4 || bGPSLoss==true; */
    c_out = (((*rty_vom_status) != VOM_UMAN) || ((fabs
               (system_state_machine_B.Add4) > 1.3962634015954636) ||
              ((system_state_machine_B.Max < 4.0) || (*rty_sFlags_bGPSLoss))));

    /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    if (c_out) {
      /* Transition: '<S8>:1426' */
      system_state_machine_DW.is_CoG_SM =
        system_state_machine_IN_CoG_tracking_off;

      /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
      /* Chart: '<S1>/fcs_state_machine_stateflow' */
      /* Entry 'CoG_tracking_off': '<S8>:1423' */
      /* '<S8>:1423:3' CoG_tracking = 0; */
      *rty_CoG_tracking = 0U;

      /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
    }
  }
}

/* System initialize for referenced model: 'system_state_machine' */
void system_state_machine_Init(boolean_T *rty_bInAirFlag, vom_t *rty_vom_status,
  busMode_data *rty_mode_data, boolean_T *rty_sFlags_bGPSLoss10sec, boolean_T
  *rty_sFlags_bGPSLoss, boolean_T *rty_sFlags_rampup_phase, boolean_T
  *rty_sFlags_bTelemtryLinkLoss1sec, boolean_T *rty_sFlags_ep_loss1sec,
  safety_state_t *rty_safety_state, uint8_T *rty_loiter_mode, uint8_T
  *rty_TECS_mode, lifter_state_t *rty_lifter_state, pic_t *rty_pic_status,
  uint8_T *rty_CoG_tracking)
{
  /* SystemInitialize for Atomic SubSystem: '<Root>/system_state_machine' */
  /* InitializeConditions for UnitDelay: '<S1>/Unit Delay1' */
  system_state_machine_DW.UnitDelay1_DSTATE_jsa3d2gsdq = VOM_READY;

  /* InitializeConditions for Delay: '<S6>/Delay One Step1' */
  system_state_machine_DW.icLoad = true;

  /* SystemInitialize for Chart: '<S1>/fcs_state_machine_stateflow' */
  system_state_machine_DW.temporalCounter_i2 = 0U;
  system_state_machine_DW.temporalCounter_i1 = 0U;
  system_state_machine_DW.temporalCounter_i3 = 0U;
  *rty_bInAirFlag = false;
  system_state_machine_DW.timer_Tele = 0.0;
  system_state_machine_DW.bTelemtryLinkLoss5sec = false;
  system_state_machine_DW.timer_RC = 0.0;
  system_state_machine_DW.bHover5sec = false;
  system_state_machine_DW.TimerHover = 0.0;
  *rty_vom_status = VOM_INVALID;
  *rty_sFlags_bGPSLoss10sec = false;
  system_state_machine_DW.timer_GPSLoss = 0.0;
  *rty_sFlags_bGPSLoss = false;
  *rty_safety_state = AC_DISARMED;
  system_state_machine_DW.safety_state_time = 0.0;
  system_state_machine_DW.rampup_timer = 0.0;
  *rty_sFlags_rampup_phase = false;
  rty_mode_data->startup_data.startup_engineARMFlag = false;
  rty_mode_data->startup_data.startup_motorARMFlag = false;
  rty_mode_data->hover_data.hover_y = 0.0;
  rty_mode_data->hover_data.hover_x = 0.0;
  rty_mode_data->hover_data.hover_alt_agl = 0.0;
  rty_mode_data->hover_data.hover_yaw_ref = 0.0;
  rty_mode_data->hover_data.hover_yaw_override = false;
  rty_mode_data->autotakeoff_data.takeoff_x = 0.0;
  rty_mode_data->autotakeoff_data.takeoff_y = 0.0;
  rty_mode_data->autotakeoff_data.takeoff_alt_agl = 0.0;
  rty_mode_data->autotakeoff_data.takeoff_yaw_ref = 0.0;
  rty_mode_data->autoland_data.land_x = 0.0;
  rty_mode_data->autoland_data.land_y = 0.0;
  rty_mode_data->autoland_data.land_yaw_ref = 0.0;
  rty_mode_data->rth_data.rth_x = 0.0;
  rty_mode_data->rth_data.rth_y = 0.0;
  rty_mode_data->rth_data.rth_velX = 0.0;
  rty_mode_data->rth_data.rth_velY = 0.0;
  rty_mode_data->rth_data.rth_alt_agl = 0.0;
  rty_mode_data->rth_data.rth_yaw_ref = 0.0;
  rty_mode_data->rth_data.bRTH_PosModeFlag = false;
  rty_mode_data->rth_data.bBreaking = false;
  rty_mode_data->ft_data.FT_x = 0.0;
  rty_mode_data->ft_data.FT_y = 0.0;
  rty_mode_data->ft_data.FT_Altitude = 0.0;
  rty_mode_data->ft_data.FT_Heading = 0.0;
  rty_mode_data->ft_data.FT_AirspeedRef = 0.0;
  rty_mode_data->ft_data.FT_CoG = 0.0;
  rty_mode_data->ft_data.FT_PusherThrottle = 0.0;
  rty_mode_data->ft_data.bFT_AttitudeModeFlag = false;
  rty_mode_data->ft_data.FT_PitchCmd = 0.0;
  rty_mode_data->ft_data.FT_RollCmd = 0.0;
  rty_mode_data->ft_data.eLifter_Mode = ON;
  rty_mode_data->flightDirector_data.FD_CoG = 0.0;
  rty_mode_data->flightDirector_data.FD_AirspeedRef = 0.0;
  rty_mode_data->flightDirector_data.FD_Altitude = 0.0;
  rty_mode_data->flightDirector_data.FTransition_y = 0.0;
  rty_mode_data->loiter_data.loiter_radius = 0.0;
  rty_mode_data->loiter_data.loiter_direction = 0.0;
  rty_mode_data->loiter_data.loiter_Center_Lat = 0.0;
  rty_mode_data->loiter_data.loiter_Center_Lon = 0.0;
  rty_mode_data->loiter_data.loiter_altitude = 0.0;
  rty_mode_data->loiter_data.loiter_AirSpeedRef = 0.0;
  rty_mode_data->bt_data.BT_Hover_Lat = 0.0;
  rty_mode_data->bt_data.BT_Hover_Lon = 0.0;
  rty_mode_data->bt_data.BT_Altitude = 0.0;
  rty_mode_data->bt_data.BT_Heading = 0.0;
  rty_mode_data->bt_data.BT_PusherThrottle = 0.0;
  rty_mode_data->bt_data.bBT_AttitudeModeFlag = false;
  rty_mode_data->bt_data.BT_PitchCmd = 0.0;
  rty_mode_data->bt_data.BT_RollCmd = 0.0;
  rty_mode_data->bt_data.eLifter_Mode = ON;
  rty_mode_data->eFWLifter_Mode = ON;
  rty_mode_data->fwrth_data.phase = approach_circle;
  rty_mode_data->fwrth_data.CAS_sp = 0.0;
  rty_mode_data->fwrth_data.alt_sp = 0.0;
  system_state_machine_DW.bRTH2LandingFlag = 0.0;
  system_state_machine_DW.bRecoverGPS = 0.0;
  system_state_machine_DW.bTakeoff2Hover = false;
  *rty_loiter_mode = 0U;
  *rty_TECS_mode = 0U;
  *rty_sFlags_bTelemtryLinkLoss1sec = false;
  *rty_lifter_state = ON;
  system_state_machine_DW.bGPSLoss2sec = false;
  *rty_pic_status = EXTERNAL;
  *rty_CoG_tracking = 0U;
  system_state_machine_DW.bBTransition2Hover = false;
  system_state_machine_DW.bBTrans_hover_sp_set = false;
  system_state_machine_DW.goto_back_trans = false;
  system_state_machine_DW.currentLLA[0] = 0.0;
  system_state_machine_DW.currentLLA[1] = 0.0;
  system_state_machine_DW.currentLLA[2] = 0.0;
  system_state_machine_DW.bHover2WPN_yaw_aligned = false;
  system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;
  system_state_machine_DW.ip_loss5sec = false;
  system_state_machine_DW.ip_loss250ms = false;
  system_state_machine_DW.ep_loss250ms = false;
  system_state_machine_DW.ep_loss5sec = false;
  system_state_machine_DW.ip_loss1sec = false;
  *rty_sFlags_ep_loss1sec = false;
  system_state_machine_DW.hover2FLTDIR = false;
  system_state_machine_DW.hover2WAYPNT = false;
  system_state_machine_DW.startup2manual = false;
  system_state_machine_DW.startup2takeoff = false;
  system_state_machine_DW.startup2uman = false;
  system_state_machine_DW.bEPIPLossFlag = false;
  system_state_machine_DW.TimerEPIPLoss = 0.0;
  system_state_machine_DW.is_active_c4_system_state_machine = 0U;
  system_state_machine_DW.is_FMM = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_FW_Modes = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_BackTransition =
    system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_FTransition =
    system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_FW_RTH = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_HOVER = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_RTH = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_STARTUP = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_InAirOnGroundLogic =
    system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_Telemtry_LinkLoss =
    system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_ep_loss = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_HoverTimer =
    system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_GPS_Loss = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_ip_loss = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_EP_IP_Loss_5sec =
    system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_Safety_SM = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_Loiter_SM = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_TECS_SM = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_PIC_SM = system_state_machine_IN_NO_ACTIVE_CHILD;
  system_state_machine_DW.is_CoG_SM = system_state_machine_IN_NO_ACTIVE_CHILD;

  /* End of SystemInitialize for SubSystem: '<Root>/system_state_machine' */
}

/* Output and update for referenced model: 'system_state_machine' */
void system_state_machine(const vom_t *rtu_std_command_vom_cmd, const uint16_T
  *rtu_std_command_vom_cmd_cnt, const pic_t *rtu_std_command_pic_cmd, const
  uint16_T *rtu_std_command_pic_cmd_cnt, const real_T rtu_sensor_eul_ang[3],
  const real_T rtu_sensor_pos_lla[3], const real_T rtu_sensor_vel_ned[3], const
  real_T *rtu_sensor_aspd_cas, const real_T *rtu_sensor_h_radar_agl, const
  real_T *rtu_sensor_chi, const real_T *rtu_pilot_input_throttle_ch, const
  real_T *rtu_pilot_input_arm_ch, const int8_T *rtu_pilot_input_switch_1, const
  real_T *rtu_controllerAltCtrl_forceDes, const boolean_T
  *rtu_failure_flags_gcs_link_loss, const boolean_T *rtu_failure_flags_gps_loss,
  const boolean_T *rtu_failure_flags_ep_data_loss, const boolean_T
  *rtu_failure_flags_ip_data_loss, const uint8_T *rtu_extd_cmd_loiter_cmd, const
  uint8_T *rtu_extd_cmd_loiter_cmd_cnt, const uint8_T *rtu_extd_cmd_tecs_cmd,
  const uint8_T *rtu_extd_cmd_tecs_cmd_cnt, const boolean_T
  *rtu_FWRTH_SM_in_approach_circle_done, const boolean_T
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
  pic_t *rty_pic_status, uint8_T *rty_CoG_tracking, real_T *rty_h_rel_takeoff)
{
  real_T sin_lat[9];
  real_T rtb_Product_invsy3i2yj[3];
  real_T rtb_Sqrt;
  real_T rtb_Sum_be4knqil4l_idx_0;
  real_T rtb_Sum_evx323najj;
  real_T rtb_Switch;
  real_T rtb_Switch_pjqesw53bu;
  real_T rtb_TrigonometricFunction2;
  real_T rtb_alt;
  int32_T i;
  boolean_T FixPtRelationalOperator;
  boolean_T FixPtRelationalOperator_cwrqbb5ufj;
  boolean_T rtb_Compare_d1opexicre;
  boolean_T rtb_Compare_peqkw15qn5;
  boolean_T rtb_FixPtRelationalOperator;
  vom_t vom_status;

  /* Outputs for Atomic SubSystem: '<Root>/system_state_machine' */
  /* UnitDelay: '<S1>/Unit Delay1' */
  vom_status = system_state_machine_DW.UnitDelay1_DSTATE_jsa3d2gsdq;

  /* UnitDelay: '<S1>/Unit Delay' */
  system_state_machine_B.bInAirFlag = system_state_machine_DW.UnitDelay_DSTATE;

  /* Switch: '<S2>/Switch' incorporates:
   *  Constant: '<S17>/Constant'
   *  Logic: '<S16>/NOT'
   *  RelationalOperator: '<S17>/Compare'
   *  UnitDelay: '<S2>/Unit Delay1'
   */
  if ((*rtu_std_command_vom_cmd) != VOM_READY) {
    rtb_Switch = system_state_machine_DW.UnitDelay1_DSTATE;
  } else {
    rtb_Switch = rtu_sensor_pos_lla[2];
  }

  /* End of Switch: '<S2>/Switch' */

  /* Sum: '<S2>/Add' */
  *rty_h_rel_takeoff = rtu_sensor_pos_lla[2] - rtb_Switch;

  /* Sqrt: '<S9>/Sqrt' incorporates:
   *  Product: '<S9>/Product'
   *  Product: '<S9>/Product1'
   *  Sum: '<S9>/Add'
   */
  rtb_alt = sqrt((rtu_sensor_vel_ned[0] * rtu_sensor_vel_ned[0]) +
                 (rtu_sensor_vel_ned[1] * rtu_sensor_vel_ned[1]));

  /* MinMax: '<S9>/Max' */
  if (rtb_alt >= 0.01) {
    /* MinMax: '<S9>/Max' */
    system_state_machine_B.Max = rtb_alt;
  } else {
    /* MinMax: '<S9>/Max' */
    system_state_machine_B.Max = 0.01;
  }

  /* End of MinMax: '<S9>/Max' */

  /* RelationalOperator: '<S33>/Compare' incorporates:
   *  Constant: '<S33>/Constant'
   */
  rtb_Compare_d1opexicre = ((*rtu_std_command_vom_cmd) == VOM_STARTUP);

  /* RelationalOperator: '<S35>/FixPt Relational Operator' incorporates:
   *  UnitDelay: '<S35>/Delay Input1'
   *
   * Block description for '<S35>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_FixPtRelationalOperator = (((int32_T)(rtb_Compare_d1opexicre ? 1 : 0)) >
    ((int32_T)(system_state_machine_DW.DelayInput1_DSTATE_es42msgisa ? 1 : 0)));

  /* Delay: '<S6>/Delay One Step1' */
  system_state_machine_DW.icLoad = ((rtb_FixPtRelationalOperator &&
    (system_state_machine_PrevZCX.DelayOneStep1_Reset_ZCE != ((uint8_T)POS_ZCSIG)))
    || (system_state_machine_DW.icLoad));
  system_state_machine_PrevZCX.DelayOneStep1_Reset_ZCE =
    rtb_FixPtRelationalOperator ? ((ZCSigState)1) : ((ZCSigState)0);
  if (system_state_machine_DW.icLoad) {
    system_state_machine_DW.DelayOneStep1_DSTATE = rtu_sensor_pos_lla[2];
  }

  rtb_alt = system_state_machine_DW.DelayOneStep1_DSTATE;

  /* Reshape: '<S1>/Reshape1' incorporates:
   *  Constant: '<S6>/AlAin_Runway'
   *  Delay: '<S6>/Delay One Step1'
   */
  system_state_machine_B.sLLA[0] = 0.42700150356474043;
  system_state_machine_B.sLLA[1] = 0.97080864372042563;
  system_state_machine_B.sLLA[2] = system_state_machine_DW.DelayOneStep1_DSTATE;

  /* Sum: '<S10>/Add' */
  rtb_TrigonometricFunction2 = 0.97080864372042563 - rtu_sensor_pos_lla[1];

  /* Trigonometry: '<S10>/Trigonometric Function6' incorporates:
   *  Product: '<S10>/Product'
   *  Product: '<S10>/Product1'
   *  Product: '<S10>/Product2'
   *  Sum: '<S10>/Add1'
   *  Trigonometry: '<S10>/Trigonometric Function'
   *  Trigonometry: '<S10>/Trigonometric Function1'
   *  Trigonometry: '<S10>/Trigonometric Function2'
   *  Trigonometry: '<S10>/Trigonometric Function3'
   *  Trigonometry: '<S10>/Trigonometric Function4'
   *  Trigonometry: '<S10>/Trigonometric Function5'
   */
  system_state_machine_B.TrigonometricFunction6 = atan2(0.91021164717306147 *
    sin(rtb_TrigonometricFunction2), (cos(rtu_sensor_pos_lla[0]) *
    0.4141434019159333) - ((sin(rtu_sensor_pos_lla[0]) * 0.91021164717306147) *
    cos(rtb_TrigonometricFunction2)));

  /* Sum: '<S27>/Sum' */
  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S28>:1' */
  /* '<S28>:1:6' cos_lat = cos(LLA(1)); */
  /* '<S28>:1:7' sin_lat = sin(LLA(1)); */
  /* '<S28>:1:8' cos_long = cos(LLA(2)); */
  /* '<S28>:1:9' sin_long = sin(LLA(2)); */
  /* '<S28>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S28>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S28>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  rtb_TrigonometricFunction2 = rtu_sensor_pos_lla[2] + 6.3818017966873543E+6;

  /* Trigonometry: '<S26>/Trigonometric Function' */
  rtb_Switch_pjqesw53bu = sin(rtu_sensor_pos_lla[0]);

  /* Product: '<S29>/Product' incorporates:
   *  Constant: '<S29>/e'
   */
  rtb_Sum_evx323najj = rtb_Switch_pjqesw53bu * 0.0818191908425;

  /* Sqrt: '<S29>/Sqrt' incorporates:
   *  Constant: '<S29>/const'
   *  Math: '<S29>/Square'
   *  Sum: '<S29>/Add'
   */
  rtb_Sum_evx323najj = sqrt(1.0 - (rtb_Sum_evx323najj * rtb_Sum_evx323najj));

  /* Switch: '<S30>/Switch3' incorporates:
   *  Abs: '<S30>/Abs'
   *  Product: '<S30>/Product'
   */
  if (rtb_Sum_evx323najj <= 0.001) {
    rtb_Sum_evx323najj = -0.001;
  }

  /* End of Switch: '<S30>/Switch3' */

  /* Product: '<S29>/Divide' incorporates:
   *  Constant: '<S29>/R_0'
   */
  rtb_Sum_evx323najj = 6.378137E+6 / rtb_Sum_evx323najj;

  /* Product: '<S26>/Product' incorporates:
   *  Product: '<S26>/Product1'
   *  Sum: '<S26>/Sum'
   *  Trigonometry: '<S26>/Trigonometric Function1'
   *  Trigonometry: '<S26>/Trigonometric Function3'
   */
  rtb_Sqrt = (rtb_Sum_evx323najj + rtu_sensor_pos_lla[2]) * cos
    (rtu_sensor_pos_lla[0]);
  rtb_Product_invsy3i2yj[0] = rtb_Sqrt * cos(rtu_sensor_pos_lla[1]);

  /* MATLAB Function: '<S25>/MATLAB Function1' */
  sin_lat[0] = -0.23383874476525865;
  sin_lat[3] = -0.3418101794814053;
  sin_lat[6] = 0.91021164717306147;
  sin_lat[1] = -0.825342569506369;
  sin_lat[4] = 0.56463230775490036;
  sin_lat[7] = 0.0;
  sin_lat[2] = -0.5139349028887148;
  sin_lat[5] = -0.751236419672439;
  sin_lat[8] = -0.4141434019159333;

  /* Sum: '<S23>/Sum' incorporates:
   *  Constant: '<S26>/1-e^2'
   *  Product: '<S25>/Product'
   *  Product: '<S26>/Product1'
   *  Product: '<S26>/Product2'
   *  Product: '<S26>/Product3'
   *  Product: '<S27>/Product'
   *  Product: '<S27>/Product1'
   *  Product: '<S27>/Product2'
   *  Product: '<S27>/Product3'
   *  Sum: '<S26>/Sum1'
   *  Sum: '<S27>/Sum1'
   *  Trigonometry: '<S26>/Trigonometric Function2'
   *  Trigonometry: '<S27>/Trigonometric Function'
   *  Trigonometry: '<S27>/Trigonometric Function1'
   *  Trigonometry: '<S27>/Trigonometric Function2'
   *  Trigonometry: '<S27>/Trigonometric Function3'
   */
  rtb_Sum_be4knqil4l_idx_0 = rtb_Product_invsy3i2yj[0] -
    ((rtb_TrigonometricFunction2 * 0.91021164717306147) * 0.56463230775490036);
  rtb_TrigonometricFunction2 = (rtb_Sqrt * sin(rtu_sensor_pos_lla[1])) -
    ((rtb_TrigonometricFunction2 * 0.91021164717306147) * 0.825342569506369);
  rtb_Sum_evx323najj = (rtb_Switch_pjqesw53bu * ((0.993305620009879 *
    rtb_Sum_evx323najj) + rtu_sensor_pos_lla[2])) - (0.4141434019159333 *
    (rtu_sensor_pos_lla[2] + 6.3390795904386919E+6));

  /* Product: '<S25>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S25>/Product' */
    rtb_Product_invsy3i2yj[i] = ((sin_lat[i] * rtb_Sum_be4knqil4l_idx_0) +
      (sin_lat[i + 3] * rtb_TrigonometricFunction2)) + (sin_lat[i + 6] *
      rtb_Sum_evx323najj);
  }

  /* End of Product: '<S25>/Product' */

  /* Sqrt: '<S24>/Sqrt' incorporates:
   *  Product: '<S24>/Product'
   *  Product: '<S24>/Product1'
   *  Sum: '<S24>/Add'
   */
  rtb_Sum_evx323najj = sqrt((rtb_Product_invsy3i2yj[0] * rtb_Product_invsy3i2yj
    [0]) + (rtb_Product_invsy3i2yj[1] * rtb_Product_invsy3i2yj[1]));

  /* RelationalOperator: '<S21>/Compare' incorporates:
   *  Constant: '<S21>/Constant'
   */
  rtb_FixPtRelationalOperator = (rtu_sensor_pos_lla[2] >= 30.0);

  /* Switch: '<S5>/Switch1' */
  if (rtb_FixPtRelationalOperator) {
    /* Switch: '<S5>/Switch1' incorporates:
     *  Constant: '<S22>/Constant'
     *  RelationalOperator: '<S22>/Compare'
     */
    system_state_machine_B.Switch1 = (rtb_Sum_evx323najj <= 10.0);
  } else {
    /* RelationalOperator: '<S19>/Compare' incorporates:
     *  Constant: '<S19>/Constant'
     */
    rtb_FixPtRelationalOperator = (rtu_sensor_pos_lla[2] >= 3.0);

    /* Switch: '<S5>/Switch' incorporates:
     *  Constant: '<S20>/Constant'
     *  Constant: '<S5>/Constant1'
     *  Constant: '<S5>/Constant2'
     *  Product: '<S5>/Product'
     *  RelationalOperator: '<S20>/Compare'
     *  RelationalOperator: '<S5>/Relational Operator'
     *  Sum: '<S5>/Add'
     *  Sum: '<S5>/Add1'
     */
    if (rtb_FixPtRelationalOperator) {
      rtb_FixPtRelationalOperator = (rtb_Sum_evx323najj <=
        ((system_state_machine_ConstB.Tan * (rtu_sensor_pos_lla[2] - 3.0)) + 1.5));
    } else {
      rtb_FixPtRelationalOperator = (rtb_Sum_evx323najj <= 1.5);
    }

    /* End of Switch: '<S5>/Switch' */

    /* Switch: '<S5>/Switch1' */
    system_state_machine_B.Switch1 = rtb_FixPtRelationalOperator;
  }

  /* End of Switch: '<S5>/Switch1' */

  /* BusCreator generated from: '<S1>/fcs_state_machine_stateflow' */
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gcs_link_loss
    = *rtu_failure_flags_gcs_link_loss;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.gps_loss
    = *rtu_failure_flags_gps_loss;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ep_data_loss
    = *rtu_failure_flags_ep_data_loss;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb.ip_data_loss
    = *rtu_failure_flags_ip_data_loss;

  /* RelationalOperator: '<S18>/Compare' incorporates:
   *  Constant: '<S18>/Constant'
   *  Constant: '<S3>/Constant'
   *  RelationalOperator: '<S3>/Compare'
   */
  rtb_FixPtRelationalOperator = ((*rtu_pilot_input_arm_ch) >= 0.4);

  /* RelationalOperator: '<S4>/FixPt Relational Operator' incorporates:
   *  UnitDelay: '<S4>/Delay Input1'
   *
   * Block description for '<S4>/Delay Input1':
   *
   *  Store in Global RAM
   */
  FixPtRelationalOperator = (((int32_T)(rtb_FixPtRelationalOperator ? 1 : 0)) >
    ((int32_T)(system_state_machine_DW.DelayInput1_DSTATE_bgpgka0occ ? 1 : 0)));

  /* Switch: '<S7>/Switch' incorporates:
   *  Constant: '<S36>/Constant'
   *  Constant: '<S7>/Constant1'
   *  RelationalOperator: '<S36>/Compare'
   *  UnitDelay: '<S7>/Unit Delay2'
   */
  if ((*rtu_pilot_input_arm_ch) <= -0.4) {
    rtb_Switch_pjqesw53bu = system_state_machine_DW.UnitDelay2_DSTATE;
  } else {
    rtb_Switch_pjqesw53bu = 0.0;
  }

  /* End of Switch: '<S7>/Switch' */

  /* RelationalOperator: '<S39>/Compare' incorporates:
   *  Constant: '<S37>/Constant'
   *  Gain: '<S7>/convert_to_seconds'
   *  RelationalOperator: '<S37>/Compare'
   */
  rtb_Compare_peqkw15qn5 = ((0.01 * rtb_Switch_pjqesw53bu) >= 1.0);

  /* RelationalOperator: '<S38>/FixPt Relational Operator' incorporates:
   *  UnitDelay: '<S38>/Delay Input1'
   *
   * Block description for '<S38>/Delay Input1':
   *
   *  Store in Global RAM
   */
  FixPtRelationalOperator_cwrqbb5ufj = (((int32_T)(rtb_Compare_peqkw15qn5 ? 1 :
    0)) > ((int32_T)(system_state_machine_DW.DelayInput1_DSTATE_hdp33oolxt ? 1 :
                     0)));

  /* BusCreator generated from: '<S1>/fcs_state_machine_stateflow' */
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.approach_circle_done
    = *rtu_FWRTH_SM_in_approach_circle_done;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.reduce_speed_alt_done
    = *rtu_FWRTH_SM_in_reduce_speed_alt_done;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.circle_align_done
    = *rtu_FWRTH_SM_in_circle_align_done;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.cross_tangent_point_done
    = *rtu_FWRTH_SM_in_cross_tangent_point_done;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.land_lat
    = *rtu_FWRTH_SM_in_land_lat;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.land_lon
    = *rtu_FWRTH_SM_in_land_lon;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe.approach_ang
    = *rtu_FWRTH_SM_in_approach_ang;

  /* Gain: '<S1>/Gain' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Sum: '<S1>/Add'
   */
  rtb_Sum_evx323najj = 0.5 * ((*rtu_pilot_input_throttle_ch) + 1.0);

  /* Switch: '<S11>/Switch' incorporates:
   *  RelationalOperator: '<S68>/FixPt Relational Operator'
   *  UnitDelay: '<S68>/Delay Input1'
   *
   * Block description for '<S68>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((*rtu_extd_cmd_loiter_cmd_cnt) !=
      system_state_machine_DW.DelayInput1_DSTATE_lufocpaa5h) {
    /* Switch: '<S11>/Switch' */
    rtb_TrigonometricFunction2 = (real_T)(*rtu_extd_cmd_loiter_cmd);
  } else {
    /* Switch: '<S11>/Switch' incorporates:
     *  Constant: '<S11>/pic_t '
     */
    rtb_TrigonometricFunction2 = 255.0;
  }

  /* End of Switch: '<S11>/Switch' */

  /* Switch: '<S12>/Switch' incorporates:
   *  RelationalOperator: '<S69>/FixPt Relational Operator'
   *  UnitDelay: '<S69>/Delay Input1'
   *
   * Block description for '<S69>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((*rtu_extd_cmd_tecs_cmd_cnt) !=
      system_state_machine_DW.DelayInput1_DSTATE_brq5z3xipc) {
    /* Switch: '<S12>/Switch' */
    rtb_Sum_be4knqil4l_idx_0 = (real_T)(*rtu_extd_cmd_tecs_cmd);
  } else {
    /* Switch: '<S12>/Switch' incorporates:
     *  Constant: '<S12>/pic_t '
     */
    rtb_Sum_be4knqil4l_idx_0 = 255.0;
  }

  /* End of Switch: '<S12>/Switch' */

  /* Switch: '<S13>/Switch' incorporates:
   *  RelationalOperator: '<S70>/FixPt Relational Operator'
   *  UnitDelay: '<S70>/Delay Input1'
   *
   * Block description for '<S70>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((*rtu_std_command_pic_cmd_cnt) >
      system_state_machine_DW.DelayInput1_DSTATE) {
    /* Switch: '<S13>/Switch' */
    system_state_machine_B.Switch_po1f1vqhhu = *rtu_std_command_pic_cmd;
  } else {
    /* Switch: '<S13>/Switch' incorporates:
     *  Constant: '<S13>/pic_t '
     */
    system_state_machine_B.Switch_po1f1vqhhu = PIC_INVALID;
  }

  /* End of Switch: '<S13>/Switch' */

  /* Sum: '<S15>/Add4' incorporates:
   *  Constant: '<S15>/Constant2'
   *  Math: '<S15>/Mod2'
   *  Math: '<S15>/Mod3'
   *  Sum: '<S15>/Add3'
   *  Sum: '<S15>/Sum'
   *  Sum: '<S1>/Sum'
   */
  system_state_machine_B.Add4 = rt_modd(rt_modd((((*rtu_sensor_chi) -
    rtu_sensor_eul_ang[2]) - -3.1415926535897931) +
    system_state_machine_ConstB.Sum1, system_state_machine_ConstB.Sum1),
    system_state_machine_ConstB.Sum1) - 3.1415926535897931;

  /* BusCreator generated from: '<S1>/fcs_state_machine_stateflow' */
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.cur_leg_heading
    = *rtu_WP_SMdata_in_cur_leg_heading;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.cur_leg_length
    = *rtu_WP_SMdata_in_cur_leg_length;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.cur_leg_remaining_dist
    = *rtu_WP_SMdata_in_cur_leg_remaining_dist;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.wp_list_valid
    = *rtu_WP_SMdata_in_wp_list_valid;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.last_wp_land
    = *rtu_WP_SMdata_in_last_wp_land;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.land_wp_lat
    = *rtu_WP_SMdata_in_land_wp_lat;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.land_wp_lon
    = *rtu_WP_SMdata_in_land_wp_lon;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.curpos_to_wp_heading
    = *rtu_WP_SMdata_in_curpos_to_wp_heading;
  system_state_machine_B.BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_.WPN_cmd_received
    = *rtu_WP_SMdata_in_WPN_cmd_received;

  /* Switch: '<S14>/Switch' incorporates:
   *  RelationalOperator: '<S71>/FixPt Relational Operator'
   *  UnitDelay: '<S71>/Delay Input1'
   *
   * Block description for '<S71>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((*rtu_std_command_vom_cmd_cnt) >
      system_state_machine_DW.DelayInput1_DSTATE_citpu4sg5i) {
    /* Switch: '<S14>/Switch' */
    system_state_machine_B.Switch_d533txexjk = *rtu_std_command_vom_cmd;
  } else {
    /* Switch: '<S14>/Switch' incorporates:
     *  Constant: '<S14>/vom_t '
     */
    system_state_machine_B.Switch_d533txexjk = VOM_INVALID;
  }

  /* End of Switch: '<S14>/Switch' */

  /* Chart: '<S1>/fcs_state_machine_stateflow' */
  if (system_state_machine_DW.temporalCounter_i1 < ((uint16_T)8191)) {
    system_state_machine_DW.temporalCounter_i1 = (uint16_T)((int32_T)(((int32_T)
      system_state_machine_DW.temporalCounter_i1) + 1));
  }

  if (system_state_machine_DW.temporalCounter_i2 < ((uint8_T)127)) {
    system_state_machine_DW.temporalCounter_i2 = (uint8_T)((int32_T)(((int32_T)
      system_state_machine_DW.temporalCounter_i2) + 1));
  }

  if (system_state_machine_DW.temporalCounter_i3 < ((uint8_T)127)) {
    system_state_machine_DW.temporalCounter_i3 = (uint8_T)((int32_T)(((int32_T)
      system_state_machine_DW.temporalCounter_i3) + 1));
  }

  /* Gateway: system_state_machine/fcs_state_machine_stateflow */
  /* During: system_state_machine/fcs_state_machine_stateflow */
  if (system_state_machine_DW.is_active_c4_system_state_machine == ((uint8_T)0))
  {
    /* Entry: system_state_machine/fcs_state_machine_stateflow */
    system_state_machine_DW.is_active_c4_system_state_machine = 1U;

    /* Entry Internal: system_state_machine/fcs_state_machine_stateflow */
    /* Transition: '<S8>:162' */
    /* Entry Internal 'FlightManagement': '<S8>:161' */
    /* Entry Internal 'FMM': '<S8>:118' */
    /* Transition: '<S8>:150' */
    system_state_machine_DW.is_FMM = system_state_machine_IN_READY;

    /* Entry 'READY': '<S8>:277' */
    /* '<S8>:277:3' vom_status=vom_t.VOM_READY; */
    *rty_vom_status = VOM_READY;

    /* '<S8>:277:4' dHoverTimer_sec=0; */
    /* '<S8>:277:5' bL2RTH_LinkLossFlag=0; */
    system_state_machine_DW.bL2RTH_LinkLossFlag = 0.0;

    /* Entry Internal 'InAirOnGroundLogic': '<S8>:202' */
    /* Transition: '<S8>:247' */
    system_state_machine_DW.is_InAirOnGroundLogic =
      system_state_machine_IN_OnGround;

    /* Entry 'OnGround': '<S8>:245' */
    /* '<S8>:245:2' InAir_mode=false; */
    *rty_bInAirFlag = false;

    /* Entry Internal 'ContingencyStatus': '<S8>:384' */
    /* Transition: '<S8>:594' */
    /* Entry Internal 'ContingencyStatusManagment': '<S8>:407' */
    /* Entry 'Telemtry_LinkLoss': '<S8>:408' */
    /* Entry Internal 'Telemtry_LinkLoss': '<S8>:408' */
    /* Transition: '<S8>:756' */
    system_state_machine_DW.is_Telemtry_LinkLoss =
      system_state_machine_IN_Tele_loss_false;

    /* Entry 'Tele_loss_false': '<S8>:758' */
    /* '<S8>:758:3' bTelemtryLinkLoss250ms=false; */
    /* '<S8>:758:4' bTelemtryLinkLoss5sec=false; */
    system_state_machine_DW.bTelemtryLinkLoss5sec = false;

    /* '<S8>:758:5' bTelemtryLinkLoss1sec=false; */
    *rty_sFlags_bTelemtryLinkLoss1sec = false;

    /* Entry 'ep_loss': '<S8>:734' */
    /* Entry Internal 'ep_loss': '<S8>:734' */
    /* Transition: '<S8>:761' */
    system_state_machine_DW.is_ep_loss = system_state_machine_IN_RC_loss_false;

    /* Entry 'RC_loss_false': '<S8>:770' */
    /* '<S8>:770:3' ep_loss250ms=false; */
    system_state_machine_DW.ep_loss250ms = false;

    /* '<S8>:770:4' ep_loss1sec=false; */
    *rty_sFlags_ep_loss1sec = false;

    /* '<S8>:770:5' ep_loss5sec=false; */
    system_state_machine_DW.ep_loss5sec = false;

    /* Entry Internal 'HoverTimer': '<S8>:595' */
    /* Transition: '<S8>:658' */
    system_state_machine_DW.is_HoverTimer = system_state_machine_IN_TimerInit;

    /* Entry 'TimerInit': '<S8>:657' */
    /* '<S8>:657:3' bHover5sec=false; */
    system_state_machine_DW.bHover5sec = false;

    /* '<S8>:657:4' TimerHover=0; */
    system_state_machine_DW.TimerHover = 0.0;

    /* Entry 'GPS_Loss': '<S8>:820' */
    /* '<S8>:820:3' bRecoverGPS=0; */
    system_state_machine_DW.bRecoverGPS = 0.0;

    /* Entry Internal 'GPS_Loss': '<S8>:820' */
    /* Transition: '<S8>:821' */
    system_state_machine_DW.is_GPS_Loss = system_state_machine_IN_GPS_loss_false;

    /* Entry 'GPS_loss_false': '<S8>:827' */
    /* '<S8>:827:3' bGPSLoss10sec=false; */
    *rty_sFlags_bGPSLoss10sec = false;

    /* '<S8>:827:4' bGPSLoss5sec=false */
    /* '<S8>:827:5' bGPSLoss2sec=false; */
    system_state_machine_DW.bGPSLoss2sec = false;

    /* '<S8>:827:6' bGPSLoss=false; */
    *rty_sFlags_bGPSLoss = false;

    /* '<S8>:827:7' timer_GPSLoss=0; */
    system_state_machine_DW.timer_GPSLoss = 0.0;

    /* Entry 'ip_loss': '<S8>:2305' */
    /* Entry Internal 'ip_loss': '<S8>:2305' */
    /* Transition: '<S8>:2311' */
    system_state_machine_DW.is_ip_loss = system_state_machine_IN_RC_loss_false;

    /* Entry 'RC_loss_false': '<S8>:2326' */
    /* '<S8>:2326:3' ip_loss250ms=false; */
    system_state_machine_DW.ip_loss250ms = false;

    /* '<S8>:2326:4' ip_loss1sec=false; */
    system_state_machine_DW.ip_loss1sec = false;

    /* '<S8>:2326:5' ip_loss5sec=false; */
    system_state_machine_DW.ip_loss5sec = false;

    /* Entry Internal 'EP_IP_Loss_5sec': '<S8>:2846' */
    /* Transition: '<S8>:2847' */
    system_state_machine_DW.is_EP_IP_Loss_5sec =
      system_state_machine_IN_TimerInit;

    /* Entry 'TimerInit': '<S8>:2854' */
    /* '<S8>:2854:3' bEPIPLossFlag=false; */
    system_state_machine_DW.bEPIPLossFlag = false;

    /* '<S8>:2854:4' TimerEPIPLoss=0; */
    system_state_machine_DW.TimerEPIPLoss = 0.0;

    /* Entry Internal 'Safety_SM': '<S8>:952' */
    /* Transition: '<S8>:953' */
    system_state_machine_DW.is_Safety_SM = system_state_machine_IN_DISARMED;

    /* Entry 'DISARMED': '<S8>:954' */
    /* '<S8>:954:2' safety_state=safety_state_t.AC_DISARMED; */
    *rty_safety_state = AC_DISARMED;

    /* '<S8>:954:3' safety_state_time=0; */
    system_state_machine_DW.safety_state_time = 0.0;

    /* Entry Internal 'Loiter_SM': '<S8>:1349' */
    /* Transition: '<S8>:1353' */
    system_state_machine_DW.is_Loiter_SM = system_state_machine_IN_Loiter_off;

    /* Entry 'Loiter_off': '<S8>:1352' */
    /* '<S8>:1352:3' loiter_mode = 0; */
    *rty_loiter_mode = 0U;

    /* Entry Internal 'TECS_SM': '<S8>:1359' */
    /* Transition: '<S8>:1361' */
    system_state_machine_DW.is_TECS_SM = system_state_machine_IN_TECS_off;

    /* Entry 'TECS_off': '<S8>:1360' */
    /* '<S8>:1360:3' TECS_mode = 0; */
    *rty_TECS_mode = 0U;

    /* '<S8>:1360:4' lifter_state=lifter_state_t.ON; */
    *rty_lifter_state = ON;

    /* Entry Internal 'PIC_SM': '<S8>:1411' */
    /* Transition: '<S8>:1414' */
    system_state_machine_DW.is_PIC_SM = system_state_machine_IN_PIC_EXT;

    /* Entry 'PIC_EXT': '<S8>:1412' */
    /* '<S8>:1412:3' pic_status = pic_t.EXTERNAL; */
    *rty_pic_status = EXTERNAL;

    /* Entry Internal 'CoG_SM': '<S8>:1420' */
    /* Transition: '<S8>:1421' */
    system_state_machine_DW.is_CoG_SM = system_state_machine_IN_CoG_tracking_off;

    /* Entry 'CoG_tracking_off': '<S8>:1423' */
    /* '<S8>:1423:3' CoG_tracking = 0; */
    *rty_CoG_tracking = 0U;
  } else {
    system_state_machine_fhrgqlbunr(&FixPtRelationalOperator,
      &FixPtRelationalOperator_cwrqbb5ufj, &rtb_Sum_evx323najj, &vom_status,
      &rtb_TrigonometricFunction2, &rtb_Sum_be4knqil4l_idx_0, rtu_sensor_eul_ang,
      rtu_sensor_pos_lla, rtu_sensor_aspd_cas, rtu_sensor_h_radar_agl,
      rtu_sensor_chi, rtu_pilot_input_switch_1, rtu_controllerAltCtrl_forceDes,
      rtu_wp_data_wp_list_valid, rty_bInAirFlag, rty_vom_status, rty_mode_data,
      rty_sFlags_bGPSLoss10sec, rty_sFlags_bGPSLoss, rty_sFlags_rampup_phase,
      rty_sFlags_bTelemtryLinkLoss1sec, rty_sFlags_ep_loss1sec, rty_safety_state,
      rty_loiter_mode, rty_TECS_mode, rty_lifter_state, rty_pic_status,
      rty_CoG_tracking, rty_h_rel_takeoff);
  }

  /* End of Chart: '<S1>/fcs_state_machine_stateflow' */

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   */
  *rty_SM_state_inAir_flag = (*rty_bInAirFlag) ? ((uint8_T)1) : ((uint8_T)0);

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   */
  *rty_SM_state_pic_status = (uint8_T)(*rty_pic_status);

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion6'
   */
  *rty_SM_state_lifter_mode = (uint8_T)(*rty_lifter_state);

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   */
  *rty_SM_state_vom_status = *rty_vom_status;

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   */
  *rty_SM_state_safety_state = *rty_safety_state;

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   */
  *rty_SM_state_loiter_mode = *rty_loiter_mode;

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   */
  *rty_SM_state_tecs_mode = *rty_TECS_mode;

  /* SignalConversion generated from: '<Root>/SM_state' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   */
  *rty_SM_state_CoG_tracking = *rty_CoG_tracking;

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  system_state_machine_DW.UnitDelay1_DSTATE_jsa3d2gsdq = *rty_vom_status;

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  system_state_machine_DW.UnitDelay_DSTATE = *rty_bInAirFlag;

  /* Update for UnitDelay: '<S2>/Unit Delay1' */
  system_state_machine_DW.UnitDelay1_DSTATE = rtb_Switch;

  /* Update for UnitDelay: '<S35>/Delay Input1'
   *
   * Block description for '<S35>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE_es42msgisa = rtb_Compare_d1opexicre;

  /* Update for Delay: '<S6>/Delay One Step1' */
  system_state_machine_DW.icLoad = false;
  system_state_machine_DW.DelayOneStep1_DSTATE = rtb_alt;

  /* Update for UnitDelay: '<S4>/Delay Input1'
   *
   * Block description for '<S4>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE_bgpgka0occ =
    rtb_FixPtRelationalOperator;

  /* Update for UnitDelay: '<S7>/Unit Delay2' incorporates:
   *  Constant: '<S7>/Constant2'
   *  Sum: '<S7>/Add'
   */
  system_state_machine_DW.UnitDelay2_DSTATE = rtb_Switch_pjqesw53bu + 1.0;

  /* Update for UnitDelay: '<S38>/Delay Input1'
   *
   * Block description for '<S38>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE_hdp33oolxt = rtb_Compare_peqkw15qn5;

  /* Update for UnitDelay: '<S68>/Delay Input1'
   *
   * Block description for '<S68>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE_lufocpaa5h =
    *rtu_extd_cmd_loiter_cmd_cnt;

  /* Update for UnitDelay: '<S69>/Delay Input1'
   *
   * Block description for '<S69>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE_brq5z3xipc =
    *rtu_extd_cmd_tecs_cmd_cnt;

  /* Update for UnitDelay: '<S70>/Delay Input1'
   *
   * Block description for '<S70>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE = *rtu_std_command_pic_cmd_cnt;

  /* Update for UnitDelay: '<S71>/Delay Input1'
   *
   * Block description for '<S71>/Delay Input1':
   *
   *  Store in Global RAM
   */
  system_state_machine_DW.DelayInput1_DSTATE_citpu4sg5i =
    *rtu_std_command_vom_cmd_cnt;

  /* End of Outputs for SubSystem: '<Root>/system_state_machine' */
}

/* Model initialize function */
void system_state_machine_initialize(void)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) &system_state_machine_B), 0,
                sizeof(system_state_machine_TB));

  {
    system_state_machine_B.Switch_d533txexjk = VOM_INVALID;
    system_state_machine_B.Switch_po1f1vqhhu = EXTERNAL;
  }

  /* states (dwork) */
  (void) memset((void *)&system_state_machine_DW, 0,
                sizeof(system_state_machine_TDW));
  system_state_machine_PrevZCX.DelayOneStep1_Reset_ZCE = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
