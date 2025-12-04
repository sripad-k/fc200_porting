/*
 * File: xrot_ZYyX39cx.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.80
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:00 2025
 */

#include "rtwtypes.h"
#include "xrot_ZYyX39cx.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xrot_ZYyX39cx(real_T x[32], int32_T ix0, int32_T iy0, real_T c, real_T s)
{
  real_T temp_tmp;
  real_T temp_tmp_0;
  int32_T k;
  int32_T temp_tmp_tmp;
  int32_T temp_tmp_tmp_0;
  for (k = 0; k < 8; k++) {
    temp_tmp_tmp = (iy0 + k) - 1;
    temp_tmp = x[temp_tmp_tmp];
    temp_tmp_tmp_0 = (ix0 + k) - 1;
    temp_tmp_0 = x[temp_tmp_tmp_0];
    x[temp_tmp_tmp] = (c * temp_tmp) - (s * temp_tmp_0);
    x[temp_tmp_tmp_0] = (c * temp_tmp_0) + (s * temp_tmp);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
