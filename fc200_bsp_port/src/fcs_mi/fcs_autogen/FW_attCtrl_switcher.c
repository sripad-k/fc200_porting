/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_attCtrl_switcher.c
 *
 * Code generated for Simulink model 'FW_attCtrl_switcher'.
 *
 * Model version                  : 2.57
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:14 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FW_attCtrl_switcher.h"
#include "FW_attCtrl_switcher_types.h"
#include "rtwtypes.h"
#include "FW_attCtrl_switcher_private.h"
#include <string.h>

/* Block states (default storage) */
FW_attCtrl_switcher_TDW FW_attCtrl_switcher_DW;

/* System initialize for referenced model: 'FW_attCtrl_switcher' */
void FW_attCtrl_switcher_Init(void)
{
  /* InitializeConditions for Delay: '<S14>/Delay' */
  FW_attCtrl_switcher_DW.icLoad = true;
}

/* Output and update for referenced model: 'FW_attCtrl_switcher' */
void FW_attCtrl_switcher(const vom_t *rtu_vom_status, const real_T
  *rtu_Pilot_engine_ch, const real_T *rtu_Sensor_aspd_cas, const uint8_T
  *rtu_TECS_mode, const real_T *rtu_ControllerTECS_engineCmd, const real_T
  *rtu_ControllerTECS_theta_cmd, const real_T *rtu_MR_att_cmd_rollCmd, const
  real_T *rtu_MR_att_cmd_pitchCmd, const real_T
  *rtu_mode_data_ft_data_FT_PusherThrottle, const real_T
  *rtu_mode_data_bt_data_BT_PusherThrottle, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const real_T *rtu_rollCmd_FW_guidance,
  busFW_Controller_att *rty_busFW_Controller_att)
{
  real_T rtb_BusAssignment2_kto4fbsvxd_pitch_cmd;
  real_T rtb_Saturation;
  real_T rtb_Saturation_ey2vnfgrbn;
  real_T rtb_Saturation_kt2iok1bou;
  real_T rtb_Switch;
  real_T rtb_UkYk1;
  boolean_T rtb_BusAssignment2_bjcftf0k5t_roll_reset;
  boolean_T rtb_BusAssignment2_feseftyew1_roll_reset;
  boolean_T rtb_BusAssignment2_gp1xqxoqtd_roll_reset;
  boolean_T rtb_BusAssignment2_hpg3cr2njl_roll_reset;
  boolean_T rtb_BusAssignment2_kto4fbsvxd_roll_reset;
  boolean_T rtb_Compare_evr0ymsvl3;

  /* MultiPortSwitch: '<S2>/Multiport Switch1' incorporates:
   *  Constant: '<S11>/Constant'
   *  RelationalOperator: '<S11>/Compare'
   */
  if ((*rtu_mode_data_eFWLifter_Mode) != OFF) {
    /* BusAssignment: '<S2>/Bus Assignment2' */
    rtb_BusAssignment2_kto4fbsvxd_pitch_cmd = *rtu_MR_att_cmd_pitchCmd;

    /* MultiPortSwitch: '<S2>/Multiport Switch2' */
    rtb_Saturation_ey2vnfgrbn = *rtu_mode_data_bt_data_BT_PusherThrottle;
  } else {
    /* BusAssignment: '<S2>/Bus Assignment2' */
    rtb_BusAssignment2_kto4fbsvxd_pitch_cmd = *rtu_ControllerTECS_theta_cmd;

    /* MultiPortSwitch: '<S2>/Multiport Switch2' */
    rtb_Saturation_ey2vnfgrbn = *rtu_ControllerTECS_engineCmd;
  }

  /* End of MultiPortSwitch: '<S2>/Multiport Switch1' */

  /* Saturate: '<S2>/Saturation' */
  if (rtb_Saturation_ey2vnfgrbn > 1.0) {
    rtb_Saturation_kt2iok1bou = 1.0;
  } else if (rtb_Saturation_ey2vnfgrbn < 0.0) {
    rtb_Saturation_kt2iok1bou = 0.0;
  } else {
    rtb_Saturation_kt2iok1bou = rtb_Saturation_ey2vnfgrbn;
  }

  /* End of Saturate: '<S2>/Saturation' */

  /* BusAssignment: '<S2>/Bus Assignment2' incorporates:
   *  Constant: '<S9>/Constant'
   *  RelationalOperator: '<S9>/Compare'
   */
  rtb_BusAssignment2_kto4fbsvxd_roll_reset = ((*rtu_Sensor_aspd_cas) <= 5.0);

  /* BusAssignment: '<S3>/Bus Assignment2' incorporates:
   *  Constant: '<S12>/Constant'
   *  RelationalOperator: '<S12>/Compare'
   */
  rtb_BusAssignment2_hpg3cr2njl_roll_reset = ((*rtu_Sensor_aspd_cas) <= 5.0);

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S4>/100pwm_per_sec'
   *  Constant: '<S4>/500pwm_per_sec'
   *  UnitDelay: '<S4>/Unit Delay'
   */
  if (FW_attCtrl_switcher_DW.UnitDelay_DSTATE) {
    rtb_Switch = 0.1;
  } else {
    rtb_Switch = 0.5;
  }

  /* End of Switch: '<S4>/Switch' */

  /* Product: '<S14>/delta rise limit' incorporates:
   *  SampleTimeMath: '<S14>/sample time'
   *
   * About '<S14>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  rtb_Saturation_ey2vnfgrbn = rtb_Switch * 0.01;

  /* Delay: '<S14>/Delay' */
  if (FW_attCtrl_switcher_DW.icLoad) {
    FW_attCtrl_switcher_DW.Delay_DSTATE =
      *rtu_mode_data_ft_data_FT_PusherThrottle;
  }

  /* Sum: '<S14>/Difference Inputs1' incorporates:
   *  Delay: '<S14>/Delay'
   *
   * Block description for '<S14>/Difference Inputs1':
   *
   *  Add in CPU
   */
  rtb_UkYk1 = (*rtu_mode_data_ft_data_FT_PusherThrottle) -
    FW_attCtrl_switcher_DW.Delay_DSTATE;

  /* Switch: '<S16>/Switch2' incorporates:
   *  RelationalOperator: '<S16>/LowerRelop1'
   */
  if (rtb_UkYk1 <= rtb_Saturation_ey2vnfgrbn) {
    /* Product: '<S14>/delta fall limit' incorporates:
     *  Gain: '<S4>/Gain'
     *  SampleTimeMath: '<S14>/sample time'
     *
     * About '<S14>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    rtb_Saturation_ey2vnfgrbn = 0.01 * (-rtb_Switch);

    /* Switch: '<S16>/Switch' incorporates:
     *  RelationalOperator: '<S16>/UpperRelop'
     */
    if (rtb_UkYk1 >= rtb_Saturation_ey2vnfgrbn) {
      rtb_Saturation_ey2vnfgrbn = rtb_UkYk1;
    }

    /* End of Switch: '<S16>/Switch' */
  }

  /* End of Switch: '<S16>/Switch2' */

  /* Sum: '<S14>/Difference Inputs2' incorporates:
   *  Delay: '<S14>/Delay'
   *
   * Block description for '<S14>/Difference Inputs2':
   *
   *  Add in CPU
   */
  rtb_UkYk1 = rtb_Saturation_ey2vnfgrbn + FW_attCtrl_switcher_DW.Delay_DSTATE;

  /* Saturate: '<S4>/Saturation' */
  if (rtb_UkYk1 > 1.0) {
    rtb_Saturation_ey2vnfgrbn = 1.0;
  } else if (rtb_UkYk1 < 0.0) {
    rtb_Saturation_ey2vnfgrbn = 0.0;
  } else {
    rtb_Saturation_ey2vnfgrbn = rtb_UkYk1;
  }

  /* End of Saturate: '<S4>/Saturation' */

  /* BusAssignment: '<S4>/Bus Assignment2' incorporates:
   *  Constant: '<S13>/Constant'
   *  RelationalOperator: '<S13>/Compare'
   */
  rtb_Switch = rtb_Saturation_ey2vnfgrbn;
  rtb_BusAssignment2_feseftyew1_roll_reset = ((*rtu_Sensor_aspd_cas) <= 5.0);

  /* Update for UnitDelay: '<S4>/Unit Delay' incorporates:
   *  Constant: '<S15>/Constant'
   *  RelationalOperator: '<S15>/Compare'
   */
  FW_attCtrl_switcher_DW.UnitDelay_DSTATE = (rtb_Saturation_ey2vnfgrbn >= 0.5);

  /* BusAssignment: '<S5>/Bus Assignment2' incorporates:
   *  Constant: '<S17>/Constant'
   *  RelationalOperator: '<S17>/Compare'
   */
  rtb_BusAssignment2_bjcftf0k5t_roll_reset = ((*rtu_Sensor_aspd_cas) <= 5.0);

  /* BusAssignment: '<S6>/Bus Assignment2' incorporates:
   *  Constant: '<S18>/Constant'
   *  RelationalOperator: '<S18>/Compare'
   */
  rtb_BusAssignment2_gp1xqxoqtd_roll_reset = ((*rtu_Sensor_aspd_cas) <= 5.0);

  /* MultiPortSwitch: '<S7>/Multiport Switch1' */
  if ((*rtu_TECS_mode) == 0U) {
    rtb_Saturation_ey2vnfgrbn = *rtu_MR_att_cmd_pitchCmd;

    /* MultiPortSwitch: '<S7>/Multiport Switch2' */
    rtb_Saturation = *rtu_Pilot_engine_ch;
  } else {
    rtb_Saturation_ey2vnfgrbn = *rtu_ControllerTECS_theta_cmd;

    /* MultiPortSwitch: '<S7>/Multiport Switch2' */
    rtb_Saturation = *rtu_ControllerTECS_engineCmd;
  }

  /* End of MultiPortSwitch: '<S7>/Multiport Switch1' */

  /* RelationalOperator: '<S19>/Compare' incorporates:
   *  Constant: '<S19>/Constant'
   */
  rtb_Compare_evr0ymsvl3 = ((*rtu_Sensor_aspd_cas) <= 5.0);

  /* MultiPortSwitch: '<Root>/Multiport Switch' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment2'
   *  BusAssignment: '<S2>/Bus Assignment2'
   *  BusAssignment: '<S3>/Bus Assignment2'
   *  BusAssignment: '<S4>/Bus Assignment2'
   *  BusAssignment: '<S5>/Bus Assignment2'
   *  BusAssignment: '<S6>/Bus Assignment2'
   *  BusAssignment: '<S7>/Bus Assignment2'
   *  BusAssignment: '<S8>/Bus Assignment2'
   *  Constant: '<S1>/Constant3'
   *  Constant: '<S8>/Constant'
   */
  switch (*rtu_vom_status) {
   case UMAN:
    rty_busFW_Controller_att->roll_cmd = *rtu_MR_att_cmd_rollCmd;
    rty_busFW_Controller_att->pitch_cmd = rtb_Saturation_ey2vnfgrbn;

    /* Saturate: '<S7>/Saturation' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment2'
     */
    if (rtb_Saturation > 1.0) {
      rty_busFW_Controller_att->pusher_cmd = 1.0;
    } else if (rtb_Saturation < 0.0) {
      rty_busFW_Controller_att->pusher_cmd = 0.0;
    } else {
      rty_busFW_Controller_att->pusher_cmd = rtb_Saturation;
    }

    /* End of Saturate: '<S7>/Saturation' */
    rty_busFW_Controller_att->roll_reset = rtb_Compare_evr0ymsvl3;
    rty_busFW_Controller_att->pitch_reset = rtb_Compare_evr0ymsvl3;
    rty_busFW_Controller_att->yaw_reset = rtb_Compare_evr0ymsvl3;
    break;

   case F_TRANS:
    rty_busFW_Controller_att->roll_cmd = *rtu_MR_att_cmd_rollCmd;
    rty_busFW_Controller_att->pitch_cmd = *rtu_MR_att_cmd_pitchCmd;
    rty_busFW_Controller_att->pusher_cmd = rtb_Switch;
    rty_busFW_Controller_att->roll_reset =
      rtb_BusAssignment2_feseftyew1_roll_reset;
    rty_busFW_Controller_att->pitch_reset =
      rtb_BusAssignment2_feseftyew1_roll_reset;
    rty_busFW_Controller_att->yaw_reset =
      rtb_BusAssignment2_feseftyew1_roll_reset;
    break;

   case B_TRANS:
    rty_busFW_Controller_att->roll_cmd = *rtu_MR_att_cmd_rollCmd;
    rty_busFW_Controller_att->pitch_cmd =
      rtb_BusAssignment2_kto4fbsvxd_pitch_cmd;
    rty_busFW_Controller_att->pusher_cmd = rtb_Saturation_kt2iok1bou;
    rty_busFW_Controller_att->roll_reset =
      rtb_BusAssignment2_kto4fbsvxd_roll_reset;
    rty_busFW_Controller_att->pitch_reset =
      rtb_BusAssignment2_kto4fbsvxd_roll_reset;
    rty_busFW_Controller_att->yaw_reset =
      rtb_BusAssignment2_kto4fbsvxd_roll_reset;
    break;

   case FLTDIR:
    rty_busFW_Controller_att->roll_cmd = *rtu_rollCmd_FW_guidance;
    rty_busFW_Controller_att->pitch_cmd = *rtu_ControllerTECS_theta_cmd;

    /* Saturate: '<S3>/Saturation' */
    if ((*rtu_ControllerTECS_engineCmd) > 1.0) {
      rty_busFW_Controller_att->pusher_cmd = 1.0;
    } else if ((*rtu_ControllerTECS_engineCmd) < 0.0) {
      rty_busFW_Controller_att->pusher_cmd = 0.0;
    } else {
      rty_busFW_Controller_att->pusher_cmd = *rtu_ControllerTECS_engineCmd;
    }

    /* End of Saturate: '<S3>/Saturation' */
    rty_busFW_Controller_att->roll_reset =
      rtb_BusAssignment2_hpg3cr2njl_roll_reset;
    rty_busFW_Controller_att->pitch_reset =
      rtb_BusAssignment2_hpg3cr2njl_roll_reset;
    rty_busFW_Controller_att->yaw_reset =
      rtb_BusAssignment2_hpg3cr2njl_roll_reset;
    break;

   case LOITER:
    rty_busFW_Controller_att->roll_cmd = *rtu_rollCmd_FW_guidance;
    rty_busFW_Controller_att->pitch_cmd = *rtu_ControllerTECS_theta_cmd;

    /* Saturate: '<S6>/Saturation' */
    if ((*rtu_ControllerTECS_engineCmd) > 1.0) {
      rty_busFW_Controller_att->pusher_cmd = 1.0;
    } else if ((*rtu_ControllerTECS_engineCmd) < 0.0) {
      rty_busFW_Controller_att->pusher_cmd = 0.0;
    } else {
      rty_busFW_Controller_att->pusher_cmd = *rtu_ControllerTECS_engineCmd;
    }

    /* End of Saturate: '<S6>/Saturation' */
    rty_busFW_Controller_att->roll_reset =
      rtb_BusAssignment2_gp1xqxoqtd_roll_reset;
    rty_busFW_Controller_att->pitch_reset =
      rtb_BusAssignment2_gp1xqxoqtd_roll_reset;
    rty_busFW_Controller_att->yaw_reset =
      rtb_BusAssignment2_gp1xqxoqtd_roll_reset;
    break;

   case WAYPNT:
    rty_busFW_Controller_att->roll_cmd = *rtu_rollCmd_FW_guidance;
    rty_busFW_Controller_att->pitch_cmd = *rtu_ControllerTECS_theta_cmd;

    /* Saturate: '<S8>/Saturation' */
    if ((*rtu_ControllerTECS_engineCmd) > 1.0) {
      rty_busFW_Controller_att->pusher_cmd = 1.0;
    } else if ((*rtu_ControllerTECS_engineCmd) < 0.0) {
      rty_busFW_Controller_att->pusher_cmd = 0.0;
    } else {
      rty_busFW_Controller_att->pusher_cmd = *rtu_ControllerTECS_engineCmd;
    }

    /* End of Saturate: '<S8>/Saturation' */
    rty_busFW_Controller_att->roll_reset = false;
    rty_busFW_Controller_att->pitch_reset = false;
    rty_busFW_Controller_att->yaw_reset = false;
    break;

   case FW_RTH:
    rty_busFW_Controller_att->roll_cmd = *rtu_rollCmd_FW_guidance;
    rty_busFW_Controller_att->pitch_cmd = *rtu_ControllerTECS_theta_cmd;

    /* Saturate: '<S5>/Saturation' */
    if ((*rtu_ControllerTECS_engineCmd) > 1.0) {
      rty_busFW_Controller_att->pusher_cmd = 1.0;
    } else if ((*rtu_ControllerTECS_engineCmd) < 0.0) {
      rty_busFW_Controller_att->pusher_cmd = 0.0;
    } else {
      rty_busFW_Controller_att->pusher_cmd = *rtu_ControllerTECS_engineCmd;
    }

    /* End of Saturate: '<S5>/Saturation' */
    rty_busFW_Controller_att->roll_reset =
      rtb_BusAssignment2_bjcftf0k5t_roll_reset;
    rty_busFW_Controller_att->pitch_reset =
      rtb_BusAssignment2_bjcftf0k5t_roll_reset;
    rty_busFW_Controller_att->yaw_reset =
      rtb_BusAssignment2_bjcftf0k5t_roll_reset;
    break;

   default:
    rty_busFW_Controller_att->roll_cmd = *rtu_MR_att_cmd_rollCmd;
    rty_busFW_Controller_att->pitch_cmd = *rtu_MR_att_cmd_pitchCmd;
    rty_busFW_Controller_att->pusher_cmd = FW_attCtrl_switcher_ConstB.Saturation;
    rty_busFW_Controller_att->roll_reset = true;
    rty_busFW_Controller_att->pitch_reset = true;
    rty_busFW_Controller_att->yaw_reset = true;
    break;
  }

  /* End of MultiPortSwitch: '<Root>/Multiport Switch' */

  /* Update for Delay: '<S14>/Delay' */
  FW_attCtrl_switcher_DW.icLoad = false;
  FW_attCtrl_switcher_DW.Delay_DSTATE = rtb_UkYk1;
}

/* Model initialize function */
void FW_attCtrl_switcher_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&FW_attCtrl_switcher_DW, 0,
                sizeof(FW_attCtrl_switcher_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
