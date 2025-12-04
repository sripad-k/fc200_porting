/*
 * File: velCtrl_types.h
 *
 * Code generated for Simulink model 'velCtrl'.
 *
 * Model version                  : 2.9
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:20 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef velCtrl_types_h_
#define velCtrl_types_h_
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

#ifndef DEFINED_TYPEDEF_FOR_busControllerVelCtrl_
#define DEFINED_TYPEDEF_FOR_busControllerVelCtrl_

typedef struct {
  real_T velCmd[2];
  real_T velRef[2];
  real_T vel_xy_N[2];
} busControllerVelCtrl;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerIF_att_
#define DEFINED_TYPEDEF_FOR_busControllerIF_att_

typedef struct {
  real_T rollCmd;
  real_T pitchCmd;
  real_T yawCmd;
  real_T yawRateCmd;
  boolean_T yawHold;
  real_T yawFF;
  boolean_T init_reset;
} busControllerIF_att;

#endif
#endif                                 /* velCtrl_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
