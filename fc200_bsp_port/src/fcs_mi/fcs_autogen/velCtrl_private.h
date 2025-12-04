/*
 * File: velCtrl_private.h
 *
 * Code generated for Simulink model 'velCtrl'.
 *
 * Model version                  : 2.9
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:20 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef velCtrl_private_h_
#define velCtrl_private_h_
#include "rtwtypes.h"
#include "velCtrl_types.h"

/* Block states (default storage) for model 'velCtrl' */
typedef struct {
  real_T DiscreteTimeIntegrator1_DSTATE[2];/* '<S1>/Discrete-Time Integrator1' */
  int8_T DiscreteTimeIntegrator1_PrevResetState;/* '<S1>/Discrete-Time Integrator1' */
} velCtrl_TDW;

/* Invariant block signals for model 'velCtrl' */
typedef struct {
  const real_T Abs;                    /* '<S13>/Abs' */
  const real_T Switch1;                /* '<S13>/Switch1' */
  const real_T Product;                /* '<S13>/Product' */
  const real_T Switch3;                /* '<S13>/Switch3' */
} velCtrl_TConstB;

/* Invariant block signals (default storage) */
extern const velCtrl_TConstB velCtrl_ConstB;

/* Block states (default storage) */
extern velCtrl_TDW velCtrl_DW;

#endif                                 /* velCtrl_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
