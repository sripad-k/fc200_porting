/*
 * File: FW_attCtrl.c
 *
 * Code generated for Simulink model 'FW_attCtrl'.
 *
 * Model version                  : 2.68
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:15 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FW_attCtrl.h"
#include "rtwtypes.h"
#include "FW_attCtrl_types.h"
#include "FW_attCtrl_private.h"
#include <math.h>
#include <string.h>

/* Block states (default storage) */
FW_attCtrl_TDW FW_attCtrl_DW;

/* System initialize for referenced model: 'FW_attCtrl' */
void FW_attCtrl_Init(void)
{
  /* InitializeConditions for DiscreteIntegrator: '<S3>/Discrete-Time Integrator' */
  FW_attCtrl_DW.DiscreteTimeIntegrator_PrevResetState = 0;

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter2' */
  FW_attCtrl_DW.PrevY = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */
  FW_attCtrl_DW.DiscreteTimeIntegrator_PrevResetState_i2m1oat1uz = 0;

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter' */
  FW_attCtrl_DW.PrevY_ayxso3nroc = 0.0;
}

/* Output and update for referenced model: 'FW_attCtrl' */
void FW_attCtrl(const real_T *rtu_FW_Controller_att_pitch_cmd, const real_T
                *rtu_FW_Controller_att_roll_cmd, const boolean_T
                *rtu_FW_Controller_att_roll_reset, const boolean_T
                *rtu_FW_Controller_att_pitch_reset, const std_sensor_t
                *rtu_Sensor, const real_T *rtu_controllerCA_FW_c_erp1, const
                real_T *rtu_controllerCA_FW_c_erp2, const real_T
                *rtu_FW_IntCF_roll_CF, const real_T *rtu_FW_IntCF_pitch_CF,
                real_T *rty_ele_L, real_T *rty_ail_LO, real_T *rty_rud_L, real_T
                *rty_FW_IntData_roll_int, real_T *rty_FW_IntData_pitch_int,
                real_T *rty_FW_IntData_roll_sat, real_T
                *rty_FW_IntData_pitch_sat, real_T *rty_FwAttCmd_pitchCmd, real_T
                *rty_FwAttCmd_rollCmd, real_T *rty_FwAttCmd_raw_pitchCmd, real_T
                *rty_FwAttCmd_raw_rollCmd)
{
  real_T b[9];
  real_T tmp_2[9];
  real_T tmp_3[9];
  real_T tmp_4[9];
  real_T rtu_Sensor_0[3];
  real_T b_1;
  real_T b_2;
  real_T rtb_RateLimiter;
  real_T rtb_RateLimiter2;
  real_T rtb_Sum3_bewwz0zu24;
  real_T rtb_Sum_fphn0s52rh;
  real_T tmp;
  real_T tmp_0;
  real_T tmp_1;
  real_T u;
  int32_T b_0;
  int32_T i;
  int32_T i_0;
  int32_T rtb_Switch;
  int32_T rtb_Switch_mn4j10pnvz;

  /* DiscreteIntegrator: '<S3>/Discrete-Time Integrator' */
  if ((*rtu_FW_Controller_att_pitch_reset) ||
      (FW_attCtrl_DW.DiscreteTimeIntegrator_PrevResetState != ((int8_T)0))) {
    FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  }

  /* RateLimiter: '<Root>/Rate Limiter2' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   */
  rtb_Sum3_bewwz0zu24 = FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_is34kceaix -
    FW_attCtrl_DW.PrevY;
  if (rtb_Sum3_bewwz0zu24 > 0.0078539816339744835) {
    rtb_RateLimiter2 = FW_attCtrl_DW.PrevY + 0.0078539816339744835;
  } else if (rtb_Sum3_bewwz0zu24 < -0.0078539816339744835) {
    rtb_RateLimiter2 = FW_attCtrl_DW.PrevY - 0.0078539816339744835;
  } else {
    rtb_RateLimiter2 = FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_is34kceaix;
  }

  FW_attCtrl_DW.PrevY = rtb_RateLimiter2;

  /* End of RateLimiter: '<Root>/Rate Limiter2' */

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<S3>/K_itgt_theta1'
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum1'
   */
  rtb_Sum_fphn0s52rh = ((*rtu_FW_IntCF_pitch_CF) / 22.837) + (rtb_RateLimiter2 -
    rtu_Sensor->eul_ang[1]);

  /* Product: '<S6>/Product' incorporates:
   *  DiscreteIntegrator: '<S3>/Discrete-Time Integrator'
   */
  u = rtb_Sum_fphn0s52rh * FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE;

  /* Signum: '<S6>/Sign' */
  if (u < 0.0) {
    i_0 = -1;
  } else {
    i_0 = (u > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Switch: '<S6>/Switch' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S9>/Constant'
   *  Logic: '<S6>/AND'
   *  RelationalOperator: '<S10>/Compare'
   *  RelationalOperator: '<S9>/Compare'
   *  Signum: '<S6>/Sign'
   */
  rtb_Switch = (((*rtu_controllerCA_FW_c_erp1) >= 0.95) || (i_0 <= ((int32_T)
    ((int8_T)0)))) ? ((int32_T)1) : ((int32_T)0);

  /* DiscreteIntegrator: '<S4>/Discrete-Time Integrator' */
  if ((*rtu_FW_Controller_att_roll_reset) ||
      (FW_attCtrl_DW.DiscreteTimeIntegrator_PrevResetState_i2m1oat1uz !=
       ((int8_T)0))) {
    FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ej0a0oln2v = 0.0;
  }

  /* RateLimiter: '<Root>/Rate Limiter' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   */
  rtb_Sum3_bewwz0zu24 = FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ntgr3gtdbx -
    FW_attCtrl_DW.PrevY_ayxso3nroc;
  if (rtb_Sum3_bewwz0zu24 > 0.0078539816339744835) {
    rtb_RateLimiter = FW_attCtrl_DW.PrevY_ayxso3nroc + 0.0078539816339744835;
  } else if (rtb_Sum3_bewwz0zu24 < -0.0078539816339744835) {
    rtb_RateLimiter = FW_attCtrl_DW.PrevY_ayxso3nroc - 0.0078539816339744835;
  } else {
    rtb_RateLimiter = FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ntgr3gtdbx;
  }

  FW_attCtrl_DW.PrevY_ayxso3nroc = rtb_RateLimiter;

  /* End of RateLimiter: '<Root>/Rate Limiter' */

  /* Sum: '<S4>/Sum4' incorporates:
   *  Gain: '<S4>/Gain3'
   *  Sum: '<S4>/Sum'
   */
  rtb_Sum3_bewwz0zu24 = (0.028300471768864389 * (*rtu_FW_IntCF_roll_CF)) +
    (rtb_RateLimiter - rtu_Sensor->eul_ang[0]);

  /* Product: '<S13>/Product' incorporates:
   *  DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
   */
  u = rtb_Sum3_bewwz0zu24 *
    FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ej0a0oln2v;

  /* Signum: '<S13>/Sign' */
  if (u < 0.0) {
    i_0 = -1;
  } else {
    i_0 = (u > 0.0) ? ((int32_T)1) : ((int32_T)0);
  }

  /* Switch: '<S13>/Switch' incorporates:
   *  Constant: '<S15>/Constant'
   *  Constant: '<S16>/Constant'
   *  Logic: '<S13>/AND'
   *  RelationalOperator: '<S15>/Compare'
   *  RelationalOperator: '<S16>/Compare'
   *  Signum: '<S13>/Sign'
   */
  rtb_Switch_mn4j10pnvz = (((*rtu_controllerCA_FW_c_erp2) >= 0.95) || (i_0 <=
    ((int32_T)((int8_T)0)))) ? ((int32_T)1) : ((int32_T)0);

  /* SignalConversion generated from: '<Root>/FW_IntData' incorporates:
   *  DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
   */
  *rty_FW_IntData_roll_int =
    FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ej0a0oln2v;

  /* SignalConversion generated from: '<Root>/FW_IntData' incorporates:
   *  DiscreteIntegrator: '<S3>/Discrete-Time Integrator'
   */
  *rty_FW_IntData_pitch_int = FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE;

  /* SignalConversion generated from: '<Root>/FW_IntData' incorporates:
   *  Constant: '<S17>/Constant'
   *  Logic: '<S14>/Logical Operator1'
   *  RelationalOperator: '<S17>/Compare'
   */
  *rty_FW_IntData_roll_sat = (real_T)(((*rtu_FW_Controller_att_roll_reset) ||
    (rtb_Switch_mn4j10pnvz == ((int32_T)((int8_T)0)))) ? ((int32_T)1) :
    ((int32_T)0));

  /* SignalConversion generated from: '<Root>/FW_IntData' incorporates:
   *  Constant: '<S11>/Constant'
   *  Logic: '<S7>/Logical Operator1'
   *  RelationalOperator: '<S11>/Compare'
   */
  *rty_FW_IntData_pitch_sat = (real_T)(((*rtu_FW_Controller_att_pitch_reset) ||
    (rtb_Switch == ((int32_T)((int8_T)0)))) ? ((int32_T)1) : ((int32_T)0));

  /* SignalConversion generated from: '<Root>/FwAttCmd' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment1'
   */
  *rty_FwAttCmd_pitchCmd = rtb_RateLimiter2;

  /* SignalConversion generated from: '<Root>/FwAttCmd' incorporates:
   *  DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
   */
  *rty_FwAttCmd_rollCmd = FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ej0a0oln2v;

  /* SignalConversion generated from: '<Root>/FwAttCmd' */
  *rty_FwAttCmd_raw_pitchCmd = *rtu_FW_Controller_att_pitch_cmd;

  /* SignalConversion generated from: '<Root>/FwAttCmd' */
  *rty_FwAttCmd_raw_rollCmd = *rtu_FW_Controller_att_roll_cmd;

  /* MATLAB Function: '<S8>/MATLAB Function' */
  /*  ref: groves 2nd ed. page 38 eqn 2.24 */
  /*  R = zeros(3); */
  /* MATLAB Function 'euler2dcm/MATLAB Function': '<S12>:1' */
  /* '<S12>:1:5' roll = eul(1); */
  /* '<S12>:1:6' pitch = eul(2); */
  /* '<S12>:1:7' yaw = eul(3); */
  /* '<S12>:1:9' rM=[1    0           0; */
  /* '<S12>:1:10'     0    cos(roll)   sin(roll); */
  /* '<S12>:1:11'     0    -sin(roll)  cos(roll)]; */
  /* '<S12>:1:13' pM=[cos(pitch)      0       -sin(pitch); */
  /* '<S12>:1:14'     0               1       0; */
  /* '<S12>:1:15'     sin(pitch)      0       cos(pitch)]; */
  /* '<S12>:1:17' yM=[cos(yaw)        sin(yaw)        0; */
  /* '<S12>:1:18'     -sin(yaw)       cos(yaw)        0; */
  /* '<S12>:1:19'     0               0               1]; */
  /* '<S12>:1:21' R=rM*(pM*yM); */
  /*  R = R'; */
  u = sin(rtu_Sensor->eul_ang[0]);
  b_1 = cos(rtu_Sensor->eul_ang[0]);
  b_2 = sin(rtu_Sensor->eul_ang[1]);
  tmp = cos(rtu_Sensor->eul_ang[1]);
  tmp_0 = sin(rtu_Sensor->eul_ang[2]);
  tmp_1 = cos(rtu_Sensor->eul_ang[2]);
  tmp_2[0] = tmp;
  tmp_2[3] = 0.0;
  tmp_2[6] = -b_2;
  tmp_2[2] = b_2;
  tmp_2[5] = 0.0;
  tmp_2[8] = tmp;
  tmp_3[0] = tmp_1;
  tmp_3[3] = tmp_0;
  tmp_3[6] = 0.0;
  tmp_3[1] = -tmp_0;
  tmp_3[4] = tmp_1;
  tmp_3[7] = 0.0;
  tmp_2[1] = 0.0;
  tmp_3[2] = 0.0;
  b[0] = 1.0;
  tmp_2[4] = 1.0;
  tmp_3[5] = 0.0;
  b[3] = 0.0;
  tmp_2[7] = 0.0;
  tmp_3[8] = 1.0;
  b[6] = 0.0;
  b[1] = 0.0;
  b[4] = b_1;
  b[7] = u;
  b[2] = 0.0;
  b[5] = -u;
  b[8] = b_1;
  for (i_0 = 0; i_0 < 3; i_0++) {
    u = tmp_3[3 * i_0];
    b_1 = tmp_3[(3 * i_0) + 1];
    b_2 = tmp_3[(3 * i_0) + 2];
    for (i = 0; i < 3; i++) {
      tmp_4[i + (3 * i_0)] = ((tmp_2[i] * u) + (tmp_2[i + 3] * b_1)) + (tmp_2[i
        + 6] * b_2);
    }
  }

  for (i_0 = 0; i_0 < 3; i_0++) {
    /* Sum: '<S3>/Sum3' */
    u = 0.0;
    b_0 = (int32_T)b[i_0];
    b_1 = b[i_0 + 3];
    b_2 = b[i_0 + 6];

    /* Sum: '<S3>/Sum3' incorporates:
     *  Concatenate: '<S3>/Vector Concatenate1'
     *  Product: '<S3>/Product'
     */
    for (i = 0; i < 3; i++) {
      u += (((((real_T)b_0) * tmp_4[3 * i]) + (b_1 * tmp_4[(3 * i) + 1])) + (b_2
             * tmp_4[(3 * i) + 2])) * FW_attCtrl_ConstB.VectorConcatenate1[i];
    }

    rtu_Sensor_0[i_0] = rtu_Sensor->accel_b[i_0] + u;
  }

  /* End of MATLAB Function: '<S8>/MATLAB Function' */

  /* Switch: '<S3>/Switch' incorporates:
   *  Constant: '<S3>/Constant4'
   *  Constant: '<S3>/Constant5'
   */
  if (*rtu_FW_Controller_att_pitch_reset) {
    u = 1.0;
  } else {
    u = 0.6;
  }

  /* Sum: '<S3>/Add' incorporates:
   *  Constant: '<S3>/K_az'
   *  Constant: '<S3>/K_q'
   *  Constant: '<S3>/K_theta'
   *  DiscreteIntegrator: '<S3>/Discrete-Time Integrator'
   *  Product: '<S3>/Product2'
   *  Product: '<S3>/Product3'
   *  Product: '<S3>/Product4'
   *  Product: '<S3>/Product5'
   *  Sum: '<S3>/Sum2'
   *  Sum: '<S3>/Sum3'
   *  Switch: '<S3>/Switch'
   */
  *rty_ele_L = ((FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE + (31.0457 * ((u *
    rtb_RateLimiter2) - rtu_Sensor->eul_ang[1]))) - (8.9398 * rtu_Sensor->omg[1]))
    - (0.051 * rtu_Sensor_0[2]);

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant4'
   */
  if (*rtu_FW_Controller_att_roll_reset) {
    u = 1.0;
  } else {
    u = 0.6;
  }

  /* Sum: '<S4>/Sum2' incorporates:
   *  DiscreteIntegrator: '<S4>/Discrete-Time Integrator'
   *  Gain: '<S4>/Gain1'
   *  Gain: '<S4>/Gain2'
   *  Product: '<S4>/Product5'
   *  Sum: '<S4>/Sum1'
   *  Sum: '<S4>/Sum3'
   *  Switch: '<S4>/Switch'
   */
  *rty_ail_LO = (FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ej0a0oln2v +
                 (43.1874 * ((rtb_RateLimiter * u) - rtu_Sensor->eul_ang[0]))) -
    (8.3536 * rtu_Sensor->omg[0]);

  /* Switch: '<S5>/Switch' incorporates:
   *  Constant: '<S5>/Constant1'
   *  RelationalOperator: '<S5>/GreaterThan'
   */
  if (rtu_Sensor->gspd > 5.0) {
    u = rtu_Sensor->gspd;
  } else {
    u = 5.0;
  }

  /* Gain: '<S5>/Gain2' incorporates:
   *  Constant: '<S5>/Constant'
   *  Product: '<S5>/Product'
   *  Sum: '<S5>/Sum'
   *  Switch: '<S5>/Switch'
   *  Trigonometry: '<S5>/Sin'
   */
  *rty_rud_L = 30.0 * (((9.81 * sin(rtb_RateLimiter)) / u) - rtu_Sensor->omg[2]);

  /* Update for DiscreteIntegrator: '<S3>/Discrete-Time Integrator' incorporates:
   *  Constant: '<S3>/K_itgt_theta'
   *  Product: '<S3>/Product6'
   *  Product: '<S3>/Product7'
   */
  FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE += 0.01 * (22.837 * (((real_T)
    rtb_Switch) * rtb_Sum_fphn0s52rh));
  FW_attCtrl_DW.DiscreteTimeIntegrator_PrevResetState = (int8_T)
    ((*rtu_FW_Controller_att_pitch_reset) ? 1 : 0);

  /* Saturate: '<Root>/Saturation2' */
  if ((*rtu_FW_Controller_att_pitch_cmd) > 0.26179938779914941) {
    u = 0.26179938779914941;
  } else if ((*rtu_FW_Controller_att_pitch_cmd) < -0.26179938779914941) {
    u = -0.26179938779914941;
  } else {
    u = *rtu_FW_Controller_att_pitch_cmd;
  }

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S1>/Gain'
   *  Saturate: '<Root>/Saturation2'
   *  Sum: '<S1>/Sum'
   */
  FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_is34kceaix += 0.01 * (10.0 * (u -
    FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_is34kceaix));

  /* Update for DiscreteIntegrator: '<S4>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S4>/Gain'
   *  Product: '<S4>/Product6'
   */
  FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ej0a0oln2v += 0.01 * (((real_T)
    rtb_Switch_mn4j10pnvz) * (35.3351 * rtb_Sum3_bewwz0zu24));
  FW_attCtrl_DW.DiscreteTimeIntegrator_PrevResetState_i2m1oat1uz = (int8_T)
    ((*rtu_FW_Controller_att_roll_reset) ? 1 : 0);

  /* Saturate: '<Root>/Saturation1' */
  if ((*rtu_FW_Controller_att_roll_cmd) > 0.5236) {
    u = 0.5236;
  } else if ((*rtu_FW_Controller_att_roll_cmd) < -0.5236) {
    u = -0.5236;
  } else {
    u = *rtu_FW_Controller_att_roll_cmd;
  }

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  Gain: '<S2>/Gain'
   *  Saturate: '<Root>/Saturation1'
   *  Sum: '<S2>/Sum'
   */
  FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ntgr3gtdbx += 0.01 * (10.0 * (u -
    FW_attCtrl_DW.DiscreteTimeIntegrator_DSTATE_ntgr3gtdbx));
}

/* Model initialize function */
void FW_attCtrl_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&FW_attCtrl_DW, 0,
                sizeof(FW_attCtrl_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
