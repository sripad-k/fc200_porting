/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_throttle_private.h
 *
 * Code generated for Simulink model 'FW_throttle'.
 *
 * Model version                  : 2.3
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:27 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_throttle_private_h_
#define FW_throttle_private_h_
#include "rtwtypes.h"
#include "FW_throttle_types.h"

/* Block states (default storage) for model 'FW_throttle' */
typedef struct {
  real_T PrevY;                        /* '<Root>/100%_per_sec' */
} FW_throttle_TDW;

/* Block states (default storage) */
extern FW_throttle_TDW FW_throttle_DW;

#endif                                 /* FW_throttle_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
