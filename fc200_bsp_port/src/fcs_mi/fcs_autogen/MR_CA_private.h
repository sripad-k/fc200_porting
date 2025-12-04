/*
 * File: MR_CA_private.h
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.80
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:00 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef MR_CA_private_h_
#define MR_CA_private_h_
#include "rtwtypes.h"
#include "MR_CA_types.h"

/* Block states (default storage) for model 'MR_CA' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S13>/Discrete-Time Integrator' */
  real_T PrevY;                        /* '<S2>/Rate Limiter1' */
  real_T PrevY_gomqn2vwp1;             /* '<S2>/Rate Limiter' */
  int8_T DiscreteTimeIntegrator_PrevResetState;/* '<S13>/Discrete-Time Integrator' */
} MR_CA_TDW;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Expression: CA_B)
   * Referenced by:
   *   '<Root>/Constant'
   *   '<S5>/Constant'
   */
  real_T pooled1[32];

  /* Expression: CA_eye
   * Referenced by: '<S5>/Constant3'
   */
  real_T Constant3_Value_gqydu0pxng[64];

  /* Expression: [1;0]
   * Referenced by: '<Root>/1-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData[2];

  /* Expression: [8;10]
   * Referenced by: '<Root>/1-D Lookup Table1'
   */
  real_T uDLookupTable1_bp01Data[2];
} MR_CA_ilbysjtq22_T;

/* Constant parameters (default storage) */
extern const MR_CA_ilbysjtq22_T MR_CA_ConstP;

/* Block states (default storage) */
extern MR_CA_TDW MR_CA_DW;

#endif                                 /* MR_CA_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
