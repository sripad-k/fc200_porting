/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: MR_CA.c
 *
 * Code generated for Simulink model 'MR_CA'.
 *
 * Model version                  : 2.72
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:44 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "MR_CA.h"
#include "rtwtypes.h"
#include "MR_CA_types.h"
#include "MR_CA_private.h"
#include <string.h>
#include "svd_o1pFIz8b.h"
#include <math.h>
#include "look1_binlc.h"

/* Block states (default storage) */
MR_CA_TDW MR_CA_DW;

/* System initialize for referenced model: 'MR_CA' */
void MR_CA_Init(void)
{
  /* InitializeConditions for RateLimiter: '<S2>/Rate Limiter' */
  MR_CA_DW.PrevY = 0.1;

  /* InitializeConditions for DiscreteIntegrator: '<S12>/Discrete-Time Integrator' */
  MR_CA_DW.DiscreteTimeIntegrator_PrevResetState = 0;
}

/* Output and update for referenced model: 'MR_CA' */
void MR_CA(const real_T *rtu_busControllerAltCtrl_forceDes, const real_T
           rtu_busControllerAttCtrl_momentDes[3], const vom_t *rtu_vom_status,
           const boolean_T *rtu_sFlags_rampup_phase, const real_T
           *rtu_Sensor_aspd_cas, const lifter_state_t *rtu_lifter_state, const
           lifter_state_t *rtu_FW_LifterMode_eFWLifter_Mode, busControllerCA
           *rty_controllerCA)
{
  std_ctrl_t rtb_BusAssignment2;
  real_T BtW_0[64];
  real_T M[64];
  real_T x[64];
  real_T BtW[32];
  real_T U[32];
  real_T tmp[32];
  real_T V[16];
  real_T M_0[8];
  real_T c1[8];
  real_T c1_tmp[8];
  real_T rtb_Product[8];
  real_T rtb_F_err[4];
  real_T rtb_Saturation2[4];
  real_T rtb_VectorConcatenate_fo5jsv5trp[4];
  real_T s[4];
  real_T absxk;
  real_T rtb_Product1;
  real_T rtb_Product_0;
  real_T rtb_Saturation2_0;
  real_T rtb_VectorConcatenate_idx_0;
  real_T rtb_VectorConcatenate_idx_3;
  real_T scale;
  real_T t;
  int32_T ar;
  int32_T b_k;
  int32_T br;
  int32_T d;
  int32_T i;
  int32_T r;
  int32_T vcol;
  boolean_T exitg1;
  boolean_T rtb_Compare_cxljnxpqat;
  boolean_T rtb_Compare_pdi014sibj;
  boolean_T rtb_OR2;

  /* Lookup_n-D: '<Root>/1-D Lookup Table1' */
  rtb_Product1 = look1_binlc(*rtu_Sensor_aspd_cas,
    MR_CA_ConstP.uDLookupTable1_bp01Data, MR_CA_ConstP.uDLookupTable1_tableData,
    1U);

  /* SignalConversion generated from: '<Root>/Bus Assignment2' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment2'
   */
  (void)memset(&rtb_BusAssignment2, 0, sizeof(std_ctrl_t));

  /* SignalConversion generated from: '<Root>/Vector Concatenate' */
  rtb_VectorConcatenate_fo5jsv5trp[0] = *rtu_busControllerAltCtrl_forceDes;

  /* SignalConversion generated from: '<Root>/Vector Concatenate' */
  rtb_VectorConcatenate_fo5jsv5trp[1] = rtu_busControllerAttCtrl_momentDes[0];
  rtb_VectorConcatenate_fo5jsv5trp[2] = rtu_busControllerAttCtrl_momentDes[1];
  rtb_VectorConcatenate_fo5jsv5trp[3] = rtu_busControllerAttCtrl_momentDes[2];

  /* Saturate: '<Root>/Saturation2' incorporates:
   *  SignalConversion generated from: '<Root>/Vector Concatenate'
   */
  if ((*rtu_busControllerAltCtrl_forceDes) > 1245.24) {
    rtb_Saturation2[0] = 1245.24;
  } else if ((*rtu_busControllerAltCtrl_forceDes) < 0.0) {
    rtb_Saturation2[0] = 0.0;
  } else {
    rtb_Saturation2[0] = *rtu_busControllerAltCtrl_forceDes;
  }

  if (rtu_busControllerAttCtrl_momentDes[0] > 734.69) {
    scale = 734.69;
    rtb_Saturation2[1] = 734.69;
  } else if (rtu_busControllerAttCtrl_momentDes[0] < -734.69) {
    scale = -734.69;
    rtb_Saturation2[1] = -734.69;
  } else {
    scale = rtu_busControllerAttCtrl_momentDes[0];
    rtb_Saturation2[1] = rtu_busControllerAttCtrl_momentDes[0];
  }

  if (rtu_busControllerAttCtrl_momentDes[1] > 484.58) {
    rtb_Saturation2[2] = 484.58;
  } else if (rtu_busControllerAttCtrl_momentDes[1] < -484.56) {
    rtb_Saturation2[2] = -484.56;
  } else {
    rtb_Saturation2[2] = rtu_busControllerAttCtrl_momentDes[1];
  }

  if (rtu_busControllerAttCtrl_momentDes[2] > 63.773) {
    absxk = 63.773;
    rtb_Saturation2[3] = 63.773;
  } else if (rtu_busControllerAttCtrl_momentDes[2] < -63.773) {
    absxk = -63.773;
    rtb_Saturation2[3] = -63.773;
  } else {
    absxk = rtu_busControllerAttCtrl_momentDes[2];
    rtb_Saturation2[3] = rtu_busControllerAttCtrl_momentDes[2];
  }

  /* Product: '<S5>/Product' */
  for (i = 0; i < 8; i++) {
    rtb_Product[i] = 0.0641 * rtb_Product1;
  }

  /* End of Product: '<S5>/Product' */

  /* Sum: '<S5>/Sum1' incorporates:
   *  Constant: '<S5>/One1'
   *  Constant: '<S5>/One2'
   *  Product: '<S5>/Product2'
   *  Product: '<S5>/Product3'
   *  Sum: '<S5>/Sum'
   */
  rtb_Product1 += (1.0 - rtb_Product1) * 0.01;

  /* SignalConversion generated from: '<S5>/Vector Concatenate' */
  rtb_VectorConcatenate_idx_0 = rtb_Product1;

  /* Gain: '<S5>/Gain' */
  rtb_VectorConcatenate_idx_3 = 0.1 * rtb_Product1;

  /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
   *  Constant: '<S5>/Constant'
   *  Constant: '<S5>/Constant2'
   *  Constant: '<S5>/Constant3'
   *  Saturate: '<Root>/Saturation2'
   */
  /* MATLAB Function 'Subsystem/Variant Subsystem/fixed_ca/MATLAB Function': '<S6>:1' */
  /*  Fixed point iteration QP solution for CA based on */
  /*   */
  /* '<S6>:1:6' eps = 0.0000001; */
  /* '<S6>:1:7' F_err = 1e9*ones(4,1); */
  rtb_F_err[0] = 1.0E+9;
  rtb_F_err[1] = 1.0E+9;
  rtb_F_err[2] = 1.0E+9;
  rtb_F_err[3] = 1.0E+9;

  /* '<S6>:1:8' u_a = pinv(B)*F_d; */
  (void)memset(&BtW[0], 0, (sizeof(real_T)) << 5U);
  for (vcol = 0; vcol < 4; vcol++) {
    for (i = 0; i < 8; i++) {
      tmp[i + (8 * vcol)] = MR_CA_ConstP.pooled1[vcol + (4 * i)];
    }
  }

  svd_o1pFIz8b(tmp, U, s, V);
  rtb_Product1 = fabs(s[0]);
  if (rtb_Product1 < 4.4501477170144028E-308) {
    rtb_Product1 = 4.94065645841247E-324;
  } else {
    (void)frexp(rtb_Product1, &r);
    rtb_Product1 = ldexp(1.0, r - 53);
  }

  rtb_Product1 *= 8.0;
  r = -1;
  b_k = 0;
  while ((b_k < 4) && (!(s[b_k] <= rtb_Product1))) {
    r++;
    b_k++;
  }

  if ((r + 1) > 0) {
    vcol = 1;
    for (b_k = 0; b_k <= r; b_k++) {
      rtb_Product1 = 1.0 / s[b_k];
      for (i = vcol; i <= (vcol + 3); i++) {
        V[i - 1] *= rtb_Product1;
      }

      vcol += 4;
    }

    for (b_k = 0; b_k <= 28; b_k += 4) {
      for (i = b_k + 1; i <= (b_k + 4); i++) {
        BtW[i - 1] = 0.0;
      }
    }

    br = 0;
    for (b_k = 0; b_k <= 28; b_k += 4) {
      ar = -1;
      br++;
      d = br + (8 * r);
      for (i = br; i <= d; i += 8) {
        for (vcol = b_k + 1; vcol <= (b_k + 4); vcol++) {
          BtW[vcol - 1] += U[i - 1] * V[(ar + vcol) - b_k];
        }

        ar += 4;
      }
    }
  }

  /* '<S6>:1:9' u_a(u_a>u_max) = u_max(u_a>u_max); */
  /* '<S6>:1:10' u_a(u_a<u_min) = u_min(u_a<u_min); */
  t = rtb_Saturation2[0];
  rtb_Saturation2_0 = rtb_Saturation2[2];
  for (i = 0; i < 8; i++) {
    rtb_Product1 = (((BtW[4 * i] * t) + (BtW[(4 * i) + 1] * scale)) + (BtW[(4 *
      i) + 2] * rtb_Saturation2_0)) + (BtW[(4 * i) + 3] * absxk);
    c1_tmp[i] = rtb_Product1;
    if (rtb_Product1 > 1.0) {
      rtb_Product1 = 1.0;
      c1_tmp[i] = 1.0;
    }

    rtb_Product_0 = rtb_Product[i];
    if (rtb_Product1 < rtb_Product_0) {
      c1_tmp[i] = rtb_Product_0;
    }
  }

  /* '<S6>:1:11' u_pinv = u_a; */
  /* '<S6>:1:13' W = diag(W_diag); */
  (void)memset(&V[0], 0, (sizeof(real_T)) << 4U);
  V[0] = rtb_VectorConcatenate_idx_0;
  V[5] = 1.0;
  V[10] = 1.0;
  V[15] = rtb_VectorConcatenate_idx_3;

  /* '<S6>:1:14' BtW = B'*W; */
  /* '<S6>:1:15' M = (1-eps)*(BtW*B) + eps*I; */
  for (vcol = 0; vcol < 8; vcol++) {
    t = tmp[vcol];
    rtb_Product1 = tmp[vcol + 8];
    rtb_VectorConcatenate_idx_0 = tmp[vcol + 16];
    rtb_VectorConcatenate_idx_3 = tmp[vcol + 24];
    for (i = 0; i < 4; i++) {
      BtW[vcol + (8 * i)] = (((t * V[4 * i]) + (rtb_Product1 * V[(4 * i) + 1]))
        + (rtb_VectorConcatenate_idx_0 * V[(4 * i) + 2])) +
        (rtb_VectorConcatenate_idx_3 * V[(4 * i) + 3]);
    }

    rtb_Product1 = BtW[vcol];
    t = BtW[vcol + 8];
    rtb_VectorConcatenate_idx_0 = BtW[vcol + 16];
    rtb_VectorConcatenate_idx_3 = BtW[vcol + 24];
    for (i = 0; i < 8; i++) {
      BtW_0[vcol + (8 * i)] = (((rtb_Product1 * MR_CA_ConstP.pooled1[4 * i]) +
        (t * MR_CA_ConstP.pooled1[(4 * i) + 1])) + (rtb_VectorConcatenate_idx_0 *
        MR_CA_ConstP.pooled1[(4 * i) + 2])) + (rtb_VectorConcatenate_idx_3 *
        MR_CA_ConstP.pooled1[(4 * i) + 3]);
    }
  }

  /* '<S6>:1:16' eta = 1/sqrt(sum(sum(M.*M))); */
  for (vcol = 0; vcol < 64; vcol++) {
    rtb_Product1 = (0.9999999 * BtW_0[vcol]) + (1.0E-7 *
      MR_CA_ConstP.Constant3_Value_gqydu0pxng[vcol]);
    M[vcol] = rtb_Product1;
    x[vcol] = rtb_Product1 * rtb_Product1;
  }

  for (r = 0; r < 8; r++) {
    i = r * 8;
    rtb_Product1 = x[i];
    for (b_k = 0; b_k < 7; b_k++) {
      rtb_Product1 += x[(i + b_k) + 1];
    }

    c1[r] = rtb_Product1;
  }

  rtb_Product1 = c1[0];
  for (r = 0; r < 7; r++) {
    rtb_Product1 += c1[r + 1];
  }

  rtb_Product1 = 1.0 / sqrt(rtb_Product1);

  /* '<S6>:1:18' A1 = (I - eta*M); */
  for (vcol = 0; vcol < 64; vcol++) {
    M[vcol] = MR_CA_ConstP.Constant3_Value_gqydu0pxng[vcol] - (rtb_Product1 *
      M[vcol]);
  }

  /* '<S6>:1:19' c1 = (1-eps)*eta*BtW*F_d; */
  rtb_Product1 *= 0.9999999;
  t = rtb_Saturation2[0];
  rtb_Saturation2_0 = rtb_Saturation2[2];
  for (vcol = 0; vcol < 8; vcol++) {
    c1[vcol] = ((((rtb_Product1 * BtW[vcol]) * t) + ((rtb_Product1 * BtW[vcol +
      8]) * scale)) + ((rtb_Product1 * BtW[vcol + 16]) * rtb_Saturation2_0)) +
      ((rtb_Product1 * BtW[vcol + 24]) * absxk);
  }

  /* '<S6>:1:21' for k=1:N_max */
  b_k = 0;
  exitg1 = false;
  while (((exitg1 ? ((uint32_T)1U) : ((uint32_T)0U)) == false) && (b_k <= 99)) {
    /* '<S6>:1:23' u_a =  A1*u_a + c1; */
    for (vcol = 0; vcol < 8; vcol++) {
      t = 0.0;
      for (i = 0; i < 8; i++) {
        t += M[vcol + (8 * i)] * c1_tmp[i];
      }

      M_0[vcol] = t + c1[vcol];
    }

    /* '<S6>:1:24' u_a(u_a>u_max) = u_max(u_a>u_max); */
    /* '<S6>:1:25' u_a(u_a<u_min) = u_min(u_a<u_min); */
    for (i = 0; i < 8; i++) {
      rtb_Product1 = M_0[i];
      c1_tmp[i] = rtb_Product1;
      if (rtb_Product1 > 1.0) {
        rtb_Product1 = 1.0;
        c1_tmp[i] = 1.0;
      }

      rtb_Product_0 = rtb_Product[i];
      if (rtb_Product1 < rtb_Product_0) {
        c1_tmp[i] = rtb_Product_0;
      }
    }

    /* '<S6>:1:27' F_err = F_d - B*u_a; */
    /* '<S6>:1:28' F_err_norm = norm(F_err); */
    rtb_Product1 = 0.0;
    scale = 3.3121686421112381E-170;
    for (r = 0; r < 4; r++) {
      t = 0.0;
      for (vcol = 0; vcol < 8; vcol++) {
        t += MR_CA_ConstP.pooled1[r + (4 * vcol)] * c1_tmp[vcol];
      }

      t = rtb_Saturation2[r] - t;
      rtb_F_err[r] = t;
      absxk = fabs(t);
      if (absxk > scale) {
        t = scale / absxk;
        rtb_Product1 = ((rtb_Product1 * t) * t) + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        rtb_Product1 += t * t;
      }
    }

    rtb_Product1 = scale * sqrt(rtb_Product1);

    /* '<S6>:1:29' if(F_err_norm<1e-1) */
    if (rtb_Product1 < 0.1) {
      exitg1 = true;
    } else {
      b_k++;
    }
  }

  /* End of MATLAB Function: '<S5>/MATLAB Function' */
  /* '<S6>:1:35' epsilon = 0.01; */
  /* '<S6>:1:36' motor_sat = any(u_a >= u_max*(1-epsilon)) | any(u_a <= u_min*(1+epsilon)); */
  /*  F_a = B*u_a; */
  for (i = 0; i < 8; i++) {
    /* Sqrt: '<S28>/Sqrt' */
    rtb_BusAssignment2.lifter_cval_cmd[i] = sqrt(c1_tmp[i]);

    /* Product: '<S28>/Product' incorporates:
     *  Constant: '<S28>/Constant8'
     *  Sqrt: '<S28>/Sqrt'
     */
    rtb_BusAssignment2.lifter_rpm_cmd[i] = 0.0;
  }

  /* Logic: '<S11>/OR2' incorporates:
   *  Constant: '<S14>/Constant'
   *  Constant: '<S15>/Constant'
   *  Constant: '<S16>/Constant'
   *  Constant: '<S17>/Constant'
   *  Constant: '<S18>/Constant'
   *  Constant: '<S19>/Constant'
   *  RelationalOperator: '<S14>/Compare'
   *  RelationalOperator: '<S15>/Compare'
   *  RelationalOperator: '<S16>/Compare'
   *  RelationalOperator: '<S17>/Compare'
   *  RelationalOperator: '<S18>/Compare'
   *  RelationalOperator: '<S19>/Compare'
   */
  rtb_OR2 = (((((((*rtu_vom_status) == F_TRANS) || ((*rtu_vom_status) == B_TRANS))
                || ((*rtu_vom_status) == FLTDIR)) || ((*rtu_vom_status) ==
    LOITER)) || ((*rtu_vom_status) == WAYPNT)) || ((*rtu_vom_status) == FW_RTH));

  /* Switch: '<S2>/Switch' incorporates:
   *  Constant: '<S2>/STARTUP_PWM_2'
   *  Constant: '<S2>/readyPWM2'
   */
  if (*rtu_sFlags_rampup_phase) {
    rtb_Product1 = 0.25;
  } else {
    rtb_Product1 = 0.1;
  }

  /* End of Switch: '<S2>/Switch' */

  /* RateLimiter: '<S2>/Rate Limiter' */
  scale = rtb_Product1 - MR_CA_DW.PrevY;
  if (scale > 0.00075) {
    rtb_Product1 = MR_CA_DW.PrevY + 0.00075;
  } else if (scale < -10.0) {
    rtb_Product1 = MR_CA_DW.PrevY - 10.0;
  } else {
    /* no actions */
  }

  MR_CA_DW.PrevY = rtb_Product1;

  /* End of RateLimiter: '<S2>/Rate Limiter' */

  /* RelationalOperator: '<S9>/Compare' incorporates:
   *  Constant: '<S9>/Constant'
   */
  rtb_Compare_cxljnxpqat = ((*rtu_vom_status) == READY);

  /* DiscreteIntegrator: '<S12>/Discrete-Time Integrator' */
  if (rtb_Compare_cxljnxpqat || (MR_CA_DW.DiscreteTimeIntegrator_PrevResetState
       != 0)) {
    MR_CA_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  }

  /* RelationalOperator: '<S8>/Compare' incorporates:
   *  Constant: '<S8>/Constant'
   */
  rtb_Compare_pdi014sibj = ((*rtu_vom_status) == STARTUP);

  /* Switch: '<S2>/Switch5' incorporates:
   *  Constant: '<S10>/Constant'
   *  Logic: '<S2>/Logical Operator'
   *  Logic: '<S2>/OR'
   *  RelationalOperator: '<S10>/Compare'
   *  Switch: '<S2>/Switch1'
   */
  if (((*rtu_vom_status) == UMAN) || rtb_OR2) {
    for (i = 0; i < 8; i++) {
      /* Switch: '<S2>/Switch2' incorporates:
       *  MultiPortSwitch: '<S2>/Multiport Switch'
       */
      if (rtb_OR2) {
        /* MultiPortSwitch: '<S2>/Multiport Switch2' */
        if ((*rtu_FW_LifterMode_eFWLifter_Mode) != ON) {
          /* Switch: '<S2>/Switch5' incorporates:
           *  Constant: '<S2>/lifter_off1'
           *  Switch: '<S2>/Switch2'
           */
          rtb_BusAssignment2.lifter_cval_cmd[i] = 0.0;
        }

        /* End of MultiPortSwitch: '<S2>/Multiport Switch2' */
      } else if ((*rtu_lifter_state) != ON) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S2>/lifter_off'
         *  MultiPortSwitch: '<S2>/Multiport Switch'
         *  Switch: '<S2>/Switch2'
         */
        rtb_BusAssignment2.lifter_cval_cmd[i] = 0.0;
      } else {
        /* no actions */
      }

      /* End of Switch: '<S2>/Switch2' */
    }
  } else if (rtb_Compare_pdi014sibj || rtb_Compare_cxljnxpqat) {
    /* Switch: '<S2>/Switch4' incorporates:
     *  Switch: '<S2>/Switch1'
     *  Switch: '<S2>/Switch3'
     */
    if (*rtu_sFlags_rampup_phase) {
      /* Switch: '<S2>/Switch5' */
      for (i = 0; i < 8; i++) {
        rtb_BusAssignment2.lifter_cval_cmd[i] = rtb_Product1;
      }
    } else if (rtb_Compare_pdi014sibj) {
      /* Switch: '<S12>/Switch1' incorporates:
       *  Constant: '<S20>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S20>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 1.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[0] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[0] = 0.0;
      }

      /* End of Switch: '<S12>/Switch1' */

      /* Switch: '<S12>/Switch2' incorporates:
       *  Constant: '<S21>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S21>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 2.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[1] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[1] = 0.0;
      }

      /* End of Switch: '<S12>/Switch2' */

      /* Switch: '<S12>/Switch3' incorporates:
       *  Constant: '<S22>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S22>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 3.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[2] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[2] = 0.0;
      }

      /* End of Switch: '<S12>/Switch3' */

      /* Switch: '<S12>/Switch4' incorporates:
       *  Constant: '<S23>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S23>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 4.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[3] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[3] = 0.0;
      }

      /* End of Switch: '<S12>/Switch4' */

      /* Switch: '<S12>/Switch5' incorporates:
       *  Constant: '<S24>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S24>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 5.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[4] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[4] = 0.0;
      }

      /* End of Switch: '<S12>/Switch5' */

      /* Switch: '<S12>/Switch6' incorporates:
       *  Constant: '<S25>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S25>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 6.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[5] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[5] = 0.0;
      }

      /* End of Switch: '<S12>/Switch6' */

      /* Switch: '<S12>/Switch9' incorporates:
       *  Constant: '<S26>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S26>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 7.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[6] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[6] = 0.0;
      }

      /* End of Switch: '<S12>/Switch9' */

      /* Switch: '<S12>/Switch7' incorporates:
       *  Constant: '<S27>/Constant'
       *  DiscreteIntegrator: '<S12>/Discrete-Time Integrator'
       *  RelationalOperator: '<S27>/Compare'
       *  Switch: '<S2>/Switch3'
       */
      if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE >= 8.0) {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/STARTUP_PWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[7] = 0.1;
      } else {
        /* Switch: '<S2>/Switch5' incorporates:
         *  Constant: '<S12>/readyPWM'
         */
        rtb_BusAssignment2.lifter_cval_cmd[7] = 0.0;
      }

      /* End of Switch: '<S12>/Switch7' */
    } else {
      /* Switch: '<S2>/Switch5' incorporates:
       *  Constant: '<S2>/readyPWM'
       *  Switch: '<S2>/Switch3'
       */
      (void)memset(&rtb_BusAssignment2.lifter_cval_cmd[0], 0, (sizeof(real_T)) <<
                   3U);
    }

    /* End of Switch: '<S2>/Switch4' */
  } else {
    /* no actions */
  }

  /* End of Switch: '<S2>/Switch5' */
  (void)memset(rty_controllerCA, 0, sizeof(busControllerCA));

  /* BusAssignment: '<Root>/Bus Assignment1' incorporates:
   *  Abs: '<S5>/Abs'
   *  BusAssignment: '<Root>/Bus Assignment2'
   *  Concatenate: '<Root>/Vector Concatenate'
   *  Constant: '<Root>/Constant'
   *  Constant: '<S5>/Constant5'
   *  DataTypeConversion: '<S5>/Cast To Double'
   *  Product: '<Root>/Product'
   *  Product: '<S28>/Product'
   *  Product: '<S5>/Product1'
   *  RelationalOperator: '<S5>/Relational Operator'
   *  Saturate: '<Root>/Saturation2'
   */
  rty_controllerCA->stdCtrl_IF = rtb_BusAssignment2;
  (void)memcpy(&rty_controllerCA->lifterCommand[0],
               &rtb_BusAssignment2.lifter_rpm_cmd[0], (sizeof(real_T)) << 3U);
  rty_controllerCA->c_erp1 = (real_T)((int32_T)(((fabs(rtb_F_err[1]) <
    0.86920000000000008) ? ((int32_T)1) : ((int32_T)0)) * ((fabs(rtb_F_err[2]) <
    0.7768) ? ((int32_T)1) : ((int32_T)0))));
  rty_controllerCA->c_erp2 = (real_T)((fabs(rtb_F_err[0]) < 1.7999999999999998) ?
    ((int32_T)1) : ((int32_T)0));
  rty_controllerCA->c_erp3 = (real_T)((fabs(rtb_F_err[3]) < 1.4872) ? ((int32_T)
    1) : ((int32_T)0));
  for (i = 0; i < 4; i++) {
    /* Product: '<Root>/Product' incorporates:
     *  Constant: '<Root>/Constant'
     */
    scale = 0.0;
    for (vcol = 0; vcol < 8; vcol++) {
      scale += MR_CA_ConstP.pooled1[i + (4 * vcol)] * c1_tmp[vcol];
    }

    rty_controllerCA->nu_allocated[i] = scale;
    rty_controllerCA->nu_filtered[i] = rtb_Saturation2[i];
    rty_controllerCA->nu_des[i] = rtb_VectorConcatenate_fo5jsv5trp[i];
  }

  /* End of BusAssignment: '<Root>/Bus Assignment1' */

  /* Update for DiscreteIntegrator: '<S12>/Discrete-Time Integrator' incorporates:
   *  Switch: '<S12>/Switch8'
   */
  MR_CA_DW.DiscreteTimeIntegrator_DSTATE += 0.01 * ((real_T)
    (rtb_Compare_pdi014sibj ? 1.0 : 0.0));
  if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE > 8.0) {
    MR_CA_DW.DiscreteTimeIntegrator_DSTATE = 8.0;
  } else if (MR_CA_DW.DiscreteTimeIntegrator_DSTATE < 0.0) {
    MR_CA_DW.DiscreteTimeIntegrator_DSTATE = 0.0;
  } else {
    /* no actions */
  }

  MR_CA_DW.DiscreteTimeIntegrator_PrevResetState = (int8_T)
    (rtb_Compare_cxljnxpqat ? 1 : 0);

  /* End of Update for DiscreteIntegrator: '<S12>/Discrete-Time Integrator' */
}

/* Model initialize function */
void MR_CA_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&MR_CA_DW, 0,
                sizeof(MR_CA_TDW));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
