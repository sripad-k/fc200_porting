/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: altCtrl.c
 *
 * Code generated for Simulink model 'altCtrl'.
 *
 * Model version                  : 2.16
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:56 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "altCtrl.h"
#include "rtwtypes.h"
#include "altCtrl_types.h"
#include "altCtrl_private.h"
#include <string.h>

/* Block states (default storage) */
altCtrl_TDW altCtrl_DW;

/* System initialize for referenced model: 'altCtrl' */
void altCtrl_Init(void)
{
  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter3' */
  altCtrl_DW.PrevY = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' */
  altCtrl_DW.DiscreteTimeIntegrator1_DSTATE = altCtrl_ConstB.Constant;
  if (altCtrl_DW.DiscreteTimeIntegrator1_DSTATE < -1.0) {
    altCtrl_DW.DiscreteTimeIntegrator1_DSTATE = -1.0;
  }

  altCtrl_DW.DiscreteTimeIntegrator1_PrevResetState = 0;

  /* End of InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' */
}

/* Output and update for referenced model: 'altCtrl' */
void altCtrl(const real_T *rtu_ctrlIF_vel_hRateCmd, const real_T
             *rtu_ctrlIF_vel_hCmd, const boolean_T *rtu_ctrlIF_vel_hHold, const
             boolean_T *rtu_ctrlIF_vel_init_reset_hdot, const std_sensor_t
             *rtu_Sensor, const real_T *rtu_busControllerCA_c_erp2, const real_T
             rtu_busControllerCA_nu_des[4], const real_T
             rtu_busControllerCA_nu_allocated[4], busControllerAltCtrl
             *rty_controllerAltCtrl)
{
  real_T rtb_Sum_ijs2kwqy4e;
  real_T rtb_velD_to_hdot;
  int32_T tmp;

  /* Switch: '<Root>/Switch' */
  if (*rtu_ctrlIF_vel_hHold) {
    /* Gain: '<S2>/Gain' incorporates:
     *  Sum: '<S2>/Sum'
     */
    rtb_Sum_ijs2kwqy4e = 0.5 * ((*rtu_ctrlIF_vel_hCmd) - rtu_Sensor->pos_lla[2]);

    /* Switch: '<S8>/Switch2' incorporates:
     *  Constant: '<S2>/Constant3'
     *  RelationalOperator: '<S8>/LowerRelop1'
     *  RelationalOperator: '<S8>/UpperRelop'
     *  Switch: '<S8>/Switch'
     */
    if (rtb_Sum_ijs2kwqy4e > 2.0) {
      rtb_Sum_ijs2kwqy4e = 2.0;
    } else if (rtb_Sum_ijs2kwqy4e < altCtrl_ConstB.Gain1) {
      /* Switch: '<S8>/Switch' */
      rtb_Sum_ijs2kwqy4e = altCtrl_ConstB.Gain1;
    } else {
      /* no actions */
    }

    /* End of Switch: '<S8>/Switch2' */
  } else {
    rtb_Sum_ijs2kwqy4e = *rtu_ctrlIF_vel_hRateCmd;
  }

  /* End of Switch: '<Root>/Switch' */

  /* Saturate: '<Root>/Saturation1' */
  if (rtb_Sum_ijs2kwqy4e > 2.0) {
    rtb_Sum_ijs2kwqy4e = 2.0;
  } else if (rtb_Sum_ijs2kwqy4e < -2.0) {
    rtb_Sum_ijs2kwqy4e = -2.0;
  } else {
    /* no actions */
  }

  /* End of Saturate: '<Root>/Saturation1' */

  /* RateLimiter: '<Root>/Rate Limiter3' */
  rtb_velD_to_hdot = rtb_Sum_ijs2kwqy4e - altCtrl_DW.PrevY;
  if (rtb_velD_to_hdot > 0.05) {
    rtb_Sum_ijs2kwqy4e = altCtrl_DW.PrevY + 0.05;
  } else if (rtb_velD_to_hdot < -0.05) {
    rtb_Sum_ijs2kwqy4e = altCtrl_DW.PrevY - 0.05;
  } else {
    /* no actions */
  }

  altCtrl_DW.PrevY = rtb_Sum_ijs2kwqy4e;

  /* End of RateLimiter: '<Root>/Rate Limiter3' */

  /* DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' */
  if ((*rtu_ctrlIF_vel_init_reset_hdot) ||
      (altCtrl_DW.DiscreteTimeIntegrator1_PrevResetState != 0)) {
    altCtrl_DW.DiscreteTimeIntegrator1_DSTATE = altCtrl_ConstB.Constant;
    if (altCtrl_DW.DiscreteTimeIntegrator1_DSTATE < -1.0) {
      altCtrl_DW.DiscreteTimeIntegrator1_DSTATE = -1.0;
    }
  }

  /* SignalConversion generated from: '<Root>/controllerAltCtrl' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment1'
   */
  rty_controllerAltCtrl->hDotCmd = rtb_Sum_ijs2kwqy4e;

  /* Sum: '<S1>/Add' incorporates:
   *  Constant: '<S1>/Constant3'
   *  Constant: '<S1>/b'
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
   *  Gain: '<Root>/velD_to_hdot'
   *  Product: '<S1>/Product2'
   *  Product: '<S1>/Product6'
   *  Sum: '<S1>/Sum2'
   */
  rtb_velD_to_hdot = (3.0729 * ((0.4 * rtb_Sum_ijs2kwqy4e) -
    (-rtu_Sensor->vel_ned[2]))) + altCtrl_DW.DiscreteTimeIntegrator1_DSTATE;

  /* Saturate: '<Root>/Saturation' */
  if (rtb_velD_to_hdot > 20.0) {
    rtb_velD_to_hdot = 20.0;
  } else if (rtb_velD_to_hdot < 0.0) {
    rtb_velD_to_hdot = 0.0;
  } else {
    /* no actions */
  }

  /* SignalConversion generated from: '<Root>/controllerAltCtrl' incorporates:
   *  Gain: '<Root>/Multiply'
   *  Saturate: '<Root>/Saturation'
   */
  rty_controllerAltCtrl->forceDes = 65.45 * rtb_velD_to_hdot;
  rty_controllerAltCtrl->hCmd = *rtu_ctrlIF_vel_hCmd;
  rty_controllerAltCtrl->hHold = *rtu_ctrlIF_vel_hHold;

  /* Sum: '<S1>/Sum' incorporates:
   *  Gain: '<Root>/velD_to_hdot'
   */
  rtb_Sum_ijs2kwqy4e -= -rtu_Sensor->vel_ned[2];

  /* Update for DiscreteIntegrator: '<S5>/omega' incorporates:
   *  Constant: '<S1>/c'
   *  Constant: '<S5>/Constant3'
   *  Gain: '<Root>/velD_to_hdot'
   *  Product: '<S1>/Product5'
   *  Product: '<S5>/Product'
   *  Sum: '<S1>/Sum1'
   *  Sum: '<S5>/Sum'
   */
  altCtrl_DW.omega_DSTATE += 0.01 * (40.0 * ((0.0 - (-rtu_Sensor->vel_ned[2])) -
    altCtrl_DW.omega_DSTATE));

  /* Product: '<S4>/Product' incorporates:
   *  Sum: '<Root>/Sum'
   */
  rtb_velD_to_hdot = rtb_Sum_ijs2kwqy4e * (rtu_busControllerCA_nu_des[0] -
    rtu_busControllerCA_nu_allocated[0]);

  /* Signum: '<S4>/Sign' */
  if (rtb_velD_to_hdot < 0.0) {
    tmp = -1;
  } else {
    tmp = (rtb_velD_to_hdot > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' incorporates:
   *  Constant: '<S1>/Constant2'
   *  Constant: '<S6>/Constant'
   *  Constant: '<S7>/Constant'
   *  Logic: '<S4>/AND'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product4'
   *  RelationalOperator: '<S6>/Compare'
   *  RelationalOperator: '<S7>/Compare'
   *  Signum: '<S4>/Sign'
   */
  altCtrl_DW.DiscreteTimeIntegrator1_DSTATE += 0.01 * (3.1529 *
    (rtb_Sum_ijs2kwqy4e * ((real_T)((((*rtu_busControllerCA_c_erp2) >= 0.95) ||
    (tmp <= 0)) ? ((int32_T)1) : ((int32_T)0)))));
  if (altCtrl_DW.DiscreteTimeIntegrator1_DSTATE < -1.0) {
    altCtrl_DW.DiscreteTimeIntegrator1_DSTATE = -1.0;
  }

  altCtrl_DW.DiscreteTimeIntegrator1_PrevResetState = (int8_T)
    ((*rtu_ctrlIF_vel_init_reset_hdot) ? 1 : 0);

  /* End of Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' */
}

/* Model initialize function */
void altCtrl_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&altCtrl_DW, 0,
                sizeof(altCtrl_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
