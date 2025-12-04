/*
 * File: FW_CA_private.h
 *
 * Code generated for Simulink model 'FW_CA'.
 *
 * Model version                  : 2.108
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:04:17 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_CA_private_h_
#define FW_CA_private_h_
#include "rtwtypes.h"
#include "FW_CA_types.h"

/* Block states (default storage) for model 'FW_CA' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S33>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_cnt2bxweu5;/* '<S34>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_hiklcoov13;/* '<S32>/Discrete-Time Integrator' */
  real_T PrevY;                        /* '<S10>/20%_per_sec' */
  real_T PrevY_hbjyry413j;             /* '<S10>/Rate Limiter3' */
  real_T PrevY_djckibarev;             /* '<S10>/Rate Limiter4' */
  real_T PrevY_cf2xsvidl4;             /* '<S10>/Rate Limiter1' */
  uint32_T m_bpIndex;                  /* '<Root>/1-D Lookup Table1' */
} FW_CA_TDW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: FW_ail_B
   * Referenced by: '<S2>/Constant2'
   */
  real_T Constant2_Value_kmf41b2fgv[2];

  /* Expression: FW_ail_B_pinv
   * Referenced by: '<S2>/Constant3'
   */
  real_T Constant3_Value_n5qw4ehq0c[2];

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S3>/Constant2'
   *   '<S5>/Constant2'
   */
  real_T pooled4[2];

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S3>/Constant3'
   *   '<S5>/Constant3'
   */
  real_T pooled5[2];

  /* Expression: [0 1]
   * Referenced by: '<Root>/1-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData[2];

  /* Expression: [6 10]
   * Referenced by: '<Root>/1-D Lookup Table1'
   */
  real_T uDLookupTable1_bp01Data[2];
} FW_CA_koai545rzw_T;

extern void FW_CA_i10nsmykqz(real_T rtu_nu, const real_T rtu_B[2], const real_T
  rtu_B_inv[2], const real_T rtu_lb[2], const real_T rtu_ub[2], real_T
  rty_u_out[2], boolean_T *rty_sat_flag);

/* Constant parameters (default storage) */
extern const FW_CA_koai545rzw_T FW_CA_ConstP;

/* Block states (default storage) */
extern FW_CA_TDW FW_CA_DW;

#endif                                 /* FW_CA_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
