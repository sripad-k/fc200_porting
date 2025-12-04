/*
 * File: vel_ctrl_switcher_private.h
 *
 * Code generated for Simulink model 'vel_ctrl_switcher'.
 *
 * Model version                  : 2.87
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:25 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef vel_ctrl_switcher_private_h_
#define vel_ctrl_switcher_private_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "vel_ctrl_switcher_types.h"

/* Block states (default storage) for model 'vel_ctrl_switcher' */
typedef struct {
  real_T Delay_DSTATE;                 /* '<S14>/Delay' */
  real_T Delay_DSTATE_pqtvtrkc0x;      /* '<S19>/Delay' */
  real_T PrevY;                        /* '<S3>/Rate Limiter' */
  boolean_T DelayInput1_DSTATE;        /* '<S21>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_jqcqpgbfgg;/* '<S22>/Delay Input1' */
  boolean_T icLoad;                    /* '<S14>/Delay' */
  boolean_T icLoad_k1hpx4rg1s;         /* '<S19>/Delay' */
} vel_ctrl_switcher_TDW;

/* Zero-crossing (trigger) state for model 'vel_ctrl_switcher' */
typedef struct {
  ZCSigState Delay_Reset_ZCE;          /* '<S14>/Delay' */
  ZCSigState Delay_Reset_ZCE_fbmawcfj04;/* '<S19>/Delay' */
} vel_ctrl_switcher_TZCE;

/* Block states (default storage) */
extern vel_ctrl_switcher_TDW vel_ctrl_switcher_DW;

/* Previous zero-crossings (trigger) states */
extern vel_ctrl_switcher_TZCE vel_ctrl_switcher_PrevZCX;

#endif                                 /* vel_ctrl_switcher_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
