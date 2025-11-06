/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_TECS_switcher.c
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

#include "FW_TECS_switcher.h"
#include "rtwtypes.h"
#include "FW_TECS_switcher_types.h"
#include "FW_TECS_switcher_private.h"
#include <math.h>
#include "rt_modd.h"
#include "zero_crossing_types.h"
#include <string.h>

/* Block states (default storage) */
FW_TECS_switcher_TDW FW_TECS_switcher_DW;

/* Previous zero-crossings (trigger) states */
FW_TECS_switcher_TZCE FW_TECS_switcher_PrevZCX;

/*
 * Output and update for atomic system:
 *    '<S13>/MATLAB Function1'
 *    '<S31>/MATLAB Function1'
 *    '<S47>/MATLAB Function1'
 *    '<S81>/MATLAB Function1'
 *    '<S119>/MATLAB Function1'
 *    '<S127>/MATLAB Function1'
 */
void FW_TECS_switcher_illvnqr3hx(const real_T rtu_LLA[3], real_T rty_C[9])
{
  real_T cos_lat;
  real_T cos_long;
  real_T sin_lat;
  real_T sin_long;

  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S16>:1' */
  /* '<S16>:1:6' cos_lat = cos(LLA(1)); */
  cos_lat = cos(rtu_LLA[0]);

  /* '<S16>:1:7' sin_lat = sin(LLA(1)); */
  sin_lat = sin(rtu_LLA[0]);

  /* '<S16>:1:8' cos_long = cos(LLA(2)); */
  cos_long = cos(rtu_LLA[1]);

  /* '<S16>:1:9' sin_long = sin(LLA(2)); */
  sin_long = sin(rtu_LLA[1]);

  /* '<S16>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S16>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S16>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  rty_C[0] = (-sin_lat) * cos_long;
  rty_C[3] = (-sin_lat) * sin_long;
  rty_C[6] = cos_lat;
  rty_C[1] = -sin_long;
  rty_C[4] = cos_long;
  rty_C[7] = 0.0;
  rty_C[2] = (-cos_lat) * cos_long;
  rty_C[5] = (-cos_lat) * sin_long;
  rty_C[8] = -sin_lat;
}

/* System initialize for referenced model: 'FW_TECS_switcher' */
void FW_TECS_switcher_Init(void)
{
  /* InitializeConditions for Delay: '<S106>/cur_pos_sp' */
  FW_TECS_switcher_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S106>/prev_pos_sp' */
  FW_TECS_switcher_DW.icLoad_jveizoqakw = true;

  /* InitializeConditions for Delay: '<S8>/Delay1' */
  FW_TECS_switcher_DW.icLoad_ncnilzog2z = true;

  /* InitializeConditions for Delay: '<S8>/Delay2' */
  FW_TECS_switcher_DW.icLoad_fj3gi4q45w = true;

  /* InitializeConditions for Delay: '<S23>/Delay1' */
  FW_TECS_switcher_DW.icLoad_ogna325jjd = true;

  /* InitializeConditions for Delay: '<S22>/Delay' */
  FW_TECS_switcher_DW.icLoad_cnbza4ewej = true;

  /* InitializeConditions for Delay: '<S90>/Delay1' */
  FW_TECS_switcher_DW.icLoad_kbyaompepr = true;

  /* InitializeConditions for Delay: '<S106>/cur_idx' */
  FW_TECS_switcher_DW.icLoad_hfgm0d2zgp = true;
}

/* Output and update for referenced model: 'FW_TECS_switcher' */
void FW_TECS_switcher(const vom_t *rtu_vom_status, const real_T
                      *rtu_Pilot_roll_ch, const real_T *rtu_Pilot_throttle_ch,
                      const real_T rtu_Sensor_eul_ang[3], const real_T
                      rtu_Sensor_pos_lla[3], const real_T rtu_Sensor_vel_ned[3],
                      const real_T *rtu_Sensor_aspd_cas, const real_T
                      *rtu_Sensor_gspd, const real_T *rtu_Sensor_chi, const
                      uint8_T *rtu_TECS_mode, const real_T
                      *rtu_mode_data_ft_data_FT_x, const real_T
                      *rtu_mode_data_ft_data_FT_y, const real_T
                      *rtu_mode_data_ft_data_FT_Altitude, const real_T
                      *rtu_mode_data_ft_data_FT_Heading, const real_T
                      *rtu_mode_data_ft_data_FT_AirspeedRef, const real_T
                      *rtu_mode_data_loiter_data_loiter_radius, const real_T
                      *rtu_mode_data_loiter_data_loiter_direction, const real_T *
                      rtu_mode_data_loiter_data_loiter_Center_Lat, const real_T *
                      rtu_mode_data_loiter_data_loiter_Center_Lon, const real_T *
                      rtu_mode_data_loiter_data_loiter_altitude, const real_T
                      *rtu_mode_data_loiter_data_loiter_AirSpeedRef, const
                      real_T *rtu_mode_data_bt_data_BT_Hover_Lat, const real_T
                      *rtu_mode_data_bt_data_BT_Hover_Lon, const real_T
                      *rtu_mode_data_bt_data_BT_Altitude, const real_T
                      *rtu_mode_data_bt_data_BT_Heading, const lifter_state_t
                      *rtu_mode_data_eFWLifter_Mode, const eFWRTH_phase
                      *rtu_mode_data_fwrth_data_phase, const real_T
                      *rtu_mode_data_fwrth_data_CAS_sp, const real_T
                      *rtu_mode_data_fwrth_data_alt_sp, const boolean_T
                      *rtu_wp_data_wp_list_valid, const uint16_T
                      *rtu_wp_data_wp_list_count, const real_T
                      *rtu_wp_data_cur_wp_lat, const real_T
                      *rtu_wp_data_cur_wp_lon, const real_T
                      *rtu_wp_data_cur_wp_alt, const uint16_T
                      *rtu_wp_data_cmd_wp_idx, const boolean_T
                      *rtu_wp_data_last_wp_land, const vom_t
                      *rtu_std_command_vom_cmd, const real_T
                      *rtu_std_command_airspeed_cas_cmd, const real_T
                      *rtu_std_command_fwrth_apr_deg, const boolean_T
                      *rtu_bGPSLossFlag, const boolean_T
                      *rtu_bTelemtryLinkLoss1sec, busFW_TECS_switcher
                      *rty_busFW_TECS_switcher, uint16_T *rty_wp_req_idx,
                      boolean_T *rty_FWRTH_SM_in_approach_circle_done, boolean_T
                      *rty_FWRTH_SM_in_reduce_speed_alt_done, boolean_T
                      *rty_FWRTH_SM_in_circle_align_done, boolean_T
                      *rty_FWRTH_SM_in_cross_tangent_point_done, real_T
                      *rty_FWRTH_SM_in_land_lat, real_T
                      *rty_FWRTH_SM_in_land_lon, real_T
                      *rty_FWRTH_SM_in_approach_ang, real_T
                      *rty_WP_SMdata_cur_leg_heading, real_T
                      *rty_WP_SMdata_cur_leg_length, real_T
                      *rty_WP_SMdata_cur_leg_remaining_dist, boolean_T
                      *rty_WP_SMdata_wp_list_valid, boolean_T
                      *rty_WP_SMdata_last_wp_land, real_T
                      *rty_WP_SMdata_land_wp_lat, real_T
                      *rty_WP_SMdata_land_wp_lon, real_T
                      *rty_WP_SMdata_curpos_to_wp_heading, boolean_T
                      *rty_WP_SMdata_WPN_cmd_received)
{
  real_T rtb_C_pex2miccv0[9];
  real_T rtb_VectorConcatenate[3];
  real_T rtb_VectorConcatenate1[3];
  real_T AC_idx_0;
  real_T AC_idx_0_0;
  real_T AC_idx_1;
  real_T AC_idx_1_0;
  real_T AC_norm;
  real_T AC_norm_tmp;
  real_T C_idx_0;
  real_T PB_norm;
  real_T Vg;
  real_T Vg_0;
  real_T absxk;
  real_T curvature;
  real_T dir_idx_0;
  real_T dir_idx_1;
  real_T rtb_Product1_bkygj4a2su;
  real_T rtb_Sqrt;
  real_T rtb_Sqrt_jug5wuieqd;
  real_T rtb_Sum_imgtnbczew_idx_1;
  real_T rtb_Sum_imgtnbczew_tmp;
  real_T rtb_Sum_imgtnbczew_tmp_0;
  real_T rtb_Sum_lskvbzjz0i;
  real_T rtb_TrigonometricFunction2_h0qciwy3lv_tmp;
  real_T rtb_TrigonometricFunction2_i3ecl5ucim;
  real_T rtb_TrigonometricFunction6_onwcg4eh52;
  real_T rtb_TrigonometricFunction_paar0fmbpe;
  real_T rtb_VectorConcatenate1_oigmvxpsrq_idx_0;
  real_T rtb_VectorConcatenate1_oigmvxpsrq_idx_1;
  real_T rtb_VectorConcatenate1_psilb5rrki_idx_0;
  real_T rtb_VectorConcatenate1_psilb5rrki_idx_1;
  real_T scale;
  real_T t;
  real_T y;
  int32_T i;
  boolean_T rtb_Compare_ag4sp0vjdy;
  boolean_T rtb_Compare_i5bisll1rb;
  boolean_T rtb_Delay3;
  boolean_T rtb_Equal_bb4ze044et;
  boolean_T rtb_FixPtRelationalOperator_fishi4l242_tmp;
  boolean_T rtb_FixPtRelationalOperator_h1ymkdiv2b;
  boolean_T rtb_FixPtRelationalOperator_lyd2dqurri;
  boolean_T rtb_Memory_mouojdlutb;
  boolean_T rtb_OR;

  /* SignalConversion generated from: '<S2>/Vector Concatenate1' */
  rtb_VectorConcatenate1[0] = *rtu_mode_data_bt_data_BT_Hover_Lat;

  /* SignalConversion generated from: '<S2>/Vector Concatenate1' */
  rtb_VectorConcatenate1[1] = *rtu_mode_data_bt_data_BT_Hover_Lon;

  /* SignalConversion generated from: '<S2>/Vector Concatenate1' */
  rtb_VectorConcatenate1[2] = rtu_Sensor_pos_lla[2];

  /* MATLAB Function: '<S13>/MATLAB Function1' */
  FW_TECS_switcher_illvnqr3hx(rtb_VectorConcatenate1, rtb_C_pex2miccv0);

  /* Trigonometry: '<S15>/Trigonometric Function' */
  rtb_TrigonometricFunction2_i3ecl5ucim = sin
    (*rtu_mode_data_bt_data_BT_Hover_Lat);

  /* Product: '<S19>/Product' incorporates:
   *  Constant: '<S19>/e'
   */
  rtb_Sqrt = rtb_TrigonometricFunction2_i3ecl5ucim * 0.0818191908425;

  /* Sqrt: '<S19>/Sqrt' incorporates:
   *  Constant: '<S19>/const'
   *  Math: '<S19>/Square'
   *  Sum: '<S19>/Add'
   */
  rtb_Sqrt = sqrt(1.0 - (rtb_Sqrt * rtb_Sqrt));

  /* Switch: '<S20>/Switch3' incorporates:
   *  Abs: '<S20>/Abs'
   *  Product: '<S20>/Product'
   */
  if (rtb_Sqrt <= 0.001) {
    rtb_Sqrt = -0.001;
  }

  /* End of Switch: '<S20>/Switch3' */

  /* Product: '<S19>/Divide' incorporates:
   *  Constant: '<S19>/R_0'
   */
  rtb_Sqrt = 6.378137E+6 / rtb_Sqrt;

  /* Product: '<S15>/Product' incorporates:
   *  Product: '<S15>/Product1'
   *  SignalConversion generated from: '<S2>/Vector Concatenate1'
   *  Sum: '<S15>/Sum'
   *  Trigonometry: '<S15>/Trigonometric Function1'
   */
  rtb_TrigonometricFunction6_onwcg4eh52 = (rtb_Sqrt + rtu_Sensor_pos_lla[2]) *
    cos(*rtu_mode_data_bt_data_BT_Hover_Lat);

  /* Trigonometry: '<S14>/Trigonometric Function' incorporates:
   *  MATLAB Function: '<S58>/MATLAB Function1'
   *  Trigonometry: '<S120>/Trigonometric Function'
   *  Trigonometry: '<S136>/Trigonometric Function'
   *  Trigonometry: '<S32>/Trigonometric Function'
   *  Trigonometry: '<S48>/Trigonometric Function'
   *  Trigonometry: '<S60>/Trigonometric Function'
   *  Trigonometry: '<S68>/Trigonometric Function3'
   *  Trigonometry: '<S82>/Trigonometric Function'
   *  Trigonometry: '<S94>/Trigonometric Function'
   */
  curvature = sin(rtu_Sensor_pos_lla[0]);

  /* Product: '<S17>/Product' incorporates:
   *  Constant: '<S17>/e'
   *  Product: '<S123>/Product'
   *  Product: '<S139>/Product'
   *  Product: '<S35>/Product'
   *  Product: '<S51>/Product'
   *  Product: '<S64>/Product'
   *  Product: '<S85>/Product'
   *  Product: '<S97>/Product'
   *  Trigonometry: '<S14>/Trigonometric Function'
   */
  AC_norm_tmp = curvature * 0.0818191908425;

  /* Sqrt: '<S17>/Sqrt' incorporates:
   *  Constant: '<S17>/const'
   *  Math: '<S17>/Square'
   *  Product: '<S17>/Product'
   *  Sqrt: '<S123>/Sqrt'
   *  Sqrt: '<S139>/Sqrt'
   *  Sqrt: '<S35>/Sqrt'
   *  Sqrt: '<S51>/Sqrt'
   *  Sqrt: '<S64>/Sqrt'
   *  Sqrt: '<S85>/Sqrt'
   *  Sqrt: '<S97>/Sqrt'
   *  Sum: '<S17>/Add'
   */
  AC_norm_tmp = sqrt(1.0 - (AC_norm_tmp * AC_norm_tmp));
  AC_norm = AC_norm_tmp;

  /* Switch: '<S18>/Switch3' incorporates:
   *  Product: '<S18>/Product'
   *  Sqrt: '<S17>/Sqrt'
   */
  if (AC_norm_tmp <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S18>/Switch3' */

  /* Product: '<S17>/Divide' incorporates:
   *  Constant: '<S17>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* Trigonometry: '<S14>/Trigonometric Function1' incorporates:
   *  MATLAB Function: '<S58>/MATLAB Function1'
   *  Trigonometry: '<S120>/Trigonometric Function1'
   *  Trigonometry: '<S136>/Trigonometric Function1'
   *  Trigonometry: '<S32>/Trigonometric Function1'
   *  Trigonometry: '<S48>/Trigonometric Function1'
   *  Trigonometry: '<S60>/Trigonometric Function1'
   *  Trigonometry: '<S68>/Trigonometric Function'
   *  Trigonometry: '<S82>/Trigonometric Function1'
   *  Trigonometry: '<S94>/Trigonometric Function1'
   */
  rtb_TrigonometricFunction2_h0qciwy3lv_tmp = cos(rtu_Sensor_pos_lla[0]);

  /* Trigonometry: '<S14>/Trigonometric Function3' incorporates:
   *  MATLAB Function: '<S58>/MATLAB Function1'
   *  Trigonometry: '<S120>/Trigonometric Function3'
   *  Trigonometry: '<S136>/Trigonometric Function3'
   *  Trigonometry: '<S32>/Trigonometric Function3'
   *  Trigonometry: '<S48>/Trigonometric Function3'
   *  Trigonometry: '<S60>/Trigonometric Function3'
   *  Trigonometry: '<S82>/Trigonometric Function3'
   *  Trigonometry: '<S94>/Trigonometric Function3'
   */
  rtb_Sum_imgtnbczew_tmp = cos(rtu_Sensor_pos_lla[1]);

  /* Trigonometry: '<S14>/Trigonometric Function2' incorporates:
   *  MATLAB Function: '<S58>/MATLAB Function1'
   *  Trigonometry: '<S120>/Trigonometric Function2'
   *  Trigonometry: '<S136>/Trigonometric Function2'
   *  Trigonometry: '<S32>/Trigonometric Function2'
   *  Trigonometry: '<S48>/Trigonometric Function2'
   *  Trigonometry: '<S60>/Trigonometric Function2'
   *  Trigonometry: '<S82>/Trigonometric Function2'
   *  Trigonometry: '<S94>/Trigonometric Function2'
   */
  rtb_Sum_imgtnbczew_tmp_0 = sin(rtu_Sensor_pos_lla[1]);

  /* Product: '<S14>/Product' incorporates:
   *  Product: '<S14>/Product1'
   *  Sum: '<S14>/Sum'
   *  Trigonometry: '<S14>/Trigonometric Function1'
   */
  dir_idx_1 = (AC_norm + rtu_Sensor_pos_lla[2]) *
    rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S11>/Sum' incorporates:
   *  Constant: '<S14>/1-e^2'
   *  Constant: '<S15>/1-e^2'
   *  Product: '<S14>/Product'
   *  Product: '<S14>/Product1'
   *  Product: '<S14>/Product2'
   *  Product: '<S14>/Product3'
   *  Product: '<S15>/Product'
   *  Product: '<S15>/Product1'
   *  Product: '<S15>/Product2'
   *  Product: '<S15>/Product3'
   *  SignalConversion generated from: '<S2>/Vector Concatenate1'
   *  Sum: '<S14>/Sum1'
   *  Sum: '<S15>/Sum1'
   *  Trigonometry: '<S14>/Trigonometric Function'
   *  Trigonometry: '<S14>/Trigonometric Function2'
   *  Trigonometry: '<S14>/Trigonometric Function3'
   *  Trigonometry: '<S15>/Trigonometric Function2'
   *  Trigonometry: '<S15>/Trigonometric Function3'
   */
  Vg = (dir_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    (rtb_TrigonometricFunction6_onwcg4eh52 * cos
     (*rtu_mode_data_bt_data_BT_Hover_Lon));
  rtb_Sum_imgtnbczew_idx_1 = (dir_idx_1 * rtb_Sum_imgtnbczew_tmp_0) -
    (rtb_TrigonometricFunction6_onwcg4eh52 * sin
     (*rtu_mode_data_bt_data_BT_Hover_Lon));
  dir_idx_0 = (curvature * ((0.993305620009879 * AC_norm) + rtu_Sensor_pos_lla[2]))
    - (rtb_TrigonometricFunction2_i3ecl5ucim * ((0.993305620009879 * rtb_Sqrt) +
        rtu_Sensor_pos_lla[2]));

  /* Product: '<S13>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S13>/Product' */
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] * Vg) +
      (rtb_C_pex2miccv0[i + 3] * rtb_Sum_imgtnbczew_idx_1)) +
      (rtb_C_pex2miccv0[i + 6] * dir_idx_0);
  }

  /* End of Product: '<S13>/Product' */

  /* MATLAB Function: '<S10>/guidance_line' incorporates:
   *  MATLAB Function: '<S105>/guidance_circle'
   *  MATLAB Function: '<S28>/guidance_line'
   *  MATLAB Function: '<S39>/MATLAB Function'
   *  MATLAB Function: '<S78>/MATLAB Function1'
   *  MATLAB Function: '<S89>/MATLAB Function'
   *  MATLAB Function: '<S8>/guidance_line1'
   *  SignalConversion generated from: '<S10>/Vector Concatenate1'
   * */
  /* MATLAB Function 'BTransition/L1_guidance/guidance_line': '<S12>:1' */
  /*  straight line L1-guidance */
  /*  start_pos         [x; y] (m) */
  /*  start_dir_ang     direction from North (rad) */
  /*  cur_pos           [x; y] (m) */
  /*  Vg                [vx, vy] ground-speed (m/s) */
  /*  lat_ax            (m/s/s) lateral acceleration command */
  /* '<S12>:1:11' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  rtb_TrigonometricFunction2_i3ecl5ucim = sqrt((rtu_Sensor_vel_ned[0] *
    rtu_Sensor_vel_ned[0]) + (rtu_Sensor_vel_ned[1] * rtu_Sensor_vel_ned[1]));
  Vg = rtb_TrigonometricFunction2_i3ecl5ucim;

  /*  horizontal ground speed */
  /* '<S12>:1:12' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S12>:1:13' Vg =0.001; */
    Vg = 0.001;
  }

  /* '<S12>:1:16' vel_dir = vel_NE/Vg; */
  /* '<S12>:1:18' L1_P = 15; */
  /*  L1 peroid (s) */
  /* '<S12>:1:19' L1_zeta = 0.7; */
  /*  L1 damping ratio */
  /* '<S12>:1:20' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Sqrt = 3.3422538049298023 * Vg;

  /*  L1 length */
  /* '<S12>:1:22' P = start_pos; */
  /* '<S12>:1:23' dir = [cos(start_dir_ang); sin(start_dir_ang)]; */
  dir_idx_0 = cos(*rtu_mode_data_bt_data_BT_Heading);
  dir_idx_1 = sin(*rtu_mode_data_bt_data_BT_Heading);

  /* '<S12>:1:24' A = cur_pos; */
  /* '<S12>:1:26' PA = A - P; */
  /* '<S12>:1:27' C = P + dir*dot(PA, dir); */
  rtb_VectorConcatenate1_oigmvxpsrq_idx_0 = rtu_Sensor_vel_ned[0] / Vg;
  rtb_VectorConcatenate1_oigmvxpsrq_idx_1 = rtu_Sensor_vel_ned[1] / Vg;
  AC_norm = (rtb_VectorConcatenate1[0] * dir_idx_0) + (rtb_VectorConcatenate1[1]
    * dir_idx_1);

  /* '<S12>:1:28' AC = C - A; */
  absxk = dir_idx_0 * AC_norm;
  C_idx_0 = absxk;
  AC_idx_0 = absxk - rtb_VectorConcatenate1[0];
  absxk = dir_idx_1 * AC_norm;
  AC_idx_1 = absxk - rtb_VectorConcatenate1[1];

  /* '<S12>:1:29' AC_norm = sqrt(AC(1)^2 + AC(2)^2); */
  AC_norm = sqrt((AC_idx_0 * AC_idx_0) + (AC_idx_1 * AC_idx_1));

  /* '<S12>:1:31' if(AC_norm < L1) */
  if (AC_norm < rtb_Sqrt) {
    /* '<S12>:1:32' CB_norm = sqrt(L1^2 - AC_norm^2); */
    AC_norm = sqrt((rtb_Sqrt * rtb_Sqrt) - (AC_norm * AC_norm));
  } else {
    /* '<S12>:1:33' else */
    /* '<S12>:1:34' CB_norm = 0; */
    AC_norm = 0.0;
  }

  /* '<S12>:1:37' B = C + dir*(CB_norm); */
  /* '<S12>:1:38' AB_dir = (B-A)/norm(B-A); */
  AC_idx_0 = (C_idx_0 + (dir_idx_0 * AC_norm)) - rtb_VectorConcatenate1[0];
  AC_idx_1 = (absxk + (dir_idx_1 * AC_norm)) - rtb_VectorConcatenate1[1];
  scale = 3.3121686421112381E-170;
  absxk = fabs(AC_idx_0);
  if (absxk > 3.3121686421112381E-170) {
    AC_norm = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    AC_norm = t * t;
  }

  absxk = fabs(AC_idx_1);
  if (absxk > scale) {
    t = scale / absxk;
    AC_norm = ((AC_norm * t) * t) + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    AC_norm += t * t;
  }

  AC_norm = scale * sqrt(AC_norm);
  AC_idx_0 /= AC_norm;
  AC_idx_1 /= AC_norm;

  /* Switch: '<S2>/Switch2' */
  /* '<S12>:1:40' sin_eta = vel_dir(1)*AB_dir(2) - vel_dir(2)*AB_dir(1); */
  /*  2D cross product */
  /* '<S12>:1:41' cos_eta = vel_dir(1)*AB_dir(1) + vel_dir(2)*AB_dir(2); */
  /*  2D dot product */
  /* '<S12>:1:42' eta = atan2(sin_eta, cos_eta); */
  /* '<S12>:1:43' eta = max(eta, -pi/2); */
  /* '<S12>:1:44' eta = min(eta,  pi/2); */
  /* '<S12>:1:46' path_done = false; */
  /* '<S12>:1:47' PB = B - P; */
  /* '<S12>:1:48' PB_norm = sqrt(PB(1)^2 + PB(2)^2); */
  /* '<S12>:1:50' if sign(dot(PB,dir))*PB_norm > path_len */
  /* '<S12>:1:54' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  if (*rtu_bGPSLossFlag) {
    /* BusAssignment: '<S2>/Bus Assignment2' incorporates:
     *  Constant: '<S2>/Constant4'
     */
    y = 0.0;
  } else {
    /* MATLAB Function: '<S10>/guidance_line' */
    y = atan2((rtb_VectorConcatenate1_oigmvxpsrq_idx_0 * AC_idx_1) - (AC_idx_0 *
               rtb_VectorConcatenate1_oigmvxpsrq_idx_1),
              (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 * AC_idx_0) +
              (rtb_VectorConcatenate1_oigmvxpsrq_idx_1 * AC_idx_1));
    if (y < -1.5707963267948966) {
      y = -1.5707963267948966;
    }

    if (y > 1.5707963267948966) {
      y = 1.5707963267948966;
    }

    /* Trigonometry: '<S10>/Atan' incorporates:
     *  MATLAB Function: '<S10>/guidance_line'
     */
    y = atan2(((1.9599999999999997 * (Vg * Vg)) / rtb_Sqrt) * sin(y), 9.81);

    /* Saturate: '<S2>/Saturation2' */
    if (y > 0.5236) {
      /* BusAssignment: '<S2>/Bus Assignment2' */
      y = 0.5236;
    } else if (y < -0.5236) {
      /* BusAssignment: '<S2>/Bus Assignment2' */
      y = -0.5236;
    } else {
      /* no actions */
    }

    /* End of Saturate: '<S2>/Saturation2' */
  }

  /* End of Switch: '<S2>/Switch2' */

  /* Sqrt: '<S64>/Sqrt' */
  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S61>:1' */
  /* '<S61>:1:6' cos_lat = cos(LLA(1)); */
  /* '<S61>:1:7' sin_lat = sin(LLA(1)); */
  /* '<S61>:1:8' cos_long = cos(LLA(2)); */
  /* '<S61>:1:9' sin_long = sin(LLA(2)); */
  /* '<S61>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S61>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S61>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  AC_norm = AC_norm_tmp;

  /* Switch: '<S65>/Switch3' incorporates:
   *  Product: '<S65>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S65>/Switch3' */

  /* Product: '<S64>/Divide' incorporates:
   *  Constant: '<S64>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* Product: '<S60>/Product' incorporates:
   *  Product: '<S60>/Product1'
   *  Sum: '<S60>/Sum'
   */
  dir_idx_1 = (AC_norm + rtu_Sensor_pos_lla[2]) *
    rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Product: '<S60>/Product3' incorporates:
   *  Constant: '<S60>/1-e^2'
   *  Product: '<S60>/Product2'
   *  Sum: '<S60>/Sum1'
   */
  dir_idx_0 = curvature * ((0.993305620009879 * AC_norm) + rtu_Sensor_pos_lla[2]);

  /* Gain: '<S5>/Gain' */
  rtb_Product1_bkygj4a2su = 0.017453292519943295 *
    (*rtu_std_command_fwrth_apr_deg);

  /* Sum: '<S76>/Add4' incorporates:
   *  Constant: '<S44>/Constant'
   *  Constant: '<S76>/Constant2'
   *  Constant: '<S77>/Constant2'
   *  Math: '<S76>/Mod2'
   *  Math: '<S76>/Mod3'
   *  Math: '<S77>/Mod2'
   *  Math: '<S77>/Mod3'
   *  Sum: '<S44>/Sum'
   *  Sum: '<S44>/Sum1'
   *  Sum: '<S76>/Add3'
   *  Sum: '<S76>/Sum'
   *  Sum: '<S77>/Add3'
   *  Sum: '<S77>/Add4'
   *  Sum: '<S77>/Sum'
   */
  AC_norm = rt_modd(rt_modd(((FW_TECS_switcher_ConstB.Atan2 + (rt_modd(rt_modd
    (((rtb_Product1_bkygj4a2su + 3.1415926535897931) - -3.1415926535897931) +
     FW_TECS_switcher_ConstB.Sum1_o5uamo4z5t,
     FW_TECS_switcher_ConstB.Sum1_o5uamo4z5t),
    FW_TECS_switcher_ConstB.Sum1_o5uamo4z5t) - 3.1415926535897931)) -
    -3.1415926535897931) + FW_TECS_switcher_ConstB.Sum1_j0ejptrk3o,
    FW_TECS_switcher_ConstB.Sum1_j0ejptrk3o),
                    FW_TECS_switcher_ConstB.Sum1_j0ejptrk3o) -
    3.1415926535897931;

  /* Sum: '<S75>/Add' incorporates:
   *  Product: '<S75>/Product1'
   *  Trigonometry: '<S75>/Trigonometric Function5'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 = FW_TECS_switcher_ConstB.Product +
    ((FW_TECS_switcher_ConstB.TrigonometricFunction3 *
      FW_TECS_switcher_ConstB.TrigonometricFunction4) * cos(AC_norm));

  /* Saturate: '<S75>/Saturation' */
  if (rtb_VectorConcatenate1_psilb5rrki_idx_0 > 1.0) {
    rtb_VectorConcatenate1_psilb5rrki_idx_0 = 1.0;
  } else if (rtb_VectorConcatenate1_psilb5rrki_idx_0 < -1.0) {
    rtb_VectorConcatenate1_psilb5rrki_idx_0 = -1.0;
  } else {
    /* no actions */
  }

  /* Trigonometry: '<S75>/Trigonometric Function' incorporates:
   *  Saturate: '<S75>/Saturation'
   */
  scale = asin(rtb_VectorConcatenate1_psilb5rrki_idx_0);

  /* SignalConversion generated from: '<S44>/Vector Concatenate1' */
  rtb_VectorConcatenate1[0] = scale;

  /* Trigonometry: '<S75>/Trigonometric Function8' incorporates:
   *  Trigonometry: '<S49>/Trigonometric Function'
   *  Trigonometry: '<S59>/Trigonometric Function'
   *  Trigonometry: '<S68>/Trigonometric Function4'
   */
  Vg = sin(scale);

  /* Trigonometry: '<S75>/Trigonometric Function6' incorporates:
   *  Product: '<S75>/Product2'
   *  Product: '<S75>/Product3'
   *  Sum: '<S75>/Add2'
   *  Trigonometry: '<S75>/Trigonometric Function7'
   *  Trigonometry: '<S75>/Trigonometric Function8'
   */
  rtb_VectorConcatenate1_oigmvxpsrq_idx_0 = atan2((sin(AC_norm) *
    FW_TECS_switcher_ConstB.TrigonometricFunction4) *
    FW_TECS_switcher_ConstB.TrigonometricFunction3,
    FW_TECS_switcher_ConstB.TrigonometricFunction2 -
    (FW_TECS_switcher_ConstB.TrigonometricFunction1 * Vg));

  /* Sum: '<S75>/Add1' incorporates:
   *  Constant: '<S5>/ADSAC_RTH_land'
   */
  rtb_VectorConcatenate1[1] = rtb_VectorConcatenate1_oigmvxpsrq_idx_0 +
    0.95705178402328117;

  /* SignalConversion generated from: '<S44>/Vector Concatenate1' */
  rtb_VectorConcatenate1[2] = *rtu_mode_data_fwrth_data_alt_sp;

  /* Product: '<S62>/Product' incorporates:
   *  Constant: '<S62>/e'
   *  Product: '<S53>/Product'
   */
  rtb_Sqrt = Vg * 0.0818191908425;

  /* Sqrt: '<S62>/Sqrt' incorporates:
   *  Constant: '<S62>/const'
   *  Math: '<S62>/Square'
   *  Product: '<S62>/Product'
   *  Sqrt: '<S53>/Sqrt'
   *  Sum: '<S62>/Add'
   */
  rtb_Sqrt = sqrt(1.0 - (rtb_Sqrt * rtb_Sqrt));
  scale = rtb_Sqrt;

  /* Switch: '<S63>/Switch3' incorporates:
   *  Product: '<S63>/Product'
   *  Sqrt: '<S62>/Sqrt'
   */
  if (rtb_Sqrt <= 0.001) {
    scale = -0.001;
  }

  /* End of Switch: '<S63>/Switch3' */

  /* Product: '<S62>/Divide' incorporates:
   *  Constant: '<S62>/R_0'
   */
  scale = 6.378137E+6 / scale;

  /* Trigonometry: '<S59>/Trigonometric Function1' incorporates:
   *  Trigonometry: '<S49>/Trigonometric Function1'
   *  Trigonometry: '<S68>/Trigonometric Function2'
   */
  rtb_Sum_imgtnbczew_idx_1 = cos(rtb_VectorConcatenate1[0]);

  /* Trigonometry: '<S59>/Trigonometric Function3' incorporates:
   *  Constant: '<S5>/ADSAC_RTH_land'
   *  Sum: '<S75>/Add1'
   *  Trigonometry: '<S49>/Trigonometric Function3'
   *  Trigonometry: '<S75>/Trigonometric Function6'
   */
  rtb_TrigonometricFunction6_onwcg4eh52 = cos
    (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 + 0.95705178402328117);

  /* Trigonometry: '<S59>/Trigonometric Function2' incorporates:
   *  Constant: '<S5>/ADSAC_RTH_land'
   *  Sum: '<S75>/Add1'
   *  Trigonometry: '<S49>/Trigonometric Function2'
   *  Trigonometry: '<S75>/Trigonometric Function6'
   */
  Vg_0 = sin(rtb_VectorConcatenate1_oigmvxpsrq_idx_0 + 0.95705178402328117);

  /* MATLAB Function: '<S58>/MATLAB Function1' */
  rtb_C_pex2miccv0[0] = (-curvature) * rtb_Sum_imgtnbczew_tmp;
  rtb_C_pex2miccv0[3] = (-curvature) * rtb_Sum_imgtnbczew_tmp_0;
  rtb_C_pex2miccv0[6] = rtb_TrigonometricFunction2_h0qciwy3lv_tmp;
  rtb_C_pex2miccv0[1] = -rtb_Sum_imgtnbczew_tmp_0;
  rtb_C_pex2miccv0[4] = rtb_Sum_imgtnbczew_tmp;
  rtb_C_pex2miccv0[7] = 0.0;
  rtb_C_pex2miccv0[2] = (-rtb_TrigonometricFunction2_h0qciwy3lv_tmp) *
    rtb_Sum_imgtnbczew_tmp;
  rtb_C_pex2miccv0[5] = (-rtb_TrigonometricFunction2_h0qciwy3lv_tmp) *
    rtb_Sum_imgtnbczew_tmp_0;
  rtb_C_pex2miccv0[8] = -curvature;

  /* Product: '<S59>/Product' incorporates:
   *  Product: '<S59>/Product1'
   *  Sum: '<S59>/Sum'
   *  Trigonometry: '<S59>/Trigonometric Function1'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (scale +
    (*rtu_mode_data_fwrth_data_alt_sp)) * rtb_Sum_imgtnbczew_idx_1;

  /* Sum: '<S57>/Sum' incorporates:
   *  Constant: '<S59>/1-e^2'
   *  Product: '<S59>/Product'
   *  Product: '<S59>/Product1'
   *  Product: '<S59>/Product2'
   *  Product: '<S59>/Product3'
   *  Product: '<S60>/Product'
   *  Product: '<S60>/Product1'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S59>/Sum1'
   *  Trigonometry: '<S59>/Trigonometric Function2'
   *  Trigonometry: '<S59>/Trigonometric Function3'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 *
     rtb_TrigonometricFunction6_onwcg4eh52) - (dir_idx_1 *
    rtb_Sum_imgtnbczew_tmp);
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 * Vg_0) - (dir_idx_1 *
    rtb_Sum_imgtnbczew_tmp_0);
  AC_norm = (Vg * ((0.993305620009879 * scale) +
                   (*rtu_mode_data_fwrth_data_alt_sp))) - dir_idx_0;

  /* Product: '<S58>/Product' */
  for (i = 0; i < 3; i++) {
    /* Concatenate: '<S136>/Vector Concatenate' */
    rtb_VectorConcatenate[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* End of Product: '<S58>/Product' */

  /* Sum: '<S68>/Add' incorporates:
   *  Constant: '<S5>/ADSAC_RTH_land'
   *  Sum: '<S75>/Add1'
   *  Trigonometry: '<S75>/Trigonometric Function6'
   */
  AC_norm = rtu_Sensor_pos_lla[1] - (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 +
    0.95705178402328117);

  /* Trigonometry: '<S68>/Trigonometric Function6' incorporates:
   *  Product: '<S68>/Product'
   *  Product: '<S68>/Product1'
   *  Product: '<S68>/Product2'
   *  Sum: '<S68>/Add1'
   *  Trigonometry: '<S68>/Trigonometric Function1'
   *  Trigonometry: '<S68>/Trigonometric Function5'
   */
  absxk = atan2(rtb_TrigonometricFunction2_h0qciwy3lv_tmp * sin(AC_norm),
                (rtb_Sum_imgtnbczew_idx_1 * curvature) - ((Vg *
    rtb_TrigonometricFunction2_h0qciwy3lv_tmp) * cos(AC_norm)));

  /* MATLAB Function: '<S47>/MATLAB Function1' */
  FW_TECS_switcher_illvnqr3hx(rtb_VectorConcatenate1, rtb_C_pex2miccv0);

  /* Sqrt: '<S53>/Sqrt' */
  AC_norm = rtb_Sqrt;

  /* Switch: '<S54>/Switch3' incorporates:
   *  Product: '<S54>/Product'
   */
  if (rtb_Sqrt <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S54>/Switch3' */

  /* Product: '<S53>/Divide' incorporates:
   *  Constant: '<S53>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* Product: '<S49>/Product' incorporates:
   *  Product: '<S49>/Product1'
   *  Sum: '<S49>/Sum'
   */
  dir_idx_1 = (AC_norm + (*rtu_mode_data_fwrth_data_alt_sp)) *
    rtb_Sum_imgtnbczew_idx_1;

  /* Product: '<S49>/Product3' incorporates:
   *  Constant: '<S49>/1-e^2'
   *  Product: '<S49>/Product2'
   *  Sum: '<S49>/Sum1'
   */
  dir_idx_0 = Vg * ((0.993305620009879 * AC_norm) +
                    (*rtu_mode_data_fwrth_data_alt_sp));

  /* Sqrt: '<S51>/Sqrt' */
  AC_norm = AC_norm_tmp;

  /* Switch: '<S52>/Switch3' incorporates:
   *  Product: '<S52>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S52>/Switch3' */

  /* Product: '<S51>/Divide' incorporates:
   *  Constant: '<S51>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* Product: '<S48>/Product' incorporates:
   *  Product: '<S48>/Product1'
   *  Sum: '<S48>/Sum'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (AC_norm + rtu_Sensor_pos_lla[2]) *
    rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S46>/Sum' incorporates:
   *  Constant: '<S48>/1-e^2'
   *  Product: '<S48>/Product'
   *  Product: '<S48>/Product1'
   *  Product: '<S48>/Product2'
   *  Product: '<S48>/Product3'
   *  Product: '<S49>/Product'
   *  Product: '<S49>/Product1'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S48>/Sum1'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    (dir_idx_1 * rtb_TrigonometricFunction6_onwcg4eh52);
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 *
               rtb_Sum_imgtnbczew_tmp_0) - (dir_idx_1 * Vg_0);
  AC_norm = (curvature * ((0.993305620009879 * AC_norm) + rtu_Sensor_pos_lla[2]))
    - dir_idx_0;

  /* Product: '<S47>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S47>/Product' */
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* End of Product: '<S47>/Product' */

  /* MATLAB Function: '<S39>/MATLAB Function' incorporates:
   *  SignalConversion generated from: '<S39>/Vector Concatenate1'
   * */
  /* MATLAB Function 'FW_RTH/L1_guidance/MATLAB Function': '<S45>:1' */
  /*  circle L1-guidance */
  /*  start_pos     [x; y] (m) */
  /*  direction     +1 CW, -1 CCW as seen from top */
  /*  cur_pos       [x; y] (m) */
  /*  vel_NE        [vx, vy] ground-speed (m/s) */
  /*  R             Turn Radius (m) */
  /*  R_min         minimum Radius (m) */
  /*  lat_ax        (m/s/s) lateral acceleration command */
  /* '<S45>:1:13' R_min = 100; */
  /* '<S45>:1:15' if R < R_min */
  /* '<S45>:1:18' curvature = direction*1/R; */
  /* '<S45>:1:20' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  Vg = rtb_TrigonometricFunction2_i3ecl5ucim;

  /*  horizontal ground speed */
  /* '<S45>:1:22' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S45>:1:23' Vg =0.001; */
    Vg = 0.001;
  }

  /* '<S45>:1:25' vel_dir = vel_NE/Vg; */
  /* '<S45>:1:26' chi = atan2(vel_dir(2), vel_dir(1)); */
  /* '<S45>:1:28' L1_P = 12; */
  /*  L1 peroid (s) */
  /* '<S45>:1:29' L1_zeta = 0.7; */
  /*  L1 samping ratio */
  /* '<S45>:1:30' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Sqrt = 2.6738030439438414 * Vg;

  /*  L1 length */
  /*  P = start_pos; */
  /*  dir = start_dir; */
  /* '<S45>:1:34' A = cur_pos; */
  /*  find center of circle O */
  /*  O = center_pos; %P + sign(curvature)*R*[-dir(2); dir(1)]; */
  /* '<S45>:1:39' OA = A; */
  /*  - O; */
  /* '<S45>:1:40' a = sqrt(OA(1)^2 + OA(2)^2); */
  AC_norm = sqrt((rtb_VectorConcatenate1[0] * rtb_VectorConcatenate1[0]) +
                 (rtb_VectorConcatenate1[1] * rtb_VectorConcatenate1[1]));

  /* '<S45>:1:42' if (L1 + a) <= R */
  if ((rtb_Sqrt + AC_norm) <= 150.0) {
    /* '<S45>:1:43' gamma = pi; */
    AC_norm = 3.1415926535897931;
  } else if (AC_norm >= (rtb_Sqrt + 150.0)) {
    /* '<S45>:1:44' elseif a >= (L1 + R) */
    /* '<S45>:1:45' gamma = 0; */
    AC_norm = 0.0;
  } else {
    /* '<S45>:1:46' else */
    /* '<S45>:1:47' temp1 = (L1^2 + a^2 - R^2)/(2*L1*a); */
    /*  here a != 0, if condition-1 covers a=0 case */
    /* '<S45>:1:48' temp1 = max(temp1, -1); */
    /* '<S45>:1:49' temp1 = min(temp1, 1); */
    /* '<S45>:1:50' gamma = acos(temp1); */
    rtb_VectorConcatenate1_psilb5rrki_idx_0 = (((rtb_Sqrt * rtb_Sqrt) + (AC_norm
      * AC_norm)) - 22500.0) / ((2.0 * rtb_Sqrt) * AC_norm);
    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 < -1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = -1.0;
    }

    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 > 1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = 1.0;
    }

    AC_norm = acos(rtb_VectorConcatenate1_psilb5rrki_idx_0);
  }

  /* '<S45>:1:53' chi_ao = atan2(-OA(2), -OA(1)); */
  /* '<S45>:1:54' chi_L = wrap_pi(chi_ao - sign(curvature)*gamma); */
  AC_norm = atan2(-rtb_VectorConcatenate1[1], -rtb_VectorConcatenate1[0]) -
    (-AC_norm);

  /* '<S45>:1:69' if ang_in > pi */
  if (AC_norm > 3.1415926535897931) {
    /* '<S45>:1:70' ang_in = ang_in - 2*pi; */
    AC_norm -= 6.2831853071795862;
  } else if (AC_norm < -3.1415926535897931) {
    /* '<S45>:1:71' elseif ang_in < -pi */
    /* '<S45>:1:72' ang_in = ang_in + 2*pi; */
    AC_norm += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* '<S45>:1:75' ang_out = ang_in; */
  /* '<S45>:1:56' eta = wrap_pi(chi_L - chi); */
  rtb_Sum_imgtnbczew_idx_1 = AC_norm - atan2(rtu_Sensor_vel_ned[1] / Vg,
    rtu_Sensor_vel_ned[0] / Vg);

  /* '<S45>:1:69' if ang_in > pi */
  if (rtb_Sum_imgtnbczew_idx_1 > 3.1415926535897931) {
    /* '<S45>:1:70' ang_in = ang_in - 2*pi; */
    rtb_Sum_imgtnbczew_idx_1 -= 6.2831853071795862;
  } else if (rtb_Sum_imgtnbczew_idx_1 < -3.1415926535897931) {
    /* '<S45>:1:71' elseif ang_in < -pi */
    /* '<S45>:1:72' ang_in = ang_in + 2*pi; */
    rtb_Sum_imgtnbczew_idx_1 += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  Abs: '<S40>/Abs'
   *  Constant: '<S55>/Constant'
   *  Constant: '<S56>/Constant'
   *  Constant: '<S5>/loiter_R'
   *  Logic: '<S40>/AND'
   *  Math: '<S40>/Square1'
   *  Memory: '<S5>/Memory4'
   *  RelationalOperator: '<S55>/Compare'
   *  RelationalOperator: '<S56>/Compare'
   *  Sqrt: '<S40>/Sqrt1'
   *  Sum: '<S40>/Sum of Elements1'
   *  Sum: '<S40>/Sum1'
   */
  /* '<S45>:1:75' ang_out = ang_in; */
  /* '<S45>:1:57' eta = max(eta, -pi); */
  /* '<S45>:1:58' eta = min(eta, pi); */
  /* '<S45>:1:60' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  /* '<S45>:1:62' L1_angle = chi_L; */
  *rty_FWRTH_SM_in_approach_circle_done = ((fabs(sqrt((rtb_VectorConcatenate[0] *
    rtb_VectorConcatenate[0]) + (rtb_VectorConcatenate[1] *
    rtb_VectorConcatenate[1])) - 150.0) <= 10.0) &&
    (FW_TECS_switcher_DW.Memory4_PreviousInput == approach_circle));

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  Abs: '<S42>/Abs'
   *  Abs: '<S42>/Abs1'
   *  Constant: '<S42>/Constant2'
   *  Constant: '<S42>/Constant3'
   *  Constant: '<S71>/Constant'
   *  Delay: '<S5>/Delay'
   *  Logic: '<S42>/AND'
   *  RelationalOperator: '<S42>/Less Than'
   *  RelationalOperator: '<S42>/Less Than1'
   *  RelationalOperator: '<S71>/Compare'
   *  Sum: '<S42>/Sum'
   *  Sum: '<S42>/Sum1'
   */
  *rty_FWRTH_SM_in_reduce_speed_alt_done = (((fabs((*rtu_Sensor_aspd_cas) -
    (*rtu_mode_data_fwrth_data_CAS_sp)) < 0.5) && (fabs
    ((*rtu_mode_data_fwrth_data_alt_sp) - rtu_Sensor_pos_lla[2]) < 1.0)) &&
    (FW_TECS_switcher_DW.Delay_DSTATE_iwjnjngoev[0] == set_speed_alt));

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  Abs: '<S41>/Abs'
   *  Constant: '<S41>/Constant2'
   *  Constant: '<S66>/Constant'
   *  Constant: '<S67>/Constant'
   *  Constant: '<S69>/Constant2'
   *  Constant: '<S70>/Constant2'
   *  Delay: '<S5>/Delay1'
   *  Logic: '<S41>/AND'
   *  Math: '<S69>/Mod2'
   *  Math: '<S69>/Mod3'
   *  Math: '<S70>/Mod2'
   *  Math: '<S70>/Mod3'
   *  RelationalOperator: '<S66>/Compare'
   *  RelationalOperator: '<S67>/Compare'
   *  Sum: '<S41>/Sum'
   *  Sum: '<S41>/Sum1'
   *  Sum: '<S69>/Add3'
   *  Sum: '<S69>/Add4'
   *  Sum: '<S69>/Sum'
   *  Sum: '<S70>/Add3'
   *  Sum: '<S70>/Add4'
   *  Sum: '<S70>/Sum'
   *  Trigonometry: '<S41>/Atan2'
   */
  *rty_FWRTH_SM_in_circle_align_done = ((fabs(rt_modd(rt_modd((((rt_modd(rt_modd
    (((absxk - 1.5707963267948966) - -3.1415926535897931) +
     FW_TECS_switcher_ConstB.Sum1_ifh4nmipzo,
     FW_TECS_switcher_ConstB.Sum1_ifh4nmipzo),
    FW_TECS_switcher_ConstB.Sum1_ifh4nmipzo) - 3.1415926535897931) - atan2
    (rtu_Sensor_vel_ned[1], rtu_Sensor_vel_ned[0])) - -3.1415926535897931) +
    FW_TECS_switcher_ConstB.Sum1_dex1bd4twp,
    FW_TECS_switcher_ConstB.Sum1_dex1bd4twp),
    FW_TECS_switcher_ConstB.Sum1_dex1bd4twp) - 3.1415926535897931) <=
    0.17453292519943295) && (FW_TECS_switcher_DW.Delay1_DSTATE_niw4uw2lp0[0] ==
    align_circle));

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  Abs: '<S43>/Abs1'
   *  Constant: '<S72>/Constant'
   *  Constant: '<S73>/Constant'
   *  Constant: '<S74>/Constant2'
   *  Delay: '<S5>/Delay2'
   *  Logic: '<S43>/AND'
   *  MATLAB Function: '<S39>/MATLAB Function'
   *  Math: '<S74>/Mod2'
   *  Math: '<S74>/Mod3'
   *  RelationalOperator: '<S72>/Compare'
   *  RelationalOperator: '<S73>/Compare'
   *  Sum: '<S43>/Sum2'
   *  Sum: '<S74>/Add3'
   *  Sum: '<S74>/Add4'
   *  Sum: '<S74>/Sum'
   */
  *rty_FWRTH_SM_in_cross_tangent_point_done = ((fabs(rt_modd(rt_modd(((AC_norm -
    rtb_Product1_bkygj4a2su) - -3.1415926535897931) +
    FW_TECS_switcher_ConstB.Sum1_obqa34fcll,
    FW_TECS_switcher_ConstB.Sum1_obqa34fcll),
    FW_TECS_switcher_ConstB.Sum1_obqa34fcll) - 3.1415926535897931) <=
    0.087266462599716474) && (FW_TECS_switcher_DW.Delay2_DSTATE_ctj124hvji[0] ==
    cross_tangent));

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  BusAssignment: '<S5>/Bus Assignment'
   *  Constant: '<S5>/ADSAC_RTH_land'
   */
  *rty_FWRTH_SM_in_land_lat = 0.43151857274853433;

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  BusAssignment: '<S5>/Bus Assignment'
   *  Constant: '<S5>/ADSAC_RTH_land'
   */
  *rty_FWRTH_SM_in_land_lon = 0.95705178402328117;

  /* SignalConversion generated from: '<Root>/FWRTH_SM_in' incorporates:
   *  BusAssignment: '<S5>/Bus Assignment'
   */
  *rty_FWRTH_SM_in_approach_ang = rtb_Product1_bkygj4a2su;

  /* Delay: '<S106>/cur_pos_sp' incorporates:
   *  Memory: '<S106>/Memory2'
   */
  FW_TECS_switcher_DW.icLoad = (((FW_TECS_switcher_DW.Memory2_PreviousInput) &&
    (((uint32_T)FW_TECS_switcher_PrevZCX.cur_pos_sp_Reset_ZCE) != POS_ZCSIG)) ||
    (FW_TECS_switcher_DW.icLoad));
  FW_TECS_switcher_PrevZCX.cur_pos_sp_Reset_ZCE =
    FW_TECS_switcher_DW.Memory2_PreviousInput ? ((ZCSigState)1) : ((ZCSigState)0);
  if (FW_TECS_switcher_DW.icLoad) {
    FW_TECS_switcher_DW.cur_pos_sp_DSTATE[0] = *rtu_wp_data_cur_wp_lat;
    FW_TECS_switcher_DW.cur_pos_sp_DSTATE[1] = *rtu_wp_data_cur_wp_lon;
    FW_TECS_switcher_DW.cur_pos_sp_DSTATE[2] = *rtu_wp_data_cur_wp_alt;
  }

  /* Trigonometry: '<S116>/Trigonometric Function' incorporates:
   *  Delay: '<S106>/cur_pos_sp'
   *  Trigonometry: '<S128>/Trigonometric Function1'
   */
  Vg_0 = cos(FW_TECS_switcher_DW.cur_pos_sp_DSTATE[0]);

  /* RelationalOperator: '<S112>/FixPt Relational Operator' incorporates:
   *  UnitDelay: '<S112>/Delay Input1'
   *
   * Block description for '<S112>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_Equal_bb4ze044et = ((*rtu_wp_data_cmd_wp_idx) !=
    FW_TECS_switcher_DW.DelayInput1_DSTATE);

  /* RelationalOperator: '<S102>/Compare' incorporates:
   *  Constant: '<S102>/Constant'
   */
  rtb_Compare_ag4sp0vjdy = ((*rtu_vom_status) == WAYPNT);

  /* Memory: '<S8>/Memory' */
  rtb_Memory_mouojdlutb = FW_TECS_switcher_DW.Memory_PreviousInput_frepmgenr3;

  /* RelationalOperator: '<S114>/FixPt Relational Operator' incorporates:
   *  Memory: '<S8>/Memory'
   *  UnitDelay: '<S114>/Delay Input1'
   *
   * Block description for '<S114>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_FixPtRelationalOperator_h1ymkdiv2b = (((int32_T)
    (FW_TECS_switcher_DW.Memory_PreviousInput_frepmgenr3 ? 1 : 0)) > ((int32_T)
    (FW_TECS_switcher_DW.DelayInput1_DSTATE_eu53xg3kmp ? 1 : 0)));

  /* Logic: '<S106>/OR' incorporates:
   *  RelationalOperator: '<S113>/FixPt Relational Operator'
   *  UnitDelay: '<S113>/Delay Input1'
   *
   * Block description for '<S113>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_OR = ((rtb_Equal_bb4ze044et || (((int32_T)(rtb_Compare_ag4sp0vjdy ? 1 : 0))
              > ((int32_T)(FW_TECS_switcher_DW.DelayInput1_DSTATE_hck53lu3v0 ? 1
    : 0)))) || rtb_FixPtRelationalOperator_h1ymkdiv2b);

  /* Delay: '<S106>/prev_pos_sp' */
  FW_TECS_switcher_DW.icLoad_jveizoqakw = ((rtb_OR && (((uint32_T)
    FW_TECS_switcher_PrevZCX.prev_pos_sp_Reset_ZCE) != POS_ZCSIG)) ||
    (FW_TECS_switcher_DW.icLoad_jveizoqakw));
  FW_TECS_switcher_PrevZCX.prev_pos_sp_Reset_ZCE = rtb_OR ? ((ZCSigState)1) :
    ((ZCSigState)0);
  if (FW_TECS_switcher_DW.icLoad_jveizoqakw) {
    /* Switch: '<S106>/Switch' incorporates:
     *  Delay: '<S106>/cur_pos_sp'
     */
    if (rtb_FixPtRelationalOperator_h1ymkdiv2b) {
      FW_TECS_switcher_DW.prev_pos_sp_DSTATE[0] =
        FW_TECS_switcher_DW.cur_pos_sp_DSTATE[0];
      FW_TECS_switcher_DW.prev_pos_sp_DSTATE[1] =
        FW_TECS_switcher_DW.cur_pos_sp_DSTATE[1];
      FW_TECS_switcher_DW.prev_pos_sp_DSTATE[2] =
        FW_TECS_switcher_DW.cur_pos_sp_DSTATE[2];
    } else {
      FW_TECS_switcher_DW.prev_pos_sp_DSTATE[0] = rtu_Sensor_pos_lla[0];
      FW_TECS_switcher_DW.prev_pos_sp_DSTATE[1] = rtu_Sensor_pos_lla[1];
      FW_TECS_switcher_DW.prev_pos_sp_DSTATE[2] = rtu_Sensor_pos_lla[2];
    }

    /* End of Switch: '<S106>/Switch' */
  }

  /* Sum: '<S116>/Add' incorporates:
   *  Delay: '<S106>/cur_pos_sp'
   *  Delay: '<S106>/prev_pos_sp'
   */
  rtb_Product1_bkygj4a2su = FW_TECS_switcher_DW.cur_pos_sp_DSTATE[1] -
    FW_TECS_switcher_DW.prev_pos_sp_DSTATE[1];

  /* Trigonometry: '<S116>/Trigonometric Function4' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   *  Trigonometry: '<S121>/Trigonometric Function'
   *  Trigonometry: '<S129>/Trigonometric Function'
   */
  rtb_Sum_lskvbzjz0i = sin(FW_TECS_switcher_DW.prev_pos_sp_DSTATE[0]);

  /* Trigonometry: '<S116>/Trigonometric Function2' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   *  Trigonometry: '<S121>/Trigonometric Function1'
   *  Trigonometry: '<S129>/Trigonometric Function1'
   */
  PB_norm = cos(FW_TECS_switcher_DW.prev_pos_sp_DSTATE[0]);

  /* Trigonometry: '<S116>/Trigonometric Function3' incorporates:
   *  Delay: '<S106>/cur_pos_sp'
   *  Trigonometry: '<S128>/Trigonometric Function'
   */
  rtb_TrigonometricFunction_paar0fmbpe = sin
    (FW_TECS_switcher_DW.cur_pos_sp_DSTATE[0]);

  /* Trigonometry: '<S116>/Trigonometric Function6' incorporates:
   *  Product: '<S116>/Product'
   *  Product: '<S116>/Product1'
   *  Product: '<S116>/Product2'
   *  Sum: '<S116>/Add1'
   *  Trigonometry: '<S116>/Trigonometric Function'
   *  Trigonometry: '<S116>/Trigonometric Function1'
   *  Trigonometry: '<S116>/Trigonometric Function2'
   *  Trigonometry: '<S116>/Trigonometric Function3'
   *  Trigonometry: '<S116>/Trigonometric Function4'
   *  Trigonometry: '<S116>/Trigonometric Function5'
   */
  rtb_TrigonometricFunction6_onwcg4eh52 = atan2(Vg_0 * sin
    (rtb_Product1_bkygj4a2su), (PB_norm * rtb_TrigonometricFunction_paar0fmbpe)
    - ((rtb_Sum_lskvbzjz0i * Vg_0) * cos(rtb_Product1_bkygj4a2su)));

  /* MATLAB Function: '<S127>/MATLAB Function1' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   */
  FW_TECS_switcher_illvnqr3hx(FW_TECS_switcher_DW.prev_pos_sp_DSTATE,
    rtb_C_pex2miccv0);

  /* Product: '<S133>/Product' incorporates:
   *  Constant: '<S133>/e'
   *  Product: '<S125>/Product'
   */
  rtb_VectorConcatenate1_oigmvxpsrq_idx_0 = rtb_Sum_lskvbzjz0i * 0.0818191908425;

  /* Sqrt: '<S133>/Sqrt' incorporates:
   *  Constant: '<S133>/const'
   *  Math: '<S133>/Square'
   *  Product: '<S133>/Product'
   *  Sqrt: '<S125>/Sqrt'
   *  Sum: '<S133>/Add'
   */
  rtb_VectorConcatenate1_oigmvxpsrq_idx_0 = sqrt(1.0 -
    (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 *
     rtb_VectorConcatenate1_oigmvxpsrq_idx_0));
  rtb_Product1_bkygj4a2su = rtb_VectorConcatenate1_oigmvxpsrq_idx_0;

  /* Switch: '<S134>/Switch3' incorporates:
   *  Product: '<S134>/Product'
   *  Sqrt: '<S133>/Sqrt'
   */
  if (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S134>/Switch3' */

  /* Product: '<S133>/Divide' incorporates:
   *  Constant: '<S133>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Trigonometry: '<S129>/Trigonometric Function3' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   *  Trigonometry: '<S121>/Trigonometric Function3'
   */
  scale = cos(FW_TECS_switcher_DW.prev_pos_sp_DSTATE[1]);

  /* Product: '<S129>/Product' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   *  Product: '<S129>/Product1'
   *  Sum: '<S129>/Sum'
   */
  dir_idx_1 = (rtb_Product1_bkygj4a2su + FW_TECS_switcher_DW.prev_pos_sp_DSTATE
               [2]) * PB_norm;

  /* Trigonometry: '<S129>/Trigonometric Function2' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   *  Trigonometry: '<S121>/Trigonometric Function2'
   */
  absxk = sin(FW_TECS_switcher_DW.prev_pos_sp_DSTATE[1]);

  /* Product: '<S129>/Product3' incorporates:
   *  Constant: '<S129>/1-e^2'
   *  Delay: '<S106>/prev_pos_sp'
   *  Product: '<S129>/Product2'
   *  Sum: '<S129>/Sum1'
   */
  dir_idx_0 = rtb_Sum_lskvbzjz0i * ((0.993305620009879 * rtb_Product1_bkygj4a2su)
    + FW_TECS_switcher_DW.prev_pos_sp_DSTATE[2]);

  /* Product: '<S131>/Product' incorporates:
   *  Constant: '<S131>/e'
   */
  rtb_Product1_bkygj4a2su = rtb_TrigonometricFunction_paar0fmbpe *
    0.0818191908425;

  /* Sqrt: '<S131>/Sqrt' incorporates:
   *  Constant: '<S131>/const'
   *  Math: '<S131>/Square'
   *  Sum: '<S131>/Add'
   */
  rtb_Product1_bkygj4a2su = sqrt(1.0 - (rtb_Product1_bkygj4a2su *
    rtb_Product1_bkygj4a2su));

  /* Switch: '<S132>/Switch3' incorporates:
   *  Abs: '<S132>/Abs'
   *  Product: '<S132>/Product'
   */
  if (rtb_Product1_bkygj4a2su <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S132>/Switch3' */

  /* Product: '<S131>/Divide' incorporates:
   *  Constant: '<S131>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Product: '<S128>/Product' incorporates:
   *  Delay: '<S106>/cur_pos_sp'
   *  Product: '<S128>/Product1'
   *  Sum: '<S128>/Sum'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (rtb_Product1_bkygj4a2su +
    FW_TECS_switcher_DW.cur_pos_sp_DSTATE[2]) * Vg_0;

  /* Sum: '<S118>/Sum' incorporates:
   *  Constant: '<S128>/1-e^2'
   *  Delay: '<S106>/cur_pos_sp'
   *  Product: '<S128>/Product'
   *  Product: '<S128>/Product1'
   *  Product: '<S128>/Product2'
   *  Product: '<S128>/Product3'
   *  Product: '<S129>/Product'
   *  Product: '<S129>/Product1'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S128>/Sum1'
   *  Trigonometry: '<S128>/Trigonometric Function2'
   *  Trigonometry: '<S128>/Trigonometric Function3'
   *  Trigonometry: '<S129>/Trigonometric Function2'
   *  Trigonometry: '<S129>/Trigonometric Function3'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 * cos
     (FW_TECS_switcher_DW.cur_pos_sp_DSTATE[1])) - (dir_idx_1 * scale);
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 * sin
               (FW_TECS_switcher_DW.cur_pos_sp_DSTATE[1])) - (dir_idx_1 * absxk);
  AC_norm = (rtb_TrigonometricFunction_paar0fmbpe * ((0.993305620009879 *
    rtb_Product1_bkygj4a2su) + FW_TECS_switcher_DW.cur_pos_sp_DSTATE[2])) -
    dir_idx_0;

  /* Product: '<S127>/Product' */
  for (i = 0; i < 3; i++) {
    /* Concatenate: '<S136>/Vector Concatenate' */
    rtb_VectorConcatenate[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* End of Product: '<S127>/Product' */

  /* Sqrt: '<S115>/Sqrt' incorporates:
   *  Math: '<S115>/Square'
   *  Sum: '<S115>/Sum of Elements'
   */
  rtb_Sqrt_jug5wuieqd = sqrt((rtb_VectorConcatenate[0] * rtb_VectorConcatenate[0])
    + (rtb_VectorConcatenate[1] * rtb_VectorConcatenate[1]));

  /* MATLAB Function: '<S119>/MATLAB Function1' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   */
  FW_TECS_switcher_illvnqr3hx(FW_TECS_switcher_DW.prev_pos_sp_DSTATE,
    rtb_C_pex2miccv0);

  /* Sqrt: '<S125>/Sqrt' */
  rtb_Product1_bkygj4a2su = rtb_VectorConcatenate1_oigmvxpsrq_idx_0;

  /* Switch: '<S126>/Switch3' incorporates:
   *  Product: '<S126>/Product'
   */
  if (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S126>/Switch3' */

  /* Product: '<S125>/Divide' incorporates:
   *  Constant: '<S125>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Product: '<S121>/Product' incorporates:
   *  Delay: '<S106>/prev_pos_sp'
   *  Product: '<S121>/Product1'
   *  Sum: '<S121>/Sum'
   */
  dir_idx_1 = (rtb_Product1_bkygj4a2su + FW_TECS_switcher_DW.prev_pos_sp_DSTATE
               [2]) * PB_norm;

  /* Product: '<S121>/Product3' incorporates:
   *  Constant: '<S121>/1-e^2'
   *  Delay: '<S106>/prev_pos_sp'
   *  Product: '<S121>/Product2'
   *  Sum: '<S121>/Sum1'
   */
  dir_idx_0 = rtb_Sum_lskvbzjz0i * ((0.993305620009879 * rtb_Product1_bkygj4a2su)
    + FW_TECS_switcher_DW.prev_pos_sp_DSTATE[2]);

  /* Sqrt: '<S123>/Sqrt' */
  rtb_Product1_bkygj4a2su = AC_norm_tmp;

  /* Switch: '<S124>/Switch3' incorporates:
   *  Product: '<S124>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S124>/Switch3' */

  /* Product: '<S123>/Divide' incorporates:
   *  Constant: '<S123>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Product: '<S120>/Product' incorporates:
   *  Product: '<S120>/Product1'
   *  Sum: '<S120>/Sum'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (rtb_Product1_bkygj4a2su +
    rtu_Sensor_pos_lla[2]) * rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S117>/Sum' incorporates:
   *  Constant: '<S120>/1-e^2'
   *  Product: '<S120>/Product'
   *  Product: '<S120>/Product1'
   *  Product: '<S120>/Product2'
   *  Product: '<S120>/Product3'
   *  Product: '<S121>/Product'
   *  Product: '<S121>/Product1'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S120>/Sum1'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    (dir_idx_1 * scale);
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 *
               rtb_Sum_imgtnbczew_tmp_0) - (dir_idx_1 * absxk);
  AC_norm = (curvature * ((0.993305620009879 * rtb_Product1_bkygj4a2su) +
              rtu_Sensor_pos_lla[2])) - dir_idx_0;

  /* Product: '<S119>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S119>/Product' */
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* End of Product: '<S119>/Product' */

  /* MATLAB Function: '<S8>/guidance_line1' */
  /* MATLAB Function 'WPNav/guidance_line1': '<S107>:1' */
  /*  straight line L1-guidance */
  /*  start_pos         [x; y] (m) */
  /*  start_dir_ang     direction from North (rad) */
  /*  cur_pos           [x; y] (m) */
  /*  Vg                [vx, vy] ground-speed (m/s) */
  /*  lat_ax            (m/s/s) lateral acceleration command */
  /*  dist_to_cover       distance to reach current waypoint along the path (m) */
  /* '<S107>:1:12' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  Vg_0 = rtb_TrigonometricFunction2_i3ecl5ucim;

  /*  horizontal ground speed */
  /* '<S107>:1:13' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S107>:1:14' Vg =0.001; */
    Vg_0 = 0.001;
  }

  /* '<S107>:1:17' vel_dir = vel_NE/Vg; */
  /* '<S107>:1:19' L1_P = 15; */
  /*  L1 peroid (s) */
  /* '<S107>:1:20' L1_zeta = 0.7; */
  /*  L1 damping ratio */
  /* '<S107>:1:21' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Sum_lskvbzjz0i = 3.3422538049298023 * Vg_0;

  /*  L1 length */
  /* '<S107>:1:23' P = start_pos; */
  /* '<S107>:1:24' dir = [cos(start_dir_ang); sin(start_dir_ang)]; */
  dir_idx_0 = cos(rtb_TrigonometricFunction6_onwcg4eh52);
  dir_idx_1 = sin(rtb_TrigonometricFunction6_onwcg4eh52);

  /* '<S107>:1:25' A = cur_pos; */
  /* '<S107>:1:27' PA = A - P; */
  /* '<S107>:1:28' C = P + dir*dot(PA, dir); */
  rtb_VectorConcatenate1_oigmvxpsrq_idx_0 = rtu_Sensor_vel_ned[0] / Vg_0;
  rtb_VectorConcatenate1_oigmvxpsrq_idx_1 = rtu_Sensor_vel_ned[1] / Vg_0;
  AC_norm = (rtb_VectorConcatenate1[0] * dir_idx_0) + (rtb_VectorConcatenate1[1]
    * dir_idx_1);

  /* '<S107>:1:29' AC = C - A; */
  absxk = dir_idx_0 * AC_norm;
  C_idx_0 = absxk;
  AC_idx_0 = absxk - rtb_VectorConcatenate1[0];
  absxk = dir_idx_1 * AC_norm;
  AC_idx_1 = absxk - rtb_VectorConcatenate1[1];

  /* '<S107>:1:30' AC_norm = sqrt(AC(1)^2 + AC(2)^2); */
  AC_norm = sqrt((AC_idx_0 * AC_idx_0) + (AC_idx_1 * AC_idx_1));

  /* '<S107>:1:32' if(AC_norm < L1) */
  if (AC_norm < rtb_Sum_lskvbzjz0i) {
    /* '<S107>:1:33' CB_norm = sqrt(L1^2 - AC_norm^2); */
    AC_norm = sqrt((rtb_Sum_lskvbzjz0i * rtb_Sum_lskvbzjz0i) - (AC_norm *
      AC_norm));
  } else {
    /* '<S107>:1:34' else */
    /* '<S107>:1:35' CB_norm = 0; */
    AC_norm = 0.0;
  }

  /* '<S107>:1:38' B = C + dir*(CB_norm); */
  /* '<S107>:1:39' AB_dir = (B-A)/norm(B-A); */
  rtb_Product1_bkygj4a2su = C_idx_0 + (dir_idx_0 * AC_norm);
  C_idx_0 = rtb_Product1_bkygj4a2su;
  AC_idx_0 = rtb_Product1_bkygj4a2su - rtb_VectorConcatenate1[0];
  rtb_Product1_bkygj4a2su = absxk + (dir_idx_1 * AC_norm);
  AC_idx_1 = rtb_Product1_bkygj4a2su - rtb_VectorConcatenate1[1];
  scale = 3.3121686421112381E-170;
  absxk = fabs(AC_idx_0);
  if (absxk > 3.3121686421112381E-170) {
    AC_norm = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    AC_norm = t * t;
  }

  absxk = fabs(AC_idx_1);
  if (absxk > scale) {
    t = scale / absxk;
    AC_norm = ((AC_norm * t) * t) + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    AC_norm += t * t;
  }

  AC_norm = scale * sqrt(AC_norm);

  /* Update for Memory: '<S8>/Memory' incorporates:
   *  MATLAB Function: '<S8>/guidance_line1'
   */
  /* '<S107>:1:41' sin_eta = vel_dir(1)*AB_dir(2) - vel_dir(2)*AB_dir(1); */
  /*  2D cross product */
  /* '<S107>:1:42' cos_eta = vel_dir(1)*AB_dir(1) + vel_dir(2)*AB_dir(2); */
  /*  2D dot product */
  /* '<S107>:1:43' eta = atan2(sin_eta, cos_eta); */
  /* '<S107>:1:44' eta = max(eta, -pi/2); */
  /* '<S107>:1:45' eta = min(eta,  pi/2); */
  /* '<S107>:1:47' path_done = false; */
  FW_TECS_switcher_DW.Memory_PreviousInput_frepmgenr3 = false;

  /* MATLAB Function: '<S8>/guidance_line1' */
  /* '<S107>:1:48' PB = B - P; */
  /* '<S107>:1:49' PB_norm = sqrt(PB(1)^2 + PB(2)^2); */
  /* '<S107>:1:51' if sign(dot(PB,dir))*PB_norm > path_len */
  AC_idx_0 /= AC_norm;
  AC_idx_1 /= AC_norm;
  rtb_TrigonometricFunction_paar0fmbpe = (C_idx_0 * dir_idx_0) +
    (rtb_Product1_bkygj4a2su * dir_idx_1);
  PB_norm = sqrt((C_idx_0 * C_idx_0) + (rtb_Product1_bkygj4a2su *
    rtb_Product1_bkygj4a2su));
  if (rtb_TrigonometricFunction_paar0fmbpe < 0.0) {
    i = -1;
  } else {
    i = (rtb_TrigonometricFunction_paar0fmbpe > 0.0) ? ((int32_T)1) : ((int32_T)
      0);
  }

  if ((((real_T)i) * PB_norm) > rtb_Sqrt_jug5wuieqd) {
    /* Update for Memory: '<S8>/Memory' */
    /* '<S107>:1:52' path_done = true; */
    FW_TECS_switcher_DW.Memory_PreviousInput_frepmgenr3 = true;
  }

  /* Delay: '<S8>/Delay3' */
  /* '<S107>:1:55' dist_to_cover = path_len - PB_norm; */
  /* '<S107>:1:57' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  rtb_Delay3 = FW_TECS_switcher_DW.Delay3_DSTATE[0];

  /* RelationalOperator: '<S104>/FixPt Relational Operator' incorporates:
   *  Delay: '<S8>/Delay3'
   *  UnitDelay: '<S104>/Delay Input1'
   *
   * Block description for '<S104>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_FixPtRelationalOperator_lyd2dqurri = (((int32_T)
    (FW_TECS_switcher_DW.Delay3_DSTATE[0] ? 1 : 0)) > ((int32_T)
    (FW_TECS_switcher_DW.DelayInput1_DSTATE_ezpkpkh21k ? 1 : 0)));

  /* Delay: '<S8>/Delay1' */
  FW_TECS_switcher_DW.icLoad_ncnilzog2z =
    ((rtb_FixPtRelationalOperator_lyd2dqurri && (((uint32_T)
        FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE) != POS_ZCSIG)) ||
     (FW_TECS_switcher_DW.icLoad_ncnilzog2z));
  FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE =
    rtb_FixPtRelationalOperator_lyd2dqurri ? ((ZCSigState)1) : ((ZCSigState)0);
  if (FW_TECS_switcher_DW.icLoad_ncnilzog2z) {
    FW_TECS_switcher_DW.Delay1_DSTATE[0] = *rtu_wp_data_cur_wp_lat;
    FW_TECS_switcher_DW.Delay1_DSTATE[1] = *rtu_wp_data_cur_wp_lon;
    FW_TECS_switcher_DW.Delay1_DSTATE[2] = *rtu_wp_data_cur_wp_alt;
  }

  /* Trigonometry: '<S108>/Trigonometric Function' incorporates:
   *  Delay: '<S8>/Delay1'
   */
  rtb_TrigonometricFunction_paar0fmbpe = cos(FW_TECS_switcher_DW.Delay1_DSTATE[0]);

  /* Delay: '<S8>/Delay2' incorporates:
   *  Delay: '<S8>/Delay1'
   */
  FW_TECS_switcher_DW.icLoad_fj3gi4q45w =
    ((rtb_FixPtRelationalOperator_lyd2dqurri && (((uint32_T)
        FW_TECS_switcher_PrevZCX.Delay2_Reset_ZCE) != POS_ZCSIG)) ||
     (FW_TECS_switcher_DW.icLoad_fj3gi4q45w));
  FW_TECS_switcher_PrevZCX.Delay2_Reset_ZCE =
    rtb_FixPtRelationalOperator_lyd2dqurri ? ((ZCSigState)1) : ((ZCSigState)0);
  if (FW_TECS_switcher_DW.icLoad_fj3gi4q45w) {
    FW_TECS_switcher_DW.Delay2_DSTATE[0] = rtu_Sensor_pos_lla[0];
    FW_TECS_switcher_DW.Delay2_DSTATE[1] = rtu_Sensor_pos_lla[1];
    FW_TECS_switcher_DW.Delay2_DSTATE[2] = rtu_Sensor_pos_lla[2];
  }

  /* Sum: '<S108>/Add' incorporates:
   *  Delay: '<S8>/Delay1'
   *  Delay: '<S8>/Delay2'
   */
  rtb_Product1_bkygj4a2su = FW_TECS_switcher_DW.Delay1_DSTATE[1] -
    FW_TECS_switcher_DW.Delay2_DSTATE[1];

  /* SignalConversion generated from: '<Root>/WP_SMdata' incorporates:
   *  BusAssignment: '<S8>/Bus Assignment'
   */
  *rty_WP_SMdata_cur_leg_heading = rtb_TrigonometricFunction6_onwcg4eh52;

  /* SignalConversion generated from: '<Root>/WP_SMdata' incorporates:
   *  BusAssignment: '<S8>/Bus Assignment'
   */
  *rty_WP_SMdata_cur_leg_length = rtb_Sqrt_jug5wuieqd;

  /* SignalConversion generated from: '<Root>/WP_SMdata' incorporates:
   *  MATLAB Function: '<S8>/guidance_line1'
   */
  *rty_WP_SMdata_cur_leg_remaining_dist = rtb_Sqrt_jug5wuieqd - PB_norm;

  /* SignalConversion generated from: '<Root>/WP_SMdata' */
  *rty_WP_SMdata_wp_list_valid = *rtu_wp_data_wp_list_valid;

  /* SignalConversion generated from: '<Root>/WP_SMdata' incorporates:
   *  Delay: '<S8>/Delay'
   */
  *rty_WP_SMdata_last_wp_land = FW_TECS_switcher_DW.Delay_DSTATE_perkgn1qf5[0];

  /* SignalConversion generated from: '<Root>/WP_SMdata' */
  *rty_WP_SMdata_land_wp_lat = *rtu_wp_data_cur_wp_lat;

  /* SignalConversion generated from: '<Root>/WP_SMdata' */
  *rty_WP_SMdata_land_wp_lon = *rtu_wp_data_cur_wp_lon;

  /* SignalConversion generated from: '<Root>/WP_SMdata' incorporates:
   *  Delay: '<S8>/Delay1'
   *  Delay: '<S8>/Delay2'
   *  Product: '<S108>/Product'
   *  Product: '<S108>/Product1'
   *  Product: '<S108>/Product2'
   *  Sum: '<S108>/Add1'
   *  Trigonometry: '<S108>/Trigonometric Function1'
   *  Trigonometry: '<S108>/Trigonometric Function2'
   *  Trigonometry: '<S108>/Trigonometric Function3'
   *  Trigonometry: '<S108>/Trigonometric Function4'
   *  Trigonometry: '<S108>/Trigonometric Function5'
   *  Trigonometry: '<S108>/Trigonometric Function6'
   */
  *rty_WP_SMdata_curpos_to_wp_heading = atan2
    (rtb_TrigonometricFunction_paar0fmbpe * sin(rtb_Product1_bkygj4a2su), (cos
      (FW_TECS_switcher_DW.Delay2_DSTATE[0]) * sin
      (FW_TECS_switcher_DW.Delay1_DSTATE[0])) - ((sin
       (FW_TECS_switcher_DW.Delay2_DSTATE[0]) *
       rtb_TrigonometricFunction_paar0fmbpe) * cos(rtb_Product1_bkygj4a2su)));

  /* SignalConversion generated from: '<Root>/WP_SMdata' incorporates:
   *  BusAssignment: '<S8>/Bus Assignment'
   */
  *rty_WP_SMdata_WPN_cmd_received = rtb_FixPtRelationalOperator_lyd2dqurri;

  /* Logic: '<S26>/AND' incorporates:
   *  Constant: '<S26>/Lower Limit'
   *  Constant: '<S26>/Upper Limit'
   *  Logic: '<S101>/AND'
   *  RelationalOperator: '<S26>/Lower Test'
   *  RelationalOperator: '<S26>/Upper Test'
   */
  rtb_FixPtRelationalOperator_fishi4l242_tmp = (((*rtu_Pilot_throttle_ch) >=
    -0.2) && ((*rtu_Pilot_throttle_ch) <= 0.2));

  /* Logic: '<S23>/Logical Operator2' incorporates:
   *  Logic: '<S26>/AND'
   */
  rtb_FixPtRelationalOperator_lyd2dqurri =
    (rtb_FixPtRelationalOperator_fishi4l242_tmp || (*rtu_bGPSLossFlag));

  /* RelationalOperator: '<S21>/Compare' incorporates:
   *  Constant: '<S21>/Constant'
   */
  rtb_Compare_i5bisll1rb = ((*rtu_mode_data_eFWLifter_Mode) == OFF);

  /* Delay: '<S23>/Delay1' */
  FW_TECS_switcher_DW.icLoad_ogna325jjd = ((rtb_Compare_i5bisll1rb &&
    (((uint32_T)FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE_pzkzowkb1n) !=
     POS_ZCSIG)) || (FW_TECS_switcher_DW.icLoad_ogna325jjd));
  FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE_pzkzowkb1n = rtb_Compare_i5bisll1rb ?
    ((ZCSigState)1) : ((ZCSigState)0);
  if (FW_TECS_switcher_DW.icLoad_ogna325jjd) {
    FW_TECS_switcher_DW.Delay1_DSTATE_n1as2selwq = rtu_Sensor_pos_lla[2];
  }

  /* Switch: '<S23>/Switch' incorporates:
   *  Delay: '<S23>/Delay1'
   *  Gain: '<S23>/Gain'
   *  Gain: '<S23>/velD_to_hdot'
   *  Logic: '<S23>/Logical Operator1'
   *  Sum: '<S23>/Sum1'
   */
  if (!rtb_FixPtRelationalOperator_lyd2dqurri) {
    rtb_TrigonometricFunction6_onwcg4eh52 = (1.5 * (-rtu_Sensor_vel_ned[2])) +
      rtu_Sensor_pos_lla[2];
  } else {
    rtb_TrigonometricFunction6_onwcg4eh52 =
      FW_TECS_switcher_DW.Delay1_DSTATE_n1as2selwq;
  }

  /* End of Switch: '<S23>/Switch' */

  /* Saturate: '<S22>/Saturation' */
  if ((*rtu_Sensor_gspd) >= 1.0) {
    rtb_TrigonometricFunction_paar0fmbpe = *rtu_Sensor_gspd;
  } else {
    rtb_TrigonometricFunction_paar0fmbpe = 1.0;
  }

  /* End of Saturate: '<S22>/Saturation' */

  /* Logic: '<S22>/Logical Operator1' incorporates:
   *  Constant: '<S24>/Lower Limit'
   *  Constant: '<S24>/Upper Limit'
   *  Logic: '<S24>/AND'
   *  RelationalOperator: '<S24>/Lower Test'
   *  RelationalOperator: '<S24>/Upper Test'
   */
  rtb_Compare_i5bisll1rb = (((*rtu_Pilot_roll_ch) < -0.2) ||
    ((*rtu_Pilot_roll_ch) > 0.2));

  /* Delay: '<S22>/Delay' */
  if (FW_TECS_switcher_DW.icLoad_cnbza4ewej) {
    FW_TECS_switcher_DW.Delay_DSTATE = *rtu_Sensor_chi;
  }

  /* Switch: '<S22>/Switch' incorporates:
   *  Constant: '<S22>/Constant2'
   *  Constant: '<S3>/Constant2'
   *  Delay: '<S22>/Delay'
   *  Gain: '<S22>/Gain'
   *  Logic: '<S22>/Logical Operator'
   *  Product: '<S22>/Product2'
   *  RelationalOperator: '<S3>/Equal'
   *  Sum: '<S22>/Sum2'
   *  Trigonometry: '<S22>/Trigonometric Function1'
   */
  if (rtb_Compare_i5bisll1rb || ((*rtu_vom_status) != FLTDIR)) {
    rtb_Sqrt_jug5wuieqd = (2.0 * ((tan(rtu_Sensor_eul_ang[0]) * 9.80665) /
      rtb_TrigonometricFunction_paar0fmbpe)) + (*rtu_Sensor_chi);
  } else {
    rtb_Sqrt_jug5wuieqd = FW_TECS_switcher_DW.Delay_DSTATE;
  }

  /* End of Switch: '<S22>/Switch' */

  /* Switch: '<S3>/Switch1' incorporates:
   *  Switch: '<S22>/Switch1'
   */
  if (*rtu_bGPSLossFlag) {
    /* Switch: '<S3>/Switch2' */
    if (*rtu_bTelemtryLinkLoss1sec) {
      /* BusAssignment: '<S3>/Bus Assignment2' incorporates:
       *  Constant: '<S3>/Constant1'
       */
      PB_norm = -0.43633231299858238;
    } else {
      /* BusAssignment: '<S3>/Bus Assignment2' incorporates:
       *  Gain: '<S3>/Gain2'
       */
      PB_norm = 0.5236 * (*rtu_Pilot_roll_ch);
    }

    /* End of Switch: '<S3>/Switch2' */
  } else {
    if (rtb_Compare_i5bisll1rb) {
      /* Switch: '<S22>/Switch1' incorporates:
       *  Constant: '<S22>/Constant'
       *  Gain: '<S22>/Gain2'
       *  Product: '<S22>/Product'
       *  Trigonometry: '<S22>/Trigonometric Function'
       */
      rtb_Product1_bkygj4a2su = (tan(0.41888 * (*rtu_Pilot_roll_ch)) * 9.80665) /
        rtb_TrigonometricFunction_paar0fmbpe;
    } else {
      /* Switch: '<S22>/Switch1' incorporates:
       *  Constant: '<S25>/Constant2'
       *  Gain: '<S22>/Kp_chi'
       *  Math: '<S25>/Mod2'
       *  Math: '<S25>/Mod3'
       *  Sum: '<S22>/Sum'
       *  Sum: '<S25>/Add3'
       *  Sum: '<S25>/Add4'
       *  Sum: '<S25>/Sum'
       */
      rtb_Product1_bkygj4a2su = 0.4 * (rt_modd(rt_modd(((rtb_Sqrt_jug5wuieqd - (*
        rtu_Sensor_chi)) - -3.1415926535897931) + FW_TECS_switcher_ConstB.Sum1,
        FW_TECS_switcher_ConstB.Sum1), FW_TECS_switcher_ConstB.Sum1) -
        3.1415926535897931);
    }

    /* Product: '<S22>/Product1' incorporates:
     *  Constant: '<S22>/Constant1'
     */
    PB_norm = (rtb_Product1_bkygj4a2su * rtb_TrigonometricFunction_paar0fmbpe) /
      9.80665;

    /* Saturate: '<S3>/Saturation2' */
    if (PB_norm > 0.5236) {
      /* BusAssignment: '<S3>/Bus Assignment2' */
      PB_norm = 0.5236;
    } else if (PB_norm < -0.5236) {
      /* BusAssignment: '<S3>/Bus Assignment2' */
      PB_norm = -0.5236;
    } else {
      /* no actions */
    }

    /* End of Saturate: '<S3>/Saturation2' */
  }

  /* End of Switch: '<S3>/Switch1' */

  /* SignalConversion generated from: '<S4>/Vector Concatenate1' */
  rtb_VectorConcatenate1[0] = *rtu_mode_data_ft_data_FT_x;

  /* SignalConversion generated from: '<S4>/Vector Concatenate1' */
  rtb_VectorConcatenate1[1] = *rtu_mode_data_ft_data_FT_y;

  /* SignalConversion generated from: '<S4>/Vector Concatenate1' */
  rtb_VectorConcatenate1[2] = *rtu_mode_data_ft_data_FT_Altitude;

  /* MATLAB Function: '<S31>/MATLAB Function1' */
  FW_TECS_switcher_illvnqr3hx(rtb_VectorConcatenate1, rtb_C_pex2miccv0);

  /* Trigonometry: '<S33>/Trigonometric Function' */
  rtb_TrigonometricFunction_paar0fmbpe = sin(*rtu_mode_data_ft_data_FT_x);

  /* Product: '<S37>/Product' incorporates:
   *  Constant: '<S37>/e'
   */
  rtb_Product1_bkygj4a2su = rtb_TrigonometricFunction_paar0fmbpe *
    0.0818191908425;

  /* Sqrt: '<S37>/Sqrt' incorporates:
   *  Constant: '<S37>/const'
   *  Math: '<S37>/Square'
   *  Sum: '<S37>/Add'
   */
  rtb_Product1_bkygj4a2su = sqrt(1.0 - (rtb_Product1_bkygj4a2su *
    rtb_Product1_bkygj4a2su));

  /* Switch: '<S38>/Switch3' incorporates:
   *  Abs: '<S38>/Abs'
   *  Product: '<S38>/Product'
   */
  if (rtb_Product1_bkygj4a2su <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S38>/Switch3' */

  /* Product: '<S37>/Divide' incorporates:
   *  Constant: '<S37>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Product: '<S33>/Product' incorporates:
   *  Product: '<S33>/Product1'
   *  Sum: '<S33>/Sum'
   *  Trigonometry: '<S33>/Trigonometric Function1'
   */
  dir_idx_1 = (rtb_Product1_bkygj4a2su + (*rtu_mode_data_ft_data_FT_Altitude)) *
    cos(*rtu_mode_data_ft_data_FT_x);

  /* Product: '<S33>/Product3' incorporates:
   *  Constant: '<S33>/1-e^2'
   *  Product: '<S33>/Product2'
   *  Sum: '<S33>/Sum1'
   */
  dir_idx_0 = rtb_TrigonometricFunction_paar0fmbpe * ((0.993305620009879 *
    rtb_Product1_bkygj4a2su) + (*rtu_mode_data_ft_data_FT_Altitude));

  /* Sqrt: '<S35>/Sqrt' */
  rtb_Product1_bkygj4a2su = AC_norm_tmp;

  /* Switch: '<S36>/Switch3' incorporates:
   *  Product: '<S36>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S36>/Switch3' */

  /* Product: '<S35>/Divide' incorporates:
   *  Constant: '<S35>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Product: '<S32>/Product' incorporates:
   *  Product: '<S32>/Product1'
   *  Sum: '<S32>/Sum'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (rtb_Product1_bkygj4a2su +
    rtu_Sensor_pos_lla[2]) * rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S29>/Sum' incorporates:
   *  Constant: '<S32>/1-e^2'
   *  Product: '<S32>/Product'
   *  Product: '<S32>/Product1'
   *  Product: '<S32>/Product2'
   *  Product: '<S32>/Product3'
   *  Product: '<S33>/Product'
   *  Product: '<S33>/Product1'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S32>/Sum1'
   *  Trigonometry: '<S33>/Trigonometric Function2'
   *  Trigonometry: '<S33>/Trigonometric Function3'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    (dir_idx_1 * cos(*rtu_mode_data_ft_data_FT_y));
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 *
               rtb_Sum_imgtnbczew_tmp_0) - (dir_idx_1 * sin
    (*rtu_mode_data_ft_data_FT_y));
  AC_norm = (curvature * ((0.993305620009879 * rtb_Product1_bkygj4a2su) +
              rtu_Sensor_pos_lla[2])) - dir_idx_0;

  /* Product: '<S31>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S31>/Product' */
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* End of Product: '<S31>/Product' */

  /* MATLAB Function: '<S28>/guidance_line' incorporates:
   *  SignalConversion generated from: '<S28>/Vector Concatenate1'
   * */
  /* MATLAB Function 'FTransition/L1_guidance/guidance_line': '<S30>:1' */
  /*  straight line L1-guidance */
  /*  start_pos         [x; y] (m) */
  /*  start_dir_ang     direction from North (rad) */
  /*  cur_pos           [x; y] (m) */
  /*  Vg                [vx, vy] ground-speed (m/s) */
  /*  lat_ax            (m/s/s) lateral acceleration command */
  /* '<S30>:1:11' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  rtb_TrigonometricFunction_paar0fmbpe = rtb_TrigonometricFunction2_i3ecl5ucim;

  /*  horizontal ground speed */
  /* '<S30>:1:13' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S30>:1:14' Vg =0.001; */
    rtb_TrigonometricFunction_paar0fmbpe = 0.001;
  }

  /* '<S30>:1:17' vel_dir = vel_NE/Vg; */
  /* '<S30>:1:18' L1_P = 15; */
  /*  L1 peroid (s) */
  /* '<S30>:1:19' L1_zeta = 0.7; */
  /*  L1 damping ratio */
  /* '<S30>:1:20' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Product1_bkygj4a2su = 3.3422538049298023 *
    rtb_TrigonometricFunction_paar0fmbpe;

  /*  L1 length */
  /* '<S30>:1:22' P = start_pos; */
  /* '<S30>:1:23' dir = [cos(start_dir_ang); sin(start_dir_ang)]; */
  dir_idx_0 = cos(*rtu_mode_data_ft_data_FT_Heading);
  dir_idx_1 = sin(*rtu_mode_data_ft_data_FT_Heading);

  /* '<S30>:1:24' A = cur_pos; */
  /* '<S30>:1:26' PA = A - P; */
  /* '<S30>:1:27' C = P + dir*dot(PA, dir); */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 = rtu_Sensor_vel_ned[0] /
    rtb_TrigonometricFunction_paar0fmbpe;
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = rtu_Sensor_vel_ned[1] /
    rtb_TrigonometricFunction_paar0fmbpe;
  AC_norm = (rtb_VectorConcatenate1[0] * dir_idx_0) + (rtb_VectorConcatenate1[1]
    * dir_idx_1);

  /* '<S30>:1:28' AC = C - A; */
  absxk = dir_idx_0 * AC_norm;
  C_idx_0 = absxk;
  AC_idx_0_0 = absxk - rtb_VectorConcatenate1[0];
  absxk = dir_idx_1 * AC_norm;
  AC_idx_1_0 = absxk - rtb_VectorConcatenate1[1];

  /* '<S30>:1:29' AC_norm = sqrt(AC(1)^2 + AC(2)^2); */
  AC_norm = sqrt((AC_idx_0_0 * AC_idx_0_0) + (AC_idx_1_0 * AC_idx_1_0));

  /* '<S30>:1:31' if(AC_norm < L1) */
  if (AC_norm < rtb_Product1_bkygj4a2su) {
    /* '<S30>:1:32' CB_norm = sqrt(L1^2 - AC_norm^2); */
    AC_norm = sqrt((rtb_Product1_bkygj4a2su * rtb_Product1_bkygj4a2su) -
                   (AC_norm * AC_norm));
  } else {
    /* '<S30>:1:33' else */
    /* '<S30>:1:34' CB_norm = 0; */
    AC_norm = 0.0;
  }

  /* '<S30>:1:37' B = C + dir*(CB_norm); */
  /* '<S30>:1:38' AB_dir = (B-A)/norm(B-A); */
  AC_idx_0_0 = (C_idx_0 + (dir_idx_0 * AC_norm)) - rtb_VectorConcatenate1[0];
  AC_idx_1_0 = (absxk + (dir_idx_1 * AC_norm)) - rtb_VectorConcatenate1[1];
  scale = 3.3121686421112381E-170;
  absxk = fabs(AC_idx_0_0);
  if (absxk > 3.3121686421112381E-170) {
    AC_norm = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    AC_norm = t * t;
  }

  absxk = fabs(AC_idx_1_0);
  if (absxk > scale) {
    t = scale / absxk;
    AC_norm = ((AC_norm * t) * t) + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    AC_norm += t * t;
  }

  AC_norm = scale * sqrt(AC_norm);
  AC_idx_0_0 /= AC_norm;
  AC_idx_1_0 /= AC_norm;

  /* Switch: '<S4>/Switch2' incorporates:
   *  Constant: '<S4>/Constant4'
   *  Switch: '<S5>/Switch2'
   */
  /* '<S30>:1:40' sin_eta = vel_dir(1)*AB_dir(2) - vel_dir(2)*AB_dir(1); */
  /*  2D cross product */
  /* '<S30>:1:41' cos_eta = vel_dir(1)*AB_dir(1) + vel_dir(2)*AB_dir(2); */
  /*  2D dot product */
  /* '<S30>:1:42' eta = atan2(sin_eta, cos_eta); */
  /* '<S30>:1:43' eta = max(eta, -pi/2); */
  /* '<S30>:1:44' eta = min(eta,  pi/2); */
  /* '<S30>:1:46' path_done = false; */
  /* '<S30>:1:47' PB = B - P; */
  /* '<S30>:1:48' PB_norm = sqrt(PB(1)^2 + PB(2)^2); */
  /* '<S30>:1:50' if sign(dot(PB,dir))*PB_norm > path_len */
  /* '<S30>:1:54' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  if (*rtu_bGPSLossFlag) {
    rtb_TrigonometricFunction_paar0fmbpe = 0.0;

    /* BusAssignment: '<S5>/Bus Assignment2' incorporates:
     *  Constant: '<S4>/Constant4'
     *  Constant: '<S5>/Constant9'
     */
    t = -0.43633231299858238;
  } else {
    /* MATLAB Function: '<S28>/guidance_line' */
    absxk = atan2((rtb_VectorConcatenate1_psilb5rrki_idx_0 * AC_idx_1_0) -
                  (AC_idx_0_0 * rtb_VectorConcatenate1_psilb5rrki_idx_1),
                  (rtb_VectorConcatenate1_psilb5rrki_idx_0 * AC_idx_0_0) +
                  (rtb_VectorConcatenate1_psilb5rrki_idx_1 * AC_idx_1_0));
    if (absxk < -1.5707963267948966) {
      absxk = -1.5707963267948966;
    }

    if (absxk > 1.5707963267948966) {
      absxk = 1.5707963267948966;
    }

    /* Trigonometry: '<S28>/Atan' incorporates:
     *  MATLAB Function: '<S28>/guidance_line'
     */
    rtb_TrigonometricFunction_paar0fmbpe = atan2(((1.9599999999999997 *
      (rtb_TrigonometricFunction_paar0fmbpe *
       rtb_TrigonometricFunction_paar0fmbpe)) / rtb_Product1_bkygj4a2su) * sin
      (absxk), 9.81);

    /* Saturate: '<S4>/Saturation2' */
    if (rtb_TrigonometricFunction_paar0fmbpe > 0.5236) {
      rtb_TrigonometricFunction_paar0fmbpe = 0.5236;
    } else if (rtb_TrigonometricFunction_paar0fmbpe < -0.5236) {
      rtb_TrigonometricFunction_paar0fmbpe = -0.5236;
    } else {
      /* no actions */
    }

    /* End of Saturate: '<S4>/Saturation2' */

    /* MATLAB Function: '<S39>/MATLAB Function' */
    if (rtb_Sum_imgtnbczew_idx_1 < -3.1415926535897931) {
      rtb_Sum_imgtnbczew_idx_1 = -3.1415926535897931;
    }

    if (rtb_Sum_imgtnbczew_idx_1 > 3.1415926535897931) {
      rtb_Sum_imgtnbczew_idx_1 = 3.1415926535897931;
    }

    /* Trigonometry: '<S39>/Atan' incorporates:
     *  MATLAB Function: '<S39>/MATLAB Function'
     */
    t = atan2(((1.9599999999999997 * (Vg * Vg)) / rtb_Sqrt) * sin
              (rtb_Sum_imgtnbczew_idx_1), 9.81);

    /* Saturate: '<S5>/Saturation2' */
    if (t > 0.5236) {
      /* BusAssignment: '<S5>/Bus Assignment2' */
      t = 0.5236;
    } else if (t < -0.5236) {
      /* BusAssignment: '<S5>/Bus Assignment2' */
      t = -0.5236;
    } else {
      /* no actions */
    }

    /* End of Saturate: '<S5>/Saturation2' */
  }

  /* End of Switch: '<S4>/Switch2' */

  /* SignalConversion generated from: '<S6>/Vector Concatenate1' */
  rtb_VectorConcatenate1[0] = *rtu_mode_data_loiter_data_loiter_Center_Lat;

  /* SignalConversion generated from: '<S6>/Vector Concatenate1' */
  rtb_VectorConcatenate1[1] = *rtu_mode_data_loiter_data_loiter_Center_Lon;

  /* SignalConversion generated from: '<S6>/Vector Concatenate1' */
  rtb_VectorConcatenate1[2] = *rtu_mode_data_loiter_data_loiter_altitude;

  /* MATLAB Function: '<S81>/MATLAB Function1' */
  FW_TECS_switcher_illvnqr3hx(rtb_VectorConcatenate1, rtb_C_pex2miccv0);

  /* Trigonometry: '<S83>/Trigonometric Function' */
  rtb_Product1_bkygj4a2su = sin(*rtu_mode_data_loiter_data_loiter_Center_Lat);

  /* Product: '<S87>/Product' incorporates:
   *  Constant: '<S87>/e'
   */
  AC_norm = rtb_Product1_bkygj4a2su * 0.0818191908425;

  /* Sqrt: '<S87>/Sqrt' incorporates:
   *  Constant: '<S87>/const'
   *  Math: '<S87>/Square'
   *  Sum: '<S87>/Add'
   */
  AC_norm = sqrt(1.0 - (AC_norm * AC_norm));

  /* Switch: '<S88>/Switch3' incorporates:
   *  Abs: '<S88>/Abs'
   *  Product: '<S88>/Product'
   */
  if (AC_norm <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S88>/Switch3' */

  /* Product: '<S87>/Divide' incorporates:
   *  Constant: '<S87>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* Product: '<S83>/Product' incorporates:
   *  Product: '<S83>/Product1'
   *  Sum: '<S83>/Sum'
   *  Trigonometry: '<S83>/Trigonometric Function1'
   */
  dir_idx_1 = (AC_norm + (*rtu_mode_data_loiter_data_loiter_altitude)) * cos
    (*rtu_mode_data_loiter_data_loiter_Center_Lat);

  /* Product: '<S83>/Product3' incorporates:
   *  Constant: '<S83>/1-e^2'
   *  Product: '<S83>/Product2'
   *  Sum: '<S83>/Sum1'
   */
  dir_idx_0 = rtb_Product1_bkygj4a2su * ((0.993305620009879 * AC_norm) +
    (*rtu_mode_data_loiter_data_loiter_altitude));

  /* Sqrt: '<S85>/Sqrt' */
  AC_norm = AC_norm_tmp;

  /* Switch: '<S86>/Switch3' incorporates:
   *  Product: '<S86>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S86>/Switch3' */

  /* Product: '<S85>/Divide' incorporates:
   *  Constant: '<S85>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* Product: '<S82>/Product' incorporates:
   *  Product: '<S82>/Product1'
   *  Sum: '<S82>/Sum'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (AC_norm + rtu_Sensor_pos_lla[2]) *
    rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S80>/Sum' incorporates:
   *  Constant: '<S82>/1-e^2'
   *  Product: '<S82>/Product'
   *  Product: '<S82>/Product1'
   *  Product: '<S82>/Product2'
   *  Product: '<S82>/Product3'
   *  Product: '<S83>/Product'
   *  Product: '<S83>/Product1'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S82>/Sum1'
   *  Trigonometry: '<S83>/Trigonometric Function2'
   *  Trigonometry: '<S83>/Trigonometric Function3'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    (dir_idx_1 * cos(*rtu_mode_data_loiter_data_loiter_Center_Lon));
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 *
               rtb_Sum_imgtnbczew_tmp_0) - (dir_idx_1 * sin
    (*rtu_mode_data_loiter_data_loiter_Center_Lon));
  AC_norm = (curvature * ((0.993305620009879 * AC_norm) + rtu_Sensor_pos_lla[2]))
    - dir_idx_0;

  /* Product: '<S81>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S81>/Product' */
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* End of Product: '<S81>/Product' */

  /* MATLAB Function: '<S78>/MATLAB Function1' incorporates:
   *  SignalConversion generated from: '<S78>/Vector Concatenate1'
   * */
  rtb_Sum_imgtnbczew_idx_1 = *rtu_mode_data_loiter_data_loiter_radius;

  /* MATLAB Function 'LOITER/L1_guidance/MATLAB Function1': '<S79>:1' */
  /*  circle L1-guidance */
  /*  start_pos     [x; y] (m) */
  /*  direction     +1 CW, -1 CCW as seen from top */
  /*  cur_pos       [x; y] (m) */
  /*  vel_NE        [vx, vy] ground-speed (m/s) */
  /*  R             Turn Radius (m) */
  /*  R_min         minimum Radius (m) */
  /*  lat_ax        (m/s/s) lateral acceleration command */
  /* '<S79>:1:13' R_min = 100; */
  /* '<S79>:1:15' if R < R_min */
  if ((*rtu_mode_data_loiter_data_loiter_radius) < 100.0) {
    /* '<S79>:1:16' R = R_min; */
    rtb_Sum_imgtnbczew_idx_1 = 100.0;
  }

  /* '<S79>:1:18' curvature = direction*1/R; */
  /* '<S79>:1:20' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  Vg = rtb_TrigonometricFunction2_i3ecl5ucim;

  /*  horizontal ground speed */
  /* '<S79>:1:22' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S79>:1:23' Vg =0.001; */
    Vg = 0.001;
  }

  /* '<S79>:1:25' vel_dir = vel_NE/Vg; */
  /* '<S79>:1:26' chi = atan2(vel_dir(2), vel_dir(1)); */
  /* '<S79>:1:28' L1_P = 12; */
  /*  L1 peroid (s) */
  /* '<S79>:1:29' L1_zeta = 0.7; */
  /*  L1 samping ratio */
  /* '<S79>:1:30' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Sqrt = 2.6738030439438414 * Vg;

  /*  L1 length */
  /*  P = start_pos; */
  /*  dir = start_dir; */
  /* '<S79>:1:34' A = cur_pos; */
  /*  find center of circle O */
  /*  O = center_pos; %P + sign(curvature)*R*[-dir(2); dir(1)]; */
  /* '<S79>:1:39' OA = A; */
  /*  - O; */
  /* '<S79>:1:40' a = sqrt(OA(1)^2 + OA(2)^2); */
  AC_norm = sqrt((rtb_VectorConcatenate1[0] * rtb_VectorConcatenate1[0]) +
                 (rtb_VectorConcatenate1[1] * rtb_VectorConcatenate1[1]));

  /* '<S79>:1:42' if (L1 + a) <= R */
  if ((rtb_Sqrt + AC_norm) <= rtb_Sum_imgtnbczew_idx_1) {
    /* '<S79>:1:43' gamma = pi; */
    AC_norm = 3.1415926535897931;
  } else if (AC_norm >= (rtb_Sqrt + rtb_Sum_imgtnbczew_idx_1)) {
    /* '<S79>:1:44' elseif a >= (L1 + R) */
    /* '<S79>:1:45' gamma = 0; */
    AC_norm = 0.0;
  } else {
    /* '<S79>:1:46' else */
    /* '<S79>:1:47' temp1 = (L1^2 + a^2 - R^2)/(2*L1*a); */
    /*  here a != 0, if condition-1 covers a=0 case */
    /* '<S79>:1:48' temp1 = max(temp1, -1); */
    /* '<S79>:1:49' temp1 = min(temp1, 1); */
    /* '<S79>:1:50' gamma = acos(temp1); */
    rtb_VectorConcatenate1_psilb5rrki_idx_0 = (((rtb_Sqrt * rtb_Sqrt) + (AC_norm
      * AC_norm)) - (rtb_Sum_imgtnbczew_idx_1 * rtb_Sum_imgtnbczew_idx_1)) /
      ((2.0 * rtb_Sqrt) * AC_norm);
    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 < -1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = -1.0;
    }

    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 > 1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = 1.0;
    }

    AC_norm = acos(rtb_VectorConcatenate1_psilb5rrki_idx_0);
  }

  /* '<S79>:1:53' chi_ao = atan2(-OA(2), -OA(1)); */
  /* '<S79>:1:54' chi_L = wrap_pi(chi_ao - sign(curvature)*gamma); */
  rtb_Product1_bkygj4a2su = (*rtu_mode_data_loiter_data_loiter_direction) /
    rtb_Sum_imgtnbczew_idx_1;
  if (rtb_Product1_bkygj4a2su < 0.0) {
    i = -1;
  } else {
    i = (rtb_Product1_bkygj4a2su > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  AC_norm = atan2(-rtb_VectorConcatenate1[1], -rtb_VectorConcatenate1[0]) -
    (((real_T)i) * AC_norm);

  /* '<S79>:1:69' if ang_in > pi */
  if (AC_norm > 3.1415926535897931) {
    /* '<S79>:1:70' ang_in = ang_in - 2*pi; */
    AC_norm -= 6.2831853071795862;
  } else if (AC_norm < -3.1415926535897931) {
    /* '<S79>:1:71' elseif ang_in < -pi */
    /* '<S79>:1:72' ang_in = ang_in + 2*pi; */
    AC_norm += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* '<S79>:1:75' ang_out = ang_in; */
  /* '<S79>:1:56' eta = wrap_pi(chi_L - chi); */
  rtb_Sum_imgtnbczew_idx_1 = AC_norm - atan2(rtu_Sensor_vel_ned[1] / Vg,
    rtu_Sensor_vel_ned[0] / Vg);

  /* '<S79>:1:69' if ang_in > pi */
  if (rtb_Sum_imgtnbczew_idx_1 > 3.1415926535897931) {
    /* '<S79>:1:70' ang_in = ang_in - 2*pi; */
    rtb_Sum_imgtnbczew_idx_1 -= 6.2831853071795862;
  } else if (rtb_Sum_imgtnbczew_idx_1 < -3.1415926535897931) {
    /* '<S79>:1:71' elseif ang_in < -pi */
    /* '<S79>:1:72' ang_in = ang_in + 2*pi; */
    rtb_Sum_imgtnbczew_idx_1 += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* Switch: '<S6>/Switch2' incorporates:
   *  MATLAB Function: '<S78>/MATLAB Function1'
   */
  /* '<S79>:1:75' ang_out = ang_in; */
  /* '<S79>:1:57' eta = max(eta, -pi); */
  /* '<S79>:1:58' eta = min(eta, pi); */
  /* '<S79>:1:60' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  /* '<S79>:1:62' L1_angle = chi_L; */
  if (*rtu_bGPSLossFlag) {
    /* BusAssignment: '<S6>/Bus Assignment2' incorporates:
     *  Constant: '<S6>/Constant4'
     */
    rtb_Product1_bkygj4a2su = -0.43633231299858238;
  } else {
    if (rtb_Sum_imgtnbczew_idx_1 < -3.1415926535897931) {
      /* MATLAB Function: '<S78>/MATLAB Function1' */
      rtb_Sum_imgtnbczew_idx_1 = -3.1415926535897931;
    }

    /* MATLAB Function: '<S78>/MATLAB Function1' */
    if (rtb_Sum_imgtnbczew_idx_1 > 3.1415926535897931) {
      rtb_Sum_imgtnbczew_idx_1 = 3.1415926535897931;
    }

    /* Trigonometry: '<S78>/Atan' incorporates:
     *  MATLAB Function: '<S78>/MATLAB Function1'
     */
    rtb_Product1_bkygj4a2su = atan2(((1.9599999999999997 * (Vg * Vg)) / rtb_Sqrt)
      * sin(rtb_Sum_imgtnbczew_idx_1), 9.81);

    /* Saturate: '<S6>/Saturation2' */
    if (rtb_Product1_bkygj4a2su > 0.5236) {
      /* BusAssignment: '<S6>/Bus Assignment2' */
      rtb_Product1_bkygj4a2su = 0.5236;
    } else if (rtb_Product1_bkygj4a2su < -0.5236) {
      /* BusAssignment: '<S6>/Bus Assignment2' */
      rtb_Product1_bkygj4a2su = -0.5236;
    } else {
      /* no actions */
    }

    /* End of Saturate: '<S6>/Saturation2' */
  }

  /* End of Switch: '<S6>/Switch2' */

  /* Delay: '<S90>/Delay1' */
  FW_TECS_switcher_DW.icLoad_kbyaompepr = ((((*rtu_TECS_mode) > 0U) &&
    (((uint32_T)FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE_okp35hdmnj) !=
     POS_ZCSIG)) || (FW_TECS_switcher_DW.icLoad_kbyaompepr));
  FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE_okp35hdmnj = (ZCSigState)
    (((*rtu_TECS_mode) > 0U) ? ((int32_T)1) : ((int32_T)0));
  if (FW_TECS_switcher_DW.icLoad_kbyaompepr) {
    FW_TECS_switcher_DW.Delay1_DSTATE_g1oevxsf0d = rtu_Sensor_pos_lla[2];
  }

  /* Switch: '<S90>/Switch' incorporates:
   *  Delay: '<S90>/Delay1'
   *  Gain: '<S90>/Gain'
   *  Gain: '<S90>/velD_to_hdot'
   *  Logic: '<S90>/Logical Operator1'
   *  Sum: '<S90>/Sum1'
   */
  if (!rtb_FixPtRelationalOperator_fishi4l242_tmp) {
    absxk = (1.5 * (-rtu_Sensor_vel_ned[2])) + rtu_Sensor_pos_lla[2];
  } else {
    absxk = FW_TECS_switcher_DW.Delay1_DSTATE_g1oevxsf0d;
  }

  /* End of Switch: '<S90>/Switch' */

  /* Sqrt: '<S97>/Sqrt' */
  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S96>:1' */
  /* '<S96>:1:6' cos_lat = cos(LLA(1)); */
  /* '<S96>:1:7' sin_lat = sin(LLA(1)); */
  /* '<S96>:1:8' cos_long = cos(LLA(2)); */
  /* '<S96>:1:9' sin_long = sin(LLA(2)); */
  /* '<S96>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S96>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S96>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  AC_norm = AC_norm_tmp;

  /* Switch: '<S98>/Switch3' incorporates:
   *  Product: '<S98>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    AC_norm = -0.001;
  }

  /* End of Switch: '<S98>/Switch3' */

  /* Product: '<S97>/Divide' incorporates:
   *  Constant: '<S97>/R_0'
   */
  AC_norm = 6.378137E+6 / AC_norm;

  /* MATLAB Function: '<S93>/MATLAB Function1' */
  rtb_C_pex2miccv0[0] = -0.24088103093692706;
  rtb_C_pex2miccv0[3] = -0.34193153916516583;
  rtb_C_pex2miccv0[6] = 0.90832766745206206;
  rtb_C_pex2miccv0[1] = -0.81751089051303827;
  rtb_C_pex2miccv0[4] = 0.57591313919078047;
  rtb_C_pex2miccv0[7] = 0.0;
  rtb_C_pex2miccv0[2] = -0.52311783837615633;
  rtb_C_pex2miccv0[5] = -0.74256776029636617;
  rtb_C_pex2miccv0[8] = -0.41825930777580567;

  /* Product: '<S94>/Product' incorporates:
   *  Product: '<S94>/Product1'
   *  Sum: '<S94>/Sum'
   */
  dir_idx_1 = (AC_norm + rtu_Sensor_pos_lla[2]) *
    rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S92>/Sum' incorporates:
   *  Concatenate: '<S95>/Vector Concatenate'
   *  Constant: '<S94>/1-e^2'
   *  Product: '<S94>/Product'
   *  Product: '<S94>/Product1'
   *  Product: '<S94>/Product2'
   *  Product: '<S94>/Product3'
   *  Sum: '<S109>/Sum'
   *  Sum: '<S94>/Sum1'
   */
  Vg = (dir_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    FW_TECS_switcher_ConstB.VectorConcatenate[0];
  rtb_Sum_imgtnbczew_idx_1 = (dir_idx_1 * rtb_Sum_imgtnbczew_tmp_0) -
    FW_TECS_switcher_ConstB.VectorConcatenate[1];
  dir_idx_0 = (curvature * ((0.993305620009879 * AC_norm) + rtu_Sensor_pos_lla[2]))
    - FW_TECS_switcher_ConstB.VectorConcatenate[2];

  /* Product: '<S93>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S93>/Product' */
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] * Vg) +
      (rtb_C_pex2miccv0[i + 3] * rtb_Sum_imgtnbczew_idx_1)) +
      (rtb_C_pex2miccv0[i + 6] * dir_idx_0);
  }

  /* End of Product: '<S93>/Product' */

  /* MATLAB Function: '<S89>/MATLAB Function' incorporates:
   *  SignalConversion generated from: '<S89>/Vector Concatenate1'
   * */
  /* MATLAB Function 'UMAN/L1_guidance/MATLAB Function': '<S91>:1' */
  /*  circle L1-guidance */
  /*  start_pos     [x; y] (m) */
  /*  direction     [dx; dy] (unit vector) */
  /*  cur_pos       [x; y] (m) */
  /*  Vg            [vx, vy] ground-speed (m/s) */
  /*  R             Turn Radius (m) */
  /*  R_min         minimum Radius (m) */
  /*  lat_ax        (m/s/s) lateral acceleration command */
  /* '<S91>:1:13' R_min = 20; */
  /* '<S91>:1:15' if abs(R) < R_min */
  /* '<S91>:1:18' curvature = 1/R; */
  /* '<S91>:1:20' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  Vg = rtb_TrigonometricFunction2_i3ecl5ucim;

  /*  horizontal ground speed */
  /* '<S91>:1:22' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S91>:1:23' Vg =0.001; */
    Vg = 0.001;
  }

  /* '<S91>:1:25' vel_dir = vel_NE/Vg; */
  /* '<S91>:1:26' chi = atan2(vel_dir(2), vel_dir(1)); */
  /* '<S91>:1:28' L1_P = 10; */
  /*  L1 peroid (s) */
  /* '<S91>:1:29' L1_zeta = 0.7; */
  /*  L1 samping ratio */
  /* '<S91>:1:30' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Sqrt = 2.228169203286535 * Vg;

  /*  L1 length */
  /*  P = start_pos; */
  /*  dir = start_dir; */
  /* '<S91>:1:34' A = cur_pos; */
  /*  find center of circle O */
  /*  O = center_pos; %P + sign(curvature)*R*[-dir(2); dir(1)]; */
  /* '<S91>:1:39' OA = A; */
  /*  - O; */
  /* '<S91>:1:40' a = sqrt(OA(1)^2 + OA(2)^2); */
  AC_norm = sqrt((rtb_VectorConcatenate1[0] * rtb_VectorConcatenate1[0]) +
                 (rtb_VectorConcatenate1[1] * rtb_VectorConcatenate1[1]));

  /* '<S91>:1:42' if (L1 + a) <= R */
  if ((rtb_Sqrt + AC_norm) <= 200.0) {
    /* '<S91>:1:43' gamma = pi; */
    AC_norm = 3.1415926535897931;
  } else if (AC_norm >= (rtb_Sqrt + 200.0)) {
    /* '<S91>:1:44' elseif a >= (L1 + R) */
    /* '<S91>:1:45' gamma = 0; */
    AC_norm = 0.0;
  } else {
    /* '<S91>:1:46' else */
    /* '<S91>:1:47' temp1 = (L1^2 + a^2 - R^2)/(2*L1*a); */
    /*  here a != 0, if condition-1 covers a=0 case */
    /* '<S91>:1:48' temp1 = max(temp1, -1); */
    /* '<S91>:1:49' temp1 = min(temp1, 1); */
    /* '<S91>:1:50' gamma = acos(temp1); */
    rtb_VectorConcatenate1_psilb5rrki_idx_0 = (((rtb_Sqrt * rtb_Sqrt) + (AC_norm
      * AC_norm)) - 40000.0) / ((2.0 * rtb_Sqrt) * AC_norm);
    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 < -1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = -1.0;
    }

    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 > 1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = 1.0;
    }

    AC_norm = acos(rtb_VectorConcatenate1_psilb5rrki_idx_0);
  }

  /* '<S91>:1:53' chi_ao = atan2(-OA(2), -OA(1)); */
  /* '<S91>:1:54' chi_L = wrap_pi(chi_ao - sign(curvature)*gamma); */
  AC_norm = atan2(-rtb_VectorConcatenate1[1], -rtb_VectorConcatenate1[0]) -
    AC_norm;

  /* '<S91>:1:67' if ang_in > pi */
  if (AC_norm > 3.1415926535897931) {
    /* '<S91>:1:68' ang_in = ang_in - 2*pi; */
    AC_norm -= 6.2831853071795862;
  } else if (AC_norm < -3.1415926535897931) {
    /* '<S91>:1:69' elseif ang_in < -pi */
    /* '<S91>:1:70' ang_in = ang_in + 2*pi; */
    AC_norm += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* '<S91>:1:73' ang_out = ang_in; */
  /* '<S91>:1:56' eta = wrap_pi(chi_L - chi); */
  rtb_Sum_imgtnbczew_idx_1 = AC_norm - atan2(rtu_Sensor_vel_ned[1] / Vg,
    rtu_Sensor_vel_ned[0] / Vg);

  /* '<S91>:1:67' if ang_in > pi */
  if (rtb_Sum_imgtnbczew_idx_1 > 3.1415926535897931) {
    /* '<S91>:1:68' ang_in = ang_in - 2*pi; */
    rtb_Sum_imgtnbczew_idx_1 -= 6.2831853071795862;
  } else if (rtb_Sum_imgtnbczew_idx_1 < -3.1415926535897931) {
    /* '<S91>:1:69' elseif ang_in < -pi */
    /* '<S91>:1:70' ang_in = ang_in + 2*pi; */
    rtb_Sum_imgtnbczew_idx_1 += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* Switch: '<S8>/Switch6' incorporates:
   *  UnitDelay: '<S8>/Unit Delay1'
   */
  /* '<S91>:1:73' ang_out = ang_in; */
  /* '<S91>:1:57' eta = max(eta, -pi); */
  /* '<S91>:1:58' eta = min(eta, pi); */
  /* '<S91>:1:60' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  if (*rtu_bGPSLossFlag) {
    scale = FW_TECS_switcher_DW.UnitDelay1_DSTATE;
  } else {
    scale = rtu_Sensor_pos_lla[2];
  }

  /* End of Switch: '<S8>/Switch6' */

  /* Switch: '<S8>/Switch4' incorporates:
   *  UnitDelay: '<S8>/Unit Delay2'
   */
  if (rtb_Compare_ag4sp0vjdy) {
    C_idx_0 = FW_TECS_switcher_DW.UnitDelay2_DSTATE;
  } else {
    C_idx_0 = rtu_Sensor_pos_lla[2];
  }

  /* End of Switch: '<S8>/Switch4' */

  /* Switch: '<S8>/Switch2' incorporates:
   *  Constant: '<S8>/Constant9'
   *  Switch: '<S8>/Switch'
   */
  if (*rtu_bGPSLossFlag) {
    AC_norm = -0.43633231299858238;
  } else {
    if (rtb_Compare_ag4sp0vjdy) {
      /* Switch: '<S8>/Switch' incorporates:
       *  Memory: '<S8>/Memory1'
       */
      AC_norm = FW_TECS_switcher_DW.Memory1_PreviousInput;
    } else {
      /* Switch: '<S8>/Switch' incorporates:
       *  Constant: '<S8>/Constant2'
       */
      AC_norm = 0.0;
    }

    /* Trigonometry: '<S8>/Atan2' incorporates:
     *  Switch: '<S8>/Switch'
     */
    AC_norm = atan2(AC_norm, 9.81);

    /* Saturate: '<S8>/Saturation1' */
    if (AC_norm > 0.52359877559829882) {
      AC_norm = 0.52359877559829882;
    } else if (AC_norm < -0.52359877559829882) {
      AC_norm = -0.52359877559829882;
    } else {
      /* no actions */
    }

    /* End of Saturate: '<S8>/Saturation1' */
  }

  /* End of Switch: '<S8>/Switch2' */

  /* MultiPortSwitch: '<Root>/Multiport Switch' incorporates:
   *  BusAssignment: '<S2>/Bus Assignment2'
   *  BusAssignment: '<S3>/Bus Assignment2'
   *  BusAssignment: '<S4>/Bus Assignment2'
   *  BusAssignment: '<S5>/Bus Assignment2'
   *  BusAssignment: '<S6>/Bus Assignment2'
   *  BusAssignment: '<S7>/Bus Assignment2'
   *  BusAssignment: '<S8>/Bus Assignment2'
   *  Constant: '<S23>/Constant'
   *  Constant: '<S27>/Constant'
   *  Constant: '<S2>/Constant'
   *  Constant: '<S2>/Constant1'
   *  Constant: '<S2>/Constant2'
   *  Constant: '<S3>/Constant3'
   *  Constant: '<S4>/Constant'
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S5>/Constant'
   *  Constant: '<S5>/Constant2'
   *  Constant: '<S5>/Constant3'
   *  Constant: '<S6>/Constant'
   *  Constant: '<S6>/Constant1'
   *  Constant: '<S6>/Constant3'
   *  Constant: '<S8>/Constant12'
   *  Constant: '<S8>/Constant3'
   *  Constant: '<S8>/Constant5'
   *  Constant: '<S90>/Constant'
   *  Constant: '<S9>/Constant'
   *  Logic: '<S7>/NOT'
   *  Product: '<S23>/Product'
   *  Product: '<S90>/Product'
   *  RelationalOperator: '<S27>/Compare'
   *  RelationalOperator: '<S9>/Compare'
   */
  switch (*rtu_vom_status) {
   case UMAN:
    rty_busFW_TECS_switcher->h_dot_cmd = 3.0 * (*rtu_Pilot_throttle_ch);
    rty_busFW_TECS_switcher->h_cmd = absxk;
    rty_busFW_TECS_switcher->V_cmd = *rtu_std_command_airspeed_cas_cmd;

    /* MATLAB Function: '<S89>/MATLAB Function' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment2'
     *  Constant: '<S90>/Constant'
     *  Product: '<S90>/Product'
     */
    if (rtb_Sum_imgtnbczew_idx_1 < -3.1415926535897931) {
      rtb_Sum_imgtnbczew_idx_1 = -3.1415926535897931;
    }

    if (rtb_Sum_imgtnbczew_idx_1 > 3.1415926535897931) {
      rtb_Sum_imgtnbczew_idx_1 = 3.1415926535897931;
    }

    /* Trigonometry: '<S89>/Atan' incorporates:
     *  MATLAB Function: '<S89>/MATLAB Function'
     */
    y = atan2(((1.9599999999999997 * (Vg * Vg)) / rtb_Sqrt) * sin
              (rtb_Sum_imgtnbczew_idx_1), 9.81);

    /* Saturate: '<S7>/Saturation2' */
    if (y > 0.5236) {
      rty_busFW_TECS_switcher->roll_cmd = 0.5236;
    } else if (y < -0.5236) {
      rty_busFW_TECS_switcher->roll_cmd = -0.5236;
    } else {
      rty_busFW_TECS_switcher->roll_cmd = y;
    }

    /* End of Saturate: '<S7>/Saturation2' */
    rty_busFW_TECS_switcher->TECS_reset = ((*rtu_TECS_mode) == 0U);
    rty_busFW_TECS_switcher->h_hold = rtb_FixPtRelationalOperator_fishi4l242_tmp;
    break;

   case F_TRANS:
    rty_busFW_TECS_switcher->h_dot_cmd = 0.0;
    rty_busFW_TECS_switcher->h_cmd = *rtu_mode_data_ft_data_FT_Altitude;
    rty_busFW_TECS_switcher->V_cmd = *rtu_mode_data_ft_data_FT_AirspeedRef;
    rty_busFW_TECS_switcher->roll_cmd = rtb_TrigonometricFunction_paar0fmbpe;
    rty_busFW_TECS_switcher->TECS_reset = ((*rtu_mode_data_eFWLifter_Mode) == ON);
    rty_busFW_TECS_switcher->h_hold = true;
    break;

   case B_TRANS:
    rty_busFW_TECS_switcher->h_dot_cmd = 0.0;
    rty_busFW_TECS_switcher->h_cmd = *rtu_mode_data_bt_data_BT_Altitude;
    rty_busFW_TECS_switcher->V_cmd = 18.0;
    rty_busFW_TECS_switcher->roll_cmd = y;
    rty_busFW_TECS_switcher->TECS_reset = ((*rtu_mode_data_eFWLifter_Mode) == ON);
    rty_busFW_TECS_switcher->h_hold = true;
    break;

   case FLTDIR:
    rty_busFW_TECS_switcher->h_dot_cmd = 3.0 * (*rtu_Pilot_throttle_ch);
    rty_busFW_TECS_switcher->h_cmd = rtb_TrigonometricFunction6_onwcg4eh52;

    /* Switch: '<S3>/Switch' incorporates:
     *  BusAssignment: '<S3>/Bus Assignment2'
     *  Constant: '<S23>/Constant'
     *  Constant: '<S3>/Constant'
     *  Product: '<S23>/Product'
     */
    if (*rtu_bGPSLossFlag) {
      rty_busFW_TECS_switcher->V_cmd = 22.0;
    } else {
      rty_busFW_TECS_switcher->V_cmd = *rtu_std_command_airspeed_cas_cmd;
    }

    /* End of Switch: '<S3>/Switch' */
    rty_busFW_TECS_switcher->roll_cmd = PB_norm;
    rty_busFW_TECS_switcher->TECS_reset = false;
    rty_busFW_TECS_switcher->h_hold = rtb_FixPtRelationalOperator_lyd2dqurri;
    break;

   case LOITER:
    rty_busFW_TECS_switcher->h_dot_cmd = 0.0;
    rty_busFW_TECS_switcher->h_cmd = *rtu_mode_data_loiter_data_loiter_altitude;

    /* Switch: '<S6>/Switch' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment2'
     *  Constant: '<S6>/Constant'
     *  Constant: '<S6>/Constant2'
     */
    if (*rtu_bGPSLossFlag) {
      rty_busFW_TECS_switcher->V_cmd = 22.0;
    } else {
      rty_busFW_TECS_switcher->V_cmd =
        *rtu_mode_data_loiter_data_loiter_AirSpeedRef;
    }

    /* End of Switch: '<S6>/Switch' */
    rty_busFW_TECS_switcher->roll_cmd = rtb_Product1_bkygj4a2su;
    rty_busFW_TECS_switcher->TECS_reset = false;
    rty_busFW_TECS_switcher->h_hold = true;
    break;

   case WAYPNT:
    rty_busFW_TECS_switcher->h_dot_cmd = 0.0;

    /* Switch: '<S8>/Switch5' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment2'
     *  Constant: '<S8>/Constant10'
     *  Constant: '<S8>/Constant11'
     *  Constant: '<S8>/Constant5'
     *  Switch: '<S8>/Switch3'
     */
    if (*rtu_bGPSLossFlag) {
      rty_busFW_TECS_switcher->h_cmd = scale;
      rty_busFW_TECS_switcher->V_cmd = 22.0;
    } else {
      rty_busFW_TECS_switcher->h_cmd = C_idx_0;
      rty_busFW_TECS_switcher->V_cmd = 25.0;
    }

    /* End of Switch: '<S8>/Switch5' */
    rty_busFW_TECS_switcher->roll_cmd = AC_norm;
    rty_busFW_TECS_switcher->TECS_reset = false;
    rty_busFW_TECS_switcher->h_hold = true;
    break;

   case FW_RTH:
    rty_busFW_TECS_switcher->h_dot_cmd = 0.0;
    rty_busFW_TECS_switcher->h_cmd = *rtu_mode_data_fwrth_data_alt_sp;

    /* Switch: '<S5>/Switch1' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment2'
     *  Constant: '<S5>/Constant2'
     *  Constant: '<S5>/Constant4'
     */
    if (*rtu_bGPSLossFlag) {
      rty_busFW_TECS_switcher->V_cmd = 22.0;
    } else {
      rty_busFW_TECS_switcher->V_cmd = *rtu_mode_data_fwrth_data_CAS_sp;
    }

    /* End of Switch: '<S5>/Switch1' */
    rty_busFW_TECS_switcher->roll_cmd = t;
    rty_busFW_TECS_switcher->TECS_reset = false;
    rty_busFW_TECS_switcher->h_hold = true;
    break;

   default:
    *rty_busFW_TECS_switcher = FW_TECS_switcher_ConstB.BusAssignment2;
    break;
  }

  /* End of MultiPortSwitch: '<Root>/Multiport Switch' */

  /* Logic: '<S106>/OR1' */
  rtb_Equal_bb4ze044et = (rtb_Equal_bb4ze044et ||
    rtb_FixPtRelationalOperator_h1ymkdiv2b);

  /* RelationalOperator: '<S106>/Equal' incorporates:
   *  Constant: '<S106>/Constant1'
   *  Memory: '<S106>/Memory'
   *  Sum: '<S106>/Sum1'
   */
  rtb_FixPtRelationalOperator_lyd2dqurri = ((((int32_T)
    (*rtu_wp_data_wp_list_count)) - 1) == ((int32_T)
    FW_TECS_switcher_DW.Memory_PreviousInput));

  /* Delay: '<S106>/cur_idx' */
  FW_TECS_switcher_DW.icLoad_hfgm0d2zgp = ((rtb_Equal_bb4ze044et && (((uint32_T)
    FW_TECS_switcher_PrevZCX.cur_idx_Reset_ZCE) != POS_ZCSIG)) ||
    (FW_TECS_switcher_DW.icLoad_hfgm0d2zgp));
  FW_TECS_switcher_PrevZCX.cur_idx_Reset_ZCE = rtb_Equal_bb4ze044et ?
    ((ZCSigState)1) : ((ZCSigState)0);
  if (FW_TECS_switcher_DW.icLoad_hfgm0d2zgp) {
    /* Switch: '<S106>/Switch1' incorporates:
     *  Memory: '<S106>/Memory'
     *  Sum: '<S106>/Sum'
     *  Switch: '<S106>/Switch2'
     */
    if (rtb_FixPtRelationalOperator_h1ymkdiv2b) {
      FW_TECS_switcher_DW.cur_idx_DSTATE = (uint16_T)(((uint32_T)
        FW_TECS_switcher_DW.Memory_PreviousInput) + ((uint32_T)
        ((!rtb_FixPtRelationalOperator_lyd2dqurri) ? ((int32_T)1) : ((int32_T)0))));
    } else {
      FW_TECS_switcher_DW.cur_idx_DSTATE = *rtu_wp_data_cmd_wp_idx;
    }

    /* End of Switch: '<S106>/Switch1' */
  }

  *rty_wp_req_idx = FW_TECS_switcher_DW.cur_idx_DSTATE;

  /* End of Delay: '<S106>/cur_idx' */

  /* MATLAB Function: '<S135>/MATLAB Function1' incorporates:
   *  Trigonometry: '<S137>/Trigonometric Function'
   *  Trigonometry: '<S137>/Trigonometric Function1'
   *  Trigonometry: '<S137>/Trigonometric Function2'
   *  Trigonometry: '<S137>/Trigonometric Function3'
   */
  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'WPNav/pos_NED2/ECEF2NED/MATLAB Function1': '<S138>:1' */
  /* '<S138>:1:6' cos_lat = cos(LLA(1)); */
  rtb_TrigonometricFunction_paar0fmbpe = cos(*rtu_wp_data_cur_wp_lat);

  /* '<S138>:1:7' sin_lat = sin(LLA(1)); */
  AC_norm = sin(*rtu_wp_data_cur_wp_lat);

  /* '<S138>:1:8' cos_long = cos(LLA(2)); */
  Vg = cos(*rtu_wp_data_cur_wp_lon);

  /* '<S138>:1:9' sin_long = sin(LLA(2)); */
  y = sin(*rtu_wp_data_cur_wp_lon);

  /* Product: '<S141>/Product' incorporates:
   *  Constant: '<S141>/e'
   */
  /* '<S138>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S138>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S138>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  rtb_Product1_bkygj4a2su = AC_norm * 0.0818191908425;

  /* Sqrt: '<S141>/Sqrt' incorporates:
   *  Constant: '<S141>/const'
   *  Math: '<S141>/Square'
   *  Sum: '<S141>/Add'
   */
  rtb_Product1_bkygj4a2su = sqrt(1.0 - (rtb_Product1_bkygj4a2su *
    rtb_Product1_bkygj4a2su));

  /* Switch: '<S142>/Switch3' incorporates:
   *  Abs: '<S142>/Abs'
   *  Product: '<S142>/Product'
   */
  if (rtb_Product1_bkygj4a2su <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S142>/Switch3' */

  /* Product: '<S141>/Divide' incorporates:
   *  Constant: '<S141>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* Product: '<S137>/Product' incorporates:
   *  Product: '<S137>/Product1'
   *  Sum: '<S137>/Sum'
   */
  dir_idx_1 = (rtb_Product1_bkygj4a2su + (*rtu_wp_data_cur_wp_alt)) *
    rtb_TrigonometricFunction_paar0fmbpe;

  /* Product: '<S137>/Product3' incorporates:
   *  Constant: '<S137>/1-e^2'
   *  Product: '<S137>/Product2'
   *  Sum: '<S137>/Sum1'
   */
  dir_idx_0 = AC_norm * ((0.993305620009879 * rtb_Product1_bkygj4a2su) +
    (*rtu_wp_data_cur_wp_alt));

  /* Sqrt: '<S139>/Sqrt' */
  rtb_Product1_bkygj4a2su = AC_norm_tmp;

  /* Switch: '<S140>/Switch3' incorporates:
   *  Product: '<S140>/Product'
   */
  if (AC_norm_tmp <= 0.001) {
    rtb_Product1_bkygj4a2su = -0.001;
  }

  /* End of Switch: '<S140>/Switch3' */

  /* Product: '<S139>/Divide' incorporates:
   *  Constant: '<S139>/R_0'
   */
  rtb_Product1_bkygj4a2su = 6.378137E+6 / rtb_Product1_bkygj4a2su;

  /* MATLAB Function: '<S105>/guidance_circle' */
  /* MATLAB Function 'WPNav/Subsystem/guidance_circle': '<S110>:1' */
  /*  circle L1-guidance */
  /*  start_pos     [x; y] (m) */
  /*  direction     [dx; dy] (unit vector) */
  /*  cur_pos       [x; y] (m) */
  /*  Vg            [vx, vy] ground-speed (m/s) */
  /*  lat_ax        (m/s/s) lateral acceleration command */
  /* '<S110>:1:11' R_min = 100; */
  /* '<S110>:1:13' if R < R_min */
  /* '<S110>:1:16' curvature = direction*1/R; */
  /* '<S110>:1:19' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  /*  horizontal ground speed */
  /* '<S110>:1:20' if (Vg<0.01) */
  if (rtb_TrigonometricFunction2_i3ecl5ucim < 0.01) {
    /* '<S110>:1:21' Vg =0.001; */
    rtb_TrigonometricFunction2_i3ecl5ucim = 0.001;
  }

  /* '<S110>:1:23' vel_dir = vel_NE/Vg; */
  /* '<S110>:1:24' chi = atan2(vel_dir(2), vel_dir(1)); */
  /* '<S110>:1:26' L1_P = 12; */
  /*  L1 peroid (s) */
  /* '<S110>:1:27' L1_zeta = 0.7; */
  /*  L1 samping ratio */
  /* '<S110>:1:28' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_Sqrt = 2.6738030439438414 * rtb_TrigonometricFunction2_i3ecl5ucim;

  /* MATLAB Function: '<S135>/MATLAB Function1' */
  /*  L1 length */
  /* '<S110>:1:30' A = cur_pos; */
  /*  find center of circle O */
  /* '<S110>:1:33' O = [0;0]; */
  /* '<S110>:1:35' OA = A - O; */
  rtb_C_pex2miccv0[0] = (-AC_norm) * Vg;
  rtb_C_pex2miccv0[3] = (-AC_norm) * y;
  rtb_C_pex2miccv0[6] = rtb_TrigonometricFunction_paar0fmbpe;
  rtb_C_pex2miccv0[1] = -y;
  rtb_C_pex2miccv0[4] = Vg;
  rtb_C_pex2miccv0[7] = 0.0;
  rtb_C_pex2miccv0[2] = (-rtb_TrigonometricFunction_paar0fmbpe) * Vg;
  rtb_C_pex2miccv0[5] = (-rtb_TrigonometricFunction_paar0fmbpe) * y;
  rtb_C_pex2miccv0[8] = -AC_norm;

  /* Product: '<S136>/Product' incorporates:
   *  Product: '<S136>/Product1'
   *  Sum: '<S136>/Sum'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_1 = (rtb_Product1_bkygj4a2su +
    rtu_Sensor_pos_lla[2]) * rtb_TrigonometricFunction2_h0qciwy3lv_tmp;

  /* Sum: '<S109>/Sum' incorporates:
   *  Constant: '<S136>/1-e^2'
   *  Product: '<S136>/Product'
   *  Product: '<S136>/Product1'
   *  Product: '<S136>/Product2'
   *  Product: '<S136>/Product3'
   *  Product: '<S137>/Product'
   *  Product: '<S137>/Product1'
   *  Sum: '<S136>/Sum1'
   */
  rtb_VectorConcatenate1_psilb5rrki_idx_0 =
    (rtb_VectorConcatenate1_psilb5rrki_idx_1 * rtb_Sum_imgtnbczew_tmp) -
    (dir_idx_1 * Vg);
  dir_idx_1 = (rtb_VectorConcatenate1_psilb5rrki_idx_1 *
               rtb_Sum_imgtnbczew_tmp_0) - (dir_idx_1 * y);
  AC_norm = (curvature * ((0.993305620009879 * rtb_Product1_bkygj4a2su) +
              rtu_Sensor_pos_lla[2])) - dir_idx_0;

  /* Product: '<S135>/Product' */
  for (i = 0; i < 3; i++) {
    rtb_VectorConcatenate1[i] = ((rtb_C_pex2miccv0[i] *
      rtb_VectorConcatenate1_psilb5rrki_idx_0) + (rtb_C_pex2miccv0[i + 3] *
      dir_idx_1)) + (rtb_C_pex2miccv0[i + 6] * AC_norm);
  }

  /* MATLAB Function: '<S105>/guidance_circle' incorporates:
   *  Product: '<S135>/Product'
   */
  /* '<S110>:1:36' a = sqrt(OA(1)^2 + OA(2)^2); */
  AC_norm = sqrt((rtb_VectorConcatenate1[0] * rtb_VectorConcatenate1[0]) +
                 (rtb_VectorConcatenate1[1] * rtb_VectorConcatenate1[1]));

  /* '<S110>:1:38' if (L1 + a) <= R */
  if ((rtb_Sqrt + AC_norm) <= 150.0) {
    /* '<S110>:1:39' gamma = pi; */
    /* '<S110>:1:40' chi_ao = atan2(-OA(2), -OA(1)); */
    /* '<S110>:1:41' chi_L = chi_ao - sign(curvature)*gamma; */
    curvature = atan2(-rtb_VectorConcatenate1[1], -rtb_VectorConcatenate1[0]) -
      -3.1415926535897931;
  } else if (AC_norm >= (rtb_Sqrt + 150.0)) {
    /* '<S110>:1:42' elseif a >= (L1 + R) */
    /* '<S110>:1:43' gamma = 0; */
    /* '<S110>:1:44' chi_ao = atan2(-OA(2), -OA(1)); */
    /* '<S110>:1:45' chi_L = chi_ao - sign(curvature)*gamma; */
    curvature = atan2(-rtb_VectorConcatenate1[1], -rtb_VectorConcatenate1[0]);
  } else {
    /* '<S110>:1:46' else */
    /* '<S110>:1:47' temp1 = (L1^2 + a^2 - R^2)/(2*L1*a); */
    /*  here a != 0, if condition-1 covers a=0 case */
    /* '<S110>:1:48' temp1 = max(temp1, -1); */
    /* '<S110>:1:49' temp1 = min(temp1, 1); */
    /* '<S110>:1:50' gamma = acos(temp1); */
    /* '<S110>:1:51' chi_ao = atan2(-OA(2), -OA(1)); */
    /* '<S110>:1:52' chi_L = chi_ao - sign(curvature)*gamma; */
    rtb_VectorConcatenate1_psilb5rrki_idx_0 = (((rtb_Sqrt * rtb_Sqrt) + (AC_norm
      * AC_norm)) - 22500.0) / ((2.0 * rtb_Sqrt) * AC_norm);
    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 < -1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = -1.0;
    }

    if (rtb_VectorConcatenate1_psilb5rrki_idx_0 > 1.0) {
      rtb_VectorConcatenate1_psilb5rrki_idx_0 = 1.0;
    }

    curvature = atan2(-rtb_VectorConcatenate1[1], -rtb_VectorConcatenate1[0]) -
      (-acos(rtb_VectorConcatenate1_psilb5rrki_idx_0));
  }

  /* Switch: '<S8>/Switch1' */
  /* eta = wrap_pi(chi_L - chi); */
  /* eta = max(eta, -pi); */
  /* eta = min(eta, pi); */
  /* lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  if (rtb_FixPtRelationalOperator_lyd2dqurri) {
    /* Update for Memory: '<S8>/Memory1' incorporates:
     *  Constant: '<S105>/Constant'
     *  Constant: '<S111>/Constant2'
     *  MATLAB Function: '<S105>/guidance_circle'
     *  Math: '<S105>/Square'
     *  Math: '<S105>/Square1'
     *  Math: '<S111>/Mod2'
     *  Math: '<S111>/Mod3'
     *  Product: '<S105>/Divide'
     *  Sum: '<S105>/Sum'
     *  Sum: '<S111>/Add3'
     *  Sum: '<S111>/Add4'
     *  Sum: '<S111>/Sum'
     *  Trigonometry: '<S105>/Sin'
     */
    FW_TECS_switcher_DW.Memory1_PreviousInput = (((4.0 * sin(rt_modd(rt_modd
      (((curvature - (*rtu_Sensor_chi)) - -3.1415926535897931) +
       FW_TECS_switcher_ConstB.Sum1_a4lnrlft1l,
       FW_TECS_switcher_ConstB.Sum1_a4lnrlft1l),
      FW_TECS_switcher_ConstB.Sum1_a4lnrlft1l) - 3.1415926535897931)) *
      0.48999999999999994) * (rtb_TrigonometricFunction2_i3ecl5ucim *
      rtb_TrigonometricFunction2_i3ecl5ucim)) / rtb_Sqrt;
  } else {
    /* MATLAB Function: '<S8>/guidance_line1' */
    y = atan2((rtb_VectorConcatenate1_oigmvxpsrq_idx_0 * AC_idx_1) - (AC_idx_0 *
               rtb_VectorConcatenate1_oigmvxpsrq_idx_1),
              (rtb_VectorConcatenate1_oigmvxpsrq_idx_0 * AC_idx_0) +
              (rtb_VectorConcatenate1_oigmvxpsrq_idx_1 * AC_idx_1));
    if (y < -1.5707963267948966) {
      y = -1.5707963267948966;
    }

    if (y > 1.5707963267948966) {
      y = 1.5707963267948966;
    }

    /* Update for Memory: '<S8>/Memory1' incorporates:
     *  MATLAB Function: '<S8>/guidance_line1'
     */
    FW_TECS_switcher_DW.Memory1_PreviousInput = ((1.9599999999999997 * (Vg_0 *
      Vg_0)) / rtb_Sum_lskvbzjz0i) * sin(y);
  }

  /* End of Switch: '<S8>/Switch1' */

  /* Update for Memory: '<S5>/Memory4' */
  FW_TECS_switcher_DW.Memory4_PreviousInput = *rtu_mode_data_fwrth_data_phase;

  /* Update for Delay: '<S5>/Delay' */
  FW_TECS_switcher_DW.Delay_DSTATE_iwjnjngoev[0] =
    FW_TECS_switcher_DW.Delay_DSTATE_iwjnjngoev[1];
  FW_TECS_switcher_DW.Delay_DSTATE_iwjnjngoev[1] =
    *rtu_mode_data_fwrth_data_phase;

  /* Update for Delay: '<S5>/Delay1' */
  FW_TECS_switcher_DW.Delay1_DSTATE_niw4uw2lp0[0] =
    FW_TECS_switcher_DW.Delay1_DSTATE_niw4uw2lp0[1];
  FW_TECS_switcher_DW.Delay1_DSTATE_niw4uw2lp0[1] =
    *rtu_mode_data_fwrth_data_phase;

  /* Update for Delay: '<S5>/Delay2' */
  FW_TECS_switcher_DW.Delay2_DSTATE_ctj124hvji[0] =
    FW_TECS_switcher_DW.Delay2_DSTATE_ctj124hvji[1];
  FW_TECS_switcher_DW.Delay2_DSTATE_ctj124hvji[1] =
    *rtu_mode_data_fwrth_data_phase;

  /* Update for Memory: '<S106>/Memory2' */
  FW_TECS_switcher_DW.Memory2_PreviousInput = rtb_OR;

  /* Update for Delay: '<S106>/cur_pos_sp' */
  FW_TECS_switcher_DW.icLoad = false;

  /* Update for UnitDelay: '<S112>/Delay Input1'
   *
   * Block description for '<S112>/Delay Input1':
   *
   *  Store in Global RAM
   */
  FW_TECS_switcher_DW.DelayInput1_DSTATE = *rtu_wp_data_cmd_wp_idx;

  /* Update for UnitDelay: '<S113>/Delay Input1'
   *
   * Block description for '<S113>/Delay Input1':
   *
   *  Store in Global RAM
   */
  FW_TECS_switcher_DW.DelayInput1_DSTATE_hck53lu3v0 = rtb_Compare_ag4sp0vjdy;

  /* Update for UnitDelay: '<S114>/Delay Input1'
   *
   * Block description for '<S114>/Delay Input1':
   *
   *  Store in Global RAM
   */
  FW_TECS_switcher_DW.DelayInput1_DSTATE_eu53xg3kmp = rtb_Memory_mouojdlutb;

  /* Update for Delay: '<S106>/prev_pos_sp' */
  FW_TECS_switcher_DW.icLoad_jveizoqakw = false;

  /* Update for Delay: '<S8>/Delay' incorporates:
   *  Constant: '<S8>/Constant4'
   *  Logic: '<S8>/AND'
   *  RelationalOperator: '<S8>/Equal'
   *  Sum: '<S8>/Sum'
   */
  FW_TECS_switcher_DW.Delay_DSTATE_perkgn1qf5[0] =
    FW_TECS_switcher_DW.Delay_DSTATE_perkgn1qf5[1];
  FW_TECS_switcher_DW.Delay_DSTATE_perkgn1qf5[1] = ((*rtu_wp_data_last_wp_land) &&
    ((((int32_T)(*rtu_wp_data_wp_list_count)) - 1) == ((int32_T)(*rty_wp_req_idx))));

  /* Update for Delay: '<S8>/Delay3' incorporates:
   *  Constant: '<S103>/Constant'
   *  RelationalOperator: '<S103>/Compare'
   */
  FW_TECS_switcher_DW.Delay3_DSTATE[0] = FW_TECS_switcher_DW.Delay3_DSTATE[1];
  FW_TECS_switcher_DW.Delay3_DSTATE[1] = ((*rtu_std_command_vom_cmd) == WAYPNT);

  /* Update for UnitDelay: '<S104>/Delay Input1'
   *
   * Block description for '<S104>/Delay Input1':
   *
   *  Store in Global RAM
   */
  FW_TECS_switcher_DW.DelayInput1_DSTATE_ezpkpkh21k = rtb_Delay3;

  /* Update for Delay: '<S8>/Delay1' */
  FW_TECS_switcher_DW.icLoad_ncnilzog2z = false;

  /* Update for Delay: '<S8>/Delay2' */
  FW_TECS_switcher_DW.icLoad_fj3gi4q45w = false;

  /* Update for Delay: '<S23>/Delay1' */
  FW_TECS_switcher_DW.icLoad_ogna325jjd = false;
  FW_TECS_switcher_DW.Delay1_DSTATE_n1as2selwq =
    rtb_TrigonometricFunction6_onwcg4eh52;

  /* Update for Delay: '<S22>/Delay' */
  FW_TECS_switcher_DW.icLoad_cnbza4ewej = false;
  FW_TECS_switcher_DW.Delay_DSTATE = rtb_Sqrt_jug5wuieqd;

  /* Update for Delay: '<S90>/Delay1' */
  FW_TECS_switcher_DW.icLoad_kbyaompepr = false;
  FW_TECS_switcher_DW.Delay1_DSTATE_g1oevxsf0d = absxk;

  /* Update for UnitDelay: '<S8>/Unit Delay1' */
  FW_TECS_switcher_DW.UnitDelay1_DSTATE = scale;

  /* Update for UnitDelay: '<S8>/Unit Delay2' */
  FW_TECS_switcher_DW.UnitDelay2_DSTATE = C_idx_0;

  /* Update for Memory: '<S106>/Memory' */
  FW_TECS_switcher_DW.Memory_PreviousInput = *rty_wp_req_idx;

  /* Update for Delay: '<S106>/cur_idx' */
  FW_TECS_switcher_DW.icLoad_hfgm0d2zgp = false;
  FW_TECS_switcher_DW.cur_idx_DSTATE = *rty_wp_req_idx;
}

/* Model initialize function */
void FW_TECS_switcher_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&FW_TECS_switcher_DW, 0,
                sizeof(FW_TECS_switcher_TDW));
  FW_TECS_switcher_PrevZCX.cur_pos_sp_Reset_ZCE = POS_ZCSIG;
  FW_TECS_switcher_PrevZCX.prev_pos_sp_Reset_ZCE = POS_ZCSIG;
  FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE = POS_ZCSIG;
  FW_TECS_switcher_PrevZCX.Delay2_Reset_ZCE = POS_ZCSIG;
  FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE_pzkzowkb1n = POS_ZCSIG;
  FW_TECS_switcher_PrevZCX.Delay1_Reset_ZCE_okp35hdmnj = POS_ZCSIG;
  FW_TECS_switcher_PrevZCX.cur_idx_Reset_ZCE = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
