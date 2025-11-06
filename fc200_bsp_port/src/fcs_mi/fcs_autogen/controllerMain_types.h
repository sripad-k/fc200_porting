/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controllerMain_types.h
 *
 * Code generated for Simulink model 'controllerMain'.
 *
 * Model version                  : 3.599
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:53:18 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef controllerMain_types_h_
#define controllerMain_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_pic_t_
#define DEFINED_TYPEDEF_FOR_pic_t_

typedef enum {
  EXTERNAL = 0,                        /* Default value */
  INTERNAL = 1,
  PIC_INVALID = 255
} pic_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_safety_state_t_
#define DEFINED_TYPEDEF_FOR_safety_state_t_

typedef enum {
  DISARMED = 0,                        /* Default value */
  ARMED
} safety_state_t;

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

#ifndef DEFINED_TYPEDEF_FOR_pilot_int_t_
#define DEFINED_TYPEDEF_FOR_pilot_int_t_

typedef struct {
  real_T roll_ch;
  real_T pitch_ch;
  real_T throttle_ch;
  real_T yaw_ch;
  real_T engine_ch;
} pilot_int_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_nav_data_t_
#define DEFINED_TYPEDEF_FOR_nav_data_t_

typedef struct {
  real_T eul_ang[3];
  real_T omg[3];
  real_T acc[3];
  real_T lat;
  real_T lon;
  real_T alt_gps_amsl;
  real_T v_ned[3];
  boolean_T data_timeout;
  boolean_T att_invalid;
  boolean_T omg_invalid;
  boolean_T acc_invalid;
  real_T eph;
  real_T epv;
  boolean_T pos_invalid;
} nav_data_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ads_data_t_
#define DEFINED_TYPEDEF_FOR_ads_data_t_

typedef struct {
  real_T aoa;
  real_T aos;
  real_T aspd_cas;
  real_T alt_baro_amsl;
  boolean_T aoa_invalid;
  boolean_T aos_invalid;
  boolean_T aspd_cas_invalid;
  boolean_T baro_invalid;
} ads_data_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_sensor_t_
#define DEFINED_TYPEDEF_FOR_sensor_t_

typedef struct {
  nav_data_t ins_1;
  nav_data_t ins_2;
  ads_data_t ads_1;
  ads_data_t ads_2;
  real_T h_radar_agl;
  real_T wow[3];
  real_T rotor_rpm[8];
} sensor_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_std_command_t_
#define DEFINED_TYPEDEF_FOR_std_command_t_

typedef struct {
  vom_t vom_cmd;
  uint16_T vom_cmd_cnt;
  pic_t pic_cmd;
  uint16_T pic_cmd_cnt;
  real_T home_pos_lla[3];
  uint8_T home_pos_set;
  real_T airspeed_cas_cmd;
  real_T fwrth_apr_deg;
} std_command_t;

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

#ifndef DEFINED_TYPEDEF_FOR_lifter_state_t_
#define DEFINED_TYPEDEF_FOR_lifter_state_t_

typedef enum {
  OFF = 0,
  ON                                   /* Default value */
} lifter_state_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ext_command_t_
#define DEFINED_TYPEDEF_FOR_ext_command_t_

typedef struct {
  uint8_T loiter_cmd;
  lifter_state_t lifter_cmd;
  uint8_T tecs_cmd;
} ext_command_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_wp_data_t_
#define DEFINED_TYPEDEF_FOR_wp_data_t_

/* This is the output of waypoint database and connects as input to controller structure */
typedef struct {
  /* if there is a valid waypoint list in the wp manager */
  boolean_T wp_list_valid;

  /* number of wp in the wp_list */
  uint16_T wp_list_count;

  /* lat of the current wp output from wp manager */
  real_T cur_wp_lat;

  /* lon of the current wp output from wp manager */
  real_T cur_wp_lon;

  /* alt of the current wp output from wp manager */
  real_T cur_wp_alt;

  /* index of the current wp output from wp manager */
  uint16_T cur_wp_idx;

  /* GCS commanded waypoint index */
  uint16_T cmd_wp_idx;
  boolean_T last_wp_land;
} wp_data_t;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerAltCtrl_
#define DEFINED_TYPEDEF_FOR_busControllerAltCtrl_

typedef struct {
  real_T hDotCmd;
  real_T forceDes;
  real_T hCmd;
  boolean_T hHold;
} busControllerAltCtrl;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerAttCtrl_
#define DEFINED_TYPEDEF_FOR_busControllerAttCtrl_

typedef struct {
  real_T rollCmd;
  real_T pitchCmd;
  real_T yawRateCmd;
  real_T momentDes[3];
} busControllerAttCtrl;

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

#ifndef DEFINED_TYPEDEF_FOR_busIntegratorCF_
#define DEFINED_TYPEDEF_FOR_busIntegratorCF_

/* Integrator crossfeed for Roll and Pitch controller */
typedef struct {
  real_T roll_CF;
  real_T pitch_CF;
  real_T lambda;
} busIntegratorCF;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busWaypointManager_
#define DEFINED_TYPEDEF_FOR_busWaypointManager_

typedef struct {
  real_T posLLA[3];
  real_T yawCmd;
} busWaypointManager;

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

#ifndef DEFINED_TYPEDEF_FOR_busControllerVelCtrl_
#define DEFINED_TYPEDEF_FOR_busControllerVelCtrl_

typedef struct {
  real_T velCmd[2];
  real_T velRef[2];
  real_T vel_xy_N[2];
} busControllerVelCtrl;

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

#ifndef DEFINED_TYPEDEF_FOR_busSensorManager_
#define DEFINED_TYPEDEF_FOR_busSensorManager_

typedef struct {
  std_sensor_t static_sensor_voting_out;
  real_T rad_alt_out;
  boolean_T rad_alt_spike;
  real_T airspeedCAS_filt;
} busSensorManager;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busIntegrator_
#define DEFINED_TYPEDEF_FOR_busIntegrator_

/* Integrator crossfeed input data for roll and pitch */
typedef struct {
  real_T roll_int;
  real_T pitch_int;
  real_T roll_sat;
  real_T pitch_sat;
} busIntegrator;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busIntegratorCFLog_
#define DEFINED_TYPEDEF_FOR_busIntegratorCFLog_

/* Integrator cross-feed data for logging. */
typedef struct {
  busIntegrator MR_IntData;
  busIntegrator FW_IntData;
  busIntegratorCF MR_IntCF;
  busIntegratorCF FW_IntCF;
  real_T lambda;
} busIntegratorCFLog;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busControllerFwAtt_
#define DEFINED_TYPEDEF_FOR_busControllerFwAtt_

typedef struct {
  real_T pitchCmd;
  real_T rollCmd;
  real_T raw_pitchCmd;
  real_T raw_rollCmd;
} busControllerFwAtt;

#endif

#ifndef DEFINED_TYPEDEF_FOR_busController_
#define DEFINED_TYPEDEF_FOR_busController_

typedef struct {
  busControllerCA controllerCA;
  busControllerAttCtrl controllerAttCtrl;
  busControllerAltCtrl controllerAltCtrl;
  busControllerVelCtrl controllerVelCtrl;
  busControllerPosCtrl controllerPosCtrl;
  busControllerTECS controllerTECS;
  busStateMachine stateMachine;
  busControllerIF_att controllerIF_att;
  busSensorManager SensorMgmt;
  busIntegratorCFLog IntegratorCF;
  busControllerFwAtt fwAttCmd;
} busController;

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
#endif                                 /* controllerMain_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
