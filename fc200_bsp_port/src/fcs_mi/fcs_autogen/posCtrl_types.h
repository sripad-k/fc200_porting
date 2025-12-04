/*
 * File: posCtrl_types.h
 *
 * Code generated for Simulink model 'posCtrl'.
 *
 * Model version                  : 2.51
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:39 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef posCtrl_types_h_
#define posCtrl_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_vom_t_
#define DEFINED_TYPEDEF_FOR_vom_t_

typedef enum {
  VOM_INVALID = 0,                     /* Default value */
  VOM_INIT = 1,
  VOM_READY = 2,
  VOM_TAXI = 3,
  VOM_PFBIT = 4,
  VOM_GNDOPS = 5,
  VOM_ZEROG = 6,
  VOM_STARTUP = 10,
  VOM_TAKEOFF = 20,
  VOM_HOVER = 30,
  VOM_MANUAL = 40,
  VOM_LAND = 50,
  VOM_MR_RTH = 60,
  VOM_UMAN = 70,
  VOM_F_TRANS = 80,
  VOM_B_TRANS = 90,
  VOM_WAYPNT = 100,
  VOM_FLTDIR = 110,
  VOM_LOITER = 120,
  VOM_FW_RTH = 130
} vom_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busWaypointManager_
#define DEFINED_TYPEDEF_FOR_busWaypointManager_

typedef struct {
  real_T posLLA[3];
  real_T yawCmd;
} busWaypointManager;

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

#ifndef DEFINED_TYPEDEF_FOR_busControllerPosCtrl_
#define DEFINED_TYPEDEF_FOR_busControllerPosCtrl_

typedef struct {
  real_T pos_xy_cmd[2];
  real_T pos_xy[2];
  real_T hCmd;
  real_T psiCmd;
} busControllerPosCtrl;

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
#endif                                 /* posCtrl_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
