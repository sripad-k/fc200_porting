/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xnrm2_rtnUeag4.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 */

#include "rtwtypes.h"
#include "xnrm2_rtnUeag4.h"
#include <math.h>

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
real_T xnrm2_rtnUeag4(int32_T n, const real_T x[32], int32_T ix0)
{
  real_T absxk;
  real_T scale;
  real_T t;
  real_T y;
  int32_T k;
  int32_T kend;
  y = 0.0;
  scale = 3.3121686421112381E-170;
  kend = ix0 + n;
  for (k = ix0; k < kend; k++) {
    absxk = fabs(x[k - 1]);
    if (absxk > scale) {
      t = scale / absxk;
      y = ((y * t) * t) + 1.0;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
