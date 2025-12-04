/*
 * File: att_ctrl_switcher_data.c
 *
 * Code generated for Simulink model 'att_ctrl_switcher'.
 *
 * Model version                  : 2.209
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:26 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "att_ctrl_switcher_private.h"

/* Constant parameters (default storage) */
const att_ctrl_switcher_d5yyrvesiv_T att_ctrl_switcher_ConstP = {
  /* Expression: [30, 20, 15, 10]*pi/180
   * Referenced by: '<S11>/1-D Lookup Table'
   */
  { 0.52359877559829882, 0.3490658503988659, 0.26179938779914941,
    0.17453292519943295 },

  /* Expression: [0, 10, 20, 30]
   * Referenced by: '<S11>/1-D Lookup Table'
   */
  { 0.0, 10.0, 20.0, 30.0 },

  /* Expression: [10, 5, 2]*pi/180
   * Referenced by: '<S9>/1-D Lookup Table'
   */
  { 0.17453292519943295, 0.087266462599716474, 0.034906585039886591 },

  /* Expression: [10, 13, 30]
   * Referenced by: '<S9>/1-D Lookup Table'
   */
  { 10.0, 13.0, 30.0 }
};

/* Invariant block signals (default storage) */
const att_ctrl_switcher_TConstB att_ctrl_switcher_ConstB = {
  -0.5,                                /* '<S25>/Gain' */
  6.2831853071795862,                  /* '<S33>/Sum1' */
  -0.52359877559829882,                /* '<S7>/Gain' */
  6.2831853071795862,                  /* '<S37>/Sum1' */
  6.2831853071795862                   /* '<S48>/Sum1' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
