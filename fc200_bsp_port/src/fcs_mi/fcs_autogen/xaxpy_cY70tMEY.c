/*
 * File: xaxpy_cY70tMEY.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.80
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:00 2025
 */

#include "rtwtypes.h"
#include "xaxpy_cY70tMEY.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xaxpy_cY70tMEY(int32_T n, real_T a, int32_T ix0, real_T y[16], int32_T iy0)
{
  int32_T k;
  int32_T tmp;
  if (a != 0.0) {
    for (k = 0; k < n; k++) {
      tmp = (iy0 + k) - 1;
      y[tmp] += a * y[(ix0 + k) - 1];
    }
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
