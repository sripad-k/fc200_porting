/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: posCtrl.c
 *
 * Code generated for Simulink model 'posCtrl'.
 *
 * Model version                  : 2.50
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:23 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "posCtrl.h"
#include "rtwtypes.h"
#include "posCtrl_types.h"
#include "posCtrl_private.h"
#include <math.h>
#include <string.h>
#include "zero_crossing_types.h"

/* Block states (default storage) */
posCtrl_TDW posCtrl_DW;

/* Previous zero-crossings (trigger) states */
posCtrl_TZCE posCtrl_PrevZCX;

/*
 * Output and update for atomic system:
 *    '<S11>/MATLAB Function1'
 *    '<S19>/MATLAB Function1'
 */
void posCtrl_i04w0oyqe2(const real_T rtu_LLA[3], real_T rty_C[9])
{
  real_T cos_lat;
  real_T cos_long;
  real_T sin_lat;
  real_T sin_long;

  /*  LLA as */
  /*  C as */
  /*  C_e_n as */
  /*  Calculate ECEF to NED coordinate transformation matrix using (2.150) */
  /* MATLAB Function 'pos_NED/ECEF2NED/MATLAB Function1': '<S14>:1' */
  /* '<S14>:1:6' cos_lat = cos(LLA(1)); */
  cos_lat = cos(rtu_LLA[0]);

  /* '<S14>:1:7' sin_lat = sin(LLA(1)); */
  sin_lat = sin(rtu_LLA[0]);

  /* '<S14>:1:8' cos_long = cos(LLA(2)); */
  cos_long = cos(rtu_LLA[1]);

  /* '<S14>:1:9' sin_long = sin(LLA(2)); */
  sin_long = sin(rtu_LLA[1]);

  /* '<S14>:1:10' C = [-sin_lat * cos_long, -sin_lat * sin_long,  cos_lat;... */
  /* '<S14>:1:11'                    -sin_long,            cos_long,        0;... */
  /* '<S14>:1:12'          -cos_lat * cos_long, -cos_lat * sin_long, -sin_lat]; */
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

/* System initialize for referenced model: 'posCtrl' */
void posCtrl_Init(void)
{
  /* InitializeConditions for Delay: '<Root>/Delay1' */
  posCtrl_DW.icLoad = true;

  /* InitializeConditions for Delay: '<Root>/Delay' */
  posCtrl_DW.icLoad_gwql4htcwe = true;
}

/* Output and update for referenced model: 'posCtrl' */
void posCtrl(const busWaypointManager *rtu_busWptManager, const std_sensor_t
             *rtu_Sensor, const vom_t *rtu_vom_status, busControllerPosCtrl
             *rty_controllerPosCtrl, real_T *rty_busControllerIF_vel_velCmdH_x,
             real_T *rty_busControllerIF_vel_velCmdH_y, real_T
             *rty_busControllerIF_vel_hRateCmd, real_T
             *rty_busControllerIF_vel_hCmd, boolean_T
             *rty_busControllerIF_vel_hHold, boolean_T
             *rty_busControllerIF_vel_init_reset_vel, boolean_T
             *rty_busControllerIF_vel_init_reset_hdot)
{
  real_T rtb_C[9];
  real_T rtb_C_0[3];
  real_T cps;
  real_T rtb_Product_haif3f5y2j_idx_0_tmp;
  real_T rtb_Product_haif3f5y2j_tmp;
  real_T rtb_Product_haif3f5y2j_tmp_0;
  real_T rtb_R_idx_0;
  real_T rtb_R_idx_1;
  real_T rtb_R_idx_2;
  real_T rtb_Sqrt_ii2xp45anb;
  real_T rtb_Sum1_tmp;
  real_T rtb_Sum_mnw5bny0bd_idx_0;
  real_T rtb_Sum_mnw5bny0bd_idx_0_tmp;
  real_T rtb_Sum_mnw5bny0bd_idx_2;
  real_T rtb_TrigonometricFunction;
  real_T sps;
  real_T sps_tmp;
  int32_T i;
  boolean_T rtb_LogicalOperator;

  /* MATLAB Function: '<S10>/MATLAB Function' */
  /*  psi sa */
  /*  fcn Computes a rotation matrix for a given angle. */
  /*    R = fcn(psi) returns a 2x2 rotation matrix R for the angle psi. */
  /* MATLAB Function 'rot_2D/MATLAB Function': '<S27>:1' */
  /* '<S27>:1:5' cps = cos(psi); */
  /* '<S27>:1:6' sps = sin(psi); */
  sps = sin(rtu_Sensor->eul_ang[2]);

  /* '<S27>:1:8' R = [cps, sps; */
  /* '<S27>:1:9'     -sps cps]; */
  rtb_R_idx_0 = cos(rtu_Sensor->eul_ang[2]);
  rtb_R_idx_2 = sps;
  rtb_R_idx_1 = -sps;

  /* Delay: '<Root>/Delay1' */
  if (posCtrl_DW.icLoad) {
    posCtrl_DW.Delay1_DSTATE = *rtu_vom_status;
  }

  /* Logic: '<Root>/Logical Operator' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S2>/Constant'
   *  Constant: '<S3>/Constant'
   *  Constant: '<S4>/Constant'
   *  Constant: '<S5>/Constant'
   *  Constant: '<S6>/Constant'
   *  Delay: '<Root>/Delay1'
   *  Logic: '<Root>/OR'
   *  RelationalOperator: '<Root>/Relational Operator'
   *  RelationalOperator: '<S1>/Compare'
   *  RelationalOperator: '<S2>/Compare'
   *  RelationalOperator: '<S3>/Compare'
   *  RelationalOperator: '<S4>/Compare'
   *  RelationalOperator: '<S5>/Compare'
   *  RelationalOperator: '<S6>/Compare'
   */
  rtb_LogicalOperator = ((((((((*rtu_vom_status) == TAKEOFF) ||
    ((*rtu_vom_status) == LAND)) || ((*rtu_vom_status) == HOVER)) ||
    ((*rtu_vom_status) == MR_RTH)) || ((*rtu_vom_status) == F_TRANS)) ||
    ((*rtu_vom_status) == B_TRANS)) && ((*rtu_vom_status) !=
    posCtrl_DW.Delay1_DSTATE));

  /* Delay: '<Root>/Delay' */
  posCtrl_DW.icLoad_gwql4htcwe = ((rtb_LogicalOperator && (((uint32_T)
    posCtrl_PrevZCX.Delay_Reset_ZCE) != POS_ZCSIG)) ||
    (posCtrl_DW.icLoad_gwql4htcwe));
  posCtrl_PrevZCX.Delay_Reset_ZCE = rtb_LogicalOperator ? ((ZCSigState)1) :
    ((ZCSigState)0);
  if (posCtrl_DW.icLoad_gwql4htcwe) {
    posCtrl_DW.Delay_DSTATE[0] = rtu_Sensor->pos_lla[0];
    posCtrl_DW.Delay_DSTATE[1] = rtu_Sensor->pos_lla[1];
    posCtrl_DW.Delay_DSTATE[2] = rtu_Sensor->pos_lla[2];
  }

  /* MATLAB Function: '<S11>/MATLAB Function1' incorporates:
   *  Delay: '<Root>/Delay'
   */
  posCtrl_i04w0oyqe2(posCtrl_DW.Delay_DSTATE, rtb_C);

  /* Trigonometry: '<S13>/Trigonometric Function' incorporates:
   *  Delay: '<Root>/Delay'
   *  Trigonometry: '<S21>/Trigonometric Function'
   */
  cps = sin(posCtrl_DW.Delay_DSTATE[0]);

  /* Product: '<S17>/Product' incorporates:
   *  Constant: '<S17>/e'
   *  Product: '<S25>/Product'
   *  Trigonometry: '<S13>/Trigonometric Function'
   */
  sps_tmp = cps * 0.0818191908425;

  /* Sqrt: '<S17>/Sqrt' incorporates:
   *  Constant: '<S17>/const'
   *  Math: '<S17>/Square'
   *  Product: '<S17>/Product'
   *  Sqrt: '<S25>/Sqrt'
   *  Sum: '<S17>/Add'
   */
  sps_tmp = sqrt(1.0 - (sps_tmp * sps_tmp));
  sps = sps_tmp;

  /* Switch: '<S18>/Switch3' incorporates:
   *  Product: '<S18>/Product'
   *  Sqrt: '<S17>/Sqrt'
   */
  if (sps_tmp <= 0.001) {
    sps = -0.001;
  }

  /* End of Switch: '<S18>/Switch3' */

  /* Product: '<S17>/Divide' incorporates:
   *  Constant: '<S17>/R_0'
   */
  sps = 6.378137E+6 / sps;

  /* Trigonometry: '<S13>/Trigonometric Function1' incorporates:
   *  Delay: '<Root>/Delay'
   *  Trigonometry: '<S21>/Trigonometric Function1'
   */
  rtb_Sum1_tmp = cos(posCtrl_DW.Delay_DSTATE[0]);

  /* Trigonometry: '<S13>/Trigonometric Function3' incorporates:
   *  Delay: '<Root>/Delay'
   *  Trigonometry: '<S21>/Trigonometric Function3'
   */
  rtb_Product_haif3f5y2j_tmp = cos(posCtrl_DW.Delay_DSTATE[1]);

  /* Product: '<S13>/Product' incorporates:
   *  Delay: '<Root>/Delay'
   *  Product: '<S13>/Product1'
   *  Sum: '<S13>/Sum'
   *  Trigonometry: '<S13>/Trigonometric Function1'
   */
  rtb_Product_haif3f5y2j_idx_0_tmp = (sps + posCtrl_DW.Delay_DSTATE[2]) *
    rtb_Sum1_tmp;

  /* Trigonometry: '<S13>/Trigonometric Function2' incorporates:
   *  Delay: '<Root>/Delay'
   *  Trigonometry: '<S21>/Trigonometric Function2'
   */
  rtb_Product_haif3f5y2j_tmp_0 = sin(posCtrl_DW.Delay_DSTATE[1]);

  /* Trigonometry: '<S12>/Trigonometric Function' */
  rtb_TrigonometricFunction = sin(rtu_busWptManager->posLLA[0]);

  /* Product: '<S15>/Product' incorporates:
   *  Constant: '<S15>/e'
   */
  rtb_Sqrt_ii2xp45anb = rtb_TrigonometricFunction * 0.0818191908425;

  /* Sqrt: '<S15>/Sqrt' incorporates:
   *  Constant: '<S15>/const'
   *  Math: '<S15>/Square'
   *  Sum: '<S15>/Add'
   */
  rtb_Sqrt_ii2xp45anb = sqrt(1.0 - (rtb_Sqrt_ii2xp45anb * rtb_Sqrt_ii2xp45anb));

  /* Switch: '<S16>/Switch3' incorporates:
   *  Abs: '<S16>/Abs'
   *  Product: '<S16>/Product'
   */
  if (rtb_Sqrt_ii2xp45anb <= 0.001) {
    rtb_Sqrt_ii2xp45anb = -0.001;
  }

  /* End of Switch: '<S16>/Switch3' */

  /* Product: '<S15>/Divide' incorporates:
   *  Constant: '<S15>/R_0'
   */
  rtb_Sqrt_ii2xp45anb = 6.378137E+6 / rtb_Sqrt_ii2xp45anb;

  /* Product: '<S12>/Product' incorporates:
   *  Product: '<S12>/Product1'
   *  Sum: '<S12>/Sum'
   *  Trigonometry: '<S12>/Trigonometric Function1'
   */
  rtb_Sum_mnw5bny0bd_idx_0_tmp = (rtb_Sqrt_ii2xp45anb +
    rtu_busWptManager->posLLA[2]) * cos(rtu_busWptManager->posLLA[0]);

  /* Sum: '<S7>/Sum' incorporates:
   *  Constant: '<S12>/1-e^2'
   *  Constant: '<S13>/1-e^2'
   *  Delay: '<Root>/Delay'
   *  Product: '<S12>/Product'
   *  Product: '<S12>/Product1'
   *  Product: '<S12>/Product2'
   *  Product: '<S12>/Product3'
   *  Product: '<S13>/Product'
   *  Product: '<S13>/Product1'
   *  Product: '<S13>/Product2'
   *  Product: '<S13>/Product3'
   *  Sum: '<S12>/Sum1'
   *  Sum: '<S13>/Sum1'
   *  Trigonometry: '<S12>/Trigonometric Function2'
   *  Trigonometry: '<S12>/Trigonometric Function3'
   *  Trigonometry: '<S13>/Trigonometric Function'
   *  Trigonometry: '<S13>/Trigonometric Function2'
   *  Trigonometry: '<S13>/Trigonometric Function3'
   */
  rtb_Sum_mnw5bny0bd_idx_0 = (rtb_Sum_mnw5bny0bd_idx_0_tmp * cos
    (rtu_busWptManager->posLLA[1])) - (rtb_Product_haif3f5y2j_idx_0_tmp *
    rtb_Product_haif3f5y2j_tmp);
  rtb_Product_haif3f5y2j_idx_0_tmp = (rtb_Sum_mnw5bny0bd_idx_0_tmp * sin
    (rtu_busWptManager->posLLA[1])) - (rtb_Product_haif3f5y2j_idx_0_tmp *
    rtb_Product_haif3f5y2j_tmp_0);
  rtb_Sum_mnw5bny0bd_idx_2 = (rtb_TrigonometricFunction * ((0.993305620009879 *
    rtb_Sqrt_ii2xp45anb) + rtu_busWptManager->posLLA[2])) - (cps *
    ((0.993305620009879 * sps) + posCtrl_DW.Delay_DSTATE[2]));

  /* Product: '<S11>/Product' */
  for (i = 0; i < 3; i++) {
    rtb_C_0[i] = ((rtb_C[i] * rtb_Sum_mnw5bny0bd_idx_0) + (rtb_C[i + 3] *
      rtb_Product_haif3f5y2j_idx_0_tmp)) + (rtb_C[i + 6] *
      rtb_Sum_mnw5bny0bd_idx_2);
  }

  /* End of Product: '<S11>/Product' */

  /* Product: '<Root>/Product' */
  sps = (rtb_R_idx_0 * rtb_C_0[0]) + (rtb_R_idx_2 * rtb_C_0[1]);
  rtb_Sum_mnw5bny0bd_idx_0 = (rtb_R_idx_1 * rtb_C_0[0]) + (rtb_R_idx_0 *
    rtb_C_0[1]);

  /* MATLAB Function: '<S19>/MATLAB Function1' incorporates:
   *  Delay: '<Root>/Delay'
   */
  posCtrl_i04w0oyqe2(posCtrl_DW.Delay_DSTATE, rtb_C);

  /* Sqrt: '<S25>/Sqrt' */
  rtb_Sqrt_ii2xp45anb = sps_tmp;
  (void)memset(rty_controllerPosCtrl, 0, sizeof(busControllerPosCtrl));

  /* Switch: '<S26>/Switch3' incorporates:
   *  Product: '<S26>/Product'
   */
  if (sps_tmp <= 0.001) {
    rtb_Sqrt_ii2xp45anb = -0.001;
  }

  /* End of Switch: '<S26>/Switch3' */

  /* Product: '<S25>/Divide' incorporates:
   *  Constant: '<S25>/R_0'
   */
  rtb_Sqrt_ii2xp45anb = 6.378137E+6 / rtb_Sqrt_ii2xp45anb;

  /* Product: '<S21>/Product' incorporates:
   *  Delay: '<Root>/Delay'
   *  Product: '<S21>/Product1'
   *  Sum: '<S21>/Sum'
   */
  rtb_Sum_mnw5bny0bd_idx_0_tmp = (rtb_Sqrt_ii2xp45anb + posCtrl_DW.Delay_DSTATE
    [2]) * rtb_Sum1_tmp;

  /* Product: '<S21>/Product3' incorporates:
   *  Constant: '<S21>/1-e^2'
   *  Delay: '<Root>/Delay'
   *  Product: '<S21>/Product2'
   *  Sum: '<S21>/Sum1'
   */
  rtb_Sum_mnw5bny0bd_idx_2 = cps * ((0.993305620009879 * rtb_Sqrt_ii2xp45anb) +
    posCtrl_DW.Delay_DSTATE[2]);

  /* Trigonometry: '<S20>/Trigonometric Function' */
  rtb_TrigonometricFunction = sin(rtu_Sensor->pos_lla[0]);

  /* Product: '<S23>/Product' incorporates:
   *  Constant: '<S23>/e'
   */
  rtb_Sqrt_ii2xp45anb = rtb_TrigonometricFunction * 0.0818191908425;

  /* Sqrt: '<S23>/Sqrt' incorporates:
   *  Constant: '<S23>/const'
   *  Math: '<S23>/Square'
   *  Sum: '<S23>/Add'
   */
  rtb_Sqrt_ii2xp45anb = sqrt(1.0 - (rtb_Sqrt_ii2xp45anb * rtb_Sqrt_ii2xp45anb));

  /* Switch: '<S24>/Switch3' incorporates:
   *  Abs: '<S24>/Abs'
   *  Product: '<S24>/Product'
   */
  if (rtb_Sqrt_ii2xp45anb <= 0.001) {
    rtb_Sqrt_ii2xp45anb = -0.001;
  }

  /* End of Switch: '<S24>/Switch3' */

  /* Product: '<S23>/Divide' incorporates:
   *  Constant: '<S23>/R_0'
   */
  rtb_Sqrt_ii2xp45anb = 6.378137E+6 / rtb_Sqrt_ii2xp45anb;

  /* Product: '<S20>/Product' incorporates:
   *  Product: '<S20>/Product1'
   *  Sum: '<S20>/Sum'
   *  Trigonometry: '<S20>/Trigonometric Function1'
   */
  rtb_Product_haif3f5y2j_idx_0_tmp = (rtb_Sqrt_ii2xp45anb + rtu_Sensor->pos_lla
    [2]) * cos(rtu_Sensor->pos_lla[0]);

  /* Sum: '<S8>/Sum' incorporates:
   *  Constant: '<S20>/1-e^2'
   *  Product: '<S20>/Product'
   *  Product: '<S20>/Product1'
   *  Product: '<S20>/Product2'
   *  Product: '<S20>/Product3'
   *  Product: '<S21>/Product'
   *  Product: '<S21>/Product1'
   *  Sum: '<S20>/Sum1'
   *  Trigonometry: '<S20>/Trigonometric Function2'
   *  Trigonometry: '<S20>/Trigonometric Function3'
   */
  cps = (rtb_Product_haif3f5y2j_idx_0_tmp * cos(rtu_Sensor->pos_lla[1])) -
    (rtb_Sum_mnw5bny0bd_idx_0_tmp * rtb_Product_haif3f5y2j_tmp);
  rtb_Product_haif3f5y2j_tmp_0 = (rtb_Product_haif3f5y2j_idx_0_tmp * sin
    (rtu_Sensor->pos_lla[1])) - (rtb_Sum_mnw5bny0bd_idx_0_tmp *
    rtb_Product_haif3f5y2j_tmp_0);
  rtb_TrigonometricFunction = (rtb_TrigonometricFunction * ((0.993305620009879 *
    rtb_Sqrt_ii2xp45anb) + rtu_Sensor->pos_lla[2])) - rtb_Sum_mnw5bny0bd_idx_2;

  /* Product: '<S19>/Product' */
  for (i = 0; i < 3; i++) {
    rtb_C_0[i] = ((rtb_C[i] * cps) + (rtb_C[i + 3] *
      rtb_Product_haif3f5y2j_tmp_0)) + (rtb_C[i + 6] * rtb_TrigonometricFunction);
  }

  /* End of Product: '<S19>/Product' */

  /* BusAssignment: '<Root>/Bus Assignment1' */
  rty_controllerPosCtrl->psiCmd = rtu_busWptManager->yawCmd;

  /* Product: '<Root>/Product1' incorporates:
   *  Gain: '<S9>/Gain'
   */
  rtb_R_idx_2 = (rtb_R_idx_0 * rtb_C_0[0]) + (rtb_R_idx_2 * rtb_C_0[1]);

  /* BusAssignment: '<Root>/Bus Assignment1' incorporates:
   *  Gain: '<S9>/Gain'
   *  Product: '<Root>/Product'
   *  Product: '<Root>/Product1'
   */
  rty_controllerPosCtrl->pos_xy_cmd[0] = sps;
  rty_controllerPosCtrl->pos_xy[0] = rtb_R_idx_2;

  /* Product: '<Root>/Product1' incorporates:
   *  Gain: '<S9>/Gain'
   */
  rtb_R_idx_1 = (rtb_R_idx_1 * rtb_C_0[0]) + (rtb_R_idx_0 * rtb_C_0[1]);

  /* BusAssignment: '<Root>/Bus Assignment1' incorporates:
   *  Gain: '<S9>/Gain'
   *  Product: '<Root>/Product'
   *  Product: '<Root>/Product1'
   */
  rty_controllerPosCtrl->pos_xy_cmd[1] = rtb_Sum_mnw5bny0bd_idx_0;
  rty_controllerPosCtrl->pos_xy[1] = rtb_R_idx_1;

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  Gain: '<S9>/Gain'
   *  Product: '<Root>/Product'
   *  Product: '<Root>/Product1'
   *  Sum: '<S9>/Sum'
   */
  *rty_busControllerIF_vel_velCmdH_x = 0.25 * (sps - rtb_R_idx_2);

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  Gain: '<S9>/Gain'
   *  Product: '<Root>/Product'
   *  Product: '<Root>/Product1'
   *  Sum: '<S9>/Sum'
   */
  *rty_busControllerIF_vel_velCmdH_y = 0.25 * (rtb_Sum_mnw5bny0bd_idx_0 -
    rtb_R_idx_1);

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  *rty_busControllerIF_vel_hRateCmd = 0.0;

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  *rty_busControllerIF_vel_hCmd = 0.0;

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  *rty_busControllerIF_vel_hHold = false;

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  *rty_busControllerIF_vel_init_reset_vel = false;

  /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  *rty_busControllerIF_vel_init_reset_hdot = false;

  /* Update for Delay: '<Root>/Delay1' */
  posCtrl_DW.icLoad = false;
  posCtrl_DW.Delay1_DSTATE = *rtu_vom_status;

  /* Update for Delay: '<Root>/Delay' */
  posCtrl_DW.icLoad_gwql4htcwe = false;
}

/* Model initialize function */
void posCtrl_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&posCtrl_DW, 0,
                sizeof(posCtrl_TDW));
  posCtrl_PrevZCX.Delay_Reset_ZCE = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
