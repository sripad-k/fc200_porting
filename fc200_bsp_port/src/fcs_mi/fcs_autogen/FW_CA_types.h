/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_CA_types.h
 *
 * Code generated for Simulink model 'FW_CA'.
 *
 * Model version                  : 2.106
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:50:05 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_CA_types_h_
#define FW_CA_types_h_
#include "rtwtypes.h"
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

#ifndef DEFINED_TYPEDEF_FOR_std_sensor_t_
#define DEFINED_TYPEDEF_FOR_std_sensor_t_

typedef struct {
  real_T dcm_e2b[9];
  real_T eul_ang[3];
  real_T omg[3];
  real_T pos_lla[3];
  real_T vel_ned[3];
  real_T accel_b[3];
  real_T aspd_cas;
  real_T aoa;
  real_T aos;
  real_T h_gps_amsl;
  real_T h_baro_amsl;
  real_T h_radar_agl;
  real_T wow[3];
  real_T rotor_rpm[8];

  /* Ground speed. Always greater than 0.01 m/s */
  real_T gspd;

  /* Course over ground */
  real_T chi;
} std_sensor_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_std_ctrl_t_
#define DEFINED_TYPEDEF_FOR_std_ctrl_t_

typedef struct {
  real_T lifter_rpm_cmd[8];
  real_T eng_throttle_cmd;
  real_T acs_servo_deg_cmd[12];
  real_T nlg_servo_deg_cmd;
  real_T lifter_cval_cmd[8];
  real_T eng_mixture_cmd;
  real_T eng_ignition_cmd;
  real_T pusher_pwm_cmd;
} std_ctrl_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerCA_
#define DEFINED_TYPEDEF_FOR_busControllerCA_

typedef struct {
  std_ctrl_t stdCtrl_IF;
  real_T lifterCommand[8];
  real_T c_erp1;
  real_T c_erp2;
  real_T c_erp3;
  real_T nu_des[4];
  real_T nu_allocated[4];
  real_T nu_filtered[4];
  real_T ACS_deg[12];
} busControllerCA;

#endif
#endif                                 /* FW_CA_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
