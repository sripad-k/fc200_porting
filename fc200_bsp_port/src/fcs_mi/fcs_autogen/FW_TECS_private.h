/*
 * File: FW_TECS_private.h
 *
 * Code generated for Simulink model 'FW_TECS'.
 *
 * Model version                  : 2.61
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:04:29 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_TECS_private_h_
#define FW_TECS_private_h_
#include "rtwtypes.h"
#include "FW_TECS_types.h"

/* Block states (default storage) for model 'FW_TECS' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S2>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator1_DSTATE;/* '<S2>/Discrete-Time Integrator1' */
  real_T PrevY;                        /* '<S3>/Rate Limiter3' */
  int8_T DiscreteTimeIntegrator_PrevResetState;/* '<S2>/Discrete-Time Integrator' */
  int8_T DiscreteTimeIntegrator1_PrevResetState;/* '<S2>/Discrete-Time Integrator1' */
  uint8_T DiscreteTimeIntegrator_IC_LOADING;/* '<S2>/Discrete-Time Integrator' */
  uint8_T DiscreteTimeIntegrator1_IC_LOADING;/* '<S2>/Discrete-Time Integrator1' */
} FW_TECS_TDW;

/* Invariant block signals for model 'FW_TECS' */
typedef struct {
  const real_T Abs;                    /* '<S8>/Abs' */
  const real_T Switch1;                /* '<S8>/Switch1' */
  const real_T Product;                /* '<S8>/Product' */
  const real_T Switch3;                /* '<S8>/Switch3' */
  const real_T Gain1;                  /* '<S3>/Gain1' */
} FW_TECS_TConstB;

/* Invariant block signals (default storage) */
extern const FW_TECS_TConstB FW_TECS_ConstB;

/* Block states (default storage) */
extern FW_TECS_TDW FW_TECS_DW;

#endif                                 /* FW_TECS_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
