/*
 * File: FW_attCtrl_switcher_private.h
 *
 * Code generated for Simulink model 'FW_attCtrl_switcher'.
 *
 * Model version                  : 2.60
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:23 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_attCtrl_switcher_private_h_
#define FW_attCtrl_switcher_private_h_
#include "rtwtypes.h"
#include "FW_attCtrl_switcher_types.h"

/* Block states (default storage) for model 'FW_attCtrl_switcher' */
typedef struct {
  real_T Delay_DSTATE;                 /* '<S14>/Delay' */
  boolean_T UnitDelay_DSTATE;          /* '<S4>/Unit Delay' */
  boolean_T icLoad;                    /* '<S14>/Delay' */
} FW_attCtrl_switcher_TDW;

/* Invariant block signals for model 'FW_attCtrl_switcher' */
typedef struct {
  const real_T Saturation;             /* '<S1>/Saturation' */
} FW_attCtrl_switcher_TConstB;

/* Invariant block signals (default storage) */
extern const FW_attCtrl_switcher_TConstB FW_attCtrl_switcher_ConstB;

/* Block states (default storage) */
extern FW_attCtrl_switcher_TDW FW_attCtrl_switcher_DW;

#endif                                 /* FW_attCtrl_switcher_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
