/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: altCtrl_private.h
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

#ifndef altCtrl_private_h_
#define altCtrl_private_h_
#include "rtwtypes.h"
#include "altCtrl_types.h"

/* Block states (default storage) for model 'altCtrl' */
typedef struct {
  real_T omega_DSTATE;                 /* '<S5>/omega' */
  real_T DiscreteTimeIntegrator1_DSTATE;/* '<S1>/Discrete-Time Integrator1' */
  real_T PrevY;                        /* '<Root>/Rate Limiter3' */
  int8_T DiscreteTimeIntegrator1_PrevResetState;/* '<S1>/Discrete-Time Integrator1' */
} altCtrl_TDW;

/* Invariant block signals for model 'altCtrl' */
typedef struct {
  const real_T Constant;               /* '<Root>/Constant' */
  const real_T Gain1;                  /* '<S2>/Gain1' */
} altCtrl_TConstB;

/* Invariant block signals (default storage) */
extern const altCtrl_TConstB altCtrl_ConstB;

/* Block states (default storage) */
extern altCtrl_TDW altCtrl_DW;

#endif                                 /* altCtrl_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
