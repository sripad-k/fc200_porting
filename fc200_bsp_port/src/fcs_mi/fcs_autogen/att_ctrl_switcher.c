/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: att_ctrl_switcher.c
 *
 * Code generated for Simulink model 'att_ctrl_switcher'.
 *
 * Model version                  : 2.203
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:10 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "att_ctrl_switcher.h"
#include "rtwtypes.h"
#include "att_ctrl_switcher_types.h"
#include "att_ctrl_switcher_private.h"
#include "rt_modd.h"
#include <math.h>
#include "look1_binlca.h"
#include "look1_binlc.h"
#include "zero_crossing_types.h"
#include <string.h>

/* Block states (default storage) */
att_ctrl_switcher_TDW att_ctrl_switcher_DW;

/* Previous zero-crossings (trigger) states */
att_ctrl_switcher_TZCE att_ctrl_switcher_PrevZCX;

/* System initialize for referenced model: 'att_ctrl_switcher' */
void att_ctrl_switcher_Init(void)
{
  /* InitializeConditions for UnitDelay: '<S30>/FixPt Unit Delay2' */
  att_ctrl_switcher_DW.FixPtUnitDelay2_DSTATE = 1U;

  /* InitializeConditions for Delay: '<S26>/Delay' */
  att_ctrl_switcher_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S34>/Delay' */
  att_ctrl_switcher_DW.icLoad_fqpjqu1dam = true;

  /* InitializeConditions for RateLimiter: '<S9>/Rate Limiter' */
  att_ctrl_switcher_DW.PrevY = 0.0;

  /* InitializeConditions for Delay: '<S45>/Delay' */
  att_ctrl_switcher_DW.icLoad_b1rfgb1u0f = true;
}

/* Output and update for referenced model: 'att_ctrl_switcher' */
void att_ctrl_switcher(const real_T rtu_sensor_eul_ang[3], const real_T
  rtu_sensor_omg[3], const real_T *rtu_sensor_aspd_cas, const real_T
  *rtu_sensor_h_radar_agl, const real_T *rtu_sensor_gspd, const real_T
  *rtu_sensor_chi, const real_T *rtu_attAltCmd_rollCmd, const real_T
  *rtu_attAltCmd_pitchCmd, const vom_t *rtu_vom_status, const pilot_ext_t
  *rtu_Pilot, const boolean_T *rtu_bInAirFlag, const boolean_T
  *rtu_sFlags_bGPSLoss, const real_T *rtu_mode_data_hover_data_hover_yaw_ref,
  const boolean_T *rtu_mode_data_hover_data_hover_yaw_override, const real_T
  *rtu_mode_data_autotakeoff_data_takeoff_yaw_ref, const real_T
  *rtu_mode_data_autoland_data_land_yaw_ref, const real_T
  *rtu_mode_data_rth_data_rth_yaw_ref, const boolean_T
  *rtu_mode_data_rth_data_bBreaking, const real_T
  *rtu_mode_data_ft_data_FT_Heading, const real_T
  *rtu_mode_data_bt_data_BT_Heading, const lifter_state_t
  *rtu_mode_data_eFWLifter_Mode, const uint8_T *rtu_loiter_mode, const real_T
  *rtu_rollCmd_fw_guid, const uint8_T *rtu_TECS_mode, const real_T
  *rtu_dTECSThetaCmd_rad, const uint8_T *rtu_CoG_tracking, const lifter_state_t *
  rtu_lifter_state, real_T *rty_busControllerIF_att_rollCmd, real_T
  *rty_busControllerIF_att_pitchCmd, real_T *rty_busControllerIF_att_yawCmd,
  real_T *rty_busControllerIF_att_yawRateCmd, boolean_T
  *rty_busControllerIF_att_yawHold, real_T *rty_busControllerIF_att_yawFF,
  boolean_T *rty_busControllerIF_att_init_reset)
{
  real_T rtb_BusAssignment1_bei013wcip_pitchCmd;
  real_T rtb_BusAssignment1_bei013wcip_rollCmd;
  real_T rtb_BusAssignment1_bx3v0n5vhq_pitchCmd;
  real_T rtb_BusAssignment1_bx3v0n5vhq_rollCmd;
  real_T rtb_BusAssignment1_hzqxy3zxod_pitchCmd;
  real_T rtb_BusAssignment1_nfzy05a3pt_pitchCmd;
  real_T rtb_Gain_pvwdd3azmu;
  real_T rtb_Switch;
  real_T rtb_Switch1_lxdynkvoci;
  real_T rtb_Switch2;
  real_T rtb_Switch2_oqkzp4xkeg;
  real_T rtb_Switch3_h3usfnxrts;
  real_T rtb_Switch_jkvud5bem4_idx_0;
  real_T rtb_Switch_jkvud5bem4_idx_1;
  real_T rtb_UkYk1;
  real_T rtb_deltafalllimit;
  real_T rtb_psi_cmd_nxa3f4f3og;
  real_T rtb_uDLookupTable;
  int32_T tmp;
  boolean_T rtb_Compare_bdk0wsg3vg;
  boolean_T rtb_LogicalOperator2_tmp;
  boolean_T rtb_OR_kzzf1eb5vp;
  boolean_T rtb_OR_lhfjdl0z3o;

  /* Switch: '<S12>/Switch' incorporates:
   *  Constant: '<S14>/Constant'
   *  Constant: '<S15>/Constant'
   *  RelationalOperator: '<S14>/Compare'
   *  Switch: '<S13>/Switch'
   *  Switch: '<S15>/Switch'
   */
  if (*rtu_sFlags_bGPSLoss) {
    /* BusAssignment: '<S2>/Bus Assignment1' incorporates:
     *  Constant: '<S12>/Constant'
     *  Constant: '<S12>/Constant1'
     */
    rtb_BusAssignment1_bx3v0n5vhq_rollCmd = 0.0;
    rtb_BusAssignment1_bx3v0n5vhq_pitchCmd = 0.0;
    *rty_busControllerIF_att_rollCmd = 0.0;

    /* BusAssignment: '<S3>/Bus Assignment1' incorporates:
     *  Constant: '<S15>/Constant'
     *  Constant: '<S15>/Constant1'
     */
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd = 0.0;
  } else {
    if ((*rtu_sensor_h_radar_agl) <= 5.0) {
      /* Saturate: '<S13>/Saturation' incorporates:
       *  Switch: '<S13>/Switch'
       */
      if ((*rtu_attAltCmd_rollCmd) > 0.17453292519943295) {
        /* BusAssignment: '<S2>/Bus Assignment1' */
        rtb_BusAssignment1_bx3v0n5vhq_rollCmd = 0.17453292519943295;
      } else if ((*rtu_attAltCmd_rollCmd) < -0.17453292519943295) {
        /* BusAssignment: '<S2>/Bus Assignment1' */
        rtb_BusAssignment1_bx3v0n5vhq_rollCmd = -0.17453292519943295;
      } else {
        /* BusAssignment: '<S2>/Bus Assignment1' */
        rtb_BusAssignment1_bx3v0n5vhq_rollCmd = *rtu_attAltCmd_rollCmd;
      }

      if ((*rtu_attAltCmd_pitchCmd) > 0.17453292519943295) {
        /* BusAssignment: '<S2>/Bus Assignment1' */
        rtb_BusAssignment1_bx3v0n5vhq_pitchCmd = 0.17453292519943295;
      } else if ((*rtu_attAltCmd_pitchCmd) < -0.17453292519943295) {
        /* BusAssignment: '<S2>/Bus Assignment1' */
        rtb_BusAssignment1_bx3v0n5vhq_pitchCmd = -0.17453292519943295;
      } else {
        /* BusAssignment: '<S2>/Bus Assignment1' */
        rtb_BusAssignment1_bx3v0n5vhq_pitchCmd = *rtu_attAltCmd_pitchCmd;
      }

      /* End of Saturate: '<S13>/Saturation' */
    } else {
      /* BusAssignment: '<S2>/Bus Assignment1' incorporates:
       *  Switch: '<S13>/Switch'
       */
      rtb_BusAssignment1_bx3v0n5vhq_rollCmd = *rtu_attAltCmd_rollCmd;
      rtb_BusAssignment1_bx3v0n5vhq_pitchCmd = *rtu_attAltCmd_pitchCmd;
    }

    /* Switch: '<S16>/Switch' incorporates:
     *  Constant: '<S17>/Constant'
     *  RelationalOperator: '<S17>/Compare'
     */
    if ((*rtu_sensor_h_radar_agl) <= 2.0) {
      /* Saturate: '<S16>/Saturation' */
      if ((*rtu_attAltCmd_rollCmd) > 0.17453292519943295) {
        *rty_busControllerIF_att_rollCmd = 0.17453292519943295;
      } else if ((*rtu_attAltCmd_rollCmd) < -0.17453292519943295) {
        *rty_busControllerIF_att_rollCmd = -0.17453292519943295;
      } else {
        *rty_busControllerIF_att_rollCmd = *rtu_attAltCmd_rollCmd;
      }

      if ((*rtu_attAltCmd_pitchCmd) > 0.17453292519943295) {
        /* BusAssignment: '<S3>/Bus Assignment1' */
        rtb_BusAssignment1_nfzy05a3pt_pitchCmd = 0.17453292519943295;
      } else if ((*rtu_attAltCmd_pitchCmd) < -0.17453292519943295) {
        /* BusAssignment: '<S3>/Bus Assignment1' */
        rtb_BusAssignment1_nfzy05a3pt_pitchCmd = -0.17453292519943295;
      } else {
        /* BusAssignment: '<S3>/Bus Assignment1' */
        rtb_BusAssignment1_nfzy05a3pt_pitchCmd = *rtu_attAltCmd_pitchCmd;
      }

      /* End of Saturate: '<S16>/Saturation' */
    } else {
      *rty_busControllerIF_att_rollCmd = *rtu_attAltCmd_rollCmd;

      /* BusAssignment: '<S3>/Bus Assignment1' */
      rtb_BusAssignment1_nfzy05a3pt_pitchCmd = *rtu_attAltCmd_pitchCmd;
    }

    /* End of Switch: '<S16>/Switch' */
  }

  /* End of Switch: '<S12>/Switch' */

  /* RelationalOperator: '<S21>/Compare' incorporates:
   *  Constant: '<S21>/Constant'
   */
  rtb_Compare_bdk0wsg3vg = ((*rtu_vom_status) == HOVER);

  /* Switch: '<S25>/Switch' incorporates:
   *  UnitDelay: '<S25>/ud'
   */
  if (rtb_Compare_bdk0wsg3vg) {
    rtb_Switch = *rtu_attAltCmd_pitchCmd;
  } else {
    rtb_Switch = att_ctrl_switcher_DW.ud_DSTATE;
  }

  /* End of Switch: '<S25>/Switch' */

  /* Product: '<S27>/delta rise limit' incorporates:
   *  Constant: '<S25>/Constant1'
   *  SampleTimeMath: '<S27>/sample time'
   *
   * About '<S27>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  rtb_Switch2_oqkzp4xkeg = 0.005;

  /* UnitDelay: '<S30>/FixPt Unit Delay1' */
  rtb_Switch3_h3usfnxrts = att_ctrl_switcher_DW.FixPtUnitDelay1_DSTATE;

  /* Switch: '<S30>/Init' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion1'
   *  DataTypeConversion: '<S27>/Data Type Conversion'
   *  UnitDelay: '<S30>/FixPt Unit Delay2'
   */
  if (att_ctrl_switcher_DW.FixPtUnitDelay2_DSTATE != 0U) {
    rtb_Switch3_h3usfnxrts = rtb_Compare_bdk0wsg3vg ? 1.0 : 0.0;
  }

  /* End of Switch: '<S30>/Init' */

  /* Sum: '<S27>/Difference Inputs1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion1'
   *
   * Block description for '<S27>/Difference Inputs1':
   *
   *  Add in CPU
   */
  rtb_UkYk1 = ((real_T)(rtb_Compare_bdk0wsg3vg ? 1.0 : 0.0)) -
    rtb_Switch3_h3usfnxrts;

  /* Switch: '<S29>/Switch2' incorporates:
   *  RelationalOperator: '<S29>/LowerRelop1'
   */
  if (rtb_UkYk1 <= 0.005) {
    /* Product: '<S27>/delta fall limit' incorporates:
     *  SampleTimeMath: '<S27>/sample time'
     *
     * About '<S27>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    rtb_deltafalllimit = 0.01 * att_ctrl_switcher_ConstB.Gain;

    /* Switch: '<S29>/Switch' incorporates:
     *  RelationalOperator: '<S29>/UpperRelop'
     */
    if (rtb_UkYk1 < rtb_deltafalllimit) {
      rtb_Switch2_oqkzp4xkeg = rtb_deltafalllimit;
    } else {
      rtb_Switch2_oqkzp4xkeg = rtb_UkYk1;
    }

    /* End of Switch: '<S29>/Switch' */
  }

  /* End of Switch: '<S29>/Switch2' */

  /* Sum: '<S27>/Difference Inputs2'
   *
   * Block description for '<S27>/Difference Inputs2':
   *
   *  Add in CPU
   */
  rtb_UkYk1 = rtb_Switch2_oqkzp4xkeg + rtb_Switch3_h3usfnxrts;

  /* Switch: '<S25>/Switch1' incorporates:
   *  UnitDelay: '<Root>/Unit Delay'
   *  UnitDelay: '<S25>/ud1'
   */
  if (rtb_Compare_bdk0wsg3vg) {
    rtb_deltafalllimit = att_ctrl_switcher_DW.ud1_DSTATE;
  } else {
    rtb_deltafalllimit = att_ctrl_switcher_DW.UnitDelay_DSTATE;
  }

  /* End of Switch: '<S25>/Switch1' */

  /* Switch: '<S24>/Switch' */
  if (*rtu_sFlags_bGPSLoss) {
    /* BusAssignment: '<S6>/Bus Assignment1' incorporates:
     *  Constant: '<S24>/Constant'
     *  Constant: '<S24>/Constant1'
     */
    rtb_BusAssignment1_bei013wcip_rollCmd = 0.0;
    rtb_BusAssignment1_bei013wcip_pitchCmd = 0.0;
  } else {
    /* BusAssignment: '<S6>/Bus Assignment1' incorporates:
     *  Fcn: '<S25>/Fcn'
     *  Product: '<S25>/Divide'
     *  Product: '<S25>/Divide1'
     *  Sum: '<S25>/Add'
     *  Switch: '<S25>/Switch2'
     *  Switch: '<S25>/Switch4'
     */
    rtb_BusAssignment1_bei013wcip_rollCmd = *rtu_attAltCmd_rollCmd;
    rtb_BusAssignment1_bei013wcip_pitchCmd = (rtb_Switch * rtb_UkYk1) + ((1.0 -
      rtb_UkYk1) * rtb_deltafalllimit);
  }

  /* End of Switch: '<S24>/Switch' */

  /* Logic: '<S32>/AND' incorporates:
   *  Constant: '<S32>/Lower Limit'
   *  Constant: '<S32>/Upper Limit'
   *  Logic: '<S36>/AND'
   *  Logic: '<S47>/AND'
   *  RelationalOperator: '<S32>/Lower Test'
   *  RelationalOperator: '<S32>/Upper Test'
   */
  rtb_OR_lhfjdl0z3o = ((rtu_Pilot->yaw_ch >= -0.1) && (rtu_Pilot->yaw_ch <= 0.1));

  /* Logic: '<S26>/Logical Operator2' incorporates:
   *  Logic: '<S32>/AND'
   *  Logic: '<S34>/Logical Operator2'
   *  Logic: '<S45>/Logical Operator2'
   */
  rtb_LogicalOperator2_tmp = !rtb_OR_lhfjdl0z3o;

  /* Logic: '<S26>/OR' incorporates:
   *  Logic: '<S26>/Logical Operator2'
   *  Logic: '<S6>/OR'
   *  RelationalOperator: '<S22>/FixPt Relational Operator'
   *  RelationalOperator: '<S23>/FixPt Relational Operator'
   *  RelationalOperator: '<S31>/FixPt Relational Operator'
   *  UnitDelay: '<S22>/Delay Input1'
   *  UnitDelay: '<S23>/Delay Input1'
   *  UnitDelay: '<S31>/Delay Input1'
   *
   * Block description for '<S22>/Delay Input1':
   *
   *  Store in Global RAM
   *
   * Block description for '<S23>/Delay Input1':
   *
   *  Store in Global RAM
   *
   * Block description for '<S31>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_OR_kzzf1eb5vp = ((((int32_T)(rtb_LogicalOperator2_tmp ? 1 : 0)) <
                        ((int32_T)
    (att_ctrl_switcher_DW.DelayInput1_DSTATE_nuj420kciy ? 1 : 0))) ||
                       ((((int32_T)(rtb_Compare_bdk0wsg3vg ? 1 : 0)) > ((int32_T)
    (att_ctrl_switcher_DW.DelayInput1_DSTATE_mqqh0iege3 ? 1 : 0))) ||
                        ((*rtu_mode_data_hover_data_hover_yaw_override) !=
    att_ctrl_switcher_DW.DelayInput1_DSTATE_gkffkrgovk)));

  /* Delay: '<S26>/Delay' incorporates:
   *  Gain: '<S26>/Gain'
   *  Sum: '<S26>/Sum2'
   */
  att_ctrl_switcher_DW.icLoad = ((rtb_OR_kzzf1eb5vp && (((uint32_T)
    att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE) != POS_ZCSIG)) ||
    (att_ctrl_switcher_DW.icLoad));
  att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE = rtb_OR_kzzf1eb5vp ? ((ZCSigState)1)
    : ((ZCSigState)0);
  if (att_ctrl_switcher_DW.icLoad) {
    att_ctrl_switcher_DW.Delay_DSTATE = (0.7 * rtu_sensor_omg[2]) +
      rtu_sensor_eul_ang[2];
  }

  /* Switch: '<S6>/Switch' */
  if (*rtu_mode_data_hover_data_hover_yaw_override) {
    /* BusAssignment: '<S6>/Bus Assignment1' */
    *rty_busControllerIF_att_yawCmd = *rtu_mode_data_hover_data_hover_yaw_ref;
  } else {
    /* BusAssignment: '<S6>/Bus Assignment1' incorporates:
     *  Constant: '<S33>/Constant2'
     *  Delay: '<S26>/Delay'
     *  Math: '<S33>/Mod2'
     *  Math: '<S33>/Mod3'
     *  Sum: '<S33>/Add3'
     *  Sum: '<S33>/Add4'
     *  Sum: '<S33>/Sum'
     */
    *rty_busControllerIF_att_yawCmd = rt_modd(rt_modd
      ((att_ctrl_switcher_DW.Delay_DSTATE - -3.1415926535897931) +
       att_ctrl_switcher_ConstB.Sum1, att_ctrl_switcher_ConstB.Sum1),
      att_ctrl_switcher_ConstB.Sum1) - 3.1415926535897931;
  }

  /* End of Switch: '<S6>/Switch' */

  /* RelationalOperator: '<S35>/Compare' incorporates:
   *  Constant: '<S35>/Constant'
   */
  rtb_OR_kzzf1eb5vp = ((*rtu_vom_status) == MANUAL);

  /* Delay: '<S34>/Delay' */
  att_ctrl_switcher_DW.icLoad_fqpjqu1dam = ((rtb_OR_kzzf1eb5vp && (((uint32_T)
    att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_axihigadeh) != POS_ZCSIG)) ||
    (att_ctrl_switcher_DW.icLoad_fqpjqu1dam));
  att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_axihigadeh = rtb_OR_kzzf1eb5vp ?
    ((ZCSigState)1) : ((ZCSigState)0);
  if (att_ctrl_switcher_DW.icLoad_fqpjqu1dam) {
    att_ctrl_switcher_DW.Delay_DSTATE_psumudlq5n = rtu_sensor_eul_ang[2];
  }

  /* Switch: '<S34>/Switch' incorporates:
   *  Delay: '<S34>/Delay'
   *  Gain: '<S34>/Gain'
   *  Sum: '<S34>/Sum2'
   */
  if (rtb_LogicalOperator2_tmp) {
    rtb_psi_cmd_nxa3f4f3og = (0.7 * rtu_sensor_omg[2]) + rtu_sensor_eul_ang[2];
  } else {
    rtb_psi_cmd_nxa3f4f3og = att_ctrl_switcher_DW.Delay_DSTATE_psumudlq5n;
  }

  /* End of Switch: '<S34>/Switch' */

  /* BusAssignment: '<S7>/Bus Assignment1' */
  *rty_busControllerIF_att_yawHold = rtb_OR_lhfjdl0z3o;

  /* Lookup_n-D: '<S9>/1-D Lookup Table' */
  rtb_uDLookupTable = look1_binlca(*rtu_sensor_aspd_cas,
    att_ctrl_switcher_ConstP.uDLookupTable_bp01Data_p0sgdmqdiw,
    att_ctrl_switcher_ConstP.uDLookupTable_tableData_gdcbzsomvt, 2U);

  /* Switch: '<S39>/Switch2' incorporates:
   *  RelationalOperator: '<S39>/LowerRelop1'
   */
  if ((*rtu_attAltCmd_pitchCmd) <= rtb_uDLookupTable) {
    /* Switch: '<S39>/Switch' incorporates:
     *  Gain: '<S9>/Gain'
     *  RelationalOperator: '<S39>/UpperRelop'
     */
    if ((*rtu_attAltCmd_pitchCmd) < (-rtb_uDLookupTable)) {
      rtb_uDLookupTable = -rtb_uDLookupTable;
    } else {
      rtb_uDLookupTable = *rtu_attAltCmd_pitchCmd;
    }

    /* End of Switch: '<S39>/Switch' */
  }

  /* End of Switch: '<S39>/Switch2' */

  /* RateLimiter: '<S9>/Rate Limiter' */
  rtb_Gain_pvwdd3azmu = rtb_uDLookupTable - att_ctrl_switcher_DW.PrevY;
  if (rtb_Gain_pvwdd3azmu > 0.00017453292519943296) {
    rtb_Gain_pvwdd3azmu = att_ctrl_switcher_DW.PrevY + 0.00017453292519943296;
  } else if (rtb_Gain_pvwdd3azmu < -0.00017453292519943296) {
    rtb_Gain_pvwdd3azmu = att_ctrl_switcher_DW.PrevY - 0.00017453292519943296;
  } else {
    rtb_Gain_pvwdd3azmu = rtb_uDLookupTable;
  }

  att_ctrl_switcher_DW.PrevY = rtb_Gain_pvwdd3azmu;

  /* End of RateLimiter: '<S9>/Rate Limiter' */

  /* Switch: '<S38>/Switch' incorporates:
   *  Constant: '<S38>/Constant'
   *  Constant: '<S38>/Constant1'
   */
  if (*rtu_sFlags_bGPSLoss) {
    rtb_Switch_jkvud5bem4_idx_0 = 0.0;
    rtb_Switch_jkvud5bem4_idx_1 = 0.0;
  } else {
    rtb_Switch_jkvud5bem4_idx_0 = *rtu_attAltCmd_rollCmd;

    /* Switch: '<S9>/Switch' */
    if (*rtu_mode_data_rth_data_bBreaking) {
      rtb_Switch_jkvud5bem4_idx_1 = rtb_Gain_pvwdd3azmu;
    } else {
      rtb_Switch_jkvud5bem4_idx_1 = *rtu_attAltCmd_pitchCmd;
    }

    /* End of Switch: '<S9>/Switch' */
  }

  /* End of Switch: '<S38>/Switch' */

  /* Switch: '<S11>/Switch1' incorporates:
   *  Constant: '<S11>/Constant'
   *  Product: '<S11>/Product'
   */
  if ((*rtu_loiter_mode) > 0U) {
    rtb_Switch1_lxdynkvoci = *rtu_rollCmd_fw_guid;
  } else {
    rtb_Switch1_lxdynkvoci = 0.52359877559829882 * rtu_Pilot->roll_ch;
  }

  /* End of Switch: '<S11>/Switch1' */

  /* Switch: '<S11>/Switch4' */
  if ((*rtu_TECS_mode) > 0U) {
    /* BusAssignment: '<S11>/Bus Assignment1' */
    rtb_BusAssignment1_hzqxy3zxod_pitchCmd = *rtu_dTECSThetaCmd_rad;
  } else {
    /* BusAssignment: '<S11>/Bus Assignment1' incorporates:
     *  Gain: '<S11>/Gain'
     *  Lookup_n-D: '<S11>/1-D Lookup Table'
     *  Product: '<S11>/Product1'
     */
    rtb_BusAssignment1_hzqxy3zxod_pitchCmd = look1_binlc(*rtu_sensor_aspd_cas,
      att_ctrl_switcher_ConstP.uDLookupTable_bp01Data,
      att_ctrl_switcher_ConstP.uDLookupTable_tableData, 3U) *
      (-rtu_Pilot->pitch_ch);
  }

  /* End of Switch: '<S11>/Switch4' */

  /* RelationalOperator: '<S42>/Compare' incorporates:
   *  Constant: '<S42>/Constant'
   */
  rtb_OR_kzzf1eb5vp = ((*rtu_vom_status) == UMAN);

  /* Logic: '<S41>/OR' incorporates:
   *  RelationalOperator: '<S43>/FixPt Relational Operator'
   *  RelationalOperator: '<S44>/FixPt Relational Operator'
   *  UnitDelay: '<S43>/Delay Input1'
   *  UnitDelay: '<S44>/Delay Input1'
   *
   * Block description for '<S43>/Delay Input1':
   *
   *  Store in Global RAM
   *
   * Block description for '<S44>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_OR_lhfjdl0z3o = ((((int32_T)(rtb_OR_kzzf1eb5vp ? 1 : 0)) > ((int32_T)
    (att_ctrl_switcher_DW.DelayInput1_DSTATE_dt2lks5n5h ? 1 : 0))) ||
                       ((*rtu_CoG_tracking) !=
                        att_ctrl_switcher_DW.DelayInput1_DSTATE));

  /* Delay: '<S45>/Delay' */
  att_ctrl_switcher_DW.icLoad_b1rfgb1u0f = ((rtb_OR_lhfjdl0z3o && (((uint32_T)
    att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_i5j3dzdcmn) != POS_ZCSIG)) ||
    (att_ctrl_switcher_DW.icLoad_b1rfgb1u0f));
  att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_i5j3dzdcmn = rtb_OR_lhfjdl0z3o ?
    ((ZCSigState)1) : ((ZCSigState)0);
  if (att_ctrl_switcher_DW.icLoad_b1rfgb1u0f) {
    att_ctrl_switcher_DW.Delay_DSTATE_jzsp25meix = rtu_sensor_eul_ang[2];
  }

  /* Switch: '<S45>/Switch' incorporates:
   *  Delay: '<S45>/Delay'
   *  Gain: '<S45>/Gain'
   *  Sum: '<S45>/Sum2'
   */
  if (rtb_LogicalOperator2_tmp) {
    rtb_Gain_pvwdd3azmu = (0.7 * rtu_sensor_omg[2]) + rtu_sensor_eul_ang[2];
  } else {
    rtb_Gain_pvwdd3azmu = att_ctrl_switcher_DW.Delay_DSTATE_jzsp25meix;
  }

  /* End of Switch: '<S45>/Switch' */

  /* Switch: '<S41>/Switch2' incorporates:
   *  Constant: '<S41>/Constant3'
   *  Constant: '<S41>/Constant6'
   *  Constant: '<S48>/Constant2'
   *  Math: '<S48>/Mod2'
   *  Math: '<S48>/Mod3'
   *  Product: '<S41>/Divide'
   *  Product: '<S41>/Product2'
   *  Sum: '<S48>/Add3'
   *  Sum: '<S48>/Add4'
   *  Sum: '<S48>/Sum'
   *  Switch: '<S41>/Switch3'
   *  Trigonometry: '<S41>/Tan'
   */
  if ((*rtu_CoG_tracking) > 0U) {
    rtb_Switch2_oqkzp4xkeg = *rtu_sensor_chi;

    /* Switch: '<S46>/Switch3' incorporates:
     *  Abs: '<S46>/Abs'
     *  Constant: '<S46>/Constant1'
     *  Product: '<S46>/Product'
     *  Switch: '<S46>/Switch1'
     */
    if (fabs(*rtu_sensor_gspd) > 0.001) {
      rtb_Switch3_h3usfnxrts = *rtu_sensor_gspd;
    } else {
      if ((*rtu_sensor_gspd) >= 0.0) {
        /* Switch: '<S46>/Switch1' incorporates:
         *  Constant: '<S46>/Constant2'
         */
        tmp = 1;
      } else {
        /* Switch: '<S46>/Switch1' incorporates:
         *  Constant: '<S46>/Constant3'
         */
        tmp = -1;
      }

      rtb_Switch3_h3usfnxrts = 0.001 * ((real_T)tmp);
    }

    /* End of Switch: '<S46>/Switch3' */
    rtb_Switch3_h3usfnxrts = (9.81 * tan(rtb_Switch1_lxdynkvoci)) /
      rtb_Switch3_h3usfnxrts;
  } else {
    rtb_Switch2_oqkzp4xkeg = rt_modd(rt_modd((rtb_Gain_pvwdd3azmu -
      -3.1415926535897931) + att_ctrl_switcher_ConstB.Sum1_m1umzkpppt,
      att_ctrl_switcher_ConstB.Sum1_m1umzkpppt),
      att_ctrl_switcher_ConstB.Sum1_m1umzkpppt) - 3.1415926535897931;
    rtb_Switch3_h3usfnxrts = 0.0;
  }

  /* End of Switch: '<S41>/Switch2' */

  /* RelationalOperator: '<S19>/Compare' incorporates:
   *  Constant: '<S19>/Constant'
   */
  rtb_OR_lhfjdl0z3o = ((*rtu_sensor_gspd) <= 8.0);

  /* Switch: '<S5>/Switch2' incorporates:
   *  Constant: '<S5>/Constant6'
   *  Switch: '<S5>/Switch1'
   */
  if (*rtu_sFlags_bGPSLoss) {
    rtb_Switch2 = 0.0;
  } else if (rtb_OR_lhfjdl0z3o) {
    /* Switch: '<S5>/Switch1' incorporates:
     *  Constant: '<S5>/Constant2'
     */
    rtb_Switch2 = 0.0;
  } else {
    rtb_Switch2 = *rtu_rollCmd_fw_guid;
  }

  /* End of Switch: '<S5>/Switch2' */

  /* Switch: '<S5>/Switch5' incorporates:
   *  Abs: '<S20>/Abs'
   *  Constant: '<S5>/Constant7'
   *  Constant: '<S5>/Constant8'
   *  Product: '<S5>/Divide'
   *  Product: '<S5>/Product2'
   *  Switch: '<S20>/Switch1'
   *  Switch: '<S20>/Switch3'
   *  Trigonometry: '<S5>/Tan'
   */
  if (rtb_OR_lhfjdl0z3o) {
    rtb_uDLookupTable = 0.0;
  } else {
    if (fabs(*rtu_sensor_gspd) > 0.001) {
      /* Switch: '<S20>/Switch3' */
      rtb_uDLookupTable = *rtu_sensor_gspd;
    } else {
      if ((*rtu_sensor_gspd) >= 0.0) {
        /* Switch: '<S20>/Switch1' incorporates:
         *  Constant: '<S20>/Constant2'
         *  Switch: '<S20>/Switch3'
         */
        tmp = 1;
      } else {
        /* Switch: '<S20>/Switch1' incorporates:
         *  Constant: '<S20>/Constant3'
         *  Switch: '<S20>/Switch3'
         */
        tmp = -1;
      }

      /* Switch: '<S20>/Switch3' incorporates:
       *  Constant: '<S20>/Constant1'
       *  Product: '<S20>/Product'
       *  Switch: '<S20>/Switch1'
       */
      rtb_uDLookupTable = 0.001 * ((real_T)tmp);
    }

    rtb_uDLookupTable = (9.81 * tan(rtb_Switch2)) / rtb_uDLookupTable;
  }

  /* End of Switch: '<S5>/Switch5' */

  /* MultiPortSwitch: '<Root>/Multiport Switch' incorporates:
   *  BusAssignment: '<S10>/Bus Assignment1'
   *  BusAssignment: '<S11>/Bus Assignment1'
   *  BusAssignment: '<S1>/Bus Assignment1'
   *  BusAssignment: '<S2>/Bus Assignment1'
   *  BusAssignment: '<S5>/Bus Assignment1'
   *  BusAssignment: '<S6>/Bus Assignment1'
   *  BusAssignment: '<S8>/Bus Assignment1'
   *  BusAssignment: '<S9>/Bus Assignment1'
   *  Constant: '<S5>/Constant5'
   *  Product: '<S7>/Product1'
   */
  switch (*rtu_vom_status) {
   case READY:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtu_sensor_eul_ang[0];
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd = rtu_sensor_eul_ang[1];

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = rtu_sensor_eul_ang[2];

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     *  Constant: '<S8>/Constant'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     *  Constant: '<S8>/Constant1'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment1'
     *  Constant: '<S8>/Constant2'
     */
    *rty_busControllerIF_att_init_reset = true;
    break;

   case STARTUP:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtu_sensor_eul_ang[0];
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd = rtu_sensor_eul_ang[1];

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = rtu_sensor_eul_ang[2];

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     *  Constant: '<S10>/Constant1'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     *  Constant: '<S10>/Constant3'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment1'
     *  Constant: '<S10>/Constant2'
     */
    *rty_busControllerIF_att_init_reset = true;
    break;

   case TAKEOFF:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' */
    *rty_busControllerIF_att_yawCmd =
      *rtu_mode_data_autotakeoff_data_takeoff_yaw_ref;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S3>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S3>/Bus Assignment1'
     *  Constant: '<S3>/Constant'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S3>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' */
    *rty_busControllerIF_att_init_reset = !(*rtu_bInAirFlag);
    break;

   case HOVER:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtb_BusAssignment1_bei013wcip_rollCmd;
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd =
      rtb_BusAssignment1_bei013wcip_pitchCmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     *  Constant: '<S26>/Constant2'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     *  Product: '<S26>/Product2'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.52359877559829882 *
      rtu_Pilot->yaw_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S6>/Bus Assignment1'
     *  Constant: '<S6>/Constant2'
     */
    *rty_busControllerIF_att_init_reset = false;
    break;

   case MANUAL:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  Constant: '<S7>/Constant'
     *  Product: '<S7>/Product'
     */
    *rty_busControllerIF_att_rollCmd = 0.52359877559829882 * rtu_Pilot->roll_ch;
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd =
      att_ctrl_switcher_ConstB.Gain_dfrjuxii0j * rtu_Pilot->pitch_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  Constant: '<S37>/Constant2'
     *  Math: '<S37>/Mod2'
     *  Math: '<S37>/Mod3'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     *  Product: '<S7>/Product1'
     *  Sum: '<S37>/Add3'
     *  Sum: '<S37>/Add4'
     *  Sum: '<S37>/Sum'
     */
    *rty_busControllerIF_att_yawCmd = rt_modd(rt_modd((rtb_psi_cmd_nxa3f4f3og -
      -3.1415926535897931) + att_ctrl_switcher_ConstB.Sum1_b22ks15dzw,
      att_ctrl_switcher_ConstB.Sum1_b22ks15dzw),
      att_ctrl_switcher_ConstB.Sum1_b22ks15dzw) - 3.1415926535897931;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  Constant: '<S34>/Constant2'
     *  Product: '<S34>/Product2'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.52359877559829882 *
      rtu_Pilot->yaw_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' */
    *rty_busControllerIF_att_init_reset = !(*rtu_bInAirFlag);
    break;

   case LAND:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtb_BusAssignment1_bx3v0n5vhq_rollCmd;
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd =
      rtb_BusAssignment1_bx3v0n5vhq_pitchCmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = *rtu_mode_data_autoland_data_land_yaw_ref;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     *  Constant: '<S2>/Constant'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' */
    *rty_busControllerIF_att_init_reset = !(*rtu_bInAirFlag);
    break;

   case MR_RTH:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtb_Switch_jkvud5bem4_idx_0;
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd = rtb_Switch_jkvud5bem4_idx_1;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = *rtu_mode_data_rth_data_rth_yaw_ref;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     *  Constant: '<S9>/Constant'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S9>/Bus Assignment1'
     *  Constant: '<S9>/Constant1'
     */
    *rty_busControllerIF_att_init_reset = false;
    break;

   case UMAN:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtb_Switch1_lxdynkvoci;
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd =
      rtb_BusAssignment1_hzqxy3zxod_pitchCmd;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = rtb_Switch2_oqkzp4xkeg;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  Constant: '<S45>/Constant2'
     *  Product: '<S45>/Product2'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.52359877559829882 *
      rtu_Pilot->yaw_ch;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = rtb_Switch3_h3usfnxrts;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  Constant: '<S40>/Constant'
     *  Logic: '<S11>/OR1'
     *  RelationalOperator: '<S40>/Compare'
     */
    *rty_busControllerIF_att_init_reset = ((!(*rtu_bInAirFlag)) ||
      ((*rtu_lifter_state) != ON));
    break;

   case F_TRANS:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtb_Switch2;
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     *  Constant: '<S5>/Constant5'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = *rtu_mode_data_ft_data_FT_Heading;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     *  Constant: '<S5>/Constant1'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = rtb_uDLookupTable;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S5>/Bus Assignment1'
     *  Constant: '<S5>/Constant3'
     */
    *rty_busControllerIF_att_init_reset = false;
    break;

   case B_TRANS:
    /* Switch: '<S4>/Switch3' incorporates:
     *  Switch: '<S4>/Switch1'
     */
    if (*rtu_sFlags_bGPSLoss) {
      /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
       *  Constant: '<S4>/Constant5'
       */
      *rty_busControllerIF_att_rollCmd = 0.0;
    } else if ((*rtu_sensor_gspd) > 5.0) {
      /* Switch: '<S4>/Switch1' incorporates:
       *  SignalConversion generated from: '<Root>/busControllerIF_att'
       */
      *rty_busControllerIF_att_rollCmd = *rtu_rollCmd_fw_guid;
    } else {
      /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
       *  Constant: '<S4>/Zero'
       *  Switch: '<S4>/Switch1'
       */
      *rty_busControllerIF_att_rollCmd = 0.0;
    }

    /* End of Switch: '<S4>/Switch3' */

    /* Switch: '<S4>/Switch' incorporates:
     *  Constant: '<S4>/Zero'
     *  Constant: '<S4>/Zero1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    if ((*rtu_sensor_aspd_cas) > 15.0) {
      rtb_BusAssignment1_nfzy05a3pt_pitchCmd = 0.0;
    } else {
      rtb_BusAssignment1_nfzy05a3pt_pitchCmd = 0.034906585039886591;
    }

    /* End of Switch: '<S4>/Switch' */

    /* SignalConversion generated from: '<Root>/busControllerIF_att' */
    *rty_busControllerIF_att_yawCmd = *rtu_mode_data_bt_data_BT_Heading;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S4>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S4>/Bus Assignment1'
     *  Constant: '<S4>/Constant1'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S4>/Bus Assignment1'
     *  Constant: '<S4>/Constant2'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  Constant: '<S18>/Constant'
     *  RelationalOperator: '<S18>/Compare'
     */
    *rty_busControllerIF_att_init_reset = ((*rtu_mode_data_eFWLifter_Mode) != ON);
    break;

   default:
    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     */
    *rty_busControllerIF_att_rollCmd = rtu_sensor_eul_ang[0];
    rtb_BusAssignment1_nfzy05a3pt_pitchCmd = rtu_sensor_eul_ang[1];

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     *  MultiPortSwitch: '<Root>/Multiport Switch'
     */
    *rty_busControllerIF_att_yawCmd = rtu_sensor_eul_ang[2];

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     *  Constant: '<S1>/Constant'
     */
    *rty_busControllerIF_att_yawRateCmd = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     *  Constant: '<S1>/Constant1'
     */
    *rty_busControllerIF_att_yawHold = true;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     */
    *rty_busControllerIF_att_yawFF = 0.0;

    /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment1'
     *  Constant: '<S1>/Constant2'
     */
    *rty_busControllerIF_att_init_reset = true;
    break;
  }

  /* End of MultiPortSwitch: '<Root>/Multiport Switch' */

  /* SignalConversion generated from: '<Root>/busControllerIF_att' incorporates:
   *  MultiPortSwitch: '<Root>/Multiport Switch'
   */
  *rty_busControllerIF_att_pitchCmd = rtb_BusAssignment1_nfzy05a3pt_pitchCmd;

  /* Update for UnitDelay: '<S25>/ud' */
  att_ctrl_switcher_DW.ud_DSTATE = rtb_Switch;

  /* Update for UnitDelay: '<S30>/FixPt Unit Delay2' incorporates:
   *  Constant: '<S30>/FixPt Constant'
   */
  att_ctrl_switcher_DW.FixPtUnitDelay2_DSTATE = 0U;

  /* Update for UnitDelay: '<S30>/FixPt Unit Delay1' */
  att_ctrl_switcher_DW.FixPtUnitDelay1_DSTATE = rtb_UkYk1;

  /* Update for UnitDelay: '<S25>/ud1' */
  att_ctrl_switcher_DW.ud1_DSTATE = rtb_deltafalllimit;

  /* Update for UnitDelay: '<Root>/Unit Delay' incorporates:
   *  MultiPortSwitch: '<Root>/Multiport Switch'
   */
  att_ctrl_switcher_DW.UnitDelay_DSTATE = rtb_BusAssignment1_nfzy05a3pt_pitchCmd;

  /* Update for UnitDelay: '<S31>/Delay Input1' incorporates:
   *  Logic: '<S26>/Logical Operator2'
   *
   * Block description for '<S31>/Delay Input1':
   *
   *  Store in Global RAM
   */
  att_ctrl_switcher_DW.DelayInput1_DSTATE_nuj420kciy = rtb_LogicalOperator2_tmp;

  /* Update for UnitDelay: '<S23>/Delay Input1'
   *
   * Block description for '<S23>/Delay Input1':
   *
   *  Store in Global RAM
   */
  att_ctrl_switcher_DW.DelayInput1_DSTATE_mqqh0iege3 = rtb_Compare_bdk0wsg3vg;

  /* Update for UnitDelay: '<S22>/Delay Input1'
   *
   * Block description for '<S22>/Delay Input1':
   *
   *  Store in Global RAM
   */
  att_ctrl_switcher_DW.DelayInput1_DSTATE_gkffkrgovk =
    *rtu_mode_data_hover_data_hover_yaw_override;

  /* Update for Delay: '<S26>/Delay' */
  att_ctrl_switcher_DW.icLoad = false;

  /* Update for Delay: '<S34>/Delay' */
  att_ctrl_switcher_DW.icLoad_fqpjqu1dam = false;
  att_ctrl_switcher_DW.Delay_DSTATE_psumudlq5n = rtb_psi_cmd_nxa3f4f3og;

  /* Update for UnitDelay: '<S44>/Delay Input1'
   *
   * Block description for '<S44>/Delay Input1':
   *
   *  Store in Global RAM
   */
  att_ctrl_switcher_DW.DelayInput1_DSTATE_dt2lks5n5h = rtb_OR_kzzf1eb5vp;

  /* Update for UnitDelay: '<S43>/Delay Input1'
   *
   * Block description for '<S43>/Delay Input1':
   *
   *  Store in Global RAM
   */
  att_ctrl_switcher_DW.DelayInput1_DSTATE = *rtu_CoG_tracking;

  /* Update for Delay: '<S45>/Delay' */
  att_ctrl_switcher_DW.icLoad_b1rfgb1u0f = false;
  att_ctrl_switcher_DW.Delay_DSTATE_jzsp25meix = rtb_Gain_pvwdd3azmu;
}

/* Model initialize function */
void att_ctrl_switcher_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&att_ctrl_switcher_DW, 0,
                sizeof(att_ctrl_switcher_TDW));
  att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE = POS_ZCSIG;
  att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_axihigadeh = POS_ZCSIG;
  att_ctrl_switcher_PrevZCX.Delay_Reset_ZCE_i5j3dzdcmn = POS_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
