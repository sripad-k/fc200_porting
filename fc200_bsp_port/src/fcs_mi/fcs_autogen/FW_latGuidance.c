/*
 * File: FW_latGuidance.c
 *
 * Code generated for Simulink model 'FW_latGuidance'.
 *
 * Model version                  : 2.81
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:32 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FW_latGuidance.h"
#include "rtwtypes.h"
#include "FW_latGuidance_types.h"
#include <math.h>
#include "FW_latGuidance_private.h"

/* Output and update for referenced model: 'FW_latGuidance' */
void FW_latGuidance(const real_T *rtu_FW_TECS_switcher_roll_cmd, const
                    std_sensor_t *rtu_Sensor, real_T *rty_rollCmd, real_T
                    *rty_rollCmd_FW_guidance)
{
  real_T sin_lat[9];
  real_T rtb_Product[3];
  real_T cos_lat;
  real_T rtb_Sqrt;
  real_T rtb_Sum_jyf4qwqbhl_idx_0_tmp;
  real_T rtb_TrigonometricFunction;
  int32_T i;

  /* Trigonometry: '<S8>/Trigonometric Function' */
  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S10>:1' */
  /* '<S10>:1:6' cos_lat = cos(LLA(1)); */
  /* '<S10>:1:7' sin_lat = sin(LLA(1)); */
  /* '<S10>:1:8' cos_long = cos(LLA(2)); */
  /* '<S10>:1:9' sin_long = sin(LLA(2)); */
  /* '<S10>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S10>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S10>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
  rtb_TrigonometricFunction = sin(rtu_Sensor->pos_lla[0]);

  /* Product: '<S11>/Product' incorporates:
   *  Constant: '<S11>/e'
   */
  rtb_Sqrt = rtb_TrigonometricFunction * 0.0818191908425;

  /* Sqrt: '<S11>/Sqrt' incorporates:
   *  Constant: '<S11>/const'
   *  Math: '<S11>/Square'
   *  Sum: '<S11>/Add'
   */
  rtb_Sqrt = sqrt(1.0 - (rtb_Sqrt * rtb_Sqrt));

  /* Switch: '<S12>/Switch3' incorporates:
   *  Abs: '<S12>/Abs'
   *  Product: '<S12>/Product'
   */
  if (rtb_Sqrt <= 0.001) {
    rtb_Sqrt = -0.001;
  }

  /* End of Switch: '<S12>/Switch3' */

  /* Product: '<S11>/Divide' incorporates:
   *  Constant: '<S11>/R_0'
   */
  rtb_Sqrt = 6.378137E+6 / rtb_Sqrt;

  /* MATLAB Function: '<S7>/MATLAB Function1' */
  sin_lat[0] = -0.23381853358350183;
  sin_lat[3] = -0.34181934761104993;
  sin_lat[6] = 0.91021339638109977;
  sin_lat[1] = -0.82537236894794752;
  sin_lat[4] = 0.5645887464139302;
  sin_lat[7] = 0.0;
  sin_lat[2] = -0.51389624043197091;
  sin_lat[5] = -0.75126498721922552;
  sin_lat[8] = -0.414139557454227;

  /* Product: '<S8>/Product' incorporates:
   *  Product: '<S8>/Product1'
   *  Sum: '<S8>/Sum'
   *  Trigonometry: '<S8>/Trigonometric Function1'
   */
  rtb_Sum_jyf4qwqbhl_idx_0_tmp = (rtb_Sqrt + rtu_Sensor->pos_lla[2]) * cos
    (rtu_Sensor->pos_lla[0]);

  /* Sum: '<S6>/Sum' incorporates:
   *  Concatenate: '<S9>/Vector Concatenate'
   *  Constant: '<S8>/1-e^2'
   *  Product: '<S8>/Product'
   *  Product: '<S8>/Product1'
   *  Product: '<S8>/Product2'
   *  Product: '<S8>/Product3'
   *  Sum: '<S8>/Sum1'
   *  Trigonometry: '<S8>/Trigonometric Function2'
   *  Trigonometry: '<S8>/Trigonometric Function3'
   */
  cos_lat = (rtb_Sum_jyf4qwqbhl_idx_0_tmp * cos(rtu_Sensor->pos_lla[1])) -
    FW_latGuidance_ConstB.VectorConcatenate[0];
  rtb_Sum_jyf4qwqbhl_idx_0_tmp = (rtb_Sum_jyf4qwqbhl_idx_0_tmp * sin
    (rtu_Sensor->pos_lla[1])) - FW_latGuidance_ConstB.VectorConcatenate[1];
  rtb_TrigonometricFunction = (rtb_TrigonometricFunction * ((0.993305620009879 *
    rtb_Sqrt) + rtu_Sensor->pos_lla[2])) -
    FW_latGuidance_ConstB.VectorConcatenate[2];

  /* Product: '<S7>/Product' */
  for (i = 0; i < 3; i++) {
    /* Product: '<S7>/Product' */
    rtb_Product[i] = ((sin_lat[i] * cos_lat) + (sin_lat[i + 3] *
      rtb_Sum_jyf4qwqbhl_idx_0_tmp)) + (sin_lat[i + 6] *
      rtb_TrigonometricFunction);
  }

  /* End of Product: '<S7>/Product' */

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  SignalConversion generated from: '<S2>/Vector Concatenate1'
   * */
  /* MATLAB Function 'L1_guidance/MATLAB Function': '<S5>:1' */
  /*  circle L1-guidance */
  /*  start_pos     [x; y] (m) */
  /*  direction     [dx; dy] (unit vector) */
  /*  cur_pos       [x; y] (m) */
  /*  Vg            [vx, vy] ground-speed (m/s) */
  /*  R             Turn Radius (m) */
  /*  R_min         minimum Radius (m) */
  /*  lat_ax        (m/s/s) lateral acceleration command */
  /* '<S5>:1:13' R_min = 20; */
  /* '<S5>:1:15' if abs(R) < R_min */
  /* '<S5>:1:18' curvature = 1/R; */
  /* '<S5>:1:20' Vg = sqrt(vel_NE(1)^2 + vel_NE(2)^2); */
  cos_lat = sqrt((rtu_Sensor->vel_ned[0] * rtu_Sensor->vel_ned[0]) +
                 (rtu_Sensor->vel_ned[1] * rtu_Sensor->vel_ned[1]));

  /*  horizontal ground speed */
  /* '<S5>:1:22' if (Vg<0.01) */
  if (cos_lat < 0.01) {
    /* '<S5>:1:23' Vg =0.001; */
    cos_lat = 0.001;
  }

  /* '<S5>:1:25' vel_dir = vel_NE/Vg; */
  /* '<S5>:1:26' chi = atan2(vel_dir(2), vel_dir(1)); */
  /* '<S5>:1:28' L1_P = 10; */
  /*  L1 peroid (s) */
  /* '<S5>:1:29' L1_zeta = 0.7; */
  /*  L1 samping ratio */
  /* '<S5>:1:30' L1 = L1_P*L1_zeta/pi*Vg; */
  rtb_TrigonometricFunction = 2.228169203286535 * cos_lat;

  /*  L1 length */
  /*  P = start_pos; */
  /*  dir = start_dir; */
  /* '<S5>:1:34' A = cur_pos; */
  /*  find center of circle O */
  /*  O = center_pos; %P + sign(curvature)*R*[-dir(2); dir(1)]; */
  /* '<S5>:1:39' OA = A; */
  /*  - O; */
  /* '<S5>:1:40' a = sqrt(OA(1)^2 + OA(2)^2); */
  rtb_Sqrt = sqrt((rtb_Product[0] * rtb_Product[0]) + (rtb_Product[1] *
    rtb_Product[1]));

  /* '<S5>:1:42' if (L1 + a) <= R */
  if ((rtb_TrigonometricFunction + rtb_Sqrt) <= 200.0) {
    /* '<S5>:1:43' gamma = pi; */
    rtb_Sqrt = 3.1415926535897931;
  } else if (rtb_Sqrt >= (rtb_TrigonometricFunction + 200.0)) {
    /* '<S5>:1:44' elseif a >= (L1 + R) */
    /* '<S5>:1:45' gamma = 0; */
    rtb_Sqrt = 0.0;
  } else {
    /* '<S5>:1:46' else */
    /* '<S5>:1:47' temp1 = (L1^2 + a^2 - R^2)/(2*L1*a); */
    /*  here a != 0, if condition-1 covers a=0 case */
    /* '<S5>:1:48' temp1 = max(temp1, -1); */
    /* '<S5>:1:49' temp1 = min(temp1, 1); */
    /* '<S5>:1:50' gamma = acos(temp1); */
    rtb_Sqrt = (((rtb_TrigonometricFunction * rtb_TrigonometricFunction) +
                 (rtb_Sqrt * rtb_Sqrt)) - 40000.0) / ((2.0 *
      rtb_TrigonometricFunction) * rtb_Sqrt);
    if (rtb_Sqrt < -1.0) {
      rtb_Sqrt = -1.0;
    }

    if (rtb_Sqrt > 1.0) {
      rtb_Sqrt = 1.0;
    }

    rtb_Sqrt = acos(rtb_Sqrt);
  }

  /* '<S5>:1:53' chi_ao = atan2(-OA(2), -OA(1)); */
  /* '<S5>:1:54' chi_L = wrap_pi(chi_ao - sign(curvature)*gamma); */
  rtb_Sqrt = atan2(-rtb_Product[1], -rtb_Product[0]) - rtb_Sqrt;

  /* '<S5>:1:67' if ang_in > pi */
  if (rtb_Sqrt > 3.1415926535897931) {
    /* '<S5>:1:68' ang_in = ang_in - 2*pi; */
    rtb_Sqrt -= 6.2831853071795862;
  } else if (rtb_Sqrt < -3.1415926535897931) {
    /* '<S5>:1:69' elseif ang_in < -pi */
    /* '<S5>:1:70' ang_in = ang_in + 2*pi; */
    rtb_Sqrt += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* '<S5>:1:73' ang_out = ang_in; */
  /* '<S5>:1:56' eta = wrap_pi(chi_L - chi); */
  rtb_Sqrt -= atan2(rtu_Sensor->vel_ned[1] / cos_lat, rtu_Sensor->vel_ned[0] /
                    cos_lat);

  /* '<S5>:1:67' if ang_in > pi */
  if (rtb_Sqrt > 3.1415926535897931) {
    /* '<S5>:1:68' ang_in = ang_in - 2*pi; */
    rtb_Sqrt -= 6.2831853071795862;
  } else if (rtb_Sqrt < -3.1415926535897931) {
    /* '<S5>:1:69' elseif ang_in < -pi */
    /* '<S5>:1:70' ang_in = ang_in + 2*pi; */
    rtb_Sqrt += 6.2831853071795862;
  } else {
    /* no actions */
  }

  /* '<S5>:1:73' ang_out = ang_in; */
  /* '<S5>:1:57' eta = max(eta, -pi); */
  /* '<S5>:1:58' eta = min(eta, pi); */
  /* '<S5>:1:60' lat_ax = 4*L1_zeta^2*Vg^2/L1*sin(eta); */
  if (rtb_Sqrt < -3.1415926535897931) {
    rtb_Sqrt = -3.1415926535897931;
  }

  if (rtb_Sqrt > 3.1415926535897931) {
    rtb_Sqrt = 3.1415926535897931;
  }

  /* Trigonometry: '<S2>/Atan' incorporates:
   *  MATLAB Function: '<S2>/MATLAB Function'
   */
  cos_lat = atan2(((1.9599999999999997 * (cos_lat * cos_lat)) /
                   rtb_TrigonometricFunction) * sin(rtb_Sqrt), 9.81);

  /* Saturate: '<Root>/Saturation1' */
  if (cos_lat > 0.52359877559829882) {
    *rty_rollCmd = 0.52359877559829882;
  } else if (cos_lat < -0.52359877559829882) {
    *rty_rollCmd = -0.52359877559829882;
  } else {
    *rty_rollCmd = cos_lat;
  }

  /* End of Saturate: '<Root>/Saturation1' */

  /* SignalConversion generated from: '<Root>/rollCmd_FW_guidance' */
  *rty_rollCmd_FW_guidance = *rtu_FW_TECS_switcher_roll_cmd;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
