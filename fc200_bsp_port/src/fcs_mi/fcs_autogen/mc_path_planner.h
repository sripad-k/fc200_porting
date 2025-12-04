/*
 * File: mc_path_planner.h
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

#ifndef mc_path_planner_h_
#define mc_path_planner_h_
#ifndef mc_path_planner_COMMON_INCLUDES_
#define mc_path_planner_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* mc_path_planner_COMMON_INCLUDES_ */

#include "mc_path_planner_types.h"

extern void mc_path_planner(const real_T rtu_sensor_pos_lla[3], const vom_t
  *rtu_vom_status, const real_T *rtu_mode_data_hover_data_hover_y, const real_T *
  rtu_mode_data_hover_data_hover_x, const real_T
  *rtu_mode_data_hover_data_hover_alt_agl, const real_T
  *rtu_mode_data_hover_data_hover_yaw_ref, const real_T
  *rtu_mode_data_autotakeoff_data_takeoff_x, const real_T
  *rtu_mode_data_autotakeoff_data_takeoff_y, const real_T
  *rtu_mode_data_autotakeoff_data_takeoff_alt_agl, const real_T
  *rtu_mode_data_autotakeoff_data_takeoff_yaw_ref, const real_T
  *rtu_mode_data_autoland_data_land_x, const real_T
  *rtu_mode_data_autoland_data_land_y, const real_T
  *rtu_mode_data_autoland_data_land_yaw_ref, const real_T
  *rtu_mode_data_rth_data_rth_x, const real_T *rtu_mode_data_rth_data_rth_y,
  const real_T *rtu_mode_data_rth_data_rth_alt_agl, const real_T
  *rtu_mode_data_rth_data_rth_yaw_ref, const real_T *rtu_mode_data_ft_data_FT_x,
  const real_T *rtu_mode_data_ft_data_FT_y, const real_T
  *rtu_mode_data_ft_data_FT_Altitude, const real_T
  *rtu_mode_data_ft_data_FT_Heading, const real_T
  *rtu_mode_data_bt_data_BT_Hover_Lat, const real_T
  *rtu_mode_data_bt_data_BT_Hover_Lon, const real_T
  *rtu_mode_data_bt_data_BT_Altitude, const real_T
  *rtu_mode_data_bt_data_BT_Heading, real_T rty_busWaypointManager_posLLA[3],
  real_T *rty_busWaypointManager_yawCmd);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'mc_path_planner'
 * '<S1>'   : 'mc_path_planner/ALL FIXED MODES'
 * '<S2>'   : 'mc_path_planner/AUTOLAND'
 * '<S3>'   : 'mc_path_planner/AUTOTAKEOFF'
 * '<S4>'   : 'mc_path_planner/BT'
 * '<S5>'   : 'mc_path_planner/FT'
 * '<S6>'   : 'mc_path_planner/HOVER'
 * '<S7>'   : 'mc_path_planner/MANUEL'
 * '<S8>'   : 'mc_path_planner/READY'
 * '<S9>'   : 'mc_path_planner/RTH'
 * '<S10>'  : 'mc_path_planner/STARTUP'
 * '<S11>'  : 'mc_path_planner/UMAN'
 */

/*-
 * Requirements for '<Root>': mc_path_planner

 */
#endif                                 /* mc_path_planner_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
