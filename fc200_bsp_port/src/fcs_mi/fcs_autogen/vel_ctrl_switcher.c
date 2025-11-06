/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: vel_ctrl_switcher.c
 *
 * Code generated for Simulink model 'vel_ctrl_switcher'.
 *
 * Model version                  : 2.83
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:53:03 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "vel_ctrl_switcher.h"
#include "rtwtypes.h"
#include "vel_ctrl_switcher_types.h"
#include "vel_ctrl_switcher_private.h"
#include "zero_crossing_types.h"
#include <string.h>

/* Block states (default storage) */
vel_ctrl_switcher_TDW vel_ctrl_switcher_DW;

/* Previous zero-crossings (trigger) states */
vel_ctrl_switcher_TZCE vel_ctrl_switcher_PrevZCX;

/* System initialize for referenced model: 'vel_ctrl_switcher' */
void vel_ctrl_switcher_Init(void)
{
  /* InitializeConditions for RateLimiter: '<S3>/Rate Limiter' */
  vel_ctrl_switcher_DW.PrevY = 0.0;

  /* InitializeConditions for Delay: '<S14>/Delay' */
  vel_ctrl_switcher_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S19>/Delay' */
  vel_ctrl_switcher_DW.icLoad_k1hpx4rg1s = true;
}

/* Output and update for referenced model: 'vel_ctrl_switcher' */
void vel_ctrl_switcher(const real_T rtu_Sensor_pos_lla[3], const real_T
  rtu_Sensor_vel_ned[3], const real_T *rtu_Sensor_h_radar_agl, const pilot_ext_t
  *rtu_Pilot, const real_T *rtu_ctrlIF_vel_velCmdH_x, const real_T
  *rtu_ctrlIF_vel_velCmdH_y, const real_T *rtu_ctrlIF_vel_hRateCmd, const vom_t *
  rtu_vom_status, const boolean_T *rtu_sFlags_bGPSLoss10sec, const boolean_T
  *rtu_sFlags_bGPSLoss, const real_T *rtu_mode_data_hover_data_hover_alt_agl,
  const real_T *rtu_mode_data_autotakeoff_data_takeoff_alt_agl, const real_T
  *rtu_mode_data_rth_data_rth_velX, const real_T
  *rtu_mode_data_rth_data_rth_velY, const real_T
  *rtu_mode_data_rth_data_rth_alt_agl, const boolean_T
  *rtu_mode_data_rth_data_bRTH_PosModeFlag, const real_T
  *rtu_mode_data_ft_data_FT_Altitude, const real_T
  *rtu_mode_data_bt_data_BT_Altitude, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const boolean_T *rtu_bInAirFlag, const uint8_T *
  rtu_TECS_mode, const lifter_state_t *rtu_lifter_state, real_T
  *rty_busControllerIF_vel_velCmdH_x, real_T *rty_busControllerIF_vel_velCmdH_y,
  real_T *rty_busControllerIF_vel_hRateCmd, real_T *rty_busControllerIF_vel_hCmd,
  boolean_T *rty_busControllerIF_vel_hHold, boolean_T
  *rty_busControllerIF_vel_init_reset_vel, boolean_T
  *rty_busControllerIF_vel_init_reset_hdot)
{
  real_T rtb_Product;
  real_T rtb_h_cmd;
  real_T rtb_h_cmd_dqcftz1lhn;
  boolean_T rtb_Compare_jiqna0fsyg;
  boolean_T rtb_Compare_llexjvrf4j;
  boolean_T rtb_OR1;
  boolean_T rtb_OR_tmp;
  boolean_T tmp;

  /* RateLimiter: '<S3>/Rate Limiter' incorporates:
   *  Constant: '<S3>/Constant6'
   */
  if ((-1.0 - vel_ctrl_switcher_DW.PrevY) > 0.00981) {
    rtb_Product = vel_ctrl_switcher_DW.PrevY + 0.00981;
  } else if ((-1.0 - vel_ctrl_switcher_DW.PrevY) < -0.00981) {
    rtb_Product = vel_ctrl_switcher_DW.PrevY - 0.00981;
  } else {
    rtb_Product = -1.0;
  }

  vel_ctrl_switcher_DW.PrevY = rtb_Product;

  /* End of RateLimiter: '<S3>/Rate Limiter' */

  /* Logic: '<S16>/AND' incorporates:
   *  Constant: '<S16>/Lower Limit'
   *  Constant: '<S16>/Upper Limit'
   *  Logic: '<S23>/AND'
   *  RelationalOperator: '<S16>/Lower Test'
   *  RelationalOperator: '<S16>/Upper Test'
   */
  rtb_OR_tmp = ((rtu_Pilot->throttle_ch >= -0.1) && (rtu_Pilot->throttle_ch <=
    0.1));

  /* RelationalOperator: '<S15>/Compare' incorporates:
   *  Constant: '<S15>/Constant'
   */
  rtb_Compare_jiqna0fsyg = ((*rtu_vom_status) == MANUAL);

  /* Delay: '<S14>/Delay' */
  vel_ctrl_switcher_DW.icLoad = ((rtb_Compare_jiqna0fsyg && (((uint32_T)
    vel_ctrl_switcher_PrevZCX.Delay_Reset_ZCE) != POS_ZCSIG)) ||
    (vel_ctrl_switcher_DW.icLoad));
  vel_ctrl_switcher_PrevZCX.Delay_Reset_ZCE = rtb_Compare_jiqna0fsyg ?
    ((ZCSigState)1) : ((ZCSigState)0);
  if (vel_ctrl_switcher_DW.icLoad) {
    vel_ctrl_switcher_DW.Delay_DSTATE = rtu_Sensor_pos_lla[2];
  }

  /* Logic: '<S14>/Logical Operator2' incorporates:
   *  Logic: '<S16>/AND'
   *  Logic: '<S19>/Logical Operator2'
   */
  tmp = !rtb_OR_tmp;

  /* Switch: '<S14>/Switch' incorporates:
   *  Delay: '<S14>/Delay'
   *  Gain: '<S14>/Gain'
   *  Gain: '<S14>/velD_to_hdot'
   *  Logic: '<S14>/Logical Operator2'
   *  Sum: '<S14>/Sum2'
   */
  if (tmp) {
    rtb_h_cmd = (0.7 * (-rtu_Sensor_vel_ned[2])) + rtu_Sensor_pos_lla[2];
  } else {
    rtb_h_cmd = vel_ctrl_switcher_DW.Delay_DSTATE;
  }

  /* End of Switch: '<S14>/Switch' */

  /* Switch: '<S9>/Switch' */
  if (*rtu_mode_data_rth_data_bRTH_PosModeFlag) {
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;
  } else {
    *rty_busControllerIF_vel_velCmdH_x = *rtu_mode_data_rth_data_rth_velX;
    *rty_busControllerIF_vel_velCmdH_y = *rtu_mode_data_rth_data_rth_velY;
  }

  /* End of Switch: '<S9>/Switch' */

  /* RelationalOperator: '<S20>/Compare' incorporates:
   *  Constant: '<S20>/Constant'
   */
  rtb_Compare_jiqna0fsyg = ((*rtu_vom_status) == UMAN);

  /* RelationalOperator: '<S24>/Compare' incorporates:
   *  Constant: '<S24>/Constant'
   */
  rtb_Compare_llexjvrf4j = ((*rtu_TECS_mode) == 0U);

  /* Logic: '<S19>/OR1' incorporates:
   *  RelationalOperator: '<S21>/FixPt Relational Operator'
   *  RelationalOperator: '<S22>/FixPt Relational Operator'
   *  UnitDelay: '<S21>/Delay Input1'
   *  UnitDelay: '<S22>/Delay Input1'
   *
   * Block description for '<S21>/Delay Input1':
   *
   *  Store in Global RAM
   *
   * Block description for '<S22>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_OR1 = ((rtb_Compare_jiqna0fsyg != vel_ctrl_switcher_DW.DelayInput1_DSTATE)
             || (rtb_Compare_llexjvrf4j !=
                 vel_ctrl_switcher_DW.DelayInput1_DSTATE_jqcqpgbfgg));

  /* Delay: '<S19>/Delay' */
  vel_ctrl_switcher_DW.icLoad_k1hpx4rg1s = ((rtb_OR1 && (((uint32_T)
    vel_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_fbmawcfj04) != POS_ZCSIG)) ||
    (vel_ctrl_switcher_DW.icLoad_k1hpx4rg1s));
  vel_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_fbmawcfj04 = rtb_OR1 ? ((ZCSigState)
    1) : ((ZCSigState)0);
  if (vel_ctrl_switcher_DW.icLoad_k1hpx4rg1s) {
    vel_ctrl_switcher_DW.Delay_DSTATE_pqtvtrkc0x = rtu_Sensor_pos_lla[2];
  }

  /* Switch: '<S19>/Switch' incorporates:
   *  Delay: '<S19>/Delay'
   *  Gain: '<S19>/Gain'
   *  Gain: '<S19>/velD_to_hdot'
   *  Sum: '<S19>/Sum2'
   */
  if (tmp) {
    rtb_h_cmd_dqcftz1lhn = (0.7 * (-rtu_Sensor_vel_ned[2])) +
      rtu_Sensor_pos_lla[2];
  } else {
    rtb_h_cmd_dqcftz1lhn = vel_ctrl_switcher_DW.Delay_DSTATE_pqtvtrkc0x;
  }

  /* End of Switch: '<S19>/Switch' */

  /* MultiPortSwitch: '<Root>/Multiport Switch' */
  switch (*rtu_vom_status) {
   case READY:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hRateCmd = *rtu_ctrlIF_vel_hRateCmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hHold = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     *  Constant: '<S8>/Constant2'
     */
    *rty_busControllerIF_vel_init_reset_vel = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     *  Constant: '<S8>/Constant1'
     */
    *rty_busControllerIF_vel_init_reset_hdot = true;
    break;

   case STARTUP:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hRateCmd = *rtu_ctrlIF_vel_hRateCmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hHold = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     *  Constant: '<S10>/Constant2'
     */
    *rty_busControllerIF_vel_init_reset_vel = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     *  Constant: '<S10>/Constant1'
     */
    *rty_busControllerIF_vel_init_reset_hdot = true;
    break;

   case TAKEOFF:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* Saturate: '<S3>/Saturation' */
    if (rtb_Product > 3.0) {
      /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
      *rty_busControllerIF_vel_hRateCmd = 3.0;
    } else if (rtb_Product < -2.0) {
      /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
      *rty_busControllerIF_vel_hRateCmd = -2.0;
    } else {
      /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
      *rty_busControllerIF_vel_hRateCmd = rtb_Product;
    }

    /* End of Saturate: '<S3>/Saturation' */

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hCmd =
      *rtu_mode_data_autotakeoff_data_takeoff_alt_agl;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hHold = !(*rtu_sFlags_bGPSLoss);

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_init_reset_vel = !(*rtu_bInAirFlag);

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S3>/Bus Assignment1'
     *  Constant: '<S3>/Constant1'
     */
    *rty_busControllerIF_vel_init_reset_hdot = false;
    break;

   case HOVER:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     *  Constant: '<S6>/Constant4'
     */
    *rty_busControllerIF_vel_hRateCmd = -1.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hCmd = *rtu_mode_data_hover_data_hover_alt_agl;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hHold = !(*rtu_sFlags_bGPSLoss10sec);

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     *  Constant: '<S6>/Constant2'
     */
    *rty_busControllerIF_vel_init_reset_vel = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     *  Constant: '<S6>/Constant5'
     */
    *rty_busControllerIF_vel_init_reset_hdot = false;
    break;

   case MANUAL:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Constant: '<S7>/Constant1'
     *  Product: '<S7>/Product1'
     */
    *rty_busControllerIF_vel_velCmdH_x = 6.0 * rtu_Pilot->pitch_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Constant: '<S7>/Constant'
     *  Product: '<S7>/Product'
     */
    *rty_busControllerIF_vel_velCmdH_y = 6.0 * rtu_Pilot->roll_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Constant: '<S14>/Constant3'
     *  Product: '<S14>/Product2'
     */
    *rty_busControllerIF_vel_hRateCmd = 2.0 * rtu_Pilot->throttle_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hCmd = rtb_h_cmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Logic: '<S16>/AND'
     */
    *rty_busControllerIF_vel_hHold = rtb_OR_tmp;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment1'
     *  Constant: '<S7>/Constant5'
     */
    *rty_busControllerIF_vel_init_reset_vel = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment1'
     *  Constant: '<S7>/Constant3'
     */
    *rty_busControllerIF_vel_init_reset_hdot = false;
    break;

   case LAND:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* Switch: '<S2>/Switch' incorporates:
     *  Constant: '<S12>/Constant'
     *  RelationalOperator: '<S12>/Compare'
     */
    if ((*rtu_Sensor_h_radar_agl) <= 10.0) {
      /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
       *  Constant: '<S2>/Constant'
       */
      *rty_busControllerIF_vel_hRateCmd = -0.5;
    } else {
      /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
       *  Constant: '<S2>/Constant1'
       */
      *rty_busControllerIF_vel_hRateCmd = -2.0;
    }

    /* End of Switch: '<S2>/Switch' */

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     *  Constant: '<S2>/Constant4'
     */
    *rty_busControllerIF_vel_hHold = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_init_reset_vel = !(*rtu_bInAirFlag);

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     *  Constant: '<S2>/Constant3'
     */
    *rty_busControllerIF_vel_init_reset_hdot = false;
    break;

   case MR_RTH:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     *  Constant: '<S9>/Constant1'
     */
    *rty_busControllerIF_vel_hRateCmd = -1.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hCmd = *rtu_mode_data_rth_data_rth_alt_agl;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hHold = !(*rtu_sFlags_bGPSLoss10sec);

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     *  Constant: '<S9>/Constant'
     */
    *rty_busControllerIF_vel_init_reset_vel = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     *  Constant: '<S9>/Constant4'
     */
    *rty_busControllerIF_vel_init_reset_hdot = false;
    break;

   case UMAN:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     *  Constant: '<S11>/Constant'
     */
    *rty_busControllerIF_vel_velCmdH_x = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     *  Constant: '<S11>/Constant'
     */
    *rty_busControllerIF_vel_velCmdH_y = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Constant: '<S19>/Constant3'
     *  Product: '<S19>/Product2'
     */
    *rty_busControllerIF_vel_hRateCmd = 2.0 * rtu_Pilot->throttle_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hCmd = rtb_h_cmd_dqcftz1lhn;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hHold = rtb_OR_tmp;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     *  Constant: '<S11>/Constant5'
     */
    *rty_busControllerIF_vel_init_reset_vel = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Constant: '<S17>/Constant'
     *  Constant: '<S18>/Constant'
     *  Logic: '<S11>/OR'
     *  RelationalOperator: '<S17>/Compare'
     *  RelationalOperator: '<S18>/Compare'
     */
    *rty_busControllerIF_vel_init_reset_hdot = (((*rtu_lifter_state) == OFF) ||
      ((*rtu_TECS_mode) == 1U));
    break;

   case F_TRANS:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     *  Constant: '<S5>/Constant1'
     */
    *rty_busControllerIF_vel_hRateCmd = -1.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hCmd = *rtu_mode_data_ft_data_FT_Altitude;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hHold = !(*rtu_sFlags_bGPSLoss10sec);

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     *  Constant: '<S5>/Constant'
     */
    *rty_busControllerIF_vel_init_reset_vel = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     *  Constant: '<S5>/Constant4'
     */
    *rty_busControllerIF_vel_init_reset_hdot = false;
    break;

   case B_TRANS:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S4>/Bus Assignment1'
     *  Constant: '<S4>/Constant1'
     */
    *rty_busControllerIF_vel_hRateCmd = -1.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hCmd = *rtu_mode_data_bt_data_BT_Altitude;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S4>/Bus Assignment1'
     *  Constant: '<S4>/Constant4'
     */
    *rty_busControllerIF_vel_hHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S4>/Bus Assignment1'
     *  Constant: '<S4>/Constant'
     */
    *rty_busControllerIF_vel_init_reset_vel = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  Constant: '<S13>/Constant'
     *  RelationalOperator: '<S13>/Compare'
     */
    *rty_busControllerIF_vel_init_reset_hdot = ((*rtu_mode_data_eFWLifter_Mode) ==
      OFF);
    break;

   default:
    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_x = *rtu_ctrlIF_vel_velCmdH_x;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_velCmdH_y = *rtu_ctrlIF_vel_velCmdH_y;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' */
    *rty_busControllerIF_vel_hRateCmd = *rtu_ctrlIF_vel_hRateCmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     */
    *rty_busControllerIF_vel_hHold = false;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     *  Constant: '<S1>/Constant2'
     */
    *rty_busControllerIF_vel_init_reset_vel = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_vel' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     *  Constant: '<S1>/Constant1'
     */
    *rty_busControllerIF_vel_init_reset_hdot = true;
    break;
  }

  /* End of MultiPortSwitch: '<Root>/Multiport Switch' */

  /* Update for Delay: '<S14>/Delay' */
  vel_ctrl_switcher_DW.icLoad = false;
  vel_ctrl_switcher_DW.Delay_DSTATE = rtb_h_cmd;

  /* Update for UnitDelay: '<S21>/Delay Input1'
   *
   * Block description for '<S21>/Delay Input1':
   *
   *  Store in Global RAM
   */
  vel_ctrl_switcher_DW.DelayInput1_DSTATE = rtb_Compare_jiqna0fsyg;

  /* Update for UnitDelay: '<S22>/Delay Input1'
   *
   * Block description for '<S22>/Delay Input1':
   *
   *  Store in Global RAM
   */
  vel_ctrl_switcher_DW.DelayInput1_DSTATE_jqcqpgbfgg = rtb_Compare_llexjvrf4j;

  /* Update for Delay: '<S19>/Delay' */
  vel_ctrl_switcher_DW.icLoad_k1hpx4rg1s = false;
  vel_ctrl_switcher_DW.Delay_DSTATE_pqtvtrkc0x = rtb_h_cmd_dqcftz1lhn;
}

/* Model initialize function */
void vel_ctrl_switcher_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&vel_ctrl_switcher_DW, 0,
                sizeof(vel_ctrl_switcher_TDW));
  vel_ctrl_switcher_PrevZCX.Delay_Reset_ZCE = POS_ZCSIG;
  vel_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_fbmawcfj04 = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
