/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_TECS_types.h
 *
 * Code generated for Simulink model 'FW_TECS'.
 *
 * Model version                  : 2.58
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:50:31 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_TECS_types_h_
#define FW_TECS_types_h_
#include "rtwtypes.h"
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

#ifndef DEFINED_TYPEDEF_FOR_busTECS_Data_Processing_
#define DEFINED_TYPEDEF_FOR_busTECS_Data_Processing_

typedef struct {
  real_T gammaRad;
  real_T VdotDivG;
  real_T Ldot;
  real_T Edot;
} busTECS_Data_Processing;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerTECS_
#define DEFINED_TYPEDEF_FOR_busControllerTECS_

typedef struct {
  real_T engineCmd;
  real_T theta_cmd;
  real_T vDotDivG_cmd;
  real_T gamma_cmd;
  real_T Edot_cmd;
  real_T Ldot_cmd;
  real_T vel_cmd;
  real_T hCmd;
  real_T vel;
  real_T h;
  real_T gamma;
  real_T vDotDivG;
  real_T Edot;
  real_T Ldot;
} busControllerTECS;

#endif
#endif                                 /* FW_TECS_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
