/*
 * File: att_ctrl_switcher_types.h
 *
 * Code generated for Simulink model 'att_ctrl_switcher'.
 *
 * Model version                  : 2.209
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:26 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef att_ctrl_switcher_types_h_
#define att_ctrl_switcher_types_h_
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

#ifndef DEFINED_TYPEDEF_FOR_eFWRTH_phase_
#define DEFINED_TYPEDEF_FOR_eFWRTH_phase_

typedef enum {
  approach_circle = 0,                 /* Default value */
  set_speed_alt,
  align_circle,
  cross_tangent
} eFWRTH_phase;

#endif
#endif                                 /* att_ctrl_switcher_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
