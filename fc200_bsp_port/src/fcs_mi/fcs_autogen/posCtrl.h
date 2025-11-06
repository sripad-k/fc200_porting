/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: posCtrl.h
 *
 * Code generated for Simulink model 'posCtrl'.
 *
 * Model version                  : 2.50
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:23 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef posCtrl_h_
#define posCtrl_h_
#ifndef posCtrl_COMMON_INCLUDES_
#define posCtrl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* posCtrl_COMMON_INCLUDES_ */

#include "posCtrl_types.h"
#include <string.h>

extern void posCtrl_Init(void);
extern void posCtrl(const busWaypointManager *rtu_busWptManager, const
                    std_sensor_t *rtu_Sensor, const vom_t *rtu_vom_status,
                    busControllerPosCtrl *rty_controllerPosCtrl, real_T
                    *rty_busControllerIF_vel_velCmdH_x, real_T
                    *rty_busControllerIF_vel_velCmdH_y, real_T
                    *rty_busControllerIF_vel_hRateCmd, real_T
                    *rty_busControllerIF_vel_hCmd, boolean_T
                    *rty_busControllerIF_vel_hHold, boolean_T
                    *rty_busControllerIF_vel_init_reset_vel, boolean_T
                    *rty_busControllerIF_vel_init_reset_hdot);

/* Model reference registration function */
extern void posCtrl_initialize(void);

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
 * '<Root>' : 'posCtrl'
 * '<S1>'   : 'posCtrl/Compare To Constant'
 * '<S2>'   : 'posCtrl/Compare To Constant1'
 * '<S3>'   : 'posCtrl/Compare To Constant2'
 * '<S4>'   : 'posCtrl/Compare To Constant3'
 * '<S5>'   : 'posCtrl/Compare To Constant4'
 * '<S6>'   : 'posCtrl/Compare To Constant5'
 * '<S7>'   : 'posCtrl/dist_from_LLAs'
 * '<S8>'   : 'posCtrl/dist_from_LLAs1'
 * '<S9>'   : 'posCtrl/pos_ctrl'
 * '<S10>'  : 'posCtrl/rot_2D'
 * '<S11>'  : 'posCtrl/dist_from_LLAs/ECEF2NED'
 * '<S12>'  : 'posCtrl/dist_from_LLAs/LLA2ECEF'
 * '<S13>'  : 'posCtrl/dist_from_LLAs/LLA2ECEF1'
 * '<S14>'  : 'posCtrl/dist_from_LLAs/ECEF2NED/MATLAB Function1'
 * '<S15>'  : 'posCtrl/dist_from_LLAs/LLA2ECEF/Subsystem2'
 * '<S16>'  : 'posCtrl/dist_from_LLAs/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S17>'  : 'posCtrl/dist_from_LLAs/LLA2ECEF1/Subsystem2'
 * '<S18>'  : 'posCtrl/dist_from_LLAs/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S19>'  : 'posCtrl/dist_from_LLAs1/ECEF2NED'
 * '<S20>'  : 'posCtrl/dist_from_LLAs1/LLA2ECEF'
 * '<S21>'  : 'posCtrl/dist_from_LLAs1/LLA2ECEF1'
 * '<S22>'  : 'posCtrl/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S23>'  : 'posCtrl/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S24>'  : 'posCtrl/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S25>'  : 'posCtrl/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S26>'  : 'posCtrl/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 * '<S27>'  : 'posCtrl/rot_2D/MATLAB Function'
 */

/*-
 * Requirements for '<Root>': posCtrl

 */
#endif                                 /* posCtrl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
