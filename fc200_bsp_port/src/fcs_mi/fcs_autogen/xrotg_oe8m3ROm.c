/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xrotg_oe8m3ROm.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 */

#include "rtwtypes.h"
#include "xrotg_oe8m3ROm.h"
#include <math.h>

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xrotg_oe8m3ROm(real_T *a, real_T *b, real_T *c, real_T *s)
{
  real_T absa;
  real_T absb;
  real_T ads;
  real_T bds;
  real_T roe;
  real_T scale;
  roe = *b;
  absa = fabs(*a);
  absb = fabs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    *a = 0.0;
    *b = 0.0;
  } else {
    ads = absa / scale;
    bds = absb / scale;
    scale *= sqrt((ads * ads) + (bds * bds));
    if (roe < 0.0) {
      scale = -scale;
    }

    *c = (*a) / scale;
    *s = (*b) / scale;
    if (absa > absb) {
      *b = *s;
    } else if ((*c) != 0.0) {
      *b = 1.0 / (*c);
    } else {
      *b = 1.0;
    }

    *a = scale;
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
