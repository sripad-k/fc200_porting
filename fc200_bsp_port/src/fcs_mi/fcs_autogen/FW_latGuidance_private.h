/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_latGuidance_private.h
 *
 * Code generated for Simulink model 'FW_latGuidance'.
 *
 * Model version                  : 2.79
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:22 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_latGuidance_private_h_
#define FW_latGuidance_private_h_
#include "rtwtypes.h"
#include "FW_latGuidance_types.h"

/* Invariant block signals for model 'FW_latGuidance' */
typedef struct {
  const real_T TrigonometricFunction;  /* '<S9>/Trigonometric Function' */
  const real_T Product;                /* '<S13>/Product' */
  const real_T Square;                 /* '<S13>/Square' */
  const real_T ue2sin2L;               /* '<S13>/Add' */
  const real_T Sqrt;                   /* '<S13>/Sqrt' */
  const real_T Abs;                    /* '<S14>/Abs' */
  const real_T Switch1;                /* '<S14>/Switch1' */
  const real_T Product_dr12p5kggg;     /* '<S14>/Product' */
  const real_T Switch3;                /* '<S14>/Switch3' */
  const real_T R_EL;                   /* '<S13>/Divide' */
  const real_T Sum;                    /* '<S9>/Sum' */
  const real_T TrigonometricFunction1; /* '<S9>/Trigonometric Function1' */
  const real_T TrigonometricFunction3; /* '<S9>/Trigonometric Function3' */
  const real_T Product_ppym1auoep;     /* '<S9>/Product' */
  const real_T TrigonometricFunction2; /* '<S9>/Trigonometric Function2' */
  const real_T Product1;               /* '<S9>/Product1' */
  const real_T Product2;               /* '<S9>/Product2' */
  const real_T Sum1;                   /* '<S9>/Sum1' */
  const real_T Product3;               /* '<S9>/Product3' */
  const real_T VectorConcatenate[3];   /* '<S9>/Vector Concatenate' */
} FW_latGuidance_TConstB;

/* Invariant block signals (default storage) */
extern const FW_latGuidance_TConstB FW_latGuidance_ConstB;

#endif                                 /* FW_latGuidance_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
