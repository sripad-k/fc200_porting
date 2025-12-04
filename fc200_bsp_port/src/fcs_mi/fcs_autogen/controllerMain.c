/*
 * File: controllerMain.c
 *
 * Code generated for Simulink model 'controllerMain'.
 *
 * Model version                  : 3.618
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:43 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "controllerMain.h"
#include <string.h>
#include <math.h>
#include "rtwtypes.h"
#include "controllerMain_types.h"
#include "look1_plinlc.h"
#include "system_state_machine.h"
#include "mc_path_planner.h"
#include "posCtrl.h"
#include "vel_ctrl_switcher.h"
#include "altCtrl.h"
#include "velCtrl.h"
#include "FW_TECS_switcher.h"
#include "FW_latGuidance.h"
#include "FW_TECS.h"
#include "att_ctrl_switcher.h"
#include "attCtrl.h"
#include "MR_CA.h"
#include "FW_attCtrl_switcher.h"
#include "FW_attCtrl.h"
#include "FW_throttle.h"
#include "FW_CA.h"
#include "transitionHandler.h"

const busMode_data controllerMain_rtZbusMode_data = {
  {
    false,                             /* startup_engineARMFlag */
    false                              /* startup_motorARMFlag */
  },                                   /* startup_data */

  {
    0.0,                               /* hover_y */
    0.0,                               /* hover_x */
    0.0,                               /* hover_alt_agl */
    0.0,                               /* hover_yaw_ref */
    false                              /* hover_yaw_override */
  },                                   /* hover_data */

  {
    0.0,                               /* takeoff_x */
    0.0,                               /* takeoff_y */
    0.0,                               /* takeoff_alt_agl */
    0.0                                /* takeoff_yaw_ref */
  },                                   /* autotakeoff_data */

  {
    0.0,                               /* land_x */
    0.0,                               /* land_y */
    0.0                                /* land_yaw_ref */
  },                                   /* autoland_data */

  {
    0.0,                               /* rth_x */
    0.0,                               /* rth_y */
    0.0,                               /* rth_velX */
    0.0,                               /* rth_velY */
    0.0,                               /* rth_alt_agl */
    0.0,                               /* rth_yaw_ref */
    false,                             /* bRTH_PosModeFlag */
    false                              /* bBreaking */
  },                                   /* rth_data */

  {
    0.0,                               /* FT_x */
    0.0,                               /* FT_y */
    0.0,                               /* FT_Altitude */
    0.0,                               /* FT_Heading */
    0.0,                               /* FT_AirspeedRef */
    0.0,                               /* FT_CoG */
    0.0,                               /* FT_PusherThrottle */
    false,                             /* bFT_AttitudeModeFlag */
    0.0,                               /* FT_PitchCmd */
    0.0,                               /* FT_RollCmd */
    ON                                 /* eLifter_Mode */
  },                                   /* ft_data */

  {
    0.0,                               /* FD_CoG */
    0.0,                               /* FD_AirspeedRef */
    0.0,                               /* FD_Altitude */
    0.0                                /* FTransition_y */
  },                                   /* flightDirector_data */

  {
    0.0,                               /* loiter_radius */
    0.0,                               /* loiter_direction */
    0.0,                               /* loiter_Center_Lat */
    0.0,                               /* loiter_Center_Lon */
    0.0,                               /* loiter_altitude */
    0.0                                /* loiter_AirSpeedRef */
  },                                   /* loiter_data */

  {
    0.0,                               /* BT_Hover_Lat */
    0.0,                               /* BT_Hover_Lon */
    0.0,                               /* BT_Altitude */
    0.0,                               /* BT_Heading */
    0.0,                               /* BT_PusherThrottle */
    false,                             /* bBT_AttitudeModeFlag */
    0.0,                               /* BT_PitchCmd */
    0.0,                               /* BT_RollCmd */
    ON                                 /* eLifter_Mode */
  },                                   /* bt_data */
  ON,                                  /* eFWLifter_Mode */

  {
    approach_circle,                   /* phase */
    0.0,                               /* CAS_sp */
    0.0                                /* alt_sp */
  }                                    /* fwrth_data */
};                                     /* busMode_data ground */

/* Block signals (default storage) */
controllerMain_n4wy4x2kbb_T controllerMain_B;

/* Block states (default storage) */
controllerMain_l5cfqqw4qz_T controllerMain_DW;

/* External inputs (root inport signals with default storage) */
controllerMain_csevfv4opo_T controllerMain_U;

/* External outputs (root outports fed by signals with default storage) */
controllerMain_jzldyrnxon_T controllerMain_Y;

/* Model step function */
void controllerMain_step(void)
{
  /* local block i/o variables */
  busControllerCA rtb_MR_CA_ref;
  busControllerCA rtb_Model8;
  busControllerTECS rtb_Model6;
  busControllerIF_att rtb_Model2_o1;
  busControllerAttCtrl rtb_Model1_o1;
  busFW_TECS_switcher rtb_FW_TECS_switcher_modelref_o1;
  busFW_Controller_att rtb_Model12;
  busControllerAltCtrl rtb_altCtrl_ref;
  real_T rtb_velCmdH_x;
  real_T rtb_velCmdH_y;
  real_T rtb_hRateCmd;
  real_T rtb_hCmd;
  real_T rtb_velCmdH_x_plbsly3dk1;
  real_T rtb_velCmdH_y_fn5sqdulrh;
  real_T rtb_hRateCmd_n0d5yq2y1h;
  real_T rtb_hCmd_f0mpth4u0h;
  real_T rtb_land_lat_fbnpixa3xe;
  real_T rtb_land_lon_fuvjv0mdij;
  real_T rtb_approach_ang_baxqfjn0jo;
  real_T rtb_cur_leg_heading_priipbumn4;
  real_T rtb_cur_leg_length_px1a0fpeys;
  real_T rtb_cur_leg_remaining_dist_htjoyldznw;
  real_T rtb_land_wp_lat_oe3el12bfw;
  real_T rtb_land_wp_lon_jm2bgprjv1;
  real_T rtb_curpos_to_wp_heading_cisgujy5vn;
  real_T rtb_rollCmd;
  real_T rtb_pitchCmd;
  real_T rtb_yawCmd;
  real_T rtb_yawRateCmd;
  real_T rtb_yawFF;
  real_T rtb_roll_int_n0n0i3aced;
  real_T rtb_pitch_int_b2jgrf5nof;
  real_T rtb_roll_sat_cnnujar2qf;
  real_T rtb_pitch_sat_dxrqxisnco;
  real_T rtb_roll_int_mtg3gjguro;
  real_T rtb_pitch_int_fkznhgwxng;
  real_T rtb_roll_sat_cvg42to5m1;
  real_T rtb_pitch_sat_hd1k1f2mpb;
  boolean_T rtb_hHold;
  boolean_T rtb_init_reset_vel;
  boolean_T rtb_init_reset_hdot;
  boolean_T rtb_hHold_kkfp1ixkpi;
  boolean_T rtb_init_reset_vel_oyuizswpb3;
  boolean_T rtb_init_reset_hdot_lay4g5k1gs;
  boolean_T rtb_approach_circle_done_hqf5zegy2u;
  boolean_T rtb_reduce_speed_alt_done_bd02ecbta3;
  boolean_T rtb_circle_align_done_ix4pkr1ylq;
  boolean_T rtb_cross_tangent_point_done_mtl0vknyin;
  boolean_T rtb_wp_list_valid_noeol5rznd;
  boolean_T rtb_last_wp_land_oojmz2hxa2;
  boolean_T rtb_WPN_cmd_received_cnz0eax2ri;
  boolean_T rtb_yawHold;
  boolean_T rtb_init_reset;
  busControllerPosCtrl rtb_Model3_o1;
  busControllerVelCtrl rtb_Model2_o2;
  busTECS_Data_Processing
    rtb_BusConversion_InsertedFor_Model6_at_inport_8_BusCreator1;
  busWaypointManager
    rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1;
  std_ctrl_t rtb_Model11_o1;
  real_T rtb_BusAssignment1_rotor_rpm[8];
  real_T notch0_acc1_tmp[3];
  real_T notch0_acc_tmp[3];
  real_T notch0_gyro1_tmp[3];
  real_T notch0_gyro_tmp[3];
  real_T rtb_BusAssignment1_ins_1_acc[3];
  real_T rtb_BusAssignment1_ins_1_omg[3];
  real_T notch0_gyro_states;
  real_T rtb_BusAssignment1_ads_1_alt_baro_amsl_tmp;
  real_T rtb_BusAssignment1_ads_1_aoa_tmp;
  real_T rtb_BusAssignment1_ads_1_aos_tmp;
  real_T rtb_BusAssignment1_ads_1_aspd_cas;
  real_T rtb_BusAssignment1_cvb352wjyj_gspd;
  real_T rtb_BusAssignment1_h_radar_agl_tmp;
  real_T rtb_BusAssignment1_ins_1_alt_gps_amsl_tmp;
  real_T rtb_BusAssignment1_ins_1_eul_ang_idx_0;
  real_T rtb_BusAssignment1_ins_1_eul_ang_idx_1;
  real_T rtb_BusAssignment1_ins_1_eul_ang_idx_2;
  real_T rtb_BusAssignment1_ins_1_v_ned_idx_0;
  real_T rtb_BusAssignment1_ins_1_v_ned_idx_1;
  real_T rtb_BusAssignment1_ins_1_v_ned_idx_2;
  real_T rtb_BusAssignment1_nc41wq0lh0_pitch_CF;
  real_T rtb_BusAssignment1_wow_idx_0;
  real_T rtb_BusAssignment1_wow_idx_1;
  real_T rtb_BusAssignment1_wow_idx_2;
  real_T rtb_Model4_o1;
  real_T rtb_RateLimiter;
  real_T rtb_RateLimiter_kbd0fqq3vq;
  real_T rtb_Switch_di0iual0gm;
  real_T rtb_UkYk1;
  real_T rtb_approach_ang;
  real_T rtb_cur_leg_heading;
  real_T rtb_cur_leg_length;
  real_T rtb_cur_leg_remaining_dist;
  real_T rtb_curpos_to_wp_heading;
  real_T rtb_deltafalllimit;
  real_T rtb_engine_ch;
  real_T rtb_engine_ch_bjavj5sodg;
  real_T rtb_land_wp_lat;
  real_T rtb_land_wp_lon;
  real_T rtb_omega_jjutubfppl;
  real_T rtb_pitch_ch;
  real_T rtb_pitch_ch_i1x0haaskm;
  real_T rtb_roll_ch;
  real_T rtb_roll_cmd;
  real_T rtb_throttle_ch;
  real_T rtb_throttle_ch_oryunzuyq0;
  real_T rtb_uDLookupTable;
  real_T rtb_va_filt;
  real_T rtb_yaw_ch;
  real_T rtb_yaw_ch_jthfpygiuk;
  int32_T i;
  int32_T memOffset_tmp;
  uint8_T rtb_CoG_tracking;
  uint8_T rtb_inAir_flag;
  uint8_T rtb_lifter_mode;
  uint8_T rtb_loiter_mode;
  uint8_T rtb_pic_status;
  uint8_T rtb_tecs_mode;
  boolean_T rtb_Compare_lvr1zt3dde;
  boolean_T rtb_Delay5;
  boolean_T rtb_WPN_cmd_received;
  boolean_T rtb_circle_align_done;
  boolean_T rtb_cross_tangent_point_done;
  boolean_T rtb_last_wp_land;
  boolean_T rtb_reduce_speed_alt_done;
  boolean_T rtb_wp_list_valid;
  safety_state_t rtb_safety_state;
  vom_t rtb_vom_status_ofjzuvdpj2;

  /* SignalConversion generated from: '<S27>/Bus Assignment1' incorporates:
   *  BusAssignment: '<S27>/Bus Assignment1'
   *  Inport: '<Root>/sensor_in'
   */
  rtb_BusAssignment1_ins_1_alt_gps_amsl_tmp =
    controllerMain_U.sensor_in.ins_1.alt_gps_amsl;
  rtb_BusAssignment1_ads_1_aoa_tmp = controllerMain_U.sensor_in.ads_1.aoa;
  rtb_BusAssignment1_ads_1_aos_tmp = controllerMain_U.sensor_in.ads_1.aos;
  rtb_BusAssignment1_ads_1_aspd_cas = controllerMain_U.sensor_in.ads_1.aspd_cas;
  rtb_BusAssignment1_ads_1_alt_baro_amsl_tmp =
    controllerMain_U.sensor_in.ads_1.alt_baro_amsl;
  rtb_BusAssignment1_h_radar_agl_tmp = controllerMain_U.sensor_in.h_radar_agl;
  rtb_BusAssignment1_ins_1_eul_ang_idx_0 =
    controllerMain_U.sensor_in.ins_1.eul_ang[0];
  rtb_BusAssignment1_ins_1_v_ned_idx_0 = controllerMain_U.sensor_in.ins_1.v_ned
    [0];
  rtb_BusAssignment1_wow_idx_0 = controllerMain_U.sensor_in.wow[0];
  rtb_BusAssignment1_ins_1_eul_ang_idx_1 =
    controllerMain_U.sensor_in.ins_1.eul_ang[1];
  rtb_BusAssignment1_ins_1_v_ned_idx_1 = controllerMain_U.sensor_in.ins_1.v_ned
    [1];
  rtb_BusAssignment1_wow_idx_1 = controllerMain_U.sensor_in.wow[1];
  rtb_BusAssignment1_ins_1_eul_ang_idx_2 =
    controllerMain_U.sensor_in.ins_1.eul_ang[2];
  rtb_BusAssignment1_ins_1_v_ned_idx_2 = controllerMain_U.sensor_in.ins_1.v_ned
    [2];
  rtb_BusAssignment1_wow_idx_2 = controllerMain_U.sensor_in.wow[2];
  (void)memcpy(&rtb_BusAssignment1_rotor_rpm[0],
               &controllerMain_U.sensor_in.rotor_rpm[0], (sizeof(real_T)) <<
               ((uint32_T)3U));
  for (i = 0; i < 3; i++) {
    /* DiscreteFilter: '<S27>/notch0_gyro' incorporates:
     *  BusAssignment: '<S27>/Bus Assignment1'
     *  DiscreteFilter: '<S27>/notch0_acc'
     *  DiscreteFilter: '<S27>/notch0_acc1'
     *  DiscreteFilter: '<S27>/notch0_gyro1'
     *  Inport: '<Root>/sensor_in'
     */
    memOffset_tmp = i * 2;
    notch0_gyro_states = controllerMain_DW.notch0_gyro_states[memOffset_tmp];
    rtb_va_filt = controllerMain_DW.notch0_gyro_states[memOffset_tmp + 1];
    rtb_BusAssignment1_cvb352wjyj_gspd = (controllerMain_U.sensor_in.ins_1.omg[i]
      - (-1.5843030311528565 * notch0_gyro_states)) - (0.92439049165820752 *
      rtb_va_filt);
    notch0_gyro_tmp[i] = rtb_BusAssignment1_cvb352wjyj_gspd;
    rtb_BusAssignment1_ins_1_omg[i] = ((0.96219524582910376 *
      rtb_BusAssignment1_cvb352wjyj_gspd) + (-1.5843030311528565 *
      notch0_gyro_states)) + (0.96219524582910376 * rtb_va_filt);

    /* DiscreteFilter: '<S27>/notch0_acc' incorporates:
     *  BusAssignment: '<S27>/Bus Assignment1'
     *  Inport: '<Root>/sensor_in'
     */
    notch0_gyro_states = controllerMain_DW.notch0_acc_states[memOffset_tmp];
    rtb_va_filt = controllerMain_DW.notch0_acc_states[memOffset_tmp + 1];
    rtb_BusAssignment1_cvb352wjyj_gspd = (controllerMain_U.sensor_in.ins_1.acc[i]
      - (-1.5843030311528565 * notch0_gyro_states)) - (0.92439049165820752 *
      rtb_va_filt);
    notch0_acc_tmp[i] = rtb_BusAssignment1_cvb352wjyj_gspd;
    rtb_BusAssignment1_ins_1_acc[i] = ((0.96219524582910376 *
      rtb_BusAssignment1_cvb352wjyj_gspd) + (-1.5843030311528565 *
      notch0_gyro_states)) + (0.96219524582910376 * rtb_va_filt);

    /* DiscreteFilter: '<S27>/notch0_gyro1' incorporates:
     *  Inport: '<Root>/sensor_in'
     */
    notch0_gyro1_tmp[i] = (controllerMain_U.sensor_in.ins_2.omg[i] -
      (-1.5843030311528565 * controllerMain_DW.notch0_gyro1_states[memOffset_tmp]))
      - (0.92439049165820752 *
         controllerMain_DW.notch0_gyro1_states[memOffset_tmp + 1]);

    /* DiscreteFilter: '<S27>/notch0_acc1' incorporates:
     *  Inport: '<Root>/sensor_in'
     */
    notch0_acc1_tmp[i] = (controllerMain_U.sensor_in.ins_2.acc[i] -
                          (-1.5843030311528565 *
      controllerMain_DW.notch0_acc1_states[memOffset_tmp])) -
      (0.92439049165820752 * controllerMain_DW.notch0_acc1_states[memOffset_tmp
       + 1]);
  }

  /* SignalConversion generated from: '<S27>/Bus Assignment1' incorporates:
   *  Inport: '<Root>/sensor_in'
   */
  rtb_BusAssignment1_cvb352wjyj_gspd = controllerMain_U.sensor_in.ins_1.lat;

  /* SignalConversion generated from: '<S28>/Vector Concatenate' incorporates:
   *  Inport: '<Root>/sensor_in'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   */
  rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.posLLA[0] =
    controllerMain_U.sensor_in.ins_1.lat;

  /* SignalConversion generated from: '<S27>/Bus Assignment1' incorporates:
   *  Inport: '<Root>/sensor_in'
   */
  notch0_gyro_states = controllerMain_U.sensor_in.ins_1.lon;

  /* SignalConversion generated from: '<S28>/Vector Concatenate' incorporates:
   *  Inport: '<Root>/sensor_in'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   */
  rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.posLLA[1] =
    controllerMain_U.sensor_in.ins_1.lon;

  /* SignalConversion generated from: '<S28>/Vector Concatenate' incorporates:
   *  Inport: '<Root>/sensor_in'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   */
  rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.posLLA[2] =
    controllerMain_U.sensor_in.ins_1.alt_gps_amsl;

  /* Memory: '<S24>/Memory' */
  rtb_va_filt = controllerMain_DW.Memory_PreviousInput;

  /* RateLimiter: '<S4>/Rate Limiter' incorporates:
   *  Inport: '<Root>/sensor_in'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   */
  rtb_RateLimiter = controllerMain_U.sensor_in.h_radar_agl -
    controllerMain_DW.PrevY;
  if (rtb_RateLimiter > 0.06) {
    rtb_RateLimiter = controllerMain_DW.PrevY + 0.06;
  } else if (rtb_RateLimiter < -0.06) {
    rtb_RateLimiter = controllerMain_DW.PrevY - 0.06;
  } else {
    rtb_RateLimiter = controllerMain_U.sensor_in.h_radar_agl;
  }

  controllerMain_DW.PrevY = rtb_RateLimiter;

  /* End of RateLimiter: '<S4>/Rate Limiter' */

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S30>/Constant'
   *  Constant: '<S31>/Constant'
   *  Constant: '<S32>/Constant'
   *  Inport: '<Root>/sensor_in'
   *  Logic: '<S23>/OR'
   *  RelationalOperator: '<S30>/Compare'
   *  RelationalOperator: '<S31>/Compare'
   *  RelationalOperator: '<S32>/Compare'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   *  UnitDelay: '<Root>/Unit Delay1'
   */
  if (((controllerMain_DW.UnitDelay1_DSTATE == VOM_TAKEOFF) ||
       (controllerMain_DW.UnitDelay1_DSTATE == VOM_LAND)) ||
      (controllerMain_DW.UnitDelay1_DSTATE == VOM_MANUAL)) {
    rtb_Switch_di0iual0gm = rtb_RateLimiter;
  } else {
    rtb_Switch_di0iual0gm = controllerMain_U.sensor_in.h_radar_agl;
  }

  /* End of Switch: '<S4>/Switch' */

  /* BusAssignment: '<S4>/Bus Assignment' incorporates:
   *  BusAssignment: '<S27>/Bus Assignment1'
   *  BusAssignment: '<S28>/Bus Assignment1'
   *  Inport: '<Root>/sensor_in'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   */
  (void)memset(&controllerMain_B.BusAssignment.dcm_e2b[0], 0, 9U * (sizeof
    (real_T)));
  controllerMain_B.BusAssignment.aoa = controllerMain_U.sensor_in.ads_1.aoa;
  controllerMain_B.BusAssignment.aos = controllerMain_U.sensor_in.ads_1.aos;
  controllerMain_B.BusAssignment.h_gps_amsl =
    controllerMain_U.sensor_in.ins_1.alt_gps_amsl;
  controllerMain_B.BusAssignment.h_baro_amsl =
    controllerMain_U.sensor_in.ads_1.alt_baro_amsl;
  controllerMain_B.BusAssignment.eul_ang[0] =
    controllerMain_U.sensor_in.ins_1.eul_ang[0];
  controllerMain_B.BusAssignment.omg[0] = rtb_BusAssignment1_ins_1_omg[0];
  controllerMain_B.BusAssignment.pos_lla[0] =
    controllerMain_U.sensor_in.ins_1.lat;
  controllerMain_B.BusAssignment.vel_ned[0] =
    controllerMain_U.sensor_in.ins_1.v_ned[0];
  controllerMain_B.BusAssignment.accel_b[0] = rtb_BusAssignment1_ins_1_acc[0];
  controllerMain_B.BusAssignment.wow[0] = controllerMain_U.sensor_in.wow[0];
  controllerMain_B.BusAssignment.eul_ang[1] =
    controllerMain_U.sensor_in.ins_1.eul_ang[1];
  controllerMain_B.BusAssignment.omg[1] = rtb_BusAssignment1_ins_1_omg[1];
  controllerMain_B.BusAssignment.pos_lla[1] =
    controllerMain_U.sensor_in.ins_1.lon;
  controllerMain_B.BusAssignment.vel_ned[1] =
    controllerMain_U.sensor_in.ins_1.v_ned[1];
  controllerMain_B.BusAssignment.accel_b[1] = rtb_BusAssignment1_ins_1_acc[1];
  controllerMain_B.BusAssignment.wow[1] = controllerMain_U.sensor_in.wow[1];
  controllerMain_B.BusAssignment.eul_ang[2] =
    controllerMain_U.sensor_in.ins_1.eul_ang[2];
  controllerMain_B.BusAssignment.omg[2] = rtb_BusAssignment1_ins_1_omg[2];
  controllerMain_B.BusAssignment.pos_lla[2] =
    controllerMain_U.sensor_in.ins_1.alt_gps_amsl;
  controllerMain_B.BusAssignment.vel_ned[2] =
    controllerMain_U.sensor_in.ins_1.v_ned[2];
  controllerMain_B.BusAssignment.accel_b[2] = rtb_BusAssignment1_ins_1_acc[2];
  controllerMain_B.BusAssignment.wow[2] = controllerMain_U.sensor_in.wow[2];
  (void)memcpy(&controllerMain_B.BusAssignment.rotor_rpm[0],
               &controllerMain_U.sensor_in.rotor_rpm[0], (sizeof(real_T)) <<
               ((uint32_T)3U));

  /* Sqrt: '<S4>/Sqrt' incorporates:
   *  Inport: '<Root>/sensor_in'
   *  Product: '<S4>/Product'
   *  Product: '<S4>/Product1'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   *  Sum: '<S4>/Add'
   */
  rtb_roll_ch = sqrt((controllerMain_U.sensor_in.ins_1.v_ned[0] *
                      controllerMain_U.sensor_in.ins_1.v_ned[0]) +
                     (controllerMain_U.sensor_in.ins_1.v_ned[1] *
                      controllerMain_U.sensor_in.ins_1.v_ned[1]));

  /* Saturate: '<S4>/Saturation' */
  if (rtb_roll_ch > 300.0) {
    /* BusAssignment: '<S4>/Bus Assignment' */
    controllerMain_B.BusAssignment.gspd = 300.0;
  } else if (rtb_roll_ch < 0.01) {
    /* BusAssignment: '<S4>/Bus Assignment' */
    controllerMain_B.BusAssignment.gspd = 0.01;
  } else {
    /* BusAssignment: '<S4>/Bus Assignment' */
    controllerMain_B.BusAssignment.gspd = rtb_roll_ch;
  }

  /* End of Saturate: '<S4>/Saturation' */

  /* BusAssignment: '<S4>/Bus Assignment' incorporates:
   *  Inport: '<Root>/sensor_in'
   *  Memory: '<S24>/Memory'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   *  Trigonometry: '<S4>/Atan2'
   */
  controllerMain_B.BusAssignment.chi = atan2
    (controllerMain_U.sensor_in.ins_1.v_ned[1],
     controllerMain_U.sensor_in.ins_1.v_ned[0]);
  controllerMain_B.BusAssignment.aspd_cas =
    controllerMain_DW.Memory_PreviousInput;
  controllerMain_B.BusAssignment.h_radar_agl = rtb_Switch_di0iual0gm;

  /* RelationalOperator: '<S15>/Compare' incorporates:
   *  Constant: '<S15>/Constant'
   *  UnitDelay: '<Root>/Unit Delay2'
   */
  rtb_Compare_lvr1zt3dde = (controllerMain_DW.UnitDelay2_DSTATE == EXTERNAL);

  /* Switch generated from: '<S16>/Switch' incorporates:
   *  Inport: '<Root>/pilot_ext'
   *  UnitDelay generated from: '<S16>/ud'
   */
  if (rtb_Compare_lvr1zt3dde) {
    rtb_roll_ch = controllerMain_U.pilot_ext.roll_ch;
    rtb_pitch_ch = controllerMain_U.pilot_ext.pitch_ch;
    rtb_yaw_ch = controllerMain_U.pilot_ext.yaw_ch;
    rtb_throttle_ch = controllerMain_U.pilot_ext.throttle_ch;
    rtb_engine_ch = controllerMain_U.pilot_ext.engine_ch;
  } else {
    rtb_roll_ch = controllerMain_DW.ud_1_DSTATE;
    rtb_pitch_ch = controllerMain_DW.ud_2_DSTATE;
    rtb_yaw_ch = controllerMain_DW.ud_3_DSTATE;
    rtb_throttle_ch = controllerMain_DW.ud_4_DSTATE;
    rtb_engine_ch = controllerMain_DW.ud_5_DSTATE;
  }

  /* End of Switch generated from: '<S16>/Switch' */

  /* Product: '<S17>/delta rise limit' incorporates:
   *  Constant: '<S16>/Constant1'
   *  SampleTimeMath: '<S17>/sample time'
   *
   * About '<S17>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  rtb_omega_jjutubfppl = 0.01;

  /* Switch: '<S20>/Init' incorporates:
   *  DataTypeConversion: '<S16>/Data Type Conversion1'
   *  DataTypeConversion: '<S17>/Data Type Conversion'
   *  UnitDelay: '<S20>/FixPt Unit Delay1'
   *  UnitDelay: '<S20>/FixPt Unit Delay2'
   */
  if (controllerMain_DW.FixPtUnitDelay2_DSTATE != ((uint8_T)0)) {
    rtb_uDLookupTable = rtb_Compare_lvr1zt3dde ? 1.0 : 0.0;
  } else {
    rtb_uDLookupTable = controllerMain_DW.FixPtUnitDelay1_DSTATE;
  }

  /* End of Switch: '<S20>/Init' */

  /* Sum: '<S17>/Difference Inputs1' incorporates:
   *  DataTypeConversion: '<S16>/Data Type Conversion1'
   *
   * Block description for '<S17>/Difference Inputs1':
   *
   *  Add in CPU
   */
  rtb_UkYk1 = ((real_T)(rtb_Compare_lvr1zt3dde ? 1.0 : 0.0)) - rtb_uDLookupTable;

  /* Switch: '<S19>/Switch2' incorporates:
   *  RelationalOperator: '<S19>/LowerRelop1'
   */
  if (rtb_UkYk1 <= 0.01) {
    /* Product: '<S17>/delta fall limit' incorporates:
     *  SampleTimeMath: '<S17>/sample time'
     *
     * About '<S17>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    rtb_deltafalllimit = 0.01 * controllerMain_ConstB.Gain;

    /* Switch: '<S19>/Switch' incorporates:
     *  RelationalOperator: '<S19>/UpperRelop'
     */
    if (rtb_UkYk1 < rtb_deltafalllimit) {
      rtb_omega_jjutubfppl = rtb_deltafalllimit;
    } else {
      rtb_omega_jjutubfppl = rtb_UkYk1;
    }

    /* End of Switch: '<S19>/Switch' */
  }

  /* End of Switch: '<S19>/Switch2' */

  /* Sum: '<S17>/Difference Inputs2'
   *
   * Block description for '<S17>/Difference Inputs2':
   *
   *  Add in CPU
   */
  rtb_UkYk1 = rtb_omega_jjutubfppl + rtb_uDLookupTable;

  /* BusCreator generated from: '<Root>/Model3' incorporates:
   *  Fcn: '<S16>/Fcn'
   */
  rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.yawCmd = 1.0 -
    rtb_UkYk1;

  /* Switch generated from: '<S16>/Switch1' incorporates:
   *  Inport: '<Root>/pilot_int'
   *  UnitDelay generated from: '<S16>/ud1'
   */
  if (rtb_Compare_lvr1zt3dde) {
    rtb_deltafalllimit = controllerMain_DW.ud1_1_DSTATE;
    rtb_pitch_ch_i1x0haaskm = controllerMain_DW.ud1_2_DSTATE;
    rtb_yaw_ch_jthfpygiuk = controllerMain_DW.ud1_3_DSTATE;
    rtb_throttle_ch_oryunzuyq0 = controllerMain_DW.ud1_4_DSTATE;
    rtb_engine_ch_bjavj5sodg = controllerMain_DW.ud1_5_DSTATE;
  } else {
    rtb_deltafalllimit = controllerMain_U.pilot_int.roll_ch;
    rtb_pitch_ch_i1x0haaskm = controllerMain_U.pilot_int.pitch_ch;
    rtb_yaw_ch_jthfpygiuk = controllerMain_U.pilot_int.yaw_ch;
    rtb_throttle_ch_oryunzuyq0 = controllerMain_U.pilot_int.throttle_ch;
    rtb_engine_ch_bjavj5sodg = controllerMain_U.pilot_int.engine_ch;
  }

  /* End of Switch generated from: '<S16>/Switch1' */

  /* BusAssignment: '<S2>/Bus Assignment' incorporates:
   *  Inport: '<Root>/pilot_ext'
   */
  controllerMain_B.BusAssignment_b4zvdvf4m4 = controllerMain_U.pilot_ext;

  /* BusAssignment: '<S2>/Bus Assignment' incorporates:
   *  Fcn: '<S16>/Fcn'
   *  Product: '<S16>/Divide'
   *  Product: '<S16>/Divide1'
   *  Sum: '<S16>/Add'
   *  Switch generated from: '<S16>/Switch2'
   *  Switch generated from: '<S16>/Switch4'
   */
  controllerMain_B.BusAssignment_b4zvdvf4m4.roll_ch = (rtb_roll_ch * rtb_UkYk1)
    + ((1.0 - rtb_UkYk1) * rtb_deltafalllimit);
  controllerMain_B.BusAssignment_b4zvdvf4m4.pitch_ch = (rtb_pitch_ch * rtb_UkYk1)
    + ((1.0 - rtb_UkYk1) * rtb_pitch_ch_i1x0haaskm);
  controllerMain_B.BusAssignment_b4zvdvf4m4.yaw_ch = (rtb_yaw_ch * rtb_UkYk1) +
    ((1.0 - rtb_UkYk1) * rtb_yaw_ch_jthfpygiuk);
  controllerMain_B.BusAssignment_b4zvdvf4m4.throttle_ch = (rtb_throttle_ch *
    rtb_UkYk1) + ((1.0 - rtb_UkYk1) * rtb_throttle_ch_oryunzuyq0);
  controllerMain_B.BusAssignment_b4zvdvf4m4.engine_ch = (rtb_engine_ch *
    rtb_UkYk1) + ((1.0 - rtb_UkYk1) * rtb_engine_ch_bjavj5sodg);

  /* UnitDelay: '<Root>/Unit Delay' */
  controllerMain_B.UnitDelay = controllerMain_DW.UnitDelay_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_Compare_lvr1zt3dde = controllerMain_DW.UnitDelay3_1_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_reduce_speed_alt_done = controllerMain_DW.UnitDelay3_2_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_circle_align_done = controllerMain_DW.UnitDelay3_3_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_cross_tangent_point_done = controllerMain_DW.UnitDelay3_4_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_omega_jjutubfppl = controllerMain_DW.UnitDelay3_5_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_uDLookupTable = controllerMain_DW.UnitDelay3_6_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay3' */
  rtb_approach_ang = controllerMain_DW.UnitDelay3_7_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_cur_leg_heading = controllerMain_DW.UnitDelay4_1_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_cur_leg_length = controllerMain_DW.UnitDelay4_2_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_cur_leg_remaining_dist = controllerMain_DW.UnitDelay4_3_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_wp_list_valid = controllerMain_DW.UnitDelay4_4_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_last_wp_land = controllerMain_DW.UnitDelay4_5_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_land_wp_lat = controllerMain_DW.UnitDelay4_6_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_land_wp_lon = controllerMain_DW.UnitDelay4_7_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_curpos_to_wp_heading = controllerMain_DW.UnitDelay4_8_DSTATE;

  /* UnitDelay generated from: '<Root>/Unit Delay4' */
  rtb_WPN_cmd_received = controllerMain_DW.UnitDelay4_9_DSTATE;

  /* ModelReference generated from: '<Root>/system_state_machine' incorporates:
   *  Inport: '<Root>/extd_cmd'
   *  Inport: '<Root>/failure_flags'
   *  Inport: '<Root>/std_command'
   *  Inport: '<Root>/wp_data'
   */
  system_state_machine(&controllerMain_U.std_command.vom_cmd,
                       &controllerMain_U.std_command.vom_cmd_cnt,
                       &controllerMain_U.std_command.pic_cmd,
                       &controllerMain_U.std_command.pic_cmd_cnt,
                       &controllerMain_B.BusAssignment.eul_ang[0],
                       &controllerMain_B.BusAssignment.pos_lla[0],
                       &controllerMain_B.BusAssignment.vel_ned[0],
                       &controllerMain_B.BusAssignment.aspd_cas,
                       &controllerMain_B.BusAssignment.h_radar_agl,
                       &controllerMain_B.BusAssignment.chi,
                       &controllerMain_B.BusAssignment_b4zvdvf4m4.throttle_ch,
                       &controllerMain_B.BusAssignment_b4zvdvf4m4.arm_ch,
                       &controllerMain_B.BusAssignment_b4zvdvf4m4.switch_1,
                       &controllerMain_B.UnitDelay.forceDes,
                       &controllerMain_U.failure_flags.gcs_link_loss,
                       &controllerMain_U.failure_flags.gps_loss,
                       &controllerMain_U.failure_flags.ep_data_loss,
                       &controllerMain_U.failure_flags.ip_data_loss,
                       &controllerMain_U.extd_cmd.loiter_cmd,
                       &controllerMain_U.extd_cmd.loiter_cmd_cnt,
                       &controllerMain_U.extd_cmd.tecs_cmd,
                       &controllerMain_U.extd_cmd.tecs_cmd_cnt,
                       &rtb_Compare_lvr1zt3dde, &rtb_reduce_speed_alt_done,
                       &rtb_circle_align_done, &rtb_cross_tangent_point_done,
                       &rtb_omega_jjutubfppl, &rtb_uDLookupTable,
                       &rtb_approach_ang, &rtb_cur_leg_heading,
                       &rtb_cur_leg_length, &rtb_cur_leg_remaining_dist,
                       &rtb_wp_list_valid, &rtb_last_wp_land, &rtb_land_wp_lat,
                       &rtb_land_wp_lon, &rtb_curpos_to_wp_heading,
                       &rtb_WPN_cmd_received,
                       &controllerMain_U.wp_data.wp_list_valid,
                       &controllerMain_B.bInAirFlag,
                       &controllerMain_B.vom_status, &controllerMain_B.mode_data,
                       &controllerMain_B.bGPSLoss10sec,
                       &controllerMain_B.bGPSLoss,
                       &controllerMain_B.rampup_phase,
                       &controllerMain_B.bTelemtryLinkLoss1sec,
                       &controllerMain_B.ep_loss1sec, &rtb_vom_status_ofjzuvdpj2,
                       &rtb_safety_state, &rtb_inAir_flag, &rtb_pic_status,
                       &rtb_loiter_mode, &rtb_lifter_mode, &rtb_tecs_mode,
                       &rtb_CoG_tracking, &controllerMain_B.safety_state,
                       &controllerMain_B.loiter_mode,
                       &controllerMain_B.TECS_mode,
                       &controllerMain_B.lifter_state,
                       &controllerMain_B.pic_status,
                       &controllerMain_B.CoG_tracking,
                       &controllerMain_B.dCapturedAlt_m);

  /* RelationalOperator: '<S22>/Compare' incorporates:
   *  Constant: '<S22>/Constant'
   */
  rtb_Compare_lvr1zt3dde = (controllerMain_B.safety_state == AC_DISARMED);

  /* BusCreator generated from: '<Root>/Model3' incorporates:
   *  ModelReference generated from: '<Root>/Model7'
   */
  mc_path_planner(&controllerMain_B.BusAssignment.pos_lla[0],
                  &controllerMain_B.vom_status,
                  &controllerMain_B.mode_data.hover_data.hover_y,
                  &controllerMain_B.mode_data.hover_data.hover_x,
                  &controllerMain_B.mode_data.hover_data.hover_alt_agl,
                  &controllerMain_B.mode_data.hover_data.hover_yaw_ref,
                  &controllerMain_B.mode_data.autotakeoff_data.takeoff_x,
                  &controllerMain_B.mode_data.autotakeoff_data.takeoff_y,
                  &controllerMain_B.mode_data.autotakeoff_data.takeoff_alt_agl,
                  &controllerMain_B.mode_data.autotakeoff_data.takeoff_yaw_ref,
                  &controllerMain_B.mode_data.autoland_data.land_x,
                  &controllerMain_B.mode_data.autoland_data.land_y,
                  &controllerMain_B.mode_data.autoland_data.land_yaw_ref,
                  &controllerMain_B.mode_data.rth_data.rth_x,
                  &controllerMain_B.mode_data.rth_data.rth_y,
                  &controllerMain_B.mode_data.rth_data.rth_alt_agl,
                  &controllerMain_B.mode_data.rth_data.rth_yaw_ref,
                  &controllerMain_B.mode_data.ft_data.FT_x,
                  &controllerMain_B.mode_data.ft_data.FT_y,
                  &controllerMain_B.mode_data.ft_data.FT_Altitude,
                  &controllerMain_B.mode_data.ft_data.FT_Heading,
                  &controllerMain_B.mode_data.bt_data.BT_Hover_Lat,
                  &controllerMain_B.mode_data.bt_data.BT_Hover_Lon,
                  &controllerMain_B.mode_data.bt_data.BT_Altitude,
                  &controllerMain_B.mode_data.bt_data.BT_Heading,
                  &rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.posLLA
                  [0],
                  &rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.yawCmd);

  /* ModelReference generated from: '<Root>/Model3' */
  posCtrl(&rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1,
          &controllerMain_B.BusAssignment, &controllerMain_B.vom_status,
          &rtb_Model3_o1, &rtb_velCmdH_x, &rtb_velCmdH_y, &rtb_hRateCmd,
          &rtb_hCmd, &rtb_hHold, &rtb_init_reset_vel, &rtb_init_reset_hdot);

  /* ModelReference generated from: '<Root>/velCtrl_sw_ref' */
  vel_ctrl_switcher(&controllerMain_B.BusAssignment.pos_lla[0],
                    &controllerMain_B.BusAssignment.vel_ned[0],
                    &controllerMain_B.BusAssignment.h_radar_agl,
                    &controllerMain_B.BusAssignment_b4zvdvf4m4, &rtb_velCmdH_x,
                    &rtb_velCmdH_y, &rtb_hRateCmd, &controllerMain_B.vom_status,
                    &controllerMain_B.bGPSLoss10sec, &controllerMain_B.bGPSLoss,
                    &controllerMain_B.mode_data.hover_data.hover_alt_agl,
                    &controllerMain_B.mode_data.autotakeoff_data.takeoff_alt_agl,
                    &controllerMain_B.mode_data.rth_data.rth_velX,
                    &controllerMain_B.mode_data.rth_data.rth_velY,
                    &controllerMain_B.mode_data.rth_data.rth_alt_agl,
                    &controllerMain_B.mode_data.rth_data.bRTH_PosModeFlag,
                    &controllerMain_B.mode_data.ft_data.FT_Altitude,
                    &controllerMain_B.mode_data.bt_data.BT_Altitude,
                    &controllerMain_B.mode_data.eFWLifter_Mode,
                    &controllerMain_B.bInAirFlag, &controllerMain_B.TECS_mode,
                    &controllerMain_B.lifter_state, &rtb_velCmdH_x_plbsly3dk1,
                    &rtb_velCmdH_y_fn5sqdulrh, &rtb_hRateCmd_n0d5yq2y1h,
                    &rtb_hCmd_f0mpth4u0h, &rtb_hHold_kkfp1ixkpi,
                    &rtb_init_reset_vel_oyuizswpb3,
                    &rtb_init_reset_hdot_lay4g5k1gs);

  /* ModelReference generated from: '<Root>/altCtrl_ref' incorporates:
   *  Delay: '<Root>/Delay'
   */
  altCtrl(&rtb_hRateCmd_n0d5yq2y1h, &rtb_hCmd_f0mpth4u0h, &rtb_hHold_kkfp1ixkpi,
          &rtb_init_reset_hdot_lay4g5k1gs, &controllerMain_B.BusAssignment,
          &controllerMain_DW.Delay_DSTATE.c_erp2,
          &controllerMain_DW.Delay_DSTATE.nu_des[0],
          &controllerMain_DW.Delay_DSTATE.nu_allocated[0], &rtb_altCtrl_ref);

  /* ModelReference generated from: '<Root>/Model2' */
  velCtrl(&controllerMain_B.BusAssignment, &rtb_velCmdH_x_plbsly3dk1,
          &rtb_velCmdH_y_fn5sqdulrh, &rtb_init_reset_vel_oyuizswpb3,
          &rtb_Model2_o1, &rtb_Model2_o2);

  /* ModelReference generated from: '<Root>/FW_TECS_switcher_modelref' incorporates:
   *  Inport: '<Root>/std_command'
   *  Inport: '<Root>/wp_data'
   *  Outport: '<Root>/wp_req_idx'
   */
  FW_TECS_switcher(&controllerMain_B.vom_status,
                   &controllerMain_B.BusAssignment_b4zvdvf4m4.roll_ch,
                   &controllerMain_B.BusAssignment_b4zvdvf4m4.throttle_ch,
                   &controllerMain_B.BusAssignment.eul_ang[0],
                   &controllerMain_B.BusAssignment.pos_lla[0],
                   &controllerMain_B.BusAssignment.vel_ned[0],
                   &controllerMain_B.BusAssignment.aspd_cas,
                   &controllerMain_B.BusAssignment.gspd,
                   &controllerMain_B.BusAssignment.chi,
                   &controllerMain_B.TECS_mode,
                   &controllerMain_B.mode_data.ft_data.FT_x,
                   &controllerMain_B.mode_data.ft_data.FT_y,
                   &controllerMain_B.mode_data.ft_data.FT_Altitude,
                   &controllerMain_B.mode_data.ft_data.FT_Heading,
                   &controllerMain_B.mode_data.ft_data.FT_AirspeedRef,
                   &controllerMain_B.mode_data.loiter_data.loiter_radius,
                   &controllerMain_B.mode_data.loiter_data.loiter_direction,
                   &controllerMain_B.mode_data.loiter_data.loiter_Center_Lat,
                   &controllerMain_B.mode_data.loiter_data.loiter_Center_Lon,
                   &controllerMain_B.mode_data.loiter_data.loiter_altitude,
                   &controllerMain_B.mode_data.loiter_data.loiter_AirSpeedRef,
                   &controllerMain_B.mode_data.bt_data.BT_Hover_Lat,
                   &controllerMain_B.mode_data.bt_data.BT_Hover_Lon,
                   &controllerMain_B.mode_data.bt_data.BT_Altitude,
                   &controllerMain_B.mode_data.bt_data.BT_Heading,
                   &controllerMain_B.mode_data.eFWLifter_Mode,
                   &controllerMain_B.mode_data.fwrth_data.phase,
                   &controllerMain_B.mode_data.fwrth_data.CAS_sp,
                   &controllerMain_B.mode_data.fwrth_data.alt_sp,
                   &controllerMain_U.wp_data.wp_list_valid,
                   &controllerMain_U.wp_data.wp_list_count,
                   &controllerMain_U.wp_data.cur_wp_lat,
                   &controllerMain_U.wp_data.cur_wp_lon,
                   &controllerMain_U.wp_data.cur_wp_alt,
                   &controllerMain_U.wp_data.cmd_wp_idx,
                   &controllerMain_U.wp_data.last_wp_land,
                   &controllerMain_U.std_command.vom_cmd,
                   &controllerMain_U.std_command.airspeed_cas_cmd,
                   &controllerMain_U.std_command.fwrth_apr_deg,
                   &controllerMain_B.bGPSLoss,
                   &controllerMain_B.bTelemtryLinkLoss1sec,
                   &rtb_FW_TECS_switcher_modelref_o1,
                   &controllerMain_Y.wp_req_idx,
                   &rtb_approach_circle_done_hqf5zegy2u,
                   &rtb_reduce_speed_alt_done_bd02ecbta3,
                   &rtb_circle_align_done_ix4pkr1ylq,
                   &rtb_cross_tangent_point_done_mtl0vknyin,
                   &rtb_land_lat_fbnpixa3xe, &rtb_land_lon_fuvjv0mdij,
                   &rtb_approach_ang_baxqfjn0jo, &rtb_cur_leg_heading_priipbumn4,
                   &rtb_cur_leg_length_px1a0fpeys,
                   &rtb_cur_leg_remaining_dist_htjoyldznw,
                   &rtb_wp_list_valid_noeol5rznd, &rtb_last_wp_land_oojmz2hxa2,
                   &rtb_land_wp_lat_oe3el12bfw, &rtb_land_wp_lon_jm2bgprjv1,
                   &rtb_curpos_to_wp_heading_cisgujy5vn,
                   &rtb_WPN_cmd_received_cnz0eax2ri);

  /* ModelReference generated from: '<Root>/Model9' */
  FW_latGuidance(&rtb_FW_TECS_switcher_modelref_o1.roll_cmd,
                 &controllerMain_B.BusAssignment, &rtb_omega_jjutubfppl,
                 &rtb_roll_cmd);

  /* Saturate: '<S4>/V_d_lim_to_100mps' incorporates:
   *  BusAssignment: '<S27>/Bus Assignment1'
   *  BusAssignment: '<S28>/Bus Assignment1'
   */
  if (rtb_BusAssignment1_ins_1_v_ned_idx_2 > 100.0) {
    rtb_omega_jjutubfppl = 100.0;
  } else if (rtb_BusAssignment1_ins_1_v_ned_idx_2 < -100.0) {
    rtb_omega_jjutubfppl = -100.0;
  } else {
    rtb_omega_jjutubfppl = rtb_BusAssignment1_ins_1_v_ned_idx_2;
  }

  /* Trigonometry: '<S4>/Atan1' incorporates:
   *  Gain: '<S4>/Gain'
   *  Saturate: '<S4>/V_d_lim_to_100mps'
   */
  rtb_approach_ang = atan2(-rtb_omega_jjutubfppl, rtb_va_filt);

  /* DiscreteIntegrator: '<S26>/omega' */
  if (controllerMain_DW.omega_IC_LOADING != ((uint8_T)0)) {
    controllerMain_DW.omega_DSTATE = rtb_approach_ang;
  }

  rtb_omega_jjutubfppl = controllerMain_DW.omega_DSTATE;

  /* DiscreteIntegrator: '<S25>/omega' */
  if (controllerMain_DW.omega_IC_LOADING_bb503dutix != ((uint8_T)0)) {
    controllerMain_DW.omega_DSTATE_l5hd10dn0e = rtb_va_filt;
  }

  /* Product: '<S25>/Product' incorporates:
   *  Constant: '<S25>/Constant3'
   *  DiscreteIntegrator: '<S25>/omega'
   *  Sum: '<S25>/Sum'
   */
  rtb_cur_leg_heading = 20.0 * (rtb_va_filt -
    controllerMain_DW.omega_DSTATE_l5hd10dn0e);

  /* Product: '<S4>/Divide' */
  rtb_cur_leg_length = rtb_cur_leg_heading / controllerMain_ConstB.Switch3;

  /* BusCreator generated from: '<Root>/Model6' incorporates:
   *  DiscreteIntegrator: '<S26>/omega'
   *  Sum: '<S4>/Add1'
   *  Sum: '<S4>/Add2'
   */
  rtb_BusConversion_InsertedFor_Model6_at_inport_8_BusCreator1.gammaRad =
    controllerMain_DW.omega_DSTATE;
  rtb_BusConversion_InsertedFor_Model6_at_inport_8_BusCreator1.VdotDivG =
    rtb_cur_leg_length;
  rtb_BusConversion_InsertedFor_Model6_at_inport_8_BusCreator1.Ldot =
    controllerMain_DW.omega_DSTATE - rtb_cur_leg_length;
  rtb_BusConversion_InsertedFor_Model6_at_inport_8_BusCreator1.Edot =
    controllerMain_DW.omega_DSTATE + rtb_cur_leg_length;

  /* ModelReference generated from: '<Root>/Model6' incorporates:
   *  Delay: '<Root>/Delay2'
   */
  FW_TECS(&rtb_FW_TECS_switcher_modelref_o1.h_dot_cmd,
          &rtb_FW_TECS_switcher_modelref_o1.h_cmd,
          &rtb_FW_TECS_switcher_modelref_o1.V_cmd,
          &rtb_FW_TECS_switcher_modelref_o1.TECS_reset,
          &rtb_FW_TECS_switcher_modelref_o1.h_hold,
          &controllerMain_B.BusAssignment, &controllerMain_DW.Delay2_DSTATE,
          &rtb_BusConversion_InsertedFor_Model6_at_inport_8_BusCreator1,
          &rtb_Model6);

  /* ModelReference generated from: '<Root>/Model' */
  att_ctrl_switcher(&controllerMain_B.BusAssignment.eul_ang[0],
                    &controllerMain_B.BusAssignment.omg[0],
                    &controllerMain_B.BusAssignment.aspd_cas,
                    &controllerMain_B.BusAssignment.h_radar_agl,
                    &controllerMain_B.BusAssignment.gspd,
                    &controllerMain_B.BusAssignment.chi, &rtb_Model2_o1.rollCmd,
                    &rtb_Model2_o1.pitchCmd, &controllerMain_B.vom_status,
                    &controllerMain_B.BusAssignment_b4zvdvf4m4,
                    &controllerMain_B.bInAirFlag, &controllerMain_B.bGPSLoss,
                    &controllerMain_B.mode_data.hover_data.hover_yaw_ref,
                    &controllerMain_B.mode_data.hover_data.hover_yaw_override,
                    &controllerMain_B.mode_data.autotakeoff_data.takeoff_yaw_ref,
                    &controllerMain_B.mode_data.autoland_data.land_yaw_ref,
                    &controllerMain_B.mode_data.rth_data.rth_yaw_ref,
                    &controllerMain_B.mode_data.rth_data.bBreaking,
                    &controllerMain_B.mode_data.ft_data.FT_Heading,
                    &controllerMain_B.mode_data.bt_data.BT_Heading,
                    &controllerMain_B.mode_data.eFWLifter_Mode,
                    &controllerMain_B.loiter_mode, &rtb_roll_cmd,
                    &controllerMain_B.TECS_mode, &rtb_Model6.theta_cmd,
                    &controllerMain_B.CoG_tracking,
                    &controllerMain_B.lifter_state, &rtb_rollCmd, &rtb_pitchCmd,
                    &rtb_yawCmd, &rtb_yawRateCmd, &rtb_yawHold, &rtb_yawFF,
                    &rtb_init_reset);

  /* Lookup_n-D: '<S1>/1-D Lookup Table' */
  rtb_uDLookupTable = look1_plinlc(controllerMain_B.BusAssignment.aspd_cas,
    controllerMain_ConstP.uDLookupTable_bp01Data,
    controllerMain_ConstP.uDLookupTable_tableData, &controllerMain_DW.m_bpIndex,
    1U);

  /* RelationalOperator: '<S10>/Compare' incorporates:
   *  Constant: '<S10>/Constant'
   */
  rtb_Delay5 = (rtb_uDLookupTable == 0.0);

  /* RelationalOperator: '<S8>/Compare' incorporates:
   *  Constant: '<S8>/Constant'
   */
  rtb_reduce_speed_alt_done = (rtb_uDLookupTable == 1.0);

  /* Saturate: '<S1>/Saturation' */
  if (controllerMain_B.BusAssignment.aspd_cas > 300.0) {
    rtb_cur_leg_length = 300.0;
  } else if (controllerMain_B.BusAssignment.aspd_cas < 0.01) {
    rtb_cur_leg_length = 0.01;
  } else {
    rtb_cur_leg_length = controllerMain_B.BusAssignment.aspd_cas;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Math: '<S1>/Square' */
  rtb_cur_leg_length *= rtb_cur_leg_length;

  /* Gain: '<S1>/Gain4' */
  rtb_cur_leg_remaining_dist = 0.028106416800000002 * rtb_cur_leg_length;

  /* Gain: '<S1>/Gain2' incorporates:
   *  Constant: '<S1>/Constant3'
   *  Constant: '<S1>/Ixx'
   *  Delay generated from: '<Root>/Delay4'
   *  Delay generated from: '<Root>/Delay7'
   *  Product: '<S1>/Divide2'
   *  Product: '<S1>/Product3'
   *  Product: '<S1>/Product4'
   *  Product: '<S1>/Product5'
   *  Sum: '<S1>/Sum2'
   *  Sum: '<S1>/Sum3'
   */
  rtb_land_wp_lat = -5.0 * (((1.0 - rtb_uDLookupTable) *
    ((rtb_cur_leg_remaining_dist * controllerMain_DW.Delay7_1_DSTATE) / 43.46))
    - (controllerMain_DW.Delay4_1_DSTATE * rtb_uDLookupTable));

  /* Logic: '<S6>/Logical Operator3' incorporates:
   *  Logic: '<S7>/Logical Operator3'
   */
  rtb_circle_align_done = !rtb_reduce_speed_alt_done;

  /* Switch: '<S6>/Switch' incorporates:
   *  Delay generated from: '<Root>/Delay7'
   *  Logic: '<S6>/Logical Operator1'
   *  Logic: '<S6>/Logical Operator2'
   *  Logic: '<S6>/Logical Operator3'
   */
  if (((controllerMain_DW.Delay7_3_DSTATE != 0.0) || rtb_Delay5) &&
      rtb_circle_align_done) {
    /* Gain: '<S1>/Gain5' incorporates:
     *  Constant: '<S6>/Constant4'
     */
    rtb_land_wp_lon = -0.0;
  } else {
    /* Gain: '<S1>/Gain5' */
    rtb_land_wp_lon = -rtb_land_wp_lat;
  }

  /* End of Switch: '<S6>/Switch' */

  /* Gain: '<S1>/Gain1' */
  rtb_cur_leg_length *= 0.025806517920000004;

  /* Gain: '<S1>/Gain' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S1>/Iyy'
   *  Delay generated from: '<Root>/Delay4'
   *  Delay generated from: '<Root>/Delay7'
   *  Product: '<S1>/Divide'
   *  Product: '<S1>/Product'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product2'
   *  Sum: '<S1>/Sum'
   *  Sum: '<S1>/Sum1'
   */
  rtb_curpos_to_wp_heading = -5.0 * (((1.0 - rtb_uDLookupTable) *
    ((rtb_cur_leg_length * controllerMain_DW.Delay7_2_DSTATE) / 38.84)) -
    (controllerMain_DW.Delay4_2_DSTATE * rtb_uDLookupTable));

  /* Switch: '<S7>/Switch' incorporates:
   *  Constant: '<S13>/Constant'
   *  Constant: '<S14>/Constant'
   *  Delay generated from: '<Root>/Delay7'
   *  Logic: '<S7>/Logical Operator1'
   *  Logic: '<S7>/Logical Operator2'
   *  RelationalOperator: '<S13>/Compare'
   *  RelationalOperator: '<S14>/Compare'
   */
  if (((controllerMain_DW.Delay7_4_DSTATE == 1.0) || (rtb_uDLookupTable == 0.0))
      && rtb_circle_align_done) {
    /* Gain: '<S1>/Gain3' incorporates:
     *  Constant: '<S7>/Constant4'
     */
    rtb_Model4_o1 = -0.0;
  } else {
    /* Gain: '<S1>/Gain3' */
    rtb_Model4_o1 = -rtb_curpos_to_wp_heading;
  }

  /* End of Switch: '<S7>/Switch' */

  /* Outport: '<Root>/ctrl_log' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   */
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntCF.roll_CF = rtb_land_wp_lon;
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntCF.pitch_CF = rtb_Model4_o1;

  /* ModelReference generated from: '<Root>/Model1' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment'
   *  Delay: '<Root>/Delay1'
   */
  attCtrl(&rtb_rollCmd, &rtb_pitchCmd, &rtb_yawCmd, &rtb_yawRateCmd,
          &rtb_yawHold, &rtb_yawFF, &rtb_init_reset,
          &controllerMain_B.BusAssignment,
          &controllerMain_DW.Delay1_DSTATE.c_erp1,
          &controllerMain_DW.Delay1_DSTATE.c_erp3, &rtb_land_wp_lon,
          &rtb_Model4_o1, &rtb_uDLookupTable, &rtb_Model1_o1,
          &rtb_roll_int_n0n0i3aced, &rtb_pitch_int_b2jgrf5nof,
          &rtb_roll_sat_cnnujar2qf, &rtb_pitch_sat_dxrqxisnco);

  /* ModelReference generated from: '<Root>/MR_CA_ref' */
  MR_CA(&rtb_altCtrl_ref.forceDes, &rtb_Model1_o1.momentDes[0],
        &controllerMain_B.vom_status, &controllerMain_B.rampup_phase,
        &controllerMain_B.BusAssignment.aspd_cas, &controllerMain_B.lifter_state,
        &controllerMain_B.mode_data.eFWLifter_Mode,
        &controllerMain_B.BusAssignment_b4zvdvf4m4.throttle_ch, &rtb_MR_CA_ref);

  /* ModelReference generated from: '<Root>/Model12' */
  FW_attCtrl_switcher(&controllerMain_B.vom_status,
                      &controllerMain_B.BusAssignment_b4zvdvf4m4.engine_ch,
                      &controllerMain_B.BusAssignment.aspd_cas,
                      &controllerMain_B.TECS_mode, &rtb_Model6.engineCmd,
                      &rtb_Model6.theta_cmd, &rtb_rollCmd, &rtb_pitchCmd,
                      &controllerMain_B.mode_data.ft_data.FT_PusherThrottle,
                      &controllerMain_B.mode_data.bt_data.BT_PusherThrottle,
                      &controllerMain_B.mode_data.eFWLifter_Mode, &rtb_roll_cmd,
                      &rtb_Model12);

  /* Logic: '<S6>/Logical Operator6' incorporates:
   *  Logic: '<S7>/Logical Operator6'
   */
  rtb_circle_align_done = !rtb_Delay5;

  /* Switch: '<S6>/Switch1' incorporates:
   *  Constant: '<S6>/Constant4'
   *  Constant: '<S9>/Constant'
   *  Delay generated from: '<Root>/Delay4'
   *  Logic: '<S6>/Logical Operator4'
   *  Logic: '<S6>/Logical Operator5'
   *  Logic: '<S6>/Logical Operator6'
   *  RelationalOperator: '<S9>/Compare'
   */
  if ((rtb_reduce_speed_alt_done || (controllerMain_DW.Delay4_3_DSTATE == 1.0)) &&
      rtb_circle_align_done) {
    rtb_land_wp_lat = 0.0;
  }

  /* BusAssignment: '<S1>/Bus Assignment1' incorporates:
   *  Constant: '<S1>/Ixx'
   *  Product: '<S1>/Divide3'
   *  Switch: '<S6>/Switch1'
   */
  rtb_roll_cmd = (rtb_land_wp_lat * 43.46) / rtb_cur_leg_remaining_dist;

  /* Switch: '<S7>/Switch1' incorporates:
   *  Constant: '<S11>/Constant'
   *  Constant: '<S12>/Constant'
   *  Constant: '<S7>/Constant4'
   *  Delay generated from: '<Root>/Delay4'
   *  Logic: '<S7>/Logical Operator4'
   *  Logic: '<S7>/Logical Operator5'
   *  RelationalOperator: '<S11>/Compare'
   *  RelationalOperator: '<S12>/Compare'
   */
  if (((rtb_uDLookupTable == 1.0) || (controllerMain_DW.Delay4_4_DSTATE == 1.0))
      && rtb_circle_align_done) {
    rtb_curpos_to_wp_heading = 0.0;
  }

  /* BusAssignment: '<S1>/Bus Assignment1' incorporates:
   *  Constant: '<S1>/Iyy'
   *  Product: '<S1>/Divide1'
   *  Switch: '<S7>/Switch1'
   */
  rtb_BusAssignment1_nc41wq0lh0_pitch_CF = (rtb_curpos_to_wp_heading * 38.84) /
    rtb_cur_leg_length;

  /* Outport: '<Root>/ctrl_log' incorporates:
   *  BusAssignment: '<S1>/Bus Assignment1'
   *  Delay: '<Root>/Delay3'
   *  ModelReference generated from: '<Root>/Model4'
   */
  FW_attCtrl(&rtb_Model12.pitch_cmd, &rtb_Model12.roll_cmd,
             &rtb_Model12.roll_reset, &rtb_Model12.pitch_reset,
             &controllerMain_B.BusAssignment,
             &controllerMain_DW.Delay3_DSTATE.c_erp1,
             &controllerMain_DW.Delay3_DSTATE.c_erp2, &rtb_roll_cmd,
             &rtb_BusAssignment1_nc41wq0lh0_pitch_CF, &rtb_Model4_o1,
             &rtb_curpos_to_wp_heading, &rtb_cur_leg_length,
             &rtb_roll_int_mtg3gjguro, &rtb_pitch_int_fkznhgwxng,
             &rtb_roll_sat_cvg42to5m1, &rtb_pitch_sat_hd1k1f2mpb,
             &rtb_cur_leg_remaining_dist, &rtb_land_wp_lat, &rtb_land_wp_lon,
             &controllerMain_Y.ctrl_log.fwAttCmd.raw_rollCmd);

  /* ModelReference generated from: '<Root>/Model5' */
  FW_throttle(&rtb_Model12.pusher_cmd, &rtb_RateLimiter_kbd0fqq3vq);

  /* ModelReference generated from: '<Root>/Model8' */
  FW_CA(&controllerMain_B.vom_status, &rtb_Model4_o1, &rtb_curpos_to_wp_heading,
        &rtb_cur_leg_length, &rtb_RateLimiter_kbd0fqq3vq,
        &controllerMain_B.BusAssignment,
        &controllerMain_B.BusAssignment_b4zvdvf4m4.roll_ch,
        &controllerMain_B.BusAssignment_b4zvdvf4m4.pitch_ch,
        &controllerMain_B.BusAssignment_b4zvdvf4m4.yaw_ch,
        &controllerMain_B.BusAssignment_b4zvdvf4m4.engine_ch, &rtb_Model8);

  /* Outport: '<Root>/ctrl_log' incorporates:
   *  ModelReference: '<Root>/Model11'
   */
  transitionHandler(&rtb_MR_CA_ref, &rtb_Model8, &rtb_Model11_o1,
                    &controllerMain_Y.ctrl_log.controllerCA);

  /* Outport: '<Root>/std_ctrl' incorporates:
   *  BusAssignment: '<S3>/Bus Assignment1'
   *  Constant: '<S3>/Constant2'
   *  Constant: '<S3>/Constant5'
   *  Constant: '<S3>/Constant7'
   *  Constant: '<S3>/Constant8'
   */
  for (i = 0; i < 8; i++) {
    /* Switch: '<S21>/Switch3' incorporates:
     *  Constant: '<S21>/DISARMED_omg_d'
     */
    if (rtb_Compare_lvr1zt3dde) {
      controllerMain_Y.std_ctrl.lifter_rpm_cmd[i] = 0.0;
    } else {
      controllerMain_Y.std_ctrl.lifter_rpm_cmd[i] =
        rtb_Model11_o1.lifter_rpm_cmd[i];
    }

    /* End of Switch: '<S21>/Switch3' */
  }

  controllerMain_Y.std_ctrl.eng_throttle_cmd = 0.0;
  (void)memcpy(&controllerMain_Y.std_ctrl.acs_servo_deg_cmd[0],
               &rtb_Model11_o1.acs_servo_deg_cmd[0], 12U * (sizeof(real_T)));
  controllerMain_Y.std_ctrl.nlg_servo_deg_cmd = 0.0;
  for (i = 0; i < 8; i++) {
    /* Switch: '<S21>/Switch4' incorporates:
     *  Constant: '<S21>/DISARMED_dutyCycle'
     */
    if (rtb_Compare_lvr1zt3dde) {
      controllerMain_Y.std_ctrl.lifter_cval_cmd[i] = 0.0;
    } else {
      controllerMain_Y.std_ctrl.lifter_cval_cmd[i] =
        rtb_Model11_o1.lifter_cval_cmd[i];
    }

    /* End of Switch: '<S21>/Switch4' */
  }

  controllerMain_Y.std_ctrl.eng_mixture_cmd = 0.0;
  controllerMain_Y.std_ctrl.eng_ignition_cmd = 0.0;

  /* Switch: '<S21>/Switch2' */
  if (rtb_Compare_lvr1zt3dde) {
    /* Outport: '<Root>/std_ctrl' incorporates:
     *  Constant: '<S21>/DISARMED_PWM'
     */
    controllerMain_Y.std_ctrl.pusher_pwm_cmd = 1000.0;
  } else {
    /* Outport: '<Root>/std_ctrl' */
    controllerMain_Y.std_ctrl.pusher_pwm_cmd = rtb_Model11_o1.pusher_pwm_cmd;
  }

  /* End of Switch: '<S21>/Switch2' */

  /* Outport: '<Root>/ctrl_log' incorporates:
   *  BusAssignment: '<Root>/Bus Assignment1'
   *  BusAssignment: '<S1>/Bus Assignment'
   *  BusAssignment: '<S1>/Bus Assignment1'
   *  BusAssignment: '<S1>/Bus Assignment2'
   *  BusAssignment: '<S27>/Bus Assignment1'
   *  BusAssignment: '<S28>/Bus Assignment1'
   *  BusAssignment: '<S4>/Bus Assignment1'
   *  BusCreator generated from: '<Root>/Bus Assignment1'
   *  Delay generated from: '<Root>/Delay4'
   *  Delay generated from: '<Root>/Delay7'
   *  Inport: '<Root>/sensor_in'
   *  ModelReference generated from: '<Root>/Model1'
   *  ModelReference generated from: '<Root>/Model2'
   *  ModelReference generated from: '<Root>/Model3'
   *  RelationalOperator: '<S4>/NotEqual'
   *  SignalConversion generated from: '<S27>/Bus Assignment1'
   * */
  controllerMain_Y.ctrl_log.controllerAttCtrl = rtb_Model1_o1;
  controllerMain_Y.ctrl_log.controllerAltCtrl = rtb_altCtrl_ref;
  controllerMain_Y.ctrl_log.controllerVelCtrl = rtb_Model2_o2;
  controllerMain_Y.ctrl_log.controllerPosCtrl = rtb_Model3_o1;
  controllerMain_Y.ctrl_log.controllerTECS = rtb_Model6;
  controllerMain_Y.ctrl_log.stateMachine.vom_status = rtb_vom_status_ofjzuvdpj2;
  controllerMain_Y.ctrl_log.stateMachine.safety_state = rtb_safety_state;
  controllerMain_Y.ctrl_log.stateMachine.inAir_flag = rtb_inAir_flag;
  controllerMain_Y.ctrl_log.stateMachine.pic_status = rtb_pic_status;
  controllerMain_Y.ctrl_log.stateMachine.loiter_mode = rtb_loiter_mode;
  controllerMain_Y.ctrl_log.stateMachine.lifter_mode = rtb_lifter_mode;
  controllerMain_Y.ctrl_log.stateMachine.tecs_mode = rtb_tecs_mode;
  controllerMain_Y.ctrl_log.stateMachine.CoG_tracking = rtb_CoG_tracking;
  controllerMain_Y.ctrl_log.controllerIF_att.rollCmd = rtb_rollCmd;
  controllerMain_Y.ctrl_log.controllerIF_att.pitchCmd = rtb_pitchCmd;
  controllerMain_Y.ctrl_log.controllerIF_att.yawCmd = rtb_yawCmd;
  controllerMain_Y.ctrl_log.controllerIF_att.yawRateCmd = rtb_yawRateCmd;
  controllerMain_Y.ctrl_log.controllerIF_att.yawHold = rtb_yawHold;
  controllerMain_Y.ctrl_log.controllerIF_att.yawFF = rtb_yawFF;
  controllerMain_Y.ctrl_log.controllerIF_att.init_reset = rtb_init_reset;
  (void)memset
    (&controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.dcm_e2b[0],
     0, 9U * (sizeof(real_T)));
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.aspd_cas =
    rtb_BusAssignment1_ads_1_aspd_cas;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.aoa =
    rtb_BusAssignment1_ads_1_aoa_tmp;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.aos =
    rtb_BusAssignment1_ads_1_aos_tmp;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.h_gps_amsl =
    rtb_BusAssignment1_ins_1_alt_gps_amsl_tmp;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.h_baro_amsl =
    rtb_BusAssignment1_ads_1_alt_baro_amsl_tmp;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.h_radar_agl =
    rtb_BusAssignment1_h_radar_agl_tmp;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.eul_ang[0] =
    rtb_BusAssignment1_ins_1_eul_ang_idx_0;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.omg[0] =
    rtb_BusAssignment1_ins_1_omg[0];
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.pos_lla[0] =
    rtb_BusAssignment1_cvb352wjyj_gspd;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.vel_ned[0] =
    rtb_BusAssignment1_ins_1_v_ned_idx_0;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.accel_b[0] =
    rtb_BusAssignment1_ins_1_acc[0];
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.wow[0] =
    rtb_BusAssignment1_wow_idx_0;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.eul_ang[1] =
    rtb_BusAssignment1_ins_1_eul_ang_idx_1;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.omg[1] =
    rtb_BusAssignment1_ins_1_omg[1];
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.pos_lla[1] =
    notch0_gyro_states;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.vel_ned[1] =
    rtb_BusAssignment1_ins_1_v_ned_idx_1;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.accel_b[1] =
    rtb_BusAssignment1_ins_1_acc[1];
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.wow[1] =
    rtb_BusAssignment1_wow_idx_1;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.eul_ang[2] =
    rtb_BusAssignment1_ins_1_eul_ang_idx_2;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.omg[2] =
    rtb_BusAssignment1_ins_1_omg[2];
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.pos_lla[2] =
    rtb_BusAssignment1_ins_1_alt_gps_amsl_tmp;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.vel_ned[2] =
    rtb_BusAssignment1_ins_1_v_ned_idx_2;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.accel_b[2] =
    rtb_BusAssignment1_ins_1_acc[2];
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.wow[2] =
    rtb_BusAssignment1_wow_idx_2;
  (void)memcpy
    (&controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.rotor_rpm[0],
     &rtb_BusAssignment1_rotor_rpm[0], (sizeof(real_T)) << ((uint32_T)3U));
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.gspd = 0.0;
  controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.chi = 0.0;
  controllerMain_Y.ctrl_log.SensorMgmt.rad_alt_out = rtb_Switch_di0iual0gm;
  controllerMain_Y.ctrl_log.SensorMgmt.rad_alt_spike = (rtb_RateLimiter !=
    rtb_BusAssignment1_h_radar_agl_tmp);
  controllerMain_Y.ctrl_log.SensorMgmt.airspeedCAS_filt = rtb_va_filt;
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntData.roll_int =
    controllerMain_DW.Delay4_1_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntData.pitch_int =
    controllerMain_DW.Delay4_2_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntData.roll_sat =
    controllerMain_DW.Delay4_3_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntData.pitch_sat =
    controllerMain_DW.Delay4_4_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntData.roll_int =
    controllerMain_DW.Delay7_1_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntData.pitch_int =
    controllerMain_DW.Delay7_2_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntData.roll_sat =
    controllerMain_DW.Delay7_3_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntData.pitch_sat =
    controllerMain_DW.Delay7_4_DSTATE;
  controllerMain_Y.ctrl_log.IntegratorCF.MR_IntCF.lambda = rtb_uDLookupTable;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntCF.roll_CF = rtb_roll_cmd;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntCF.pitch_CF =
    rtb_BusAssignment1_nc41wq0lh0_pitch_CF;
  controllerMain_Y.ctrl_log.IntegratorCF.FW_IntCF.lambda = rtb_uDLookupTable;
  controllerMain_Y.ctrl_log.IntegratorCF.lambda = rtb_uDLookupTable;
  controllerMain_Y.ctrl_log.fwAttCmd.pitchCmd = rtb_cur_leg_remaining_dist;
  controllerMain_Y.ctrl_log.fwAttCmd.rollCmd = rtb_land_wp_lat;
  controllerMain_Y.ctrl_log.fwAttCmd.raw_pitchCmd = rtb_land_wp_lon;

  /* BusCreator generated from: '<Root>/fcs_state' incorporates:
   *  Outport: '<Root>/fcs_state'
   */
  controllerMain_Y.fcs_state.vom_status = rtb_vom_status_ofjzuvdpj2;
  controllerMain_Y.fcs_state.safety_state = rtb_safety_state;
  controllerMain_Y.fcs_state.inAir_flag = rtb_inAir_flag;
  controllerMain_Y.fcs_state.pic_status = rtb_pic_status;
  controllerMain_Y.fcs_state.loiter_mode = rtb_loiter_mode;
  controllerMain_Y.fcs_state.lifter_mode = rtb_lifter_mode;
  controllerMain_Y.fcs_state.tecs_mode = rtb_tecs_mode;
  controllerMain_Y.fcs_state.CoG_tracking = rtb_CoG_tracking;

  /* Outport: '<Root>/gcs_fb' incorporates:
   *  BusAssignment: '<S4>/Bus Assignment'
   *  BusAssignment: '<S5>/Bus Assignment'
   */
  controllerMain_Y.gcs_fb.mr_roll_sp = rtb_rollCmd;
  controllerMain_Y.gcs_fb.mr_pitch_sp = rtb_pitchCmd;
  controllerMain_Y.gcs_fb.mr_yaw_sp = rtb_yawCmd;
  controllerMain_Y.gcs_fb.mr_yawrate_sp = rtb_yawRateCmd;
  controllerMain_Y.gcs_fb.mr_yaw_hold = rtb_yawHold;
  controllerMain_Y.gcs_fb.mr_att_intg_reset = rtb_init_reset;
  controllerMain_Y.gcs_fb.mr_alt_sp = rtb_hCmd_f0mpth4u0h;
  controllerMain_Y.gcs_fb.mr_altrate_sp = rtb_hRateCmd_n0d5yq2y1h;
  controllerMain_Y.gcs_fb.mr_altrate_intg_reset = rtb_init_reset_hdot_lay4g5k1gs;
  controllerMain_Y.gcs_fb.mr_alt_hold = rtb_hHold_kkfp1ixkpi;
  controllerMain_Y.gcs_fb.mr_vel_sp_X = rtb_velCmdH_x_plbsly3dk1;
  controllerMain_Y.gcs_fb.mr_vel_sp_Y = rtb_velCmdH_y_fn5sqdulrh;
  controllerMain_Y.gcs_fb.mr_vel_intg_reset = rtb_init_reset_vel_oyuizswpb3;
  controllerMain_Y.gcs_fb.mr_pos_sp_lat =
    rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.posLLA[0];
  controllerMain_Y.gcs_fb.mr_pos_sp_lon =
    rtb_BusConversion_InsertedFor_Model3_at_inport_0_BusCreator1.posLLA[1];
  controllerMain_Y.gcs_fb.mr_thrust_cmd = rtb_altCtrl_ref.forceDes;
  controllerMain_Y.gcs_fb.h_rel_takeoff = controllerMain_B.dCapturedAlt_m;
  controllerMain_Y.gcs_fb.voter = controllerMain_B.BusAssignment;

  /* Saturate: '<S24>/Saturation' incorporates:
   *  BusAssignment: '<S27>/Bus Assignment1'
   *  BusAssignment: '<S28>/Bus Assignment1'
   */
  if (rtb_BusAssignment1_ads_1_aspd_cas > 40.0) {
    rtb_RateLimiter_kbd0fqq3vq = 40.0;
  } else if (rtb_BusAssignment1_ads_1_aspd_cas < 0.0) {
    rtb_RateLimiter_kbd0fqq3vq = 0.0;
  } else {
    rtb_RateLimiter_kbd0fqq3vq = rtb_BusAssignment1_ads_1_aspd_cas;
  }

  /* End of Saturate: '<S24>/Saturation' */

  /* RateLimiter: '<S24>/Rate Limiter' */
  rtb_RateLimiter = rtb_RateLimiter_kbd0fqq3vq -
    controllerMain_DW.PrevY_fbnop0txyt;
  if (rtb_RateLimiter > 0.05) {
    rtb_RateLimiter_kbd0fqq3vq = controllerMain_DW.PrevY_fbnop0txyt + 0.05;
  } else if (rtb_RateLimiter < -0.05) {
    rtb_RateLimiter_kbd0fqq3vq = controllerMain_DW.PrevY_fbnop0txyt - 0.05;
  } else {
    /* no actions */
  }

  controllerMain_DW.PrevY_fbnop0txyt = rtb_RateLimiter_kbd0fqq3vq;

  /* End of RateLimiter: '<S24>/Rate Limiter' */

  /* Update for DiscreteFilter: '<S27>/notch0_gyro' */
  controllerMain_DW.notch0_gyro_states[1] =
    controllerMain_DW.notch0_gyro_states[0];
  controllerMain_DW.notch0_gyro_states[0] = notch0_gyro_tmp[0];

  /* Update for DiscreteFilter: '<S27>/notch0_acc' */
  controllerMain_DW.notch0_acc_states[1] = controllerMain_DW.notch0_acc_states[0];
  controllerMain_DW.notch0_acc_states[0] = notch0_acc_tmp[0];

  /* Update for DiscreteFilter: '<S27>/notch0_gyro1' */
  controllerMain_DW.notch0_gyro1_states[1] =
    controllerMain_DW.notch0_gyro1_states[0];
  controllerMain_DW.notch0_gyro1_states[0] = notch0_gyro1_tmp[0];

  /* Update for DiscreteFilter: '<S27>/notch0_acc1' */
  controllerMain_DW.notch0_acc1_states[1] =
    controllerMain_DW.notch0_acc1_states[0];
  controllerMain_DW.notch0_acc1_states[0] = notch0_acc1_tmp[0];

  /* Update for DiscreteFilter: '<S27>/notch0_gyro' */
  controllerMain_DW.notch0_gyro_states[3] =
    controllerMain_DW.notch0_gyro_states[2];
  controllerMain_DW.notch0_gyro_states[2] = notch0_gyro_tmp[1];

  /* Update for DiscreteFilter: '<S27>/notch0_acc' */
  controllerMain_DW.notch0_acc_states[3] = controllerMain_DW.notch0_acc_states[2];
  controllerMain_DW.notch0_acc_states[2] = notch0_acc_tmp[1];

  /* Update for DiscreteFilter: '<S27>/notch0_gyro1' */
  controllerMain_DW.notch0_gyro1_states[3] =
    controllerMain_DW.notch0_gyro1_states[2];
  controllerMain_DW.notch0_gyro1_states[2] = notch0_gyro1_tmp[1];

  /* Update for DiscreteFilter: '<S27>/notch0_acc1' */
  controllerMain_DW.notch0_acc1_states[3] =
    controllerMain_DW.notch0_acc1_states[2];
  controllerMain_DW.notch0_acc1_states[2] = notch0_acc1_tmp[1];

  /* Update for DiscreteFilter: '<S27>/notch0_gyro' */
  controllerMain_DW.notch0_gyro_states[5] =
    controllerMain_DW.notch0_gyro_states[4];
  controllerMain_DW.notch0_gyro_states[4] = notch0_gyro_tmp[2];

  /* Update for DiscreteFilter: '<S27>/notch0_acc' */
  controllerMain_DW.notch0_acc_states[5] = controllerMain_DW.notch0_acc_states[4];
  controllerMain_DW.notch0_acc_states[4] = notch0_acc_tmp[2];

  /* Update for DiscreteFilter: '<S27>/notch0_gyro1' */
  controllerMain_DW.notch0_gyro1_states[5] =
    controllerMain_DW.notch0_gyro1_states[4];
  controllerMain_DW.notch0_gyro1_states[4] = notch0_gyro1_tmp[2];

  /* Update for DiscreteFilter: '<S27>/notch0_acc1' */
  controllerMain_DW.notch0_acc1_states[5] =
    controllerMain_DW.notch0_acc1_states[4];
  controllerMain_DW.notch0_acc1_states[4] = notch0_acc1_tmp[2];

  /* Update for Memory: '<S24>/Memory' incorporates:
   *  Constant: '<S24>/Constant1'
   *  Product: '<S24>/Product'
   *  Product: '<S24>/Product1'
   *  Sum: '<S24>/Sum'
   */
  controllerMain_DW.Memory_PreviousInput = (rtb_RateLimiter_kbd0fqq3vq *
    controllerMain_ConstB.Sum1) + (0.985 * rtb_va_filt);

  /* Update for UnitDelay: '<Root>/Unit Delay1' */
  controllerMain_DW.UnitDelay1_DSTATE = controllerMain_B.vom_status;

  /* Update for UnitDelay: '<Root>/Unit Delay2' */
  controllerMain_DW.UnitDelay2_DSTATE = controllerMain_B.pic_status;

  /* Update for UnitDelay generated from: '<S16>/ud' */
  controllerMain_DW.ud_1_DSTATE = rtb_roll_ch;

  /* Update for UnitDelay generated from: '<S16>/ud' */
  controllerMain_DW.ud_2_DSTATE = rtb_pitch_ch;

  /* Update for UnitDelay generated from: '<S16>/ud' */
  controllerMain_DW.ud_3_DSTATE = rtb_yaw_ch;

  /* Update for UnitDelay generated from: '<S16>/ud' */
  controllerMain_DW.ud_4_DSTATE = rtb_throttle_ch;

  /* Update for UnitDelay generated from: '<S16>/ud' */
  controllerMain_DW.ud_5_DSTATE = rtb_engine_ch;

  /* Update for UnitDelay: '<S20>/FixPt Unit Delay2' incorporates:
   *  Constant: '<S20>/FixPt Constant'
   */
  controllerMain_DW.FixPtUnitDelay2_DSTATE = 0U;

  /* Update for UnitDelay: '<S20>/FixPt Unit Delay1' */
  controllerMain_DW.FixPtUnitDelay1_DSTATE = rtb_UkYk1;

  /* Update for UnitDelay generated from: '<S16>/ud1' */
  controllerMain_DW.ud1_1_DSTATE = rtb_deltafalllimit;

  /* Update for UnitDelay generated from: '<S16>/ud1' */
  controllerMain_DW.ud1_2_DSTATE = rtb_pitch_ch_i1x0haaskm;

  /* Update for UnitDelay generated from: '<S16>/ud1' */
  controllerMain_DW.ud1_3_DSTATE = rtb_yaw_ch_jthfpygiuk;

  /* Update for UnitDelay generated from: '<S16>/ud1' */
  controllerMain_DW.ud1_4_DSTATE = rtb_throttle_ch_oryunzuyq0;

  /* Update for UnitDelay generated from: '<S16>/ud1' */
  controllerMain_DW.ud1_5_DSTATE = rtb_engine_ch_bjavj5sodg;

  /* Update for UnitDelay: '<Root>/Unit Delay' */
  controllerMain_DW.UnitDelay_DSTATE = rtb_altCtrl_ref;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_1_DSTATE = rtb_approach_circle_done_hqf5zegy2u;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_2_DSTATE = rtb_reduce_speed_alt_done_bd02ecbta3;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_3_DSTATE = rtb_circle_align_done_ix4pkr1ylq;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_4_DSTATE =
    rtb_cross_tangent_point_done_mtl0vknyin;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_5_DSTATE = rtb_land_lat_fbnpixa3xe;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_6_DSTATE = rtb_land_lon_fuvjv0mdij;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay3' */
  controllerMain_DW.UnitDelay3_7_DSTATE = rtb_approach_ang_baxqfjn0jo;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_1_DSTATE = rtb_cur_leg_heading_priipbumn4;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_2_DSTATE = rtb_cur_leg_length_px1a0fpeys;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_3_DSTATE = rtb_cur_leg_remaining_dist_htjoyldznw;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_4_DSTATE = rtb_wp_list_valid_noeol5rznd;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_5_DSTATE = rtb_last_wp_land_oojmz2hxa2;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_6_DSTATE = rtb_land_wp_lat_oe3el12bfw;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_7_DSTATE = rtb_land_wp_lon_jm2bgprjv1;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_8_DSTATE = rtb_curpos_to_wp_heading_cisgujy5vn;

  /* Update for UnitDelay generated from: '<Root>/Unit Delay4' */
  controllerMain_DW.UnitDelay4_9_DSTATE = rtb_WPN_cmd_received_cnz0eax2ri;

  /* Update for Delay: '<Root>/Delay' incorporates:
   *  ModelReference generated from: '<Root>/MR_CA_ref'
   */
  controllerMain_DW.Delay_DSTATE = rtb_MR_CA_ref;

  /* Update for Delay: '<Root>/Delay2' */
  controllerMain_DW.Delay2_DSTATE = rtb_Model12.pusher_cmd;

  /* Update for DiscreteIntegrator: '<S26>/omega' incorporates:
   *  Constant: '<S26>/Constant3'
   *  Product: '<S26>/Product'
   *  Sum: '<S26>/Sum'
   */
  controllerMain_DW.omega_IC_LOADING = 0U;
  controllerMain_DW.omega_DSTATE += 0.01 * (20.0 * (rtb_approach_ang -
    rtb_omega_jjutubfppl));

  /* Update for DiscreteIntegrator: '<S25>/omega' */
  controllerMain_DW.omega_IC_LOADING_bb503dutix = 0U;
  controllerMain_DW.omega_DSTATE_l5hd10dn0e += 0.01 * rtb_cur_leg_heading;

  /* Update for Delay: '<Root>/Delay1' incorporates:
   *  ModelReference generated from: '<Root>/MR_CA_ref'
   */
  controllerMain_DW.Delay1_DSTATE = rtb_MR_CA_ref;

  /* Update for Delay generated from: '<Root>/Delay7' */
  controllerMain_DW.Delay7_3_DSTATE = rtb_roll_sat_cvg42to5m1;

  /* Update for Delay generated from: '<Root>/Delay4' */
  controllerMain_DW.Delay4_1_DSTATE = rtb_roll_int_n0n0i3aced;

  /* Update for Delay generated from: '<Root>/Delay7' */
  controllerMain_DW.Delay7_1_DSTATE = rtb_roll_int_mtg3gjguro;

  /* Update for Delay generated from: '<Root>/Delay7' */
  controllerMain_DW.Delay7_4_DSTATE = rtb_pitch_sat_hd1k1f2mpb;

  /* Update for Delay generated from: '<Root>/Delay4' */
  controllerMain_DW.Delay4_2_DSTATE = rtb_pitch_int_b2jgrf5nof;

  /* Update for Delay generated from: '<Root>/Delay7' */
  controllerMain_DW.Delay7_2_DSTATE = rtb_pitch_int_fkznhgwxng;

  /* Update for Delay: '<Root>/Delay3' incorporates:
   *  ModelReference generated from: '<Root>/Model8'
   */
  controllerMain_DW.Delay3_DSTATE = rtb_Model8;

  /* Update for Delay generated from: '<Root>/Delay4' */
  controllerMain_DW.Delay4_3_DSTATE = rtb_roll_sat_cnnujar2qf;

  /* Update for Delay generated from: '<Root>/Delay4' */
  controllerMain_DW.Delay4_4_DSTATE = rtb_pitch_sat_dxrqxisnco;
}

/* Model initialize function */
void controllerMain_initialize(void)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) &controllerMain_B), 0,
                sizeof(controllerMain_n4wy4x2kbb_T));

  {
    controllerMain_B.mode_data = controllerMain_rtZbusMode_data;
    controllerMain_B.vom_status = VOM_INVALID;
    controllerMain_B.safety_state = AC_DISARMED;
    controllerMain_B.pic_status = EXTERNAL;
    controllerMain_B.lifter_state = ON;
  }

  /* states (dwork) */
  (void) memset((void *)&controllerMain_DW, 0,
                sizeof(controllerMain_l5cfqqw4qz_T));

  /* external inputs */
  (void)memset(&controllerMain_U, 0, sizeof(controllerMain_csevfv4opo_T));

  /* external outputs */
  (void)memset(&controllerMain_Y, 0, sizeof(controllerMain_jzldyrnxon_T));

  /* Model Initialize function for ModelReference Block: '<Root>/FW_TECS_switcher_modelref' */
  FW_TECS_switcher_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/MR_CA_ref' */
  MR_CA_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model' */
  att_ctrl_switcher_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model1' */
  attCtrl_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model12' */
  FW_attCtrl_switcher_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model2' */
  velCtrl_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model3' */
  posCtrl_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model4' */
  FW_attCtrl_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model5' */
  FW_throttle_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model6' */
  FW_TECS_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/Model8' */
  FW_CA_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/altCtrl_ref' */
  altCtrl_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/system_state_machine' */
  system_state_machine_initialize();

  /* Model Initialize function for ModelReference Block: '<Root>/velCtrl_sw_ref' */
  vel_ctrl_switcher_initialize();

  /* InitializeConditions for RateLimiter: '<S4>/Rate Limiter' */
  controllerMain_DW.PrevY = 0.0;

  /* InitializeConditions for UnitDelay: '<Root>/Unit Delay1' */
  controllerMain_DW.UnitDelay1_DSTATE = VOM_READY;

  /* InitializeConditions for UnitDelay: '<S20>/FixPt Unit Delay2' */
  controllerMain_DW.FixPtUnitDelay2_DSTATE = 1U;

  /* InitializeConditions for Delay: '<Root>/Delay2' */
  controllerMain_DW.Delay2_DSTATE = 1000.0;

  /* InitializeConditions for DiscreteIntegrator: '<S26>/omega' */
  controllerMain_DW.omega_IC_LOADING = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S25>/omega' */
  controllerMain_DW.omega_IC_LOADING_bb503dutix = 1U;

  /* InitializeConditions for RateLimiter: '<S24>/Rate Limiter' */
  controllerMain_DW.PrevY_fbnop0txyt = 0.0;

  /* SystemInitialize for ModelReference generated from: '<Root>/system_state_machine' incorporates:
   *  Inport: '<Root>/extd_cmd'
   *  Inport: '<Root>/failure_flags'
   *  Inport: '<Root>/std_command'
   *  Inport: '<Root>/wp_data'
   */
  system_state_machine_Init(&controllerMain_B.bInAirFlag,
    &controllerMain_B.vom_status, &controllerMain_B.mode_data,
    &controllerMain_B.bGPSLoss10sec, &controllerMain_B.bGPSLoss,
    &controllerMain_B.rampup_phase, &controllerMain_B.bTelemtryLinkLoss1sec,
    &controllerMain_B.ep_loss1sec, &controllerMain_B.safety_state,
    &controllerMain_B.loiter_mode, &controllerMain_B.TECS_mode,
    &controllerMain_B.lifter_state, &controllerMain_B.pic_status,
    &controllerMain_B.CoG_tracking);

  /* SystemInitialize for ModelReference generated from: '<Root>/Model3' */
  posCtrl_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/velCtrl_sw_ref' */
  vel_ctrl_switcher_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/altCtrl_ref' */
  altCtrl_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model2' */
  velCtrl_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/FW_TECS_switcher_modelref' incorporates:
   *  Inport: '<Root>/std_command'
   *  Inport: '<Root>/wp_data'
   *  Outport: '<Root>/wp_req_idx'
   */
  FW_TECS_switcher_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model6' */
  FW_TECS_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model' */
  att_ctrl_switcher_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model1' */
  attCtrl_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/MR_CA_ref' */
  MR_CA_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model12' */
  FW_attCtrl_switcher_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model4' */
  FW_attCtrl_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model5' */
  FW_throttle_Init();

  /* SystemInitialize for ModelReference generated from: '<Root>/Model8' */
  FW_CA_Init();
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
