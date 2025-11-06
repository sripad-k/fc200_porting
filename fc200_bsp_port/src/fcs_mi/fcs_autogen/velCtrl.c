/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: velCtrl.c
 *
 * Code generated for Simulink model 'velCtrl'.
 *
 * Model version                  : 2.9
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:58 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "velCtrl.h"
#include "velCtrl_types.h"
#include "rtwtypes.h"
#include "velCtrl_private.h"
#include <string.h>
#include <math.h>

/* Block states (default storage) */
velCtrl_TDW velCtrl_DW;

/* System initialize for referenced model: 'velCtrl' */
void velCtrl_Init(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' */
  velCtrl_DW.DiscreteTimeIntegrator1_PrevResetState = 0;
}

/* Output and update for referenced model: 'velCtrl' */
void velCtrl(const std_sensor_t *rtu_Sensor, const real_T
             *rtu_ctrlIF_vel_velCmdH_x, const real_T *rtu_ctrlIF_vel_velCmdH_y,
             const boolean_T *rtu_ctrlIF_vel_init_reset_vel, busControllerIF_att
             *rty_attAltCmd, busControllerVelCtrl *rty_controllerVelCtrl)
{
  real_T cps;
  real_T rtb_R_idx_0;
  real_T rtb_R_idx_3;
  real_T rtb_Sum1_idx_0;
  real_T rtb_Sum1_idx_1;
  real_T rtb_Switch_hsy2p5dif5_0;
  real_T rtb_Switch_hsy2p5dif5_idx_0;
  real_T rtb_Switch_hsy2p5dif5_idx_1;
  real_T sps;
  int32_T tmp;
  boolean_T DiscreteTimeIntegrator1_DSTATE_tmp;
  (void)memset(rty_attAltCmd, 0, sizeof(busControllerIF_att));

  /* MATLAB Function: '<S3>/MATLAB Function' */
  /*  psi sa */
  /*  fcn Computes a rotation matrix for a given angle. */
  /*    R = fcn(psi) returns a 2x2 rotation matrix R for the angle psi. */
  /* MATLAB Function 'rot_2D/MATLAB Function': '<S14>:1' */
  /* '<S14>:1:5' cps = cos(psi); */
  cps = cos(rtu_Sensor->eul_ang[2]);

  /* '<S14>:1:6' sps = sin(psi); */
  sps = sin(rtu_Sensor->eul_ang[2]);

  /* '<S14>:1:8' R = [cps, sps; */
  /* '<S14>:1:9'     -sps cps]; */
  rtb_R_idx_0 = cps;
  rtb_R_idx_3 = cps;

  /* Sum: '<S4>/Add' incorporates:
   *  Constant: '<Root>/Constant'
   *  Constant: '<Root>/Constant3'
   *  Constant: '<Root>/Constant4'
   *  Constant: '<S15>/Constant'
   *  Constant: '<S16>/Constant'
   *  Product: '<S4>/Product'
   *  Product: '<S4>/Product1'
   *  RelationalOperator: '<S15>/Compare'
   *  RelationalOperator: '<S16>/Compare'
   */
  rtb_Switch_hsy2p5dif5_idx_0 = (((*rtu_ctrlIF_vel_velCmdH_x) >= 0.0) ? 10.0 :
    0.0) + (((*rtu_ctrlIF_vel_velCmdH_x) < 0.0) ? 3.0 : 0.0);
  rtb_Switch_hsy2p5dif5_idx_1 = (((*rtu_ctrlIF_vel_velCmdH_y) >= 0.0) ? 6.0 :
    0.0) + (((*rtu_ctrlIF_vel_velCmdH_y) < 0.0) ? 6.0 : 0.0);

  /* Switch: '<S20>/Switch3' incorporates:
   *  Abs: '<S20>/Abs'
   *  Product: '<S20>/Product'
   */
  if (rtb_Switch_hsy2p5dif5_idx_0 <= 0.001) {
    rtb_Switch_hsy2p5dif5_idx_0 = 0.001;
  }

  if (rtb_Switch_hsy2p5dif5_idx_1 <= 0.001) {
    rtb_Switch_hsy2p5dif5_idx_1 = 0.001;
  }

  /* End of Switch: '<S20>/Switch3' */

  /* Product: '<S17>/Divide' */
  rtb_Switch_hsy2p5dif5_idx_0 = (*rtu_ctrlIF_vel_velCmdH_x) /
    rtb_Switch_hsy2p5dif5_idx_0;
  rtb_Switch_hsy2p5dif5_idx_1 = (*rtu_ctrlIF_vel_velCmdH_y) /
    rtb_Switch_hsy2p5dif5_idx_1;

  /* Sum: '<S17>/Sum of Elements' incorporates:
   *  Math: '<S17>/Square'
   */
  cps = (rtb_Switch_hsy2p5dif5_idx_0 * rtb_Switch_hsy2p5dif5_idx_0) +
    (rtb_Switch_hsy2p5dif5_idx_1 * rtb_Switch_hsy2p5dif5_idx_1);

  /* Switch: '<S17>/Switch' incorporates:
   *  Constant: '<S18>/Constant'
   *  Product: '<S17>/Divide1'
   *  RelationalOperator: '<S18>/Compare'
   */
  if (cps <= 1.0) {
    rtb_Sum1_idx_0 = *rtu_ctrlIF_vel_velCmdH_x;
    rtb_Sum1_idx_1 = *rtu_ctrlIF_vel_velCmdH_y;
  } else {
    /* Sqrt: '<S17>/Sqrt' */
    cps = sqrt(cps);

    /* Switch: '<S19>/Switch3' incorporates:
     *  Abs: '<S19>/Abs'
     *  Product: '<S19>/Product'
     */
    if (cps <= 0.001) {
      cps = -0.001;
    }

    /* End of Switch: '<S19>/Switch3' */
    rtb_Sum1_idx_0 = (*rtu_ctrlIF_vel_velCmdH_x) / cps;
    rtb_Sum1_idx_1 = (*rtu_ctrlIF_vel_velCmdH_y) / cps;
  }

  /* End of Switch: '<S17>/Switch' */

  /* Product: '<Root>/Product' incorporates:
   *  MATLAB Function: '<S3>/MATLAB Function'
   *  Math: '<Root>/Transpose'
   *  SignalConversion generated from: '<Root>/Vector Concatenate'
   *  Sum: '<S1>/Sum1'
   * */
  cps = (rtb_R_idx_0 * rtb_Sum1_idx_0) + ((-sps) * rtb_Sum1_idx_1);

  /* BusAssignment: '<Root>/Bus Assignment1' incorporates:
   *  Product: '<Root>/Product'
   *  Sum: '<S1>/Sum1'
   */
  rty_controllerVelCtrl->velRef[0] = cps;
  rty_controllerVelCtrl->velCmd[0] = cps;
  rty_controllerVelCtrl->vel_xy_N[0] = rtu_Sensor->vel_ned[0];

  /* Product: '<Root>/Product' incorporates:
   *  MATLAB Function: '<S3>/MATLAB Function'
   *  Math: '<Root>/Transpose'
   *  SignalConversion generated from: '<Root>/Vector Concatenate'
   *  Sum: '<S1>/Sum1'
   * */
  rtb_Sum1_idx_1 = (sps * rtb_Sum1_idx_0) + (rtb_R_idx_3 * rtb_Sum1_idx_1);

  /* BusAssignment: '<Root>/Bus Assignment1' incorporates:
   *  Product: '<Root>/Product'
   *  Sum: '<S1>/Sum1'
   */
  rty_controllerVelCtrl->velRef[1] = rtb_Sum1_idx_1;
  rty_controllerVelCtrl->velCmd[1] = rtb_Sum1_idx_1;
  rty_controllerVelCtrl->vel_xy_N[1] = rtu_Sensor->vel_ned[1];

  /* DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' */
  if ((*rtu_ctrlIF_vel_init_reset_vel) ||
      (velCtrl_DW.DiscreteTimeIntegrator1_PrevResetState != 0)) {
    velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[0] = 0.0;
    velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[1] = 0.0;
  }

  /* Sum: '<S1>/Add' incorporates:
   *  Constant: '<S1>/Constant3'
   *  Constant: '<S1>/b'
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
   *  Product: '<Root>/Product'
   *  Product: '<S1>/Product3'
   *  Product: '<S1>/Product6'
   *  Sum: '<S1>/Sum1'
   *  Sum: '<S1>/Sum2'
   */
  rtb_Switch_hsy2p5dif5_idx_0 = (0.8 * ((0.75 * cps) - rtu_Sensor->vel_ned[0]))
    + velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[0];
  rtb_Sum1_idx_0 = (0.8 * ((0.75 * rtb_Sum1_idx_1) - rtu_Sensor->vel_ned[1])) +
    velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[1];

  /* Product: '<S1>/Product2' incorporates:
   *  MATLAB Function: '<S3>/MATLAB Function'
   */
  rtb_R_idx_0 = (rtb_R_idx_0 * rtb_Switch_hsy2p5dif5_idx_0) + (sps *
    rtb_Sum1_idx_0);

  /* Product: '<S10>/Divide' incorporates:
   *  Product: '<S1>/Product2'
   *  Switch: '<S10>/Switch'
   */
  rtb_Switch_hsy2p5dif5_idx_1 = rtb_R_idx_0 / velCtrl_ConstB.Switch3;

  /* Math: '<S10>/Square' incorporates:
   *  Switch: '<S10>/Switch'
   */
  rtb_Switch_hsy2p5dif5_0 = rtb_Switch_hsy2p5dif5_idx_1 *
    rtb_Switch_hsy2p5dif5_idx_1;

  /* Product: '<S1>/Product2' incorporates:
   *  MATLAB Function: '<S3>/MATLAB Function'
   */
  sps = ((-sps) * rtb_Switch_hsy2p5dif5_idx_0) + (rtb_R_idx_3 * rtb_Sum1_idx_0);

  /* Product: '<S10>/Divide' incorporates:
   *  Product: '<S1>/Product2'
   *  Switch: '<S10>/Switch'
   */
  rtb_Switch_hsy2p5dif5_idx_1 = sps / velCtrl_ConstB.Switch3;

  /* Sum: '<S10>/Sum of Elements' incorporates:
   *  Math: '<S10>/Square'
   *  Switch: '<S10>/Switch'
   */
  rtb_R_idx_3 = rtb_Switch_hsy2p5dif5_0 + (rtb_Switch_hsy2p5dif5_idx_1 *
    rtb_Switch_hsy2p5dif5_idx_1);

  /* Switch: '<S10>/Switch' incorporates:
   *  Constant: '<S11>/Constant'
   *  Product: '<S10>/Divide1'
   *  Product: '<S1>/Product2'
   *  RelationalOperator: '<S11>/Compare'
   */
  if (rtb_R_idx_3 <= 1.0) {
    rtb_Switch_hsy2p5dif5_idx_0 = rtb_R_idx_0;
    rtb_Switch_hsy2p5dif5_idx_1 = sps;
  } else {
    /* Sqrt: '<S10>/Sqrt' */
    rtb_R_idx_3 = sqrt(rtb_R_idx_3);

    /* Switch: '<S12>/Switch3' incorporates:
     *  Abs: '<S12>/Abs'
     *  Product: '<S12>/Product'
     */
    if (rtb_R_idx_3 <= 0.001) {
      rtb_R_idx_3 = -0.001;
    }

    /* End of Switch: '<S12>/Switch3' */
    rtb_Switch_hsy2p5dif5_idx_0 = rtb_R_idx_0 / rtb_R_idx_3;
    rtb_Switch_hsy2p5dif5_idx_1 = sps / rtb_R_idx_3;
  }

  /* End of Switch: '<S10>/Switch' */

  /* Sum: '<S1>/Sum1' incorporates:
   *  Product: '<Root>/Product'
   */
  rtb_Sum1_idx_0 = cps - rtu_Sensor->vel_ned[0];
  rtb_Sum1_idx_1 -= rtu_Sensor->vel_ned[1];

  /* BusAssignment: '<Root>/Bus Assignment2' incorporates:
   *  Gain: '<S8>/Gain'
   *  Gain: '<S8>/Gain1'
   *  Switch: '<S10>/Switch'
   */
  rty_attAltCmd->rollCmd = 0.10197162129779283 * rtb_Switch_hsy2p5dif5_idx_1;
  rty_attAltCmd->pitchCmd = -(0.10197162129779283 * rtb_Switch_hsy2p5dif5_idx_0);

  /* Signum: '<S5>/Sign' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
   *  Product: '<S1>/Product4'
   *  Product: '<S5>/Product'
   *  Sum: '<S1>/Sum1'
   */
  cps = rtb_Sum1_idx_0 * velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[0];
  if (cps < 0.0) {
    tmp = -1;
  } else {
    tmp = (cps > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Logic: '<S5>/AND' incorporates:
   *  Constant: '<S6>/Constant'
   *  Gain: '<S8>/Gain1'
   *  Logic: '<S2>/Logical Operator'
   *  Product: '<S1>/Product2'
   *  RelationalOperator: '<S2>/Relational Operator'
   *  RelationalOperator: '<S6>/Compare'
   *  Switch: '<S10>/Switch'
   */
  DiscreteTimeIntegrator1_DSTATE_tmp = (((real_T)(((rtb_R_idx_0 ==
    rtb_Switch_hsy2p5dif5_idx_0) && (sps == rtb_Switch_hsy2p5dif5_idx_1)) ?
    ((int32_T)1) : ((int32_T)0))) >= 0.95);

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Constant: '<S7>/Constant'
   *  Logic: '<S5>/AND'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product4'
   *  RelationalOperator: '<S7>/Compare'
   *  Signum: '<S5>/Sign'
   *  Sum: '<S1>/Sum1'
   */
  velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[0] += 0.01 * ((0.15 * rtb_Sum1_idx_0)
    * ((real_T)((DiscreteTimeIntegrator1_DSTATE_tmp || (tmp <= 0)) ? ((int32_T)1)
                : ((int32_T)0))));

  /* Signum: '<S5>/Sign' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
   *  Product: '<S1>/Product4'
   *  Product: '<S5>/Product'
   *  Sum: '<S1>/Sum1'
   */
  cps = rtb_Sum1_idx_1 * velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[1];
  if (cps < 0.0) {
    tmp = -1;
  } else {
    tmp = (cps > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Constant: '<S7>/Constant'
   *  Logic: '<S5>/AND'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product4'
   *  RelationalOperator: '<S7>/Compare'
   *  Signum: '<S5>/Sign'
   *  Sum: '<S1>/Sum1'
   */
  velCtrl_DW.DiscreteTimeIntegrator1_DSTATE[1] += 0.01 * ((0.15 * rtb_Sum1_idx_1)
    * ((real_T)((DiscreteTimeIntegrator1_DSTATE_tmp || (tmp <= 0)) ? ((int32_T)1)
                : ((int32_T)0))));
  velCtrl_DW.DiscreteTimeIntegrator1_PrevResetState = (int8_T)
    ((*rtu_ctrlIF_vel_init_reset_vel) ? 1 : 0);
}

/* Model initialize function */
void velCtrl_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&velCtrl_DW, 0,
                sizeof(velCtrl_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
