/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: vel_ctrl_switcher_types.h
 *
 * Code generated for Simulink model 'vel_ctrl_switcher'.
 *
 * Model version                  : 2.83
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:53:03 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef vel_ctrl_switcher_types_h_
#define vel_ctrl_switcher_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_lifter_state_t_
#define DEFINED_TYPEDEF_FOR_lifter_state_t_

typedef enum {
  OFF = 0,
  ON                                   /* Default value */
} lifter_state_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_vom_t_
#define DEFINED_TYPEDEF_FOR_vom_t_

typedef enum {
  INVALID = 0,                         /* Default value */
  INIT = 1,
  READY = 2,
  TAXI = 3,
  PFBIT = 4,
  GNDOPS = 5,
  STARTUP = 10,
  TAKEOFF = 20,
  HOVER = 30,
  MANUAL = 40,
  LAND = 50,
  MR_RTH = 60,
  UMAN = 70,
  F_TRANS = 80,
  B_TRANS = 90,
  WAYPNT = 100,
  FLTDIR = 110,
  LOITER = 120,
  FW_RTH = 130
} vom_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_pilot_ext_t_
#define DEFINED_TYPEDEF_FOR_pilot_ext_t_

typedef struct {
  real_T roll_ch;
  real_T pitch_ch;
  real_T throttle_ch;
  real_T yaw_ch;
  real_T engine_ch;
  real_T arm_ch;
  int8_T switch_1;
  int8_T switch_2;
  int8_T switch_3;
  int8_T switch_4;
} pilot_ext_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerIF_vel_
#define DEFINED_TYPEDEF_FOR_busControllerIF_vel_

typedef struct {
  real_T velCmdH_x;
  real_T velCmdH_y;
  real_T hRateCmd;
  real_T hCmd;
  boolean_T hHold;
  boolean_T init_reset_vel;
  boolean_T init_reset_hdot;
} busControllerIF_vel;

#endif

#ifndef DEFINED_TYPEDEF_FOR_eFWRTH_phase_
#define DEFINED_TYPEDEF_FOR_eFWRTH_phase_

typedef enum {
  approach_circle = 0,                 /* Default value */
  set_speed_alt,
  align_circle,
  cross_tangent
} eFWRTH_phase;

#endif
#endif                                 /* vel_ctrl_switcher_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
