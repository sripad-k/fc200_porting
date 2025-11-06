/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: att_ctrl_switcher_private.h
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

#ifndef att_ctrl_switcher_private_h_
#define att_ctrl_switcher_private_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "att_ctrl_switcher_types.h"

/* Block states (default storage) for model 'att_ctrl_switcher' */
typedef struct {
  real_T ud_DSTATE;                    /* '<S25>/ud' */
  real_T FixPtUnitDelay1_DSTATE;       /* '<S30>/FixPt Unit Delay1' */
  real_T ud1_DSTATE;                   /* '<S25>/ud1' */
  real_T UnitDelay_DSTATE;             /* '<Root>/Unit Delay' */
  real_T Delay_DSTATE;                 /* '<S26>/Delay' */
  real_T Delay_DSTATE_psumudlq5n;      /* '<S34>/Delay' */
  real_T Delay_DSTATE_jzsp25meix;      /* '<S45>/Delay' */
  real_T PrevY;                        /* '<S9>/Rate Limiter' */
  uint8_T FixPtUnitDelay2_DSTATE;      /* '<S30>/FixPt Unit Delay2' */
  uint8_T DelayInput1_DSTATE;          /* '<S43>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_nuj420kciy;/* '<S31>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_mqqh0iege3;/* '<S23>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_gkffkrgovk;/* '<S22>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_dt2lks5n5h;/* '<S44>/Delay Input1' */
  boolean_T icLoad;                    /* '<S26>/Delay' */
  boolean_T icLoad_fqpjqu1dam;         /* '<S34>/Delay' */
  boolean_T icLoad_b1rfgb1u0f;         /* '<S45>/Delay' */
} att_ctrl_switcher_TDW;

/* Zero-crossing (trigger) state for model 'att_ctrl_switcher' */
typedef struct {
  ZCSigState Delay_Reset_ZCE;          /* '<S26>/Delay' */
  ZCSigState Delay_Reset_ZCE_axihigadeh;/* '<S34>/Delay' */
  ZCSigState Delay_Reset_ZCE_i5j3dzdcmn;/* '<S45>/Delay' */
} att_ctrl_switcher_TZCE;

/* Invariant block signals for model 'att_ctrl_switcher' */
typedef struct {
  const real_T Gain;                   /* '<S25>/Gain' */
  const real_T Sum1;                   /* '<S33>/Sum1' */
  const real_T Gain_dfrjuxii0j;        /* '<S7>/Gain' */
  const real_T Sum1_b22ks15dzw;        /* '<S37>/Sum1' */
  const real_T Sum1_m1umzkpppt;        /* '<S48>/Sum1' */
} att_ctrl_switcher_TConstB;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [30, 20, 15, 10]*pi/180
   * Referenced by: '<S11>/1-D Lookup Table'
   */
  real_T uDLookupTable_tableData[4];

  /* Expression: [0, 10, 20, 30]
   * Referenced by: '<S11>/1-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data[4];

  /* Expression: [10, 5, 2]*pi/180
   * Referenced by: '<S9>/1-D Lookup Table'
   */
  real_T uDLookupTable_tableData_gdcbzsomvt[3];

  /* Expression: [10, 13, 30]
   * Referenced by: '<S9>/1-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data_p0sgdmqdiw[3];
} att_ctrl_switcher_d5yyrvesiv_T;

/* Constant parameters (default storage) */
extern const att_ctrl_switcher_d5yyrvesiv_T att_ctrl_switcher_ConstP;

/* Invariant block signals (default storage) */
extern const att_ctrl_switcher_TConstB att_ctrl_switcher_ConstB;

/* Block states (default storage) */
extern att_ctrl_switcher_TDW att_ctrl_switcher_DW;

/* Previous zero-crossings (trigger) states */
extern att_ctrl_switcher_TZCE att_ctrl_switcher_PrevZCX;

#endif                                 /* att_ctrl_switcher_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
