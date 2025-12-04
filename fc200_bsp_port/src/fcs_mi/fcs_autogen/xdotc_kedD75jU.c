/*
 * File: xdotc_kedD75jU.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.80
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:00 2025
 */

#include "rtwtypes.h"
#include "xdotc_kedD75jU.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
real_T xdotc_kedD75jU(int32_T n, const real_T x[32], int32_T ix0, const real_T
                      y[32], int32_T iy0)
{
  real_T d;
  int32_T b;
  int32_T k;
  d = 0.0;
  b = (int32_T)((uint8_T)n);
  for (k = 0; k < b; k++) {
    d += x[(ix0 + k) - 1] * y[(iy0 + k) - 1];
  }

  return d;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
