/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: mc_path_planner_types.h
 *
 * Code generated for Simulink model 'mc_path_planner'.
 *
 * Model version                  : 2.23
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:16 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef mc_path_planner_types_h_
#define mc_path_planner_types_h_
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

#ifndef DEFINED_TYPEDEF_FOR_busWaypointManager_
#define DEFINED_TYPEDEF_FOR_busWaypointManager_

typedef struct {
  real_T posLLA[3];
  real_T yawCmd;
} busWaypointManager;

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
#endif                                 /* mc_path_planner_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
