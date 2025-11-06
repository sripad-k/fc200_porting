/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xzlascl_Alz1u9wU.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 */

#include "rtwtypes.h"
#include "xzlascl_Alz1u9wU.h"
#include <math.h>

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void xzlascl_Alz1u9wU(real_T cfrom, real_T cto, real_T A[32])
{
  real_T cfrom1;
  real_T cfromc;
  real_T cto1;
  real_T ctoc;
  real_T mul;
  int32_T i;
  boolean_T notdone;
  cfromc = cfrom;
  ctoc = cto;
  notdone = true;
  while (notdone) {
    cfrom1 = cfromc * 2.0041683600089728E-292;
    cto1 = ctoc / 4.9896007738368E+291;
    if ((fabs(cfrom1) > fabs(ctoc)) && (ctoc != 0.0)) {
      mul = 2.0041683600089728E-292;
      cfromc = cfrom1;
    } else if (fabs(cto1) > fabs(cfromc)) {
      mul = 4.9896007738368E+291;
      ctoc = cto1;
    } else {
      mul = ctoc / cfromc;
      notdone = false;
    }

    for (i = 0; i < 32; i++) {
      A[i] *= mul;
    }
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
