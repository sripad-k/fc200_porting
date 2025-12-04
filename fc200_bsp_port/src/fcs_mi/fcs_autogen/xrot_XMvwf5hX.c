/*
 * File: xrot_XMvwf5hX.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.80
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:00 2025
 */

#include "rtwtypes.h"
#include "xrot_XMvwf5hX.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xrot_XMvwf5hX(real_T x[16], int32_T ix0, int32_T iy0, real_T c, real_T s)
{
  real_T temp;
  real_T temp_tmp;
  temp = x[iy0 - 1];
  temp_tmp = x[ix0 - 1];
  x[iy0 - 1] = (c * temp) - (s * temp_tmp);
  x[ix0 - 1] = (c * temp_tmp) + (s * temp);
  temp = (c * x[ix0]) + (s * x[iy0]);
  x[iy0] = (c * x[iy0]) - (s * x[ix0]);
  x[ix0] = temp;
  temp = x[iy0 + 1];
  temp_tmp = x[ix0 + 1];
  x[iy0 + 1] = (c * temp) - (s * temp_tmp);
  x[ix0 + 1] = (c * temp_tmp) + (s * temp);
  temp = x[iy0 + 2];
  temp_tmp = x[ix0 + 2];
  x[iy0 + 2] = (c * temp) - (s * temp_tmp);
  x[ix0 + 2] = (c * temp_tmp) + (s * temp);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
