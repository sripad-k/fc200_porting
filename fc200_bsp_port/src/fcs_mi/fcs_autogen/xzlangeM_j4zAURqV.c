/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xzlangeM_j4zAURqV.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 */

#include "rtwtypes.h"
#include "xzlangeM_j4zAURqV.h"
#include <math.h>

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
real_T xzlangeM_j4zAURqV(const real_T x[32])
{
  real_T absxk;
  real_T y;
  int32_T k;
  y = 0.0;
  for (k = 0; k < 32; k++) {
    absxk = fabs(x[k]);
    if (absxk > y) {
      y = absxk;
    }
  }

  return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
