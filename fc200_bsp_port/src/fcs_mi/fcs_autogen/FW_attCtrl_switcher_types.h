/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_attCtrl_switcher_types.h
 *
 * Code generated for Simulink model 'FW_attCtrl_switcher'.
 *
 * Model version                  : 2.57
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:51:14 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_attCtrl_switcher_types_h_
#define FW_attCtrl_switcher_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_lifter_state_t_
#define DEFINED_TYPEDEF_FOR_lifter_state_t_

typedef enum {
  OFF = 0,
  ON                                   /* Default value */
} lifter_state_t;

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

#ifndef DEFINED_TYPEDEF_FOR_busFW_Controller_att_
#define DEFINED_TYPEDEF_FOR_busFW_Controller_att_

typedef struct {
  real_T roll_cmd;
  real_T pitch_cmd;
  real_T pusher_cmd;
  boolean_T roll_reset;
  boolean_T pitch_reset;
  boolean_T yaw_reset;
} busFW_Controller_att;

#endif
#endif                                 /* FW_attCtrl_switcher_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
