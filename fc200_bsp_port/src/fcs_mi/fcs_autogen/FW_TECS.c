/*
 * File: FW_TECS.c
 *
 * Code generated for Simulink model 'FW_TECS'.
 *
 * Model version                  : 2.61
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:04:29 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FW_TECS.h"
#include "rtwtypes.h"
#include "FW_TECS_types.h"
#include "FW_TECS_private.h"
#include <math.h>
#include <string.h>

/* Block states (default storage) */
FW_TECS_TDW FW_TECS_DW;

/* System initialize for referenced model: 'FW_TECS' */
void FW_TECS_Init(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  FW_TECS_DW.DiscreteTimeIntegrator_PrevResetState = 0;
  FW_TECS_DW.DiscreteTimeIntegrator_IC_LOADING = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator1' */
  FW_TECS_DW.DiscreteTimeIntegrator1_PrevResetState = 0;
  FW_TECS_DW.DiscreteTimeIntegrator1_IC_LOADING = 1U;

  /* InitializeConditions for RateLimiter: '<S3>/Rate Limiter3' */
  FW_TECS_DW.PrevY = 0.0;
}

/* Output and update for referenced model: 'FW_TECS' */
void FW_TECS(const real_T *rtu_FW_TECS_switcher_h_dot_cmd, const real_T
             *rtu_FW_TECS_switcher_h_cmd, const real_T
             *rtu_FW_TECS_switcher_V_cmd, const boolean_T
             *rtu_FW_TECS_switcher_TECS_reset, const boolean_T
             *rtu_FW_TECS_switcher_h_hold, const std_sensor_t *rtu_Sensor, const
             real_T *rtu_engineCmd, const busTECS_Data_Processing
             *rtu_sTECS_Data_Processing, busControllerTECS *rty_controllerTECS)
{
  real_T rtb_Add;
  real_T rtb_Add1;
  real_T rtb_CastToDouble_hqijlyrn5q;
  real_T rtb_DiscreteTimeIntegrator;
  real_T rtb_Switch2_k5ccdp3up5;
  real_T rtb_cval2;
  real_T rtb_theta_cmd;

  /* Saturate: '<S7>/Saturation' */
  if ((*rtu_engineCmd) > 1.0) {
    rtb_DiscreteTimeIntegrator = 1.0;
  } else if ((*rtu_engineCmd) < 0.1) {
    rtb_DiscreteTimeIntegrator = 0.1;
  } else {
    rtb_DiscreteTimeIntegrator = *rtu_engineCmd;
  }

  /* End of Saturate: '<S7>/Saturation' */

  /* Product: '<S7>/Divide1' incorporates:
   *  Constant: '<S7>/Constant'
   *  Constant: '<S7>/linearized'
   *  Math: '<S7>/Square'
   */
  rtb_DiscreteTimeIntegrator = ((rtb_DiscreteTimeIntegrator *
    rtb_DiscreteTimeIntegrator) * 229.0411) / 661.4585425;

  /* DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  if (FW_TECS_DW.DiscreteTimeIntegrator_IC_LOADING != ((uint8_T)0)) {
    FW_TECS_DW.DiscreteTimeIntegrator_DSTATE = rtb_DiscreteTimeIntegrator;
  }

  if ((*rtu_FW_TECS_switcher_TECS_reset) ||
      (FW_TECS_DW.DiscreteTimeIntegrator_PrevResetState != ((int8_T)0))) {
    FW_TECS_DW.DiscreteTimeIntegrator_DSTATE = rtb_DiscreteTimeIntegrator;
  }

  /* Product: '<S2>/Divide' incorporates:
   *  Constant: '<S2>/Constant'
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   *  Gain: '<S2>/Gain2'
   *  Product: '<S2>/Product1'
   *  Sum: '<S2>/Add2'
   *  Sum: '<S2>/Sum2'
   */
  rtb_cval2 = (661.4585425 * (FW_TECS_DW.DiscreteTimeIntegrator_DSTATE - (0.1 *
    (rtu_sTECS_Data_Processing->gammaRad + rtu_sTECS_Data_Processing->VdotDivG))))
    / FW_TECS_ConstB.Switch3;

  /* Switch: '<S6>/Switch2' incorporates:
   *  Constant: '<S2>/Constant1'
   *  Constant: '<S2>/Constant2'
   *  RelationalOperator: '<S6>/LowerRelop1'
   *  RelationalOperator: '<S6>/UpperRelop'
   *  Switch: '<S6>/Switch'
   */
  if (rtb_cval2 > 1.0) {
    rtb_Switch2_k5ccdp3up5 = 1.0;
  } else if (rtb_cval2 < 0.01) {
    /* Switch: '<S6>/Switch' incorporates:
     *  Constant: '<S2>/Constant1'
     */
    rtb_Switch2_k5ccdp3up5 = 0.01;
  } else {
    rtb_Switch2_k5ccdp3up5 = rtb_cval2;
  }

  /* End of Switch: '<S6>/Switch2' */

  /* Sqrt: '<S2>/Sqrt' */
  rty_controllerTECS->engineCmd = sqrt(rtb_Switch2_k5ccdp3up5);

  /* DiscreteIntegrator: '<S2>/Discrete-Time Integrator1' */
  if (FW_TECS_DW.DiscreteTimeIntegrator1_IC_LOADING != ((uint8_T)0)) {
    FW_TECS_DW.DiscreteTimeIntegrator1_DSTATE = rtu_Sensor->eul_ang[1];
  }

  if ((*rtu_FW_TECS_switcher_TECS_reset) ||
      (FW_TECS_DW.DiscreteTimeIntegrator1_PrevResetState != ((int8_T)0))) {
    FW_TECS_DW.DiscreteTimeIntegrator1_DSTATE = rtu_Sensor->eul_ang[1];
  }

  /* Sum: '<S2>/Sum3' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator1'
   *  Gain: '<S2>/Gain3'
   *  Sum: '<S2>/Add3'
   */
  rtb_DiscreteTimeIntegrator = FW_TECS_DW.DiscreteTimeIntegrator1_DSTATE - (0.1 *
    (rtu_sTECS_Data_Processing->gammaRad - rtu_sTECS_Data_Processing->VdotDivG));

  /* Saturate: '<S2>/Saturation1' */
  if (rtb_DiscreteTimeIntegrator > 0.26179938779914941) {
    rtb_theta_cmd = 0.26179938779914941;
  } else if (rtb_DiscreteTimeIntegrator < -0.26179938779914941) {
    rtb_theta_cmd = -0.26179938779914941;
  } else {
    rtb_theta_cmd = rtb_DiscreteTimeIntegrator;
  }

  /* End of Saturate: '<S2>/Saturation1' */

  /* Gain: '<S3>/KPV' incorporates:
   *  Sum: '<S3>/Sum5'
   */
  rtb_CastToDouble_hqijlyrn5q = 0.25 * ((*rtu_FW_TECS_switcher_V_cmd) -
    rtu_Sensor->aspd_cas);

  /* Saturate: '<S3>/3mps2_acc_limit' */
  if (rtb_CastToDouble_hqijlyrn5q > 3.0) {
    rtb_CastToDouble_hqijlyrn5q = 3.0;
  } else if (rtb_CastToDouble_hqijlyrn5q < -3.0) {
    rtb_CastToDouble_hqijlyrn5q = -3.0;
  } else {
    /* no actions */
  }

  /* Product: '<S3>/Divide' incorporates:
   *  Constant: '<S3>/g'
   *  Saturate: '<S3>/3mps2_acc_limit'
   */
  rtb_CastToDouble_hqijlyrn5q /= 9.80665;

  /* Switch: '<S3>/Switch' */
  if (*rtu_FW_TECS_switcher_h_hold) {
    /* Gain: '<S3>/Gain' incorporates:
     *  Sum: '<S3>/Sum'
     */
    rtb_Add1 = 0.2 * ((*rtu_FW_TECS_switcher_h_cmd) - rtu_Sensor->pos_lla[2]);

    /* Switch: '<S11>/Switch2' incorporates:
     *  Constant: '<S3>/Constant3'
     *  RelationalOperator: '<S11>/LowerRelop1'
     *  RelationalOperator: '<S11>/UpperRelop'
     *  Switch: '<S11>/Switch'
     */
    if (rtb_Add1 > 3.0) {
      rtb_Add1 = 3.0;
    } else if (rtb_Add1 < FW_TECS_ConstB.Gain1) {
      /* Switch: '<S11>/Switch' */
      rtb_Add1 = FW_TECS_ConstB.Gain1;
    } else {
      /* no actions */
    }

    /* End of Switch: '<S11>/Switch2' */
  } else {
    rtb_Add1 = *rtu_FW_TECS_switcher_h_dot_cmd;
  }

  /* End of Switch: '<S3>/Switch' */

  /* Saturate: '<S3>/Saturation1' */
  if (rtb_Add1 > 3.0) {
    rtb_Add1 = 3.0;
  } else if (rtb_Add1 < -3.0) {
    rtb_Add1 = -3.0;
  } else {
    /* no actions */
  }

  /* End of Saturate: '<S3>/Saturation1' */

  /* RateLimiter: '<S3>/Rate Limiter3' */
  rtb_Add = rtb_Add1 - FW_TECS_DW.PrevY;
  if (rtb_Add > 0.05) {
    rtb_Add1 = FW_TECS_DW.PrevY + 0.05;
  } else if (rtb_Add < -0.05) {
    rtb_Add1 = FW_TECS_DW.PrevY - 0.05;
  } else {
    /* no actions */
  }

  FW_TECS_DW.PrevY = rtb_Add1;

  /* End of RateLimiter: '<S3>/Rate Limiter3' */

  /* Trigonometry: '<S3>/Atan2' */
  rtb_Add1 = atan2(rtb_Add1, rtu_Sensor->aspd_cas);

  /* BusCreator: '<Root>/Bus Creator' incorporates:
   *  Sum: '<S3>/Add'
   *  Sum: '<S3>/Add1'
   */
  rty_controllerTECS->theta_cmd = rtb_theta_cmd;
  rty_controllerTECS->vDotDivG_cmd = rtb_CastToDouble_hqijlyrn5q;
  rty_controllerTECS->gamma_cmd = rtb_Add1;
  rty_controllerTECS->Edot_cmd = rtb_CastToDouble_hqijlyrn5q + rtb_Add1;
  rty_controllerTECS->Ldot_cmd = rtb_Add1 - rtb_CastToDouble_hqijlyrn5q;
  rty_controllerTECS->vel_cmd = *rtu_FW_TECS_switcher_V_cmd;
  rty_controllerTECS->hCmd = *rtu_FW_TECS_switcher_h_cmd;
  rty_controllerTECS->vel = rtu_Sensor->aspd_cas;
  rty_controllerTECS->h = rtu_Sensor->pos_lla[2];
  rty_controllerTECS->gamma = rtu_sTECS_Data_Processing->gammaRad;
  rty_controllerTECS->vDotDivG = rtu_sTECS_Data_Processing->VdotDivG;
  rty_controllerTECS->Edot = rtu_sTECS_Data_Processing->Edot;
  rty_controllerTECS->Ldot = rtu_sTECS_Data_Processing->Ldot;

  /* Sum: '<S2>/Sum' */
  rtb_Add1 -= rtu_sTECS_Data_Processing->gammaRad;

  /* Sum: '<S2>/Sum1' */
  rtb_CastToDouble_hqijlyrn5q -= rtu_sTECS_Data_Processing->VdotDivG;

  /* Sum: '<S2>/Add' */
  rtb_Add = rtb_Add1 + rtb_CastToDouble_hqijlyrn5q;

  /* DataTypeConversion: '<S5>/Cast To Double' incorporates:
   *  Constant: '<S10>/Constant'
   *  Product: '<S5>/Product'
   *  RelationalOperator: '<S10>/Compare'
   *  Sum: '<S2>/Sum4'
   */
  rtb_cval2 = (real_T)((((rtb_cval2 - rtb_Switch2_k5ccdp3up5) * rtb_Add) <= 0.0)
                       ? ((int32_T)1) : ((int32_T)0));

  /* Switch: '<S2>/speedPrioSw' incorporates:
   *  Constant: '<S2>/speedPriority'
   */
  if (rtb_cval2 <= 0.0) {
    rtb_Add1 = 0.0;
  }

  /* Sum: '<S2>/Add1' incorporates:
   *  Switch: '<S2>/speedPrioSw'
   */
  rtb_Add1 -= rtb_CastToDouble_hqijlyrn5q;

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S2>/Gain4'
   *  Product: '<S2>/Product'
   */
  FW_TECS_DW.DiscreteTimeIntegrator_IC_LOADING = 0U;
  FW_TECS_DW.DiscreteTimeIntegrator_DSTATE += 0.01 * (rtb_cval2 * (0.5 * rtb_Add));
  FW_TECS_DW.DiscreteTimeIntegrator_PrevResetState = (int8_T)
    ((*rtu_FW_TECS_switcher_TECS_reset) ? 1 : 0);

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator1' incorporates:
   *  Constant: '<S9>/Constant'
   *  DataTypeConversion: '<S4>/Cast To Double'
   *  Gain: '<S2>/Gain5'
   *  Product: '<S2>/Product2'
   *  Product: '<S4>/Product'
   *  RelationalOperator: '<S9>/Compare'
   *  Sum: '<S2>/Sum5'
   */
  FW_TECS_DW.DiscreteTimeIntegrator1_IC_LOADING = 0U;
  FW_TECS_DW.DiscreteTimeIntegrator1_DSTATE += 0.01 * (((real_T)
    ((((rtb_DiscreteTimeIntegrator - rtb_theta_cmd) * rtb_Add1) <= 0.0) ?
     ((int32_T)1) : ((int32_T)0))) * (0.25 * rtb_Add1));
  FW_TECS_DW.DiscreteTimeIntegrator1_PrevResetState = (int8_T)
    ((*rtu_FW_TECS_switcher_TECS_reset) ? 1 : 0);
}

/* Model initialize function */
void FW_TECS_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&FW_TECS_DW, 0,
                sizeof(FW_TECS_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
