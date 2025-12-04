/*
 * File: attCtrl_private.h
 *
 * Code generated for Simulink model 'attCtrl'.
 *
 * Model version                  : 2.68
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:19 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef attCtrl_private_h_
#define attCtrl_private_h_
#include "rtwtypes.h"
#include "attCtrl_types.h"

/* Block states (default storage) for model 'attCtrl' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE[2];/* '<S10>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_gdd4rgtej1[2];/* '<S6>/Discrete-Time Integrator' */
  real_T omega_DSTATE[2];              /* '<S9>/omega' */
  real_T DiscreteTimeIntegrator_DSTATE_ijqjwdlaw0;/* '<S2>/Discrete-Time Integrator' */
  real_T PrevY[2];                     /* '<S6>/Rate Limiter' */
  real_T PrevY_fueqrscxop;             /* '<Root>/Rate Limiter3' */
  uint32_T m_bpIndex;                  /* '<Root>/1-D Lookup Table' */
  int8_T DiscreteTimeIntegrator_PrevResetState;/* '<S6>/Discrete-Time Integrator' */
  int8_T DiscreteTimeIntegrator_PrevResetState_pabim4iwvd;/* '<S2>/Discrete-Time Integrator' */
} attCtrl_TDW;

/* Invariant block signals for model 'attCtrl' */
typedef struct {
  const real_T integralResetValue;     /* '<S6>/integralResetValue' */
  const real_T integralResetValue_hxh4svakxk;/* '<S2>/integralResetValue' */
  const real_T Gain1;                  /* '<S5>/Gain1' */
  const real_T Sum1;                   /* '<S20>/Sum1' */
} attCtrl_TConstB;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [0,1]
   * Referenced by: '<Root>/1-D Lookup Table'
   */
  real_T uDLookupTable_tableData[2];

  /* Expression: transition_lambda_bp
   * Referenced by: '<Root>/1-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data[2];

  /* Expression: inertial_J
   * Referenced by: '<Root>/Multiply'
   */
  real_T Multiply_Gain[9];
} attCtrl_mlvuba4hqv_T;

/* Constant parameters (default storage) */
extern const attCtrl_mlvuba4hqv_T attCtrl_ConstP;

/* Invariant block signals (default storage) */
extern const attCtrl_TConstB attCtrl_ConstB;

/* Block states (default storage) */
extern attCtrl_TDW attCtrl_DW;

#endif                                 /* attCtrl_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
