/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: posCtrl_private.h
 *
 * Code generated for Simulink model 'posCtrl'.
 *
 * Model version                  : 2.50
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:23 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef posCtrl_private_h_
#define posCtrl_private_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "posCtrl_types.h"

/* Block states (default storage) for model 'posCtrl' */
typedef struct {
  real_T Delay_DSTATE[3];              /* '<Root>/Delay' */
  vom_t Delay1_DSTATE;                 /* '<Root>/Delay1' */
  boolean_T icLoad;                    /* '<Root>/Delay1' */
  boolean_T icLoad_gwql4htcwe;         /* '<Root>/Delay' */
} posCtrl_TDW;

/* Zero-crossing (trigger) state for model 'posCtrl' */
typedef struct {
  ZCSigState Delay_Reset_ZCE;          /* '<Root>/Delay' */
} posCtrl_TZCE;

extern void posCtrl_i04w0oyqe2(const real_T rtu_LLA[3], real_T rty_C[9]);

/* Block states (default storage) */
extern posCtrl_TDW posCtrl_DW;

/* Previous zero-crossings (trigger) states */
extern posCtrl_TZCE posCtrl_PrevZCX;

#endif                                 /* posCtrl_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
