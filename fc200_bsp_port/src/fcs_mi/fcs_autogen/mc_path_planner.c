/*
 * File: mc_path_planner.c
 *
 * Code generated for Simulink model 'mc_path_planner'.
 *
 * Model version                  : 2.24
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:32 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "mc_path_planner.h"
#include "rtwtypes.h"
#include "mc_path_planner_types.h"

/* Output and update for referenced model: 'mc_path_planner' */
void mc_path_planner(const real_T rtu_sensor_pos_lla[3], const vom_t
                     *rtu_vom_status, const real_T
                     *rtu_mode_data_hover_data_hover_y, const real_T
                     *rtu_mode_data_hover_data_hover_x, const real_T
                     *rtu_mode_data_hover_data_hover_alt_agl, const real_T
                     *rtu_mode_data_hover_data_hover_yaw_ref, const real_T
                     *rtu_mode_data_autotakeoff_data_takeoff_x, const real_T
                     *rtu_mode_data_autotakeoff_data_takeoff_y, const real_T
                     *rtu_mode_data_autotakeoff_data_takeoff_alt_agl, const
                     real_T *rtu_mode_data_autotakeoff_data_takeoff_yaw_ref,
                     const real_T *rtu_mode_data_autoland_data_land_x, const
                     real_T *rtu_mode_data_autoland_data_land_y, const real_T
                     *rtu_mode_data_autoland_data_land_yaw_ref, const real_T
                     *rtu_mode_data_rth_data_rth_x, const real_T
                     *rtu_mode_data_rth_data_rth_y, const real_T
                     *rtu_mode_data_rth_data_rth_alt_agl, const real_T
                     *rtu_mode_data_rth_data_rth_yaw_ref, const real_T
                     *rtu_mode_data_ft_data_FT_x, const real_T
                     *rtu_mode_data_ft_data_FT_y, const real_T
                     *rtu_mode_data_ft_data_FT_Altitude, const real_T
                     *rtu_mode_data_ft_data_FT_Heading, const real_T
                     *rtu_mode_data_bt_data_BT_Hover_Lat, const real_T
                     *rtu_mode_data_bt_data_BT_Hover_Lon, const real_T
                     *rtu_mode_data_bt_data_BT_Altitude, const real_T
                     *rtu_mode_data_bt_data_BT_Heading, real_T
                     rty_busWaypointManager_posLLA[3], real_T
                     *rty_busWaypointManager_yawCmd)
{
  /* SignalConversion generated from: '<S3>/Vector Concatenate' */
  rty_busWaypointManager_posLLA[0] = *rtu_mode_data_autotakeoff_data_takeoff_x;

  /* SignalConversion generated from: '<S3>/Vector Concatenate' */
  rty_busWaypointManager_posLLA[1] = *rtu_mode_data_autotakeoff_data_takeoff_y;

  /* SignalConversion generated from: '<S3>/Vector Concatenate' */
  rty_busWaypointManager_posLLA[2] =
    *rtu_mode_data_autotakeoff_data_takeoff_alt_agl;

  /* MultiPortSwitch: '<Root>/Multiport Switch' */
  switch (*rtu_vom_status) {
   case VOM_READY:
    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment2'
     */
    rty_busWaypointManager_posLLA[0] = rtu_sensor_pos_lla[0];
    rty_busWaypointManager_posLLA[1] = rtu_sensor_pos_lla[1];
    rty_busWaypointManager_posLLA[2] = rtu_sensor_pos_lla[2];

    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S8>/Bus Assignment2'
     *  Constant: '<S8>/Constant9'
     */
    *rty_busWaypointManager_yawCmd = 0.0;
    break;

   case VOM_STARTUP:
    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment2'
     */
    rty_busWaypointManager_posLLA[0] = rtu_sensor_pos_lla[0];
    rty_busWaypointManager_posLLA[1] = rtu_sensor_pos_lla[1];
    rty_busWaypointManager_posLLA[2] = rtu_sensor_pos_lla[2];

    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S10>/Bus Assignment2'
     *  Constant: '<S10>/Constant9'
     */
    *rty_busWaypointManager_yawCmd = 0.0;
    break;

   case VOM_TAKEOFF:
    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    *rty_busWaypointManager_yawCmd =
      *rtu_mode_data_autotakeoff_data_takeoff_yaw_ref;
    break;

   case VOM_HOVER:
    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    rty_busWaypointManager_posLLA[0] = *rtu_mode_data_hover_data_hover_x;
    rty_busWaypointManager_posLLA[1] = *rtu_mode_data_hover_data_hover_y;
    rty_busWaypointManager_posLLA[2] = *rtu_mode_data_hover_data_hover_alt_agl;

    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    *rty_busWaypointManager_yawCmd = *rtu_mode_data_hover_data_hover_yaw_ref;
    break;

   case VOM_MANUAL:
    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment2'
     */
    rty_busWaypointManager_posLLA[0] = rtu_sensor_pos_lla[0];
    rty_busWaypointManager_posLLA[1] = rtu_sensor_pos_lla[1];
    rty_busWaypointManager_posLLA[2] = rtu_sensor_pos_lla[2];

    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S7>/Bus Assignment2'
     *  Constant: '<S7>/Constant9'
     */
    *rty_busWaypointManager_yawCmd = 0.0;
    break;

   case VOM_LAND:
    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S2>/Bus Assignment1'
     *  Concatenate: '<S2>/Vector Concatenate'
     */
    rty_busWaypointManager_posLLA[0] = *rtu_mode_data_autoland_data_land_x;
    rty_busWaypointManager_posLLA[1] = *rtu_mode_data_autoland_data_land_y;
    rty_busWaypointManager_posLLA[2] = 0.0;

    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    *rty_busWaypointManager_yawCmd = *rtu_mode_data_autoland_data_land_yaw_ref;
    break;

   case VOM_MR_RTH:
    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    rty_busWaypointManager_posLLA[0] = *rtu_mode_data_rth_data_rth_x;
    rty_busWaypointManager_posLLA[1] = *rtu_mode_data_rth_data_rth_y;
    rty_busWaypointManager_posLLA[2] = *rtu_mode_data_rth_data_rth_alt_agl;

    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    *rty_busWaypointManager_yawCmd = *rtu_mode_data_rth_data_rth_yaw_ref;
    break;

   case VOM_UMAN:
    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment2'
     */
    rty_busWaypointManager_posLLA[0] = rtu_sensor_pos_lla[0];
    rty_busWaypointManager_posLLA[1] = rtu_sensor_pos_lla[1];
    rty_busWaypointManager_posLLA[2] = rtu_sensor_pos_lla[2];

    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S11>/Bus Assignment2'
     *  Constant: '<S11>/Constant9'
     */
    *rty_busWaypointManager_yawCmd = 0.0;
    break;

   case VOM_F_TRANS:
    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    rty_busWaypointManager_posLLA[0] = *rtu_mode_data_ft_data_FT_x;
    rty_busWaypointManager_posLLA[1] = *rtu_mode_data_ft_data_FT_y;
    rty_busWaypointManager_posLLA[2] = *rtu_mode_data_ft_data_FT_Altitude;

    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    *rty_busWaypointManager_yawCmd = *rtu_mode_data_ft_data_FT_Heading;
    break;

   case VOM_B_TRANS:
    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    rty_busWaypointManager_posLLA[0] = *rtu_mode_data_bt_data_BT_Hover_Lat;
    rty_busWaypointManager_posLLA[1] = *rtu_mode_data_bt_data_BT_Hover_Lon;
    rty_busWaypointManager_posLLA[2] = *rtu_mode_data_bt_data_BT_Altitude;

    /* SignalConversion generated from: '<Root>/busWaypointManager' */
    *rty_busWaypointManager_yawCmd = *rtu_mode_data_bt_data_BT_Heading;
    break;

   default:
    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment2'
     */
    rty_busWaypointManager_posLLA[0] = rtu_sensor_pos_lla[0];
    rty_busWaypointManager_posLLA[1] = rtu_sensor_pos_lla[1];
    rty_busWaypointManager_posLLA[2] = rtu_sensor_pos_lla[2];

    /* SignalConversion generated from: '<Root>/busWaypointManager' incorporates:
     *  BusAssignment: '<S1>/Bus Assignment2'
     *  Constant: '<S1>/Constant9'
     */
    *rty_busWaypointManager_yawCmd = 0.0;
    break;
  }

  /* End of MultiPortSwitch: '<Root>/Multiport Switch' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
