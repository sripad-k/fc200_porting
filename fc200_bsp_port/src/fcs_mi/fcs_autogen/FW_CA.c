/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_CA.c
 *
 * Code generated for Simulink model 'FW_CA'.
 *
 * Model version                  : 2.106
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:50:05 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FW_CA.h"
#include "rtwtypes.h"
#include "FW_CA_types.h"
#include "FW_CA_private.h"
#include <string.h>
#include "look1_plinlc.h"

/* Block states (default storage) */
FW_CA_TDW FW_CA_DW;

/*
 * Output and update for atomic system:
 *    '<S2>/MATLAB Function'
 *    '<S3>/MATLAB Function'
 *    '<S5>/MATLAB Function'
 */
void FW_CA_i10nsmykqz(real_T rtu_nu, const real_T rtu_B[2], const real_T
                      rtu_B_inv[2], const real_T rtu_lb[2], const real_T rtu_ub
                      [2], real_T rty_u_out[2], boolean_T *rty_sat_flag)
{
  real_T u0;
  real_T u0_idx_0;
  real_T u_sat_idx_0;
  real_T u_sat_idx_1;
  boolean_T u_sat1;
  boolean_T u_sat2;

  /*  CA block for control surface allocation, inspired from CGI. */
  /*  assumes that the problem is single axis, and there is only 2 actuators. */
  /* MATLAB Function 'CA_aileron/MATLAB Function': '<S12>:1' */
  /*  This way, there is no need to compute B_inv again when an actuator */
  /*  saturates. */
  /* '<S12>:1:8' nu_sat = nu; */
  /*  to be used if there is saturation */
  /* '<S12>:1:9' u0 = B_inv*nu; */
  /* '<S12>:1:10' u_out = [0;0]; */
  /* '<S12>:1:12' u_sat = min(max(u0,lb),ub); */
  u0 = rtu_B_inv[0] * rtu_nu;
  u0_idx_0 = u0;
  if (u0 >= rtu_lb[0]) {
    u_sat_idx_1 = u0;
  } else {
    u_sat_idx_1 = rtu_lb[0];
  }

  if (u_sat_idx_1 <= rtu_ub[0]) {
    u_sat_idx_0 = u_sat_idx_1;
  } else {
    u_sat_idx_0 = rtu_ub[0];
  }

  u0 = rtu_B_inv[1] * rtu_nu;
  if (u0 >= rtu_lb[1]) {
    u_sat_idx_1 = u0;
  } else {
    u_sat_idx_1 = rtu_lb[1];
  }

  if (u_sat_idx_1 > rtu_ub[1]) {
    u_sat_idx_1 = rtu_ub[1];
  }

  /* '<S12>:1:14' u_sat1 = (u_sat(1) ~= u0(1)); */
  u_sat1 = (u_sat_idx_0 != u0_idx_0);

  /* '<S12>:1:15' u_sat2 = (u_sat(2) ~= u0(2)); */
  u_sat2 = (u_sat_idx_1 != u0);

  /* '<S12>:1:17' sat_flag = true; */
  *rty_sat_flag = true;

  /* '<S12>:1:18' if(u_sat1 && u_sat2) */
  if (u_sat1 && u_sat2) {
    /*  both saturated, just return the saturated vector */
    /* '<S12>:1:20' u_out = u_sat; */
    rty_u_out[0] = u_sat_idx_0;
    rty_u_out[1] = u_sat_idx_1;

    /* '<S12>:1:21' sat_flag = false; */
    *rty_sat_flag = false;
  } else if (u_sat1) {
    /* '<S12>:1:22' elseif(u_sat1) */
    /*  first one is saturated */
    /* '<S12>:1:24' u_out(1) = u_sat(1); */
    rty_u_out[0] = u_sat_idx_0;

    /* '<S12>:1:25' nu_sat = nu_sat - B(1)*u_sat(1); */
    /*  drop one column from B, and pinv will be doubled. */
    /* '<S12>:1:27' u_sat(2) = 2*B_inv(2)*nu_sat; */
    u_sat_idx_1 = (2.0 * rtu_B_inv[1]) * (rtu_nu - (rtu_B[0] * u_sat_idx_0));

    /*  saturate again */
    /* '<S12>:1:29' u_out(2) = min(max(u_sat(2),lb(2)),ub(2)); */
    if (u_sat_idx_1 >= rtu_lb[1]) {
      u0 = u_sat_idx_1;
    } else {
      u0 = rtu_lb[1];
    }

    if (u0 <= rtu_ub[1]) {
      rty_u_out[1] = u0;
    } else {
      rty_u_out[1] = rtu_ub[1];
    }

    /*  check if 2nd is also saturated */
    /* '<S12>:1:31' if(u_out(2) ~= u_sat(2)) */
    if (rty_u_out[1] != u_sat_idx_1) {
      /* '<S12>:1:32' sat_flag = false; */
      *rty_sat_flag = false;
    }
  } else if (u_sat2) {
    /* '<S12>:1:34' elseif(u_sat2) */
    /*  2nd one is saturated */
    /* '<S12>:1:36' u_out(2) = u_sat(2); */
    rty_u_out[1] = u_sat_idx_1;

    /* '<S12>:1:37' nu_sat = nu_sat - B(2)*u_sat(2); */
    /*  drop one column from B, and pinv will be doubled. */
    /* '<S12>:1:39' u_sat(1) = 2*B_inv(1)*nu_sat; */
    u_sat_idx_0 = (2.0 * rtu_B_inv[0]) * (rtu_nu - (rtu_B[1] * u_sat_idx_1));

    /*  saturate again */
    /* '<S12>:1:41' u_out(1) = min(max(u_sat(1),lb(1)),ub(1)); */
    if (u_sat_idx_0 >= rtu_lb[0]) {
      u0 = u_sat_idx_0;
    } else {
      u0 = rtu_lb[0];
    }

    if (u0 <= rtu_ub[0]) {
      rty_u_out[0] = u0;
    } else {
      rty_u_out[0] = rtu_ub[0];
    }

    /*  check if 1st is also saturated */
    /* '<S12>:1:43' if(u_out(1) ~= u_sat(1)) */
    if (rty_u_out[0] != u_sat_idx_0) {
      /* '<S12>:1:44' sat_flag = false; */
      *rty_sat_flag = false;
    }
  } else {
    /* '<S12>:1:46' else */
    /*  no saturation, move on. */
    /* '<S12>:1:48' u_out = u_sat; */
    rty_u_out[0] = u_sat_idx_0;
    rty_u_out[1] = u_sat_idx_1;
  }
}

/* System initialize for referenced model: 'FW_CA' */
void FW_CA_Init(void)
{
  /* InitializeConditions for RateLimiter: '<S10>/20%_per_sec' */
  FW_CA_DW.PrevY = 0.0;

  /* InitializeConditions for RateLimiter: '<S10>/Rate Limiter3' */
  FW_CA_DW.PrevY_hbjyry413j = 0.0;

  /* InitializeConditions for RateLimiter: '<S10>/Rate Limiter4' */
  FW_CA_DW.PrevY_djckibarev = 0.0;

  /* InitializeConditions for RateLimiter: '<S10>/Rate Limiter1' */
  FW_CA_DW.PrevY_cf2xsvidl4 = 0.0;
}

/* Output and update for referenced model: 'FW_CA' */
void FW_CA(const vom_t *rtu_vom_status, const real_T *rtu_elevator, const real_T
           *rtu_aileron, const real_T *rtu_rudder, const real_T *rtu_engineCmd,
           const std_sensor_t *rtu_Sensor, const real_T *rtu_pilot_roll_ch,
           const real_T *rtu_pilot_pitch_ch, const real_T *rtu_pilot_yaw_ch,
           const real_T *rtu_pilot_engine_ch, busControllerCA *rty_controllerCA)
{
  std_ctrl_t rtb_BusAssignment2;
  real_T rtb_Product_dq4dbzhv5s[12];
  real_T rtb_Product[2];
  real_T rtb_Product1_lwaim5ixbq[2];
  real_T rtb_u_out[2];
  real_T rtb_u_out_cuqvwmtzjx[2];
  real_T rtb_u_out_i1bd14vhxu[2];
  real_T rtb_CastToDouble;
  real_T rtb_CastToDouble_g5aahf3td0;
  real_T rtb_DiscreteTimeIntegrator_k5rcxc0xuj;
  real_T rtb_Switch;
  boolean_T rtb_OR;
  boolean_T rtb_OR1;
  boolean_T rtb_OR2;
  boolean_T rtb_sat_flag;
  boolean_T rtb_sat_flag_eyf34dhd5h;
  boolean_T rtb_sat_flag_owj3wsswku;

  /* Lookup_n-D: '<Root>/1-D Lookup Table1' */
  rtb_CastToDouble = look1_plinlc(rtu_Sensor->aspd_cas,
    FW_CA_ConstP.uDLookupTable1_bp01Data, FW_CA_ConstP.uDLookupTable1_tableData,
    &FW_CA_DW.m_bpIndex, 1U);

  /* Logic: '<S24>/OR2' incorporates:
   *  Constant: '<S25>/Constant'
   *  Constant: '<S26>/Constant'
   *  Constant: '<S27>/Constant'
   *  Constant: '<S28>/Constant'
   *  Constant: '<S29>/Constant'
   *  Constant: '<S30>/Constant'
   *  RelationalOperator: '<S25>/Compare'
   *  RelationalOperator: '<S26>/Compare'
   *  RelationalOperator: '<S27>/Compare'
   *  RelationalOperator: '<S28>/Compare'
   *  RelationalOperator: '<S29>/Compare'
   *  RelationalOperator: '<S30>/Compare'
   */
  rtb_OR2 = (((((((*rtu_vom_status) == F_TRANS) || ((*rtu_vom_status) == B_TRANS))
                || ((*rtu_vom_status) == FLTDIR)) || ((*rtu_vom_status) ==
    LOITER)) || ((*rtu_vom_status) == WAYPNT)) || ((*rtu_vom_status) == FW_RTH));

  /* Logic: '<S8>/OR' incorporates:
   *  Constant: '<S19>/Constant'
   *  Constant: '<S20>/Constant'
   *  RelationalOperator: '<S19>/Compare'
   *  RelationalOperator: '<S20>/Compare'
   */
  rtb_OR = (((*rtu_vom_status) == UMAN) || ((*rtu_vom_status) == STARTUP));

  /* Saturate: '<S10>/Saturation1' */
  if ((*rtu_pilot_engine_ch) > 0.2) {
    rtb_CastToDouble_g5aahf3td0 = 0.2;
  } else if ((*rtu_pilot_engine_ch) < 0.0) {
    rtb_CastToDouble_g5aahf3td0 = 0.0;
  } else {
    rtb_CastToDouble_g5aahf3td0 = *rtu_pilot_engine_ch;
  }

  /* End of Saturate: '<S10>/Saturation1' */

  /* RateLimiter: '<S10>/20%_per_sec' */
  rtb_DiscreteTimeIntegrator_k5rcxc0xuj = rtb_CastToDouble_g5aahf3td0 -
    FW_CA_DW.PrevY;
  if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj > 0.002) {
    rtb_CastToDouble_g5aahf3td0 = FW_CA_DW.PrevY + 0.002;
  } else if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj < -0.002) {
    rtb_CastToDouble_g5aahf3td0 = FW_CA_DW.PrevY - 0.002;
  } else {
    /* no actions */
  }

  FW_CA_DW.PrevY = rtb_CastToDouble_g5aahf3td0;

  /* End of RateLimiter: '<S10>/20%_per_sec' */

  /* RelationalOperator: '<S17>/Compare' incorporates:
   *  Constant: '<S17>/Constant'
   */
  rtb_OR1 = ((*rtu_vom_status) == STARTUP);

  /* Switch: '<S4>/Switch' */
  if (!rtb_OR1) {
    rtb_CastToDouble_g5aahf3td0 = *rtu_engineCmd;
  }

  /* RateLimiter: '<S10>/Rate Limiter3' incorporates:
   *  DiscreteIntegrator: '<S33>/Discrete-Time Integrator'
   */
  rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.DiscreteTimeIntegrator_DSTATE
    - FW_CA_DW.PrevY_hbjyry413j;
  if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj > 0.3) {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.PrevY_hbjyry413j + 0.3;
  } else if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj < -0.3) {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.PrevY_hbjyry413j - 0.3;
  } else {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj =
      FW_CA_DW.DiscreteTimeIntegrator_DSTATE;
  }

  FW_CA_DW.PrevY_hbjyry413j = rtb_DiscreteTimeIntegrator_k5rcxc0xuj;

  /* End of RateLimiter: '<S10>/Rate Limiter3' */

  /* Logic: '<S7>/OR' incorporates:
   *  Constant: '<S18>/Constant'
   *  RelationalOperator: '<S18>/Compare'
   */
  rtb_OR1 = (((*rtu_vom_status) == READY) || rtb_OR1);

  /* Switch: '<S2>/Switch' incorporates:
   *  Constant: '<S2>/Constant4'
   *  Switch: '<S2>/Switch1'
   */
  if (rtb_OR1) {
    rtb_Switch = rtb_DiscreteTimeIntegrator_k5rcxc0xuj;
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = 1.0;
  } else {
    rtb_Switch = *rtu_aileron;
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = rtb_CastToDouble;
  }

  /* End of Switch: '<S2>/Switch' */

  /* Product: '<S2>/Product' incorporates:
   *  Constant: '<S2>/Constant'
   */
  rtb_Product[0] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * -30.0;

  /* Product: '<S2>/Product1' incorporates:
   *  Constant: '<S2>/Constant1'
   */
  rtb_Product1_lwaim5ixbq[0] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * 30.0;

  /* Product: '<S2>/Product' incorporates:
   *  Constant: '<S2>/Constant'
   */
  rtb_Product[1] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * -30.0;

  /* Product: '<S2>/Product1' incorporates:
   *  Constant: '<S2>/Constant1'
   */
  rtb_Product1_lwaim5ixbq[1] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * 30.0;

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  Constant: '<S2>/Constant2'
   *  Constant: '<S2>/Constant3'
   */
  FW_CA_i10nsmykqz(rtb_Switch, FW_CA_ConstP.Constant2_Value_kmf41b2fgv,
                   FW_CA_ConstP.Constant3_Value_n5qw4ehq0c, rtb_Product,
                   rtb_Product1_lwaim5ixbq, rtb_u_out_i1bd14vhxu,
                   &rtb_sat_flag_eyf34dhd5h);

  /* RateLimiter: '<S10>/Rate Limiter4' incorporates:
   *  DiscreteIntegrator: '<S34>/Discrete-Time Integrator'
   */
  rtb_DiscreteTimeIntegrator_k5rcxc0xuj =
    FW_CA_DW.DiscreteTimeIntegrator_DSTATE_cnt2bxweu5 -
    FW_CA_DW.PrevY_djckibarev;
  if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj > 0.3) {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.PrevY_djckibarev + 0.3;
  } else if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj < -0.3) {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.PrevY_djckibarev - 0.3;
  } else {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj =
      FW_CA_DW.DiscreteTimeIntegrator_DSTATE_cnt2bxweu5;
  }

  FW_CA_DW.PrevY_djckibarev = rtb_DiscreteTimeIntegrator_k5rcxc0xuj;

  /* End of RateLimiter: '<S10>/Rate Limiter4' */

  /* Switch: '<S5>/Switch' incorporates:
   *  Constant: '<S5>/Constant1'
   *  Switch: '<S5>/Switch1'
   */
  if (rtb_OR1) {
    rtb_Switch = rtb_DiscreteTimeIntegrator_k5rcxc0xuj;
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = 1.0;
  } else {
    rtb_Switch = *rtu_rudder;
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = rtb_CastToDouble;
  }

  /* End of Switch: '<S5>/Switch' */

  /* Product: '<S5>/Product' incorporates:
   *  Constant: '<S5>/Constant4'
   */
  rtb_Product[0] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * -25.0;

  /* Product: '<S5>/Product1' incorporates:
   *  Constant: '<S5>/Constant5'
   */
  rtb_Product1_lwaim5ixbq[0] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * 25.0;

  /* Product: '<S5>/Product' incorporates:
   *  Constant: '<S5>/Constant4'
   */
  rtb_Product[1] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * -25.0;

  /* Product: '<S5>/Product1' incorporates:
   *  Constant: '<S5>/Constant5'
   */
  rtb_Product1_lwaim5ixbq[1] = rtb_DiscreteTimeIntegrator_k5rcxc0xuj * 25.0;

  /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
   *  Constant: '<S5>/Constant2'
   *  Constant: '<S5>/Constant3'
   */
  FW_CA_i10nsmykqz(rtb_Switch, FW_CA_ConstP.pooled3, FW_CA_ConstP.pooled4,
                   rtb_Product, rtb_Product1_lwaim5ixbq, rtb_u_out,
                   &rtb_sat_flag);

  /* RateLimiter: '<S10>/Rate Limiter1' incorporates:
   *  DiscreteIntegrator: '<S32>/Discrete-Time Integrator'
   */
  rtb_DiscreteTimeIntegrator_k5rcxc0xuj =
    FW_CA_DW.DiscreteTimeIntegrator_DSTATE_hiklcoov13 -
    FW_CA_DW.PrevY_cf2xsvidl4;
  if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj > 0.3) {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.PrevY_cf2xsvidl4 + 0.3;
  } else if (rtb_DiscreteTimeIntegrator_k5rcxc0xuj < -0.3) {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = FW_CA_DW.PrevY_cf2xsvidl4 - 0.3;
  } else {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj =
      FW_CA_DW.DiscreteTimeIntegrator_DSTATE_hiklcoov13;
  }

  FW_CA_DW.PrevY_cf2xsvidl4 = rtb_DiscreteTimeIntegrator_k5rcxc0xuj;

  /* End of RateLimiter: '<S10>/Rate Limiter1' */

  /* Switch: '<S3>/Switch1' incorporates:
   *  Constant: '<S3>/Constant4'
   *  Switch: '<S3>/Switch'
   */
  if (rtb_OR1) {
    rtb_CastToDouble = 1.0;
  } else {
    rtb_DiscreteTimeIntegrator_k5rcxc0xuj = *rtu_elevator;
  }

  /* End of Switch: '<S3>/Switch1' */

  /* Product: '<S3>/Product' incorporates:
   *  Constant: '<S3>/Constant'
   */
  rtb_Product[0] = rtb_CastToDouble * -30.0;

  /* Product: '<S3>/Product1' incorporates:
   *  Constant: '<S3>/Constant1'
   */
  rtb_Product1_lwaim5ixbq[0] = rtb_CastToDouble * 30.0;

  /* Product: '<S3>/Product' incorporates:
   *  Constant: '<S3>/Constant'
   */
  rtb_Product[1] = rtb_CastToDouble * -30.0;

  /* Product: '<S3>/Product1' incorporates:
   *  Constant: '<S3>/Constant1'
   */
  rtb_Product1_lwaim5ixbq[1] = rtb_CastToDouble * 30.0;

  /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
   *  Constant: '<S3>/Constant2'
   *  Constant: '<S3>/Constant3'
   *  Switch: '<S3>/Switch'
   */
  FW_CA_i10nsmykqz(rtb_DiscreteTimeIntegrator_k5rcxc0xuj, FW_CA_ConstP.pooled3,
                   FW_CA_ConstP.pooled4, rtb_Product, rtb_Product1_lwaim5ixbq,
                   rtb_u_out_cuqvwmtzjx, &rtb_sat_flag_owj3wsswku);

  /* Logic: '<S8>/OR1' incorporates:
   *  Constant: '<S21>/Constant'
   *  Constant: '<S22>/Constant'
   *  Constant: '<S23>/Constant'
   *  RelationalOperator: '<S21>/Compare'
   *  RelationalOperator: '<S22>/Compare'
   *  RelationalOperator: '<S23>/Compare'
   */
  rtb_OR1 = ((((rtb_OR2 || ((*rtu_vom_status) == MR_RTH)) || rtb_OR) ||
              ((*rtu_vom_status) == HOVER)) || ((*rtu_vom_status) == READY));

  /* Product: '<S11>/Product' incorporates:
   *  Constant: '<S11>/FLAPS'
   *  Constant: '<S11>/L_Aileron'
   *  Constant: '<S11>/R_Aileron'
   *  DataTypeConversion: '<S8>/Data Type Conversion3'
   */
  rtb_Product_dq4dbzhv5s[0] = 0.0;
  rtb_CastToDouble = rtb_u_out_i1bd14vhxu[0] * ((real_T)(rtb_OR1 ? 1.0 : 0.0));
  rtb_Product_dq4dbzhv5s[1] = rtb_CastToDouble;
  rtb_Product_dq4dbzhv5s[2] = rtb_CastToDouble;
  rtb_Product_dq4dbzhv5s[3] = 0.0;
  rtb_Product_dq4dbzhv5s[4] = 0.0;
  rtb_CastToDouble = rtb_u_out_i1bd14vhxu[1] * ((real_T)(rtb_OR1 ? 1.0 : 0.0));
  rtb_Product_dq4dbzhv5s[5] = rtb_CastToDouble;
  rtb_Product_dq4dbzhv5s[6] = rtb_CastToDouble;
  rtb_Product_dq4dbzhv5s[7] = 0.0;
  rtb_Product_dq4dbzhv5s[8] = rtb_u_out[0] * ((real_T)(rtb_OR1 ? 1.0 : 0.0));
  rtb_Product_dq4dbzhv5s[10] = rtb_u_out_cuqvwmtzjx[0] * ((real_T)(rtb_OR1 ? 1.0
    : 0.0));
  rtb_Product_dq4dbzhv5s[9] = rtb_u_out[1] * ((real_T)(rtb_OR1 ? 1.0 : 0.0));
  rtb_Product_dq4dbzhv5s[11] = rtb_u_out_cuqvwmtzjx[1] * ((real_T)(rtb_OR1 ? 1.0
    : 0.0));

  /* SignalConversion generated from: '<Root>/Bus Assignment2' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  (void)memset(&rtb_BusAssignment2, 0, sizeof(std_ctrl_t));

  /* Outputs for Iterator SubSystem: '<Root>/Filter_forEach' incorporates:
   *  ForEach: '<S6>/For Each'
   */
  /* ForEachSliceAssignment generated from: '<S6>/Out1' incorporates:
   *  ForEachSliceSelector generated from: '<S6>/acs_servo_deg_cmd'
   *  Product: '<S11>/Product'
   */
  (void)memcpy(&rtb_BusAssignment2.acs_servo_deg_cmd[0],
               &rtb_Product_dq4dbzhv5s[0], 12U * (sizeof(real_T)));

  /* End of Outputs for SubSystem: '<Root>/Filter_forEach' */

  /* Gain: '<S11>/Gain' incorporates:
   *  Bias: '<S11>/Bias'
   *  DataTypeConversion: '<S8>/Data Type Conversion1'
   *  Logic: '<S8>/OR2'
   *  Product: '<S11>/Product1'
   *  Switch: '<S4>/Switch'
   */
  rtb_CastToDouble_g5aahf3td0 = 1000.0 * ((((real_T)((rtb_OR2 || rtb_OR) ?
    ((int32_T)1) : ((int32_T)0))) * rtb_CastToDouble_g5aahf3td0) + 1.0);

  /* Saturate: '<S11>/Saturation' */
  if (rtb_CastToDouble_g5aahf3td0 > 2000.0) {
    /* BusAssignment: '<Root>/Bus Assignment2' */
    rtb_BusAssignment2.pusher_pwm_cmd = 2000.0;
  } else if (rtb_CastToDouble_g5aahf3td0 < 1000.0) {
    /* BusAssignment: '<Root>/Bus Assignment2' */
    rtb_BusAssignment2.pusher_pwm_cmd = 1000.0;
  } else {
    /* BusAssignment: '<Root>/Bus Assignment2' */
    rtb_BusAssignment2.pusher_pwm_cmd = rtb_CastToDouble_g5aahf3td0;
  }

  /* End of Saturate: '<S11>/Saturation' */
  (void)memset(rty_controllerCA, 0, sizeof(busControllerCA));

  /* BusAssignment: '<Root>/Bus Assignment1' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   *  DataTypeConversion: '<S2>/Cast To Double'
   *  DataTypeConversion: '<S3>/Cast To Double'
   *  DataTypeConversion: '<S5>/Cast To Double'
   */
  rty_controllerCA->stdCtrl_IF = rtb_BusAssignment2;
  rty_controllerCA->c_erp1 = rtb_sat_flag_owj3wsswku ? 1.0 : 0.0;
  rty_controllerCA->c_erp2 = rtb_sat_flag_eyf34dhd5h ? 1.0 : 0.0;
  rty_controllerCA->c_erp3 = rtb_sat_flag ? 1.0 : 0.0;

  /* Update for DiscreteIntegrator: '<S33>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S10>/Constant1'
   *  Gain: '<S33>/Gain'
   *  Product: '<S10>/Product'
   *  Sum: '<S33>/Sum'
   */
  FW_CA_DW.DiscreteTimeIntegrator_DSTATE += 0.01 * (10.0 * ((30.0 *
    (*rtu_pilot_roll_ch)) - FW_CA_DW.DiscreteTimeIntegrator_DSTATE));

  /* Update for DiscreteIntegrator: '<S34>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S10>/Constant5'
   *  Gain: '<S34>/Gain'
   *  Product: '<S10>/Product2'
   *  Sum: '<S34>/Sum'
   */
  FW_CA_DW.DiscreteTimeIntegrator_DSTATE_cnt2bxweu5 += 0.01 * (10.0 * ((25.0 * (*
    rtu_pilot_yaw_ch)) - FW_CA_DW.DiscreteTimeIntegrator_DSTATE_cnt2bxweu5));

  /* Update for DiscreteIntegrator: '<S32>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S10>/Constant4'
   *  Gain: '<S32>/Gain'
   *  Product: '<S10>/Product1'
   *  Sum: '<S32>/Sum'
   */
  FW_CA_DW.DiscreteTimeIntegrator_DSTATE_hiklcoov13 += 0.01 * (10.0 * ((-30.0 *
    (*rtu_pilot_pitch_ch)) - FW_CA_DW.DiscreteTimeIntegrator_DSTATE_hiklcoov13));
}

/* Model initialize function */
void FW_CA_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&FW_CA_DW, 0,
                sizeof(FW_CA_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
