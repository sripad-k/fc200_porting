/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_TECS_switcher_private.h
 *
 * Code generated for Simulink model 'FW_TECS_switcher'.
 *
 * Model version                  : 2.176
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:50:56 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_TECS_switcher_private_h_
#define FW_TECS_switcher_private_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "FW_TECS_switcher_types.h"

/* Block states (default storage) for model 'FW_TECS_switcher' */
typedef struct {
  real_T cur_pos_sp_DSTATE[3];         /* '<S106>/cur_pos_sp' */
  real_T prev_pos_sp_DSTATE[3];        /* '<S106>/prev_pos_sp' */
  real_T Delay1_DSTATE[3];             /* '<S8>/Delay1' */
  real_T Delay2_DSTATE[3];             /* '<S8>/Delay2' */
  real_T Delay1_DSTATE_n1as2selwq;     /* '<S23>/Delay1' */
  real_T Delay_DSTATE;                 /* '<S22>/Delay' */
  real_T Delay1_DSTATE_g1oevxsf0d;     /* '<S90>/Delay1' */
  real_T UnitDelay1_DSTATE;            /* '<S8>/Unit Delay1' */
  real_T UnitDelay2_DSTATE;            /* '<S8>/Unit Delay2' */
  real_T Memory1_PreviousInput;        /* '<S8>/Memory1' */
  eFWRTH_phase Delay_DSTATE_iwjnjngoev[2];/* '<S5>/Delay' */
  eFWRTH_phase Delay1_DSTATE_niw4uw2lp0[2];/* '<S5>/Delay1' */
  eFWRTH_phase Delay2_DSTATE_ctj124hvji[2];/* '<S5>/Delay2' */
  eFWRTH_phase Memory4_PreviousInput;  /* '<S5>/Memory4' */
  uint16_T DelayInput1_DSTATE;         /* '<S112>/Delay Input1' */
  uint16_T cur_idx_DSTATE;             /* '<S106>/cur_idx' */
  uint16_T Memory_PreviousInput;       /* '<S106>/Memory' */
  boolean_T DelayInput1_DSTATE_hck53lu3v0;/* '<S113>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_eu53xg3kmp;/* '<S114>/Delay Input1' */
  boolean_T Delay_DSTATE_perkgn1qf5[2];/* '<S8>/Delay' */
  boolean_T Delay3_DSTATE[2];          /* '<S8>/Delay3' */
  boolean_T DelayInput1_DSTATE_ezpkpkh21k;/* '<S104>/Delay Input1' */
  boolean_T Memory2_PreviousInput;     /* '<S106>/Memory2' */
  boolean_T icLoad;                    /* '<S106>/cur_pos_sp' */
  boolean_T Memory_PreviousInput_frepmgenr3;/* '<S8>/Memory' */
  boolean_T icLoad_jveizoqakw;         /* '<S106>/prev_pos_sp' */
  boolean_T icLoad_ncnilzog2z;         /* '<S8>/Delay1' */
  boolean_T icLoad_fj3gi4q45w;         /* '<S8>/Delay2' */
  boolean_T icLoad_ogna325jjd;         /* '<S23>/Delay1' */
  boolean_T icLoad_cnbza4ewej;         /* '<S22>/Delay' */
  boolean_T icLoad_kbyaompepr;         /* '<S90>/Delay1' */
  boolean_T icLoad_hfgm0d2zgp;         /* '<S106>/cur_idx' */
} FW_TECS_switcher_TDW;

/* Zero-crossing (trigger) state for model 'FW_TECS_switcher' */
typedef struct {
  ZCSigState cur_pos_sp_Reset_ZCE;     /* '<S106>/cur_pos_sp' */
  ZCSigState prev_pos_sp_Reset_ZCE;    /* '<S106>/prev_pos_sp' */
  ZCSigState Delay1_Reset_ZCE;         /* '<S8>/Delay1' */
  ZCSigState Delay2_Reset_ZCE;         /* '<S8>/Delay2' */
  ZCSigState Delay1_Reset_ZCE_pzkzowkb1n;/* '<S23>/Delay1' */
  ZCSigState Delay1_Reset_ZCE_okp35hdmnj;/* '<S90>/Delay1' */
  ZCSigState cur_idx_Reset_ZCE;        /* '<S106>/cur_idx' */
} FW_TECS_switcher_TZCE;

/* Invariant block signals for model 'FW_TECS_switcher' */
typedef struct {
  const busFW_TECS_switcher BusAssignment2;/* '<S1>/Bus Assignment2' */
  const real_T Sum1;                   /* '<S25>/Sum1' */
  const real_T Sum1_ifh4nmipzo;        /* '<S69>/Sum1' */
  const real_T Sum1_dex1bd4twp;        /* '<S70>/Sum1' */
  const real_T Sum1_obqa34fcll;        /* '<S74>/Sum1' */
  const real_T Atan2;                  /* '<S44>/Atan2' */
  const real_T TmpSignalConversionAtSquareInport1[2];
  const real_T Square[2];              /* '<S44>/Square' */
  const real_T SumofElements;          /* '<S44>/Sum of Elements' */
  const real_T Sqrt;                   /* '<S44>/Sqrt' */
  const real_T Divide;                 /* '<S75>/Divide' */
  const real_T TrigonometricFunction1; /* '<S75>/Trigonometric Function1' */
  const real_T TrigonometricFunction2; /* '<S75>/Trigonometric Function2' */
  const real_T Product;                /* '<S75>/Product' */
  const real_T TrigonometricFunction3; /* '<S75>/Trigonometric Function3' */
  const real_T TrigonometricFunction4; /* '<S75>/Trigonometric Function4' */
  const real_T Sum1_j0ejptrk3o;        /* '<S76>/Sum1' */
  const real_T Sum1_o5uamo4z5t;        /* '<S77>/Sum1' */
  const real_T TrigonometricFunction;  /* '<S95>/Trigonometric Function' */
  const real_T Product_ixlmgikysu;     /* '<S99>/Product' */
  const real_T Square_hrgnbp0wcl;      /* '<S99>/Square' */
  const real_T ue2sin2L;               /* '<S99>/Add' */
  const real_T Sqrt_kipzpqoffo;        /* '<S99>/Sqrt' */
  const real_T Abs;                    /* '<S100>/Abs' */
  const real_T Switch1;                /* '<S100>/Switch1' */
  const real_T Product_jugfhni0ik;     /* '<S100>/Product' */
  const real_T Switch3;                /* '<S100>/Switch3' */
  const real_T R_EL;                   /* '<S99>/Divide' */
  const real_T Sum;                    /* '<S95>/Sum' */
  const real_T TrigonometricFunction1_cpwo5qs0jf;/* '<S95>/Trigonometric Function1' */
  const real_T TrigonometricFunction3_g5flhwz4ih;/* '<S95>/Trigonometric Function3' */
  const real_T Product_pjio240id2;     /* '<S95>/Product' */
  const real_T TrigonometricFunction2_m40yky3bct;/* '<S95>/Trigonometric Function2' */
  const real_T Product1;               /* '<S95>/Product1' */
  const real_T Product2;               /* '<S95>/Product2' */
  const real_T Sum1_jiwnekazg5;        /* '<S95>/Sum1' */
  const real_T Product3;               /* '<S95>/Product3' */
  const real_T VectorConcatenate[3];   /* '<S95>/Vector Concatenate' */
  const real_T Sum1_a4lnrlft1l;        /* '<S111>/Sum1' */
} FW_TECS_switcher_TConstB;

extern void FW_TECS_switcher_illvnqr3hx(const real_T rtu_LLA[3], real_T rty_C[9]);

/* Invariant block signals (default storage) */
extern const FW_TECS_switcher_TConstB FW_TECS_switcher_ConstB;

/* Block states (default storage) */
extern FW_TECS_switcher_TDW FW_TECS_switcher_DW;

/* Previous zero-crossings (trigger) states */
extern FW_TECS_switcher_TZCE FW_TECS_switcher_PrevZCX;

#endif                                 /* FW_TECS_switcher_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
