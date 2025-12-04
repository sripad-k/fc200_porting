/*
 * File: FW_TECS_switcher_types.h
 *
 * Code generated for Simulink model 'FW_TECS_switcher'.
 *
 * Model version                  : 2.179
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:02 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_TECS_switcher_types_h_
#define FW_TECS_switcher_types_h_
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

#ifndef DEFINED_TYPEDEF_FOR_pic_t_
#define DEFINED_TYPEDEF_FOR_pic_t_

typedef enum {
  EXTERNAL = 0,                        /* Default value */
  INTERNAL = 1,
  PIC_INVALID = 255
} pic_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFW_TECS_switcher_
#define DEFINED_TYPEDEF_FOR_busFW_TECS_switcher_

typedef struct {
  real_T h_dot_cmd;
  real_T h_cmd;
  real_T V_cmd;
  real_T roll_cmd;
  boolean_T TECS_reset;
  boolean_T h_hold;
} busFW_TECS_switcher;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFWRTH_data_in_
#define DEFINED_TYPEDEF_FOR_busFWRTH_data_in_

typedef struct {
  boolean_T approach_circle_done;
  boolean_T reduce_speed_alt_done;
  boolean_T circle_align_done;
  boolean_T cross_tangent_point_done;
  real_T land_lat;
  real_T land_lon;
  real_T approach_ang;
} busFWRTH_data_in;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busWP_SMdata_in_
#define DEFINED_TYPEDEF_FOR_busWP_SMdata_in_

typedef struct {
  real_T cur_leg_heading;
  real_T cur_leg_length;
  real_T cur_leg_remaining_dist;
  boolean_T wp_list_valid;
  boolean_T last_wp_land;
  real_T land_wp_lat;
  real_T land_wp_lon;
  real_T curpos_to_wp_heading;
  boolean_T WPN_cmd_received;
} busWP_SMdata_in;

#endif
#endif                                 /* FW_TECS_switcher_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
