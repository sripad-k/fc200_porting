/*
 * File: xswap_SY7rNTJQ.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.80
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:00 2025
 */

#include "rtwtypes.h"
#include "xswap_SY7rNTJQ.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xswap_SY7rNTJQ(real_T x[32], int32_T ix0, int32_T iy0)
{
  real_T temp;
  int32_T k;
  int32_T temp_tmp;
  int32_T tmp;
  for (k = 0; k < 8; k++) {
    temp_tmp = (ix0 + k) - 1;
    temp = x[temp_tmp];
    tmp = (iy0 + k) - 1;
    x[temp_tmp] = x[tmp];
    x[tmp] = temp;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
