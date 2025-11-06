/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: svd_o1pFIz8b.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 */

#include "rtwtypes.h"
#include "svd_o1pFIz8b.h"
#include <string.h>
#include "xzlangeM_j4zAURqV.h"
#include "xzlascl_Alz1u9wU.h"
#include "xnrm2_rtnUeag4.h"
#include "xdotc_kedD75jU.h"
#include "xaxpy_TfmOorhp.h"
#include <math.h>
#include "xdotc_1FBr1P4Y.h"
#include "xaxpy_cY70tMEY.h"
#include "xnrm2_pMnkSmXQ.h"
#include "xaxpy_C2TeRJI8.h"
#include "xaxpy_duCrqEt2.h"
#include "xzlascl_IFZzvmrN.h"
#include "xrotg_oe8m3ROm.h"
#include "xrot_XMvwf5hX.h"
#include "xrot_ZYyX39cx.h"
#include "xswap_6UsFoJMz.h"
#include "xswap_SY7rNTJQ.h"

/* Function for MATLAB Function: '<S5>/MATLAB Function' */
void svd_o1pFIz8b(const real_T A[32], real_T U[32], real_T s[4], real_T V[16])
{
  real_T b_A[32];
  real_T Vf[16];
  real_T work[8];
  real_T b_s[4];
  real_T e[4];
  real_T anrm;
  real_T cscale;
  real_T emm1;
  real_T nrm;
  real_T rt;
  real_T shift;
  real_T smm1;
  real_T sqds;
  real_T ztest;
  int32_T exitg1;
  int32_T i;
  int32_T qjj;
  int32_T qp1;
  int32_T qp1jj;
  int32_T qq;
  int32_T qq_tmp;
  boolean_T apply_transform;
  boolean_T doscale;
  boolean_T exitg2;
  (void)memcpy(&b_A[0], &A[0], (sizeof(real_T)) << 5U);
  b_s[0] = 0.0;
  e[0] = 0.0;
  b_s[1] = 0.0;
  e[1] = 0.0;
  b_s[2] = 0.0;
  e[2] = 0.0;
  b_s[3] = 0.0;
  e[3] = 0.0;
  (void)memset(&work[0], 0, (sizeof(real_T)) << 3U);
  (void)memset(&U[0], 0, (sizeof(real_T)) << 5U);
  (void)memset(&Vf[0], 0, (sizeof(real_T)) << 4U);
  doscale = false;
  anrm = xzlangeM_j4zAURqV(A);
  cscale = anrm;
  if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
    doscale = true;
    cscale = 6.7178761075670888E-139;
    xzlascl_Alz1u9wU(anrm, cscale, b_A);
  } else if (anrm > 1.4885657073574029E+138) {
    doscale = true;
    cscale = 1.4885657073574029E+138;
    xzlascl_Alz1u9wU(anrm, cscale, b_A);
  } else {
    /* no actions */
  }

  for (i = 0; i < 4; i++) {
    qp1 = i + 2;
    qq_tmp = i + (8 * i);
    qq = qq_tmp + 1;
    apply_transform = false;
    nrm = xnrm2_rtnUeag4(8 - i, b_A, qq_tmp + 1);
    if (nrm > 0.0) {
      apply_transform = true;
      if (b_A[qq_tmp] < 0.0) {
        nrm = -nrm;
      }

      b_s[i] = nrm;
      if (fabs(nrm) >= 1.0020841800044864E-292) {
        nrm = 1.0 / nrm;
        qjj = (qq_tmp - i) + 8;
        for (qp1jj = qq; qp1jj <= qjj; qp1jj++) {
          b_A[qp1jj - 1] *= nrm;
        }
      } else {
        qjj = (qq_tmp - i) + 8;
        for (qp1jj = qq; qp1jj <= qjj; qp1jj++) {
          b_A[qp1jj - 1] /= b_s[i];
        }
      }

      b_A[qq_tmp]++;
      b_s[i] = -b_s[i];
    } else {
      b_s[i] = 0.0;
    }

    for (qp1jj = qp1; qp1jj < 5; qp1jj++) {
      qjj = i + (8 * (qp1jj - 1));
      if (apply_transform) {
        xaxpy_TfmOorhp(8 - i, -(xdotc_kedD75jU(8 - i, b_A, qq_tmp + 1, b_A, qjj
          + 1) / b_A[qq_tmp]), qq_tmp + 1, b_A, qjj + 1);
      }

      e[qp1jj - 1] = b_A[qjj];
    }

    for (qq = i + 1; qq < 9; qq++) {
      qp1jj = (qq + (8 * i)) - 1;
      U[qp1jj] = b_A[qp1jj];
    }

    if ((i + 1) <= 2) {
      nrm = xnrm2_pMnkSmXQ(3 - i, e, i + 2);
      if (nrm == 0.0) {
        e[i] = 0.0;
      } else {
        if (e[i + 1] < 0.0) {
          e[i] = -nrm;
        } else {
          e[i] = nrm;
        }

        nrm = e[i];
        if (fabs(e[i]) >= 1.0020841800044864E-292) {
          nrm = 1.0 / e[i];
          for (qjj = qp1; qjj < 5; qjj++) {
            e[qjj - 1] *= nrm;
          }
        } else {
          for (qjj = qp1; qjj < 5; qjj++) {
            e[qjj - 1] /= nrm;
          }
        }

        e[i + 1]++;
        e[i] = -e[i];
        for (qq = qp1; qq < 9; qq++) {
          work[qq - 1] = 0.0;
        }

        for (qq = qp1; qq < 5; qq++) {
          xaxpy_C2TeRJI8(7 - i, e[qq - 1], b_A, (i + (8 * (qq - 1))) + 2, work,
                         i + 2);
        }

        for (qq = qp1; qq < 5; qq++) {
          xaxpy_duCrqEt2(7 - i, (-e[qq - 1]) / e[i + 1], work, i + 2, b_A, (i +
            (8 * (qq - 1))) + 2);
        }
      }

      for (qq = qp1; qq < 5; qq++) {
        Vf[(qq + (4 * i)) - 1] = e[qq - 1];
      }
    }
  }

  i = 2;
  e[2] = b_A[26];
  e[3] = 0.0;
  for (qp1 = 3; qp1 >= 0; qp1--) {
    qq = qp1 + (8 * qp1);
    if (b_s[qp1] != 0.0) {
      for (qp1jj = qp1 + 2; qp1jj < 5; qp1jj++) {
        qjj = (qp1 + (8 * (qp1jj - 1))) + 1;
        xaxpy_TfmOorhp(8 - qp1, -(xdotc_kedD75jU(8 - qp1, U, qq + 1, U, qjj) /
          U[qq]), qq + 1, U, qjj);
      }

      for (qjj = qp1 + 1; qjj < 9; qjj++) {
        qp1jj = (qjj + (8 * qp1)) - 1;
        U[qp1jj] = -U[qp1jj];
      }

      U[qq]++;
      for (qq = 0; qq < qp1; qq++) {
        U[qq + (8 * qp1)] = 0.0;
      }
    } else {
      (void)memset(&U[qp1 * 8], 0, (sizeof(real_T)) << 3U);
      U[qq] = 1.0;
    }
  }

  for (qp1 = 3; qp1 >= 0; qp1--) {
    if (((qp1 + 1) <= 2) && (e[qp1] != 0.0)) {
      qq = (qp1 + (4 * qp1)) + 2;
      for (qjj = qp1 + 2; qjj < 5; qjj++) {
        qp1jj = (qp1 + (4 * (qjj - 1))) + 2;
        xaxpy_cY70tMEY(3 - qp1, -(xdotc_1FBr1P4Y(3 - qp1, Vf, qq, Vf, qp1jj) /
          Vf[qq - 1]), qq, Vf, qp1jj);
      }
    }

    Vf[4 * qp1] = 0.0;
    Vf[(4 * qp1) + 1] = 0.0;
    Vf[(4 * qp1) + 2] = 0.0;
    Vf[(4 * qp1) + 3] = 0.0;
    Vf[qp1 + (4 * qp1)] = 1.0;
  }

  for (qp1 = 0; qp1 < 4; qp1++) {
    nrm = b_s[qp1];
    if (nrm != 0.0) {
      rt = fabs(nrm);
      nrm /= rt;
      b_s[qp1] = rt;
      if ((qp1 + 1) < 4) {
        e[qp1] /= nrm;
      }

      qq = (8 * qp1) + 1;
      for (qjj = qq; qjj <= (qq + 7); qjj++) {
        U[qjj - 1] *= nrm;
      }
    }

    if ((qp1 + 1) < 4) {
      smm1 = e[qp1];
      if (smm1 != 0.0) {
        rt = fabs(smm1);
        nrm = rt / smm1;
        e[qp1] = rt;
        b_s[qp1 + 1] *= nrm;
        qq = (4 * (qp1 + 1)) + 1;
        for (qjj = qq; qjj <= (qq + 3); qjj++) {
          Vf[qjj - 1] *= nrm;
        }
      }
    }
  }

  qp1 = 0;
  nrm = 0.0;
  ztest = fabs(b_s[0]);
  rt = fabs(e[0]);
  if (ztest >= rt) {
    rt = ztest;
  }

  if (rt > 0.0) {
    nrm = rt;
  }

  ztest = fabs(b_s[1]);
  rt = fabs(e[1]);
  if (ztest >= rt) {
    rt = ztest;
  }

  if (nrm < rt) {
    nrm = rt;
  }

  ztest = fabs(b_s[2]);
  rt = fabs(e[2]);
  if (ztest >= rt) {
    rt = ztest;
  }

  if (nrm < rt) {
    nrm = rt;
  }

  ztest = fabs(b_s[3]);
  rt = fabs(e[3]);
  if (ztest >= rt) {
    rt = ztest;
  }

  if (nrm < rt) {
    nrm = rt;
  }

  while (((i + 2) > 0) && (qp1 < 75)) {
    qp1jj = i + 1;
    do {
      exitg1 = 0;
      qq = qp1jj;
      if (qp1jj == 0) {
        exitg1 = 1;
      } else {
        rt = fabs(e[qp1jj - 1]);
        if (rt <= (2.2204460492503131E-16 * (fabs(b_s[qp1jj - 1]) + fabs
              (b_s[qp1jj])))) {
          e[qp1jj - 1] = 0.0;
          exitg1 = 1;
        } else if ((rt <= 1.0020841800044864E-292) || ((qp1 > 20) && (rt <=
                     (2.2204460492503131E-16 * nrm)))) {
          e[qp1jj - 1] = 0.0;
          exitg1 = 1;
        } else {
          qp1jj--;
        }
      }
    } while (exitg1 == 0);

    if (qp1jj == (i + 1)) {
      qp1jj = 4;
    } else {
      qjj = i + 2;
      qq_tmp = i + 2;
      exitg2 = false;
      while (((exitg2 ? ((uint32_T)1U) : ((uint32_T)0U)) == false) && (qq_tmp >=
              qp1jj)) {
        qjj = qq_tmp;
        if (qq_tmp == qp1jj) {
          exitg2 = true;
        } else {
          rt = 0.0;
          if (qq_tmp < (i + 2)) {
            rt = fabs(e[qq_tmp - 1]);
          }

          if (qq_tmp > (qp1jj + 1)) {
            rt += fabs(e[qq_tmp - 2]);
          }

          ztest = fabs(b_s[qq_tmp - 1]);
          if ((ztest <= (2.2204460492503131E-16 * rt)) || (ztest <=
               1.0020841800044864E-292)) {
            b_s[qq_tmp - 1] = 0.0;
            exitg2 = true;
          } else {
            qq_tmp--;
          }
        }
      }

      if (qjj == qp1jj) {
        qp1jj = 3;
      } else if (qjj == (i + 2)) {
        qp1jj = 1;
      } else {
        qp1jj = 2;
        qq = qjj;
      }
    }

    switch (qp1jj) {
     case 1:
      rt = e[i];
      e[i] = 0.0;
      for (qjj = i + 1; qjj >= (qq + 1); qjj--) {
        xrotg_oe8m3ROm(&b_s[qjj - 1], &rt, &ztest, &sqds);
        if (qjj > (qq + 1)) {
          smm1 = e[qjj - 2];
          rt = (-sqds) * smm1;
          e[qjj - 2] = smm1 * ztest;
        }

        xrot_XMvwf5hX(Vf, (4 * (qjj - 1)) + 1, (4 * (i + 1)) + 1, ztest, sqds);
      }
      break;

     case 2:
      rt = e[qq - 1];
      e[qq - 1] = 0.0;
      for (qjj = qq + 1; qjj <= (i + 2); qjj++) {
        xrotg_oe8m3ROm(&b_s[qjj - 1], &rt, &ztest, &sqds);
        smm1 = e[qjj - 1];
        rt = (-sqds) * smm1;
        e[qjj - 1] = smm1 * ztest;
        xrot_ZYyX39cx(U, (8 * (qjj - 1)) + 1, (8 * (qq - 1)) + 1, ztest, sqds);
      }
      break;

     case 3:
      sqds = b_s[i + 1];
      ztest = fabs(sqds);
      rt = fabs(b_s[i]);
      if (ztest >= rt) {
        rt = ztest;
      }

      ztest = fabs(e[i]);
      if (rt >= ztest) {
        ztest = rt;
      }

      rt = fabs(b_s[qq]);
      if (ztest >= rt) {
        rt = ztest;
      }

      ztest = fabs(e[qq]);
      if (rt >= ztest) {
        ztest = rt;
      }

      rt = sqds / ztest;
      smm1 = b_s[i] / ztest;
      emm1 = e[i] / ztest;
      sqds = b_s[qq] / ztest;
      smm1 = (((smm1 + rt) * (smm1 - rt)) + (emm1 * emm1)) / 2.0;
      emm1 *= rt;
      emm1 *= emm1;
      if ((smm1 != 0.0) || (emm1 != 0.0)) {
        shift = sqrt((smm1 * smm1) + emm1);
        if (smm1 < 0.0) {
          shift = -shift;
        }

        shift = emm1 / (smm1 + shift);
      } else {
        shift = 0.0;
      }

      rt = ((sqds + rt) * (sqds - rt)) + shift;
      ztest = sqds * (e[qq] / ztest);
      for (qjj = qq + 1; qjj <= (i + 1); qjj++) {
        xrotg_oe8m3ROm(&rt, &ztest, &sqds, &smm1);
        if (qjj > (qq + 1)) {
          e[qjj - 2] = rt;
        }

        emm1 = e[qjj - 1];
        rt = b_s[qjj - 1];
        e[qjj - 1] = (sqds * emm1) - (smm1 * rt);
        ztest = smm1 * b_s[qjj];
        b_s[qjj] *= sqds;
        xrot_XMvwf5hX(Vf, (4 * (qjj - 1)) + 1, (4 * qjj) + 1, sqds, smm1);
        b_s[qjj - 1] = (sqds * rt) + (smm1 * emm1);
        xrotg_oe8m3ROm(&b_s[qjj - 1], &ztest, &sqds, &smm1);
        ztest = e[qjj - 1];
        rt = (sqds * ztest) + (smm1 * b_s[qjj]);
        b_s[qjj] = ((-smm1) * ztest) + (sqds * b_s[qjj]);
        ztest = smm1 * e[qjj];
        e[qjj] *= sqds;
        xrot_ZYyX39cx(U, (8 * (qjj - 1)) + 1, (8 * qjj) + 1, sqds, smm1);
      }

      e[i] = rt;
      qp1++;
      break;

     default:
      if (b_s[qq] < 0.0) {
        b_s[qq] = -b_s[qq];
        qp1 = (4 * qq) + 1;
        for (qjj = qp1; qjj <= (qp1 + 3); qjj++) {
          Vf[qjj - 1] = -Vf[qjj - 1];
        }
      }

      qp1 = qq + 1;
      while (((qq + 1) < 4) && (b_s[qq] < b_s[qp1])) {
        rt = b_s[qq];
        b_s[qq] = b_s[qp1];
        b_s[qp1] = rt;
        xswap_6UsFoJMz(Vf, (4 * qq) + 1, (4 * (qq + 1)) + 1);
        xswap_SY7rNTJQ(U, (8 * qq) + 1, (8 * (qq + 1)) + 1);
        qq = qp1;
        qp1++;
      }

      qp1 = 0;
      i--;
      break;
    }
  }

  s[0] = b_s[0];
  s[1] = b_s[1];
  s[2] = b_s[2];
  s[3] = b_s[3];
  if (doscale) {
    xzlascl_IFZzvmrN(cscale, anrm, s);
  }

  for (i = 0; i < 4; i++) {
    V[4 * i] = Vf[4 * i];
    qp1 = (4 * i) + 1;
    V[qp1] = Vf[qp1];
    qp1 = (4 * i) + 2;
    V[qp1] = Vf[qp1];
    qp1 = (4 * i) + 3;
    V[qp1] = Vf[qp1];
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
