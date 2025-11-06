/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xswap_6UsFoJMz.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 */

#include "rtwtypes.h"
#include "xswap_6UsFoJMz.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xswap_6UsFoJMz(real_T x[16], int32_T ix0, int32_T iy0)
{
  real_T temp;
  temp = x[ix0 - 1];
  x[ix0 - 1] = x[iy0 - 1];
  x[iy0 - 1] = temp;
  temp = x[ix0];
  x[ix0] = x[iy0];
  x[iy0] = temp;
  temp = x[ix0 + 1];
  x[ix0 + 1] = x[iy0 + 1];
  x[iy0 + 1] = temp;
  temp = x[ix0 + 2];
  x[ix0 + 2] = x[iy0 + 2];
  x[iy0 + 2] = temp;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
