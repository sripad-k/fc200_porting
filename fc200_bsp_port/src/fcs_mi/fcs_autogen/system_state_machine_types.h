/*
 * File: system_state_machine_types.h
 *
 * Code generated for Simulink model 'system_state_machine'.
 *
 * Model version                  : 3.635
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:05 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef system_state_machine_types_h_
#define system_state_machine_types_h_
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

#ifndef DEFINED_TYPEDEF_FOR_busStartup_data_
#define DEFINED_TYPEDEF_FOR_busStartup_data_

typedef struct {
  boolean_T startup_engineARMFlag;
  boolean_T startup_motorARMFlag;
} busStartup_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busHover_data_
#define DEFINED_TYPEDEF_FOR_busHover_data_

typedef struct {
  real_T hover_y;
  real_T hover_x;
  real_T hover_alt_agl;
  real_T hover_yaw_ref;
  boolean_T hover_yaw_override;
} busHover_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAutotakeoff_data_
#define DEFINED_TYPEDEF_FOR_busAutotakeoff_data_

typedef struct {
  real_T takeoff_x;
  real_T takeoff_y;
  real_T takeoff_alt_agl;
  real_T takeoff_yaw_ref;
} busAutotakeoff_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAutoland_data_
#define DEFINED_TYPEDEF_FOR_busAutoland_data_

typedef struct {
  real_T land_x;
  real_T land_y;
  real_T land_yaw_ref;
} busAutoland_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busRTH_data_
#define DEFINED_TYPEDEF_FOR_busRTH_data_

typedef struct {
  real_T rth_x;
  real_T rth_y;
  real_T rth_velX;
  real_T rth_velY;
  real_T rth_alt_agl;
  real_T rth_yaw_ref;
  boolean_T bRTH_PosModeFlag;
  boolean_T bBreaking;
} busRTH_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_lifter_state_t_
#define DEFINED_TYPEDEF_FOR_lifter_state_t_

typedef enum {
  OFF = 0,
  ON                                   /* Default value */
} lifter_state_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFTransition_data_
#define DEFINED_TYPEDEF_FOR_busFTransition_data_

typedef struct {
  real_T FT_x;
  real_T FT_y;
  real_T FT_Altitude;
  real_T FT_Heading;
  real_T FT_AirspeedRef;
  real_T FT_CoG;
  real_T FT_PusherThrottle;
  boolean_T bFT_AttitudeModeFlag;
  real_T FT_PitchCmd;
  real_T FT_RollCmd;
  lifter_state_t eLifter_Mode;
} busFTransition_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFlightDirector_data_
#define DEFINED_TYPEDEF_FOR_busFlightDirector_data_

typedef struct {
  real_T FD_CoG;
  real_T FD_AirspeedRef;
  real_T FD_Altitude;
  real_T FTransition_y;
} busFlightDirector_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busLoiter_data_
#define DEFINED_TYPEDEF_FOR_busLoiter_data_

typedef struct {
  real_T loiter_radius;
  real_T loiter_direction;
  real_T loiter_Center_Lat;
  real_T loiter_Center_Lon;
  real_T loiter_altitude;
  real_T loiter_AirSpeedRef;
} busLoiter_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busBTransiton_data_
#define DEFINED_TYPEDEF_FOR_busBTransiton_data_

typedef struct {
  real_T BT_Hover_Lat;
  real_T BT_Hover_Lon;
  real_T BT_Altitude;
  real_T BT_Heading;
  real_T BT_PusherThrottle;
  boolean_T bBT_AttitudeModeFlag;
  real_T BT_PitchCmd;
  real_T BT_RollCmd;
  lifter_state_t eLifter_Mode;
} busBTransiton_data;

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

#ifndef DEFINED_TYPEDEF_FOR_busFWRTH_data_
#define DEFINED_TYPEDEF_FOR_busFWRTH_data_

typedef struct {
  eFWRTH_phase phase;
  real_T CAS_sp;
  real_T alt_sp;
} busFWRTH_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busMode_data_
#define DEFINED_TYPEDEF_FOR_busMode_data_

typedef struct {
  busStartup_data startup_data;
  busHover_data hover_data;
  busAutotakeoff_data autotakeoff_data;
  busAutoland_data autoland_data;
  busRTH_data rth_data;
  busFTransition_data ft_data;
  busFlightDirector_data flightDirector_data;
  busLoiter_data loiter_data;
  busBTransiton_data bt_data;
  lifter_state_t eFWLifter_Mode;
  busFWRTH_data fwrth_data;
} busMode_data;

#endif

#ifndef DEFINED_TYPEDEF_FOR_pic_t_
#define DEFINED_TYPEDEF_FOR_pic_t_

typedef enum {
  EXTERNAL = 0,                        /* Default value */
  INTERNAL = 1,
  PIC_INVALID = 255
} pic_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_failure_flag_t_
#define DEFINED_TYPEDEF_FOR_failure_flag_t_

typedef struct {
  boolean_T gcs_link_loss;
  boolean_T gps_loss;
  boolean_T ep_data_loss;
  boolean_T ip_data_loss;
} failure_flag_t;

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

#ifndef DEFINED_TYPEDEF_FOR_safety_state_t_
#define DEFINED_TYPEDEF_FOR_safety_state_t_

typedef enum {
  AC_DISARMED = 0,                     /* Default value */
  AC_ARMED
} safety_state_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busStateMachine_
#define DEFINED_TYPEDEF_FOR_busStateMachine_

typedef struct {
  vom_t vom_status;
  safety_state_t safety_state;
  uint8_T inAir_flag;
  uint8_T pic_status;
  uint8_T loiter_mode;
  uint8_T lifter_mode;
  uint8_T tecs_mode;
  uint8_T CoG_tracking;
} busStateMachine;

#endif
#endif                                 /* system_state_machine_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
