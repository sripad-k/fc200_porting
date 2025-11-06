/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: attCtrl.c
 *
 * Code generated for Simulink model 'attCtrl'.
 *
 * Model version                  : 2.67
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:03 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "attCtrl.h"
#include "rtwtypes.h"
#include "attCtrl_types.h"
#include "attCtrl_private.h"
#include <math.h>
#include "rt_modd.h"
#include "look1_plinlc.h"
#include <string.h>
#define attCtrl_period                 (0.01)

/* Block states (default storage) */
attCtrl_TDW attCtrl_DW;

/* System initialize for referenced model: 'attCtrl' */
void attCtrl_Init(void)
{
  /* InitializeConditions for RateLimiter: '<S6>/Rate Limiter' */
  attCtrl_DW.PrevY[0] = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[0] =
    attCtrl_ConstB.integralResetValue;

  /* InitializeConditions for RateLimiter: '<S6>/Rate Limiter' */
  attCtrl_DW.PrevY[1] = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[1] =
    attCtrl_ConstB.integralResetValue;
  attCtrl_DW.DiscreteTimeIntegrator_PrevResetState = 0;

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter3' */
  attCtrl_DW.PrevY_fueqrscxop = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0 =
    attCtrl_ConstB.integralResetValue_hxh4svakxk;
  attCtrl_DW.DiscreteTimeIntegrator_PrevResetState_pabim4iwvd = 0;
}

/* Output and update for referenced model: 'attCtrl' */
void attCtrl(const real_T *rtu_busControllerIF_attAlt_rollCmd, const real_T
             *rtu_busControllerIF_attAlt_pitchCmd, const real_T
             *rtu_busControllerIF_attAlt_yawCmd, const real_T
             *rtu_busControllerIF_attAlt_yawRateCmd, const boolean_T
             *rtu_busControllerIF_attAlt_yawHold, const real_T
             *rtu_busControllerIF_attAlt_yawFF, const boolean_T
             *rtu_busControllerIF_attAlt_init_reset, const std_sensor_t
             *rtu_Sensor, const real_T *rtu_busControllerCA_c_erp1, const real_T
             *rtu_busControllerCA_c_erp3, const real_T *rtu_MR_IntCF_roll_CF,
             const real_T *rtu_MR_IntCF_pitch_CF, const real_T
             *rtu_MR_IntCF_lambda, busControllerAttCtrl *rty_controllerAttCtrl,
             real_T *rty_MR_Int_roll_int, real_T *rty_MR_Int_pitch_int, real_T
             *rty_MR_Int_roll_sat, real_T *rty_MR_Int_pitch_sat)
{
  real_T cp;
  real_T ct;
  real_T rtb_Product2_0;
  real_T rtb_Switch_oa0wuqozo0;
  real_T rtb_VectorConcatenate1_kdvnt1zcqc_idx_0;
  real_T rtb_VectorConcatenate2_0;
  real_T rtb_VectorConcatenate2_idx_0;
  real_T rtb_VectorConcatenate2_idx_1;
  real_T rtb_VectorConcatenate_0;
  real_T rtb_VectorConcatenate_idx_0;
  real_T sp;
  real_T tt;
  real_T u;
  int32_T i;
  int32_T rtb_VectorConcatenate3_0;
  int32_T rtb_VectorConcatenate3_idx_0;
  boolean_T rtb_VectorConcatenate3_tmp;

  /* Lookup_n-D: '<Root>/1-D Lookup Table' */
  rtb_Switch_oa0wuqozo0 = look1_plinlc(rtu_Sensor->aspd_cas,
    attCtrl_ConstP.uDLookupTable_bp01Data,
    attCtrl_ConstP.uDLookupTable_tableData, &attCtrl_DW.m_bpIndex, 1U);

  /* RateLimiter: '<S6>/Rate Limiter' incorporates:
   *  DiscreteIntegrator: '<S10>/Discrete-Time Integrator'
   */
  rtb_VectorConcatenate2_0 = attCtrl_DW.DiscreteTimeIntegrator_DSTATE[0] -
    attCtrl_DW.PrevY[0];
  if (rtb_VectorConcatenate2_0 > 0.0078539816339744835) {
    rtb_VectorConcatenate2_0 = attCtrl_DW.PrevY[0] + 0.0078539816339744835;
  } else if (rtb_VectorConcatenate2_0 < -0.0078539816339744835) {
    rtb_VectorConcatenate2_0 = attCtrl_DW.PrevY[0] - 0.0078539816339744835;
  } else {
    rtb_VectorConcatenate2_0 = attCtrl_DW.DiscreteTimeIntegrator_DSTATE[0];
  }

  rtb_VectorConcatenate2_idx_0 = rtb_VectorConcatenate2_0;
  attCtrl_DW.PrevY[0] = rtb_VectorConcatenate2_0;
  rtb_VectorConcatenate2_0 = attCtrl_DW.DiscreteTimeIntegrator_DSTATE[1] -
    attCtrl_DW.PrevY[1];
  if (rtb_VectorConcatenate2_0 > 0.0078539816339744835) {
    rtb_VectorConcatenate2_0 = attCtrl_DW.PrevY[1] + 0.0078539816339744835;
  } else if (rtb_VectorConcatenate2_0 < -0.0078539816339744835) {
    rtb_VectorConcatenate2_0 = attCtrl_DW.PrevY[1] - 0.0078539816339744835;
  } else {
    rtb_VectorConcatenate2_0 = attCtrl_DW.DiscreteTimeIntegrator_DSTATE[1];
  }

  attCtrl_DW.PrevY[1] = rtb_VectorConcatenate2_0;

  /* MATLAB Function: '<S4>/MATLAB Function' */
  /*  omegaB sds */
  /*  euler asd */
  /*  eulerdot asd */
  /*  omega2eulerdot qwe */
  /*  ref: groves 2nd ed. page 46 eqn 2.58 */
  /* MATLAB Function 'omega2eulerdot/MATLAB Function': '<S18>:1' */
  /* '<S18>:1:8' eulerdot = zeros(3,1); */
  /* '<S18>:1:9' sp = sin(euler(1)); */
  sp = sin(rtu_Sensor->eul_ang[0]);

  /* '<S18>:1:10' cp = cos(euler(1)); */
  cp = cos(rtu_Sensor->eul_ang[0]);

  /* '<S18>:1:11' ct = cos(euler(2)); */
  ct = cos(rtu_Sensor->eul_ang[1]);

  /* '<S18>:1:12' tt = tan(euler(2)); */
  tt = tan(rtu_Sensor->eul_ang[1]);

  /* Sum: '<S6>/Sum2' incorporates:
   *  Constant: '<S6>/Constant'
   *  MATLAB Function: '<S4>/MATLAB Function'
   *  Product: '<S6>/Product1'
   *  RateLimiter: '<S6>/Rate Limiter'
   *  SignalConversion generated from: '<S6>/Vector Concatenate'
   *  Sum: '<S6>/Sum1'
   * */
  /* '<S18>:1:14' eulerdot(1) = omegaB(1) + sp*tt*omegaB(2) + cp*tt*omegaB(3); */
  /* '<S18>:1:15' eulerdot(2) = cp*omegaB(2) - sp*omegaB(3); */
  /* '<S18>:1:16' eulerdot(3) = sp/ct*omegaB(2) + cp/ct*omegaB(3); */
  rtb_VectorConcatenate_idx_0 = (1.526 * (rtb_VectorConcatenate2_idx_0 -
    rtu_Sensor->eul_ang[0])) - ((rtu_Sensor->omg[0] + ((sp * tt) *
    rtu_Sensor->omg[1])) + ((cp * tt) * rtu_Sensor->omg[2]));
  tt = (1.526 * (rtb_VectorConcatenate2_0 - rtu_Sensor->eul_ang[1])) - ((cp *
    rtu_Sensor->omg[1]) - (sp * rtu_Sensor->omg[2]));

  /* Sum: '<S6>/Sum' incorporates:
   *  Constant: '<S6>/Constant4'
   *  Product: '<S6>/Divide'
   */
  rtb_VectorConcatenate2_idx_0 = rtb_VectorConcatenate_idx_0 +
    ((*rtu_MR_IntCF_roll_CF) / 4.511);
  rtb_VectorConcatenate2_idx_1 = tt + ((*rtu_MR_IntCF_pitch_CF) / 4.511);

  /* DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  if ((*rtu_busControllerIF_attAlt_init_reset) ||
      (attCtrl_DW.DiscreteTimeIntegrator_PrevResetState != 0)) {
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[0] =
      attCtrl_ConstB.integralResetValue;
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[1] =
      attCtrl_ConstB.integralResetValue;
  }

  /* Product: '<S7>/Product' incorporates:
   *  DiscreteIntegrator: '<S6>/Discrete-Time Integrator'
   */
  u = rtb_VectorConcatenate2_idx_0 *
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[0];

  /* Signum: '<S7>/Sign' */
  if (u < 0.0) {
    i = -1;
  } else {
    i = (u > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Logic: '<S7>/AND' incorporates:
   *  Constant: '<S11>/Constant'
   *  RelationalOperator: '<S11>/Compare'
   */
  rtb_VectorConcatenate3_tmp = ((*rtu_busControllerCA_c_erp1) >= 0.95);

  /* Switch: '<S7>/Switch' incorporates:
   *  Constant: '<S12>/Constant'
   *  Logic: '<S7>/AND'
   *  RelationalOperator: '<S12>/Compare'
   *  Signum: '<S7>/Sign'
   */
  rtb_VectorConcatenate3_0 = (rtb_VectorConcatenate3_tmp || (i <= 0)) ?
    ((int32_T)1) : ((int32_T)0);
  rtb_VectorConcatenate3_idx_0 = rtb_VectorConcatenate3_0;

  /* Product: '<S6>/Product5' incorporates:
   *  Bias: '<S6>/Bias'
   *  Gain: '<S6>/Gain2'
   */
  rtb_VectorConcatenate_0 = rtb_VectorConcatenate_idx_0 *
    ((-(*rtu_MR_IntCF_lambda)) + 1.0);

  /* Product: '<S6>/Product2' incorporates:
   *  Constant: '<S6>/Constant1'
   */
  rtb_Product2_0 = 5.901 * rtb_VectorConcatenate_0;

  /* Product: '<S9>/Product' incorporates:
   *  Constant: '<S9>/Constant3'
   *  DiscreteIntegrator: '<S9>/omega'
   *  Sum: '<S9>/Sum'
   */
  u = 40.0 * (rtb_VectorConcatenate_0 - attCtrl_DW.omega_DSTATE[0]);
  rtb_VectorConcatenate_idx_0 = u;

  /* Product: '<S6>/Product6' incorporates:
   *  Bias: '<S6>/Bias'
   *  DiscreteIntegrator: '<S6>/Discrete-Time Integrator'
   *  Gain: '<S6>/Gain2'
   */
  rtb_VectorConcatenate_0 = ((-(*rtu_MR_IntCF_lambda)) + 1.0) *
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[0];

  /* SignalConversion generated from: '<Root>/MR_Int' incorporates:
   *  Product: '<S6>/Product6'
   */
  *rty_MR_Int_roll_int = rtb_VectorConcatenate_0;

  /* Sum: '<S6>/Add' incorporates:
   *  Constant: '<S6>/Constant3'
   *  Product: '<S6>/Product4'
   */
  rtb_VectorConcatenate1_kdvnt1zcqc_idx_0 = (rtb_Product2_0 + (0.051 * u)) +
    rtb_VectorConcatenate_0;

  /* Product: '<S6>/Product' incorporates:
   *  Product: '<S7>/Product'
   */
  rtb_VectorConcatenate2_0 = rtb_VectorConcatenate2_idx_0 * ((real_T)
    rtb_VectorConcatenate3_0);

  /* Product: '<S7>/Product' incorporates:
   *  DiscreteIntegrator: '<S6>/Discrete-Time Integrator'
   */
  u = rtb_VectorConcatenate2_idx_1 *
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[1];

  /* Signum: '<S7>/Sign' */
  if (u < 0.0) {
    i = -1;
  } else {
    i = (u > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Switch: '<S7>/Switch' incorporates:
   *  Constant: '<S12>/Constant'
   *  Logic: '<S7>/AND'
   *  RelationalOperator: '<S12>/Compare'
   *  Signum: '<S7>/Sign'
   */
  rtb_VectorConcatenate3_0 = (rtb_VectorConcatenate3_tmp || (i <= 0)) ?
    ((int32_T)1) : ((int32_T)0);

  /* Product: '<S6>/Product5' incorporates:
   *  Bias: '<S6>/Bias'
   *  Gain: '<S6>/Gain2'
   */
  rtb_VectorConcatenate_0 = tt * ((-(*rtu_MR_IntCF_lambda)) + 1.0);

  /* Product: '<S6>/Product2' incorporates:
   *  Constant: '<S6>/Constant1'
   */
  rtb_Product2_0 = 5.901 * rtb_VectorConcatenate_0;

  /* Product: '<S9>/Product' incorporates:
   *  Constant: '<S9>/Constant3'
   *  DiscreteIntegrator: '<S9>/omega'
   *  Sum: '<S9>/Sum'
   */
  u = 40.0 * (rtb_VectorConcatenate_0 - attCtrl_DW.omega_DSTATE[1]);

  /* Product: '<S6>/Product6' incorporates:
   *  Bias: '<S6>/Bias'
   *  DiscreteIntegrator: '<S6>/Discrete-Time Integrator'
   *  Gain: '<S6>/Gain2'
   */
  rtb_VectorConcatenate_0 = ((-(*rtu_MR_IntCF_lambda)) + 1.0) *
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[1];

  /* Sum: '<S6>/Add' incorporates:
   *  Constant: '<S6>/Constant3'
   *  Product: '<S6>/Product4'
   */
  rtb_VectorConcatenate2_idx_0 = (rtb_Product2_0 + (0.051 * u)) +
    rtb_VectorConcatenate_0;

  /* Switch: '<Root>/Switch' incorporates:
   *  Sum: '<Root>/Sum'
   */
  if (*rtu_busControllerIF_attAlt_yawHold) {
    /* Gain: '<S5>/Gain' incorporates:
     *  Constant: '<S20>/Constant2'
     *  Math: '<S20>/Mod2'
     *  Math: '<S20>/Mod3'
     *  Sum: '<S20>/Add3'
     *  Sum: '<S20>/Add4'
     *  Sum: '<S20>/Sum'
     *  Sum: '<S5>/Sum'
     */
    tt = 0.5 * (rt_modd(rt_modd((((*rtu_busControllerIF_attAlt_yawCmd) -
      rtu_Sensor->eul_ang[2]) - -3.1415926535897931) + attCtrl_ConstB.Sum1,
      attCtrl_ConstB.Sum1), attCtrl_ConstB.Sum1) - 3.1415926535897931);

    /* Switch: '<S19>/Switch2' incorporates:
     *  Constant: '<S5>/Constant2'
     *  RelationalOperator: '<S19>/LowerRelop1'
     *  RelationalOperator: '<S19>/UpperRelop'
     *  Switch: '<S19>/Switch'
     */
    if (tt > 0.52359877559829882) {
      tt = 0.52359877559829882;
    } else if (tt < attCtrl_ConstB.Gain1) {
      /* Switch: '<S19>/Switch' */
      tt = attCtrl_ConstB.Gain1;
    } else {
      /* no actions */
    }

    /* End of Switch: '<S19>/Switch2' */
    tt += *rtu_busControllerIF_attAlt_yawFF;
  } else {
    tt = *rtu_busControllerIF_attAlt_yawRateCmd;
  }

  /* End of Switch: '<Root>/Switch' */

  /* Saturate: '<Root>/Saturation' */
  if (tt > 0.52359877559829882) {
    tt = 0.52359877559829882;
  } else if (tt < -0.52359877559829882) {
    tt = -0.52359877559829882;
  } else {
    /* no actions */
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* RateLimiter: '<Root>/Rate Limiter3' */
  rtb_Product2_0 = tt - attCtrl_DW.PrevY_fueqrscxop;
  if (rtb_Product2_0 > attCtrl_period) {
    tt = attCtrl_DW.PrevY_fueqrscxop + attCtrl_period;
  } else if (rtb_Product2_0 < -0.01) {
    tt = attCtrl_DW.PrevY_fueqrscxop - 0.01;
  } else {
    /* no actions */
  }

  attCtrl_DW.PrevY_fueqrscxop = tt;

  /* End of RateLimiter: '<Root>/Rate Limiter3' */

  /* Sum: '<S2>/Sum1' incorporates:
   *  MATLAB Function: '<S4>/MATLAB Function'
   */
  tt -= ((sp / ct) * rtu_Sensor->omg[1]) + ((cp / ct) * rtu_Sensor->omg[2]);

  /* DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  if ((*rtu_busControllerIF_attAlt_init_reset) ||
      (attCtrl_DW.DiscreteTimeIntegrator_PrevResetState_pabim4iwvd != 0)) {
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0 =
      attCtrl_ConstB.integralResetValue_hxh4svakxk;
  }

  /* Product: '<Root>/Product' incorporates:
   *  Bias: '<Root>/Bias'
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   *  Gain: '<Root>/Gain'
   *  Gain: '<S2>/Gain'
   *  Sum: '<S2>/Add'
   */
  sp = ((2.0 * tt) + attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0) * ((-(*
    rtu_MR_IntCF_lambda)) + 1.0);

  /* SignalConversion generated from: '<Root>/controllerAttCtrl' incorporates:
   *  Concatenate: '<Root>/Vector Concatenate1'
   *  Gain: '<Root>/Multiply'
   */
  rty_controllerAttCtrl->rollCmd = *rtu_busControllerIF_attAlt_rollCmd;
  rty_controllerAttCtrl->pitchCmd = *rtu_busControllerIF_attAlt_pitchCmd;
  rty_controllerAttCtrl->yawRateCmd = *rtu_busControllerIF_attAlt_yawRateCmd;
  for (i = 0; i < 3; i++) {
    rty_controllerAttCtrl->momentDes[i] = ((attCtrl_ConstP.Multiply_Gain[i] *
      rtb_VectorConcatenate1_kdvnt1zcqc_idx_0) + (attCtrl_ConstP.Multiply_Gain[i
      + 3] * rtb_VectorConcatenate2_idx_0)) + (attCtrl_ConstP.Multiply_Gain[i +
      6] * sp);
  }

  /* End of SignalConversion generated from: '<Root>/controllerAttCtrl' */

  /* SignalConversion generated from: '<Root>/MR_Int' incorporates:
   *  Product: '<S6>/Product6'
   */
  *rty_MR_Int_pitch_int = rtb_VectorConcatenate_0;

  /* SignalConversion generated from: '<Root>/MR_Int' incorporates:
   *  Constant: '<S13>/Constant'
   *  Logic: '<S8>/Logical Operator'
   *  RelationalOperator: '<S13>/Compare'
   */
  *rty_MR_Int_roll_sat = (real_T)(((*rtu_busControllerIF_attAlt_init_reset) ||
    (rtb_VectorConcatenate3_idx_0 == 0)) ? ((int32_T)1) : ((int32_T)0));

  /* SignalConversion generated from: '<Root>/MR_Int' incorporates:
   *  Constant: '<S14>/Constant'
   *  Logic: '<S8>/Logical Operator1'
   *  RelationalOperator: '<S14>/Compare'
   *  Switch: '<S7>/Switch'
   */
  *rty_MR_Int_pitch_sat = (real_T)(((*rtu_busControllerIF_attAlt_init_reset) ||
    (rtb_VectorConcatenate3_0 == 0)) ? ((int32_T)1) : ((int32_T)0));

  /* Sum: '<S2>/Sum' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   *  Product: '<S2>/Product1'
   */
  tt -= rtb_Switch_oa0wuqozo0 *
    attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0;

  /* Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' */
  attCtrl_DW.DiscreteTimeIntegrator_PrevResetState = (int8_T)
    ((*rtu_busControllerIF_attAlt_init_reset) ? 1 : 0);

  /* Saturate: '<Root>/Saturation1' */
  if ((*rtu_busControllerIF_attAlt_rollCmd) > 0.52359877559829882) {
    rtb_Switch_oa0wuqozo0 = 0.52359877559829882;
  } else if ((*rtu_busControllerIF_attAlt_rollCmd) < -0.52359877559829882) {
    rtb_Switch_oa0wuqozo0 = -0.52359877559829882;
  } else {
    rtb_Switch_oa0wuqozo0 = *rtu_busControllerIF_attAlt_rollCmd;
  }

  /* Update for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S10>/Gain'
   *  Saturate: '<Root>/Saturation1'
   *  Sum: '<S10>/Sum'
   */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE[0] += 0.01 * (10.0 *
    (rtb_Switch_oa0wuqozo0 - attCtrl_DW.DiscreteTimeIntegrator_DSTATE[0]));

  /* Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S6>/Constant2'
   *  Product: '<S6>/Product'
   *  Product: '<S6>/Product3'
   */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[0] += 0.01 * (4.511 *
    rtb_VectorConcatenate2_0);

  /* Update for DiscreteIntegrator: '<S9>/omega' */
  attCtrl_DW.omega_DSTATE[0] += 0.01 * rtb_VectorConcatenate_idx_0;

  /* Saturate: '<Root>/Saturation2' */
  if ((*rtu_busControllerIF_attAlt_pitchCmd) > 0.52359877559829882) {
    rtb_Switch_oa0wuqozo0 = 0.52359877559829882;
  } else if ((*rtu_busControllerIF_attAlt_pitchCmd) < -0.52359877559829882) {
    rtb_Switch_oa0wuqozo0 = -0.52359877559829882;
  } else {
    rtb_Switch_oa0wuqozo0 = *rtu_busControllerIF_attAlt_pitchCmd;
  }

  /* Update for DiscreteIntegrator: '<S10>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S10>/Gain'
   *  Saturate: '<Root>/Saturation2'
   *  Sum: '<S10>/Sum'
   */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE[1] += 0.01 * (10.0 *
    (rtb_Switch_oa0wuqozo0 - attCtrl_DW.DiscreteTimeIntegrator_DSTATE[1]));

  /* Update for DiscreteIntegrator: '<S6>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S6>/Constant2'
   *  Product: '<S6>/Product'
   *  Product: '<S6>/Product3'
   *  Product: '<S7>/Product'
   */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[1] += 0.01 * (4.511 *
    (rtb_VectorConcatenate2_idx_1 * ((real_T)rtb_VectorConcatenate3_0)));

  /* Update for DiscreteIntegrator: '<S9>/omega' incorporates:
   *  Product: '<S9>/Product'
   */
  attCtrl_DW.omega_DSTATE[1] += 0.01 * u;

  /* Product: '<S15>/Product' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   */
  u = tt * attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0;

  /* Signum: '<S15>/Sign' */
  if (u < 0.0) {
    i = -1;
  } else {
    i = (u > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S16>/Constant'
   *  Constant: '<S17>/Constant'
   *  Constant: '<S2>/Constant2'
   *  Logic: '<S15>/AND'
   *  Product: '<S2>/Product'
   *  Product: '<S2>/Product3'
   *  RelationalOperator: '<S16>/Compare'
   *  RelationalOperator: '<S17>/Compare'
   *  Signum: '<S15>/Sign'
   */
  attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0 += 0.01 * (0.5 * (tt *
    ((real_T)((((*rtu_busControllerCA_c_erp3) >= 0.95) || (i <= 0)) ? ((int32_T)
    1) : ((int32_T)0)))));
  attCtrl_DW.DiscreteTimeIntegrator_PrevResetState_pabim4iwvd = (int8_T)
    ((*rtu_busControllerIF_attAlt_init_reset) ? 1 : 0);
}

/* Model initialize function */
void attCtrl_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&attCtrl_DW, 0,
                sizeof(attCtrl_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
