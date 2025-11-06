/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_attCtrl_private.h
 *
 * Code generated for Simulink model 'FW_attCtrl'.
 *
 * Model version                  : 2.68
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:07 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_attCtrl_private_h_
#define FW_attCtrl_private_h_
#include "rtwtypes.h"
#include "FW_attCtrl_types.h"

/* Block states (default storage) for model 'FW_attCtrl' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S3>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_is34kceaix;/* '<S1>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_ej0a0oln2v;/* '<S4>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_ntgr3gtdbx;/* '<S2>/Discrete-Time Integrator' */
  real_T PrevY;                        /* '<Root>/Rate Limiter2' */
  real_T PrevY_ayxso3nroc;             /* '<Root>/Rate Limiter' */
  int8_T DiscreteTimeIntegrator_PrevResetState;/* '<S3>/Discrete-Time Integrator' */
  int8_T DiscreteTimeIntegrator_PrevResetState_i2m1oat1uz;/* '<S4>/Discrete-Time Integrator' */
} FW_attCtrl_TDW;

/* Invariant block signals for model 'FW_attCtrl' */
typedef struct {
  const real_T VectorConcatenate1[3];  /* '<S3>/Vector Concatenate1' */
} FW_attCtrl_TConstB;

/* Invariant block signals (default storage) */
extern const FW_attCtrl_TConstB FW_attCtrl_ConstB;

/* Block states (default storage) */
extern FW_attCtrl_TDW FW_attCtrl_DW;

#endif                                 /* FW_attCtrl_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
